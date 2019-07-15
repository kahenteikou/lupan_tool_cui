#ifndef AVIIOLIB_30
#define AVIIOLIB_30

// AVILIB Version 0.03

#include"fileiolib.h"

#define AVIFILEINFO_HASINDEX		0x00000010
#define ALIGNLONG(i) (((i)+3)/4*4)
#define BI_RGB        0L
#define AVIIF_KEYFRAME      0x00000010L // this frame is a key frame.



#ifdef __cplusplus
extern "C"
{
#endif


typedef struct
{
    unsigned long		ckid;
    long		dwFlags;
    long		dwChunkOffset;		// Position of chunk
    long		dwChunkLength;		// Length of chunk
} AVIINDEXENTRY;

typedef struct
{
    long		dwMicroSecPerFrame;	// frame display rate (or 0L)
    long		dwMaxBytesPerSec;	// max. transfer rate
    long		dwPaddingGranularity;	// pad to multiples of this
                                                // size; normally 2K.
    long		dwFlags;		// the ever-present flags
    long		dwTotalFrames;		// # frames in file
    long		dwInitialFrames;
    long		dwStreams;
    long		dwSuggestedBufferSize;
    
    long		dwWidth;
    long		dwHeight;
    
    long		dwReserved[4];
} MainAVIHeader;

typedef struct tagRECT
{
    unsigned short    left;
    unsigned short    top;
    unsigned short    right;
    unsigned short   bottom;
} RECT;

typedef struct 
{
    unsigned long		fccType;
    unsigned long		fccHandler;
    long		dwFlags;	/* Contains AVITF_* flags */
    short		wPriority;
    short		wLanguage;
    long		dwInitialFrames;
    long		dwScale;	
    long		dwRate;	/* dwRate / dwScale == samples/second */
    long		dwStart;
    long		dwLength; /* In units above... */
    long		dwSuggestedBufferSize;
    long		dwQuality;
    long		dwSampleSize;
    RECT		rcFrame;
} AVIStreamHeader;

typedef struct tagBITMAPINFOHEADER
{
        long      biSize;
        long       biWidth;
        long       biHeight;
        short       biPlanes;
        short       biBitCount;
        long      biCompression;
        long      biSizeImage;
        long       biXPelsPerMeter;
        long       biYPelsPerMeter;
        long      biClrUsed;
        long      biClrImportant;
} BITMAPINFOHEADER;



int fileioWriteAVIINDEXENTRY(FILE* fp,AVIINDEXENTRY* m);
int fileioWriteBITMAPINFOHEADER(FILE* fp,BITMAPINFOHEADER* m);
int fileioWriteAVIStreamHeader(FILE* fp,AVIStreamHeader* m);
int fileioWriteMainAVIHeader(FILE* fp,MainAVIHeader* m);
int fileioWriteLong(FILE*fp,int m);


#ifdef __cplusplus
}
#endif


#endif
