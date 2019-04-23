/******************************************************************************
  A simple program of Hisilicon HI3516A osd implementation.
  the flow as follows:
    1) init mpp system.
    2) start vi ( internal isp, ViDev 0, 2 vichn)                  
    3) start venc
    4) osd process, you can see video from some H264 streams files. the video will show as follows step:
        4.1) create some cover/osd regions
        4.2) display  cover/osd regions ( One Region -- Multi-VencGroup )
        4.3) change all vencGroups Regions' Layer
        4.4) change all vencGroups Regions' position
        4.5) change all vencGroups Regions' color
        4.6) change all vencGroups Regions' alpha (front and backgroud)
        4.7) load bmp form bmp-file to Region-0
        4.8) change BmpRegion-0
    6) stop venc
    7) stop vi and system.
  Copyright (C), 2010-2011, Hisilicon Tech. Co., Ltd.
 ******************************************************************************
    Modification:  2011-2 Created
******************************************************************************/
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
#include "loadbmp.h"

#include "sample_comm.h"
#include "hi_tde_api.h"
#include "hi_tde_type.h"

HI_BOOL bExit   = HI_FALSE;
HI_BOOL bTravel = HI_FALSE;
HI_BOOL bShape1 = HI_FALSE;
HI_BOOL bShape2 = HI_FALSE;
HI_BOOL bArea   = HI_FALSE;

pthread_t g_stRgnThread = 0;
pthread_t g_stVencThread = 0;
pthread_t g_stVencRgnThread = 0;
pthread_t g_stVpssRgnThread = 0;

pthread_t g_stVpssOsdReverseThread = 0;
pthread_t g_stVencOsdReverseThread = 0;

#define OSD_REVERSE_RGN_MAXCNT 16
#define MAX_VENC_WORK_CHN_NUM  4

VIDEO_NORM_E gs_enNorm = VIDEO_ENCODING_MODE_PAL;
VO_INTF_TYPE_E g_enVoIntfType = VO_INTF_LCD;
PIC_SIZE_E g_enPicSize = PIC_HD1080;


SAMPLE_VI_CONFIG_S g_stViChnConfig =
{
    SONY_IMX222_DC_1080P_30FPS,
    VIDEO_ENCODING_MODE_AUTO,

    ROTATE_NONE,
    VI_CHN_SET_NORMAL,
    WDR_MODE_NONE
};

#define SAMPLE_RGN_NOT_PASS(err)\
do {\
	printf("\033[0;31mtest case <%s>not pass at line:%d err:%x\033[0;39m\n",\
		__FUNCTION__,__LINE__,err);\
    exit(-1);\
}while(0)

pthread_mutex_t Rgnmutex_Tmp = PTHREAD_MUTEX_INITIALIZER;

typedef struct hiRGN_OSD_REVERSE_INFO_S
{
    RGN_HANDLE Handle;
    HI_U8 u8PerPixelLumaThrd;

    VPSS_GRP VpssGrp;
    VPSS_CHN VpssChn;
    VPSS_REGION_INFO_S stLumaRgnInfo;

}RGN_OSD_REVERSE_INFO_S;

typedef struct PTHREAD_VENC
{
    VENC_CHN VeChnId;
    HI_S32 s32FrmCnt;
    
    FILE  *pstream;
}VENC_PTHREAD_INFO_S;

typedef struct hiRGN_SIZE_S
{
    HI_U32 u32RgnNum;
    SIZE_S stSIZE;
}RGN_SIZE_S;

typedef struct hiRGN_ATTR_INFO_S
{
    RGN_HANDLE Handle;
    HI_U32 u32RgnNum;
}RGN_ATTR_INFO_S;

HI_S32 SAMPLE_RGN_SYS_Exit(HI_VOID)
{
    HI_MPI_SYS_Exit();
    HI_MPI_VB_Exit();

    return HI_SUCCESS;
}

HI_VOID SAMPLE_RGN_Usage1(HI_CHAR *sPrgNm)
{
    printf("Usage : %s <intf>\n", sPrgNm);
    printf("intf:\n");
    printf("\t 0) vo LCD output, default.\n");
    printf("\t 1) vo BT656 output.\n");
    return;
}

HI_VOID SAMPLE_RGN_Usage2(HI_VOID)
{
    printf("press sample command as follows!\n");
    printf("index:\n");
    printf("\t 0) VI->VPSS(COVER)->VO                             \n");
    printf("\t 1) VI->VPSS(COVEREX)->VO                           \n");
	printf("\t 2) VI->VPSS(OVERLAYEX)->VO                         \n");
    printf("\t 3) VI->VPSS(OVERLAYEX)->VO/VENC(OVERLAY)->file     \n");
    printf("\t q) quit the whole sample\n");
    printf("sample command:");
    return;
}


/******************************************************************************
* function : to process abnormal case                                        
******************************************************************************/
HI_VOID SAMPLE_RGN_HandleSig(HI_S32 signo)
{
    static int sig_handled = 0;    
    signal(SIGINT, SIG_IGN);
    if (!sig_handled &&(SIGINT == signo || SIGTERM == signo))
    {
        sig_handled = 1;
        bExit = HI_TRUE;

        if (g_stVencOsdReverseThread)
        {
            pthread_join(g_stVencOsdReverseThread, 0);
            g_stVencOsdReverseThread = 0;
        }
        if (g_stVpssOsdReverseThread)
        {
            pthread_join(g_stVpssOsdReverseThread, 0);
            g_stVpssOsdReverseThread = 0;
        }
        if (g_stVencRgnThread)
        {
            pthread_join(g_stVencRgnThread, 0);
            g_stVencRgnThread = 0;
        }
        if (g_stVpssRgnThread)
        {
            pthread_join(g_stVpssRgnThread, 0);
            g_stVpssRgnThread = 0;
        }        
        if (g_stVencThread)
        {
            pthread_join(g_stVencThread, 0);
            g_stVencThread = 0;
        }
        if (g_stRgnThread)
        {
            pthread_join(g_stRgnThread, 0);
            g_stRgnThread = 0;
        }
        
        //HI_MPI_RGN_Destroy(gs_s32RgnCntCur);
        SAMPLE_COMM_ISP_Stop();
        SAMPLE_COMM_SYS_Exit();
        printf("\033[0;31mprogram exit abnormally!\033[0;39m\n");
    }
    sig_handled = 0;
    exit(0);
}
    
/******************************************************************************
* funciton : osd region change color
******************************************************************************/
HI_S32 SAMPLE_RGN_ChgColor(RGN_HANDLE RgnHandle, HI_U32 u32Color)
{
    HI_S32 s32Ret;
    RGN_ATTR_S stRgnAttr;

    s32Ret = HI_MPI_RGN_GetAttr(RgnHandle, &stRgnAttr);
    if(HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_RGN_GetAttr (%d)) failed with %#x!\n", RgnHandle, s32Ret);
        return HI_FAILURE;
    }

    stRgnAttr.unAttr.stOverlay.u32BgColor = u32Color;

    s32Ret = HI_MPI_RGN_SetAttr(RgnHandle, &stRgnAttr);
    if(HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_RGN_SetAttr (%d)) failed with %#x!\n", RgnHandle, s32Ret);
        return HI_FAILURE;
    }
    
    return HI_SUCCESS;
}
 
/******************************************************************************
* funciton : load bmp from file
******************************************************************************/
HI_S32 SAMPLE_RGN_LoadBmp(const char *filename, BITMAP_S *pstBitmap, HI_BOOL bFil, HI_U32 u16FilColor)
{
    OSD_SURFACE_S Surface;
    OSD_BITMAPFILEHEADER bmpFileHeader;
    OSD_BITMAPINFO bmpInfo;
    HI_U32 u32BytePerPix = 0;
    
    if(GetBmpInfo(filename, &bmpFileHeader, &bmpInfo) < 0)
    {
		printf("GetBmpInfo err!\n");
        return HI_FAILURE;
    }

    Surface.enColorFmt = OSD_COLOR_FMT_RGB1555;
    u32BytePerPix      = 2;
    
    pstBitmap->pData = malloc(u32BytePerPix * (bmpInfo.bmiHeader.biWidth) * (bmpInfo.bmiHeader.biHeight));
	
    if(NULL == pstBitmap->pData)
    {
        printf("malloc osd memroy err!\n");        
        return HI_FAILURE;
    }
    
    if (0 != CreateSurfaceByBitMap(filename, &Surface, (HI_U8*)(pstBitmap->pData)))
    {
    	free(pstBitmap->pData);
		pstBitmap->pData = HI_NULL;
		return HI_FAILURE;
	}
	
    pstBitmap->u32Width      = Surface.u16Width;
    pstBitmap->u32Height     = Surface.u16Height;
    pstBitmap->enPixelFormat = PIXEL_FORMAT_RGB_1555;
    
    
    int i,j;
    HI_U16 *pu16Temp;
    pu16Temp = (HI_U16*)pstBitmap->pData;
    
    if (bFil)
    {
        for (i=0; i<pstBitmap->u32Height; i++)
        {
            for (j=0; j<pstBitmap->u32Width; j++)
            {
                if (u16FilColor == *pu16Temp)
                {
                    *pu16Temp &= 0x7FFF;
                }

                pu16Temp++;
            }
        }

    }
        
    return HI_SUCCESS;
}


HI_S32 SAMPLE_RGN_UpdateCanvas(const char *filename, BITMAP_S *pstBitmap, HI_BOOL bFil, 
    HI_U32 u16FilColor, SIZE_S *pstSize, HI_U32 u32Stride, PIXEL_FORMAT_E enPixelFmt)
{
    OSD_SURFACE_S Surface;
    OSD_BITMAPFILEHEADER bmpFileHeader;
    OSD_BITMAPINFO bmpInfo;

    if(GetBmpInfo(filename, &bmpFileHeader, &bmpInfo) < 0)
    {
		printf("GetBmpInfo err!\n");
        return HI_FAILURE;
    }

    if (PIXEL_FORMAT_RGB_1555 == enPixelFmt)
    {
        Surface.enColorFmt = OSD_COLOR_FMT_RGB1555;
    }
    else if (PIXEL_FORMAT_RGB_4444 == enPixelFmt)
    {
        Surface.enColorFmt = OSD_COLOR_FMT_RGB4444;
    }
    else if (PIXEL_FORMAT_RGB_8888 == enPixelFmt)
    {
        Surface.enColorFmt = OSD_COLOR_FMT_RGB8888;
    }
    else
    {
        printf("Pixel format is not support!\n");        
        return HI_FAILURE;
    }
	
    if(NULL == pstBitmap->pData)
    {
        printf("malloc osd memroy err!\n");        
        return HI_FAILURE;
    }
    CreateSurfaceByCanvas(filename, &Surface, (HI_U8*)(pstBitmap->pData), pstSize->u32Width, pstSize->u32Height, u32Stride);
	
    pstBitmap->u32Width  = Surface.u16Width;
    pstBitmap->u32Height = Surface.u16Height;
    
    if (PIXEL_FORMAT_RGB_1555 == enPixelFmt)
    {
        pstBitmap->enPixelFormat = PIXEL_FORMAT_RGB_1555;
    }
    else if (PIXEL_FORMAT_RGB_4444 == enPixelFmt)
    {
        pstBitmap->enPixelFormat = PIXEL_FORMAT_RGB_4444;
    }
    else if (PIXEL_FORMAT_RGB_8888 == enPixelFmt)
    {
        pstBitmap->enPixelFormat = PIXEL_FORMAT_RGB_8888;
    }

    int i,j;
    HI_U16 *pu16Temp;
    pu16Temp = (HI_U16*)pstBitmap->pData;
    
    if (bFil)
    {
        for (i=0; i<pstBitmap->u32Height; i++)
        {
            for (j=0; j<pstBitmap->u32Width; j++)
            {
                if (u16FilColor == *pu16Temp)
                {
                    *pu16Temp &= 0x7FFF;
                }

                pu16Temp++;
            }
        }

    }
        
    return HI_SUCCESS;
}


HI_S32 SAMPLE_RGN_ConvOsdCavasToTdeSurface(TDE2_SURFACE_S *pstSurface, const RGN_CANVAS_INFO_S *pstCanvasInfo)
{
    HI_ASSERT((NULL != pstSurface) && (NULL != pstCanvasInfo));
    
    switch (pstCanvasInfo->enPixelFmt)
    {
        case PIXEL_FORMAT_RGB_4444:
        {
            pstSurface->enColorFmt = TDE2_COLOR_FMT_ARGB4444;
            break ;
        }
        case PIXEL_FORMAT_RGB_1555:
        {
            pstSurface->enColorFmt = TDE2_COLOR_FMT_ARGB1555;
            break ;
        }
        case PIXEL_FORMAT_RGB_8888:
        {
            pstSurface->enColorFmt = TDE2_COLOR_FMT_ARGB8888;
            break ;
        }
        default :
        {
            printf("[Func]:%s [Line]:%d [Info]:invalid Osd pixel format(%d)\n", 
                __FUNCTION__, __LINE__, pstCanvasInfo->enPixelFmt);
            return HI_FAILURE;
        }
    }

    pstSurface->bAlphaExt1555 = HI_FALSE;
    pstSurface->bAlphaMax255  = HI_TRUE;
    pstSurface->u32PhyAddr    = pstCanvasInfo->u32PhyAddr;
    pstSurface->u32Width      = pstCanvasInfo->stSize.u32Width;
    pstSurface->u32Height     = pstCanvasInfo->stSize.u32Height;
    pstSurface->u32Stride     = pstCanvasInfo->u32Stride;

    return HI_SUCCESS;
}


HI_S32 SAMPLE_RGN_AddReverseColorTask(TDE_HANDLE handle, 
    TDE2_SURFACE_S *pstForeGround, TDE2_RECT_S *pstForeGroundRect, 
    TDE2_SURFACE_S *pstBackGround, TDE2_RECT_S *pstBackGroundRect)
{
    HI_S32 s32Ret;
    TDE2_OPT_S stOpt = {0};
    
    HI_ASSERT(NULL != pstForeGround);
    HI_ASSERT(NULL != pstForeGroundRect);
    HI_ASSERT(NULL != pstBackGround);
    HI_ASSERT(NULL != pstBackGroundRect);

    stOpt.enAluCmd        = TDE2_ALUCMD_ROP;
    stOpt.enRopCode_Alpha = TDE2_ROP_COPYPEN;
    stOpt.enRopCode_Color = TDE2_ROP_NOT;
    
    s32Ret =  HI_TDE2_Bitblit(handle, pstBackGround, pstBackGroundRect, pstForeGround, 
            pstForeGroundRect, pstBackGround, pstBackGroundRect, &stOpt);
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_TDE2_Bitblit fail! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }

    return HI_SUCCESS;
    
}


HI_S32 SAMPLE_RGN_ReverseOsdColor(TDE2_SURFACE_S *pstSrcSurface, TDE2_SURFACE_S *pstDstSurface, 
    const VPSS_REGION_INFO_S *pstRgnInfo)
{
    HI_S32 i;
    HI_S32 s32Ret;
    TDE_HANDLE handle;
    TDE2_RECT_S stRect;
    

    HI_ASSERT(NULL != pstSrcSurface);
    HI_ASSERT(NULL != pstDstSurface);
    HI_ASSERT(NULL != pstRgnInfo);

    s32Ret = HI_TDE2_Open();
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_TDE2_Open fail! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }
    
    handle = HI_TDE2_BeginJob();
    if (handle < 0)
    {
        printf("HI_TDE2_BeginJob fail!\n");
        return HI_FAILURE;
    }

    stRect.s32Xpos = 0;
    stRect.s32Ypos = 0;
    stRect.u32Width  = pstSrcSurface->u32Width;
    stRect.u32Height = pstSrcSurface->u32Height;
    s32Ret = HI_TDE2_QuickCopy(handle, pstSrcSurface, &stRect, pstDstSurface, &stRect);
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_TDE2_QuickCopy fail! s32Ret: 0x%x.\n", s32Ret);
        HI_TDE2_CancelJob(handle);
        return s32Ret;
    }
    
    for (i = 0; i < pstRgnInfo->u32RegionNum; ++i)
    {
        stRect.s32Xpos   = pstRgnInfo->pstRegion[i].s32X;
        stRect.s32Ypos   = pstRgnInfo->pstRegion[i].s32Y;
        stRect.u32Width  = pstRgnInfo->pstRegion[i].u32Width;
        stRect.u32Height = pstRgnInfo->pstRegion[i].u32Height;
        
        s32Ret = SAMPLE_RGN_AddReverseColorTask(handle, pstSrcSurface, &stRect, pstDstSurface, &stRect);
        if (HI_SUCCESS != s32Ret)
        {
            printf("SAMPLE_RGN_AddReverseColorTask fail! s32Ret: 0x%x.\n", s32Ret);
            HI_TDE2_CancelJob(handle);
            return s32Ret;
        }
    }
    
    s32Ret = HI_TDE2_EndJob(handle, HI_FALSE, HI_FALSE, 10);
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_TDE2_EndJob fail! s32Ret: 0x%x.\n", s32Ret);
        HI_TDE2_CancelJob(handle);
        return s32Ret;
    }
    
    s32Ret = HI_TDE2_WaitForDone(handle);
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_TDE2_WaitForDone fail! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }
     
    return HI_SUCCESS;
}


HI_S32 SAMPLE_RGN_GetVoDisplayNum(HI_U32 u32VoChnNum)
{
    HI_S32 s32DispNum;

    if (1 == u32VoChnNum)
    {
        s32DispNum = 1;
    }
    else if (4 == u32VoChnNum)
    {
        s32DispNum = 2;
    }
    else if (9 == u32VoChnNum)
    {
        s32DispNum = 3;
    }
    else if (16 == u32VoChnNum)
    {
        s32DispNum = 4;
    }
    else
    {
        return -1;
    }

    return s32DispNum;
}


HI_S32 SAMPLE_RGN_GetVoLayer(VO_DEV VoDev)
{
    HI_S32 s32LayerNum;
    
    if (0 == VoDev)
    {
        s32LayerNum = 0;
    }
    else
    {
        return -1;
    }

    return s32LayerNum;
}

HI_S32 SAMPLE_RGN_CreateCover(RGN_HANDLE Handle, HI_U32 u32Num)
{
    HI_S32 i;
    HI_S32 s32Ret;
    MPP_CHN_S stChn;
    RGN_ATTR_S stRgnAttr;
    RGN_CHN_ATTR_S stChnAttr;

    /* Add cover to vpss group */
    stChn.enModId  = HI_ID_VPSS;
    stChn.s32DevId = 0;
    stChn.s32ChnId = 0;
        
    /* Create cover and attach to vpss group */
    for (i=Handle; i<(Handle + u32Num); i++)
    {
        stRgnAttr.enType = COVER_RGN;

        s32Ret = HI_MPI_RGN_Create(i, &stRgnAttr);
        if(s32Ret != HI_SUCCESS)
        {
            printf("HI_MPI_RGN_Create fail! s32Ret: 0x%x.\n", s32Ret);
            return s32Ret;
        }

        stChnAttr.bShow  = HI_TRUE;
        stChnAttr.enType = COVER_RGN;
        if (i < 4)
        {
            stChnAttr.unChnAttr.stCoverChn.enCoverType = AREA_RECT;
            stChnAttr.unChnAttr.stCoverChn.stRect.s32X      = 10 * (i - Handle);
            stChnAttr.unChnAttr.stCoverChn.stRect.s32Y      = 10 * (i - Handle);
            stChnAttr.unChnAttr.stCoverChn.stRect.u32Height = 64 * (i - Handle + 1);
            stChnAttr.unChnAttr.stCoverChn.stRect.u32Width  = 64 * (i - Handle + 1);
        }
        else
        {
            stChnAttr.unChnAttr.stCoverChn.enCoverType = AREA_QUAD_RANGLE;
            stChnAttr.unChnAttr.stCoverChn.stQuadRangle.bSolid = HI_TRUE;
            stChnAttr.unChnAttr.stCoverChn.stQuadRangle.u32Thick = 2;
            stChnAttr.unChnAttr.stCoverChn.stQuadRangle.stPoint[0].s32X = 50 * i;
            stChnAttr.unChnAttr.stCoverChn.stQuadRangle.stPoint[0].s32Y = 0;
            stChnAttr.unChnAttr.stCoverChn.stQuadRangle.stPoint[1].s32X = 50 + 50 * i;
            stChnAttr.unChnAttr.stCoverChn.stQuadRangle.stPoint[1].s32Y = 50;
            stChnAttr.unChnAttr.stCoverChn.stQuadRangle.stPoint[2].s32X = 50 + 50 * i;
            stChnAttr.unChnAttr.stCoverChn.stQuadRangle.stPoint[2].s32Y = 300;
            stChnAttr.unChnAttr.stCoverChn.stQuadRangle.stPoint[3].s32X = 50 * i;
            stChnAttr.unChnAttr.stCoverChn.stQuadRangle.stPoint[3].s32Y = 200;
        }
        stChnAttr.unChnAttr.stCoverChn.u32Color         = 0x000000ff;
        if (1 == i%COVER_MAX_NUM_VPSS)
        {
            stChnAttr.unChnAttr.stCoverChn.u32Color     = 0x0000ff00;
        }
        else if (2 == i%COVER_MAX_NUM_VPSS)
        {
            stChnAttr.unChnAttr.stCoverChn.u32Color     = 0x00ff0000;
        }
        else if (3 == i%COVER_MAX_NUM_VPSS)
        {
            stChnAttr.unChnAttr.stCoverChn.u32Color     = 0x00808080;
        }
        stChnAttr.unChnAttr.stCoverChn.u32Layer         = i - Handle;
        
        s32Ret = HI_MPI_RGN_AttachToChn(i, &stChn, &stChnAttr);
        if(s32Ret != HI_SUCCESS)
        {
            printf("HI_MPI_RGN_AttachToChn fail! s32Ret: 0x%x. w %d, H %d\n", s32Ret, \
                stChnAttr.unChnAttr.stCoverChn.stRect.u32Height, stChnAttr.unChnAttr.stCoverChn.stRect.u32Width);
            return s32Ret;
        }
    }

    return HI_SUCCESS;
    
}

