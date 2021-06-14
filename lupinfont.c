#include <stdio.h>
#include <string.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "lupinfont.h"


/* 
** FrerType2110のライブラリをWin32でビルドするときは
** In include/freetype/config/ftmodule.h 
** の以下の行をコメントアウトしてください。
** FT_USE_MODULE(otv_module_class) 
** そうしないとリンク時にエラーになります。
*/


/*
** シングルスレッドのライブラリを使います。
*/
#ifdef _WIN64
#pragma comment(lib,"freetype_x64_avx.lib")
#endif

#define FONTNAME "msmincho.ttc"


/*
** TrueTypeとフォントのハンドル
*/

static 	FT_Library       font_library;
static 	FT_Face          font_face;


/*
** フォントとライブラリの初期化・終了
*/

int init_freetype(void)
{

	/* Initialize engine */
	if(FT_Init_FreeType( &font_library ))
	{
		//printf("Error:FT_Init_FreeType\n");
		return 0;
	}
	

	if ( FT_New_Face( font_library, FONTNAME, 0 , &font_face ) )
	{
		//printf("Error:FT_New_Face\n");
		return 0;
	}

	return 1;
}


void done_freetype(void)
{
     FT_Done_Face( font_face );
	 FT_Done_FreeType(font_library);
}




/*
** ビットマップ操作系
*/

unsigned char *create_bmp(int x,int y)
{
	int sz,of;
	unsigned char *kekka;
	sz=((x+3)/4)*4*y+54+256*4;
	kekka=malloc(sz);
	if(kekka==NULL)return NULL;
	memset(kekka,0,sz);

	kekka[0]='B';kekka[1]='M';
	kekka[2]=sz;kekka[3]=sz>>8;
	kekka[4]=sz>>16;kekka[5]=sz>>24;
	of=54+256*4;
	kekka[10]=of;kekka[11]=of>>8;
	kekka[14]=0x28;
	kekka[18]=x;kekka[19]=x>>8;
	kekka[22]=y;kekka[23]=y>>8;

	kekka[26]=1;kekka[28]=8;

	kekka[54+4+0]=0xff;
	kekka[54+4+1]=0xff;
	kekka[54+4+2]=0xff;

	return kekka;
}


void puset_bmp(unsigned  char*  kekka,int x,int y,int c)
{
	int xx,yy;
	if(kekka==NULL)return;

	xx=((int)kekka[18])+(((int)kekka[19])<<8);
	yy=((int)kekka[22])+(((int)kekka[23])<<8);
	xx=((xx+3)/4)*4;
	
	kekka[54+256*4+xx*(yy-1-y)+x]=c;
}

void save_bmp(unsigned  char*  kekka,char* name)
{
	int xx,yy,sz;
	FILE* fp;

	if(kekka==NULL || name==NULL)return;
	xx=((int)kekka[18])+(((int)kekka[19])<<8);
	yy=((int)kekka[22])+(((int)kekka[23])<<8);
	xx=((xx+3)/4)*4;
	
	sz=xx*yy+54+256*4;

	fp=fopen(name,"wb");
	if(fp==NULL)return;

	fwrite(kekka,1,sz,fp);
	fclose(fp);
}

void free_bmp(unsigned  char*  kekka)
{
	if(kekka==NULL)return;
	free(kekka);
}



unsigned char* offset_bmp(unsigned char* bmp)
{
	if(bmp==NULL)return bmp;
	return bmp+54+256*4;
}


static void
draw_bitmap( unsigned char* bmp,
			 FT_Bitmap*  bitmap,
             FT_Int      x,
             FT_Int      y)
{

  FT_Int  i, j, p, q;
  FT_Int  x_max = x + bitmap->width;
  FT_Int  y_max = y + bitmap->rows;

  for ( i = x, p = 0; i < x_max; i++, p++ )
  {
    for ( j = y, q = 0; j < y_max; j++, q++ )
    {
	  puset_bmp(bmp,i,j,(bitmap->buffer[q * bitmap->width + p])?1:0);
    }
  }
}




unsigned char* make_bmp(int bmp_x,int bmp_y,int bmp_space_x,int bmp_space_y,unsigned short* mozi)
{
	FT_GlyphSlot  slot;
	FT_Bitmap*  bitmap;

	int top,mojisu;
	int moji_w1,moji_w2;
	int moji_w;
	int off_x,off_y;
	int i;
	unsigned char* bmp;


	if(mozi==NULL)return NULL;

	mojisu=0;
	while(mozi[mojisu])mojisu++;

	if(mojisu<=0 || mojisu>=100)
	{
		return NULL;
	}

	moji_w1=(bmp_x-bmp_space_x*2)/mojisu;
	moji_w2=(bmp_y-bmp_space_y*2)/mojisu;

	moji_w=moji_w1;
	if(moji_w>moji_w2)moji_w=moji_w2;


	top=(int)((double)moji_w*(1.0-0.125));


	off_x=(bmp_x-moji_w*mojisu)/2;
	off_y=(bmp_y-moji_w)/2;

	bmp=create_bmp(bmp_x,bmp_y);
	if(bmp==NULL)return NULL;

	for(i=0;i<mojisu;i++)
	{

		if(mozi[i]<256)
			FT_Set_Pixel_Sizes( font_face,moji_w*2,moji_w);
		else
			FT_Set_Pixel_Sizes( font_face,moji_w,moji_w);

		FT_Load_Char( font_face,mozi[i], FT_LOAD_RENDER);

		slot=font_face->glyph;
		bitmap=&slot->bitmap;

	    draw_bitmap( bmp,&slot->bitmap,
                 off_x+moji_w*i+slot->bitmap_left,
                off_y+top- slot->bitmap_top);
	}

	return bmp;

	//save_bmp(bmp,bm.name);
	//free_bmp(bmp);

	//done_freetype();

}
