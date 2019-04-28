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

#include "sample_comm.h"
#include <../../include/mp4v2/mp4v2.h>
#include"loadbmp.h"
#if	RTSP_ENABLE

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <netdb.h>
#include <sys/socket.h>

#include <sys/ioctl.h>
#include <fcntl.h> 
#include <pthread.h>
#include <sys/ipc.h> 
#include <sys/msg.h>
#include <netinet/if_ether.h>
#include <net/if.h>

#include <linux/if_ether.h>
#include <linux/sockios.h>
#include <netinet/in.h> 
#include <arpa/inet.h> 

#endif


VIDEO_NORM_E gs_enNorm = VIDEO_ENCODING_MODE_PAL;


HI_U32 g_u32BlkCnt = 4;


#if	RTSP_ENABLE

RTP_FIXED_HEADER        *rtp_hdr;

NALU_HEADER		*nalu_hdr;
FU_INDICATOR	*fu_ind;
FU_HEADER		*fu_hdr;


RTSP_CLIENT g_rtspClients[MAX_RTSP_CLIENT];

int g_nSendDataChn = -1;
pthread_mutex_t g_mutex;
pthread_cond_t  g_cond;
pthread_mutex_t g_sendmutex;

pthread_t g_SendDataThreadId = 0;

//HAL_CLIENT_HANDLE hMainStreamClient = NULL,hSubStreamClient = NULL,hAudioClient = NULL;
char g_rtp_playload[20];
int   g_audio_rate = 8000;
//VIDEO_NORM_E gs_enNorm = VIDEO_ENCODING_MODE_NTSC;//30fps
int g_nframerate;

int exitok = 0;

int udpfd;

int count=0;


struct list_head RTPbuf_head = LIST_HEAD_INIT(RTPbuf_head);

static pthread_t gs_RtpPid;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////


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




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static char const* dateHeader()
{
	static char buf[200];
#if !defined(_WIN32_WCE)
	time_t tt = time(NULL);
	strftime(buf, sizeof buf, "Date: %a, %b %d %Y %H:%M:%S GMT\r\n", gmtime(&tt));
#endif

	return buf;
}
static char* GetLocalIP(int sock)
{
	struct ifreq ifreq;
	struct sockaddr_in *sin;
	char * LocalIP = malloc(20);
	strcpy(ifreq.ifr_name,"wlan");
	if (!(ioctl (sock, SIOCGIFADDR,&ifreq)))
    	{ 
		sin = (struct sockaddr_in *)&ifreq.ifr_addr;
		sin->sin_family = AF_INET;
       	strcpy(LocalIP,inet_ntoa(sin->sin_addr)); 
		//inet_ntop(AF_INET, &sin->sin_addr,LocalIP, 16);
    	} 
	printf("--------------------------------------------%s\n",LocalIP);
	return LocalIP;
}

char* strDupSize(char const* str) 
{
  if (str == NULL) return NULL;
  size_t len = strlen(str) + 1;
  char* copy = malloc(len);

  return copy;
}