HI_S32 SAMPLE_RGN_CreateCoverEx(RGN_HANDLE Handle, HI_U32 u32Num)
{
    HI_S32 i;
    HI_S32 s32Ret;
    MPP_CHN_S stChn;
    RGN_ATTR_S stRgnAttr;
    RGN_CHN_ATTR_S stChnAttr;
    
    /* Add cover to vpss group */
    stChn.enModId  = HI_ID_VPSS;
    stChn.s32DevId = 0;
    stChn.s32ChnId = VPSS_CHN0;
        
    /* Create cover and attach to vpss group */
    for (i=Handle; i<(Handle + u32Num); i++)
    {
        stRgnAttr.enType = COVEREX_RGN;

        s32Ret = HI_MPI_RGN_Create(i, &stRgnAttr);
        if(s32Ret != HI_SUCCESS)
        {
            printf("HI_MPI_RGN_Create fail! s32Ret: 0x%x.\n", s32Ret);
            return s32Ret;
        }

        stChnAttr.bShow  = HI_TRUE;
        stChnAttr.enType = COVEREX_RGN;
        if (i < 4)
        {
            stChnAttr.unChnAttr.stCoverExChn.enCoverType = AREA_RECT;
            stChnAttr.unChnAttr.stCoverExChn.stRect.s32X      = 10 * (i - Handle);
            stChnAttr.unChnAttr.stCoverExChn.stRect.s32Y      = 10 * (i - Handle);
            stChnAttr.unChnAttr.stCoverExChn.stRect.u32Height = 64 * (i - Handle + 1);
            stChnAttr.unChnAttr.stCoverExChn.stRect.u32Width  = 64 * (i - Handle + 1);
        }
        else
        {
            stChnAttr.unChnAttr.stCoverExChn.enCoverType = AREA_QUAD_RANGLE;
            stChnAttr.unChnAttr.stCoverExChn.stQuadRangle.bSolid = HI_TRUE;
            stChnAttr.unChnAttr.stCoverExChn.stQuadRangle.u32Thick = 2;
            stChnAttr.unChnAttr.stCoverExChn.stQuadRangle.stPoint[0].s32X = 50 * i;
            stChnAttr.unChnAttr.stCoverExChn.stQuadRangle.stPoint[0].s32Y = 0;
            stChnAttr.unChnAttr.stCoverExChn.stQuadRangle.stPoint[1].s32X = 50 + 50 * i;
            stChnAttr.unChnAttr.stCoverExChn.stQuadRangle.stPoint[1].s32Y = 50;
            stChnAttr.unChnAttr.stCoverExChn.stQuadRangle.stPoint[2].s32X = 50 + 50 * i;
            stChnAttr.unChnAttr.stCoverExChn.stQuadRangle.stPoint[2].s32Y = 300;
            stChnAttr.unChnAttr.stCoverExChn.stQuadRangle.stPoint[3].s32X = 50 * i;
            stChnAttr.unChnAttr.stCoverExChn.stQuadRangle.stPoint[3].s32Y = 200;
        }
        stChnAttr.unChnAttr.stCoverExChn.u32Color         = 0x000000ff;
        if (1 == i%COVEREX_MAX_NUM_VPSS)
        {
            stChnAttr.unChnAttr.stCoverExChn.u32Color     = 0x0000ff00;
        }
        else if (2 == i%COVEREX_MAX_NUM_VPSS)
        {
            stChnAttr.unChnAttr.stCoverExChn.u32Color     = 0x00ff0000;
        }
        else if (3 == i%COVEREX_MAX_NUM_VPSS)
        {
            stChnAttr.unChnAttr.stCoverExChn.u32Color     = 0x00808080;
        }
        stChnAttr.unChnAttr.stCoverExChn.u32Layer         = i - Handle;
        
        s32Ret = HI_MPI_RGN_AttachToChn(i, &stChn, &stChnAttr);
        if(s32Ret != HI_SUCCESS)
        {
            printf("HI_MPI_RGN_AttachToChn fail! s32Ret: 0x%x.\n", s32Ret);
            return s32Ret;
        }
    }

    return HI_SUCCESS;
    
}

HI_S32 SAMPLE_RGN_CreateOverlayExForVpss(RGN_HANDLE Handle, HI_U32 u32Num)
{
    HI_S32 i;
    HI_S32 s32Ret;
    MPP_CHN_S stChn;
    HI_U32 u32layer = 0;
    RGN_ATTR_S stRgnAttrSet;
    RGN_CHN_ATTR_S stChnAttr;

    /*attach the OSD to the vpss*/
    stChn.enModId  = HI_ID_VPSS;
    stChn.s32DevId = 0;
    stChn.s32ChnId = VPSS_CHN0;
        
    for (i=Handle; i<(Handle + u32Num); i++)    
    {
        stRgnAttrSet.enType = OVERLAYEX_RGN;
        stRgnAttrSet.unAttr.stOverlayEx.enPixelFmt       = PIXEL_FORMAT_RGB_1555;
        stRgnAttrSet.unAttr.stOverlayEx.stSize.u32Width  = 300;
        stRgnAttrSet.unAttr.stOverlayEx.stSize.u32Height = 300;
        stRgnAttrSet.unAttr.stOverlayEx.u32BgColor       = 0x000003e0;
        
        if (1 == i%u32Num)
        {
            stRgnAttrSet.unAttr.stOverlayEx.stSize.u32Width  = 180;
            stRgnAttrSet.unAttr.stOverlayEx.stSize.u32Height = 120;
            stRgnAttrSet.unAttr.stOverlayEx.u32BgColor       = 0x0000001f; 
        }
        else if (2 == i%u32Num)
        {
            stRgnAttrSet.unAttr.stOverlayEx.stSize.u32Width  = 160;
            stRgnAttrSet.unAttr.stOverlayEx.stSize.u32Height = 120;
            stRgnAttrSet.unAttr.stOverlayEx.u32BgColor       = 0x00007c00;     
        }
        else if (3 == i%u32Num)
        {
            stRgnAttrSet.unAttr.stOverlayEx.stSize.u32Width  = 100;
            stRgnAttrSet.unAttr.stOverlayEx.stSize.u32Height = 150;
            stRgnAttrSet.unAttr.stOverlayEx.u32BgColor       = 0x000007ff;   
        }

        s32Ret = HI_MPI_RGN_Create(i, &stRgnAttrSet);
        if(s32Ret != HI_SUCCESS)
        {
            printf("HI_MPI_RGN_Create failed! s32Ret: 0x%x.\n", s32Ret);
            return s32Ret;
        }

        stChnAttr.bShow  = HI_TRUE;
        stChnAttr.enType = OVERLAYEX_RGN;
        stChnAttr.unChnAttr.stOverlayExChn.stPoint.s32X = 48;
        stChnAttr.unChnAttr.stOverlayExChn.stPoint.s32Y = 48;
        stChnAttr.unChnAttr.stOverlayExChn.u32BgAlpha   = 255;
        stChnAttr.unChnAttr.stOverlayExChn.u32FgAlpha   = 255;
        stChnAttr.unChnAttr.stOverlayExChn.u32Layer     = u32layer;
        u32layer++;
        if (1 == i%4)
        {
            stChnAttr.unChnAttr.stOverlayExChn.stPoint.s32X = 130;
            stChnAttr.unChnAttr.stOverlayExChn.stPoint.s32Y = 330;
        }
        else if (2 == i%4)
        {
            stChnAttr.unChnAttr.stOverlayExChn.stPoint.s32X = 270;
            stChnAttr.unChnAttr.stOverlayExChn.stPoint.s32Y = 300;
        }
        else if (3 == i%4)
        {
            stChnAttr.unChnAttr.stOverlayExChn.stPoint.s32X = 180;
            stChnAttr.unChnAttr.stOverlayExChn.stPoint.s32Y = 400;
        }
        
        s32Ret = HI_MPI_RGN_AttachToChn(i, &stChn, &stChnAttr);
        if(s32Ret != HI_SUCCESS)
        {
            printf("HI_MPI_RGN_AttachToChn failed! s32Ret: 0x%x.\n", s32Ret);
            return s32Ret;
        }
    }
    
    return HI_SUCCESS;
    
}


HI_S32 SAMPLE_RGN_CreateOverlayForVenc(RGN_HANDLE Handle, HI_U32 u32Num)
{
    HI_S32 i;
    HI_S32 s32Ret;
    MPP_CHN_S stChn;
    RGN_ATTR_S stRgnAttr;
    RGN_CHN_ATTR_S stChnAttr;
    
    /* Add cover to vpss group */
    stChn.enModId  = HI_ID_VENC;
    stChn.s32DevId = 0;
    stChn.s32ChnId = 0;
        
    for (i=Handle; i<(Handle + u32Num); i++)
    {
        stRgnAttr.enType = OVERLAY_RGN;
        stRgnAttr.unAttr.stOverlay.enPixelFmt       = PIXEL_FORMAT_RGB_1555;
        stRgnAttr.unAttr.stOverlay.stSize.u32Width  = 300;
        stRgnAttr.unAttr.stOverlay.stSize.u32Height = 300;
        stRgnAttr.unAttr.stOverlay.u32BgColor       = 0x000003e0;

        if (1 == i%u32Num)
        {
            stRgnAttr.unAttr.stOverlay.stSize.u32Width  = 180;
            stRgnAttr.unAttr.stOverlay.stSize.u32Height = 120;
            stRgnAttr.unAttr.stOverlay.u32BgColor       = 0x0000001f; 
        }
        else if (2 == i%u32Num)
        {
            stRgnAttr.unAttr.stOverlay.stSize.u32Width  = 160;
            stRgnAttr.unAttr.stOverlay.stSize.u32Height = 120;
            stRgnAttr.unAttr.stOverlay.u32BgColor       = 0x00007c00;     
        }
        else if (3 == i%u32Num)
        {
            stRgnAttr.unAttr.stOverlay.stSize.u32Width  = 100;
            stRgnAttr.unAttr.stOverlay.stSize.u32Height = 160;
            stRgnAttr.unAttr.stOverlay.u32BgColor       = 0x000007ff;   
        }

        s32Ret = HI_MPI_RGN_Create(i, &stRgnAttr);
        if(s32Ret != HI_SUCCESS)
        {
            printf("HI_MPI_RGN_Create failed! s32Ret: 0x%x.\n", s32Ret);
            return s32Ret;
        }
    
        stChnAttr.bShow  = HI_TRUE;
        stChnAttr.enType = OVERLAY_RGN;
        stChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = 48;
        stChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = 48;
        stChnAttr.unChnAttr.stOverlayChn.u32BgAlpha   = 128;
        stChnAttr.unChnAttr.stOverlayChn.u32FgAlpha   = 128;
        stChnAttr.unChnAttr.stOverlayChn.u32Layer     = i;
        
        stChnAttr.unChnAttr.stOverlayChn.stQpInfo.bAbsQp = HI_FALSE;
        stChnAttr.unChnAttr.stOverlayChn.stQpInfo.s32Qp  = 0;
        stChnAttr.unChnAttr.stOverlayChn.stQpInfo.bQpDisable  = HI_FALSE;

        stChnAttr.unChnAttr.stOverlayChn.stInvertColor.stInvColArea.u32Height = 16*(i%2+1);
        stChnAttr.unChnAttr.stOverlayChn.stInvertColor.stInvColArea.u32Width  = 16*(i%2+1);
        stChnAttr.unChnAttr.stOverlayChn.stInvertColor.u32LumThresh = 128;
        stChnAttr.unChnAttr.stOverlayChn.stInvertColor.enChgMod     = LESSTHAN_LUM_THRESH;
        stChnAttr.unChnAttr.stOverlayChn.stInvertColor.bInvColEn    = HI_FALSE;
        if (1 == i%u32Num)
        {
            stChnAttr.unChnAttr.stOverlayExChn.stPoint.s32X = 128;
            stChnAttr.unChnAttr.stOverlayExChn.stPoint.s32Y = 320;
        }
        else if (2 == i%u32Num)
        {
            stChnAttr.unChnAttr.stOverlayExChn.stPoint.s32X = 280;
            stChnAttr.unChnAttr.stOverlayExChn.stPoint.s32Y = 240;
        }
        else if (3 == i%u32Num)
        {
            stChnAttr.unChnAttr.stOverlayExChn.stPoint.s32X = 180;
            stChnAttr.unChnAttr.stOverlayExChn.stPoint.s32Y = 400;
        }
        s32Ret = HI_MPI_RGN_AttachToChn(i, &stChn, &stChnAttr);
        if(s32Ret != HI_SUCCESS)
        {
            SAMPLE_RGN_NOT_PASS(s32Ret);
        }
    } 

    return HI_SUCCESS;
    
}


HI_S32 SAMPLE_RGN_GetVoAttr(VO_DEV VoDev, VO_INTF_SYNC_E enIntfSync, 
        VO_VIDEO_LAYER_ATTR_S *pstLayerAttr, HI_S32 s32SquareSort, VO_CHN_ATTR_S *astChnAttr)
{    
    HI_U32 u32Frmt, u32Width, u32Height, j;

    switch (enIntfSync)
    {
        case VO_OUTPUT_PAL      :    u32Width = 720;  u32Height = 576;  u32Frmt = 25; break;
        case VO_OUTPUT_NTSC     :    u32Width = 720;  u32Height = 480;  u32Frmt = 30; break;
        case VO_OUTPUT_1080P24  :    u32Width = 1920; u32Height = 1080; u32Frmt = 24; break;
        case VO_OUTPUT_1080P25  :    u32Width = 1920; u32Height = 1080; u32Frmt = 25; break;
        case VO_OUTPUT_1080P30  :    u32Width = 1920; u32Height = 1080; u32Frmt = 30; break;
        case VO_OUTPUT_720P50   :    u32Width = 1280; u32Height = 720;  u32Frmt = 50; break;
        case VO_OUTPUT_720P60   :    u32Width = 1280; u32Height = 720;  u32Frmt = 60; break;
        case VO_OUTPUT_1080I50  :    u32Width = 1920; u32Height = 1080; u32Frmt = 50; break;
        case VO_OUTPUT_1080I60  :    u32Width = 1920; u32Height = 1080; u32Frmt = 60; break;
        case VO_OUTPUT_1080P50  :    u32Width = 1920; u32Height = 1080; u32Frmt = 50; break;
        case VO_OUTPUT_1080P60  :    u32Width = 1920; u32Height = 1080; u32Frmt = 60; break;
        case VO_OUTPUT_576P50   :    u32Width = 720;  u32Height = 576;  u32Frmt = 50; break;
        case VO_OUTPUT_480P60   :    u32Width = 720;  u32Height = 480;  u32Frmt = 60; break;
        case VO_OUTPUT_800x600_60:   u32Width = 800;  u32Height = 600;  u32Frmt = 60; break;
        case VO_OUTPUT_1024x768_60:  u32Width = 1024; u32Height = 768;  u32Frmt = 60; break;
        case VO_OUTPUT_1280x1024_60: u32Width = 1280; u32Height = 1024; u32Frmt = 60; break;
        case VO_OUTPUT_1366x768_60:  u32Width = 1366; u32Height = 768;  u32Frmt = 60; break;
        case VO_OUTPUT_1440x900_60:  u32Width = 1440; u32Height = 900;  u32Frmt = 60; break;
        case VO_OUTPUT_1280x800_60:  u32Width = 1280; u32Height = 800;  u32Frmt = 60; break;
		case VO_OUTPUT_240X320_50:	 u32Width = 240; u32Height = 320;  u32Frmt = 50; break;
		case VO_OUTPUT_320X240_50:	 u32Width = 320; u32Height = 240;  u32Frmt = 50; break;
		case VO_OUTPUT_320X240_60:	 u32Width = 320; u32Height = 240;  u32Frmt = 30; break;

        default: return HI_FAILURE;
    }

    if (NULL != pstLayerAttr)
    {
        pstLayerAttr->stDispRect.s32X       = 0;
        pstLayerAttr->stDispRect.s32Y       = 0;
        pstLayerAttr->stDispRect.u32Width   = u32Width;
        pstLayerAttr->stDispRect.u32Height  = u32Height;
        pstLayerAttr->stImageSize.u32Width  = u32Width;
        pstLayerAttr->stImageSize.u32Height = u32Height;
        pstLayerAttr->bDoubleFrame          = HI_FALSE;
        pstLayerAttr->bClusterMode          = HI_FALSE;
        pstLayerAttr->u32DispFrmRt          = u32Frmt;
        pstLayerAttr->enPixFormat           = PIXEL_FORMAT_YUV_SEMIPLANAR_420;
    }

    if (NULL != astChnAttr)
    {
        for (j=0; j<(s32SquareSort * s32SquareSort); j++)
        {
            astChnAttr[j].stRect.s32X       = ALIGN_BACK((u32Width / s32SquareSort) * (j % s32SquareSort), 4);
            astChnAttr[j].stRect.s32Y       = ALIGN_BACK((u32Height / s32SquareSort) * (j / s32SquareSort), 4);
            astChnAttr[j].stRect.u32Width   = ALIGN_BACK(u32Width / s32SquareSort, 4);
            astChnAttr[j].stRect.u32Height  = ALIGN_BACK(u32Height / s32SquareSort, 4);
            astChnAttr[j].u32Priority       = 0;
            astChnAttr[j].bDeflicker        = HI_FALSE;
        }
    }
    
    return HI_SUCCESS;
}

HI_S32 SAMPLE_RGN_StartVpss(HI_S32 s32VpssGrpNum, HI_U32 u32VpssChnNum)
{
    HI_S32 i = 0;
    HI_S32 s32Ret;
    VPSS_CHN_MODE_S stVpssChnMode;
    VPSS_CHN VpssChn;
 	VPSS_GRP_ATTR_S stGrpAttr;
    VPSS_CHN_ATTR_S stVpssChnAttr;
    PIC_SIZE_E enPicSize = g_enPicSize;
    SIZE_S stSize;

    SAMPLE_COMM_SYS_GetPicSize(gs_enNorm, enPicSize, &stSize);
    memset(&stVpssChnAttr,0,sizeof(VPSS_CHN_ATTR_S));
    
    stGrpAttr.u32MaxW   = stSize.u32Width;
    stGrpAttr.u32MaxH   = stSize.u32Height;
    stGrpAttr.enPixFmt  = PIXEL_FORMAT_YUV_SEMIPLANAR_420;
    stGrpAttr.enDieMode = VPSS_DIE_MODE_NODIE;
    stGrpAttr.bIeEn     = HI_FALSE;
    stGrpAttr.bNrEn     = HI_FALSE;
    stGrpAttr.bHistEn   = HI_FALSE;
    stGrpAttr.bDciEn    = HI_FALSE;
	
    for (i = 0; i < s32VpssGrpNum; i++)
    {
        s32Ret = HI_MPI_VPSS_CreateGrp(i, &stGrpAttr);
        if (HI_SUCCESS != s32Ret)
        {
            printf("create vpss grp%d fail, s32Ret: 0x%x.\n", i, s32Ret);
            return s32Ret;

        }
    
		stVpssChnMode.bDouble 	     = HI_FALSE;
		stVpssChnMode.enChnMode 	 = VPSS_CHN_MODE_USER;
		stVpssChnMode.enPixelFormat  = PIXEL_FORMAT_YUV_SEMIPLANAR_420;
		stVpssChnMode.u32Width 	     = 720;
		stVpssChnMode.u32Height 	 = 576;
        stVpssChnMode.enCompressMode = COMPRESS_MODE_NONE;

        stVpssChnAttr.bBorderEn = HI_FALSE;
        stVpssChnAttr.bSpEn = HI_FALSE;    
        stVpssChnAttr.bFlip = HI_FALSE;
        stVpssChnAttr.bMirror = HI_FALSE;
        stVpssChnAttr.s32SrcFrameRate = -1;
        stVpssChnAttr.s32DstFrameRate = -1;

        for (VpssChn = 0; VpssChn < u32VpssChnNum; VpssChn++)
        {
            if (0 == VpssChn)
            {
                stVpssChnMode.u32Width 	     = stSize.u32Width;
		        stVpssChnMode.u32Height 	 = stSize.u32Height;
            }
			else
			{
				stVpssChnMode.u32Width 	     = 720;
				stVpssChnMode.u32Height 	 = 576;
			}
            
            s32Ret = HI_MPI_VPSS_SetChnMode(i, VpssChn, &stVpssChnMode);
            if (HI_SUCCESS != s32Ret)
            {
                printf("set vpss grp%d chn%d mode fail, s32Ret: 0x%x.\n", i, VpssChn, s32Ret);
                return s32Ret;
            }
            
            s32Ret = HI_MPI_VPSS_SetChnAttr(i, VpssChn, &stVpssChnAttr);
            if (HI_SUCCESS != s32Ret)
            {
                printf("set vpss grp%d chn%d Attr fail, s32Ret: 0x%x.\n", i, VpssChn, s32Ret);
                return s32Ret;
            }

            s32Ret = HI_MPI_VPSS_EnableChn(i, VpssChn);
            if (HI_SUCCESS != s32Ret)
            {
                printf("create vpss grp%d chnl%d fail, s32Ret: 0x%x.\n", i, VpssChn, s32Ret);
                return s32Ret;
            }
        }
        
        s32Ret = HI_MPI_VPSS_StartGrp(i);
        if (HI_SUCCESS != s32Ret)
        {
            printf("start vpss grp%d fail, s32Ret: 0x%x.\n", i, s32Ret);
            return s32Ret;
        }
    }

    return HI_SUCCESS;
}

