#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lupinfont.h"
#include "kanji.h"
#include "fileiolib.h"
#include "aviiolib.h"

#define LUPIN_WAVE_1	"lupin5.wav"
#define LUPIN_WAVE_2	"lupin6.wav"

#define LUPIN_BIT		8
#define LUPIN_LATE		5

#define WAVE_FORMAT_PCM 1

struct bmconfig{
	int w;
	int h;
	int space_x;
	int space_y;
	char name[1024];
	unsigned short mozi[1024];
};

#define I_USE_SOKKURI

void kaiseki(struct bmconfig* pbm,int argc,char* argv[])
{
	int i;

	if(pbm==NULL)return;
	pbm->w=1920;
	pbm->h=1080;
	pbm->space_x=5;
	pbm->space_y=5;
	strcpy(pbm->name,"a.avi");
	pbm->mozi[0]='S';
	pbm->mozi[1]='A';
	pbm->mozi[2]='M';
	pbm->mozi[3]='P';
	pbm->mozi[4]='L';
	pbm->mozi[5]='E';
	pbm->mozi[6]=0;


	for(i=1;i<argc;i++)
	{
		/*printf("argv[%d]=>>%s<<\n",i,argv[i]);*/
		if(((strcmp(argv[i],"-s")==0) || (strcmp(argv[i],"-S")==0))&& i+1<argc)
		{
			sscanf(argv[i+1],"%dx%d",&(pbm->w),&(pbm->h));
			i++;
		}
		if(((strcmp(argv[i],"-x")==0) || (strcmp(argv[i],"-X")==0))&& i+1<argc)
		{
			sscanf(argv[i+1],"%d",&(pbm->space_x));
			i++;
		}
		if(((strcmp(argv[i],"-y")==0) || (strcmp(argv[i],"-Y")==0))&& i+1<argc)
		{
			sscanf(argv[i+1],"%d",&(pbm->space_y));
			i++;
		}
		if(((strcmp(argv[i],"-o")==0) || (strcmp(argv[i],"-O")==0))&& i+1<argc)
		{
			sscanf(argv[i+1],"%s",&(pbm->name[0]));
			i++;
		}
		if(((strcmp(argv[i],"-m")==0) || (strcmp(argv[i],"-M")==0))&& i+1<argc)
		{
			char tmp[1024];
			tmp[0]=0;
			sscanf(argv[i+1],"%s",tmp);
#ifdef unix
			kj_euc_to_unicode(tmp,&(pbm->mozi[0]));
#else
			kj_sjis_to_unicode(tmp,&(pbm->mozi[0]));
#endif
			i++;
		}
	}
	pbm->space_x=pbm->w*pbm->space_x/100;
	pbm->space_y=pbm->h*pbm->space_y/100;
}


/*
** WAVファイルのオープン
*/

char* waveLoadLupin1(){

	FILE* fp;
	MMCKINFO mp,mq;
	int i;

	char* mem;

	fp=fopen(LUPIN_WAVE_1,"rb");
	if(fp==NULL)return NULL;

	memset(&mp,0,sizeof(MMCKINFO));
	mp.fccType=fileioFOURCC('W','A','V','E');
	fileioDescend(fp,&mp,NULL,FILEIO_FINDRIFF);
	
	memset(&mq,0,sizeof(MMCKINFO));
	mq.ckid=fileioFOURCC('d','a','t','a');
	i=fileioDescend(fp,&mq,&mp,FILEIO_FINDCHUNK);

	mem=(char*)malloc(11025*2/5);
	fread(mem,11025*2/5,1,fp);
	fclose(fp);
	return mem;
}

char* waveLoadLupin2(){
	FILE* fp;
	MMCKINFO mp,mq;
	int i;

	char* mem;

	fp=fopen(LUPIN_WAVE_2,"rb");
	if(fp==NULL)return NULL;

	memset(&mp,0,sizeof(MMCKINFO));
	mp.fccType=fileioFOURCC('W','A','V','E');
	fileioDescend(fp,&mp,NULL,FILEIO_FINDRIFF);
	
	memset(&mq,0,sizeof(MMCKINFO));
	mq.ckid=fileioFOURCC('d','a','t','a');
	i=fileioDescend(fp,&mq,&mp,FILEIO_FINDCHUNK);

	mem=(char*)malloc(11025*2*4);
	
	fread(mem,11025*2*4,1,fp);

	fclose(fp);
	return mem;
}