int ParseRequestString(char const* reqStr,
		       unsigned reqStrSize,
		       char* resultCmdName,
		       unsigned resultCmdNameMaxSize,
		       char* resultURLPreSuffix,
		       unsigned resultURLPreSuffixMaxSize,
		       char* resultURLSuffix,
		       unsigned resultURLSuffixMaxSize,
		       char* resultCSeq,
		       unsigned resultCSeqMaxSize) 
{
  // This parser is currently rather dumb; it should be made smarter #####

  // Read everything up to the first space as the command name:
  int parseSucceeded = FALSE;
  unsigned i;
  for (i = 0; i < resultCmdNameMaxSize-1 && i < reqStrSize; ++i) {
    char c = reqStr[i];
    if (c == ' ' || c == '\t') {
      parseSucceeded = TRUE;
      break;
    }

    resultCmdName[i] = c;
  }
  resultCmdName[i] = '\0';
  if (!parseSucceeded) return FALSE;
      
  // Skip over the prefix of any "rtsp://" or "rtsp:/" URL that follows:
  unsigned j = i+1;
  while (j < reqStrSize && (reqStr[j] == ' ' || reqStr[j] == '\t')) ++j; // skip over any additional white space
  for (j = i+1; j < reqStrSize-8; ++j) {
    if ((reqStr[j] == 'r' || reqStr[j] == 'R')
	&& (reqStr[j+1] == 't' || reqStr[j+1] == 'T')
	&& (reqStr[j+2] == 's' || reqStr[j+2] == 'S')
	&& (reqStr[j+3] == 'p' || reqStr[j+3] == 'P')
	&& reqStr[j+4] == ':' && reqStr[j+5] == '/') {
      j += 6;
      if (reqStr[j] == '/') {
	// This is a "rtsp://" URL; skip over the host:port part that follows:
	++j;
	while (j < reqStrSize && reqStr[j] != '/' && reqStr[j] != ' ') ++j;
      } else {
	// This is a "rtsp:/" URL; back up to the "/":
	--j;
      }
      i = j;
      break;
    }
  }

  // Look for the URL suffix (before the following "RTSP/"):
  parseSucceeded = FALSE;
  unsigned k;
  for (k = i+1; k < reqStrSize-5; ++k) {
    if (reqStr[k] == 'R' && reqStr[k+1] == 'T' &&
	reqStr[k+2] == 'S' && reqStr[k+3] == 'P' && reqStr[k+4] == '/') {
      while (--k >= i && reqStr[k] == ' ') {} // go back over all spaces before "RTSP/"
      unsigned k1 = k;
      while (k1 > i && reqStr[k1] != '/' && reqStr[k1] != ' ') --k1;
      // the URL suffix comes from [k1+1,k]

      // Copy "resultURLSuffix":
      if (k - k1 + 1 > resultURLSuffixMaxSize) return FALSE; // there's no room
      unsigned n = 0, k2 = k1+1;
      while (k2 <= k) resultURLSuffix[n++] = reqStr[k2++];
      resultURLSuffix[n] = '\0';

      // Also look for the URL 'pre-suffix' before this:
      unsigned k3 = --k1;
      while (k3 > i && reqStr[k3] != '/' && reqStr[k3] != ' ') --k3;
      // the URL pre-suffix comes from [k3+1,k1]

      // Copy "resultURLPreSuffix":
      if (k1 - k3 + 1 > resultURLPreSuffixMaxSize) return FALSE; // there's no room
      n = 0; k2 = k3+1;
      while (k2 <= k1) resultURLPreSuffix[n++] = reqStr[k2++];
      resultURLPreSuffix[n] = '\0';

      i = k + 7; // to go past " RTSP/"
      parseSucceeded = TRUE;
      break;
    }
  }
  if (!parseSucceeded) return FALSE;

  // Look for "CSeq:", skip whitespace,
  // then read everything up to the next \r or \n as 'CSeq':
  parseSucceeded = FALSE;
  for (j = i; j < reqStrSize-5; ++j) {
    if (reqStr[j] == 'C' && reqStr[j+1] == 'S' && reqStr[j+2] == 'e' &&
	reqStr[j+3] == 'q' && reqStr[j+4] == ':') {
      j += 5;
      unsigned n;
      while (j < reqStrSize && (reqStr[j] ==  ' ' || reqStr[j] == '\t')) ++j;
      for (n = 0; n < resultCSeqMaxSize-1 && j < reqStrSize; ++n,++j) {
	char c = reqStr[j];
	if (c == '\r' || c == '\n') {
	  parseSucceeded = TRUE;
	  break;
	}

	resultCSeq[n] = c;
      }
      resultCSeq[n] = '\0';
      break;
    }
  }
  if (!parseSucceeded) return FALSE;

  return TRUE;
}

int OptionAnswer(char *cseq, int sock)
{
	if (sock != 0)
	{
		char buf[1024];
		memset(buf,0,1024);
		char *pTemp = buf;
		pTemp += sprintf(pTemp,"RTSP/1.0 200 OK\r\nCSeq: %s\r\n%sPublic: %s\r\n\r\n",
			cseq,dateHeader(),"OPTIONS,DESCRIBE,SETUP,PLAY,PAUSE,TEARDOWN");
	
		int reg = send(sock, buf,strlen(buf),0);
		if(reg <= 0)
		{
			return FALSE;
		}
		else
		{
			printf(">>>>>%s\n",buf);
		}
		return TRUE;
	}
	return FALSE;
}

