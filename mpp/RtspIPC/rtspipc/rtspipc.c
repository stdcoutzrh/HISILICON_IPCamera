#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

#include "rtsp.h"
#include "sample_comm.h"
#include <../../include/mp4v2/mp4v2.h>
#include"loadbmp.h"

VIDEO_NORM_E gs_enNorm = VIDEO_ENCODING_MODE_PAL;
HI_U32 g_u32BlkCnt = 4;


/***************************************************************************************/
#define VENC_RECORD_CHNID				0

#define VENC_RECORD_LOGO_OSD_HANDLE		2

#define VENC_RECORD_TIME_OSD_HANDLE		11

/***************************************************************************************/

char*  SAMPLE_RGN_GetBMPSrc(int num,int resolution)
{
	char* file = "res/0.bmp";

	file = "res/0_1080p.bmp";

	switch(num)
	{
		case 0:
			file = "res/0_1080p.bmp";
			break;
		case 1:
			file = "res/1_1080p.bmp";
			break;
		case 2:
			file = "res/2_1080p.bmp";
			break;
		case 3:
			file = "res/3_1080p.bmp";
			break;
		case 4:
			file = "res/4_1080p.bmp";
			break;
		case 5:
			file = "res/5_1080p.bmp";
			break;
		case 6:
			file = "res/6_1080p.bmp";
			break;
		case 7:
			file = "res/7_1080p.bmp";
			break;
		case 8:
			file = "res/8_1080p.bmp";
			break;
		case 9:
			file = "res/9_1080p.bmp";
			break;
		default:
			file = "res/0_1080p.bmp";
			break;
	}


	return file;
}

int SAMPLE_RGN_LoadBMPCanvas_Logo(char* filename,int resolution, OSD_LOGO_T* pVideoLogo, OSD_COLOR_FMT_E enFmt)
{
	FILE* pFile;
	HI_U16  i, j;

	HI_U32  w, h;
	HI_U16 Bpp;

	OSD_BITMAPFILEHEADER  bmpFileHeader;
	OSD_BITMAPINFO            bmpInfo;

	HI_U8* pOrigBMPBuf;
	HI_U8* pRGBBuf;
	HI_U32 stride;
	HI_U8 r, g, b;
	HI_U8* pStart;
	HI_U16* pDst;
	extern OSD_COMP_INFO s_OSDCompInfo[];

	if (NULL == filename)
	{
		printf("OSD_LoadBMP: filename=NULL\n");
		return -1;
	}

	if (GetBmpInfo(filename, &bmpFileHeader, &bmpInfo) < 0)
	{
		return -1;
	}

	Bpp = bmpInfo.bmiHeader.biBitCount / 8;
	if (Bpp < 2)
	{
		/* only support 1555.8888  888 bitmap */
		printf("bitmap format not supported!\n");
		return -1;
	}

	if (bmpInfo.bmiHeader.biCompression != 0)
	{
		printf("not support compressed bitmap file!\n");
		return -1;
	}

	if (bmpInfo.bmiHeader.biHeight < 0)
	{
		printf("bmpInfo.bmiHeader.biHeight < 0\n");
		return -1;
	}

	if ( (pFile = fopen((char*)filename, "rb")) == NULL)
	{
		printf("Open file faild:%s!\n", filename);
		return -1;
	}

	printf("------------%d X %d -------------------\n",bmpInfo.bmiHeader.biWidth,bmpInfo.bmiHeader.biHeight);
	#if 0
	w = (HI_U16)252;
	h = (HI_U16)28;
	#else
	w = (HI_U16)bmpInfo.bmiHeader.biWidth;
	h = (HI_U16)bmpInfo.bmiHeader.biHeight;
	#endif
	//printf("w:%d, h:%d \n", w, h);
	stride = w * Bpp;  // Bpp:3,enFmt:4

	if (stride % 4)
	{
		stride = (stride & 0xfffc) + 4;
	}

	/* RGB8888 or RGB1555 */
	pOrigBMPBuf = (HI_U8*)malloc(h * stride);
	if (NULL == pOrigBMPBuf)
	{
		printf("not enough memory to malloc!\n");
		fclose(pFile);
		return -1;
	}

	pRGBBuf = pVideoLogo->pRGBBuffer;

	if (stride > pVideoLogo->stride)
	{
		printf("Bitmap's stride(%d) is bigger than canvas's stide(%d). Load bitmap error!\n", stride, pVideoLogo->stride);
		free(pOrigBMPBuf);
		fclose(pFile);
		return -1;
	}

	if (h > pVideoLogo->height)
	{
		printf("Bitmap's height(%d) is bigger than canvas's height(%d). Load bitmap error!\n", h, pVideoLogo->height);
		free(pOrigBMPBuf);
		fclose(pFile);
		return -1;
	}

	if (w > pVideoLogo->width)
	{
		printf("Bitmap's width(%d) is bigger than canvas's width(%d). Load bitmap error!\n", w, pVideoLogo->width);
		free(pOrigBMPBuf);
		fclose(pFile);
		return -1;
	}

	fseek(pFile, bmpFileHeader.bfOffBits, 0);
	if (fread(pOrigBMPBuf, 1, h * stride, pFile) != (h * stride) )
	{
		printf("fread (%d*%d)error!line:%d\n", h, stride, __LINE__);
		perror("fread:");
	}

	for (i = 0; i < h; i++)  // Bpp:3,enFmt:4
	{
		//printf("k:%d,i:%d,j:%d,h:%d,w:%d \n", k, i, j, h, w);
		for (j = 0; j < w; j++)
		{
			if (Bpp == 3) /*.....*/
			{
				switch (enFmt)
				{
				case OSD_COLOR_FMT_RGB444:
				case OSD_COLOR_FMT_RGB555:
				case OSD_COLOR_FMT_RGB565:
				case OSD_COLOR_FMT_RGB1555:
				case OSD_COLOR_FMT_RGB4444:
					/* start color convert */
					// h:48,w:32,stride:96  Canvas_h:48,Can_w:704,Can_Str:1408
					pStart = pOrigBMPBuf + ((h - 1) - i) * stride + j * Bpp;

					//pDst = (HI_U16*)(pRGBBuf + i * pVideoLogo->stride + j * 2 + k*w*2);
					// YYYY-MM-DD HH:MM:SS  k==4 || k==7 || k==13 || k==16

					pDst = (HI_U16*)(pRGBBuf + i * pVideoLogo->stride + j * 2);
					
					r = *(pStart);
					g = *(pStart + 1);
					b = *(pStart + 2);
					//printf("Func: %s, line:%d, Bpp: %d, bmp stride: %d, Canvas stride: %d, h:%d, w:%d.\n",
					//    __FUNCTION__, __LINE__, Bpp, stride, pVideoLogo->stride, i, j);
					*pDst = OSD_MAKECOLOR_U16(r, g, b, s_OSDCompInfo[enFmt]);

					break;

				case OSD_COLOR_FMT_RGB888:
				case OSD_COLOR_FMT_RGB8888:
					memcpy(pRGBBuf + i * pVideoLogo->stride + j * 4, pOrigBMPBuf + ((h - 1) - i)*stride + j * Bpp, Bpp);
					*(pRGBBuf + i * pVideoLogo->stride + j * 4 + 3) = 0xff; /*alpha*/
					break;

				default:
					printf("file(%s), line(%d), no such format!\n", __FILE__, __LINE__);
					break;
				}
			}
			else if ((Bpp == 2) || (Bpp == 4)) /*..............*/
			{
				memcpy(pRGBBuf + i * pVideoLogo->stride + j * Bpp, pOrigBMPBuf + ((h - 1) - i)*stride + j * Bpp, Bpp);
			}

		}

	}

	free(pOrigBMPBuf);
	pOrigBMPBuf = NULL;

	fclose(pFile);

	return 0;
}

