#include "aviiolib.h"
//
// AVILIB Version 0.01
//

int fileioWriteAVIINDEXENTRY(FILE* fp,AVIINDEXENTRY* m)
{

	unsigned char buf[16];

	buf[0]=(unsigned char)(0xff&((m->ckid)>>24));
	buf[1]=(unsigned char)(0xff&((m->ckid)>>16));
	buf[2]=(unsigned char)(0xff&((m->ckid)>>8));
	buf[3]=(unsigned char)(0xff&(m->ckid));

	buf[4]=(unsigned char)(0xff&(m->dwFlags));
	buf[5]=(unsigned char)(0xff&((m->dwFlags)>>8));
	buf[6]=(unsigned char)(0xff&((m->dwFlags)>>16));
	buf[7]=(unsigned char)(0xff&((m->dwFlags)>>24));

	buf[8]=(unsigned char)(0xff&(m->dwChunkOffset));
	buf[9]=(unsigned char)(0xff&((m->dwChunkOffset)>>8));
	buf[10]=(unsigned char)(0xff&((m->dwChunkOffset)>>16));
	buf[11]=(unsigned char)(0xff&((m->dwChunkOffset)>>24));

	buf[12]=(unsigned char)(0xff&(m->dwChunkLength));
	buf[13]=(unsigned char)(0xff&((m->dwChunkLength)>>8));
	buf[14]=(unsigned char)(0xff&((m->dwChunkLength)>>16));
	buf[15]=(unsigned char)(0xff&((m->dwChunkLength)>>24));
	
	return (int)fwrite(buf,16,1,fp);

}


int fileioWriteBITMAPINFOHEADER(FILE* fp,BITMAPINFOHEADER* m)
{
	unsigned char buf[40];

	buf[0]=40;
	buf[1]=0;
	buf[2]=0;
	buf[3]=0;

	buf[4]=(unsigned char)(0xff&(m->biWidth));
	buf[5]=(unsigned char)(0xff&((m->biWidth)>>8));
	buf[6]=(unsigned char)(0xff&((m->biWidth)>>16));
	buf[7]=(unsigned char)(0xff&((m->biWidth)>>24));

	buf[8]=(unsigned char)(0xff&(m->biHeight));
	buf[9]=(unsigned char)(0xff&((m->biHeight)>>8));
	buf[10]=(unsigned char)(0xff&((m->biHeight)>>16));
	buf[11]=(unsigned char)(0xff&((m->biHeight)>>24));

	buf[12]=(unsigned char)(0xff&(m->biPlanes));
	buf[13]=(unsigned char)(0xff&((m->biPlanes)>>8));

	buf[14]=(unsigned char)(0xff&(m->biBitCount));
	buf[15]=(unsigned char)(0xff&((m->biBitCount)>>8));

	buf[16]=(unsigned char)(0xff&(m->biCompression));
	buf[17]=(unsigned char)(0xff&((m->biCompression)>>8));
	buf[18]=(unsigned char)(0xff&((m->biCompression)>>16));
	buf[19]=(unsigned char)(0xff&((m->biCompression)>>24));

	buf[20]=(unsigned char)(0xff&(m->biSizeImage));
	buf[21]=(unsigned char)(0xff&((m->biSizeImage)>>8));
	buf[22]=(unsigned char)(0xff&((m->biSizeImage)>>16));
	buf[23]=(unsigned char)(0xff&((m->biSizeImage)>>24));

	buf[24]=(unsigned char)(0xff&(m->biXPelsPerMeter));
	buf[25]=(unsigned char)(0xff&((m->biXPelsPerMeter)>>8));
	buf[26]=(unsigned char)(0xff&((m->biXPelsPerMeter)>>16));
	buf[27]=(unsigned char)(0xff&((m->biXPelsPerMeter)>>24));

	buf[28]=(unsigned char)(0xff&(m->biYPelsPerMeter));
	buf[29]=(unsigned char)(0xff&((m->biYPelsPerMeter)>>8));
	buf[30]=(unsigned char)(0xff&((m->biYPelsPerMeter)>>16));
	buf[31]=(unsigned char)(0xff&((m->biYPelsPerMeter)>>24));

	buf[32]=(unsigned char)(0xff&(m->biClrUsed));
	buf[33]=(unsigned char)(0xff&((m->biClrUsed)>>8));
	buf[34]=(unsigned char)(0xff&((m->biClrUsed)>>16));
	buf[35]=(unsigned char)(0xff&((m->biClrUsed)>>24));

	buf[36]=(unsigned char)(0xff&(m->biClrImportant));
	buf[37]=(unsigned char)(0xff&((m->biClrImportant)>>8));
	buf[38]=(unsigned char)(0xff&((m->biClrImportant)>>16));
	buf[39]=(unsigned char)(0xff&((m->biClrImportant)>>24));

	return (int)fwrite(buf,40,1,fp);

}