int DescribeAnswer(char *cseq,int sock,char * urlSuffix,char* recvbuf)
{
	if (sock != 0)
	{
		char sdpMsg[1024];
		char buf[2048];
		memset(buf,0,2048);
		memset(sdpMsg,0,1024);
		char*localip;
		localip = GetLocalIP(sock);
		
		char *pTemp = buf;
		pTemp += sprintf(pTemp,"RTSP/1.0 200 OK\r\nCSeq: %s\r\n",cseq);
		pTemp += sprintf(pTemp,"%s",dateHeader());
		pTemp += sprintf(pTemp,"Content-Type: application/sdp\r\n");
		

		char *pTemp2 = sdpMsg;
		pTemp2 += sprintf(pTemp2,"v=0\r\n");
		pTemp2 += sprintf(pTemp2,"o=StreamingServer 3331435948 1116907222000 IN IP4 %s\r\n",localip);
		pTemp2 += sprintf(pTemp2,"s=H.264\r\n");
		pTemp2 += sprintf(pTemp2,"c=IN IP4 0.0.0.0\r\n");
		pTemp2 += sprintf(pTemp2,"t=0 0\r\n");
		pTemp2 += sprintf(pTemp2,"a=control:*\r\n");
		

		/*H264 TrackID=0 RTP_PT 96*/
		pTemp2 += sprintf(pTemp2,"m=video 0 RTP/AVP 96\r\n");
		pTemp2 += sprintf(pTemp2,"a=control:trackID=0\r\n");
		pTemp2 += sprintf(pTemp2,"a=rtpmap:96 H264/90000\r\n");
		pTemp2 += sprintf(pTemp2,"a=fmtp:96 packetization-mode=1; sprop-parameter-sets=%s\r\n", "AAABBCCC");
#if 1
		
		/*G726*/
		
		pTemp2 += sprintf(pTemp2,"m=audio 0 RTP/AVP 97\r\n");
		pTemp2 += sprintf(pTemp2,"a=control:trackID=1\r\n");
		if(strcmp(g_rtp_playload,"AAC")==0)
		{
			pTemp2 += sprintf(pTemp2,"a=rtpmap:97 MPEG4-GENERIC/%d/2\r\n",16000);
			pTemp2 += sprintf(pTemp2,"a=fmtp:97 streamtype=5;profile-level-id=1;mode=AAC-hbr;sizelength=13;indexlength=3;indexdeltalength=3;config=1410\r\n");
		}
		else
		{
			pTemp2 += sprintf(pTemp2,"a=rtpmap:97 G726-32/%d/1\r\n",8000);
			pTemp2 += sprintf(pTemp2,"a=fmtp:97 packetization-mode=1\r\n");
		}	
#endif
		pTemp += sprintf(pTemp,"Content-length: %d\r\n", strlen(sdpMsg));     
		pTemp += sprintf(pTemp,"Content-Base: rtsp://%s/%s/\r\n\r\n",localip,urlSuffix);
		
		//printf("mem ready\n");
		strcat(pTemp, sdpMsg);
		free(localip);
		//printf("Describe ready sent\n");
		int re = send(sock, buf, strlen(buf),0);
		if(re <= 0)
		{
			return FALSE;
		}
		else
		{
			printf(">>>>>%s\n",buf);
		}
	}

	return TRUE;
}
void ParseTransportHeader(char const* buf,
						  StreamingMode* streamingMode,
						 char**streamingModeString,
						 char**destinationAddressStr,
						 u_int8_t* destinationTTL,
						 portNumBits* clientRTPPortNum, // if UDP
						 portNumBits* clientRTCPPortNum, // if UDP
						 unsigned char* rtpChannelId, // if TCP
						 unsigned char* rtcpChannelId // if TCP
						 )
 {
	// Initialize the result parameters to default values:
	*streamingMode = RTP_UDP;
	*streamingModeString = NULL;
	*destinationAddressStr = NULL;
	*destinationTTL = 255;
	*clientRTPPortNum = 0;
	*clientRTCPPortNum = 1; 
	*rtpChannelId = *rtcpChannelId = 0xFF;
	
	portNumBits p1, p2;
	unsigned ttl, rtpCid, rtcpCid;
	
	// First, find "Transport:"
	while (1) {
		if (*buf == '\0') return; // not found
		if (strncasecmp(buf, "Transport: ", 11) == 0) break;
		++buf;
	}
	
	// Then, run through each of the fields, looking for ones we handle:
	char const* fields = buf + 11;
	char* field = strDupSize(fields);
	while (sscanf(fields, "%[^;]", field) == 1) {
		if (strcmp(field, "RTP/AVP/TCP") == 0) {
			*streamingMode = RTP_TCP;
		} else if (strcmp(field, "RAW/RAW/UDP") == 0 ||
			strcmp(field, "MP2T/H2221/UDP") == 0) {
			*streamingMode = RAW_UDP;
			//*streamingModeString = strDup(field);
		} else if (strncasecmp(field, "destination=", 12) == 0)
		{
			//delete[] destinationAddressStr;
			free(destinationAddressStr);
			//destinationAddressStr = strDup(field+12);
		} else if (sscanf(field, "ttl%u", &ttl) == 1) {
			destinationTTL = (u_int8_t)ttl;
		} else if (sscanf(field, "client_port=%hu-%hu", &p1, &p2) == 2) {
			*clientRTPPortNum = p1;
			*clientRTCPPortNum = p2;
		} else if (sscanf(field, "client_port=%hu", &p1) == 1) {
			*clientRTPPortNum = p1;
			*clientRTCPPortNum = streamingMode == RAW_UDP ? 0 : p1 + 1;
		} else if (sscanf(field, "interleaved=%u-%u", &rtpCid, &rtcpCid) == 2) {
			*rtpChannelId = (unsigned char)rtpCid;
			*rtcpChannelId = (unsigned char)rtcpCid;
		}
		
		fields += strlen(field);
		while (*fields == ';') ++fields; // skip over separating ';' chars
		if (*fields == '\0' || *fields == '\r' || *fields == '\n') break;
	}
	free(field);
}