int SAMPLE_RGN_LoadBMPCanvas_TimeSmap(char* filename,int resolution, OSD_LOGO_T* pVideoLogo, OSD_COLOR_FMT_E enFmt)
{
	FILE* pFile;
	HI_U16  i, j, k;
	const static HI_U8 spa = 16;

	HI_U32  w, h;
	HI_U16 Bpp;

	OSD_BITMAPFILEHEADER  bmpFileHeader;
	OSD_BITMAPINFO            bmpInfo;

	HI_U8* pOrigBMPBuf;
	HI_U8* pRGBBuf;
	HI_U32 stride;
	HI_U8 r, g, b;
	HI_U8* pStart;
	HI_U16* pDst;

	extern OSD_COMP_INFO s_OSDCompInfo[];
	time_t timep;
	struct tm *pLocalTime;
	time(&timep);

	#if 1
	pLocalTime = localtime(&timep);
	#else
	
	struct tm tmLoc;
	timep += global_get_cfg_apptime_zone_seconds();
	global_convert_time( timep,0,&tmLoc);
	pLocalTime = &tmLoc;
	#endif
	
	filename = "res/-_1080p.bmp";

	for (k=0; k<18; k++) // YYYY-MM-DD    HH:MM:SS
	{
		if (k==4 || k==7)
		{
			filename = "res/-_1080p.bmp";			
		}

		if (k==12 || k==15)
		{
			filename = "res/colon_1080p.bmp";			
		}

		switch(k) // YYYY-MM-DD    HH:MM:SS
		{
			// YYYY
			case 0:
				filename = SAMPLE_RGN_GetBMPSrc((1900+pLocalTime->tm_year)/1000,resolution); //2018
				break;
			case 1:
				filename = SAMPLE_RGN_GetBMPSrc(((1900+pLocalTime->tm_year)/100)%10,resolution);
				break;
			case 2:
				filename = SAMPLE_RGN_GetBMPSrc(((1900+pLocalTime->tm_year)%100)/10,resolution);
				break;
			case 3:
				filename = SAMPLE_RGN_GetBMPSrc((1900+pLocalTime->tm_year)%10,resolution);
				break;
			// MM
			case 5:
				filename = SAMPLE_RGN_GetBMPSrc((1+pLocalTime->tm_mon)/10,resolution);
				break;
			case 6:
				filename = SAMPLE_RGN_GetBMPSrc((1+pLocalTime->tm_mon)%10,resolution);
				break;
			// DD
			case 8:
				filename = SAMPLE_RGN_GetBMPSrc((pLocalTime->tm_mday)/10,resolution);
				break;
			case 9:
				filename = SAMPLE_RGN_GetBMPSrc((pLocalTime->tm_mday)%10,resolution);
				break;

			// HH
			case 10:
				filename = SAMPLE_RGN_GetBMPSrc((pLocalTime->tm_hour)/10,resolution);
				break;
			case 11:
				filename = SAMPLE_RGN_GetBMPSrc((pLocalTime->tm_hour)%10,resolution);
				break;
			// MM
			case 13:
				filename = SAMPLE_RGN_GetBMPSrc((pLocalTime->tm_min)/10,resolution);
				break;
			case 14:
				filename = SAMPLE_RGN_GetBMPSrc((pLocalTime->tm_min)%10,resolution);
				break;
			// SS
			case 16:
				filename = SAMPLE_RGN_GetBMPSrc((pLocalTime->tm_sec)/10,resolution);
				break;
			case 17:
				filename = SAMPLE_RGN_GetBMPSrc((pLocalTime->tm_sec)%10,resolution);
				break;
		}

		if (NULL == filename)
		{
			printf("OSD_LoadBMP: filename=NULL\n");
			return -1;
		}

		if (GetBmpInfo(filename, &bmpFileHeader, &bmpInfo) < 0)
		{
			return -1;
		}

		Bpp = bmpInfo.bmiHeader.biBitCount / 8;
		if (Bpp < 2)
		{
			/* only support 1555.8888  888 bitmap */
			printf("bitmap format not supported!\n");
			return -1;
		}

		if (bmpInfo.bmiHeader.biCompression != 0)
		{
			printf("not support compressed bitmap file!\n");
			return -1;
		}

		if (bmpInfo.bmiHeader.biHeight < 0)
		{
			printf("bmpInfo.bmiHeader.biHeight < 0\n");
			return -1;
		}

		if ( (pFile = fopen((char*)filename, "rb")) == NULL)
		{
			printf("Open file faild:%s!\n", filename);
			return -1;
		}

		w = (HI_U16)bmpInfo.bmiHeader.biWidth;
		h = (HI_U16)((bmpInfo.bmiHeader.biHeight > 0) ? bmpInfo.bmiHeader.biHeight : (-bmpInfo.bmiHeader.biHeight));

		//printf("w:%d, h:%d \n", w, h);
		stride = w * Bpp;  // Bpp:3,enFmt:4

		if (stride % 4)
		{
			stride = (stride & 0xfffc) + 4;
		}

		/* RGB8888 or RGB1555 */
		pOrigBMPBuf = (HI_U8*)malloc(h * stride);
		if (NULL == pOrigBMPBuf)
		{
			printf("not enough memory to malloc!\n");
			fclose(pFile);
			return -1;
		}

		pRGBBuf = pVideoLogo->pRGBBuffer;

		if (stride > pVideoLogo->stride)
		{
			printf("Bitmap's stride(%d) is bigger than canvas's stide(%d). Load bitmap error!\n", stride, pVideoLogo->stride);
			free(pOrigBMPBuf);
			fclose(pFile);
			return -1;
		}

		if (h > pVideoLogo->height)
		{
			printf("Bitmap's height(%d) is bigger than canvas's height(%d). Load bitmap error!\n", h, pVideoLogo->height);
			free(pOrigBMPBuf);
			fclose(pFile);
			return -1;
		}

		if (w > pVideoLogo->width)
		{
			printf("Bitmap's width(%d) is bigger than canvas's width(%d). Load bitmap error!\n", w, pVideoLogo->width);
			free(pOrigBMPBuf);
			fclose(pFile);
			return -1;
		}

		fseek(pFile, bmpFileHeader.bfOffBits, 0);
		if (fread(pOrigBMPBuf, 1, h * stride, pFile) != (h * stride) )
		{
			printf("fread (%d*%d)error!line:%d\n", h, stride, __LINE__);
			perror("fread:");
		}

		for (i = 0; i < h; i++)  // Bpp:3,enFmt:4
		{
			//printf("k:%d,i:%d,j:%d,h:%d,w:%d \n", k, i, j, h, w);
			for (j = 0; j < w; j++)
			{
				if (Bpp == 3) /*.....*/
				{
					switch (enFmt)
					{
						case OSD_COLOR_FMT_RGB444:
						case OSD_COLOR_FMT_RGB555:
						case OSD_COLOR_FMT_RGB565:
						case OSD_COLOR_FMT_RGB1555:
						case OSD_COLOR_FMT_RGB4444:
							/* start color convert */
							// h:48,w:32,stride:96  Canvas_h:48,Can_w:704,Can_Str:1408
							pStart = pOrigBMPBuf + ((h - 1) - i) * stride + j * Bpp;

							//pDst = (HI_U16*)(pRGBBuf + i * pVideoLogo->stride + j * 2 + k*w*2);
							// YYYY-MM-DD HH:MM:SS  k==4 || k==7 || k==13 || k==16

							if (k < 4){ //YYYY-MM-DD
								pDst = (HI_U16*)(pRGBBuf + i * pVideoLogo->stride + j * 2 + k*w*2);
							} else if (k == 4){
								pDst = (HI_U16*)(pRGBBuf + i * pVideoLogo->stride + j * 2 + k*32);
							} else if (k>4 && k<7){
								pDst = (HI_U16*)(pRGBBuf + i * pVideoLogo->stride + j * 2 + ((k-1)*32+16));
							} else if (k == 7){
								pDst = (HI_U16*)(pRGBBuf + i * pVideoLogo->stride + j * 2 + (6*32+16));
							} else if (k>7 && k<10){
								pDst = (HI_U16*)(pRGBBuf + i * pVideoLogo->stride + j * 2 + (k-1)*32);
							} else if (k==10 || k==11 || k==12){ //HH:**:**
								pDst = (HI_U16*)(pRGBBuf + i * pVideoLogo->stride + j * 2 + ((k-1)*32+ spa));
							} else if (k==13 || k==14 || k==15){ //***HH:**
								pDst = (HI_U16*)(pRGBBuf + i * pVideoLogo->stride + j * 2 + ((k-2)*32+ spa+16));
							} else if (k==16 || k==17){ //***HH:**
								pDst = (HI_U16*)(pRGBBuf + i * pVideoLogo->stride + j * 2 + ((k-2)*32+ spa));
							}


							r = *(pStart);
							g = *(pStart + 1);
							b = *(pStart + 2);
							//printf("Func: %s, line:%d, Bpp: %d, bmp stride: %d, Canvas stride: %d, h:%d, w:%d.\n",
							//    __FUNCTION__, __LINE__, Bpp, stride, pVideoLogo->stride, i, j);
							*pDst = OSD_MAKECOLOR_U16(r, g, b, s_OSDCompInfo[enFmt]);

							break;

						case OSD_COLOR_FMT_RGB888:
						case OSD_COLOR_FMT_RGB8888:
							memcpy(pRGBBuf + i * pVideoLogo->stride + j * 4, pOrigBMPBuf + ((h - 1) - i)*stride + j * Bpp, Bpp);
							*(pRGBBuf + i * pVideoLogo->stride + j * 4 + 3) = 0xff; /*alpha*/
							break;

						default:
							printf("file(%s), line(%d), no such format!\n", __FILE__, __LINE__);
							break;
					}
				}
				else if ((Bpp == 2) || (Bpp == 4)) /*..............*/
				{
					memcpy(pRGBBuf + i * pVideoLogo->stride + j * Bpp, pOrigBMPBuf + ((h - 1) - i)*stride + j * Bpp, Bpp);
				}

			}

		}

		free(pOrigBMPBuf);
		pOrigBMPBuf = NULL;

		fclose(pFile);
	}

    return 0;
}