int fileioWriteAVIStreamHeader(FILE* fp,AVIStreamHeader* m)
{
	unsigned char buf[64-8];
	int i;

	buf[0]=(unsigned char)(0xff&((m->fccType)>>24));
	buf[1]=(unsigned char)(0xff&((m->fccType)>>16));
	buf[2]=(unsigned char)(0xff&((m->fccType)>>8));
	buf[3]=(unsigned char)(0xff&(m->fccType));

	buf[7]=(unsigned char)(0xff&(m->fccHandler));
	buf[6]=(unsigned char)(0xff&((m->fccHandler)>>8));
	buf[5]=(unsigned char)(0xff&((m->fccHandler)>>16));
	buf[4]=(unsigned char)(0xff&((m->fccHandler)>>24));

	buf[8]=(unsigned char)(0xff&(m->dwFlags));
	buf[9]=(unsigned char)(0xff&((m->dwFlags)>>8));
	buf[10]=(unsigned char)(0xff&((m->dwFlags)>>16));
	buf[11]=(unsigned char)(0xff&((m->dwFlags)>>24));

	buf[12]=(unsigned char)(0xff&(m->wPriority));
	buf[13]=(unsigned char)(0xff&((m->wPriority)>>8));

	buf[14]=(unsigned char)(0xff&(m->wLanguage));
	buf[15]=(unsigned char)(0xff&((m->wLanguage)>>8));

	buf[16]=(unsigned char)(0xff&(m->dwInitialFrames));
	buf[17]=(unsigned char)(0xff&((m->dwInitialFrames)>>8));
	buf[18]=(unsigned char)(0xff&((m->dwInitialFrames)>>16));
	buf[19]=(unsigned char)(0xff&((m->dwInitialFrames)>>24));
	
	buf[20]=(unsigned char)(0xff&(m->dwScale));
	buf[21]=(unsigned char)(0xff&((m->dwScale)>>8));
	buf[22]=(unsigned char)(0xff&((m->dwScale)>>16));
	buf[23]=(unsigned char)(0xff&((m->dwScale)>>24));

	buf[24]=(unsigned char)(0xff&(m->dwRate));
	buf[25]=(unsigned char)(0xff&((m->dwRate)>>8));
	buf[26]=(unsigned char)(0xff&((m->dwRate)>>16));
	buf[27]=(unsigned char)(0xff&((m->dwRate)>>24));

	buf[28]=(unsigned char)(0xff&(m->dwStart));
	buf[29]=(unsigned char)(0xff&((m->dwStart)>>8));
	buf[30]=(unsigned char)(0xff&((m->dwStart)>>16));
	buf[31]=(unsigned char)(0xff&((m->dwStart)>>24));

	buf[32]=(unsigned char)(0xff&(m->dwLength));
	buf[33]=(unsigned char)(0xff&((m->dwLength)>>8));
	buf[34]=(unsigned char)(0xff&((m->dwLength)>>16));
	buf[35]=(unsigned char)(0xff&((m->dwLength)>>24));

	buf[36]=(unsigned char)(0xff&(m->dwSuggestedBufferSize));
	buf[37]=(unsigned char)(0xff&((m->dwSuggestedBufferSize)>>8));
	buf[38]=(unsigned char)(0xff&((m->dwSuggestedBufferSize)>>16));
	buf[39]=(unsigned char)(0xff&((m->dwSuggestedBufferSize)>>24));

	buf[40]=(unsigned char)(0xff&(m->dwQuality));
	buf[41]=(unsigned char)(0xff&((m->dwQuality)>>8));
	buf[42]=(unsigned char)(0xff&((m->dwQuality)>>16));
	buf[43]=(unsigned char)(0xff&((m->dwQuality)>>24));

	buf[44]=(unsigned char)(0xff&(m->dwSampleSize));
	buf[45]=(unsigned char)(0xff&((m->dwSampleSize)>>8));
	buf[46]=(unsigned char)(0xff&((m->dwSampleSize)>>16));
	buf[47]=(unsigned char)(0xff&((m->dwSampleSize)>>24));

	buf[48]=(unsigned char)(0xff&(m->rcFrame.left));
	buf[49]=(unsigned char)(0xff&((m->rcFrame.left)>>8));
	
	buf[50]=(unsigned char)(0xff&(m->rcFrame.top));
	buf[51]=(unsigned char)(0xff&((m->rcFrame.top)>>8));

	buf[52]=(unsigned char)(0xff&(m->rcFrame.right));
	buf[53]=(unsigned char)(0xff&((m->rcFrame.right)>>8));

	buf[54]=(unsigned char)(0xff&(m->rcFrame.bottom));
	buf[55]=(unsigned char)(0xff&((m->rcFrame.bottom)>>8));

	//for(i=48;i<64-8;i++)buf[i]=0;

	return (int)fwrite(buf,64-8,1,fp);
}