HI_S32 SAMPLE_RGN_StartVpssHD(HI_S32 s32VpssGrpNum, HI_U32 u32VpssChnNum)
{
    HI_S32 i = 0;
    HI_S32 s32Ret;
    VPSS_CHN_MODE_S stVpssChnMode;
    VPSS_CHN VpssChn;
 	VPSS_GRP_ATTR_S stGrpAttr;
    VPSS_CHN_ATTR_S stVpssChnAttr;
    PIC_SIZE_E enPicSize = g_enPicSize;
    SIZE_S stSize;

    SAMPLE_COMM_SYS_GetPicSize(gs_enNorm, enPicSize, &stSize);

    memset(&stVpssChnAttr,0,sizeof(VPSS_CHN_ATTR_S));
    
    stGrpAttr.u32MaxW      = stSize.u32Width;
    stGrpAttr.u32MaxH      = stSize.u32Height;
    stGrpAttr.enPixFmt     = PIXEL_FORMAT_YUV_SEMIPLANAR_420;
    stGrpAttr.enDieMode    = VPSS_DIE_MODE_NODIE;
    stGrpAttr.bIeEn        = HI_FALSE;
    stGrpAttr.bNrEn        = HI_FALSE;
    stGrpAttr.bHistEn      = HI_FALSE;
    stGrpAttr.bDciEn       = HI_FALSE;
        	
    for (i = 0; i < s32VpssGrpNum; i++)
    {
        s32Ret = HI_MPI_VPSS_CreateGrp(i, &stGrpAttr);
        if (HI_SUCCESS != s32Ret)
        {
            printf("creat vpss grp%d fail! s32Ret: 0x%x.\n", i, s32Ret);
            return s32Ret;

        }

		stVpssChnMode.bDouble 	     = HI_FALSE;
		stVpssChnMode.enChnMode 	 = VPSS_CHN_MODE_USER;
		stVpssChnMode.enPixelFormat  = PIXEL_FORMAT_YUV_SEMIPLANAR_420;
		stVpssChnMode.u32Width 	     = stSize.u32Width;
		stVpssChnMode.u32Height 	 = stSize.u32Height;
        stVpssChnMode.enCompressMode = COMPRESS_MODE_NONE;
        
        stVpssChnAttr.bBorderEn = HI_FALSE;
        stVpssChnAttr.bSpEn     = HI_FALSE;    
        stVpssChnAttr.bFlip     = HI_FALSE;
        stVpssChnAttr.bMirror   = HI_FALSE;
        stVpssChnAttr.s32SrcFrameRate = -1;
        stVpssChnAttr.s32DstFrameRate = -1;

        for (VpssChn = 0; VpssChn < u32VpssChnNum; VpssChn++)
        {
			if (0 == VpssChn)
            {
                stVpssChnMode.u32Width 	     = stSize.u32Width;
		        stVpssChnMode.u32Height 	 = stSize.u32Height;
            }
			else
			{
				stVpssChnMode.u32Width 	     = 720;
				stVpssChnMode.u32Height 	 = 576;
			}
			
            s32Ret = HI_MPI_VPSS_SetChnMode(i, VpssChn, &stVpssChnMode);
            if (HI_SUCCESS != s32Ret)
            {
                printf("set vpss grp%d chn%d mode fail, s32Ret: 0x%x.\n", i, VpssChn, s32Ret);
                return s32Ret;
            }
            
            s32Ret = HI_MPI_VPSS_SetChnAttr(i, VpssChn, &stVpssChnAttr);
            if (HI_SUCCESS != s32Ret)
            {
                printf("set vpss grp%d chn%d Attr fail, s32Ret: 0x%x.\n", i, VpssChn, s32Ret);
                return s32Ret;
            }

            s32Ret = HI_MPI_VPSS_EnableChn(i, VpssChn);
            if (HI_SUCCESS != s32Ret)
            {
                printf("create vpss grp%d chnl%d fail, s32Ret: 0x%x.\n", i, VpssChn, s32Ret);
                return s32Ret;
            }
        }
        
        s32Ret = HI_MPI_VPSS_StartGrp(i);
        if (HI_SUCCESS != s32Ret)
        {
            printf("start vpss grp%d fail! s32Ret: 0x%x.\n", i, s32Ret);
            return s32Ret;
        }
    }

    return HI_SUCCESS;
}


HI_S32 SAMPLE_RGN_StopVpss(HI_S32 s32VpssGrpNum)
{
    HI_S32 i = 0;
    HI_S32 s32Ret;


    for (i = 0; i < s32VpssGrpNum; i++)
    {
        s32Ret =  HI_MPI_VPSS_StopGrp(i);
        if (HI_SUCCESS != s32Ret)
        {
            printf("stop vpss grp%d fail! s32Ret: 0x%x.\n", i, s32Ret);
            return s32Ret;
        }

        s32Ret =  HI_MPI_VPSS_DestroyGrp(i);
        if (HI_SUCCESS != s32Ret)
        {
            printf("destroy vpss grp%d fail! s32Ret: 0x%x.\n", i, s32Ret);
            return s32Ret;
        }
    }

    return HI_SUCCESS;
}

HI_S32 SAMPLE_RGN_StartVo(VO_DEV VoDev, HI_U32 u32VoChnNum, VO_PUB_ATTR_S *pstPubAttr)
{
    HI_S32 i;
    HI_S32 s32Ret;
    HI_S32 s32DispNum;
    VO_LAYER VoLayer;
    VO_VIDEO_LAYER_ATTR_S stLayerAttr;
    VO_CHN_ATTR_S astChnAttr[16];

    s32DispNum = SAMPLE_RGN_GetVoDisplayNum(u32VoChnNum);
    if(s32DispNum < 0)
    {
        printf("SAMPLE_RGN_GetVoDisplayNum failed! u32VoChnNum: %d.\n", u32VoChnNum);
        return HI_FAILURE;
    }
    
    s32Ret = SAMPLE_RGN_GetVoAttr(VoDev, pstPubAttr->enIntfSync, &stLayerAttr, s32DispNum, astChnAttr);
    if (HI_SUCCESS != s32Ret)
    {
        printf("SAMPLE_RGN_GetVoAttr failed!\n");
        return HI_FAILURE;
    }

    VoLayer = SAMPLE_RGN_GetVoLayer(VoDev);
    if(VoLayer < 0)
    {
        printf("SAMPLE_RGN_GetVoLayer failed! VoDev: %d.\n", VoDev);
        return HI_FAILURE;
    }
  
    s32Ret = HI_MPI_VO_Disable(VoDev);
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_VO_Disable failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }

    s32Ret = HI_MPI_VO_SetPubAttr(VoDev, pstPubAttr);
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_VO_SetPubAttr failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }

    s32Ret = HI_MPI_VO_Enable(VoDev);
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_VO_Enable failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }
  
    s32Ret = HI_MPI_VO_SetVideoLayerAttr(VoLayer, &stLayerAttr);
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_VO_SetVideoLayerAttr failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }
    
    s32Ret = HI_MPI_VO_EnableVideoLayer(VoLayer);
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_VO_EnableVideoLayer failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }

    for (i = 0; i < u32VoChnNum; i++)
    {
        s32Ret = HI_MPI_VO_SetChnAttr(VoLayer, i, &astChnAttr[i]);
        if (HI_SUCCESS != s32Ret)
        {
            printf("HI_MPI_VO_SetChnAttr failed! s32Ret: 0x%x.\n", s32Ret);
            return s32Ret;
        }

        s32Ret = HI_MPI_VO_EnableChn(VoLayer, i);
        if (HI_SUCCESS != s32Ret)
        {
            printf("HI_MPI_VO_EnableChn failed! s32Ret: 0x%x.\n", s32Ret);
            return s32Ret;
        }
    }
    
    return HI_SUCCESS;
}

HI_S32 SAMPLE_RGN_StopVoChn(VO_DEV VoDev, HI_U32 u32VoChnNum)
{
    HI_S32 i;
    HI_S32 s32Ret;
    VO_LAYER VoLayer;
    
    VoLayer = SAMPLE_RGN_GetVoLayer(VoDev);
    if(VoLayer < 0)
    {
        printf("SAMPLE_RGN_GetVoLayer failed! VoDev: %d.\n", VoDev);
        return HI_FAILURE;
    }
    
    for (i = 0; i< u32VoChnNum; i++)
    {
        s32Ret = HI_MPI_VO_DisableChn(VoLayer, i);
        if (HI_SUCCESS != s32Ret)
        {
            printf("HI_MPI_VO_DisableChn failed! s32Ret: 0x%x.\n", s32Ret);
            return s32Ret;
        }
    }
    
    return HI_SUCCESS;
}


HI_S32 SAMPLE_RGN_StopVoDev(VO_DEV VoDev)
{
    HI_S32 s32Ret;
    VO_LAYER VoLayer;

    VoLayer = SAMPLE_RGN_GetVoLayer(VoDev);
    if(VoLayer < 0)
    {
        printf("SAMPLE_RGN_GetVoLayer failed! VoDev: %d.\n", VoDev);
        return HI_FAILURE;
    }

    s32Ret = HI_MPI_VO_DisableVideoLayer(VoLayer);
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_VO_DisableVideoLayer failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }

    s32Ret = HI_MPI_VO_Disable(VoDev);
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_VO_Disable failed! s32Ret:0x%x.\n", s32Ret);
        return s32Ret;
    }
    
    return 0;
}

HI_S32 SAMPLE_RGN_StartVenc(VENC_CHN VencChn)
{
    HI_S32 s32Ret;
    HI_U32 u32PicWidth;
    HI_U32 u32PicHeight;
    VENC_CHN_ATTR_S stChnAttr;

    u32PicWidth  = 720;
    u32PicHeight = 576;
    stChnAttr.stVeAttr.enType               = PT_H264;
    stChnAttr.stVeAttr.stAttrH264e.bByFrame = HI_TRUE;
    
    stChnAttr.stVeAttr.stAttrH264e.u32MaxPicHeight = u32PicHeight;
    stChnAttr.stVeAttr.stAttrH264e.u32MaxPicWidth  = u32PicWidth;
    stChnAttr.stVeAttr.stAttrH264e.u32PicHeight    = u32PicHeight;
    stChnAttr.stVeAttr.stAttrH264e.u32PicWidth     = u32PicWidth;
    stChnAttr.stVeAttr.stAttrH264e.u32BufSize      = u32PicWidth * u32PicHeight * 2;
    stChnAttr.stVeAttr.stAttrH264e.u32Profile      = 0;

    stChnAttr.stRcAttr.enRcMode                        = VENC_RC_MODE_H264CBR;
    stChnAttr.stRcAttr.stAttrH264Cbr.fr32DstFrmRate    = 25;
    stChnAttr.stRcAttr.stAttrH264Cbr.u32BitRate        = 1024 * 2;
    stChnAttr.stRcAttr.stAttrH264Cbr.u32FluctuateLevel = 0;
    stChnAttr.stRcAttr.stAttrH264Cbr.u32Gop            = 25;
    stChnAttr.stRcAttr.stAttrH264Cbr.u32SrcFrmRate     = 25;
    stChnAttr.stRcAttr.stAttrH264Cbr.u32StatTime       = 1;
    
    s32Ret = HI_MPI_VENC_CreateChn(VencChn, &stChnAttr);
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_VENC_CreateChn error, s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }
    
    s32Ret = HI_MPI_VENC_StartRecvPic(VencChn);
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_VENC_StartRecvPic error, s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }

    printf("Venc chn create ok, and start receive picture!\n");
    
    return HI_SUCCESS;
}

HI_S32 SAMPLE_RGN_StopVenc(VENC_CHN VencChn)
{
    HI_S32 s32Ret;

    s32Ret = HI_MPI_VENC_StopRecvPic(VencChn);
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_VENC_StopRecvPic error, s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }

    s32Ret = HI_MPI_VENC_DestroyChn(VencChn);
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_VENC_DestroyChn error, s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }

    printf("Venc chn stop receive picture and destroy ok!\n");
    
    return HI_SUCCESS;
}


/******************************************************************************
 * function : change cover
 ******************************************************************************/
void* SAMPLE_RGN_VpssCoverDynamicDisplay(void* p)
{
    HI_S32 s32Ret;
    HI_U32 u32RgnNum;
    RGN_HANDLE Handle;
    RGN_HANDLE startHandle;
    MPP_CHN_S stChn;
    RGN_CHN_ATTR_S stChnAttr;
    RGN_ATTR_INFO_S *pstRgnAttrInfo = NULL;
    
    stChn.enModId  = HI_ID_VPSS;
    stChn.s32DevId = 0;
    stChn.s32ChnId = 0;

    pstRgnAttrInfo = (RGN_ATTR_INFO_S *)p;
    startHandle    = pstRgnAttrInfo->Handle;
    u32RgnNum      = pstRgnAttrInfo->u32RgnNum;

    if (u32RgnNum > COVER_MAX_NUM_VPSS)
    {
        printf("cover num(%d) is bigger than COVER_MAX_NUM_VPSS(%d)..\n", u32RgnNum, COVER_MAX_NUM_VPSS);
        return NULL;
    }
    
    while (HI_FALSE == bExit)
    {
        for (Handle=startHandle; Handle<(startHandle+u32RgnNum); Handle++)
        {
            stChnAttr.bShow  = HI_TRUE;
            stChnAttr.enType = COVER_RGN;
            stChnAttr.unChnAttr.stCoverChn.enCoverType = AREA_RECT;
            stChnAttr.unChnAttr.stCoverChn.stRect.s32X      = 32*(Handle - startHandle);
            stChnAttr.unChnAttr.stCoverChn.stRect.s32Y      = 32*(Handle - startHandle);
            stChnAttr.unChnAttr.stCoverChn.stRect.u32Height = 64;
            stChnAttr.unChnAttr.stCoverChn.stRect.u32Width  = 64;
            stChnAttr.unChnAttr.stCoverChn.u32Color         = 0x0000ffff;
            if (Handle%2)
            {
                stChnAttr.unChnAttr.stCoverChn.u32Color = 0x00ff0000;
            }
            stChnAttr.unChnAttr.stCoverChn.u32Layer     = Handle - startHandle; 

            s32Ret = HI_MPI_RGN_SetDisplayAttr(Handle, &stChn, &stChnAttr);
            if (HI_SUCCESS != s32Ret)
            {
                SAMPLE_RGN_NOT_PASS(s32Ret);
            }
        }   
        sleep(3);

        for (Handle=startHandle; Handle<(startHandle+u32RgnNum); Handle++)
        {    
            stChnAttr.unChnAttr.stCoverChn.stRect.s32X      = 32*((Handle - startHandle) + 1);
            stChnAttr.unChnAttr.stCoverChn.stRect.s32Y      = 32;
            stChnAttr.unChnAttr.stCoverChn.stRect.u32Height = 64;
            stChnAttr.unChnAttr.stCoverChn.stRect.u32Width  = 64;
            stChnAttr.unChnAttr.stCoverChn.u32Color         = 0x0000ffff;
            if (Handle%2)
            {
                stChnAttr.unChnAttr.stCoverChn.u32Color = 0x00ff0000;
            }
            stChnAttr.unChnAttr.stCoverChn.u32Layer     = Handle - startHandle;
            
            s32Ret = HI_MPI_RGN_SetDisplayAttr(Handle, &stChn, &stChnAttr);
            if (HI_SUCCESS != s32Ret)
            {
                SAMPLE_RGN_NOT_PASS(s32Ret);
            }
        }
        sleep(3);

        for (Handle=startHandle; Handle<(startHandle+u32RgnNum); Handle++)
        {
            stChnAttr.unChnAttr.stCoverChn.stRect.s32X      = 60 * (Handle - startHandle);
            stChnAttr.unChnAttr.stCoverChn.stRect.s32Y      = 60 * (Handle - startHandle);
            stChnAttr.unChnAttr.stCoverChn.stRect.u32Height = 64 * ((Handle - startHandle) + 1);
            stChnAttr.unChnAttr.stCoverChn.stRect.u32Width  = 64 * ((Handle - startHandle) + 1);
            stChnAttr.unChnAttr.stCoverChn.u32Color         = 0x000000ff;
            if (1 == Handle)
            {
                stChnAttr.unChnAttr.stCoverChn.u32Color  = 0x0000ff00;
            }
            else if (2 == Handle)
            {
                stChnAttr.unChnAttr.stCoverChn.u32Color  = 0x00ff0000;
            }
            else if (3 == Handle)
            {
                stChnAttr.unChnAttr.stCoverChn.u32Color  = 0x00808000;
            }
            stChnAttr.unChnAttr.stCoverChn.u32Layer      = Handle - startHandle;
            
            s32Ret = HI_MPI_RGN_SetDisplayAttr(Handle, &stChn, &stChnAttr);
            if (HI_SUCCESS != s32Ret)
            {
                SAMPLE_RGN_NOT_PASS(s32Ret);
            }
        }
        sleep(3);

        for (Handle=startHandle; Handle<(startHandle+u32RgnNum); Handle++)
        {    
            stChnAttr.unChnAttr.stCoverChn.stRect.s32X      = 60*((Handle - startHandle)+1);
            stChnAttr.unChnAttr.stCoverChn.stRect.s32Y      = 60;
            stChnAttr.unChnAttr.stCoverChn.stRect.u32Height = 64 * ((Handle - startHandle) + 1);
            stChnAttr.unChnAttr.stCoverChn.stRect.u32Width  = 64 * ((Handle - startHandle) + 1);
            stChnAttr.unChnAttr.stCoverChn.u32Color         = 0x000000ff;
            if (1 == Handle)
            {
                stChnAttr.unChnAttr.stCoverChn.u32Color  = 0x0000ff00;
            }
            else if (2 == Handle)
            {
                stChnAttr.unChnAttr.stCoverChn.u32Color  = 0x00ff0000;
            }
            else if (3 == Handle)
            {
                stChnAttr.unChnAttr.stCoverChn.u32Color  = 0x00800080;
            }
            stChnAttr.unChnAttr.stCoverChn.u32Layer      = Handle - startHandle;
            
            s32Ret = HI_MPI_RGN_SetDisplayAttr(Handle, &stChn, &stChnAttr);
            if (HI_SUCCESS != s32Ret)
            {
                SAMPLE_RGN_NOT_PASS(s32Ret);
            }
        }
        
        sleep(2);
    }

    return (HI_VOID *)HI_SUCCESS;
}

/******************************************************************************
 * function : change coverex
 ******************************************************************************/