int SAMPLE_RGN_LoadCanvasEx(int Type, OSD_LOGO_T* pVideoLogo, OSD_COLOR_FMT_E enFmt)
{
	int nRet = 0;
	char filename[32] = {0};

	if(Type == VENC_RECORD_LOGO_OSD_HANDLE){		
		sprintf(filename,"%s","res/logo_1080p.bmp");
 		nRet= SAMPLE_RGN_LoadBMPCanvas_Logo(filename,0, pVideoLogo, enFmt);
	}else{
		nRet = SAMPLE_RGN_LoadBMPCanvas_TimeSmap(filename,0, pVideoLogo, enFmt);
	}

	if(nRet != HI_SUCCESS){
            printf("OSD_LoadBMP error!\n");
		return -1;
	}

	return 0;
}

HI_S32 SAMPLE_RGN_CreateSurfaceByCanvas(int Type, OSD_SURFACE_S* pstSurface, HI_U8* pu8Virt, HI_U32 u32Width, HI_U32 u32Height, HI_U32 u32Stride)
{
	OSD_LOGO_T stLogo;
	stLogo.pRGBBuffer = pu8Virt;
	stLogo.width = u32Width;
	stLogo.height = u32Height;
	stLogo.stride = u32Stride;
	if (SAMPLE_RGN_LoadCanvasEx(Type, &stLogo, pstSurface->enColorFmt) < 0)
	{
		printf("load bmp error!\n");
		return -1;
	}

	pstSurface->u16Height = u32Height;
	pstSurface->u16Width = u32Width;
	pstSurface->u16Stride = u32Stride;

	return 0;
}

