#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"fileiolib.h"

// FILEIOLIB Version 0.03

int fileioDescend(FILE* fp,MMCKINFO* mq,MMCKINFO* mp,int flag)
{

	unsigned char buf[8];
	int pos,info,info2,fs,size;

	if(mp==NULL)
	{
		pos=ftell(fp);
		fseek(fp,0,SEEK_END);
		fs=ftell(fp);
		fseek(fp,pos,SEEK_SET);
		pos=0;
	}
	else
	{
		fs=mp->cksize;
		pos=4;
	}

	for(;;)
	{
		fread(buf,8,1,fp);
		size=buf[4]|(buf[5]<<8)|(buf[6]<<16)|(buf[7]<<24);	
		info=buf[3]|(buf[2]<<8)|(buf[1]<<16)|(buf[0]<<24);
		pos+=8;
		
		if(flag==0)
		{
				mq->dwDataOffset=ftell(fp);
				mq->cksize=size;
				return 0;		
		}
		
		/*printf("%c%c%c%c  size=%08d  pos=%08x\n",
			buf[0],buf[1],buf[2],buf[3],size,pos);*/

		if(flag==FILEIO_FINDCHUNK)
		{
			if(info==(int)(mq->ckid))
			{
				mq->dwDataOffset=ftell(fp);
				mq->cksize=size;
				return 0;
			}
			fseek(fp,size,SEEK_CUR);
			pos+=size;
		}
		else
		{
			fread(buf,4,1,fp);
			info2=buf[3]|(buf[2]<<8)|(buf[1]<<16)|(buf[0]<<24);

			if((int)(mq->fccType)==info2)
			{

				if((info==(int)fileioFOURCC('R','I','F','F'))&&(flag==FILEIO_FINDRIFF))
				{
					mq->dwDataOffset=ftell(fp)-4;
					mq->cksize=size;
					return 0;	
				}
				
				if((info==(int)fileioFOURCC('L','I','S','T'))&&(flag==FILEIO_FINDLIST))
				{
					mq->dwDataOffset=ftell(fp)-4;
					mq->cksize=size;
					return 0;	
				}
			}

			fseek(fp,size-4,SEEK_CUR);
			pos+=size;
		}

		/*printf("%d %d\n",pos,fs);*/
		
		if(pos>=fs)return 1;
	}

}


int fileioWriteWaveFormat(FILE* fp,WAVEFORMATEX* wf)
{

	unsigned char buf[20];
	int i;

	buf[0]=(unsigned char)((wf->wFormatTag)&0xff);
	buf[1]=(unsigned char)(((wf->wFormatTag)>>8)&0xff);

	buf[2]=(unsigned char)((wf->nChannels)&0xff);
	buf[3]=(unsigned char)(((wf->nChannels)>>8)&0xff);

	buf[4]=(unsigned char)((wf->nSamplesPerSec)&0xff);
	buf[5]=(unsigned char)(((wf->nSamplesPerSec)>>8)&0xff);
	buf[6]=(unsigned char)(((wf->nSamplesPerSec)>>16)&0xff);
	buf[7]=(unsigned char)(((wf->nSamplesPerSec)>>24)&0xff);

	buf[8]=(unsigned char)((wf->nAvgBytesPerSec)&0xff);
	buf[9]=(unsigned char)(((wf->nAvgBytesPerSec)>>8)&0xff);
	buf[10]=(unsigned char)(((wf->nAvgBytesPerSec)>>16)&0xff);
	buf[11]=(unsigned char)(((wf->nAvgBytesPerSec)>>24)&0xff);

	buf[12]=(unsigned char)((wf->nBlockAlign)&0xff);
	buf[13]=(unsigned char)(((wf->nBlockAlign)>>8)&0xff);

	buf[14]=(unsigned char)((wf->wBitsPerSample)&0xff);
	buf[15]=(unsigned char)(((wf->wBitsPerSample)>>8)&0xff);

	buf[16]=0;
	buf[17]=0;

	i=(int)fwrite(buf,18,1,fp);

	return i;
}

int fileioReadWaveFormat(FILE* fp,WAVEFORMATEX* wf)
{
	unsigned char buf[16];
	int i;

	i=(int)fread(buf,16,1,fp);
	
	memset(wf,0,sizeof(WAVEFORMATEX));
	wf->wFormatTag=buf[0]|(buf[1]<<8);
	wf->nChannels=buf[2]|(buf[3]<<8);
	wf->nSamplesPerSec=buf[4]|(buf[5]<<8)|(buf[6]<<16)|(buf[7]<<24);
	wf->nAvgBytesPerSec=buf[8]|(buf[9]<<8)|(buf[10]<<16)|(buf[11]<<24);
	wf->nBlockAlign=buf[12]|(buf[13]<<8);
	wf->wBitsPerSample=buf[14]|(buf[15]<<8);

	return i;
}

int fileioAscend(FILE* fp,MMCKINFO* mp,int flag)
{
	unsigned char buf[4];
	int pos,i,size;

	if(mp->dwFlags==FILEIO_DIRTY)
	{
		pos=ftell(fp);
		size=pos-(mp->dwDataOffset);

		buf[0]=(unsigned char)(size&0xff);
		buf[1]=(unsigned char)((size>>8)&0xff);
		buf[2]=(unsigned char)((size>>16)&0xff);
		buf[3]=(unsigned char)((size>>24)&0xff);
		/*printf("size=%d  pos=%d\n",size,pos);*/
		fseek(fp,mp->dwDataOffset-4,SEEK_SET);
		i=(int)fwrite(buf,4,1,fp);
		fseek(fp,pos,SEEK_SET);	

		if(i==0)return 1;
		return 0;
	}
	else
	{
		fseek(fp,mp->dwDataOffset+mp->cksize,SEEK_SET);		
	}

	return 0;
}