void* SAMPLE_RGN_VpssCoverExDynamicDisplay(void* p)
{
    HI_S32 s32Ret;
    HI_U32 u32RgnNum;
    RGN_HANDLE Handle;
    RGN_HANDLE startHandle;
    MPP_CHN_S stChn;
    RGN_CHN_ATTR_S stChnAttr;
    RGN_ATTR_INFO_S *pstRgnAttrInfo = NULL;
    
    stChn.enModId  = HI_ID_VPSS;
    stChn.s32DevId = 0;
    stChn.s32ChnId = VPSS_CHN0;

    pstRgnAttrInfo = (RGN_ATTR_INFO_S *)p;
    startHandle    = pstRgnAttrInfo->Handle;
    u32RgnNum      = pstRgnAttrInfo->u32RgnNum;

    if (u32RgnNum > COVEREX_MAX_NUM_VPSS)
    {
        printf("coverex num(%d) is bigger than COVEREX_MAX_NUM_VPSS(%d)..\n", u32RgnNum, COVEREX_MAX_NUM_VPSS);
        return NULL;
    }
    
    while (HI_FALSE == bExit)
    {
        for (Handle=startHandle; Handle<(startHandle+u32RgnNum); Handle++)
        {
            stChnAttr.bShow  = HI_TRUE;
            stChnAttr.enType = COVEREX_RGN;
            stChnAttr.unChnAttr.stCoverExChn.enCoverType = AREA_RECT;
            stChnAttr.unChnAttr.stCoverExChn.stRect.s32X      = 32*(Handle - startHandle);
            stChnAttr.unChnAttr.stCoverExChn.stRect.s32Y      = 32*(Handle - startHandle);
            stChnAttr.unChnAttr.stCoverExChn.stRect.u32Height = 64;
            stChnAttr.unChnAttr.stCoverExChn.stRect.u32Width  = 64;
            stChnAttr.unChnAttr.stCoverExChn.u32Color         = 0x0000ffff;
            if (Handle%2)
            {
                stChnAttr.unChnAttr.stCoverExChn.u32Color = 0x00ff0000;
            }
            stChnAttr.unChnAttr.stCoverExChn.u32Layer     = Handle - startHandle; 

            s32Ret = HI_MPI_RGN_SetDisplayAttr(Handle, &stChn, &stChnAttr);
            if (HI_SUCCESS != s32Ret)
            {
                SAMPLE_RGN_NOT_PASS(s32Ret);
            }
        }   
        sleep(3);

        for (Handle=startHandle; Handle<(startHandle+u32RgnNum); Handle++)
        {    
            stChnAttr.unChnAttr.stCoverExChn.stRect.s32X      = 32*((Handle - startHandle) + 1);
            stChnAttr.unChnAttr.stCoverExChn.stRect.s32Y      = 32;
            stChnAttr.unChnAttr.stCoverExChn.stRect.u32Height = 64;
            stChnAttr.unChnAttr.stCoverExChn.stRect.u32Width  = 64;
            stChnAttr.unChnAttr.stCoverExChn.u32Color         = 0x0000ffff;
            if (Handle%2)
            {
                stChnAttr.unChnAttr.stCoverExChn.u32Color = 0x00ff8000;
            }
            stChnAttr.unChnAttr.stCoverExChn.u32Layer     = Handle - startHandle;
            
            s32Ret = HI_MPI_RGN_SetDisplayAttr(Handle, &stChn, &stChnAttr);
            if (HI_SUCCESS != s32Ret)
            {
                SAMPLE_RGN_NOT_PASS(s32Ret);
            }
        }
        sleep(3);

        for (Handle=startHandle; Handle<(startHandle+u32RgnNum); Handle++)
        {
            stChnAttr.unChnAttr.stCoverExChn.stRect.s32X      = 60 * (Handle - startHandle);
            stChnAttr.unChnAttr.stCoverExChn.stRect.s32Y      = 60 * (Handle - startHandle);
            stChnAttr.unChnAttr.stCoverExChn.stRect.u32Height = 64 * ((Handle - startHandle) + 1);
            stChnAttr.unChnAttr.stCoverExChn.stRect.u32Width  = 64 * ((Handle - startHandle) + 1);
            stChnAttr.unChnAttr.stCoverExChn.u32Color         = 0x000000ff;
            if (1 == Handle)
            {
                stChnAttr.unChnAttr.stCoverExChn.u32Color  = 0x0000ff00;
            }
            else if (2 == Handle)
            {
                stChnAttr.unChnAttr.stCoverExChn.u32Color  = 0x00ff0000;
            }
            else if (3 == Handle)
            {
                stChnAttr.unChnAttr.stCoverExChn.u32Color  = 0x0080800;
            }
            stChnAttr.unChnAttr.stCoverExChn.u32Layer      = Handle - startHandle;
            
            s32Ret = HI_MPI_RGN_SetDisplayAttr(Handle, &stChn, &stChnAttr);
            if (HI_SUCCESS != s32Ret)
            {
                SAMPLE_RGN_NOT_PASS(s32Ret);
            }
        }
        sleep(3);

        for (Handle=startHandle; Handle<(startHandle+u32RgnNum); Handle++)
        {    
            stChnAttr.unChnAttr.stCoverExChn.stRect.s32X      = 60*((Handle - startHandle)+1);
            stChnAttr.unChnAttr.stCoverExChn.stRect.s32Y      = 60;
            stChnAttr.unChnAttr.stCoverExChn.stRect.u32Height = 64 * ((Handle - startHandle) + 1);
            stChnAttr.unChnAttr.stCoverExChn.stRect.u32Width  = 64 * ((Handle - startHandle) + 1);
            stChnAttr.unChnAttr.stCoverExChn.u32Color         = 0x000000ff;
            if (1 == Handle)
            {
                stChnAttr.unChnAttr.stCoverExChn.u32Color  = 0x0000ff00;
            }
            else if (2 == Handle)
            {
                stChnAttr.unChnAttr.stCoverExChn.u32Color  = 0x00ff0000;
            }
            else if (3 == Handle)
            {
                stChnAttr.unChnAttr.stCoverExChn.u32Color  = 0x0080800;
            }
            stChnAttr.unChnAttr.stCoverExChn.u32Layer      = Handle - startHandle;
            
            s32Ret = HI_MPI_RGN_SetDisplayAttr(Handle, &stChn, &stChnAttr);
            if (HI_SUCCESS != s32Ret)
            {
                SAMPLE_RGN_NOT_PASS(s32Ret);
            }
        }
        
        sleep(2);
    }

    return (HI_VOID *)HI_SUCCESS;
}


/******************************************************************************
 * function : change OSD
 ******************************************************************************/
void* SAMPLE_RGN_VpssOSdDynamicDisplay(void* p)
{
    HI_S32 s32Ret;
    HI_U32 u32RgnNum;
    RGN_HANDLE Handle;
    RGN_HANDLE startHandle;
    MPP_CHN_S stChn;
    RGN_CHN_ATTR_S stChnAttr;
    RGN_ATTR_INFO_S *pstRgnAttrInfo = NULL;
    
    stChn.enModId  = HI_ID_VPSS;
    stChn.s32DevId = 0;
    stChn.s32ChnId = VPSS_CHN0;

    pstRgnAttrInfo = (RGN_ATTR_INFO_S *)p;
    startHandle    = pstRgnAttrInfo->Handle;
    u32RgnNum      = pstRgnAttrInfo->u32RgnNum;

    if (u32RgnNum > OVERLAYEX_MAX_NUM_VPSS)
    {
        printf("cover num(%d) is bigger than OVERLAY_MAX_NUM_VPSS(%d)..\n", u32RgnNum, OVERLAY_MAX_NUM_VPSS);
        return NULL;
    }
    
    while (HI_FALSE == bExit)
    {
        for (Handle=startHandle; Handle<(startHandle+u32RgnNum); Handle++)
        {
            stChnAttr.bShow  = HI_TRUE;
            stChnAttr.enType = OVERLAYEX_RGN;
            stChnAttr.unChnAttr.stOverlayExChn.stPoint.s32X = 80*(Handle - startHandle);
            stChnAttr.unChnAttr.stOverlayExChn.stPoint.s32Y = 80*(Handle - startHandle);
            stChnAttr.unChnAttr.stOverlayExChn.u32BgAlpha   = 30*(OVERLAYEX_MAX_NUM_VPSS - (Handle - startHandle));
            stChnAttr.unChnAttr.stOverlayExChn.u32FgAlpha   = 30*(OVERLAYEX_MAX_NUM_VPSS - (Handle - startHandle));
            stChnAttr.unChnAttr.stOverlayExChn.u32Layer     = Handle - startHandle;
            
            s32Ret = HI_MPI_RGN_SetDisplayAttr(Handle, &stChn, &stChnAttr);
            if (HI_SUCCESS != s32Ret)
            {
                SAMPLE_RGN_NOT_PASS(s32Ret);
            }
        }   
        sleep(3);

        for (Handle=startHandle; Handle<(startHandle+u32RgnNum); Handle++)
        {    
            stChnAttr.bShow  = HI_TRUE;
            stChnAttr.enType = OVERLAYEX_RGN;
            stChnAttr.unChnAttr.stOverlayExChn.stPoint.s32X = 50*(Handle - startHandle);
            stChnAttr.unChnAttr.stOverlayExChn.stPoint.s32Y = 50*(Handle - startHandle);
            stChnAttr.unChnAttr.stOverlayExChn.u32BgAlpha   = 30*(OVERLAYEX_MAX_NUM_VPSS - (Handle - startHandle));
            stChnAttr.unChnAttr.stOverlayExChn.u32FgAlpha   = 30*(OVERLAYEX_MAX_NUM_VPSS - (Handle - startHandle));
            stChnAttr.unChnAttr.stOverlayExChn.u32Layer     = Handle - startHandle;
            
            s32Ret = HI_MPI_RGN_SetDisplayAttr(Handle, &stChn, &stChnAttr);
            if (HI_SUCCESS != s32Ret)
            {
                SAMPLE_RGN_NOT_PASS(s32Ret);
            }
        }
        sleep(3);

        for (Handle=startHandle; Handle<(startHandle+u32RgnNum); Handle++)
        {
            stChnAttr.bShow  = HI_TRUE;
            stChnAttr.enType = OVERLAYEX_RGN;
            stChnAttr.unChnAttr.stOverlayExChn.stPoint.s32X = 60*(Handle - startHandle);
            stChnAttr.unChnAttr.stOverlayExChn.stPoint.s32Y = 60*(Handle - startHandle);
            stChnAttr.unChnAttr.stOverlayExChn.u32BgAlpha   = 30*(OVERLAYEX_MAX_NUM_VPSS - (Handle - startHandle));
            stChnAttr.unChnAttr.stOverlayExChn.u32FgAlpha   = 30*(OVERLAYEX_MAX_NUM_VPSS - (Handle - startHandle));
            stChnAttr.unChnAttr.stOverlayExChn.u32Layer     = Handle - startHandle;
            
            s32Ret = HI_MPI_RGN_SetDisplayAttr(Handle, &stChn, &stChnAttr);
            if (HI_SUCCESS != s32Ret)
            {
                SAMPLE_RGN_NOT_PASS(s32Ret);
            }
        }
        sleep(3);

        for (Handle=startHandle; Handle<(startHandle+u32RgnNum); Handle++)
        {    
            stChnAttr.bShow  = HI_TRUE;
            stChnAttr.enType = OVERLAYEX_RGN;
            stChnAttr.unChnAttr.stOverlayExChn.stPoint.s32X = 100*(Handle - startHandle);
            stChnAttr.unChnAttr.stOverlayExChn.stPoint.s32Y = 100*(Handle - startHandle);
            stChnAttr.unChnAttr.stOverlayExChn.u32BgAlpha   = 30*(OVERLAYEX_MAX_NUM_VPSS - (Handle - startHandle));
            stChnAttr.unChnAttr.stOverlayExChn.u32FgAlpha   = 30*(OVERLAYEX_MAX_NUM_VPSS - (Handle - startHandle));
            stChnAttr.unChnAttr.stOverlayExChn.u32Layer     = Handle - startHandle;
            
            s32Ret = HI_MPI_RGN_SetDisplayAttr(Handle, &stChn, &stChnAttr);
            if (HI_SUCCESS != s32Ret)
            {
                SAMPLE_RGN_NOT_PASS(s32Ret);
            }
        }
        
        sleep(2);
    }

    return (HI_VOID *)HI_SUCCESS;
}


/******************************************************************************
 * function : change OSD
 ******************************************************************************/
void* SAMPLE_RGN_VencOSdDynamicDisplay(void* p)
{
    HI_S32 s32Ret;
    RGN_HANDLE Handle;
    MPP_CHN_S stChn;
    RGN_CHN_ATTR_S stChnAttr;

    stChn.enModId  = HI_ID_VENC;
    stChn.s32DevId = 0;
    stChn.s32ChnId = 0;

        
    while (HI_FALSE == bExit)
    {
        /* Dynamically change background color and position */
        for (Handle=0; Handle<4; Handle++)
        {
            stChnAttr.bShow  = HI_TRUE;
            stChnAttr.enType = OVERLAY_RGN;
            stChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = 80*Handle;
            stChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = 80*Handle;
            stChnAttr.unChnAttr.stOverlayChn.u32BgAlpha   = 16*(8-Handle);
            stChnAttr.unChnAttr.stOverlayChn.u32FgAlpha   = 16*(8-Handle);
            stChnAttr.unChnAttr.stOverlayChn.u32Layer     = Handle;

            stChnAttr.unChnAttr.stOverlayChn.stQpInfo.bAbsQp = HI_FALSE;
            stChnAttr.unChnAttr.stOverlayChn.stQpInfo.s32Qp  = 0;
			stChnAttr.unChnAttr.stOverlayChn.stQpInfo.bQpDisable = HI_FALSE;
			
            stChnAttr.unChnAttr.stOverlayChn.stInvertColor.stInvColArea.u32Height = 16*(Handle%2+1);
            stChnAttr.unChnAttr.stOverlayChn.stInvertColor.stInvColArea.u32Width  = 16*(Handle%2+1);
            stChnAttr.unChnAttr.stOverlayChn.stInvertColor.u32LumThresh = 128;
            stChnAttr.unChnAttr.stOverlayChn.stInvertColor.enChgMod     = LESSTHAN_LUM_THRESH;
            stChnAttr.unChnAttr.stOverlayChn.stInvertColor.bInvColEn    = HI_FALSE;
        
            s32Ret = HI_MPI_RGN_SetDisplayAttr(Handle, &stChn, &stChnAttr);
            if (HI_SUCCESS != s32Ret)
            {
                SAMPLE_RGN_NOT_PASS(s32Ret);
            }
        }   
        sleep(3);

        for (Handle=0; Handle<4; Handle++)
        {    
            stChnAttr.bShow  = HI_TRUE;
            stChnAttr.enType = OVERLAY_RGN;
            stChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = 8*Handle;
            stChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = 8*Handle;
            stChnAttr.unChnAttr.stOverlayChn.u32BgAlpha   = 16*(8-Handle);
            stChnAttr.unChnAttr.stOverlayChn.u32FgAlpha   = 16*(8-Handle);
            stChnAttr.unChnAttr.stOverlayChn.u32Layer     = Handle;

            stChnAttr.unChnAttr.stOverlayChn.stQpInfo.bAbsQp = HI_FALSE;
            stChnAttr.unChnAttr.stOverlayChn.stQpInfo.s32Qp  = 0;
			stChnAttr.unChnAttr.stOverlayChn.stQpInfo.bQpDisable = HI_FALSE;

            stChnAttr.unChnAttr.stOverlayChn.stInvertColor.stInvColArea.u32Height = 16*(Handle%2+1);
            stChnAttr.unChnAttr.stOverlayChn.stInvertColor.stInvColArea.u32Width  = 16*(Handle%2+1);
            stChnAttr.unChnAttr.stOverlayChn.stInvertColor.u32LumThresh = 128;
            stChnAttr.unChnAttr.stOverlayChn.stInvertColor.enChgMod     = LESSTHAN_LUM_THRESH;
            stChnAttr.unChnAttr.stOverlayChn.stInvertColor.bInvColEn    = HI_FALSE;

            s32Ret = HI_MPI_RGN_SetDisplayAttr(Handle, &stChn, &stChnAttr);
            if (HI_SUCCESS != s32Ret)
            {
                SAMPLE_RGN_NOT_PASS(s32Ret);
            }
        }
        sleep(3);

        for (Handle=0; Handle<4; Handle++)
        {
            stChnAttr.bShow  = HI_TRUE;
            stChnAttr.enType = OVERLAY_RGN;
            stChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = 8*Handle;
            stChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = 8*Handle;
            stChnAttr.unChnAttr.stOverlayChn.u32BgAlpha   = 16*(8-Handle);
            stChnAttr.unChnAttr.stOverlayChn.u32FgAlpha   = 16*(8-Handle);
            stChnAttr.unChnAttr.stOverlayChn.u32Layer     = Handle;

            stChnAttr.unChnAttr.stOverlayChn.stQpInfo.bAbsQp = HI_FALSE;
            stChnAttr.unChnAttr.stOverlayChn.stQpInfo.s32Qp  = 0;
			stChnAttr.unChnAttr.stOverlayChn.stQpInfo.bQpDisable = HI_FALSE;

            stChnAttr.unChnAttr.stOverlayChn.stInvertColor.stInvColArea.u32Height = 16*(Handle%2+1);
            stChnAttr.unChnAttr.stOverlayChn.stInvertColor.stInvColArea.u32Width  = 16*(Handle%2+1);
            stChnAttr.unChnAttr.stOverlayChn.stInvertColor.u32LumThresh = 128;
            stChnAttr.unChnAttr.stOverlayChn.stInvertColor.enChgMod     = LESSTHAN_LUM_THRESH;
            stChnAttr.unChnAttr.stOverlayChn.stInvertColor.bInvColEn    = HI_FALSE;
            
            s32Ret = HI_MPI_RGN_SetDisplayAttr(Handle, &stChn, &stChnAttr);
            if (HI_SUCCESS != s32Ret)
            {
                SAMPLE_RGN_NOT_PASS(s32Ret);
            }
        }
        sleep(3);

        for (Handle=0; Handle<4; Handle++)
        {    
            stChnAttr.bShow  = HI_TRUE;
            stChnAttr.enType = OVERLAY_RGN;
            stChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = 8*Handle;
            stChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = 8*Handle;
            stChnAttr.unChnAttr.stOverlayChn.u32BgAlpha   = 16*(8-Handle);
            stChnAttr.unChnAttr.stOverlayChn.u32FgAlpha   = 16*(8-Handle);
            stChnAttr.unChnAttr.stOverlayChn.u32Layer     = Handle;

            stChnAttr.unChnAttr.stOverlayChn.stQpInfo.bAbsQp = HI_FALSE;
            stChnAttr.unChnAttr.stOverlayChn.stQpInfo.s32Qp  = 0;
			stChnAttr.unChnAttr.stOverlayChn.stQpInfo.bQpDisable = HI_FALSE;

            stChnAttr.unChnAttr.stOverlayChn.stInvertColor.stInvColArea.u32Height = 16*(Handle%2+1);
            stChnAttr.unChnAttr.stOverlayChn.stInvertColor.stInvColArea.u32Width  = 16*(Handle%2+1);
            stChnAttr.unChnAttr.stOverlayChn.stInvertColor.u32LumThresh = 128;
            stChnAttr.unChnAttr.stOverlayChn.stInvertColor.enChgMod     = LESSTHAN_LUM_THRESH;
            stChnAttr.unChnAttr.stOverlayChn.stInvertColor.bInvColEn    = HI_FALSE;
            
            s32Ret = HI_MPI_RGN_SetDisplayAttr(Handle, &stChn, &stChnAttr);
            if (HI_SUCCESS != s32Ret)
            {
                SAMPLE_RGN_NOT_PASS(s32Ret);
            }
        }
        
        sleep(2);
    }

    return (HI_VOID *)HI_SUCCESS;
}


HI_VOID SAMPLE_RGN_WriteStreamToFile( FILE * fp, VENC_PACK_S * pstNalu )
{
    HI_U8 * p;
    HI_U32 u32Len;

    p = (HI_U8 *) pstNalu->pu8Addr+pstNalu->u32Offset;

    u32Len = pstNalu->u32Len-pstNalu->u32Offset;

    fwrite(p, u32Len, sizeof(HI_U8), fp);
    
    fflush(fp);
}

    
HI_S32 SAMPLE_RGN_CreateRegion(RGN_SIZE_S stRgnsize)
{
    RGN_CHN_ATTR_S stChnAttr;
    RGN_ATTR_S stRgnAttr;
    HI_U32 i,j;
    HI_S32 s32Ret;
    MPP_CHN_S stChn;
    
    for(i=0; i<stRgnsize.u32RgnNum; i++)
    {
        /*creat region*/
        stRgnAttr.enType = OVERLAY_RGN;

        stRgnAttr.unAttr.stOverlay.enPixelFmt = PIXEL_FORMAT_RGB_1555;
        if(bShape1)
        {
            stRgnAttr.unAttr.stOverlay.stSize.u32Width  = 720;
            stRgnAttr.unAttr.stOverlay.stSize.u32Height = 96;
        }
        else if(bShape2)
        {
            stRgnAttr.unAttr.stOverlay.stSize.u32Width  = 96;
            stRgnAttr.unAttr.stOverlay.stSize.u32Height = 576;
        }
        else if(bArea)
        {
            stRgnAttr.unAttr.stOverlay.stSize.u32Width  = stRgnsize.stSIZE.u32Width;
            stRgnAttr.unAttr.stOverlay.stSize.u32Height = stRgnsize.stSIZE.u32Height;
        }
        else
        {
            stRgnAttr.unAttr.stOverlay.stSize.u32Width  = 96;
            stRgnAttr.unAttr.stOverlay.stSize.u32Height = 96;
        }
        stRgnAttr.unAttr.stOverlay.u32BgColor           = 0xfc + (0xff*i);

        s32Ret = HI_MPI_RGN_Create(i, &stRgnAttr);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_RGN_NOT_PASS(s32Ret);
        }

        for(j=0; j<MAX_VENC_WORK_CHN_NUM; j++)
        {
            stChn.enModId  = HI_ID_VENC;
            stChn.s32DevId = 0;
            stChn.s32ChnId = j;

            stChnAttr.bShow  = HI_TRUE;
            stChnAttr.enType = OVERLAY_RGN;
            stChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = 96*(i+1);
            stChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = 24*(j+1);
            if(bArea)
            {
                stChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = 0;
                stChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = 0;
            }
            stChnAttr.unChnAttr.stOverlayChn.u32BgAlpha = 128;
            stChnAttr.unChnAttr.stOverlayChn.u32FgAlpha = 128;
            stChnAttr.unChnAttr.stOverlayChn.u32Layer   = i;

            stChnAttr.unChnAttr.stOverlayChn.stQpInfo.bAbsQp = HI_FALSE;
            stChnAttr.unChnAttr.stOverlayChn.stQpInfo.s32Qp  = 0;
			stChnAttr.unChnAttr.stOverlayChn.stQpInfo.bQpDisable = HI_FALSE;
            
            stChnAttr.unChnAttr.stOverlayChn.stInvertColor.bInvColEn = HI_FALSE;
            
            s32Ret = HI_MPI_RGN_AttachToChn(i, &stChn, &stChnAttr);
            if (HI_SUCCESS != s32Ret)
            {
                SAMPLE_RGN_NOT_PASS(s32Ret);
            }
        }
    }
    
    return HI_SUCCESS;
}