HI_S32 SAMPLE_RGN_UpdateCanvas(int Type, BITMAP_S* pstBitmap, HI_BOOL bFil, HI_U32 u16FilColor, SIZE_S* pstSize, HI_U32 u32Stride, PIXEL_FORMAT_E enPixelFmt)
{
	OSD_SURFACE_S Surface;

	if (PIXEL_FORMAT_RGB_1555 == enPixelFmt){
		Surface.enColorFmt = OSD_COLOR_FMT_RGB1555;
	}else if (PIXEL_FORMAT_RGB_4444 == enPixelFmt){
		Surface.enColorFmt = OSD_COLOR_FMT_RGB4444;
	}else if (PIXEL_FORMAT_RGB_8888 == enPixelFmt){
		Surface.enColorFmt = OSD_COLOR_FMT_RGB8888;
	}else{
		printf("Pixel format is not support!\n");
		return HI_FAILURE;
	}

	if (NULL == pstBitmap->pData){
		printf("malloc osd memroy err!\n");
		return HI_FAILURE;
	}
	SAMPLE_RGN_CreateSurfaceByCanvas(Type, &Surface, (HI_U8*)(pstBitmap->pData), pstSize->u32Width, pstSize->u32Height, u32Stride);

	pstBitmap->u32Width  = Surface.u16Width;
	pstBitmap->u32Height = Surface.u16Height;

	if (PIXEL_FORMAT_RGB_1555 == enPixelFmt){
		pstBitmap->enPixelFormat = PIXEL_FORMAT_RGB_1555;
	}else if (PIXEL_FORMAT_RGB_4444 == enPixelFmt){
		pstBitmap->enPixelFormat = PIXEL_FORMAT_RGB_4444;
	}else if (PIXEL_FORMAT_RGB_8888 == enPixelFmt){
		pstBitmap->enPixelFormat = PIXEL_FORMAT_RGB_8888;
	}

	int i = 0, j = 0;
	HI_U16* pu16Temp = NULL;
	pu16Temp = (HI_U16*)pstBitmap->pData;

	if (bFil)
	{
		//printf("@@@@----H:%d, W:%d \n", pstBitmap->u32Height,pstBitmap->u32Width);
		for (i = 0; i < pstBitmap->u32Height; i++)
		{
			for (j = 0; j < pstBitmap->u32Width; j++)
			{
				// printf("(%d,%d): %04x!\n", i,j,*pu16Temp);

				//if (u16FilColor == *pu16Temp)
				if (0x0000==*pu16Temp || 0xFFFF==*pu16Temp) //TODO fixed value, Fun para not work
				{
					*pu16Temp &= 0x7FFF;
				}

				pu16Temp++;
			}
		}
	}

	return HI_SUCCESS;
}