int fileioWriteMainAVIHeader(FILE* fp,MainAVIHeader* m)
{
	unsigned char buf[56];
	int i;

	buf[0]=(unsigned char)(0xff&(m->dwMicroSecPerFrame));
	buf[1]=(unsigned char)(0xff&((m->dwMicroSecPerFrame)>>8));
	buf[2]=(unsigned char)(0xff&((m->dwMicroSecPerFrame)>>16));
	buf[3]=(unsigned char)(0xff&((m->dwMicroSecPerFrame)>>24));

	buf[4]=(unsigned char)(0xff&(m->dwMaxBytesPerSec));
	buf[5]=(unsigned char)(0xff&((m->dwMaxBytesPerSec)>>8));
	buf[6]=(unsigned char)(0xff&((m->dwMaxBytesPerSec)>>16));
	buf[7]=(unsigned char)(0xff&((m->dwMaxBytesPerSec)>>24));

	buf[8]=(unsigned char)(0xff&(m->dwPaddingGranularity));
	buf[9]=(unsigned char)(0xff&((m->dwPaddingGranularity)>>8));
	buf[10]=(unsigned char)(0xff&((m->dwPaddingGranularity)>>16));
	buf[11]=(unsigned char)(0xff&((m->dwPaddingGranularity)>>24));

	
	buf[12]=(unsigned char)(0xff&(m->dwFlags));
	buf[13]=(unsigned char)(0xff&((m->dwFlags)>>8));
	buf[14]=(unsigned char)(0xff&((m->dwFlags)>>16));
	buf[15]=(unsigned char)(0xff&((m->dwFlags)>>24));
		
	buf[16]=(unsigned char)(0xff&(m->dwTotalFrames));
	buf[17]=(unsigned char)(0xff&((m->dwTotalFrames)>>8));
	buf[18]=(unsigned char)(0xff&((m->dwTotalFrames)>>16));
	buf[19]=(unsigned char)(0xff&((m->dwTotalFrames)>>24));

		
	buf[20]=(unsigned char)(0xff&(m->dwInitialFrames));
	buf[21]=(unsigned char)(0xff&((m->dwInitialFrames)>>8));
	buf[22]=(unsigned char)(0xff&((m->dwInitialFrames)>>16));
	buf[23]=(unsigned char)(0xff&((m->dwInitialFrames)>>24));

		
	buf[24]=(unsigned char)(0xff&(m->dwStreams));
	buf[25]=(unsigned char)(0xff&((m->dwStreams)>>8));
	buf[26]=(unsigned char)(0xff&((m->dwStreams)>>16));
	buf[27]=(unsigned char)(0xff&((m->dwStreams)>>24));

	buf[28]=(unsigned char)(0xff&(m->dwSuggestedBufferSize));
	buf[29]=(unsigned char)(0xff&((m->dwSuggestedBufferSize)>>8));
	buf[30]=(unsigned char)(0xff&((m->dwSuggestedBufferSize)>>16));
	buf[31]=(unsigned char)(0xff&((m->dwSuggestedBufferSize)>>24));


	buf[32]=(unsigned char)(0xff&(m->dwWidth));
	buf[33]=(unsigned char)(0xff&((m->dwWidth)>>8));
	buf[34]=(unsigned char)(0xff&((m->dwWidth)>>16));
	buf[35]=(unsigned char)(0xff&((m->dwWidth)>>24));

	buf[36]=(unsigned char)(0xff&(m->dwHeight));
	buf[37]=(unsigned char)(0xff&((m->dwHeight)>>8));
	buf[38]=(unsigned char)(0xff&((m->dwHeight)>>16));
	buf[39]=(unsigned char)(0xff&((m->dwHeight)>>24));

	for(i=40;i<56;i++)buf[i]=0;

	return (int)fwrite(buf,56,1,fp);
}


int fileioWriteLong(FILE*fp,int m)
{
	unsigned char buf[4];
	buf[0]=(unsigned char)(0xff&m);
	buf[1]=(unsigned char)(0xff&(m>>8));
	buf[2]=(unsigned char)(0xff&(m>>16));
	buf[3]=(unsigned char)(0xff&(m>>24));
	return (int)fwrite(buf,4,1,fp);
}