HI_S32 SAMPLE_RGN_DestroyRegion(RGN_HANDLE Handle, HI_U32 u32Num)
{
    HI_S32 i;
    HI_S32 s32Ret;    
        
    for (i=Handle; i<(Handle + u32Num); i++)
    {
        s32Ret = HI_MPI_RGN_Destroy(i);
        if (HI_SUCCESS != s32Ret)
        {
            printf("HI_MPI_RGN_Destroy failed! s32Ret: 0x%x.\n", s32Ret);
            return s32Ret;
        }
    }

    return HI_SUCCESS;
    
}
 


HI_S32 SAMPLE_RGN_GetStream( HI_S32 chnId,FILE* pfStream ,HI_BOOL bByFrame)
{
    VENC_STREAM_S   stStream;
    HI_U32          u32Cnt;
    HI_S32          s32Ret;
    VENC_CHN_STAT_S stChnStat;
    VENC_PACK_S*    pstPack;

    memset(&stChnStat, 0, sizeof(VENC_CHN_STAT_S));
    s32Ret = HI_MPI_VENC_Query(chnId, &stChnStat);
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_VENC_Query failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }
    
    if(0 == stChnStat.u32CurPacks)
    {
        return HI_FAILURE;
    }
    
    /*malloc buf according to the number of stream packets*/
    stStream.pstPack      = (VENC_PACK_S *) malloc( sizeof(VENC_PACK_S)*stChnStat.u32CurPacks);
    stStream.u32PackCount = stChnStat.u32CurPacks;

#ifndef HI_IO_NOBLOCK
#define HI_IO_NOBLOCK 1
#endif

    /*get the stream*/
    s32Ret = HI_MPI_VENC_GetStream(chnId, &stStream, 0);
    if (HI_SUCCESS != s32Ret)
    {
        printf("Query ok but get fail ret:0x%x\n",s32Ret);
        exit(-1);
        goto __FAIL__;
    }

    pstPack = stStream.pstPack;

    for (u32Cnt=0; u32Cnt<stChnStat.u32CurPacks; u32Cnt++)
    {
        if ( HI_SUCCESS == s32Ret )
        {
            if(NULL != pfStream)
            {
                SAMPLE_RGN_WriteStreamToFile(pfStream, pstPack);
            }
        }
        else
        {
            break;
        }
        pstPack++;
    }
    
    /*release the stream*/
    if (HI_SUCCESS == s32Ret)
    {
        HI_MPI_VENC_ReleaseStream(chnId, &stStream);
        HI_ASSERT( HI_SUCCESS == s32Ret );
    }

    if(NULL != stStream.pstPack)
    {
        free(stStream.pstPack);
    }
    
    return HI_SUCCESS;
    
__FAIL__:
    if(NULL != stStream.pstPack)
    {
        free(stStream.pstPack);
    }
    return HI_FAILURE;
    
}


HI_VOID *SAMPLE_RGN_VencGetStream(void *pData)
{

    HI_S32 s32ret;
    VENC_CHN_STAT_S stChnStat;
    VENC_PTHREAD_INFO_S *pstVencThread;
    FILE * pastream;
    HI_S32 i = 0;
    HI_S32 s32Ret = HI_FAILURE;
    HI_S32 s32cnt = 0;
    RGN_HANDLE Handle;
    MPP_CHN_S stChn;
    RGN_CHN_ATTR_S stChnAttr;
    RGN_SIZE_S stRgnsize;
    HI_U32 s32Tmp = 0;
    BITMAP_S stBitmap;
    char acmd[128];

   
    HI_ASSERT(HI_NULL != pData);

    pstVencThread = (VENC_PTHREAD_INFO_S *)pData;
	snprintf(acmd, sizeof(acmd), "[[ -e jpeg%d ]] || mkdir jpeg%d",pstVencThread->VeChnId,pstVencThread->VeChnId); 
	system(acmd);
    
    while (HI_FALSE == bExit || i < pstVencThread->s32FrmCnt)
    {
        /*change the bitmap*/
        Handle = 0;
         
        s32ret = HI_MPI_VENC_Query( pstVencThread->VeChnId, &stChnStat );
        if (HI_SUCCESS != s32ret)
        {
            ;
        }
        if (stChnStat.u32CurPacks > 0 || stChnStat.u32LeftStreamBytes >0)  
        {
            if(bTravel)
            {
                char pfilename[60]; 
                snprintf(pfilename, sizeof(pfilename), "./jpeg%d/stream_fff_009_%d.jpeg",pstVencThread->VeChnId, i);
                pastream = fopen( pfilename, "wb" );  
                HI_ASSERT( NULL != pastream);
               
                
                /*change the position to display*/       
                Handle = 0;
                
                stChn.enModId  = HI_ID_VENC;
                stChn.s32DevId = pstVencThread->VeChnId;
                stChn.s32ChnId = 0;
                    
                s32Ret = HI_MPI_RGN_GetDisplayAttr(Handle,&stChn,&stChnAttr);
                if (HI_SUCCESS != s32Ret)
                {
                    SAMPLE_RGN_NOT_PASS(s32Ret);
                }

                stChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = 36 * (s32cnt % 19);        
                stChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = 32 * (s32cnt  / 19);

                s32Ret = HI_MPI_RGN_SetDisplayAttr(Handle,&stChn,&stChnAttr);
                if (HI_SUCCESS != s32Ret)
                {
                    SAMPLE_RGN_NOT_PASS(s32Ret);
                }               

                usleep(500);

                s32Ret = HI_MPI_RGN_GetDisplayAttr(Handle,&stChn,&stChnAttr);
                if (HI_SUCCESS != s32Ret)
                {
                    SAMPLE_RGN_NOT_PASS(s32Ret);
                }
                
                if(++s32cnt > (19 * 16) - 1)
                {
                    s32cnt = 0;
                }
                printf("s32cnt = %d\n", s32cnt);
                
                /*get the stream*/                         
                SAMPLE_RGN_GetStream(pstVencThread->VeChnId, pastream, HI_TRUE);
                
                i++;

                fclose(pastream);
                printf("i = %d\n", i);
                
            }
            else if(bShape1)
            {
                char pfilename[60]; 
                 s32Tmp = 12;
                if(pstVencThread->VeChnId > 0)
                    s32Tmp = 14;
                snprintf(pfilename, sizeof(pfilename), "./jpeg%d/stream_fff_0%d_%d.jpeg",pstVencThread->VeChnId, s32Tmp, i);
                pastream = fopen( pfilename, "wb" );  
                HI_ASSERT( NULL != pastream);
               
                /*change the position to display*/      
                Handle = 0;
                
                stChn.enModId = HI_ID_VENC;
                stChn.s32DevId = pstVencThread->VeChnId;
                stChn.s32ChnId = 0;
                    
                s32Ret = HI_MPI_RGN_GetDisplayAttr(Handle,&stChn,&stChnAttr);
                if (HI_SUCCESS != s32Ret)
                {
                    SAMPLE_RGN_NOT_PASS(s32Ret);
                }

                stChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = 0;        
                stChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = 32 * s32cnt;

                s32Ret = HI_MPI_RGN_SetDisplayAttr(Handle,&stChn,&stChnAttr);
                if (HI_SUCCESS != s32Ret)
                {
                    SAMPLE_RGN_NOT_PASS(s32Ret);
                }               

                sleep(1);

                s32Ret = HI_MPI_RGN_GetDisplayAttr(Handle,&stChn,&stChnAttr);
                if (HI_SUCCESS != s32Ret)
                {
                    SAMPLE_RGN_NOT_PASS(s32Ret);
                }

                if(++s32cnt > 25)
                {
                    s32cnt = 0;
                }
                printf("s32cnt = %d\n", s32cnt);
                
                /*get the steam*/                         
                SAMPLE_RGN_GetStream(pstVencThread->VeChnId, pastream, HI_TRUE);             
                i++;

                fclose(pastream);
                printf("i = %d\n", i);
            }
            else if(bShape2)
            {
                char pfilename[60]; 
                s32Tmp = 13;
                if(pstVencThread->VeChnId > 0)
                    s32Tmp = 15;
                snprintf(pfilename, sizeof(pfilename), "./jpeg%d/stream_fff_0%d_%d.jpeg",pstVencThread->VeChnId, s32Tmp,i);
                pastream = fopen( pfilename, "wb" );
                HI_ASSERT( NULL != pastream);
               
            
                /*change the position to display*/      
                Handle = 0;
                
                stChn.enModId = HI_ID_VENC;
                stChn.s32DevId = pstVencThread->VeChnId;
                stChn.s32ChnId = 0;
                    
                s32Ret = HI_MPI_RGN_GetDisplayAttr(Handle,&stChn,&stChnAttr);
                if (HI_SUCCESS != s32Ret)
                {
                    SAMPLE_RGN_NOT_PASS(s32Ret);
                }

                stChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = 32 * s32cnt;
                stChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = 0;

                s32Ret = HI_MPI_RGN_SetDisplayAttr(Handle,&stChn,&stChnAttr);
                if (HI_SUCCESS != s32Ret)
                {
                    SAMPLE_RGN_NOT_PASS(s32Ret);
                }               

                sleep(1);

                s32Ret = HI_MPI_RGN_GetDisplayAttr(Handle, &stChn, &stChnAttr);
                if (HI_SUCCESS != s32Ret)
                {
                    SAMPLE_RGN_NOT_PASS(s32Ret);
                }

                if(++s32cnt > 25)
                {
                    s32cnt = 0;
                }
                printf("s32cnt = %d\n", s32cnt);
                
                /*get the stream*/                         
                SAMPLE_RGN_GetStream(pstVencThread->VeChnId, pastream, HI_TRUE);             
                i++;

                fclose(pastream);
                printf("i = %d\n", i);
            }
            else if(bArea)
            {
                char pfilename[60]; 
                s32Tmp = 16;
                if(pstVencThread->VeChnId > 0)
                    s32Tmp = 17;
                snprintf(pfilename, sizeof(pfilename), "./jpeg%d/stream_fff_0%d_%d.jpeg",pstVencThread->VeChnId, s32Tmp,i);
                pastream = fopen( pfilename, "wb" );  
                HI_ASSERT( NULL != pastream);

                pthread_mutex_lock(&Rgnmutex_Tmp);
                
                stRgnsize.u32RgnNum = 1;
                SAMPLE_RGN_DestroyRegion(Handle, stRgnsize.u32RgnNum);
                stRgnsize.stSIZE.u32Height = 36 * (s32cnt + 1);
                stRgnsize.stSIZE.u32Width = 36 * (s32cnt + 1);
                SAMPLE_RGN_CreateRegion(stRgnsize);

				/*change the bitmap*/
				Handle = 0;

				s32Ret = SAMPLE_RGN_LoadBmp("mm.bmp", &stBitmap, HI_FALSE, 0);
				if (HI_SUCCESS != s32Ret)
				{
					SAMPLE_RGN_NOT_PASS(s32Ret);
				}

				s32Ret = HI_MPI_RGN_SetBitMap(Handle,&stBitmap);
				if (HI_SUCCESS != s32Ret)
				{
					SAMPLE_RGN_NOT_PASS(s32Ret);
				}

				if (HI_NULL != stBitmap.pData)
				{
					free(stBitmap.pData);
					stBitmap.pData = HI_NULL;
				}
                
                if(++s32cnt > 25)
                {
                    s32cnt = 0;
                }

                pthread_mutex_unlock(&Rgnmutex_Tmp);
                
                printf("s32cnt = %d\n", s32cnt);
                usleep(50000);

                /*get the stream*/                         
                SAMPLE_RGN_GetStream(pstVencThread->VeChnId, pastream, HI_TRUE);             
                i++;

				fclose(pastream);
                printf("i = %d\n", i);
            }
            else
            {     
                 SAMPLE_RGN_GetStream(pstVencThread->VeChnId, pstVencThread->pstream, HI_TRUE);
                 i++;
            }
        }
    }
    printf("\nchn %d finish!\n", pstVencThread->VeChnId);
    
    return HI_NULL;
}

HI_VOID *SAMPLE_RGN_VpssOsdReverse(void *pData)
{
    HI_S32 i = 0, j = 0;
    RGN_HANDLE Handle;
    TDE2_SURFACE_S stRgnOrignSurface = {0};
    TDE2_SURFACE_S stRgnSurface = {0};
    RGN_CANVAS_INFO_S stCanvasInfo;
    TDE_HANDLE hTde;
    TDE2_RECT_S stRect = {0};
    VPSS_REGION_INFO_S stReverseRgnInfo;
    HI_U32 au32LumaData[OSD_REVERSE_RGN_MAXCNT];
    RECT_S astOsdRevRect[OSD_REVERSE_RGN_MAXCNT];
    RGN_OSD_REVERSE_INFO_S *pstOsdReverseInfo;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 *pu32VirAddr = NULL;
    MPP_CHN_S stMppChn =  {0};
    RGN_CHN_ATTR_S stOsdChnAttr = {0};

    HI_ASSERT(NULL != pData);
    pstOsdReverseInfo = (RGN_OSD_REVERSE_INFO_S*)pData;
    Handle = pstOsdReverseInfo->Handle;
    HI_ASSERT(OSD_REVERSE_RGN_MAXCNT >= pstOsdReverseInfo->stLumaRgnInfo.u32RegionNum);

    srand(time(NULL));

    /* 1.get current osd info */
    s32Ret = HI_MPI_RGN_GetCanvasInfo(Handle, &stCanvasInfo);
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_RGN_GetCanvasInfo fail! s32Ret: 0x%x.\n", s32Ret);
        return NULL;
    }
    
    s32Ret = HI_MPI_RGN_UpdateCanvas(Handle);
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_RGN_UpdateCanvas fail! s32Ret: 0x%x.\n", s32Ret);
        return NULL;
    }

    /* 2.make a backup of current osd */
    s32Ret = SAMPLE_RGN_ConvOsdCavasToTdeSurface(&stRgnSurface, &stCanvasInfo);
    if (HI_SUCCESS != s32Ret)
    {
        printf("Func: %s, line: %d! s32Ret: 0x%x.\n", __FUNCTION__, __LINE__, s32Ret);
        return NULL;
    }
    
    memcpy(&stRgnOrignSurface, &stRgnSurface, sizeof(stRgnOrignSurface));
    
    s32Ret = HI_MPI_SYS_MmzAlloc(&stRgnOrignSurface.u32PhyAddr, (void **)(&pu32VirAddr), 
        NULL, NULL, stRgnSurface.u32Stride*stRgnSurface.u32Height);
    if (HI_SUCCESS != s32Ret)
    {
        HI_MPI_SYS_MmzFree(stRgnOrignSurface.u32PhyAddr, pu32VirAddr);
        return NULL;
    }

    s32Ret = HI_TDE2_Open();
    if (HI_SUCCESS != s32Ret)
    {
        HI_MPI_SYS_MmzFree(stRgnOrignSurface.u32PhyAddr, pu32VirAddr);
        return NULL;
    }
    
    hTde = HI_TDE2_BeginJob();
    HI_ASSERT(hTde >= 0);
    stRect.u32Width  = stRgnSurface.u32Width;
    stRect.u32Height = stRgnSurface.u32Height;
    s32Ret = HI_TDE2_QuickCopy(hTde, &stRgnSurface, &stRect, &stRgnOrignSurface, &stRect);
    if (HI_SUCCESS != s32Ret)    
    {
        printf("[Func]:%s [Line]:%d [Info]:HI_TDE2_QuickCopy failed\n", __FUNCTION__, __LINE__);
        HI_TDE2_CancelJob(hTde);
        HI_MPI_SYS_MmzFree(stRgnOrignSurface.u32PhyAddr, pu32VirAddr);
        return NULL;
    }
    
    s32Ret = HI_TDE2_EndJob(hTde, HI_FALSE, HI_FALSE, 10);
    if (HI_SUCCESS != s32Ret)
    {
        HI_TDE2_CancelJob(hTde);
        HI_MPI_SYS_MmzFree(stRgnOrignSurface.u32PhyAddr, pu32VirAddr);
        return NULL;
    }
    s32Ret = HI_TDE2_WaitForDone(hTde);
    if (HI_SUCCESS != s32Ret)
    {
        HI_MPI_SYS_MmzFree(stRgnOrignSurface.u32PhyAddr, pu32VirAddr);
        return NULL;
    }

    /* 3.get the  display attribute of OSD attached to vpss*/
    stMppChn.enModId  = HI_ID_VPSS;
    stMppChn.s32DevId = pstOsdReverseInfo->VpssGrp;
    stMppChn.s32ChnId = pstOsdReverseInfo->VpssChn;
    s32Ret = HI_MPI_RGN_GetDisplayAttr(Handle, &stMppChn, &stOsdChnAttr);
    if (HI_SUCCESS != s32Ret)
    {
        HI_MPI_SYS_MmzFree(stRgnOrignSurface.u32PhyAddr, pu32VirAddr);
        return NULL;
    }
    
    stReverseRgnInfo.pstRegion = (RECT_S *)astOsdRevRect;
    while (HI_FALSE == bExit)
    {
        /* 4.get the sum of luma of a region specified by user*/
        s32Ret = HI_MPI_VPSS_GetRegionLuma(pstOsdReverseInfo->VpssGrp, pstOsdReverseInfo->VpssChn, &(pstOsdReverseInfo->stLumaRgnInfo), au32LumaData, -1);
        if (HI_SUCCESS != s32Ret)
        {
            printf("[Func]:%s [Line]:%d [Info]:HI_MPI_VPSS_GetRegionLuma VpssGrp=%d failed, s32Ret: 0x%x.\n", 
                __FUNCTION__, __LINE__, pstOsdReverseInfo->VpssGrp, s32Ret);
            continue ;
        }

        /* 5.decide which region to be reverse color according to the sum of the region*/
        for (i = 0, j = 0; i < pstOsdReverseInfo->stLumaRgnInfo.u32RegionNum; ++i)
        {
            pstOsdReverseInfo->u8PerPixelLumaThrd = rand() % 256;
            
            if (au32LumaData[i] > (pstOsdReverseInfo->u8PerPixelLumaThrd * 
                pstOsdReverseInfo->stLumaRgnInfo.pstRegion[i].u32Width * 
                pstOsdReverseInfo->stLumaRgnInfo.pstRegion[i].u32Height))
            {
                /* 6.get the regions to be reverse color */
                stReverseRgnInfo.pstRegion[j].s32X = pstOsdReverseInfo->stLumaRgnInfo.pstRegion[i].s32X 
                    - stOsdChnAttr.unChnAttr.stOverlayExChn.stPoint.s32X;
                stReverseRgnInfo.pstRegion[j].s32Y = pstOsdReverseInfo->stLumaRgnInfo.pstRegion[i].s32Y 
                    - stOsdChnAttr.unChnAttr.stOverlayExChn.stPoint.s32Y;
                stReverseRgnInfo.pstRegion[j].u32Width = pstOsdReverseInfo->stLumaRgnInfo.pstRegion[i].u32Width;
                stReverseRgnInfo.pstRegion[j].u32Height = pstOsdReverseInfo->stLumaRgnInfo.pstRegion[i].u32Height;
                ++j;
            }
        }
        
        stReverseRgnInfo.u32RegionNum = j;
        
        /* 7.the the canvas to be update */
        if (HI_SUCCESS != HI_MPI_RGN_GetCanvasInfo(Handle, &stCanvasInfo))
        {
            printf("[Func]:%s [Line]:%d [Info]:HI_MPI_RGN_GetCanvasInfo failed\n", __FUNCTION__, __LINE__);
            HI_MPI_SYS_MmzFree(stRgnOrignSurface.u32PhyAddr, pu32VirAddr);
            return NULL;
        }
        if (HI_SUCCESS != SAMPLE_RGN_ConvOsdCavasToTdeSurface(&stRgnSurface, &stCanvasInfo))
        {
            printf("[Func]:%s [Line]:%d [Info]:SAMPLE_RGN_ConvOsdCavasToTdeSurface failed\n", __FUNCTION__, __LINE__);
            HI_MPI_SYS_MmzFree(stRgnOrignSurface.u32PhyAddr, pu32VirAddr);
            return NULL;
        }
        /* 8.reverse color */
        if (HI_SUCCESS != SAMPLE_RGN_ReverseOsdColor(&stRgnOrignSurface, &stRgnSurface, &stReverseRgnInfo))
        {
            printf("[Func]:%s [Line]:%d [Info]:SAMPLE_RGN_ReverseOsdColor failed\n", __FUNCTION__, __LINE__);
            HI_MPI_RGN_UpdateCanvas(Handle);
            HI_MPI_SYS_MmzFree(stRgnOrignSurface.u32PhyAddr, pu32VirAddr);
            return NULL;
        }
        
        /* 9.update OSD */
        if (HI_SUCCESS != HI_MPI_RGN_UpdateCanvas(Handle))
        {
            printf("[Func]:%s [Line]:%d [Info]:HI_MPI_RGN_UpdateCanvas failed\n", __FUNCTION__, __LINE__);
            HI_MPI_SYS_MmzFree(stRgnOrignSurface.u32PhyAddr, pu32VirAddr);
            return NULL;
        }
        
        sleep(2);
    }

    HI_MPI_SYS_MmzFree(stRgnOrignSurface.u32PhyAddr, pu32VirAddr);
    return HI_NULL;
    
}