/**
 * u32Width = =bmp's Widthx3/2, and is 16 align
 */
HI_S32 SAMPLE_RGN_CreateOverlayForVenc(HI_U32 u32ChnId, RGN_HANDLE Handle, HI_U32 u32X, HI_U32 u32Y, HI_U32 u32Width, HI_U32 u32Height)
{
	//printf("u32X  = %d u32Y = %d \n",u32X,u32Y);
	HI_S32 s32Ret = 0;
	MPP_CHN_S stChn;
	RGN_ATTR_S stRgnAttr;
	RGN_CHN_ATTR_S stChnAttr;

	/* Add cover to vpss group */
	stChn.enModId  = HI_ID_VENC;
	stChn.s32DevId = 0;
	stChn.s32ChnId = u32ChnId/*0*/;

	stRgnAttr.enType = OVERLAY_RGN;
	stRgnAttr.unAttr.stOverlay.enPixelFmt       = PIXEL_FORMAT_RGB_1555;
	stRgnAttr.unAttr.stOverlay.stSize.u32Width  = u32Width; //16x, value=bmp's Widthx3/2
	stRgnAttr.unAttr.stOverlay.stSize.u32Height = u32Height; //16x
	//stRgnAttr.unAttr.stOverlay.u32CanvasNum = 4; //Max:6
	stRgnAttr.unAttr.stOverlay.u32BgColor       = 0xffffffff/*0xffffffff*//*0x000003e0*/;

	s32Ret = HI_MPI_RGN_Create(Handle, &stRgnAttr);
	if (s32Ret != HI_SUCCESS)
	{
		printf("--@@_1 HI_MPI_RGN_Create failed! s32Ret: 0x%x.\n", s32Ret);
		return s32Ret;
	}

	stChnAttr.bShow  = HI_TRUE ;
	stChnAttr.enType = OVERLAY_RGN;
	stChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = u32X; //16x
	stChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = u32Y; //16x
	stChnAttr.unChnAttr.stOverlayChn.u32BgAlpha   = 0/*128*/; //Bg: transparent
	stChnAttr.unChnAttr.stOverlayChn.u32FgAlpha   = 128;
	stChnAttr.unChnAttr.stOverlayChn.u32Layer     = 4 /*7 Handle*/;

	// below para must keep
	stChnAttr.unChnAttr.stOverlayChn.stQpInfo.bAbsQp 		= HI_FALSE ;
	stChnAttr.unChnAttr.stOverlayChn.stQpInfo.s32Qp  		= 0;
	stChnAttr.unChnAttr.stOverlayChn.stQpInfo.bQpDisable 	= HI_FALSE ;

	stChnAttr.unChnAttr.stOverlayChn.stInvertColor.stInvColArea.u32Height = 48/*16 * (Handle % 2 + 1)*/;
	stChnAttr.unChnAttr.stOverlayChn.stInvertColor.stInvColArea.u32Width  = 48/*16 * (Handle % 2 + 1)*/;
	stChnAttr.unChnAttr.stOverlayChn.stInvertColor.u32LumThresh = 128;
	stChnAttr.unChnAttr.stOverlayChn.stInvertColor.enChgMod	= LESSTHAN_LUM_THRESH;
	stChnAttr.unChnAttr.stOverlayChn.stInvertColor.bInvColEn	= HI_FALSE ;


	s32Ret = HI_MPI_RGN_AttachToChn(Handle, &stChn, &stChnAttr);
	if (s32Ret != HI_SUCCESS)
	{
		printf("HI_MPI_RGN_AttachToChn failed! s32Ret: 0x%x.\n", s32Ret);
		return s32Ret;
	}

	return HI_SUCCESS;
}