int SetupAnswer(char *cseq,int sock,int SessionId,char * urlSuffix,char* recvbuf,int* rtpport, int* rtcpport)
{
	if (sock != 0)
	{
		char buf[1024];
		memset(buf,0,1024);
		
		StreamingMode streamingMode;
		char* streamingModeString; // set when RAW_UDP streaming is specified
		char* clientsDestinationAddressStr;
		u_int8_t clientsDestinationTTL;
		portNumBits clientRTPPortNum, clientRTCPPortNum;
		unsigned char rtpChannelId, rtcpChannelId;
		ParseTransportHeader(recvbuf,&streamingMode, &streamingModeString,
			&clientsDestinationAddressStr, &clientsDestinationTTL,
			&clientRTPPortNum, &clientRTCPPortNum,
			&rtpChannelId, &rtcpChannelId);

		//Port clientRTPPort(clientRTPPortNum);
		//Port clientRTCPPort(clientRTCPPortNum);
		*rtpport = clientRTPPortNum;
		*rtcpport = clientRTCPPortNum;
		
		char *pTemp = buf;
		char*localip;
		localip = GetLocalIP(sock);
		pTemp += sprintf(pTemp,"RTSP/1.0 200 OK\r\nCSeq: %s\r\n%sTransport: RTP/AVP;unicast;destination=%s;client_port=%d-%d;server_port=%d-%d\r\nSession: %d\r\n\r\n",
			cseq,dateHeader(),localip,
			ntohs(htons(clientRTPPortNum)), 
			ntohs(htons(clientRTCPPortNum)), 
			ntohs(2000),
			ntohs(2001),
			SessionId);

		free(localip);
		int reg = send(sock, buf,strlen(buf),0);
		if(reg <= 0)
		{
			return FALSE;
		}
		else
		{
			printf(">>>>>%s",buf);
		}
		return TRUE;
	}
	return FALSE;
}

int PlayAnswer(char *cseq, int sock,int SessionId,char* urlPre,char* recvbuf)
{
	if (sock != 0)
	{
		char buf[1024];
		memset(buf,0,1024);
		char *pTemp = buf;
		char*localip;
		localip = GetLocalIP(sock);
		pTemp += sprintf(pTemp,"RTSP/1.0 200 OK\r\nCSeq: %s\r\n%sRange: npt=0.000-\r\nSession: %d\r\nRTP-Info: url=rtsp://%s/%s;seq=0\r\n\r\n",
			cseq,dateHeader(),SessionId,localip,urlPre);

		free(localip);
		
		int reg = send(sock, buf,strlen(buf),0);
		if(reg <= 0)
		{
			return FALSE;
		}
		else
		{
			printf(">>>>>%s",buf);
			udpfd = socket(AF_INET,SOCK_DGRAM,0);//UDP
			struct sockaddr_in server;
			server.sin_family=AF_INET;
		   	server.sin_port=htons(g_rtspClients[0].rtpport[0]);          
		   	server.sin_addr.s_addr=inet_addr(g_rtspClients[0].IP);
			connect(udpfd,(struct sockaddr *)&server,sizeof(server));
    		printf("udp up\n");
		}
		return TRUE;
	}
	return FALSE;
}

int PauseAnswer(char *cseq,int sock,char *recvbuf)
{
	if (sock != 0)
	{
		char buf[1024];
		memset(buf,0,1024);
		char *pTemp = buf;
		pTemp += sprintf(pTemp,"RTSP/1.0 200 OK\r\nCSeq: %s\r\n%s\r\n\r\n",
			cseq,dateHeader());
	
		int reg = send(sock, buf,strlen(buf),0);
		if(reg <= 0)
		{
			return FALSE;
		}
		else
		{
			printf(">>>>>%s",buf);
		}
		return TRUE;
	}
	return FALSE;
}