int fileioCreateChunk(FILE* fp,MMCKINFO* mp,int flag)
{

	unsigned char buf[8];
	fpos_t pos;
	int i; 

	mp->dwFlags=FILEIO_DIRTY;
   
	if(flag!=0)
	{
		if(flag==FILEIO_CREATERIFF)
		{
			buf[0]='R';
			buf[1]='I';
			buf[2]='F';
			buf[3]='F';
		}
		else
		{
			buf[0]='L';
			buf[1]='I';
			buf[2]='S';
			buf[3]='T';		
		}

		buf[4]=buf[5]=buf[6]=buf[7]=0;
		fwrite(buf,8,1,fp);

		pos=ftell(fp);
		mp->dwDataOffset=(int)pos;

		buf[0]=(unsigned char)(((mp->fccType)>>24)&0xff);
		buf[1]=(unsigned char)(((mp->fccType)>>16)&0xff);
		buf[2]=(unsigned char)(((mp->fccType)>>8)&0xff);
		buf[3]=(unsigned char)((mp->fccType)&0xff);
		i=(int)fwrite(buf,4,1,fp);

		if(i==0)return 1;
		else return 0;
  
	}
	else
	{

		buf[0]=(unsigned char)(((mp->ckid)>>24)&0xff);
		buf[1]=(unsigned char)(((mp->ckid)>>16)&0xff);
		buf[2]=(unsigned char)(((mp->ckid)>>8)&0xff);
		buf[3]=(unsigned char)((mp->ckid)&0xff);
		buf[4]=buf[5]=buf[6]=buf[7]=0;
		i=(int)fwrite(buf,8,1,fp);

		pos=ftell(fp);
		mp->dwDataOffset=(int)pos;

		if(i==0)return 1;
		else return 0;
	}
	return 1;
}


unsigned long fileioFOURCC(char a,char b,char c,char d)
{
	return (a<<24)|(b<<16)|(c<<8)|d;
}


/*
main(){
	FILE* fp;
	MMCKINFO mp,mq;
	WAVEFORMATEX wf;
	int i;

	fp=fopen("bbbb.wav","rb");

	memset(&mp,0,sizeof(MMCKINFO));
	mp.fccType=fileioFOURCC('W','A','V','E');
	fileioDescend(fp,&mp,NULL,FILEIO_CREATERIFF);
	i=ftell(fp);
	printf("i=%08x\n",i);
	
	memset(&mq,0,sizeof(MMCKINFO));
	mq.ckid=fileioFOURCC('f','m','t',' ');
	fileioDescend(fp,&mq,&mp,0);
	fileioReadWaveFormat(fp,&wf);

	printf("%d\n",wf.wFormatTag);
    printf("%d\n",wf.nChannels);
    printf("%d\n",wf.nSamplesPerSec);
    printf("%d\n",wf.nAvgBytesPerSec);
    printf("%d\n",wf.nBlockAlign);
    printf("%d\n",wf.wBitsPerSample);
	printf("%d\n",wf.cbSize);

	fileioAscend(fp,&mq,0);

	memset(&mq,0,sizeof(MMCKINFO));
	mq.ckid=fileioFOURCC('d','a','t','a');
	i=fileioDescend(fp,&mq,&mp,0);
	printf("i=%08x\n",i);
	i=ftell(fp);
	printf("i=%08x\n",i);
	fclose(fp);
}
*/

/*
main(){
	FILE* fp;
	MMCKINFO mp,mq;
	WAVEFORMATEX wf;
	char buf[44100];

	memset(&mp,0,sizeof(MMCKINFO));
	mp.fccType=fileioFOURCC('W','A','V','E');
	fp=fopen("bbbb.wav","wb");
	fileioCreateChunk(fp,&mp,FILEIO_CREATERIFF);

	memset(&mq,0,sizeof(MMCKINFO));
	mq.ckid=fileioFOURCC('f','m','t',' ');
	fileioCreateChunk(fp,&mq,0);

	memset(&wf,0,sizeof(WAVEFORMATEX));	
	wf.wFormatTag=WAVE_FORMAT_PCM;
	wf.nChannels=1;
	wf.nSamplesPerSec=11025;
    wf.wBitsPerSample=8;
    wf.nBlockAlign=(wf.wBitsPerSample/8)*wf.nChannels;
	wf.nAvgBytesPerSec=wf.nBlockAlign*wf.nSamplesPerSec;
	fileioWriteWaveFormat(fp,&wf);

	fileioAscend(fp,&mq,0);	

	memset(&mq,0,sizeof(MMCKINFO));
	mq.ckid=fileioFOURCC('d','a','t','a');
	fileioCreateChunk(fp,&mq,0);
	memset(buf,128,44100);
	fwrite(buf,44100,1,fp);

	fileioAscend(fp,&mq,0);	

	fileioAscend(fp,&mp,0);
	fclose(fp);
}
*/