/**
 * for adding 1080p Video Region
 */

HI_S32 SAMPLE_RGN_Add(unsigned int Handle,int Type)
{
    	//printf("-------------------%s add rgn %d --------------------\n",__func__,Type);

	HI_S32 s32Ret = HI_SUCCESS;
	RGN_ATTR_S stRgnAttrSet;
	RGN_CANVAS_INFO_S stCanvasInfo;
	BITMAP_S stBitmap;
	SIZE_S stSize;

	/* Photo logo */
	s32Ret = HI_MPI_RGN_GetAttr(Handle/*VencOsdHandle*/, &stRgnAttrSet);
	if (HI_SUCCESS != s32Ret)
	{
		printf("HI_MPI_RGN_GetAttr failed! s32Ret: 0x%x.\n", s32Ret);
		return s32Ret;
	}

	s32Ret = HI_MPI_RGN_GetCanvasInfo(Handle/*VencOsdHandle*/, &stCanvasInfo);
	if (HI_SUCCESS != s32Ret)
	{
		printf("HI_MPI_RGN_GetCanvasInfo failed! s32Ret: 0x%x.\n", s32Ret);
		return s32Ret;
	}

	stBitmap.pData   = (void*)stCanvasInfo.u32VirtAddr;
	stSize.u32Width  = stCanvasInfo.stSize.u32Width;
	stSize.u32Height = stCanvasInfo.stSize.u32Height;
	s32Ret = SAMPLE_RGN_UpdateCanvas(Type, &stBitmap, HI_TRUE , 0x0000, &stSize, stCanvasInfo.u32Stride,stRgnAttrSet.unAttr.stOverlayEx.enPixelFmt);
	if (HI_SUCCESS != s32Ret)
	{
		printf("SAMPLE_RGN_UpdateCanvas failed! s32Ret: 0x%x.\n", s32Ret);
		return s32Ret;
	}

	s32Ret = HI_MPI_RGN_UpdateCanvas(Handle/*VencOsdHandle*/);
	if (HI_SUCCESS != s32Ret)
	{
		printf("HI_MPI_RGN_UpdateCanvas failed! s32Ret: 0x%x.\n", s32Ret);
		return s32Ret;
	}

	return HI_SUCCESS;
}

HI_VOID* SAMPLE_RGN_AddVideoTimestamp(HI_VOID* p)
{
	HI_S32 s32Ret = HI_SUCCESS;
	time_t timep;
	struct tm *pLocalTime;
	HI_U8 seconds = 80;

	RGN_HANDLE Handle;
	Handle = VENC_RECORD_TIME_OSD_HANDLE;

	while (1)
	{
		time(&timep);
		pLocalTime = localtime(&timep);
		if (seconds == pLocalTime->tm_sec){
			usleep(150*1000);
			continue;
		} else {
			seconds = pLocalTime->tm_sec;
		}

		s32Ret = SAMPLE_RGN_Add( Handle, VENC_RECORD_TIME_OSD_HANDLE);
		if (HI_SUCCESS != s32Ret)
		{
			printf("SAMPLE_RGN_Add line %d  failed! s32Ret: 0x%x.\n",__LINE__, s32Ret);
			break;
		}
	}

	pthread_detach(pthread_self());

	return 0;
}

/**
 * create all needed Regions in Video
 */