HI_VOID *SAMPLE_RGN_VencOsdReverse(void *pData)
{
    HI_S32 i = 0, j = 0;
    RGN_HANDLE Handle;
    TDE2_SURFACE_S stRgnOrignSurface = {0};
    TDE2_SURFACE_S stRgnSurface = {0};
    RGN_CANVAS_INFO_S stCanvasInfo;
    TDE_HANDLE hTde;
    TDE2_RECT_S stRect = {0};
    VPSS_REGION_INFO_S stReverseRgnInfo;
    HI_U32 au32LumaData[OSD_REVERSE_RGN_MAXCNT];
    RECT_S astOsdRevRect[OSD_REVERSE_RGN_MAXCNT];
    RGN_OSD_REVERSE_INFO_S *pstOsdReverseInfo;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 *pu32VirAddr = NULL;
    MPP_CHN_S stMppChn =  {0};
    RGN_CHN_ATTR_S stOsdChnAttr = {0};

    HI_ASSERT(NULL != pData);
    pstOsdReverseInfo = (RGN_OSD_REVERSE_INFO_S*)pData;
    Handle = pstOsdReverseInfo->Handle;
    HI_ASSERT(OSD_REVERSE_RGN_MAXCNT >= pstOsdReverseInfo->stLumaRgnInfo.u32RegionNum);

    srand(time(NULL));

    /* 1.get current osd info */
    s32Ret = HI_MPI_RGN_GetCanvasInfo(Handle, &stCanvasInfo);
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_RGN_GetCanvasInfo fail! s32Ret: 0x%x.\n", s32Ret);
        return NULL;
    }
    
    s32Ret = HI_MPI_RGN_UpdateCanvas(Handle);
    if (HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_RGN_UpdateCanvas fail! s32Ret: 0x%x.\n", s32Ret);
        return NULL;
    }

    /* 2.make a backup of current osd */
    s32Ret = SAMPLE_RGN_ConvOsdCavasToTdeSurface(&stRgnSurface, &stCanvasInfo);
    if (HI_SUCCESS != s32Ret)
    {
        printf("Func: %s, line: %d! s32Ret: 0x%x.\n", __FUNCTION__, __LINE__, s32Ret);
        return NULL;
    }
    
    memcpy(&stRgnOrignSurface, &stRgnSurface, sizeof(stRgnOrignSurface));
    
    s32Ret = HI_MPI_SYS_MmzAlloc(&stRgnOrignSurface.u32PhyAddr, (void **)(&pu32VirAddr), 
        NULL, NULL, stRgnSurface.u32Stride*stRgnSurface.u32Height);
    if (HI_SUCCESS != s32Ret)
    {
        HI_MPI_SYS_MmzFree(stRgnOrignSurface.u32PhyAddr, pu32VirAddr);
        return NULL;
    }

    s32Ret = HI_TDE2_Open();
    if (HI_SUCCESS != s32Ret)
    {
        HI_MPI_SYS_MmzFree(stRgnOrignSurface.u32PhyAddr, pu32VirAddr);
        return NULL;
    }
    
    hTde = HI_TDE2_BeginJob();
    HI_ASSERT(hTde >= 0);
    stRect.u32Width  = stRgnSurface.u32Width;
    stRect.u32Height = stRgnSurface.u32Height;
    s32Ret = HI_TDE2_QuickCopy(hTde, &stRgnSurface, &stRect, &stRgnOrignSurface, &stRect);
    if (HI_SUCCESS != s32Ret)    
    {
        printf("[Func]:%s [Line]:%d [Info]:HI_TDE2_QuickCopy failed\n", __FUNCTION__, __LINE__);
        HI_TDE2_CancelJob(hTde);
        HI_MPI_SYS_MmzFree(stRgnOrignSurface.u32PhyAddr, pu32VirAddr);
        return NULL;
    }
    
    s32Ret = HI_TDE2_EndJob(hTde, HI_FALSE, HI_FALSE, 10);
    if (HI_SUCCESS != s32Ret)
    {
        HI_TDE2_CancelJob(hTde);
        HI_MPI_SYS_MmzFree(stRgnOrignSurface.u32PhyAddr, pu32VirAddr);
        return NULL;
    }
    s32Ret = HI_TDE2_WaitForDone(hTde);
    if (HI_SUCCESS != s32Ret)
    {
        HI_MPI_SYS_MmzFree(stRgnOrignSurface.u32PhyAddr, pu32VirAddr);
        return NULL;
    }

    /* 3.get the  display attribute of OSD attached to venc*/
    stMppChn.enModId  = HI_ID_VENC;
    stMppChn.s32DevId = 0;
    stMppChn.s32ChnId = 0;
    s32Ret = HI_MPI_RGN_GetDisplayAttr(Handle, &stMppChn, &stOsdChnAttr);
    if (HI_SUCCESS != s32Ret)
    {
        HI_MPI_SYS_MmzFree(stRgnOrignSurface.u32PhyAddr, pu32VirAddr);
        return NULL;
    }
    
    stReverseRgnInfo.pstRegion = (RECT_S *)astOsdRevRect;
    while (HI_FALSE == bExit)
    {
        /* 4.get the sum of luma of a region specified by user*/
        s32Ret = HI_MPI_VPSS_GetRegionLuma(pstOsdReverseInfo->VpssGrp, pstOsdReverseInfo->VpssChn, &(pstOsdReverseInfo->stLumaRgnInfo), au32LumaData, -1);
        if (HI_SUCCESS != s32Ret)
        {
            printf("[Func]:%s [Line]:%d [Info]:HI_MPI_VPSS_GetRegionLuma VpssGrp=%d failed, s32Ret: 0x%x.\n", 
                __FUNCTION__, __LINE__, pstOsdReverseInfo->VpssGrp, s32Ret);
            continue ;
        }

        /* 5.decide which region to be reverse color according to the sum of the region*/
        for (i = 0, j = 0; i < pstOsdReverseInfo->stLumaRgnInfo.u32RegionNum; ++i)
        {
            pstOsdReverseInfo->u8PerPixelLumaThrd = rand() % 256;
            
            if (au32LumaData[i] > (pstOsdReverseInfo->u8PerPixelLumaThrd * 
                pstOsdReverseInfo->stLumaRgnInfo.pstRegion[i].u32Width * 
                pstOsdReverseInfo->stLumaRgnInfo.pstRegion[i].u32Height))
            {
                /* 6.get the regions to be reverse color */
                stReverseRgnInfo.pstRegion[j].s32X = pstOsdReverseInfo->stLumaRgnInfo.pstRegion[i].s32X 
                    - stOsdChnAttr.unChnAttr.stOverlayChn.stPoint.s32X;
                stReverseRgnInfo.pstRegion[j].s32Y = pstOsdReverseInfo->stLumaRgnInfo.pstRegion[i].s32Y 
                    - stOsdChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y;
                stReverseRgnInfo.pstRegion[j].u32Width = pstOsdReverseInfo->stLumaRgnInfo.pstRegion[i].u32Width;
                stReverseRgnInfo.pstRegion[j].u32Height = pstOsdReverseInfo->stLumaRgnInfo.pstRegion[i].u32Height;
                ++j;
            }
        }
        
        stReverseRgnInfo.u32RegionNum = j;
        
        /* 7.the the canvas to be update */
        if (HI_SUCCESS != HI_MPI_RGN_GetCanvasInfo(Handle, &stCanvasInfo))
        {
            printf("[Func]:%s [Line]:%d [Info]:HI_MPI_RGN_GetCanvasInfo failed\n", __FUNCTION__, __LINE__);
            HI_MPI_SYS_MmzFree(stRgnOrignSurface.u32PhyAddr, pu32VirAddr);
            return NULL;
        }
        if (HI_SUCCESS != SAMPLE_RGN_ConvOsdCavasToTdeSurface(&stRgnSurface, &stCanvasInfo))
        {
            printf("[Func]:%s [Line]:%d [Info]:SAMPLE_RGN_ConvOsdCavasToTdeSurface failed\n", __FUNCTION__, __LINE__);
            HI_MPI_SYS_MmzFree(stRgnOrignSurface.u32PhyAddr, pu32VirAddr);
            return NULL;
        }
        /* 8.reverse color */
        if (HI_SUCCESS != SAMPLE_RGN_ReverseOsdColor(&stRgnOrignSurface, &stRgnSurface, &stReverseRgnInfo))
        {
            printf("[Func]:%s [Line]:%d [Info]:SAMPLE_RGN_ReverseOsdColor failed\n", __FUNCTION__, __LINE__);
            HI_MPI_RGN_UpdateCanvas(Handle);
            HI_MPI_SYS_MmzFree(stRgnOrignSurface.u32PhyAddr, pu32VirAddr);
            return NULL;
        }
        
        /* 9.update OSD */
        if (HI_SUCCESS != HI_MPI_RGN_UpdateCanvas(Handle))
        {
            printf("[Func]:%s [Line]:%d [Info]:HI_MPI_RGN_UpdateCanvas failed\n", __FUNCTION__, __LINE__);
            HI_MPI_SYS_MmzFree(stRgnOrignSurface.u32PhyAddr, pu32VirAddr);
            return NULL;
        }
        
        sleep(2);
    }

    HI_MPI_SYS_MmzFree(stRgnOrignSurface.u32PhyAddr, pu32VirAddr);
    return HI_NULL;
    
}


/******************************************************************************************
    function :  Vpss cover
    process steps:                                      
    0) create some cover regions                
    1) attach them to vpss   
    2) change Regions' Layer                        
    3) change Regions' position                     
    4) change Regions' alpha (front and backgroud)  
******************************************************************************************/