int TeardownAnswer(char *cseq,int sock,int SessionId,char *recvbuf)
{
	if (sock != 0)
	{
		char buf[1024];
		memset(buf,0,1024);
		char *pTemp = buf;
		pTemp += sprintf(pTemp,"RTSP/1.0 200 OK\r\nCSeq: %s\r\n%sSession: %d\r\n\r\n",
			cseq,dateHeader(),SessionId);
	
		int reg = send(sock, buf,strlen(buf),0);
		if(reg <= 0)
		{
			return FALSE;
		}
		else
		{
			printf(">>>>>%s",buf);
			close(udpfd);
		}
		return TRUE;
	}
	return FALSE;
}
void * RtspClientMsg(void*pParam)
{
	pthread_detach(pthread_self());
	int nRes;
	char pRecvBuf[RTSP_RECV_SIZE];
	RTSP_CLIENT * pClient = (RTSP_CLIENT*)pParam;
	memset(pRecvBuf,0,sizeof(pRecvBuf));
	printf("RTSP:-----Create Client %s\n",pClient->IP);
	while(pClient->status != RTSP_IDLE)
	{
		nRes = recv(pClient->socket, pRecvBuf, RTSP_RECV_SIZE,0);
		//printf("-------------------%d\n",nRes);
		if(nRes < 1)
		{
			//usleep(1000);
			printf("RTSP:Recv Error--- %d\n",nRes);
			g_rtspClients[pClient->index].status = RTSP_IDLE;
			g_rtspClients[pClient->index].seqnum = 0;
			g_rtspClients[pClient->index].tsvid = 0;
			g_rtspClients[pClient->index].tsaud = 0;
			close(pClient->socket);
			break;
		}
		char cmdName[PARAM_STRING_MAX];
		char urlPreSuffix[PARAM_STRING_MAX];
		char urlSuffix[PARAM_STRING_MAX];
		char cseq[PARAM_STRING_MAX];

		ParseRequestString(pRecvBuf,nRes,cmdName,sizeof(cmdName),urlPreSuffix,sizeof(urlPreSuffix),
			urlSuffix,sizeof(urlSuffix),cseq,sizeof(cseq));
		
		char *p = pRecvBuf;

		printf("<<<<<%s\n",p);

		//printf("\--------------------------\n");
		//printf("%s %s\n",urlPreSuffix,urlSuffix);

		if(strstr(cmdName, "OPTIONS"))
		{
			OptionAnswer(cseq,pClient->socket);
			
		}
		else if(strstr(cmdName, "DESCRIBE"))
		{
			DescribeAnswer(cseq,pClient->socket,urlSuffix,p);
			//printf("-----------------------------DescribeAnswer %s %s\n",
			//	urlPreSuffix,urlSuffix);
		}
		else if(strstr(cmdName, "SETUP"))
		{
			int rtpport,rtcpport;
			int trackID=0;
			SetupAnswer(cseq,pClient->socket,pClient->sessionid,urlPreSuffix,p,&rtpport,&rtcpport);

			sscanf(urlSuffix, "trackID=%u", &trackID);
			//printf("----------------------------------------------TrackId %d\n",trackID);
			if(trackID<0 || trackID>=2)trackID=0;
			g_rtspClients[pClient->index].rtpport[trackID] = rtpport;
			g_rtspClients[pClient->index].rtcpport= rtcpport;
			g_rtspClients[pClient->index].reqchn = atoi(urlPreSuffix);
			if(strlen(urlPreSuffix)<100)
				strcpy(g_rtspClients[pClient->index].urlPre,urlPreSuffix);
			//printf("-----------------------------SetupAnswer %s-%d-%d\n",
			//	urlPreSuffix,g_rtspClients[pClient->index].reqchn,rtpport);
		}
		else if(strstr(cmdName, "PLAY"))
		{
			PlayAnswer(cseq,pClient->socket,pClient->sessionid,g_rtspClients[pClient->index].urlPre,p);
			g_rtspClients[pClient->index].status = RTSP_SENDING;
			printf("Start Play\n",pClient->index);
			//printf("-----------------------------PlayAnswer %d %d\n",pClient->index);
			//usleep(100);
		}
		else if(strstr(cmdName, "PAUSE"))
		{
			PauseAnswer(cseq,pClient->socket,p);
		}
		else if(strstr(cmdName, "TEARDOWN"))
		{
			TeardownAnswer(cseq,pClient->socket,pClient->sessionid,p);
			g_rtspClients[pClient->index].status = RTSP_IDLE;
			g_rtspClients[pClient->index].seqnum = 0;
			g_rtspClients[pClient->index].tsvid = 0;
			g_rtspClients[pClient->index].tsaud = 0;
			close(pClient->socket);
		}
		//if(exitok){ exitok++;return NULL; } 
	}
	printf("RTSP:-----Exit Client %s\n",pClient->IP);
	return NULL;
}