HI_S32 SAMPLE_RGN_CreateVideoRegion(HI_VOID)
{
	HI_U32 u32ChnId 		= 0;
	RGN_HANDLE Handle 	= 0;
	HI_U32 coordinate_x 	= 0;
	HI_U32 coordinate_y 	= 0;
	HI_U32 region_with 	= 0;
	HI_U32 region_height = 0;

	u32ChnId 	= VENC_RECORD_CHNID;

	region_with		= 480;// 252 *3 / 2
	region_height 	= 320;// 28 * 3 / 2
	
	coordinate_x 	= 0;
	coordinate_y 	= 720 - 320;
	
	Handle 			= VENC_RECORD_LOGO_OSD_HANDLE;

	SAMPLE_RGN_CreateOverlayForVenc(u32ChnId, Handle, coordinate_x, coordinate_y, region_with, region_height);

	SAMPLE_RGN_Add( Handle, VENC_RECORD_LOGO_OSD_HANDLE);


	Handle 		= VENC_RECORD_TIME_OSD_HANDLE;

	//24bit的bmp图片,一个点就是3字节。一个字节8bit
	region_with		= 544;  //18个字体14*32+4*16+16=528 预留16像素空间
	region_height 	= 24;  

	coordinate_x 	= 1280-16-260;//根据需求调整位置
	coordinate_y 	= 720 - 24-12;//下面预留12像素空间

	SAMPLE_RGN_CreateOverlayForVenc(u32ChnId, Handle, coordinate_x, coordinate_y, region_with, region_height);


	pthread_t osd_ThreadId = 0;

	pthread_create(&osd_ThreadId, NULL, SAMPLE_RGN_AddVideoTimestamp, NULL);

	return HI_SUCCESS;
}


/******************************************************************************
* function :  H.264@1080p@30fps+H.264@VGA@30fps


******************************************************************************/
HI_S32 SAMPLE_VENC_720P_CLASSIC(HI_VOID)
{
	PAYLOAD_TYPE_E enPayLoad[3]= {PT_H264, PT_H264,PT_H264};
	PIC_SIZE_E enSize[3] = {PIC_HD720, PIC_VGA,PIC_QVGA};
	HI_U32 u32Profile = 0;

	VB_CONF_S stVbConf;
	SAMPLE_VI_CONFIG_S stViConfig = {0};

	VPSS_GRP VpssGrp;
	VPSS_CHN VpssChn;
	VPSS_GRP_ATTR_S stVpssGrpAttr;
	VPSS_CHN_ATTR_S stVpssChnAttr;
	VPSS_CHN_MODE_S stVpssChnMode;

	VENC_CHN VencChn;
	SAMPLE_RC_E enRcMode= SAMPLE_RC_FIXQP;

	HI_S32 s32ChnNum=1;

	HI_S32 s32Ret = HI_SUCCESS;
	HI_U32 u32BlkSize;
	SIZE_S stSize;

	
	/******************************************
	 step  1: init sys variable 
	******************************************/
	memset(&stVbConf,0,sizeof(VB_CONF_S));
	printf("s32ChnNum = %d\n",s32ChnNum);

	stVbConf.u32MaxPoolCnt = 128;

	/*video buffer*/

	u32BlkSize = SAMPLE_COMM_SYS_CalcPicVbBlkSize(gs_enNorm,\
	            enSize[0], SAMPLE_PIXEL_FORMAT, SAMPLE_SYS_ALIGN_WIDTH);
	stVbConf.astCommPool[0].u32BlkSize = u32BlkSize;
	stVbConf.astCommPool[0].u32BlkCnt = g_u32BlkCnt;


	/******************************************
	 step 2: mpp system init. 
	******************************************/
	s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);
	if (HI_SUCCESS != s32Ret)
	{
		SAMPLE_PRT("system init failed with %d!\n", s32Ret);
		goto END_VENC_720P_CLASSIC_0;
	}

	/******************************************
	 step 3: start vi dev & chn to capture
	******************************************/
	stViConfig.enViMode   = SENSOR_TYPE;
	stViConfig.enRotate   = ROTATE_NONE;
	stViConfig.enNorm     = VIDEO_ENCODING_MODE_AUTO;
	stViConfig.enViChnSet = VI_CHN_SET_NORMAL;
	stViConfig.enWDRMode  = WDR_MODE_NONE;
	s32Ret = SAMPLE_COMM_VI_StartVi(&stViConfig);
	if (HI_SUCCESS != s32Ret)
	{
		SAMPLE_PRT("start vi failed!\n");
		goto END_VENC_720P_CLASSIC_1;
	}

	/******************************************
	 step 4: start vpss and vi bind vpss
	******************************************/
	s32Ret = SAMPLE_COMM_SYS_GetPicSize(gs_enNorm, enSize[0], &stSize);
	if (HI_SUCCESS != s32Ret)
	{
		SAMPLE_PRT("SAMPLE_COMM_SYS_GetPicSize failed!\n");
		goto END_VENC_720P_CLASSIC_1;
	}

	VpssGrp = 0;
	stVpssGrpAttr.u32MaxW = stSize.u32Width;
	stVpssGrpAttr.u32MaxH = stSize.u32Height;
	stVpssGrpAttr.bIeEn = HI_FALSE;
	stVpssGrpAttr.bNrEn = HI_TRUE;
	stVpssGrpAttr.bHistEn = HI_FALSE;
	stVpssGrpAttr.bDciEn = HI_FALSE;
	stVpssGrpAttr.enDieMode = VPSS_DIE_MODE_NODIE;
	stVpssGrpAttr.enPixFmt = PIXEL_FORMAT_YUV_SEMIPLANAR_420;

	s32Ret = SAMPLE_COMM_VPSS_StartGroup(VpssGrp, &stVpssGrpAttr);
	if (HI_SUCCESS != s32Ret)
	{
		SAMPLE_PRT("Start Vpss failed!\n");
		goto END_VENC_720P_CLASSIC_2;
	}

	s32Ret = SAMPLE_COMM_VI_BindVpss(stViConfig.enViMode);
	if (HI_SUCCESS != s32Ret)
	{
		SAMPLE_PRT("Vi bind Vpss failed!\n");
		goto END_VENC_720P_CLASSIC_3;
	}

	VpssChn = 0;
	stVpssChnMode.enChnMode      = VPSS_CHN_MODE_USER;
	stVpssChnMode.bDouble        = HI_FALSE;
	stVpssChnMode.enPixelFormat  = PIXEL_FORMAT_YUV_SEMIPLANAR_420;
	stVpssChnMode.u32Width       = stSize.u32Width;
	stVpssChnMode.u32Height      = stSize.u32Height;
	stVpssChnMode.enCompressMode = COMPRESS_MODE_SEG;
	memset(&stVpssChnAttr, 0, sizeof(stVpssChnAttr));
	stVpssChnAttr.s32SrcFrameRate = -1;
	stVpssChnAttr.s32DstFrameRate = -1;
	enRcMode = SAMPLE_RC_CBR;
	s32Ret = SAMPLE_COMM_VPSS_EnableChn(VpssGrp, VpssChn, &stVpssChnAttr, &stVpssChnMode, HI_NULL);
	if (HI_SUCCESS != s32Ret)
	{
		SAMPLE_PRT("Enable vpss chn failed!\n");
		goto END_VENC_720P_CLASSIC_4;
	}
	/******************************************
	step 5: start stream venc
	******************************************/
	/*** enSize[0] **/

	VpssGrp = 0;
	VpssChn = 0;
	VencChn = 0;
	s32Ret = SAMPLE_COMM_VENC_Start(VencChn, enPayLoad[0],\
	                               gs_enNorm, enSize[0], enRcMode,u32Profile);
	if (HI_SUCCESS != s32Ret)
	{
		SAMPLE_PRT("Start Venc failed!\n");
		goto END_VENC_720P_CLASSIC_5;
	}

	s32Ret = SAMPLE_COMM_VENC_BindVpss(VencChn, VpssGrp, VpssChn);
	if (HI_SUCCESS != s32Ret)
	{
		SAMPLE_PRT("Start Venc failed!\n");
		goto END_VENC_720P_CLASSIC_5;
	}

	/******************************************
	 step 6: stream venc process -- get stream, then save it to file. 
	******************************************/
	s32Ret = SAMPLE_COMM_VENC_StartGetStream(s32ChnNum);
	if (HI_SUCCESS != s32Ret)
	{
		SAMPLE_PRT("Start Venc failed!\n");
		goto END_VENC_720P_CLASSIC_5;
	}

	printf("please press twice ENTER to exit this sample\n");
	HH_OSD_Init(); //hsx
	
	while(1){
		HH_OSD_All_Refresh();//hsx
		sleep(1);
	}
	getchar();
	getchar();

	/******************************************
	 step 7: exit process
	******************************************/
	SAMPLE_COMM_VENC_StopGetStream();