HI_S32 SAMPLE_RGN_AddCoverToVpss(HI_VOID)
{
    HI_S32 s32Ret = HI_SUCCESS;
    RGN_HANDLE Handle;
    HI_U32 u32RgnNum;
    MPP_CHN_S stSrcChn, stDesChn;
    HI_S32 s32VpssGrpNum;
    HI_U32 u32VpssChn = 0;
    HI_U32 u32VpssChnNum = 1;
    VO_DEV VoDev;
    VO_LAYER VoLayer;
    HI_U32 s32VoChnNum;
    RGN_ATTR_INFO_S stRgnAttrInfo;
	VO_PUB_ATTR_S stVoPubAttr;
	VO_CSC_S	stCSC;
    PIC_SIZE_E enPicSize = g_enPicSize;
    VB_CONF_S stVbConf;
    HI_U32 u32BlkSize;

    memset(&stVbConf, 0, sizeof(VB_CONF_S));

    u32BlkSize = SAMPLE_COMM_SYS_CalcPicVbBlkSize(gs_enNorm, enPicSize,
                 SAMPLE_PIXEL_FORMAT, SAMPLE_SYS_ALIGN_WIDTH);
    stVbConf.u32MaxPoolCnt = 128;

    /*ddr0 video buffer*/
    stVbConf.astCommPool[0].u32BlkSize = u32BlkSize;
    stVbConf.astCommPool[0].u32BlkCnt  = 6;

    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("system init failed with %d!\n", s32Ret);
        return s32Ret;
    }
    
    /*************************************************
      step 1: create region and attach to vpss group
     *************************************************/
    Handle    = 0;
    u32RgnNum = 8;
    s32Ret = SAMPLE_RGN_CreateCover(Handle, u32RgnNum);
    if(HI_SUCCESS != s32Ret)
    {
        printf("SAMPLE_RGN_CreateCover failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }
    
    /*************************************************
      step 2: start vi chn
     *************************************************/
    s32Ret = SAMPLE_COMM_VI_StartVi(&g_stViChnConfig);
    if(HI_SUCCESS != s32Ret)
    {
        printf("SAMPLE_COMM_VI_StartVi failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }
    /*************************************************
      step 3: start vpss group and chn
     *************************************************/
    s32VpssGrpNum = 1;
    if(VO_INTF_BT656 == g_enVoIntfType)
    {
        u32VpssChn    = VPSS_CHN0;
        u32VpssChnNum = 2;
        s32Ret = SAMPLE_RGN_StartVpssHD(s32VpssGrpNum, u32VpssChnNum);
        if(HI_SUCCESS != s32Ret)
        {
            printf("SAMPLE_RGN_StartVpssHD failed! s32Ret: 0x%x.\n", s32Ret);
            return s32Ret;
        }
    }
    else
    {
        u32VpssChn    = VPSS_CHN0;
        u32VpssChnNum = 2;
        s32Ret = SAMPLE_RGN_StartVpss(s32VpssGrpNum, u32VpssChnNum);
        if(HI_SUCCESS != s32Ret)
        {
            printf("SAMPLE_RGN_StartVpss failed! s32Ret: 0x%x.\n", s32Ret);
            return s32Ret;
        }
    }
    
    /*************************************************
      step 4: start vo dev and chn
     *************************************************/
    VoDev       = 0;
    s32VoChnNum = 1;

	stVoPubAttr.enIntfType = g_enVoIntfType;
	stVoPubAttr.u32BgColor = 0x0;
	if (VO_INTF_BT656 == g_enVoIntfType)
    {
        stVoPubAttr.enIntfSync  = VO_OUTPUT_PAL;
    }
	else
	{
		stVoPubAttr.enIntfType = INTF_LCD;
        stVoPubAttr.enIntfSync = SYNC_LCD;
        stCSC.enCscMatrix = VO_CSC_MATRIX_BT709_TO_RGB_PC;
        stCSC.u32Contrast = 50;
        stCSC.u32Hue = 50;
        stCSC.u32Luma = 50;
        stCSC.u32Satuature = 50;
        HI_MPI_VO_SetVideoLayerCSC(0, &stCSC);
	}
	
    s32Ret = SAMPLE_RGN_StartVo(VoDev, s32VoChnNum, &stVoPubAttr);
    if(HI_SUCCESS != s32Ret)
    {
        printf("SAMPLE_RGN_StartVo failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }
    
    /*************************************************
      step 5: bind vi and vpss
     *************************************************/
    stSrcChn.enModId  = HI_ID_VIU;
    stSrcChn.s32DevId = 0;
    stSrcChn.s32ChnId = 0;

    stDesChn.enModId  = HI_ID_VPSS;
    stDesChn.s32DevId = 0;
    stDesChn.s32ChnId = 0;

    s32Ret = HI_MPI_SYS_Bind(&stSrcChn, &stDesChn);
    if(HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_SYS_Bind failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }

    /*************************************************
      step 6: bind vpss and vo
     *************************************************/
    VoLayer = SAMPLE_RGN_GetVoLayer(VoDev);
    if(VoLayer < 0)
    {
        printf("SAMPLE_RGN_GetVoLayer failed! VoDev: %d.\n", VoDev);
        return HI_FAILURE;
    }
    
    stSrcChn.enModId  = HI_ID_VPSS;
    stSrcChn.s32DevId = 0;
    stSrcChn.s32ChnId = u32VpssChn;

    stDesChn.enModId  = HI_ID_VOU;
    stDesChn.s32DevId = VoLayer;
    stDesChn.s32ChnId = 0;

    s32Ret = HI_MPI_SYS_Bind(&stSrcChn, &stDesChn);
    if(HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_SYS_Bind failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }

    /*************************************************
      step 7: create a thread to change region's 
      layer, position, alpha and other display attribute
     *************************************************/
    stRgnAttrInfo.Handle    = Handle;
    stRgnAttrInfo.u32RgnNum = u32RgnNum;
    pthread_create(&g_stRgnThread, NULL, SAMPLE_RGN_VpssCoverDynamicDisplay, (HI_VOID*)&stRgnAttrInfo);

    printf("\n#############Sample start ok! Press Enter to switch!#############\n");


    /*************************************************
      step 8: stop thread and release all the resource
     *************************************************/
    getchar();
    bExit = HI_TRUE;

    if (g_stRgnThread)
    {
        pthread_join(g_stRgnThread, 0);
        g_stRgnThread = 0;
    }
    
    bExit = HI_FALSE;
    
    /*************************************************
      step 9: unbind vpss and vo
     *************************************************/
    s32Ret = HI_MPI_SYS_UnBind(&stSrcChn, &stDesChn);
    if(HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_SYS_UnBind failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }
    
    /*************************************************
      step 10: unbind vi and vpss
     *************************************************/
    stSrcChn.enModId  = HI_ID_VIU;
    stSrcChn.s32DevId = 0;
    stSrcChn.s32ChnId = 0;

    stDesChn.enModId  = HI_ID_VPSS;
    stDesChn.s32DevId = 0;
    stDesChn.s32ChnId = 0;
    
    s32Ret = HI_MPI_SYS_UnBind(&stSrcChn, &stDesChn);
    if(HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_SYS_UnBind failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }

    /*************************************************
      step 11: stop vo dev and chn
     *************************************************/
    s32Ret = SAMPLE_RGN_StopVoChn(VoDev, s32VoChnNum);
    if(HI_SUCCESS != s32Ret)
    {
        printf("SAMPLE_RGN_StopVoChn failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }
    
    s32Ret = SAMPLE_RGN_StopVoDev(VoDev);
    if(HI_SUCCESS != s32Ret)
    {
        printf("SAMPLE_RGN_StopVoDev failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }
    
    /*************************************************
      step 12: stop vpss group and chn
     *************************************************/
    s32Ret = SAMPLE_RGN_StopVpss(s32VpssGrpNum);
    if(HI_SUCCESS != s32Ret)
    {
        printf("SAMPLE_RGN_StopVpss failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }
    
    /*************************************************
      step 13: stop vi chn
     *************************************************/
    s32Ret = SAMPLE_COMM_VI_StopVi(&g_stViChnConfig);
    if(HI_SUCCESS != s32Ret)
    {
        printf("SAMPLE_COMM_VI_StopVi failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }
    
    /*************************************************
      step 14: destroy region
     *************************************************/
    s32Ret = SAMPLE_RGN_DestroyRegion(Handle, u32RgnNum);
    if(HI_SUCCESS != s32Ret)
    {
        printf("SAMPLE_RGN_DestroyRegion failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }
    
    s32Ret = SAMPLE_RGN_SYS_Exit();
    if(HI_SUCCESS != s32Ret)
    {
        printf("SAMPLE_RGN_SYS_Exit failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }
    
    return HI_SUCCESS;
}

/******************************************************************************************
    function :  Vpss coverex
    process steps:                                      
    0) create some coverex regions                
    1) attach them to vpss   
    2) change Regions' Layer                        
    3) change Regions' position                     
    4) change Regions' alpha (front and backgroud)  
******************************************************************************************/

HI_S32 SAMPLE_RGN_AddCoverExToVpss(HI_VOID)
{
    HI_S32 s32Ret = HI_SUCCESS;
    RGN_HANDLE Handle;
    HI_U32 u32RgnNum;
    MPP_CHN_S stSrcChn, stDesChn;
    HI_S32 s32VpssGrpNum;
    HI_U32 u32VpssChn = 0;
    HI_U32 u32VpssChnNum = 1;
    VO_DEV VoDev;
    VO_LAYER VoLayer;
    HI_U32 s32VoChnNum;
    RGN_ATTR_INFO_S stRgnAttrInfo;
	VO_PUB_ATTR_S stVoPubAttr;
	VO_CSC_S	stCSC;
    PIC_SIZE_E enPicSize = g_enPicSize;
    VB_CONF_S stVbConf;
    HI_U32 u32BlkSize;

    memset(&stVbConf, 0, sizeof(VB_CONF_S));

    u32BlkSize = SAMPLE_COMM_SYS_CalcPicVbBlkSize(gs_enNorm, enPicSize,
                 SAMPLE_PIXEL_FORMAT, SAMPLE_SYS_ALIGN_WIDTH);
    stVbConf.u32MaxPoolCnt = 128;

    /*ddr0 video buffer*/
    stVbConf.astCommPool[0].u32BlkSize = u32BlkSize;
    stVbConf.astCommPool[0].u32BlkCnt  = 6;

    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("system init failed with %d!\n", s32Ret);
        return s32Ret;
    }
    
    /*************************************************
      step 1: create region and attach to vpss group
     *************************************************/
    Handle    = 0;
    u32RgnNum = 8;
    s32Ret = SAMPLE_RGN_CreateCoverEx(Handle, u32RgnNum);
    if(HI_SUCCESS != s32Ret)
    {
        printf("SAMPLE_RGN_CreateCoverEx failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }
    
    /*************************************************
      step 2: start vi chn
     *************************************************/
    s32Ret = SAMPLE_COMM_VI_StartVi(&g_stViChnConfig);
    if(HI_SUCCESS != s32Ret)
    {
        printf("SAMPLE_COMM_VI_StartVi failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }

    /*************************************************
      step 3: start vpss group and chn
     *************************************************/
    s32VpssGrpNum = 1;
    if(VO_INTF_BT656 == g_enVoIntfType)
    {
        u32VpssChn    = VPSS_CHN0;
        u32VpssChnNum = 2;
        s32Ret = SAMPLE_RGN_StartVpssHD(s32VpssGrpNum, u32VpssChnNum);
        if(HI_SUCCESS != s32Ret)
        {
            printf("SAMPLE_RGN_StartVpssHD failed! s32Ret: 0x%x.\n", s32Ret);
            return s32Ret;
        }
    }
    else
    {
        u32VpssChn    = VPSS_CHN0;
        u32VpssChnNum= 2;
        s32Ret = SAMPLE_RGN_StartVpss(s32VpssGrpNum, u32VpssChnNum);
        if(HI_SUCCESS != s32Ret)
        {
            printf("SAMPLE_RGN_StartVpss failed! s32Ret: 0x%x.\n", s32Ret);
            return s32Ret;
        }
    }
    
    /*************************************************
      step 4: start vo dev and chn
     *************************************************/
    VoDev       = 0;
    s32VoChnNum = 1;

	stVoPubAttr.enIntfType = g_enVoIntfType;
	stVoPubAttr.u32BgColor = 0x0;
	if (VO_INTF_BT656 == g_enVoIntfType)
    {
        stVoPubAttr.enIntfSync  = VO_OUTPUT_PAL;
    }
	else
	{
		stVoPubAttr.enIntfType = INTF_LCD;
        stVoPubAttr.enIntfSync = SYNC_LCD;
        stCSC.enCscMatrix = VO_CSC_MATRIX_BT709_TO_RGB_PC;
        stCSC.u32Contrast = 50;
        stCSC.u32Hue = 50;
        stCSC.u32Luma = 50;
        stCSC.u32Satuature = 50;
        HI_MPI_VO_SetVideoLayerCSC(0, &stCSC);
	}
	
    s32Ret = SAMPLE_RGN_StartVo(VoDev, s32VoChnNum, &stVoPubAttr);
	if(HI_SUCCESS != s32Ret)
    {
        printf("SAMPLE_RGN_StartVo failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }
    
    /*************************************************
      step 5: bind vi and vpss
     *************************************************/
    stSrcChn.enModId  = HI_ID_VIU;
    stSrcChn.s32DevId = 0;
    stSrcChn.s32ChnId = 0;

    stDesChn.enModId  = HI_ID_VPSS;
    stDesChn.s32DevId = 0;
    stDesChn.s32ChnId = 0;

    s32Ret = HI_MPI_SYS_Bind(&stSrcChn, &stDesChn);
    if(HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_SYS_Bind failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }

    /*************************************************
      step 6: bind vpss and vo
     *************************************************/
    VoLayer = SAMPLE_RGN_GetVoLayer(VoDev);
    if(VoLayer < 0)
    {
        printf("SAMPLE_RGN_GetVoLayer failed! VoDev: %d.\n", VoDev);
        return HI_FAILURE;
    }
    
    stSrcChn.enModId  = HI_ID_VPSS;
    stSrcChn.s32DevId = 0;
    stSrcChn.s32ChnId = u32VpssChn;

    stDesChn.enModId  = HI_ID_VOU;
    stDesChn.s32DevId = VoLayer;
    stDesChn.s32ChnId = 0;

    s32Ret = HI_MPI_SYS_Bind(&stSrcChn, &stDesChn);
    if(HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_SYS_Bind failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }

    /*************************************************
      step 7: create a thread to change region's 
      layer, position, alpha and other display attribute
     *************************************************/
    stRgnAttrInfo.Handle    = Handle;
    stRgnAttrInfo.u32RgnNum = u32RgnNum;
    pthread_create(&g_stRgnThread, NULL, SAMPLE_RGN_VpssCoverExDynamicDisplay, (HI_VOID*)&stRgnAttrInfo);

    printf("\n#############Sample start ok! Press Enter to switch!#############\n");


    /*************************************************
      step 8: stop thread and release all the resource
     *************************************************/
    getchar();
    bExit = HI_TRUE;

    if (g_stRgnThread)
    {
        pthread_join(g_stRgnThread, 0);
        g_stRgnThread = 0;
    }
    
    bExit = HI_FALSE;
    
    /*************************************************
      step 9: unbind vpss and vo
     *************************************************/
    s32Ret = HI_MPI_SYS_UnBind(&stSrcChn, &stDesChn);
    if(HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_SYS_UnBind failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }
    
    /*************************************************
      step 10: unbind vi and vpss
     *************************************************/
    stSrcChn.enModId  = HI_ID_VIU;
    stSrcChn.s32DevId = 0;
    stSrcChn.s32ChnId = 0;

    stDesChn.enModId  = HI_ID_VPSS;
    stDesChn.s32DevId = 0;
    stDesChn.s32ChnId = 0;
    
    s32Ret = HI_MPI_SYS_UnBind(&stSrcChn, &stDesChn);
    if(HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_SYS_UnBind failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }

    /*************************************************
      step 11: stop vo dev and chn
     *************************************************/
    s32Ret = SAMPLE_RGN_StopVoChn(VoDev, s32VoChnNum);
    if(HI_SUCCESS != s32Ret)
    {
        printf("SAMPLE_RGN_StopVoChn failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }
    
    s32Ret = SAMPLE_RGN_StopVoDev(VoDev);
    if(HI_SUCCESS != s32Ret)
    {
        printf("SAMPLE_RGN_StopVoDev failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }
    
    /*************************************************
      step 12: stop vpss group and chn
     *************************************************/
    s32Ret = SAMPLE_RGN_StopVpss(s32VpssGrpNum);
    if(HI_SUCCESS != s32Ret)
    {
        printf("SAMPLE_RGN_StopVpss failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }
    
    /*************************************************
      step 13: stop vi chn
     *************************************************/
    s32Ret = SAMPLE_COMM_VI_StopVi(&g_stViChnConfig);
    if(HI_SUCCESS != s32Ret)
    {
        printf("SAMPLE_COMM_VI_StopVi failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }
    
    /*************************************************
      step 14: destroy region
     *************************************************/
    s32Ret = SAMPLE_RGN_DestroyRegion(Handle, u32RgnNum);
    if(HI_SUCCESS != s32Ret)
    {
        printf("SAMPLE_RGN_DestroyRegion failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }
    
    s32Ret = SAMPLE_RGN_SYS_Exit();
    if(HI_SUCCESS != s32Ret)
    {
        printf("SAMPLE_RGN_SYS_Exit failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }
    
    return HI_SUCCESS;
}

/******************************************************************************************
    function :  Vpss Osd
    process steps:                                      
    0) create some osd regions                
    1) attach them to vpss   
    2) start a thread to handle color reverse   
    3) change Regions' Layer                        
    4) change Regions' position                     
    5) change Regions' alpha (front and backgroud)  
******************************************************************************************/

HI_S32 SAMPLE_RGN_AddOsdToVpss(HI_VOID)
{
    RGN_HANDLE Handle;
    HI_S32 u32RgnNum;
    HI_S32 s32Ret = HI_SUCCESS;
    RGN_ATTR_S stRgnAttrSet;
    RGN_CANVAS_INFO_S stCanvasInfo;
    RGN_CHN_ATTR_S stChnAttr;
    MPP_CHN_S stChn;
    HI_S32 i;
    BITMAP_S stBitmap;
    HI_S32 s32VpssGrpNum;
    HI_U32 u32VpssChn = 0;
    HI_U32 u32VpssChnNum = 1;
    VO_DEV VoDev;
    VO_LAYER VoLayer;
    HI_U32 s32VoChnNum;
    MPP_CHN_S stSrcChn, stDesChn;
    RGN_OSD_REVERSE_INFO_S stOsdReverseInfo;
    RGN_ATTR_INFO_S stRgnAttrInfo;
    RECT_S astOsdLumaRect[3];
    HI_U32 u32OsdRectCnt;
    SIZE_S stSize;
	VO_PUB_ATTR_S stVoPubAttr;
	VO_CSC_S	stCSC;
    PIC_SIZE_E enPicSize = g_enPicSize;
    VB_CONF_S stVbConf;
    HI_U32 u32BlkSize;

    memset(&stVbConf, 0, sizeof(VB_CONF_S));

    u32BlkSize = SAMPLE_COMM_SYS_CalcPicVbBlkSize(gs_enNorm, enPicSize,
                 SAMPLE_PIXEL_FORMAT, SAMPLE_SYS_ALIGN_WIDTH);
    stVbConf.u32MaxPoolCnt = 128;

    /*ddr0 video buffer*/
    stVbConf.astCommPool[0].u32BlkSize = u32BlkSize;
    stVbConf.astCommPool[0].u32BlkCnt  = 6;

    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("system init failed with %d!\n", s32Ret);
        return s32Ret;
    }
        
    /*************************************************
      step 1: create region and attach to vpss group
     *************************************************/
    Handle    = 0;
    u32RgnNum = 4;
    s32Ret = SAMPLE_RGN_CreateOverlayExForVpss(Handle, u32RgnNum);
    if(HI_SUCCESS != s32Ret)
    {
        printf("SAMPLE_RGN_CreateOverlayExForVpss failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }

    /*************************************************
      step 2: start vi chn
     *************************************************/
    s32Ret = SAMPLE_COMM_VI_StartVi(&g_stViChnConfig);
    if(HI_SUCCESS != s32Ret)
    {
        printf("SAMPLE_COMM_VI_StartVi failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }
    
    /*************************************************
      step 3: start vpss group and chn
     *************************************************/
    s32VpssGrpNum = 1;
    if(VO_INTF_BT656 == g_enVoIntfType)
    {
        u32VpssChn    = VPSS_CHN0;
        u32VpssChnNum = 2;
        s32Ret = SAMPLE_RGN_StartVpssHD(s32VpssGrpNum, u32VpssChnNum);
        if(HI_SUCCESS != s32Ret)
        {
            printf("SAMPLE_RGN_StartVpssHD failed! s32Ret: 0x%x.\n", s32Ret);
            return s32Ret;
        }
    }
    else
    {
        u32VpssChn    = VPSS_CHN0;
        u32VpssChnNum = 2;
        s32Ret = SAMPLE_RGN_StartVpss(s32VpssGrpNum, u32VpssChnNum);
        if(HI_SUCCESS != s32Ret)
        {
            printf("SAMPLE_RGN_StartVpss failed! s32Ret: 0x%x.\n", s32Ret);
            return s32Ret;
        }
    }
    
    /*************************************************
      step 4: start vo dev and chn
     *************************************************/
    VoDev       = 0;
    s32VoChnNum = 1;

	stVoPubAttr.enIntfType = g_enVoIntfType;
	stVoPubAttr.u32BgColor = 0x0;
	if (VO_INTF_BT656 == g_enVoIntfType)
    {
        stVoPubAttr.enIntfSync  = VO_OUTPUT_PAL;
    }
	else
	{
		stVoPubAttr.enIntfType = INTF_LCD;
        stVoPubAttr.enIntfSync = SYNC_LCD;
        stCSC.enCscMatrix = VO_CSC_MATRIX_BT709_TO_RGB_PC;
        stCSC.u32Contrast = 50;
        stCSC.u32Hue = 50;
        stCSC.u32Luma = 50;
        stCSC.u32Satuature = 50;
        HI_MPI_VO_SetVideoLayerCSC(0, &stCSC);
	}
	
    s32Ret = SAMPLE_RGN_StartVo(VoDev, s32VoChnNum, &stVoPubAttr);
	if(HI_SUCCESS != s32Ret)
    {
        printf("SAMPLE_RGN_StartVo failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }
    
    /*************************************************
      step 5: bind vi and vpss
     *************************************************/
    stSrcChn.enModId  = HI_ID_VIU;
    stSrcChn.s32DevId = 0;
    stSrcChn.s32ChnId = 0;

    stDesChn.enModId  = HI_ID_VPSS;
    stDesChn.s32DevId = 0;
    stDesChn.s32ChnId = 0;

    s32Ret = HI_MPI_SYS_Bind(&stSrcChn, &stDesChn);
    if(HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_SYS_Bind failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }
    
    /*************************************************
      step 6: bind vpss and vo
     *************************************************/
    VoLayer = SAMPLE_RGN_GetVoLayer(VoDev);
    if(VoLayer < 0)
    {
        printf("SAMPLE_RGN_GetVoLayer failed! VoDev: %d.\n", VoDev);
        return HI_FAILURE;
    } 
    
    stSrcChn.enModId  = HI_ID_VPSS;
    stSrcChn.s32DevId = 0;
    stSrcChn.s32ChnId = u32VpssChn;

    stDesChn.enModId  = HI_ID_VOU;
    stDesChn.s32DevId = VoLayer;
    stDesChn.s32ChnId = 0;

    s32Ret = HI_MPI_SYS_Bind(&stSrcChn, &stDesChn);
    if(HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_SYS_Bind failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }

    /*************************************************
      step 7: load bitmap to region
     *************************************************/
    s32Ret = HI_MPI_RGN_GetAttr(Handle, &stRgnAttrSet);
    if(HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_RGN_GetAttr failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }
    
    s32Ret = HI_MPI_RGN_GetCanvasInfo(Handle, &stCanvasInfo);
    if(HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_RGN_GetCanvasInfo failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }    
     
    stBitmap.pData   = (HI_VOID *)stCanvasInfo.u32VirtAddr;
    stSize.u32Width  = stCanvasInfo.stSize.u32Width;
    stSize.u32Height = stCanvasInfo.stSize.u32Height;
    s32Ret = SAMPLE_RGN_UpdateCanvas("mm.bmp", &stBitmap, HI_FALSE, 0, &stSize, stCanvasInfo.u32Stride, 
        stRgnAttrSet.unAttr.stOverlayEx.enPixelFmt);
    if(HI_SUCCESS != s32Ret)
    {
        printf("SAMPLE_RGN_UpdateCanvas failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }
    
    s32Ret = HI_MPI_RGN_UpdateCanvas(Handle);
    if(HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_RGN_UpdateCanvas failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }

    /*************************************************
      step 8: create a thread to change region's 
      layer, position, alpha and other display attribute
     *************************************************/
    stRgnAttrInfo.Handle    = Handle;
    stRgnAttrInfo.u32RgnNum = u32RgnNum; 
    pthread_create(&g_stRgnThread, NULL, SAMPLE_RGN_VpssOSdDynamicDisplay, (HI_VOID*)&stRgnAttrInfo);
    sleep(3);
    /*************************************************
      step 10: start a thread to handle osd color reverse
     *************************************************/
    u32OsdRectCnt                            = 3;
    stOsdReverseInfo.Handle                  = Handle;
    stOsdReverseInfo.VpssGrp                 = 0;
    stOsdReverseInfo.VpssChn                 = u32VpssChn;
    stOsdReverseInfo.u8PerPixelLumaThrd      = 128;
    stOsdReverseInfo.stLumaRgnInfo.u32RegionNum   = u32OsdRectCnt;
    stOsdReverseInfo.stLumaRgnInfo.pstRegion = astOsdLumaRect;
    
    stSize.u32Width  = stRgnAttrSet.unAttr.stOverlayEx.stSize.u32Width;
    stSize.u32Height = stRgnAttrSet.unAttr.stOverlayEx.stSize.u32Height;

    stChn.enModId  = HI_ID_VPSS;
    stChn.s32DevId = 0;
    stChn.s32ChnId = u32VpssChn;
    s32Ret = HI_MPI_RGN_GetDisplayAttr(Handle, &stChn, &stChnAttr);
    if(HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_RGN_GetDisplayAttr failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }
    
    for (i=0; i < u32OsdRectCnt; i++)
    {
        astOsdLumaRect[i].s32X = ((stSize.u32Width/ u32OsdRectCnt) * i) + stChnAttr.unChnAttr.stOverlayExChn.stPoint.s32X;
        astOsdLumaRect[i].s32Y = stChnAttr.unChnAttr.stOverlayExChn.stPoint.s32Y;
        astOsdLumaRect[i].u32Width  = (stSize.u32Width / u32OsdRectCnt);
        astOsdLumaRect[i].u32Height = stSize.u32Height;
    }
    pthread_create(&g_stVpssOsdReverseThread, NULL, SAMPLE_RGN_VpssOsdReverse, (HI_VOID*)&stOsdReverseInfo);

    printf("\n#############Sample start ok! Press Enter to switch!#############\n");

    

    /*************************************************
      step 11: stop thread and release all the resource
     *************************************************/
    getchar();
    bExit = HI_TRUE;

    if (g_stVpssOsdReverseThread)
    {
        pthread_join(g_stVpssOsdReverseThread, 0);
        g_stVpssOsdReverseThread = 0;
    }

    if (g_stRgnThread)
    {
        pthread_join(g_stRgnThread, 0);
        g_stRgnThread = 0;
    }
    
    bExit = HI_FALSE;
    
    /*************************************************
      step 12: unbind vpss and vo
     *************************************************/
    s32Ret = HI_MPI_SYS_UnBind(&stSrcChn, &stDesChn);
    if(HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_SYS_UnBind failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }
    
    /*************************************************
      step 13: unbind vdec and vpss
     *************************************************/
    stSrcChn.enModId  = HI_ID_VIU;
    stSrcChn.s32DevId = 0;
    stSrcChn.s32ChnId = 0;

    stDesChn.enModId  = HI_ID_VPSS;
    stDesChn.s32DevId = 0;
    stDesChn.s32ChnId = 0;
    
    s32Ret = HI_MPI_SYS_UnBind(&stSrcChn, &stDesChn);
    if(HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_SYS_UnBind failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }

    /*************************************************
      step 14: stop vo dev and chn
     *************************************************/
    s32Ret = SAMPLE_RGN_StopVoChn(VoDev, s32VoChnNum);
    if(HI_SUCCESS != s32Ret)
    {
        printf("SAMPLE_RGN_StopVoChn failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }
    
    s32Ret = SAMPLE_RGN_StopVoDev(VoDev);
    if(HI_SUCCESS != s32Ret)
    {
        printf("SAMPLE_RGN_StopVoDev failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }
    
    /*************************************************
      step 15: stop vpss group and chn
     *************************************************/
    s32Ret = SAMPLE_RGN_StopVpss(s32VpssGrpNum);
    if(HI_SUCCESS != s32Ret)
    {
        printf("SAMPLE_RGN_StopVpss failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }
    
    /*************************************************
      step 16: stop vi chn
     *************************************************/
    s32Ret = SAMPLE_COMM_VI_StopVi(&g_stViChnConfig);
    if(HI_SUCCESS != s32Ret)
    {
        printf("SAMPLE_COMM_VI_StopVi failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }
    
    /*************************************************
      step 17: destroy region
     *************************************************/
    s32Ret = SAMPLE_RGN_DestroyRegion(Handle, u32RgnNum);
    if(HI_SUCCESS != s32Ret)
    {
        printf("SAMPLE_RGN_DestroyRegion failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }
    
    s32Ret = SAMPLE_RGN_SYS_Exit();
    if(HI_SUCCESS != s32Ret)
    {
        printf("SAMPLE_RGN_SYS_Exit failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }
    
    return HI_SUCCESS;
}

/******************************************************************************************
    function :  Venc And Vpss OSD
    process steps:                                                          
      1) create some cover/osd regions                                  
      2) display  cover/osd regions ( One Region -- Multi-VencGroup )   
      3) change  Regions' Layer                           
      4) change  Regions' position                        
      5) change  Regions' color                           
      6) change  Regions' alpha (front and backgroud)     
      7) load bmp form bmp-file to Region-0                             
      8) change BmpRegion-0   
      9) enable color reverse for Region-0
******************************************************************************************/

HI_S32 SAMPLE_RGN_AddOsdToVencAndVpss(HI_VOID)
{
    HI_S32 s32Ret = HI_SUCCESS;
    RGN_HANDLE Handle;
    RGN_HANDLE VencOsdHandle;
    RGN_HANDLE VpssOsdHandle;
    HI_S32 u32VencRgnNum;
    HI_S32 u32VpssRgnNum;
    RGN_ATTR_S stRgnAttrSet;
    RGN_CANVAS_INFO_S stCanvasInfo;
    BITMAP_S stBitmap;
    MPP_CHN_S stSrcChn, stDesChn;
    RGN_CHN_ATTR_S stChnAttr;
    MPP_CHN_S stChn;
    HI_S32 i;

    VO_DEV VoDev;
    VO_LAYER VoLayer;
    HI_U32 s32VoChnNum;
    
    VENC_CHN VencChn;
    HI_S32 s32VpssGrpNum;
    HI_U32 u32VpssChn;
    HI_U32 u32VpssChnNum = 1;
    VENC_PTHREAD_INFO_S stVencGetPram;
    RGN_OSD_REVERSE_INFO_S stVpssOsdReverseInfo;
    RGN_OSD_REVERSE_INFO_S stVencOsdReverseInfo;
    RGN_ATTR_INFO_S stRgnAttrInfo;
    RECT_S astVpssOsdLumaRect[3];
    RECT_S astVencOsdLumaRect[3];
    HI_U32 u32VpssOsdRectCnt;
    HI_U32 u32VencOsdRectCnt;
    SIZE_S stSize;
    FILE * pastream = NULL;
	VO_PUB_ATTR_S stVoPubAttr;
	VO_CSC_S	stCSC;
    PIC_SIZE_E enPicSize = g_enPicSize;
    VB_CONF_S stVbConf;
    HI_U32 u32BlkSize;

    memset(&stVbConf, 0, sizeof(VB_CONF_S));

    u32BlkSize = SAMPLE_COMM_SYS_CalcPicVbBlkSize(gs_enNorm, enPicSize,
                 SAMPLE_PIXEL_FORMAT, SAMPLE_SYS_ALIGN_WIDTH);
    stVbConf.u32MaxPoolCnt = 128;

    /*ddr0 video buffer*/
    stVbConf.astCommPool[0].u32BlkSize = u32BlkSize;
    stVbConf.astCommPool[0].u32BlkCnt  = 4;

    stVbConf.astCommPool[1].u32BlkSize = 768*576*2;
    stVbConf.astCommPool[1].u32BlkCnt  = 4;

    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("system init failed with %d!\n", s32Ret);
        return s32Ret;
    }
    
    /*************************************************
      step 1: create region and attach to venc
     *************************************************/
    VencOsdHandle    = 0;
    u32VencRgnNum = 4;
    s32Ret = SAMPLE_RGN_CreateOverlayForVenc(VencOsdHandle, u32VencRgnNum);
    if(HI_SUCCESS != s32Ret)
    {
        printf("SAMPLE_RGN_CreateOverlayForVenc failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }

    /*************************************************
      step 2: create region and attach to vpss group
     *************************************************/
    VpssOsdHandle    = u32VencRgnNum;
    u32VpssRgnNum = 4;
    s32Ret = SAMPLE_RGN_CreateOverlayExForVpss(VpssOsdHandle, u32VpssRgnNum);
    if(HI_SUCCESS != s32Ret)
    {
        printf("SAMPLE_RGN_CreateOverlayExForVpss failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }
    
    /*************************************************
      step 3: start vi chn
     *************************************************/
    s32Ret = SAMPLE_COMM_VI_StartVi(&g_stViChnConfig);
    if(HI_SUCCESS != s32Ret)
    {
        printf("SAMPLE_COMM_VI_StartVi failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }

    /*************************************************
      step 4: start vpss group and chn
     *************************************************/
    s32VpssGrpNum = 1;
    if(VO_INTF_BT656 == g_enVoIntfType)
    {
        u32VpssChn    = VPSS_CHN0;
        u32VpssChnNum = 2;
        s32Ret = SAMPLE_RGN_StartVpssHD(s32VpssGrpNum, u32VpssChnNum);
        if(HI_SUCCESS != s32Ret)
        {
            printf("SAMPLE_RGN_StartVpssHD failed! s32Ret: 0x%x.\n", s32Ret);
            return s32Ret;
        }
    }
    else
    {
        u32VpssChn    = VPSS_CHN0;
        u32VpssChnNum = 2;
        s32Ret = SAMPLE_RGN_StartVpss(s32VpssGrpNum, u32VpssChnNum);
        if(HI_SUCCESS != s32Ret)
        {
            printf("SAMPLE_RGN_StartVpss failed! s32Ret: 0x%x.\n", s32Ret);
            return s32Ret;
        }
    }

    /*************************************************
      step 5: start vo dev and chn
     *************************************************/
    VoDev       = 0;
    s32VoChnNum = 1;

	stVoPubAttr.enIntfType = g_enVoIntfType;
	stVoPubAttr.u32BgColor = 0x0;
	if (VO_INTF_BT656 == g_enVoIntfType)
    {
        stVoPubAttr.enIntfSync  = VO_OUTPUT_PAL;
    }
	else
	{
		stVoPubAttr.enIntfType = INTF_LCD;
        stVoPubAttr.enIntfSync = SYNC_LCD;
        stCSC.enCscMatrix = VO_CSC_MATRIX_BT709_TO_RGB_PC;
        stCSC.u32Contrast = 50;
        stCSC.u32Hue = 50;
        stCSC.u32Luma = 50;
        stCSC.u32Satuature = 50;
        HI_MPI_VO_SetVideoLayerCSC(0, &stCSC);
	}
	
    s32Ret = SAMPLE_RGN_StartVo(VoDev, s32VoChnNum, &stVoPubAttr);
	if(HI_SUCCESS != s32Ret)
    {
        printf("SAMPLE_RGN_StartVo failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }
    
    /*************************************************
      step 6: bind vi and vpss
     *************************************************/
    stSrcChn.enModId  = HI_ID_VIU;
    stSrcChn.s32DevId = 0;
    stSrcChn.s32ChnId = 0;

    stDesChn.enModId  = HI_ID_VPSS;
    stDesChn.s32DevId = 0;
    stDesChn.s32ChnId = 0;

    s32Ret = HI_MPI_SYS_Bind(&stSrcChn, &stDesChn);
    if(HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_SYS_Bind failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }

    /*************************************************
      step 7: bind vpss and vo
     *************************************************/
    VoLayer = SAMPLE_RGN_GetVoLayer(VoDev);
    if(VoLayer < 0)
    {
        printf("SAMPLE_RGN_GetVoLayer failed! VoDev: %d.\n", VoDev);
        return HI_FAILURE;
    }
    
    stSrcChn.enModId  = HI_ID_VPSS;
    stSrcChn.s32DevId = 0;
    stSrcChn.s32ChnId = u32VpssChn;

    stDesChn.enModId  = HI_ID_VOU;
    stDesChn.s32DevId = VoLayer;
    stDesChn.s32ChnId = 0;

    s32Ret = HI_MPI_SYS_Bind(&stSrcChn, &stDesChn);
    if(HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_SYS_Bind failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }
    /*************************************************
      step 8: start venc chn
     *************************************************/
    VencChn = 0;
    s32Ret = SAMPLE_RGN_StartVenc(VencChn);
    if(HI_SUCCESS != s32Ret)
    {
        printf("SAMPLE_RGN_StartVenc failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }
    
    /*************************************************
      step 9: bind vpss and venc
     *************************************************/
    stSrcChn.enModId  = HI_ID_VPSS;
    stSrcChn.s32DevId = 0;
    stSrcChn.s32ChnId = VPSS_CHN1;

    stDesChn.enModId  = HI_ID_VENC;
    stDesChn.s32DevId = 0;
    stDesChn.s32ChnId = 0;

    s32Ret = HI_MPI_SYS_Bind(&stSrcChn, &stDesChn);
    if(HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_SYS_Bind failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }

    /*************************************************
      step 10: load bitmap to region
     *************************************************/
    for (Handle = VencOsdHandle; Handle <= u32VencRgnNum; Handle += u32VencRgnNum)
    {
        s32Ret = HI_MPI_RGN_GetAttr(Handle, &stRgnAttrSet);
        if(HI_SUCCESS != s32Ret)
        {
            printf("HI_MPI_RGN_GetAttr failed! s32Ret: 0x%x.\n", s32Ret);
            return s32Ret;
        }
        
        s32Ret = HI_MPI_RGN_GetCanvasInfo(Handle, &stCanvasInfo);
        if(HI_SUCCESS != s32Ret)
        {
            printf("HI_MPI_RGN_GetCanvasInfo failed! s32Ret: 0x%x.\n", s32Ret);
            return s32Ret;
        }    
         
        stBitmap.pData   = (HI_VOID *)stCanvasInfo.u32VirtAddr;
        stSize.u32Width  = stCanvasInfo.stSize.u32Width;
        stSize.u32Height = stCanvasInfo.stSize.u32Height;
        s32Ret = SAMPLE_RGN_UpdateCanvas("mm.bmp", &stBitmap, HI_FALSE, 0, &stSize, stCanvasInfo.u32Stride, 
            stRgnAttrSet.unAttr.stOverlayEx.enPixelFmt);
        if(HI_SUCCESS != s32Ret)
        {
            printf("SAMPLE_RGN_UpdateCanvas failed! s32Ret: 0x%x.\n", s32Ret);
            return s32Ret;
        }
        
        s32Ret = HI_MPI_RGN_UpdateCanvas(Handle);
        if(HI_SUCCESS != s32Ret)
        {
            printf("HI_MPI_RGN_UpdateCanvas failed! s32Ret: 0x%x.\n", s32Ret);
            return s32Ret;
        }
    }

    /*************************************************
      step 11: create a thread to change region's 
      layer, position, alpha and other display attribute
     *************************************************/
    pthread_create(&g_stVencRgnThread, NULL, SAMPLE_RGN_VencOSdDynamicDisplay, NULL);
    stRgnAttrInfo.Handle    = VpssOsdHandle;
    stRgnAttrInfo.u32RgnNum = u32VpssRgnNum; 
    pthread_create(&g_stVpssRgnThread, NULL, SAMPLE_RGN_VpssOSdDynamicDisplay, (HI_VOID*)&stRgnAttrInfo);
    sleep(3);
    /*************************************************
      step 12: start a thread to handle vpss osd color
      reverse
     *************************************************/
    u32VpssOsdRectCnt                            = 3;
    stVpssOsdReverseInfo.Handle                  = VpssOsdHandle;
    stVpssOsdReverseInfo.VpssGrp                 = 0;
    stVpssOsdReverseInfo.VpssChn                 = VPSS_CHN0;
    stVpssOsdReverseInfo.u8PerPixelLumaThrd      = 128;
    stVpssOsdReverseInfo.stLumaRgnInfo.u32RegionNum   = u32VpssOsdRectCnt;
    stVpssOsdReverseInfo.stLumaRgnInfo.pstRegion = astVpssOsdLumaRect;

    s32Ret = HI_MPI_RGN_GetAttr(VpssOsdHandle, &stRgnAttrSet);
    if(HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_RGN_GetAttr failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }
    stSize.u32Width  = stRgnAttrSet.unAttr.stOverlayEx.stSize.u32Width;
    stSize.u32Height = stRgnAttrSet.unAttr.stOverlayEx.stSize.u32Height;

    stChn.enModId  = HI_ID_VPSS;
    stChn.s32DevId = 0;
    stChn.s32ChnId = VPSS_CHN0;
    s32Ret = HI_MPI_RGN_GetDisplayAttr(VpssOsdHandle, &stChn, &stChnAttr);
    if(HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_RGN_GetDisplayAttr failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }
    
    for (i=0; i < u32VpssOsdRectCnt; i++)
    {
        astVpssOsdLumaRect[i].s32X = ((stSize.u32Width/ u32VpssOsdRectCnt) * i) + stChnAttr.unChnAttr.stOverlayExChn.stPoint.s32X;
        astVpssOsdLumaRect[i].s32Y = stChnAttr.unChnAttr.stOverlayExChn.stPoint.s32Y;
        astVpssOsdLumaRect[i].u32Width  = (stSize.u32Width / u32VpssOsdRectCnt);
        astVpssOsdLumaRect[i].u32Height = stSize.u32Height;
    }
    pthread_create(&g_stVpssOsdReverseThread, NULL, SAMPLE_RGN_VpssOsdReverse, (HI_VOID*)&stVpssOsdReverseInfo);
    
    /*************************************************
      step 13: start a thread to handle venc osd color
      reverse
     *************************************************/
    u32VencOsdRectCnt                            = 3;
    stVencOsdReverseInfo.Handle                  = VencOsdHandle;
    stVencOsdReverseInfo.VpssGrp                 = 0;
    stVencOsdReverseInfo.VpssChn                 = u32VpssChn;
    stVencOsdReverseInfo.u8PerPixelLumaThrd      = 128;
    stVencOsdReverseInfo.stLumaRgnInfo.u32RegionNum   = u32VencOsdRectCnt;
    stVencOsdReverseInfo.stLumaRgnInfo.pstRegion = astVencOsdLumaRect;

    s32Ret = HI_MPI_RGN_GetAttr(VencOsdHandle, &stRgnAttrSet);
    if(HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_RGN_GetAttr failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }
    stSize.u32Width  = stRgnAttrSet.unAttr.stOverlay.stSize.u32Width;
    stSize.u32Height = stRgnAttrSet.unAttr.stOverlay.stSize.u32Height;

    stChn.enModId  = HI_ID_VENC;
    stChn.s32DevId = 0;
    stChn.s32ChnId = 0;
    s32Ret = HI_MPI_RGN_GetDisplayAttr(VencOsdHandle, &stChn, &stChnAttr);
    if(HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_RGN_GetDisplayAttr failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }
    
    for (i=0; i < u32VencOsdRectCnt; i++)
    {
        astVencOsdLumaRect[i].s32X = ((stSize.u32Width/ u32VencOsdRectCnt) * i) + stChnAttr.unChnAttr.stOverlayChn.stPoint.s32X;
        astVencOsdLumaRect[i].s32Y = stChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y;
        astVencOsdLumaRect[i].u32Width  = (stSize.u32Width / u32VencOsdRectCnt);
        astVencOsdLumaRect[i].u32Height = stSize.u32Height;
    }
    pthread_create(&g_stVencOsdReverseThread, NULL, SAMPLE_RGN_VencOsdReverse, (HI_VOID*)&stVencOsdReverseInfo);
    
    /*************************************************
      step 14: create a thread for venc to save stream
      to a file
     *************************************************/
    char pfilename[30]; 
    snprintf(pfilename, sizeof(pfilename), "venc_encode_stream.h264");
    pastream = fopen(pfilename, "wb");  
    HI_ASSERT( NULL != pastream);

    stVencGetPram.pstream   = pastream;
    stVencGetPram.VeChnId   = VencChn;
    stVencGetPram.s32FrmCnt = 0;
    if (0 != pthread_create(&g_stVencThread, 0, SAMPLE_RGN_VencGetStream, (HI_VOID *)&stVencGetPram))
	{
		printf("create thread failed!\n");
	}

    printf("\n#############Sample start ok! Press Enter to switch!#############\n");

    
    /*************************************************
      step 15: stop thread and release all the resource
     *************************************************/
    getchar();
    bExit = HI_TRUE;
    if (g_stVencThread)
    {
        pthread_join(g_stVencThread, 0);
        g_stVencThread = 0;
    }
    if (g_stVpssRgnThread)
    {
        pthread_join(g_stVpssRgnThread, 0);
        g_stVpssRgnThread = 0;
    }
    if (g_stVencRgnThread)
    {
        pthread_join(g_stVencRgnThread, 0);
        g_stVencRgnThread = 0;
    }
    if (g_stVpssOsdReverseThread)
    {
        pthread_join(g_stVpssOsdReverseThread, 0);
        g_stVpssOsdReverseThread = 0;
    }
    if (g_stVencOsdReverseThread)
    {
        pthread_join(g_stVencOsdReverseThread, 0);
        g_stVencOsdReverseThread = 0;
    }
    
    bExit = HI_FALSE;

    /*************************************************
      step 16: unbind vpss and venc
     *************************************************/
    s32Ret = HI_MPI_SYS_UnBind(&stSrcChn, &stDesChn);
    if(HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_SYS_UnBind failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }

    /*************************************************
      step 17: unbind vpss and vo
     *************************************************/

    stSrcChn.enModId  = HI_ID_VPSS;
    stSrcChn.s32DevId = 0;
    stSrcChn.s32ChnId = VPSS_CHN2;

    stDesChn.enModId  = HI_ID_VOU;
    stDesChn.s32DevId = VoLayer;
    stDesChn.s32ChnId = 0;

    s32Ret = HI_MPI_SYS_UnBind(&stSrcChn, &stDesChn);
    if(HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_SYS_UnBind failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }

    /*************************************************
      step 18: unbind vi and vpss
     *************************************************/
    stSrcChn.enModId  = HI_ID_VIU;
    stSrcChn.s32DevId = 0;
    stSrcChn.s32ChnId = 0;

    stDesChn.enModId  = HI_ID_VPSS;
    stDesChn.s32DevId = 0;
    stDesChn.s32ChnId = 0;

    s32Ret = HI_MPI_SYS_UnBind(&stSrcChn, &stDesChn);
    if(HI_SUCCESS != s32Ret)
    {
        printf("HI_MPI_SYS_Bind failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }

    /*************************************************
      step 19: stop venc chn
     *************************************************/
    s32Ret = SAMPLE_RGN_StopVenc(VencChn);
    if(HI_SUCCESS != s32Ret)
    {
        printf("SAMPLE_RGN_StopVenc failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }

    /*************************************************
      step 20: stop vo dev and chn
     *************************************************/
    s32Ret = SAMPLE_RGN_StopVoChn(VoDev, s32VoChnNum);
    if(HI_SUCCESS != s32Ret)
    {
        printf("SAMPLE_RGN_StopVoChn failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }
    
    s32Ret = SAMPLE_RGN_StopVoDev(VoDev);
    if(HI_SUCCESS != s32Ret)
    {
        printf("SAMPLE_RGN_StopVoDev failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }

    /*************************************************
      step 21: stop vpss group and chn
     *************************************************/
    s32Ret = SAMPLE_RGN_StopVpss(s32VpssGrpNum);
    if(HI_SUCCESS != s32Ret)
    {
        printf("SAMPLE_RGN_StopVpss failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }
    
    /*************************************************
      step 22: stop vi chn
     *************************************************/
    s32Ret = SAMPLE_COMM_VI_StopVi(&g_stViChnConfig);
    if(HI_SUCCESS != s32Ret)
    {
        printf("SAMPLE_COMM_VI_StopVi failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }
    
    /*************************************************
      step 23: destroy region
     *************************************************/
    s32Ret = SAMPLE_RGN_DestroyRegion(VencOsdHandle, u32VencRgnNum + u32VpssRgnNum);       
    if(HI_SUCCESS != s32Ret)
    {
        printf("SAMPLE_RGN_DestroyRegion failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }
    
    s32Ret = SAMPLE_RGN_SYS_Exit();
	if(HI_SUCCESS != s32Ret)
    {
        printf("SAMPLE_RGN_SYS_Exit failed! s32Ret: 0x%x.\n", s32Ret);
        return s32Ret;
    }
    
	return s32Ret;
	
}

/******************************************************************************
* function    : main()
* Description : region
******************************************************************************/
int main(int argc, char *argv[])
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_CHAR ch;
    
    bExit = HI_FALSE;

    if ( (argc < 2) || (1 != strlen(argv[1])))
    {
        SAMPLE_RGN_Usage1(argv[0]);
        return HI_FAILURE;
    }

    signal(SIGINT, SAMPLE_RGN_HandleSig);
    signal(SIGTERM, SAMPLE_RGN_HandleSig);
    if((argc > 1) && *argv[1] == '1')   /* '0': VO_INTF_CVBS, else: BT1120 */
    {
        g_enVoIntfType = VO_INTF_BT656;
    }

    g_stViChnConfig.enViMode = SENSOR_TYPE;
    SAMPLE_COMM_VI_GetSizeBySensor(&g_enPicSize);
    
    while (1)
    {
        SAMPLE_RGN_Usage2();
        ch = (char)getchar();

		while (getchar() != '\n')
		{
			;			
		}
		
        switch (ch)
        {
            case '0': /* VI->VPSS(COVER)->VO PAL */
            {
                s32Ret = SAMPLE_RGN_AddCoverToVpss();
                break;
            }
            case '1': /* VI->VPSS(COVEREX)->VO PAL */
            {
                s32Ret = SAMPLE_RGN_AddCoverExToVpss();
                break;
            }
            case '2': /* VI->VPSS(OVERLAYEX)->VO PAL */
            {
                s32Ret = SAMPLE_RGN_AddOsdToVpss();
                break;
            }
            case '3': /* VI->VPSS(OVERLAYEX)->VO/VENC(OVERLAY)->file */
            {
				#if CHIP_ID == CHIP_HI3518E_V201
				printf("Hi3518EV201 is not support this sample!\n");
				break;
				#endif
	            s32Ret = SAMPLE_RGN_AddOsdToVencAndVpss();
			    break;
            }
            case 'q':
            case 'Q':
            {
                bExit = HI_TRUE;
                break;
            }
            default :
            {
                printf("input invaild! please try again.\n");
                break;
            }
        }
        
        if (bExit)
        {
            break;
        }
    }
            
    return s32Ret;
    
}



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