void * RtspServerListen(void*pParam)
{
	int s32Socket;
	struct sockaddr_in servaddr;
	int s32CSocket;
    	int s32Rtn;
    	int s32Socket_opt_value = 1;
	int nAddrLen;
	struct sockaddr_in addrAccept;
	int bResult;
	
	memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(RTSP_SERVER_PORT); 

	s32Socket = socket(AF_INET, SOCK_STREAM, 0);

	if (setsockopt(s32Socket ,SOL_SOCKET,SO_REUSEADDR,&s32Socket_opt_value,sizeof(int)) == -1)     
    {
        return (void *)(-1);
    }
    s32Rtn = bind(s32Socket, (struct sockaddr *)&servaddr, sizeof(struct sockaddr_in));
    if(s32Rtn < 0)
    {
        return (void *)(-2);
    }
    
    s32Rtn = listen(s32Socket, 50);   
    if(s32Rtn < 0)
    {

         return (void *)(-2);
    }


	nAddrLen = sizeof(struct sockaddr_in);
	int nSessionId = 1000;
    while ((s32CSocket = accept(s32Socket, (struct sockaddr*)&addrAccept, &nAddrLen)) >= 0)
    {
		printf("<<<<RTSP Client %s Connected...\n", inet_ntoa(addrAccept.sin_addr));

		int nMaxBuf = 10 * 1024; // ÏµÍ³œ«»á·ÖÅä 2 x nMaxBuf µÄ»º³åŽóÐ¡
		if(setsockopt(s32CSocket, SOL_SOCKET, SO_SNDBUF, (char*)&nMaxBuf, sizeof(nMaxBuf)) == -1)
			printf("RTSP:!!!!!! Enalarge socket sending buffer error !!!!!!\n");
		int i;
		int bAdd=FALSE;
		for(i=0;i<MAX_RTSP_CLIENT;i++)
		{
			if(g_rtspClients[i].status == RTSP_IDLE)
			{
				memset(&g_rtspClients[i],0,sizeof(RTSP_CLIENT));
				g_rtspClients[i].index = i;
				g_rtspClients[i].socket = s32CSocket;
				g_rtspClients[i].status = RTSP_CONNECTED ;//RTSP_SENDING;
				g_rtspClients[i].sessionid = nSessionId++;
				strcpy(g_rtspClients[i].IP,inet_ntoa(addrAccept.sin_addr));
				pthread_t threadIdlsn = 0;
				
				struct sched_param sched;
				sched.sched_priority = 1;
				//to return ACKecho
				pthread_create(&threadIdlsn, NULL, RtspClientMsg, &g_rtspClients[i]);
				//pthread_setschedparam(threadIdlsn,SCHED_RR,&sched);
				
				bAdd = TRUE;
				break;
			}
		}
		if(bAdd==FALSE)
		{
			memset(&g_rtspClients[0],0,sizeof(RTSP_CLIENT));
			g_rtspClients[0].index = 0;
			g_rtspClients[0].socket = s32CSocket;
			g_rtspClients[0].status = RTSP_CONNECTED ;//RTSP_SENDING;
			g_rtspClients[0].sessionid = nSessionId++;
			strcpy(g_rtspClients[0].IP,inet_ntoa(addrAccept.sin_addr));
			pthread_t threadIdlsn = 0;
			struct sched_param sched;
			sched.sched_priority = 1;
			//to return ACKecho
			pthread_create(&threadIdlsn, NULL, RtspClientMsg, &g_rtspClients[0]);
			//pthread_setschedparam(threadIdlsn,SCHED_RR,&sched);
			bAdd = TRUE;
		}
		//if(exitok){ exitok++;return NULL; } 	
    }
    if(s32CSocket < 0)
    {
       // HI_OUT_Printf(0, "RTSP listening on port %d,accept err, %d\n", RTSP_SERVER_PORT, s32CSocket);
    }

	printf("----- INIT_RTSP_Listen() Exit !! \n");
	
	return NULL;
}
HI_S32 VENC_Sent(char *buffer,int buflen)
{
    HI_S32 i;
	int is=0;
	int nChanNum=0;

	for(is=0;is<MAX_RTSP_CLIENT;is++)
	{
		if(g_rtspClients[is].status!=RTSP_SENDING)
		{
		    continue;
		}
		int heart = g_rtspClients[is].seqnum % 10000;
		
		char* nalu_payload;
		int nAvFrmLen = 0;
		int nIsIFrm = 0;
		int nNaluType = 0;
		char sendbuf[500*1024+32];

	
		nAvFrmLen = buflen;

		struct sockaddr_in server;
		server.sin_family=AF_INET;
	   	server.sin_port=htons(g_rtspClients[is].rtpport[0]);          
	   	server.sin_addr.s_addr=inet_addr(g_rtspClients[is].IP);
		int	bytes=0;
		unsigned int timestamp_increse=0;
		int test = 0;

		//printf("start---------------------\n");
		//while(test<=100)
		//{
		//	printf("%x ",buffer[test]);
		//	test++;
	//	}

		timestamp_increse=(unsigned int)(90000.0 / 25);

		rtp_hdr =(RTP_FIXED_HEADER*)&sendbuf[0]; 
	
		rtp_hdr->payload     = RTP_H264;   
		rtp_hdr->version     = 2;          
		rtp_hdr->marker    = 0;           
		rtp_hdr->ssrc      = htonl(10);   

		if(nAvFrmLen<=nalu_sent_len)
		{
			rtp_hdr->marker=1;
			rtp_hdr->seq_no     = htons(g_rtspClients[is].seqnum++); 
			nalu_hdr =(NALU_HEADER*)&sendbuf[12]; 
			nalu_hdr->F=0; 
			nalu_hdr->NRI=  nIsIFrm; 
			nalu_hdr->TYPE=  nNaluType;
			nalu_payload=&sendbuf[13];
			memcpy(nalu_payload,buffer,nAvFrmLen);
            		g_rtspClients[is].tsvid = g_rtspClients[is].tsvid+timestamp_increse;            
			rtp_hdr->timestamp=htonl(g_rtspClients[is].tsvid);
			bytes=nAvFrmLen+ 13 ;				
			sendto(udpfd, sendbuf, bytes, 0, (struct sockaddr *)&server,sizeof(server));
		}
		else if(nAvFrmLen>nalu_sent_len)
		{
			int k=0,l=0;
			k=nAvFrmLen/nalu_sent_len;
			l=nAvFrmLen%nalu_sent_len;
			int t=0;        

            		g_rtspClients[is].tsvid = g_rtspClients[is].tsvid+timestamp_increse;
           		rtp_hdr->timestamp=htonl(g_rtspClients[is].tsvid);            
			while(t<=k)
			{
				rtp_hdr->seq_no = htons(g_rtspClients[is].seqnum++);
				if(t==0)
				{
					rtp_hdr->marker=0;
					fu_ind =(FU_INDICATOR*)&sendbuf[12];
					fu_ind->F= 0; 
					fu_ind->NRI= nIsIFrm;
					fu_ind->TYPE=28;
	
					fu_hdr =(FU_HEADER*)&sendbuf[13];
					fu_hdr->E=0;
					fu_hdr->R=0;
					fu_hdr->S=1;
					fu_hdr->TYPE=nNaluType;
	
					nalu_payload=&sendbuf[14];
					memcpy(nalu_payload,buffer,nalu_sent_len);
	
					bytes=nalu_sent_len+14;					
					sendto( udpfd, sendbuf, bytes, 0, (struct sockaddr *)&server,sizeof(server));
					t++;
	
				}
				else if(k==t)
				{
					rtp_hdr->marker=1;
					fu_ind =(FU_INDICATOR*)&sendbuf[12]; 
					fu_ind->F= 0 ;
					fu_ind->NRI= nIsIFrm ;
					fu_ind->TYPE=28;

					fu_hdr =(FU_HEADER*)&sendbuf[13];
					fu_hdr->R=0;
					fu_hdr->S=0;
					fu_hdr->TYPE= nNaluType;
					fu_hdr->E=1;
					nalu_payload=&sendbuf[14];
					memcpy(nalu_payload,buffer+t*nalu_sent_len,l);
					bytes=l+14;		
					sendto(udpfd, sendbuf, bytes, 0, (struct sockaddr *)&server,sizeof(server));
					t++;
				}
				else if(t<k && t!=0)
				{

					rtp_hdr->marker=0;

					fu_ind =(FU_INDICATOR*)&sendbuf[12]; 
					fu_ind->F=0; 
					fu_ind->NRI=nIsIFrm;
					fu_ind->TYPE=28;
					fu_hdr =(FU_HEADER*)&sendbuf[13];
					//fu_hdr->E=0;
					fu_hdr->R=0;
					fu_hdr->S=0;
					fu_hdr->E=0;
					fu_hdr->TYPE=nNaluType;
					nalu_payload=&sendbuf[14];
					memcpy(nalu_payload,buffer+t*nalu_sent_len,nalu_sent_len);
					bytes=nalu_sent_len+14;	
					sendto(udpfd, sendbuf, bytes, 0, (struct sockaddr *)&server,sizeof(server));
					t++;
				}
			}
		}

	}

	//------------------------------------------------------------
}