END_VENC_720P_CLASSIC_5:	
	VpssGrp = 0;
	VpssChn = 0;  
	VencChn = 0;
	SAMPLE_COMM_VENC_UnBindVpss(VencChn, VpssGrp, VpssChn);
	SAMPLE_COMM_VENC_Stop(VencChn);
	SAMPLE_COMM_VI_UnBindVpss(stViConfig.enViMode);
END_VENC_720P_CLASSIC_4:	//vpss stop
	VpssGrp = 0;
	VpssChn = 0;
	SAMPLE_COMM_VPSS_DisableChn(VpssGrp, VpssChn);	
END_VENC_720P_CLASSIC_3:    //vpss stop       
	SAMPLE_COMM_VI_UnBindVpss(stViConfig.enViMode);
END_VENC_720P_CLASSIC_2:    //vpss stop   
	SAMPLE_COMM_VPSS_StopGroup(VpssGrp);
END_VENC_720P_CLASSIC_1:	//vi stop
   	SAMPLE_COMM_VI_StopVi(&stViConfig);
END_VENC_720P_CLASSIC_0:	//system exit
    	SAMPLE_COMM_SYS_Exit();

    return s32Ret;    
}

/******************************************************************************
* function    : main()
******************************************************************************/
int main(int argc, char **argv)
{
	HI_S32 s32Ret;
	MPP_VERSION_S mppVersion;

	HI_MPI_SYS_GetVersion(&mppVersion);

	printf("MPP Ver  %s\n",mppVersion.aVersion);
	
	RtspServer_init();
	s32Ret = SAMPLE_VENC_720P_CLASSIC();

	if (HI_SUCCESS == s32Ret)
	    	printf("program exit normally!\n");
	else
	    	printf("program exit abnormally!\n");
	return s32Ret;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

