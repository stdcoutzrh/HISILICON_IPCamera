#ifndef     __LOAD_BMP_H__
#define     __LOAD_BMP_H__

#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */


/* the color format OSD supported */
typedef enum hiOSD_COLOR_FMT_E
{
    OSD_COLOR_FMT_RGB444    = 0,
    OSD_COLOR_FMT_RGB4444   = 1,
    OSD_COLOR_FMT_RGB555    = 2,
    OSD_COLOR_FMT_RGB565    = 3,
    OSD_COLOR_FMT_RGB1555   = 4,
    OSD_COLOR_FMT_RGB888    = 6,
    OSD_COLOR_FMT_RGB8888   = 7,
    OSD_COLOR_FMT_BUTT
}OSD_COLOR_FMT_E;



typedef struct hiOSD_SURFACE_S
{
    OSD_COLOR_FMT_E enColorFmt;         /* color format */
    HI_U8  *pu8PhyAddr;               /* physical address */
    HI_U16  u16Height;                /* operation height */
    HI_U16  u16Width;                 /* operation width */
    HI_U16  u16Stride;                /* surface stride */
    HI_U16  u16Reserved;
}OSD_SURFACE_S;

typedef struct tag_OSD_Logo
{
    HI_U32    width;        /* out */
    HI_U32    height;       /* out */
    HI_U32    stride;       /* in */
    HI_U8 *   pRGBBuffer;   /* in/out */
}OSD_LOGO_T;

typedef struct tag_OSD_BITMAPINFOHEADER{
        HI_U16      biSize;
        HI_U32       biWidth;
        HI_S32       biHeight;
        HI_U16       biPlanes;
        HI_U16       biBitCount;
        HI_U32      biCompression;
        HI_U32      biSizeImage;
        HI_U32       biXPelsPerMeter;
        HI_U32       biYPelsPerMeter;
        HI_U32      biClrUsed;
        HI_U32      biClrImportant;
} OSD_BITMAPINFOHEADER;

typedef struct tag_OSD_BITMAPFILEHEADER {
        HI_U32   bfSize;
        HI_U16    bfReserved1;
        HI_U16    bfReserved2;
        HI_U32   bfOffBits;
} OSD_BITMAPFILEHEADER; 

typedef struct tag_OSD_RGBQUAD {
        HI_U8    rgbBlue;
        HI_U8    rgbGreen;
        HI_U8    rgbRed;
        HI_U8    rgbReserved;
} OSD_RGBQUAD;

typedef struct tag_OSD_BITMAPINFO {
    OSD_BITMAPINFOHEADER    bmiHeader;
    OSD_RGBQUAD                 bmiColors[1];
} OSD_BITMAPINFO;

typedef struct hiOSD_COMPONENT_INFO_S{

    int alen;
    int rlen;
    int glen;
    int blen;
}OSD_COMP_INFO;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* End of #ifndef __LOAD_BMP_H__*/