/*
** メイン関数
*/

#if 0
main(int argc,char* argv[])
{
	struct bmconfig bm;
	int mojisu;
	unsigned char* bmp;


	if(init_freetype()==0)
	{
		printf("Error:FontFile or Fontlibrary not found\n");
		return 1;
	}

	kaiseki(&bm,argc,argv);

	mojisu=0;
	while(bm.mozi[mojisu])mojisu++;

	if(mojisu<=0 || mojisu>=100)
	{
		printf("Error:Mojisu is not correct\n");
		done_freetype();
		return 1;
	}

	bmp=make_bmp(bm.w,bm.h,bm.space_x,bm.space_y,bm.mozi);

	if(bmp==NULL)
	{
		printf("Error:make_bmp error\n");
		done_freetype();
		return 1;
	}

	save_bmp(bmp,bm.name);
	free_bmp(bmp);
	done_freetype();
	
	return 0;
}
#endif




static 	int list[300];
static 	int list2[300];

main(int argc,char* argv[])
{
	struct bmconfig bm;
	int mojisu;
	unsigned char* bmp=NULL;

	char *mem_lupin1=NULL;
	char *mem_lupin2=NULL;
	char *mem_sound=NULL;

	FILE* fp=NULL;

	MMCKINFO mp,mq,mr,ms;
	MainAVIHeader mavih;
	BITMAPINFOHEADER bi;
	AVIStreamHeader ash;
	AVIINDEXENTRY aie;
	WAVEFORMATEX wf;
	int frame,i,ct,sndSamples,sz,pos,sl;

	if(init_freetype()==0)
	{
		printf("Error:FontFile or Fontlibrary not found\n");
		return 1;
	}

	kaiseki(&bm,argc,argv);

	mojisu=0;
	while(bm.mozi[mojisu])mojisu++;

	if(mojisu<=0 || mojisu>=100)
	{
		printf("Error:Mojisu is not correct\n");
		done_freetype();
		return 1;
	}

	frame=mojisu+LUPIN_LATE*4+LUPIN_LATE*2;

	mem_lupin1=waveLoadLupin1();
	mem_lupin2=waveLoadLupin2();

	sndSamples=11025*1*frame/LUPIN_LATE;
	mem_sound=(char*)malloc(sndSamples*2);
	if(mem_sound)memset(mem_sound,0,sndSamples*2);




	if(mem_lupin1==NULL || mem_lupin2==NULL || mem_sound==NULL)
	{
		printf("Error:wave file cannot open\n");
		if(mem_lupin1)free(mem_lupin1);
		if(mem_lupin2)free(mem_lupin2);
		if(mem_lupin2)free(mem_sound);
		done_freetype();
		return 1;
	}

	fp=fopen(bm.name,"wb");
	if(fp==NULL)
	{
		printf("Error:output file cannot open\n");
		if(mem_lupin1)free(mem_lupin1);
		if(mem_lupin2)free(mem_lupin2);
		done_freetype();
		return 1;
	}


	memset(&mp,0,sizeof(MMCKINFO));
	mp.fccType=fileioFOURCC('A','V','I',' ');
	fileioCreateChunk(fp,&mp,FILEIO_CREATERIFF);

		/*hdrlチャンク作成*/
		memset(&mq,0,sizeof(MMCKINFO));
		mq.fccType=fileioFOURCC('h','d','r','l');
		fileioCreateChunk(fp,&mq,FILEIO_CREATELIST);

			/*avihチャンク作成*/
			memset(&mr,0,sizeof(MMCKINFO));
			mr.ckid=fileioFOURCC('a','v','i','h');
			fileioCreateChunk(fp,&mr,0);

			/*avih書き込み*/
			memset(&mavih,0,sizeof(MainAVIHeader));

			mavih.dwWidth=bm.w;  
			mavih.dwHeight=bm.h;
			mavih.dwStreams=2;

			//mavih.dwSuggestedBufferSize=ALIGNLONG((bm.w*LUPIN_BIT+7)/8)*bm.h*LUPIN_LATE;
			mavih.dwTotalFrames=frame;
#if 0
			mavih.dwFlags=AVIFILEINFO_HASINDEX | 0x800;
#else
			mavih.dwFlags=AVIFILEINFO_HASINDEX;
#endif
			//mavih.dwMaxBytesPerSec=ALIGNLONG((bm.w*LUPIN_BIT+7)/8)*bm.h*LUPIN_LATE;
			mavih.dwMicroSecPerFrame=1000000/LUPIN_LATE;
			fileioWriteMainAVIHeader(fp,&mavih);

			/*avihチャンク終了*/
			fileioAscend(fp,&mr,0);


			/*strlチャンク作成*/
			memset(&mr,0,sizeof(MMCKINFO));
			mr.fccType=fileioFOURCC('s','t','r','l');
			fileioCreateChunk(fp,&mr,FILEIO_CREATELIST);
	
				/*strhチャンク作成*/
				memset(&ms,0,sizeof(MMCKINFO));
				ms.ckid=fileioFOURCC('s','t','r','h');
				fileioCreateChunk(fp,&ms,0);

				memset(&ash,0,sizeof(AVIStreamHeader));
				ash.dwSuggestedBufferSize=ALIGNLONG((bm.w*LUPIN_BIT+7)/8)*bm.h;
				ash.dwLength=frame;
				ash.dwRate=LUPIN_LATE*100;
				ash.dwScale=100;
				ash.fccType=fileioFOURCC('v','i','d','s');
				ash.fccHandler=fileioFOURCC('D','I','B',' ');
#ifdef I_USE_SOKKURI
				ash.rcFrame.right=640;
				ash.rcFrame.bottom=480;
#endif
				fileioWriteAVIStreamHeader(fp,&ash);

				/*strhチャンク終了*/
				fileioAscend(fp,&ms,0);

				/*strfチャンク作成*/
				memset(&ms,0,sizeof(MMCKINFO));
				ms.ckid=fileioFOURCC('s','t','r','f');
				fileioCreateChunk(fp,&ms,0);
				
				memset(&bi,0,sizeof(BITMAPINFOHEADER));
				bi.biSize=sizeof(BITMAPINFOHEADER);
				bi.biWidth=bm.w;
				bi.biHeight=bm.h;
				bi.biPlanes=1;
				bi.biBitCount=LUPIN_BIT;
				bi.biCompression=BI_RGB;
				bi.biSizeImage=ALIGNLONG((bm.w*LUPIN_BIT+7)/8)*bm.h;
				bi.biClrUsed=256;
				fileioWriteBITMAPINFOHEADER(fp,&bi);

#if 1
				ct=(1<<LUPIN_BIT)/2;
				for(i=0;i<ct;i++){
					fileioWriteLong(fp,0);
					fileioWriteLong(fp,0xffffff);
				}
#endif
				/*strfチャンク終了*/
				fileioAscend(fp,&ms,0);

			/*strlチャンク終了*/
			fileioAscend(fp,&mr,0);



			/*strlチャンク作成*/
			memset(&mr,0,sizeof(MMCKINFO));
			mr.fccType=fileioFOURCC('s','t','r','l');
			fileioCreateChunk(fp,&mr,FILEIO_CREATELIST);

				memset(&wf,0,sizeof(WAVEFORMATEX));
				wf.wFormatTag=WAVE_FORMAT_PCM;
				wf.nChannels=1;
				wf.nSamplesPerSec=11025;
			    wf.wBitsPerSample=16;
				wf.nBlockAlign=wf.wBitsPerSample/8*wf.nChannels;
				wf.nAvgBytesPerSec=wf.nBlockAlign*wf.nSamplesPerSec;
				//sndSamples=wf.nSamplesPerSec*frame/LUPIN_LATE;
				//mem_sound=(char*)malloc(sndSamples);
				//memset(mem_sound,0x80,sndSamples);

				for(i=0;i<frame;i++)
				{
					if((i>=LUPIN_LATE) && (i-LUPIN_LATE<mojisu))
					{
						memcpy(mem_sound+(11025*2/5)*i,mem_lupin1,11025*2/5);
					}
				}
				memcpy(mem_sound+sndSamples*2-11025*5*2,mem_lupin2,11025*4*2);
				
				/*strhチャンク作成*/
				memset(&ms,0,sizeof(MMCKINFO));
				ms.ckid=fileioFOURCC('s','t','r','h');
				fileioCreateChunk(fp,&ms,0);

				memset(&ash,0,sizeof(AVIStreamHeader));
				ash.fccType=fileioFOURCC('a','u','d','s');

#ifdef I_USE_SOKKURI
				ash.fccHandler=fileioFOURCC('\x01','\0','\0','\0');
#endif

				ash.dwRate=wf.nSamplesPerSec*wf.nBlockAlign;
				ash.dwScale=wf.nBlockAlign;
				ash.dwSuggestedBufferSize=wf.nSamplesPerSec*wf.nBlockAlign/LUPIN_LATE;
				ash.dwLength=sndSamples;
				//ash.dwQuality=10000;
				ash.dwSampleSize=wf.nBlockAlign;
				fileioWriteAVIStreamHeader(fp,&ash);

				/*strhチャンク終了*/
				fileioAscend(fp,&ms,0);

				/*strfチャンク作成*/
				memset(&ms,0,sizeof(MMCKINFO));
				ms.ckid=fileioFOURCC('s','t','r','f');
				fileioCreateChunk(fp,&ms,0);

				fileioWriteWaveFormat(fp,&wf);
				fileioAscend(fp,&ms,0);


			/*strlチャンク終了*/
			fileioAscend(fp,&mr,0);

		/*hdrlチャンク終了*/
		fileioAscend(fp,&mq,0);

		/*JUNKチャンク作成*/
		memset(&mq,0,sizeof(MMCKINFO));
		mq.ckid=fileioFOURCC('J','U','N','K');
		fileioCreateChunk(fp,&mq,0);

		sz=4096-12-ftell(fp);
		for(i=0;i<sz;i++)fwrite("",1,1,fp);

		/*JUNKチャンク終了*/
		fileioAscend(fp,&mq,0);
		

		/*movlチャンク作成*/
		memset(&mq,0,sizeof(MMCKINFO));
		mq.fccType=fileioFOURCC('m','o','v','i');
		fileioCreateChunk(fp,&mq,FILEIO_CREATELIST);
		//pos=4+mmioTell(fp)-4;
		//sl=4+mmioTell(fp)-pos;
		pos=ftell(fp)-4;
	
		//sl=ftell(fp)-pos;


#if 1

		/* オーディオを分割して出力しないとffmpegが正しく判別できない？ */
		for(i=0;i<frame;i++)
		{
			memset(&mr,0,sizeof(MMCKINFO));
			mr.ckid=fileioFOURCC('0','1','w','b');
			fileioCreateChunk(fp,&mr,0);
			list2[i]=ftell(fp)-pos-8;
			fwrite(mem_sound+wf.nAvgBytesPerSec*i/LUPIN_LATE,wf.nAvgBytesPerSec/LUPIN_LATE,1,fp);
			fileioAscend(fp,&mr,0);

			memset(&mr,0,sizeof(MMCKINFO));
			mr.ckid=fileioFOURCC('0','0','d','b');
			fileioCreateChunk(fp,&mr,0);
			list[i]=ftell(fp)-pos-8;

			if(i<LUPIN_LATE)
			{
				int msize;
				msize=ALIGNLONG((bm.w*LUPIN_BIT+7)/8)*bm.h+2048;
				bmp=malloc(msize);
				if(bmp)memset(bmp,0,msize);
			}
			else if(i-LUPIN_LATE<mojisu)
			{
				unsigned short mmoji[2];
				mmoji[0]=bm.mozi[i-LUPIN_LATE];
				mmoji[1]=0;
				bmp=make_bmp(bm.w,bm.h,bm.space_x,bm.space_y,mmoji);
			}
			else if(i<LUPIN_LATE*5+mojisu)
			{
				bmp=make_bmp(bm.w,bm.h,bm.space_x,bm.space_y,bm.mozi);
			}
			else
			{
				int msize;
				msize=ALIGNLONG((bm.w*LUPIN_BIT+7)/8)*bm.h+2048;
				bmp=malloc(msize);
				if(bmp)memset(bmp,0,msize);
			}

			if(bmp)
			{
				fwrite(offset_bmp(bmp),ALIGNLONG((bm.w*LUPIN_BIT+7)/8)*bm.h,1,fp);
				free(bmp);
			}
			bmp=NULL;
			fileioAscend(fp,&mr,0);
		}
#else

		for(i=0;i<frame;i++)
		{
			memset(&mr,0,sizeof(MMCKINFO));
			mr.ckid=fileioFOURCC('0','1','w','b');
			fileioCreateChunk(fp,&mr,0);
			list2[i]=ftell(fp)-pos-8;
			fwrite(mem_sound+wf.nAvgBytesPerSec*i/LUPIN_LATE,wf.nAvgBytesPerSec/LUPIN_LATE,1,fp);
			fileioAscend(fp,&mr,0);
		}



		for(i=0;i<frame;i++)
		{
			memset(&mr,0,sizeof(MMCKINFO));
			mr.ckid=fileioFOURCC('0','0','d','b');
			fileioCreateChunk(fp,&mr,0);
			list[i]=ftell(fp)-pos-8;

			if(i<LUPIN_LATE)
			{
				int msize;
				msize=ALIGNLONG((bm.w*LUPIN_BIT+7)/8)*bm.h+2048;
				bmp=malloc(msize);
				if(bmp)memset(bmp,0,msize);
			}
			else if(i-LUPIN_LATE<mojisu)
			{
				unsigned short mmoji[2];
				mmoji[0]=bm.mozi[i-LUPIN_LATE];
				mmoji[1]=0;
				bmp=make_bmp(bm.w,bm.h,bm.space_x,bm.space_y,mmoji);
			}
			else if(i<LUPIN_LATE*5+mojisu)
			{
				bmp=make_bmp(bm.w,bm.h,bm.space_x,bm.space_y,bm.mozi);
			}
			else
			{
				int msize;
				msize=ALIGNLONG((bm.w*LUPIN_BIT+7)/8)*bm.h+2048;
				bmp=malloc(msize);
				if(bmp)memset(bmp,0,msize);
			}

			if(bmp)
			{
				fwrite(offset_bmp(bmp),ALIGNLONG((bm.w*LUPIN_BIT+7)/8)*bm.h,1,fp);
				free(bmp);
			}
			bmp=NULL;
			fileioAscend(fp,&mr,0);
		}


#endif
		/*movlチャンク終了*/
		fileioAscend(fp,&mq,0);

		/*idx1チャンク作成*/
		memset(&mq,0,sizeof(MMCKINFO));
		mq.ckid=fileioFOURCC('i','d','x','1');
		fileioCreateChunk(fp,&mq,0);

		for(i=0;i<frame;i++)
		{
			memset(&aie,0,sizeof(AVIINDEXENTRY));
			aie.ckid=fileioFOURCC('0','1','w','b');
			aie.dwChunkOffset=list2[i];
			aie.dwChunkLength=wf.nAvgBytesPerSec/LUPIN_LATE;
			fileioWriteAVIINDEXENTRY(fp,&aie);

			memset(&aie,0,sizeof(AVIINDEXENTRY));
			aie.ckid=fileioFOURCC('0','0','d','b');
			aie.dwFlags=AVIIF_KEYFRAME;
			aie.dwChunkOffset=list[i];
			aie.dwChunkLength=ALIGNLONG((bm.w*LUPIN_BIT+7)/8)*bm.h;
			fileioWriteAVIINDEXENTRY(fp,&aie);
		}

		/*idx1チャンク終了*/
		fileioAscend(fp,&mq,0);

	/*AVIチャンク終了*/
	fileioAscend(fp,&mp,0);
	fclose(fp);


	if(bmp)free_bmp(bmp);
	if(mem_lupin1)free(mem_lupin1);
	if(mem_lupin2)free(mem_lupin2);
	if(mem_sound)free(mem_sound);
	done_freetype();
	
	return 0;
}



