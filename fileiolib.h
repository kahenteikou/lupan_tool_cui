#ifndef FILEIOLIB_30
#define FILEIOLIB_30


#include <stdio.h>


#define FILEIO_FINDCHUNK  0x010
#define FILEIO_CREATERIFF 0x20
#define FILEIO_CREATELIST 0x40
#define FILEIO_FINDRIFF 0x20
#define FILEIO_FINDLIST 0x40
#define FILEIO_DIRTY      0x10000000      /* I/O buffer is dirty */


#ifdef __cplusplus
extern "C"
{
#endif


/*�@MS�݊��\���́@MMCKINFO�@*/
typedef struct fileioMMCKINFO
{
	unsigned long ckid;
	int cksize;
	unsigned long fccType;
	int dwDataOffset;
	unsigned int dwFlags;
} MMCKINFO;

/*�@MS�݊��\���́@WAVEFORMATEX�@*/
typedef struct tWAVEFORMATEX
{
    short        wFormatTag;         /* format type */
    short        nChannels;          /* number of channels (i.e. mono, stereo...) */
    int       nSamplesPerSec;     /* sample rate */
    int       nAvgBytesPerSec;    /* for buffer estimation */
    short        nBlockAlign;        /* block size of data */
    short        wBitsPerSample;     /* number of bits per sample of mono data */
    int        cbSize;             /* the count in bytes of the size of */
} WAVEFORMATEX;



/*�@MS�݊��}���e�B���f�B�A�t�@�C�����o�͊֐��@*/
	unsigned long fileioFOURCC(char a,char b,char c,char d);
	int fileioCreateChunk(FILE*,MMCKINFO* mp,int flag);
	int fileioAscend(FILE* fp,MMCKINFO* mp,int flag);
	int fileioDescend(FILE* fp,MMCKINFO* mq,MMCKINFO* mp,int flag);

/*�@�������}���e�B���f�B�A�t�@�C�����o�͊֐��@*/
	int fileioWriteWaveFormat(FILE* fp,WAVEFORMATEX* wf);
	int fileioReadWaveFormat(FILE* fp,WAVEFORMATEX* wf);




#ifdef __cplusplus
}
#endif


#endif