/******************************************************************************
* funciton : sent H264 stream
******************************************************************************/

HI_S32 SAMPLE_COMM_VENC_Sentjin(VENC_STREAM_S *pstStream)
{
    HI_S32 i,flag=0;

    for(i=0;i<MAX_RTSP_CLIENT;i++)//have atleast a connect
    {
		if(g_rtspClients[i].status == RTSP_SENDING)
		{
		    flag = 1;
		    break;
		}
    }
    if(flag)
    {
	    for (i = 0; i < pstStream->u32PackCount; i++)
	    {
		HI_S32 lens=0,j,lastadd=0,newadd=0,showflap=0;
		char sendbuf[320*1024];
		//char tmp[640*1024];
		lens = pstStream->pstPack[i].u32Len-pstStream->pstPack[i].u32Offset;
		memcpy(&sendbuf[0],pstStream->pstPack[i].pu8Addr+pstStream->pstPack[i].u32Offset,lens);
		//printf("lens = %d, count= %d\n",lens,count++);
		VENC_Sent(sendbuf,lens);		
		lens = 0;
	    }

	
    }
    return HI_SUCCESS;
}

HI_S32 saveStream(VENC_STREAM_S *pstStream)
{
    HI_S32 i,j,lens=0;

    for(j=0;j<MAX_RTSP_CLIENT;j++)//have atleast a connect
    {
		if(g_rtspClients[j].status == RTSP_SENDING)
		{
		    for (i = 0; i < pstStream->u32PackCount; i++)
		    {
				RTPbuf_s *p = (RTPbuf_s *)malloc(sizeof(RTPbuf_s));
				INIT_LIST_HEAD(&(p->list));

				lens = pstStream->pstPack[i].u32Len-pstStream->pstPack[i].u32Offset;
				p->buf = (char *)malloc(lens);
				p->len = lens;
				memcpy(p->buf,pstStream->pstPack[i].pu8Addr+pstStream->pstPack[i].u32Offset,lens);

				list_add_tail(&(p->list),&RTPbuf_head);
				count++;
				//printf("count = %d\n",count);
		    }
    	}
    }

    return HI_SUCCESS;
}


