/**************************************************************************
 * 	FileName:		osd.h
 *	Description:	OSD Process
 *	Copyright(C):	2006-2008 HHDigital Inc.
 *	Version:		V 1.0
 *	Author:			ChenYG
 *	Created:		2006-08-06
 *	Updated:
 *
 **************************************************************************/
#ifndef   _HH_OSD_H_
#define   _HH_OSD_H_
#include <hi_common.h>
#include <hi_type.h>
#include <hi_comm_region.h>

#define   HH_OSD_TYPE_TIME			0
#define   HH_OSD_TYPE_TITLE			1


#define   HH_OSD_TYPE_CJ			    2
#define   HH_OSD_TYPE_MENU            2
#define   HH_OSD_TYPE_PTZ             3
#define   HH_OSD_TYPE_AT820           4
#define   HH_OSD_TYPE_AGC             4
#define   HH_OSD_TYPE_CLOUD           6

#define   HH_OSD_DEAFULT_POS_X			0
#define   HH_OSD_DEAFULT_TIME_POS_Y		32
#define   HH_OSD_DEAFULT_FRAME_POS_Y	36
#define   HH_OSD_DEAFULT_TITLE_POS_Y	(576 - 32 - 0)

#define   HH_OSD_MAX_BUFF_LENGTH	81920			//80k
/*********************************************************
* Colour              Red      Green      Blue      值
   黑色   Black        0        0          0         0
   白色   White        255      255        255       16777215
   灰色   Gray         192      192        192       12632256
   深灰色 Dark Grey    128      128        128       8421504
   红色   Red          255      0          0         255
   深红色 Dark Red     128      0          0         128
   绿色   Green        0        255        0         65280
   深绿色 Dark Green   0        128        0         32768
   蓝色   Blue         0        0          255       16711680
   深蓝色 Dark Blue    0        0          128       8388608
   紫红色 Magenta      255      0          255       16711935
   深紫红 Dark Magenta 128      0          128       8388736
   紫色   Cyan         0        255        255       16776960
   深紫   Dark Cyan    0        128        128       8421376
   黄色   Yellow       255      255        0         65535
   棕色   Brown        128      128        0         32896
   橙色   Orange       252		124        0
**************************************************************/
#define   HH_OSD_COLOR_BLACK		0x8000
#define   HH_OSD_COLOR_WHITE		0xFFFF
#define   HH_OSD_COLOR_RED			0xFC00
#define   HH_OSD_COLOR_YELLOW		0xFFE0
#define   HH_OSD_COLOR_CYAN			0x83FF
#define   HH_OSD_COLOR_BLUE			0x801F

#if 0
typedef struct tag_OSD_Logo
{
    HI_U32    width;        /* out */
    HI_U32    height;       /* out */
    HI_U32    stride;       /* in */
    HI_U8 *   pRGBBuffer;   /* in/out */
}OSD_LOGO_T;

#endif
typedef struct _OSD_POS_T
{
    int		x;
	int     y;
}OSD_POS_T;

int HH_OSD_Init();

int HH_Refresh_AllTime();

int HH_OSD_ChnOrg_Refresh(int nCh);

int HH_OSD_ChnAllName_Refresh(int nCh);

int HH_OSD_ChnTime_Refresh(int nCh);

int HH_OSD_AllTime_Refresh();

int HH_OSD_AllName_Refresh();

int HH_OSD_ChnColor_Refresh(int nCh);

int HH_OSD_ChnReBuild(int nCh);

int HH_OSD_Destory_Doom_Menu();
int HH_OSD_Rebuild_Doom_Menu();
int HH_OSD_All_Refresh();
int HH_OSD_Color_Refresh(int nCh, int nEnc, int osdType);
int HH_OSD_Show_Refresh(int nCh, int nMinEnc, int osdType);
int HH_OSD_ChnShow_Refresh(int nCh);

int HH_OSD_Org_Refresh(int nCh, int nEnc, int osdType);

int HH_OSD_ChName_Refresh(int nCh, int nMinEnc, int osdType);
int HH_OSD_Refresh_SelLine(int nCh, int nMin, int nType, int iLine);
int HH_OSD_SetTitle(int nCh, char* title);
int HH_OSD_SetOsdEnabel(int nCh,int osdType,int enabel);
int HH_OSD_SetOsdPos(int nCh,int osdType,int x,int y);
int HH_OSD_SetOsdPosDefault();

#if 1
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

typedef struct hiOSD_RGB_S
{
    HI_U8   u8B;
    HI_U8   u8G;
    HI_U8   u8R;
    HI_U8   u8Reserved;
}OSD_RGB_S;

typedef struct hiOSD_SURFACE_S
{
    OSD_COLOR_FMT_E enColorFmt;         /* color format */
    HI_U8  *pu8PhyAddr;               /* physical address */
    HI_U16  u16Height;                /* operation height */
    HI_U16  u16Width;                 /* operation width */
    HI_U16  u16Stride;                /* surface stride */
    HI_U16  u16Reserved;
}OSD_SURFACE_S;



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

#define   CAM_TITLE_LEN_NEW     64		//31个汉字，63个字符
typedef struct _HH_OSD_CTRL_ST
{
	unsigned int		Show;						//0: 关  1: 开
	unsigned short		X;							//x，y：显示的位置，该位置的范围为0-704，0-576，与图像的分辨率无关，x必须为4的倍数；
	unsigned short		Y;
}HH_OSD_CTRL_ST ;

typedef struct _VIDEO_OSD_STATUS_
{
	HH_OSD_CTRL_ST	stDateOSD;				// 0:不叠加日期,   1:叠加日期
	HH_OSD_CTRL_ST	stTimeOSD;				// 0:不叠加时间,   1:叠加时间
	HH_OSD_CTRL_ST	stWeekOSD;				// 0:不叠加星期,   1:叠加星期
	HH_OSD_CTRL_ST	stTitleOSD; 			// 0:不叠加标题,   1:叠加标题
	HH_OSD_CTRL_ST	stBitrateOSD;			// 0:不叠加码率,   1:叠加码率
	char		Title[CAM_TITLE_LEN_NEW];	//标题字符(64个字符  31个汉字)
	unsigned char       nOsdPrefer;   //OSD时间显示 0 则为 year-month-day
							  //            1 为   month-day-year
							  //            2 为   day-month-year
	unsigned char        nOsdColor;	  //osd 字体颜色
	unsigned char        nScene;       //百万ccd 场景澹(0-9)
	unsigned char        nOSDClientNum;// 是否显示用户连接数
	unsigned char        Reserved[24];

}VIDEO_OSD_STATUS ;

#endif


#endif













































