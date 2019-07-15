#ifndef _LUPIN_FONT_H_
#define _LUPIN_FONT_H_

#ifdef __cplusplus
extern "C"
{
#endif

int init_freetype(void);
void done_freetype(void);
unsigned char *create_bmp(int x,int y);
void puset_bmp(unsigned  char*  kekka,int x,int y,int c);
void save_bmp(unsigned  char*  kekka,char* name);
void free_bmp(unsigned  char*  kekka);
unsigned char* offset_bmp(unsigned char* bmp);
unsigned char* make_bmp(int bmp_x,int bmp_y,int bmp_space_x,int bmp_space_y,unsigned short* mozi);

#ifdef __cplusplus
}
#endif


#endif /* _LUPIN_FONT_H_ */