void RtspServer_init(void)
{
	int i;
	pthread_t threadId = 0;

	memset(&g_rtp_playload,0,sizeof(g_rtp_playload));
	strcpy(&g_rtp_playload,"G726-32");
	g_audio_rate = 8000;
	pthread_mutex_init(&g_sendmutex,NULL);
	pthread_mutex_init(&g_mutex,NULL);
	pthread_cond_init(&g_cond,NULL);
	memset(&g_rtspClients,0,sizeof(RTSP_CLIENT)*MAX_RTSP_CLIENT);
	
	//pthread_create(&g_SendDataThreadId, NULL, SendDataThread, NULL);
	
	struct sched_param thdsched;
	thdsched.sched_priority = 2;
	//to listen visiting
	pthread_create(&threadId, NULL, RtspServerListen, NULL);
	//pthread_setschedparam(threadId,SCHED_RR,&thdsched);
	printf("RTSP:-----Init Rtsp server\n");

	pthread_create(&gs_RtpPid, 0, vdRTPSendThread, NULL);
	
	//exitok++;

}
void RtspServer_exit(void)
{
	return;
}

HI_VOID* vdRTPSendThread(HI_VOID *p)
{
	while(1)
	{
		if(!list_empty(&RTPbuf_head))
		{
			
			RTPbuf_s *p = get_first_item(&RTPbuf_head,RTPbuf_s,list);
			VENC_Sent(p->buf,p->len);
			list_del(&(p->list));
			free(p->buf);
			free(p);
			p = NULL;
			count--;
			//printf("count = %d\n",count);
		
		}
		usleep(5000);
	}
}

#endif


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

