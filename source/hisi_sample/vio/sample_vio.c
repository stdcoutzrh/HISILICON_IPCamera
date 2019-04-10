/******************************************************************************
  A simple program of Hisilicon HI3531 video input and output implementation.
  Copyright (C), 2010-2011, Hisilicon Tech. Co., Ltd.
 ******************************************************************************
    Modification:  2011-8 Created
******************************************************************************/

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "sample_comm.h"


VIDEO_NORM_E gs_enNorm = VIDEO_ENCODING_MODE_PAL;
VO_INTF_TYPE_E g_enVoIntfType = VO_INTF_LCD;
PIC_SIZE_E g_enPicSize = PIC_HD1080;

SAMPLE_VIDEO_LOSS_S gs_stVideoLoss;
HI_U32 gs_u32ViFrmRate = 0;
extern VB_POOL g_Pool;


SAMPLE_VI_CONFIG_S g_stViChnConfig =
{
    APTINA_AR0130_DC_720P_30FPS,
    VIDEO_ENCODING_MODE_AUTO,

    ROTATE_NONE,
    VI_CHN_SET_NORMAL,
    WDR_MODE_NONE
};

/******************************************************************************
* function : show usage
******************************************************************************/
void SAMPLE_VIO_Usage(char* sPrgNm)
{
    printf("Usage : %s <index> <intf>\n", sPrgNm);
    printf("index:\n");
    printf("\t 0)online mode VI/VPSS; VO(0:LCD, 1:BT656). Embeded isp, phychn channel preview.\n");
    printf("\t 1)online mode VI(FPN)/VPSS; VO(0:LCD), 1:BT656).frame mode FPN,Embeded isp,phychn channel preview.\n");
    printf("\t 2)online mode VI/VPSS(ROTATE); VO(0:LCD, 1:BT656).Embeded isp, online, rotate, phychn channel preview.\n");
    printf("\t 3)online mode VI/VPSS; VO(0:LCD, 1:BT656). Embeded isp, set rgbir phychn channel preview.\n");
    printf("\t 4)online mode VI/VPSS(LDC); VO(0:LCD, 1:BT656).Embeded isp, online, LDC, phychn channel preview.\n");
    printf("\t 5)offline mode VI(ROTATE); VO(0:LCD, 1:BT656).Embeded isp, offline, rotate, phychn channel preview.\n");
    printf("\t 6)offline mode VI(LDC); VO(0:LCD, 1:BT656).Embeded isp, offline, LDC, phychn channel preview.\n");
    printf("\t 7)offline mode VI(DIS); VO(0:LCD, 1:BT656).Embeded isp, offline, DIS, phychn channel preview.\n");
    printf("\t 8)offline mode VI: BT1120; VO(0:LCD, 1:BT656).Isp bypass, phychn channel preview.\n");

#if (CHIP_ID != CHIP_HI3518E_V201)
    printf("\t 9)offline mode VI; VO(0:LCD, 1:BT656).Embeded isp, resolution switch.\n");
#endif

    printf("intf:\n");
    printf("\t 0) vo LCD output, default\n");
    printf("\t 1) vo BT656 output.\n");
    return;
}

/******************************************************************************
* function : to process abnormal case
******************************************************************************/
void SAMPLE_VIO_HandleSig(HI_S32 signo)
{
    if (SIGINT == signo || SIGTERM == signo)
    {
        SAMPLE_COMM_ISP_Stop();
        SAMPLE_COMM_SYS_Exit();
        printf("\033[0;31mprogram termination abnormally!\033[0;39m\n");
    }
    exit(-1);
}


/******************************************************************************
* function : vi/vpss: online mode. Embeded isp, phychn channel preview.
******************************************************************************/
HI_S32 SAMPLE_VIO_PreView(SAMPLE_VI_CONFIG_S* pstViConfig)
{

    VB_CONF_S stVbConf;
    VO_DEV VoDev = SAMPLE_VO_DEV_DSD0;;
    VO_CHN VoChn = 0;
    VO_PUB_ATTR_S stVoPubAttr;
    SAMPLE_VO_MODE_E enVoMode = VO_MODE_1MUX;
    PIC_SIZE_E enPicSize = g_enPicSize;
    VO_LAYER VoLayer = 0;
    VO_VIDEO_LAYER_ATTR_S stLayerAttr;
    VPSS_GRP VpssGrp = 0;
    VPSS_CHN VpssChn = 0;
    VPSS_GRP_ATTR_S stVpssGrpAttr;
    VPSS_CHN_ATTR_S stVpssChnAttr;
    VPSS_CHN_MODE_S stVpssChnMode;
    VPSS_EXT_CHN_ATTR_S stVpssExtChnAttr;

    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32BlkSize;
    SIZE_S stSize;

    VO_CSC_S stCSC = {0};

    /******************************************
     step  1: init global  variable
    ******************************************/
    gs_u32ViFrmRate = (VIDEO_ENCODING_MODE_PAL == gs_enNorm) ? 25 : 30;
    memset(&stVbConf, 0, sizeof(VB_CONF_S));

    u32BlkSize = SAMPLE_COMM_SYS_CalcPicVbBlkSize(gs_enNorm, enPicSize,
                 SAMPLE_PIXEL_FORMAT, SAMPLE_SYS_ALIGN_WIDTH);
    stVbConf.u32MaxPoolCnt = 128;

    /*ddr0 video buffer*/
    stVbConf.astCommPool[0].u32BlkSize = u32BlkSize;
    stVbConf.astCommPool[0].u32BlkCnt  = VB_CNT;

    /******************************************
     step 2: start vpss and vi bind vpss (subchn needn't bind vpss in this mode)
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_GetPicSize(gs_enNorm, enPicSize, &stSize);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_SYS_GetPicSize failed!\n");
        goto END_1080P_0;
    }

    /******************************************
     step 3: mpp system init.
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("system init failed with %d!\n", s32Ret);
        goto END_1080P_0;
    }

    /******************************************
     step 4: start vi dev & chn to capture
    ******************************************/
    s32Ret = SAMPLE_COMM_VI_StartVi(pstViConfig);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start vi failed!\n");
        goto END_1080P_0;
    }

    stVpssGrpAttr.bDciEn    = HI_FALSE;
    stVpssGrpAttr.bHistEn   = HI_FALSE;
    stVpssGrpAttr.bIeEn     = HI_FALSE;
    stVpssGrpAttr.bNrEn     = HI_TRUE;
    stVpssGrpAttr.enDieMode = VPSS_DIE_MODE_NODIE;
    stVpssGrpAttr.enPixFmt  = PIXEL_FORMAT_YUV_SEMIPLANAR_420;

    stVpssGrpAttr.u32MaxW = stSize.u32Width;
    stVpssGrpAttr.u32MaxH = stSize.u32Height;


    /******************************************
     step 5: start vpss group
    ******************************************/
    s32Ret = SAMPLE_COMM_VPSS_StartGroup(VpssGrp, &stVpssGrpAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start VPSS GROUP failed!\n");
        goto END_1080P_1;
    }

    stVpssChnAttr.bBorderEn       = 0;
    stVpssChnAttr.bFlip           = 0;
    stVpssChnAttr.bMirror         = 0;
    stVpssChnAttr.bSpEn           = 0 ;
    stVpssChnAttr.s32DstFrameRate = -1;
    stVpssChnAttr.s32SrcFrameRate = -1;
    stVpssChnMode.bDouble         = HI_FALSE;
    stVpssChnMode.enChnMode       = VPSS_CHN_MODE_USER;
    stVpssChnMode.enCompressMode  = COMPRESS_MODE_NONE;
    stVpssChnMode.enPixelFormat   = PIXEL_FORMAT_YUV_SEMIPLANAR_420;
    stVpssChnMode.u32Width        = stSize.u32Width;
    stVpssChnMode.u32Height       = stSize.u32Height;

    /******************************************
    step 6: start vpss chn
    ******************************************/
    s32Ret = SAMPLE_COMM_VPSS_EnableChn(VpssGrp, VpssChn, &stVpssChnAttr, &stVpssChnMode, &stVpssExtChnAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start VPSS CHN failed!\n");
        goto END_1080P_1;
    }

    /******************************************
    step 7: start VO SD0 (bind * vi )
    ******************************************/
    stVoPubAttr.enIntfType = g_enVoIntfType;

    if (VO_INTF_BT656 == g_enVoIntfType)
    {
        stVoPubAttr.enIntfSync = VO_OUTPUT_PAL;
        gs_u32ViFrmRate = 50;
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
    stVoPubAttr.u32BgColor = 0x000000ff;

    /* In HD, this item should be set to HI_FALSE */
    s32Ret = SAMPLE_COMM_VO_StartDev(VoDev, &stVoPubAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartDev failed!\n");
        goto END_1080P_2;
    }

    stLayerAttr.bClusterMode = HI_FALSE;
    stLayerAttr.bDoubleFrame = HI_FALSE;
    stLayerAttr.enPixFormat = PIXEL_FORMAT_YUV_SEMIPLANAR_420;

    stLayerAttr.stDispRect.s32X = 0;
    stLayerAttr.stDispRect.s32Y = 0;

    s32Ret = SAMPLE_COMM_VO_GetWH(stVoPubAttr.enIntfSync,
                                  &stLayerAttr.stDispRect.u32Width, &stLayerAttr.stDispRect.u32Height,
                                  &stLayerAttr.u32DispFrmRt);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_GetWH failed!\n");
        goto END_1080P_2;
    }

    stLayerAttr.stImageSize.u32Width  = stLayerAttr.stDispRect.u32Width;
    stLayerAttr.stImageSize.u32Height = stLayerAttr.stDispRect.u32Height;

    s32Ret = SAMPLE_COMM_VO_StartLayer(VoLayer, &stLayerAttr, HI_FALSE);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartChn failed!\n");
        goto END_1080P_3;
    }

    s32Ret = SAMPLE_COMM_VO_StartChn(VoDev, VO_MODE_1MUX);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartChn failed!\n");
        goto END_1080P_4;
    }


    s32Ret = SAMPLE_COMM_VO_BindVpss(VoDev, VoChn, VpssGrp, VpssChn);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_BindVpss(vo:%d)-(VpssChn:%d) failed with %#x!\n", VoDev, VoChn, s32Ret);
        goto END_1080P_5;
    }

    VI_PAUSE();
    /******************************************
     step 8: exit process
    ******************************************/
END_1080P_5:
    SAMPLE_COMM_VO_UnBindVpss(VoDev, VoChn, VpssGrp, VpssChn);
    SAMPLE_COMM_VO_StopChn(VoDev, enVoMode);
END_1080P_4:
    SAMPLE_COMM_VO_StopLayer(VoLayer);
END_1080P_3:
    SAMPLE_COMM_VO_StopDev(VoDev);
END_1080P_2:
    SAMPLE_COMM_VPSS_DisableChn(VpssGrp, VpssChn);
    SAMPLE_COMM_VPSS_StopGroup(VpssGrp);
END_1080P_1:
    SAMPLE_COMM_VI_StopVi(pstViConfig);
END_1080P_0:
    SAMPLE_COMM_SYS_Exit();
    return s32Ret;
}

/******************************************************************************
* function : vi/vpss: online mode. Embeded isp, set rgbir, phychn channel preview.
******************************************************************************/
HI_S32 SAMPLE_VIO_RGBIR_PreView(SAMPLE_VI_CONFIG_S* pstViConfig)
{

    VB_CONF_S stVbConf;
    VO_DEV VoDev = SAMPLE_VO_DEV_DSD0;;
    VO_CHN VoChn = 0;
    VO_PUB_ATTR_S stVoPubAttr;
    SAMPLE_VO_MODE_E enVoMode = VO_MODE_1MUX;
    PIC_SIZE_E enPicSize = g_enPicSize;
    VO_LAYER VoLayer = 0;
    VO_VIDEO_LAYER_ATTR_S stLayerAttr;
    VPSS_GRP VpssGrp = 0;
    VPSS_CHN VpssChn = 0;
    VPSS_GRP_ATTR_S stVpssGrpAttr;
    VPSS_CHN_ATTR_S stVpssChnAttr;
    VPSS_CHN_MODE_S stVpssChnMode;
    VPSS_EXT_CHN_ATTR_S stVpssExtChnAttr;
    ISP_DEV IspDev = 0;
    ISP_RGBIR_ATTR_S stIspRgbirAttr;
    ISP_RGBIR_CTRL_S stIspRgbirCtrl;

    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32BlkSize;
    SIZE_S stSize;

    VO_CSC_S stCSC = {0};

    /******************************************
     step  1: init global  variable
    ******************************************/
    gs_u32ViFrmRate = (VIDEO_ENCODING_MODE_PAL == gs_enNorm) ? 25 : 30;
    memset(&stVbConf, 0, sizeof(VB_CONF_S));

    u32BlkSize = SAMPLE_COMM_SYS_CalcPicVbBlkSize(gs_enNorm, enPicSize,
                 SAMPLE_PIXEL_FORMAT, SAMPLE_SYS_ALIGN_WIDTH);
    stVbConf.u32MaxPoolCnt = 128;

    /*ddr0 video buffer*/
    stVbConf.astCommPool[0].u32BlkSize = u32BlkSize;
    stVbConf.astCommPool[0].u32BlkCnt  = VB_CNT;

    /******************************************
     step 2: start vpss and vi bind vpss (subchn needn't bind vpss in this mode)
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_GetPicSize(gs_enNorm, enPicSize, &stSize);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_SYS_GetPicSize failed!\n");
        goto END_1080P_0;
    }

    /******************************************
     step 3: mpp system init.
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("system init failed with %d!\n", s32Ret);
        goto END_1080P_0;
    }

    /******************************************
     step 4: start vi dev & chn to capture
    ******************************************/
    s32Ret = SAMPLE_COMM_VI_StartVi(pstViConfig);

    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start vi failed!\n");
        goto END_1080P_0;
    }
    
    /******************************************
     step 5: set rgbir
    ******************************************/
    SAMPLE_COMM_VI_GetRgbirAttrBySensor(&stIspRgbirAttr);
    s32Ret = HI_MPI_ISP_SetRgbirAttr(IspDev, &stIspRgbirAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("set isp rgbir attr failed!\n");
        goto END_1080P_1;
    }

    SAMPLE_COMM_VI_GetRgbirCtrlBySensor(&stIspRgbirCtrl);
    s32Ret = HI_MPI_ISP_SetRgbirCtrl(IspDev, &stIspRgbirCtrl); 
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("set isp rgbir ctrl failed!\n");
        goto END_1080P_1;
    }    

    stVpssGrpAttr.bDciEn    = HI_FALSE;
    stVpssGrpAttr.bHistEn   = HI_FALSE;
    stVpssGrpAttr.bIeEn     = HI_FALSE;
    stVpssGrpAttr.bNrEn     = HI_TRUE;
    stVpssGrpAttr.enDieMode = VPSS_DIE_MODE_NODIE;
    stVpssGrpAttr.enPixFmt  = PIXEL_FORMAT_YUV_SEMIPLANAR_420;

    stVpssGrpAttr.u32MaxW = stSize.u32Width;
    stVpssGrpAttr.u32MaxH = stSize.u32Height;


    /******************************************
     step 6: start vpss group
    ******************************************/
    s32Ret = SAMPLE_COMM_VPSS_StartGroup(VpssGrp, &stVpssGrpAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start VPSS GROUP failed!\n");
        goto END_1080P_1;
    }

    stVpssChnAttr.bBorderEn       = 0;
    stVpssChnAttr.bFlip           = 0;
    stVpssChnAttr.bMirror         = 0;
    stVpssChnAttr.bSpEn           = 0 ;
    stVpssChnAttr.s32DstFrameRate = -1;
    stVpssChnAttr.s32SrcFrameRate = -1;
    stVpssChnMode.bDouble         = HI_FALSE;
    stVpssChnMode.enChnMode       = VPSS_CHN_MODE_USER;
    stVpssChnMode.enCompressMode  = COMPRESS_MODE_NONE;
    stVpssChnMode.enPixelFormat   = PIXEL_FORMAT_YUV_SEMIPLANAR_420;
    stVpssChnMode.u32Width        = stSize.u32Width;
    stVpssChnMode.u32Height       = stSize.u32Height;

    /******************************************
    step 7: start vpss chn
    ******************************************/
    s32Ret = SAMPLE_COMM_VPSS_EnableChn(VpssGrp, VpssChn, &stVpssChnAttr, &stVpssChnMode, &stVpssExtChnAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start VPSS CHN failed!\n");
        goto END_1080P_1;
    }

    /******************************************
    step 8: start VO SD0 (bind * vi )
    ******************************************/
    stVoPubAttr.enIntfType = g_enVoIntfType;

    if (VO_INTF_BT656 == g_enVoIntfType)
    {
        stVoPubAttr.enIntfSync = VO_OUTPUT_PAL;
        gs_u32ViFrmRate = 50;
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
    stVoPubAttr.u32BgColor = 0x000000ff;

    /* In HD, this item should be set to HI_FALSE */
    s32Ret = SAMPLE_COMM_VO_StartDev(VoDev, &stVoPubAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartDev failed!\n");
        goto END_1080P_2;
    }

    stLayerAttr.bClusterMode = HI_FALSE;
    stLayerAttr.bDoubleFrame = HI_FALSE;
    stLayerAttr.enPixFormat = PIXEL_FORMAT_YUV_SEMIPLANAR_420;

    stLayerAttr.stDispRect.s32X = 0;
    stLayerAttr.stDispRect.s32Y = 0;

    s32Ret = SAMPLE_COMM_VO_GetWH(stVoPubAttr.enIntfSync,
                                  &stLayerAttr.stDispRect.u32Width, &stLayerAttr.stDispRect.u32Height,
                                  &stLayerAttr.u32DispFrmRt);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_GetWH failed!\n");
        goto END_1080P_2;
    }

    stLayerAttr.stImageSize.u32Width  = stLayerAttr.stDispRect.u32Width;
    stLayerAttr.stImageSize.u32Height = stLayerAttr.stDispRect.u32Height;

    s32Ret = SAMPLE_COMM_VO_StartLayer(VoLayer, &stLayerAttr, HI_FALSE);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartChn failed!\n");
        goto END_1080P_3;
    }

    s32Ret = SAMPLE_COMM_VO_StartChn(VoDev, VO_MODE_1MUX);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartChn failed!\n");
        goto END_1080P_4;
    }


    s32Ret = SAMPLE_COMM_VO_BindVpss(VoDev, VoChn, VpssGrp, VpssChn);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_BindVpss(vo:%d)-(VpssChn:%d) failed with %#x!\n", VoDev, VoChn, s32Ret);
        goto END_1080P_5;
    }

    VI_PAUSE();
    /******************************************
     step 9: exit process
    ******************************************/
END_1080P_5:
    SAMPLE_COMM_VO_UnBindVpss(VoDev, VoChn, VpssGrp, VpssChn);
    SAMPLE_COMM_VO_StopChn(VoDev, enVoMode);
END_1080P_4:
    SAMPLE_COMM_VO_StopLayer(VoLayer);
END_1080P_3:
    SAMPLE_COMM_VO_StopDev(VoDev);
END_1080P_2:
    SAMPLE_COMM_VPSS_DisableChn(VpssGrp, VpssChn);
    SAMPLE_COMM_VPSS_StopGroup(VpssGrp);
END_1080P_1:
    SAMPLE_COMM_VI_StopVi(pstViConfig);
END_1080P_0:
    SAMPLE_COMM_SYS_Exit();
    return s32Ret;
}


#if 0
/******************************************************************************
* function : vi/vpss: online mode VI(WDR): DC(1080P); VO: SD0(CVBS). Embeded isp, phychn channel preview.
******************************************************************************/
HI_S32 SAMPLE_VIO_LINE_WDR_PreView(SAMPLE_VI_CONFIG_S* pstViConfig)
{
    HI_U32 u32ViChnCnt = 2;
    VB_CONF_S stVbConf;
    VO_DEV VoDev = SAMPLE_VO_DEV_DSD0;;
    VO_CHN VoChn = 0;
    VO_PUB_ATTR_S stVoPubAttr;
    SAMPLE_VO_MODE_E enVoMode = VO_MODE_1MUX;
    PIC_SIZE_E enPicSize = g_enPicSize;
    VO_LAYER VoLayer = 0;
    VO_VIDEO_LAYER_ATTR_S stLayerAttr;
    VPSS_GRP VpssGrp = 0;
    VPSS_CHN VpssChn = 0;
    VPSS_GRP_ATTR_S stVpssGrpAttr;
    VPSS_CHN_ATTR_S stVpssChnAttr;
    VPSS_CHN_MODE_S stVpssChnMode;
    VPSS_EXT_CHN_ATTR_S stVpssExtChnAttr;
    HI_U32 u32Mode = 0;
    //ISP_WDR_MODE_S  stWdrMode;

    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32BlkSize;
    SIZE_S stSize;
    ROTATE_E enRotate = ROTATE_NONE;

    /******************************************
     step  1: init global  variable
    ******************************************/
    gs_u32ViFrmRate = (VIDEO_ENCODING_MODE_PAL == gs_enNorm) ? 25 : 30;
    memset(&stVbConf, 0, sizeof(VB_CONF_S));

    u32BlkSize = SAMPLE_COMM_SYS_CalcPicVbBlkSize(gs_enNorm, enPicSize,
                 SAMPLE_PIXEL_FORMAT, SAMPLE_SYS_ALIGN_WIDTH);
    stVbConf.u32MaxPoolCnt = 128;

    /*ddr0 video buffer*/
    stVbConf.astCommPool[0].u32BlkSize = u32BlkSize;
    stVbConf.astCommPool[0].u32BlkCnt = u32ViChnCnt * 8;

    /******************************************
     step 2: start vpss and vi bind vpss (subchn needn't bind vpss in this mode)
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_GetPicSize(gs_enNorm, enPicSize, &stSize);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_SYS_GetPicSize failed!\n");
        goto END_1080P_0;
    }

    if (pstViConfig)
    {
        enRotate = pstViConfig->enRotate;
        if (ROTATE_90 == enRotate || ROTATE_270 == enRotate)
        {
            u32BlkSize = (CEILING_2_POWER(stSize.u32Width, SAMPLE_SYS_ALIGN_WIDTH) * \
                          CEILING_2_POWER(stSize.u32Height, SAMPLE_SYS_ALIGN_WIDTH) * \
                          ((PIXEL_FORMAT_YUV_SEMIPLANAR_422 == SAMPLE_PIXEL_FORMAT) ? 2 : 1.5));
            stVbConf.astCommPool[1].u32BlkSize = u32BlkSize;
            stVbConf.astCommPool[1].u32BlkCnt = 8;
        }
    }

    /******************************************
     step 3: mpp system init.
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("system init failed with %d!\n", s32Ret);
        goto END_1080P_0;
    }

    /******************************************
     step 4: isp wdr mode config;
    ******************************************/
#if 0
    stWdrMode.enWDRMode  = WDR_MODE_2To1_LINE;
    s32Ret = HI_MPI_ISP_SetWDRMode(0, &stWdrMode);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start WDR failed!\n");
        goto END_1080P_0;
    }
#endif

    /******************************************
     step 5: start vi dev & chn to capture
             vi wdr attr should be set;
    ******************************************/
    s32Ret = SAMPLE_COMM_VI_StartVi(pstViConfig);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start vi failed!\n");
        goto END_1080P_0;
    }

    stVpssGrpAttr.bDciEn    = HI_FALSE;
    stVpssGrpAttr.bHistEn   = HI_FALSE;
    stVpssGrpAttr.bIeEn     = HI_FALSE;
    stVpssGrpAttr.bNrEn     = HI_TRUE;
    stVpssGrpAttr.enDieMode = VPSS_DIE_MODE_NODIE;
    stVpssGrpAttr.enPixFmt  = PIXEL_FORMAT_YUV_SEMIPLANAR_420;
    stVpssGrpAttr.u32MaxW   = stSize.u32Width;
    stVpssGrpAttr.u32MaxH   = stSize.u32Height;

    /******************************************
     step 5: start vpss group
    ******************************************/
    s32Ret = SAMPLE_COMM_VPSS_StartGroup(VpssGrp, &stVpssGrpAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start VPSS GROUP failed!\n");
        goto END_1080P_1;
    }

    stVpssChnAttr.bBorderEn       = 0;
    stVpssChnAttr.bFlip           = 0;
    stVpssChnAttr.bMirror         = 0;
    stVpssChnAttr.bSpEn           = 0 ;
    stVpssChnAttr.s32DstFrameRate = -1;
    stVpssChnAttr.s32SrcFrameRate = -1;
    stVpssChnMode.bDouble         = HI_FALSE;
    stVpssChnMode.enChnMode       = VPSS_CHN_MODE_USER;
    stVpssChnMode.enCompressMode  = COMPRESS_MODE_SEG;
    stVpssChnMode.enPixelFormat   = PIXEL_FORMAT_YUV_SEMIPLANAR_420;
    stVpssChnMode.u32Width        = stSize.u32Width;
    stVpssChnMode.u32Height       = stSize.u32Height;

    /******************************************
    step 6: start vpss chn
    ******************************************/
    s32Ret = SAMPLE_COMM_VPSS_EnableChn(VpssGrp, VpssChn, &stVpssChnAttr, &stVpssChnMode, &stVpssExtChnAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start VPSS CHN failed!\n");
        goto END_1080P_1;
    }

    /******************************************
    step 7: start VO SD0 (bind * vi )
    ******************************************/
    stVoPubAttr.enIntfType = g_enVoIntfType;
    if (VO_INTF_BT1120 == g_enVoIntfType)
    {
        stVoPubAttr.enIntfSync = VO_OUTPUT_1080P30;
        gs_u32ViFrmRate = 50;
    }
    else
    {
        stVoPubAttr.enIntfSync = VO_OUTPUT_PAL;
    }
    stVoPubAttr.u32BgColor = 0x000000ff;

    /* In HD, this item should be set to HI_FALSE */
    s32Ret = SAMPLE_COMM_VO_StartDev(VoDev, &stVoPubAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartDev failed!\n");
        goto END_1080P_2;
    }

    stLayerAttr.bClusterMode = HI_FALSE;
    stLayerAttr.bDoubleFrame = HI_FALSE;
    stLayerAttr.enPixFormat  = PIXEL_FORMAT_YUV_SEMIPLANAR_420;
    stLayerAttr.stDispRect.s32X = 0;
    stLayerAttr.stDispRect.s32Y = 0;

    s32Ret = SAMPLE_COMM_VO_GetWH(stVoPubAttr.enIntfSync,
                                  &stLayerAttr.stDispRect.u32Width, &stLayerAttr.stDispRect.u32Height,
                                  &stLayerAttr.u32DispFrmRt);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_GetWH failed!\n");
        goto END_1080P_2;
    }
    stLayerAttr.stImageSize.u32Width  = stLayerAttr.stDispRect.u32Width;
    stLayerAttr.stImageSize.u32Height = stLayerAttr.stDispRect.u32Height;

    s32Ret = SAMPLE_COMM_VO_StartLayer(VoLayer, &stLayerAttr, HI_TRUE);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartChn failed!\n");
        goto END_1080P_3;
    }

    s32Ret = SAMPLE_COMM_VO_StartChn(VoDev, VO_MODE_1MUX);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartChn failed!\n");
        goto END_1080P_4;
    }

    HI_MPI_SYS_GetViVpssMode(&u32Mode);
    if (!u32Mode)  //off-line
    {
        s32Ret = SAMPLE_COMM_VI_BindVpss(pstViConfig->enViMode);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("SAMPLE_COMM_VI_BindVpss failed with %#x!\n", s32Ret);
            goto END_1080P_5;
        }
    }

    s32Ret = SAMPLE_COMM_VO_BindVpss(VoDev, VoChn, VpssGrp, VpssChn);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_BindVpss(vo:%d)-(VpssChn:%d) failed with %#x!\n", VoDev, VoChn, s32Ret);
        goto END_1080P_6;
    }

    VI_PAUSE();
    /******************************************
     step 8: exit process
    ******************************************/
END_1080P_6:
    SAMPLE_COMM_VO_UnBindVpss(VoDev, VoChn, VpssGrp, VpssChn);
END_1080P_5:
    if (!u32Mode)
    {
        SAMPLE_COMM_VI_UnBindVpss(pstViConfig->enViMode);
    }

    SAMPLE_COMM_VO_StopChn(VoDev, enVoMode);
END_1080P_4:
    SAMPLE_COMM_VO_StopLayer(VoLayer);
END_1080P_3:
    SAMPLE_COMM_VO_StopDev(VoDev);
END_1080P_2:
    SAMPLE_COMM_VPSS_DisableChn(VpssGrp, VpssChn);
    SAMPLE_COMM_VPSS_StopGroup(VpssGrp);
END_1080P_1:
    SAMPLE_COMM_VI_StopVi(pstViConfig);
END_1080P_0:
    SAMPLE_COMM_SYS_Exit();
    return s32Ret;
}
#endif

/******************************************************************************
* function : vi/vpss: online mode VI(FPN): DC; VO: SD0(BT656/LCD). Embeded isp, phychn channel preview.
******************************************************************************/
HI_S32 SAMPLE_VIO_FRAME_FPN_PreView(SAMPLE_VI_CONFIG_S* pstViConfig)
{
    VB_CONF_S stVbConf;
    VO_DEV VoDev = SAMPLE_VO_DEV_DSD0;
    VO_CHN VoChn = 0;
    VO_PUB_ATTR_S stVoPubAttr;
    SAMPLE_VO_MODE_E enVoMode = VO_MODE_1MUX;
    PIC_SIZE_E enPicSize = g_enPicSize;
    VO_LAYER VoLayer = 0;
    VO_VIDEO_LAYER_ATTR_S stLayerAttr;
    VPSS_GRP VpssGrp = 0;
    VPSS_CHN VpssChn = 0;
    VPSS_GRP_ATTR_S stVpssGrpAttr;
    VPSS_CHN_ATTR_S stVpssChnAttr;
    VPSS_CHN_MODE_S stVpssChnMode;
    VPSS_EXT_CHN_ATTR_S stVpssExtChnAttr;
    
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32BlkSize;
    SIZE_S stSize;

    VO_CSC_S stCSC = {0};

    /******************************************
     step  1: init global  variable
    ******************************************/
    gs_u32ViFrmRate = (VIDEO_ENCODING_MODE_PAL == gs_enNorm) ? 25 : 30;
    memset(&stVbConf, 0, sizeof(VB_CONF_S));

    u32BlkSize = SAMPLE_COMM_SYS_CalcPicVbBlkSize(gs_enNorm, enPicSize,
                 SAMPLE_PIXEL_FORMAT, SAMPLE_SYS_ALIGN_WIDTH);
    stVbConf.u32MaxPoolCnt = 128;

    /*ddr0 video buffer*/
    stVbConf.astCommPool[0].u32BlkSize = u32BlkSize;
    stVbConf.astCommPool[0].u32BlkCnt = VB_CNT - 1;

    /******************************************
     step 2: start vpss and vi bind vpss (subchn needn't bind vpss in this mode)
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_GetPicSize(gs_enNorm, enPicSize, &stSize);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_SYS_GetPicSize failed!\n");
        goto END_1080P_0;
    }

    /******************************************
     step 3: mpp system init.
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("system init failed with %d!\n", s32Ret);
        goto END_1080P_0;
    }

    /* FPN need one private 422 VB */
    u32BlkSize = SAMPLE_COMM_SYS_CalcPicVbBlkSize(gs_enNorm, enPicSize,
                 PIXEL_FORMAT_YUV_SEMIPLANAR_422, SAMPLE_SYS_ALIGN_WIDTH);
    g_Pool = HI_MPI_VB_CreatePool(u32BlkSize, 1, NULL);
    if ( VB_INVALID_POOLID == g_Pool )
    {
        printf("create vb err\n");
        goto END_1080P_0;
    }

    /******************************************
     step 4: start vi dev & chn to capture
    ******************************************/
    s32Ret = SAMPLE_COMM_VI_StartVi(pstViConfig);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start vi failed!\n");
        goto END_1080P_0;
    }


    stVpssGrpAttr.bDciEn    = HI_FALSE;
    stVpssGrpAttr.bHistEn   = HI_FALSE;
    stVpssGrpAttr.bIeEn     = HI_FALSE;
    stVpssGrpAttr.bNrEn     = HI_TRUE;
    stVpssGrpAttr.enDieMode = VPSS_DIE_MODE_NODIE;
    stVpssGrpAttr.enPixFmt  = PIXEL_FORMAT_YUV_SEMIPLANAR_420;

    stVpssGrpAttr.u32MaxW = stSize.u32Width;
    stVpssGrpAttr.u32MaxH = stSize.u32Height;

    /******************************************
     step 5: start vpss group
    ******************************************/
    s32Ret = SAMPLE_COMM_VPSS_StartGroup(VpssGrp, &stVpssGrpAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start VPSS GROUP failed!\n");
        goto END_1080P_1;
    }

    stVpssChnAttr.bBorderEn       = 0;
    stVpssChnAttr.bFlip           = 0;
    stVpssChnAttr.bMirror         = 0;
    stVpssChnAttr.bSpEn           = 0 ;
    stVpssChnAttr.s32DstFrameRate = -1;
    stVpssChnAttr.s32SrcFrameRate = -1;
    stVpssChnMode.bDouble         = HI_FALSE;
    stVpssChnMode.enChnMode       = VPSS_CHN_MODE_USER;
    stVpssChnMode.enCompressMode  = COMPRESS_MODE_SEG;
    stVpssChnMode.enPixelFormat   = PIXEL_FORMAT_YUV_SEMIPLANAR_420;
    stVpssChnMode.u32Width        = stSize.u32Width;
    stVpssChnMode.u32Height       = stSize.u32Height;

    /******************************************
     step 6: start vpss chn
    ******************************************/
    s32Ret = SAMPLE_COMM_VPSS_EnableChn(VpssGrp, VpssChn, &stVpssChnAttr, &stVpssChnMode, &stVpssExtChnAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start VPSS CHN failed!\n");
        goto END_1080P_1;
    }

    /******************************************
    step 7: start VO SD0 (bind * vi )
    ******************************************/
    stVoPubAttr.enIntfType = g_enVoIntfType;

    if (VO_INTF_BT656 == g_enVoIntfType)
    {
        stVoPubAttr.enIntfSync = VO_OUTPUT_PAL;
        gs_u32ViFrmRate = 50;
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

    stVoPubAttr.u32BgColor = 0x000000ff;

    /* In HD, this item should be set to HI_FALSE */
    s32Ret = SAMPLE_COMM_VO_StartDev(VoDev, &stVoPubAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartDev failed!\n");
        goto END_1080P_2;
    }

    stLayerAttr.bClusterMode = HI_FALSE;
    stLayerAttr.bDoubleFrame = HI_FALSE;
    stLayerAttr.enPixFormat  = PIXEL_FORMAT_YUV_SEMIPLANAR_420;

    stLayerAttr.stDispRect.s32X = 0;
    stLayerAttr.stDispRect.s32Y = 0;

    s32Ret = SAMPLE_COMM_VO_GetWH(stVoPubAttr.enIntfSync,
                                  &stLayerAttr.stDispRect.u32Width, &stLayerAttr.stDispRect.u32Height,
                                  &stLayerAttr.u32DispFrmRt);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_GetWH failed!\n");
        goto END_1080P_2;
    }

    stLayerAttr.stImageSize.u32Width  = stLayerAttr.stDispRect.u32Width;
    stLayerAttr.stImageSize.u32Height = stLayerAttr.stDispRect.u32Height;

    s32Ret = SAMPLE_COMM_VO_StartLayer(VoLayer, &stLayerAttr, HI_FALSE);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartChn failed!\n");
        goto END_1080P_3;
    }

    s32Ret = SAMPLE_COMM_VO_StartChn(VoDev, VO_MODE_1MUX);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartChn failed!\n");
        goto END_1080P_4;
    }

    s32Ret = SAMPLE_COMM_VO_BindVpss(VoDev, VoChn, VpssGrp, VpssChn);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_BindVpss(vo:%d)-(VpssChn:%d) failed with %#x!\n", VoDev, VoChn, s32Ret);
        goto END_1080P_5;
    }

    const char* fpn_file = "sample";   /* fpn file name */
    ISP_FPN_TYPE_E enFpnType = ISP_FPN_TYPE_FRAME; /* line/frame */
    PIXEL_FORMAT_E enPixelFormat = PIXEL_FORMAT_RGB_BAYER;
    COMPRESS_MODE_E	enCompressMode = COMPRESS_MODE_NONE;
    HI_U32 u32FrmNum = 16;
    HI_U32 u32Threshold = 4095;
    char fileName[256] = {0};
    VI_CHN_ATTR_S stTempChnAttr;
    VI_CHN ViChn = 0;

    /* fpn calibrate, save dark frame file */
    s32Ret = SAMPLE_COMM_VI_FPN_CALIBRATE_CONFIG(fpn_file, enFpnType,
             enPixelFormat, enCompressMode,
             u32FrmNum, u32Threshold);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("FPN calibrate failed %#x!\n", s32Ret);
        goto END_1080P_5;
    }

    /* fpn correction, read dark frame file */
    ISP_OP_TYPE_E  enOpType = OP_TYPE_AUTO; /* auto/manual */
    HI_U32 u32Strength = 0X100;             /* strength */
    s32Ret =  HI_MPI_VI_GetChnAttr(ViChn, &stTempChnAttr);
    if (HI_SUCCESS != s32Ret)
    {
        printf("get vi chn attr failed!");
        return HI_FAILURE;
    }
    snprintf(fileName, sizeof(fileName), "./%s_%d_%d_%dbit.raw", fpn_file, stTempChnAttr.stDestSize.u32Width,
            stTempChnAttr.stDestSize.u32Height , 16);
    s32Ret = SAMPLE_COMM_VI_CORRECTION_CONFIG(fileName, enFpnType, enOpType,
             u32Strength, enPixelFormat);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("FPN correction failed %#x!\n", s32Ret);
        goto END_1080P_5;
    }

    /******************************************
     step 8: exit process
    ******************************************/
END_1080P_5:
    SAMPLE_COMM_VO_UnBindVpss(VoDev, VoChn, VpssGrp, VpssChn);
    SAMPLE_COMM_VO_StopChn(VoDev, enVoMode);
END_1080P_4:
    SAMPLE_COMM_VO_StopLayer(VoLayer);
END_1080P_3:
    SAMPLE_COMM_VO_StopDev(VoDev);
END_1080P_2:
    SAMPLE_COMM_VPSS_DisableChn(VpssGrp, VpssChn);
    SAMPLE_COMM_VPSS_StopGroup(VpssGrp);
END_1080P_1:
    SAMPLE_COMM_VI_StopVi(pstViConfig);
END_1080P_0:
    if (VB_INVALID_POOLID != g_Pool)
    {
        HI_MPI_VB_DestroyPool(g_Pool);
        g_Pool = VB_INVALID_POOLID;
    }
    SAMPLE_COMM_SYS_Exit();
    return s32Ret;
}

#if 0
/******************************************************************************
* function : vi/vpss: online mode VI: DC(1080P);VPSS(LDC) VO: SD0(CVBS). Embeded isp, phychn channel preview.
******************************************************************************/
HI_S32 SAMPLE_VI_VPSS_LDC_VO_1080P_PreView(SAMPLE_VI_CONFIG_S* pstViConfig)
{
    HI_U32 u32ViChnCnt = 2;
    VB_CONF_S stVbConf;
    VO_DEV VoDev = SAMPLE_VO_DEV_DSD0;;
    VO_CHN VoChn = 0;
    VO_PUB_ATTR_S stVoPubAttr;
    SAMPLE_VO_MODE_E enVoMode = VO_MODE_1MUX;
    PIC_SIZE_E enPicSize = g_enPicSize;
    VO_LAYER VoLayer = 0;
    VO_VIDEO_LAYER_ATTR_S stLayerAttr;
    VPSS_GRP VpssGrp = 0;
    VPSS_CHN VpssChn = 0;
    VPSS_GRP_ATTR_S stVpssGrpAttr;
    VPSS_CHN_ATTR_S stVpssChnAttr;
    VPSS_CHN_MODE_S stVpssChnMode;
    VPSS_EXT_CHN_ATTR_S stVpssExtChnAttr;

    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32BlkSize;
    SIZE_S stSize;
    ROTATE_E enRotate = ROTATE_NONE;
    VPSS_LDC_ATTR_S stLDCAttr;

    /******************************************
     step  1: init global  variable
    ******************************************/
    gs_u32ViFrmRate = (VIDEO_ENCODING_MODE_PAL == gs_enNorm) ? 25 : 30;
    memset(&stVbConf, 0, sizeof(VB_CONF_S));

    u32BlkSize = SAMPLE_COMM_SYS_CalcPicVbBlkSize(gs_enNorm, enPicSize,
                 SAMPLE_PIXEL_FORMAT, SAMPLE_SYS_ALIGN_WIDTH);
    stVbConf.u32MaxPoolCnt = 128;

    /*ddr0 video buffer*/
    stVbConf.astCommPool[0].u32BlkSize = u32BlkSize;
    stVbConf.astCommPool[0].u32BlkCnt = u32ViChnCnt * 15;

    /******************************************
     step 2: start vpss and vi bind vpss (subchn needn't bind vpss in this mode)
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_GetPicSize(gs_enNorm, enPicSize, &stSize);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_SYS_GetPicSize failed!\n");
        goto END_1080P_0;
    }

    if (pstViConfig)
    {
        enRotate = pstViConfig->enRotate;
        if (ROTATE_90 == enRotate || ROTATE_270 == enRotate)
        {
            u32BlkSize = (CEILING_2_POWER(stSize.u32Width, SAMPLE_SYS_ALIGN_WIDTH) * \
                          CEILING_2_POWER(stSize.u32Height, SAMPLE_SYS_ALIGN_WIDTH) * \
                          ((PIXEL_FORMAT_YUV_SEMIPLANAR_422 == SAMPLE_PIXEL_FORMAT) ? 2 : 1.5));
            stVbConf.astCommPool[1].u32BlkSize = u32BlkSize;
            stVbConf.astCommPool[1].u32BlkCnt = 8;
        }
    }

    /******************************************
     step 3: mpp system init.
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("system init failed with %d!\n", s32Ret);
        goto END_1080P_0;
    }

    /******************************************
     step 4: start vi dev & chn to capture
    ******************************************/
    s32Ret = SAMPLE_COMM_VI_StartVi(pstViConfig);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start vi failed!\n");
        goto END_1080P_0;
    }


    stVpssGrpAttr.bDciEn    = HI_FALSE;
    stVpssGrpAttr.bHistEn   = HI_FALSE;
    stVpssGrpAttr.bIeEn     = HI_FALSE;
    stVpssGrpAttr.bNrEn     = HI_TRUE;
    stVpssGrpAttr.enDieMode = VPSS_DIE_MODE_NODIE;
    stVpssGrpAttr.enPixFmt  = PIXEL_FORMAT_YUV_SEMIPLANAR_420;
    stVpssGrpAttr.u32MaxW   = stSize.u32Width;
    stVpssGrpAttr.u32MaxH   = stSize.u32Height;

    /******************************************
     step 5: start vpss group
    ******************************************/
    s32Ret = SAMPLE_COMM_VPSS_StartGroup(VpssGrp, &stVpssGrpAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start VPSS GROUP failed!\n");
        goto END_1080P_1;
    }

    stVpssChnAttr.bBorderEn       = 0;
    stVpssChnAttr.bFlip           = 0;
    stVpssChnAttr.bMirror         = 0;
    stVpssChnAttr.bSpEn           = 0 ;
    stVpssChnAttr.s32DstFrameRate = -1;
    stVpssChnAttr.s32SrcFrameRate = -1;
    stVpssChnMode.bDouble         = HI_FALSE;
    stVpssChnMode.enChnMode       = VPSS_CHN_MODE_USER;
    stVpssChnMode.enCompressMode  = COMPRESS_MODE_NONE;
    stVpssChnMode.enPixelFormat   = PIXEL_FORMAT_YUV_SEMIPLANAR_420;
    stVpssChnMode.u32Width        = stSize.u32Width;
    stVpssChnMode.u32Height       = stSize.u32Height;

    stLDCAttr.bEnable                 = HI_TRUE;
    stLDCAttr.stAttr.enViewType       = LDC_VIEW_TYPE_ALL;
    stLDCAttr.stAttr.s32CenterXOffset = 0;
    stLDCAttr.stAttr.s32CenterYOffset = 0;
    stLDCAttr.stAttr.s32Ratio         = 168;

    s32Ret =  HI_MPI_VPSS_SetLDCAttr(VpssGrp, VpssChn,  &stLDCAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("set LDC failed!\n");
        goto END_1080P_1;
    }

    /******************************************
    step 6: start vpss chn
    ******************************************/
    s32Ret = SAMPLE_COMM_VPSS_EnableChn(VpssGrp, VpssChn, &stVpssChnAttr, &stVpssChnMode, &stVpssExtChnAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start VPSS CHN failed!\n");
        goto END_1080P_1;
    }

    /******************************************
    step 7: start VO SD0 (bind * vi )
    ******************************************/
    stVoPubAttr.enIntfType = g_enVoIntfType;
    if (VO_INTF_BT1120 == g_enVoIntfType)
    {
        stVoPubAttr.enIntfSync = VO_OUTPUT_1080P30;
        gs_u32ViFrmRate = 50;
    }
    else
    {
        stVoPubAttr.enIntfSync = VO_OUTPUT_PAL;
    }
    stVoPubAttr.u32BgColor = 0x000000ff;

    /* In HD, this item should be set to HI_FALSE */
    s32Ret = SAMPLE_COMM_VO_StartDev(VoDev, &stVoPubAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartDev failed!\n");
        goto END_1080P_2;
    }

    stLayerAttr.bClusterMode = HI_FALSE;
    stLayerAttr.bDoubleFrame = HI_FALSE;
    stLayerAttr.enPixFormat = PIXEL_FORMAT_YUV_SEMIPLANAR_420;

    stLayerAttr.stDispRect.s32X = 0;
    stLayerAttr.stDispRect.s32Y = 0;

    s32Ret = SAMPLE_COMM_VO_GetWH(stVoPubAttr.enIntfSync,
                                  &stLayerAttr.stDispRect.u32Width, &stLayerAttr.stDispRect.u32Height,
                                  &stLayerAttr.u32DispFrmRt);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_GetWH failed!\n");
        goto END_1080P_2;
    }
    stLayerAttr.stImageSize.u32Width  = stLayerAttr.stDispRect.u32Width;
    stLayerAttr.stImageSize.u32Height = stLayerAttr.stDispRect.u32Height;

    s32Ret = SAMPLE_COMM_VO_StartLayer(VoLayer, &stLayerAttr, HI_TRUE);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartChn failed!\n");
        goto END_1080P_3;
    }

    s32Ret = SAMPLE_COMM_VO_StartChn(VoDev, VO_MODE_1MUX);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartChn failed!\n");
        goto END_1080P_4;
    }

    s32Ret = SAMPLE_COMM_VO_BindVpss(VoDev, VoChn, VpssGrp, VpssChn);
    //s32Ret = SAMPLE_COMM_VO_BindVi(VoDev, VoChn, ViChn);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_BindVpss(vo:%d)-(VpssChn:%d) failed with %#x!\n", VoDev, VoChn, s32Ret);
        goto END_1080P_5;
    }

    VI_PAUSE();
    /******************************************
     step 8: exit process
    ******************************************/
END_1080P_5:
    SAMPLE_COMM_VO_UnBindVpss(VoDev, VoChn, VpssGrp, VpssChn);
    SAMPLE_COMM_VO_StopChn(VoDev, enVoMode);
END_1080P_4:
    SAMPLE_COMM_VO_StopLayer(VoLayer);
END_1080P_3:
    SAMPLE_COMM_VO_StopDev(VoDev);
END_1080P_2:
    SAMPLE_COMM_VPSS_DisableChn(VpssGrp, VpssChn);
    SAMPLE_COMM_VPSS_StopGroup(VpssGrp);
END_1080P_1:
    SAMPLE_COMM_VI_StopVi(pstViConfig);
END_1080P_0:
    SAMPLE_COMM_SYS_Exit();
    return s32Ret;

}
#endif

/******************************************************************************
* function : vi/vpss: online mode VI: DC;VPSS(ROTATE);VO: SD0(BT656/LCD). Embeded isp, phychn channel preview.
******************************************************************************/
HI_S32 SAMPLE_VI_VPSS_ROTATE_VO_PreView(SAMPLE_VI_CONFIG_S* pstViConfig)
{
    VB_CONF_S stVbConf;
    VO_DEV VoDev = SAMPLE_VO_DEV_DSD0;;
    VO_CHN VoChn = 0;
    VO_PUB_ATTR_S stVoPubAttr;
    SAMPLE_VO_MODE_E enVoMode = VO_MODE_1MUX;
    PIC_SIZE_E enPicSize = g_enPicSize;
    VO_LAYER VoLayer = 0;
    VO_VIDEO_LAYER_ATTR_S stLayerAttr;
    VPSS_GRP VpssGrp = 0;
    VPSS_CHN VpssChn = 0;
    VPSS_GRP_ATTR_S stVpssGrpAttr;
    VPSS_CHN_ATTR_S stVpssChnAttr;
    VPSS_CHN_MODE_S stVpssChnMode;
    VPSS_EXT_CHN_ATTR_S stVpssExtChnAttr;

    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32BlkSize;
    SIZE_S stSize;
    ROTATE_E enRotate = ROTATE_90;

    VO_CSC_S stCSC = {0};

    /******************************************
     step  1: init global  variable
    ******************************************/
    gs_u32ViFrmRate = (VIDEO_ENCODING_MODE_PAL == gs_enNorm) ? 25 : 30;
    memset(&stVbConf, 0, sizeof(VB_CONF_S));

    u32BlkSize = SAMPLE_COMM_SYS_CalcPicVbBlkSize(gs_enNorm, enPicSize,
                 SAMPLE_PIXEL_FORMAT, SAMPLE_SYS_ALIGN_WIDTH);
    stVbConf.u32MaxPoolCnt = 128;

    /*ddr0 video buffer*/
    stVbConf.astCommPool[0].u32BlkSize = u32BlkSize;
    stVbConf.astCommPool[0].u32BlkCnt = VB_CNT;

    /******************************************
     step 2: start vpss and vi bind vpss (subchn needn't bind vpss in this mode)
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_GetPicSize(gs_enNorm, enPicSize, &stSize);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_SYS_GetPicSize failed!\n");
        goto END_1080P_0;
    }

    /******************************************
     step 3: mpp system init.
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("system init failed with %d!\n", s32Ret);
        goto END_1080P_0;
    }

    /******************************************
     step 4: start vi dev & chn to capture
    ******************************************/
    s32Ret = SAMPLE_COMM_VI_StartVi(pstViConfig);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start vi failed!\n");
        goto END_1080P_0;
    }

    stVpssGrpAttr.bDciEn    = HI_FALSE;
    stVpssGrpAttr.bHistEn   = HI_FALSE;
    stVpssGrpAttr.bIeEn     = HI_FALSE;
    stVpssGrpAttr.bNrEn     = HI_TRUE;
    stVpssGrpAttr.enDieMode = VPSS_DIE_MODE_NODIE;
    stVpssGrpAttr.enPixFmt  = PIXEL_FORMAT_YUV_SEMIPLANAR_420;
    stVpssGrpAttr.u32MaxW   = stSize.u32Width;
    stVpssGrpAttr.u32MaxH   = stSize.u32Height;

    /******************************************
     step 5: start vpss group
    ******************************************/
    s32Ret = SAMPLE_COMM_VPSS_StartGroup(VpssGrp, &stVpssGrpAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start VPSS GROUP failed!\n");
        goto END_1080P_1;
    }

    stVpssChnAttr.bBorderEn = 0;
    stVpssChnAttr.bFlip = 0;
    stVpssChnAttr.bMirror = 0;
    stVpssChnAttr.bSpEn = 0 ;
    stVpssChnAttr.s32DstFrameRate = -1;
    stVpssChnAttr.s32SrcFrameRate = -1;
    stVpssChnMode.bDouble = HI_FALSE;
    stVpssChnMode.enChnMode = VPSS_CHN_MODE_USER;
    stVpssChnMode.enCompressMode = COMPRESS_MODE_NONE;
    stVpssChnMode.enPixelFormat = PIXEL_FORMAT_YUV_SEMIPLANAR_420;
    stVpssChnMode.u32Width  = stSize.u32Width;
    stVpssChnMode.u32Height = stSize.u32Height;

    s32Ret =  HI_MPI_VPSS_SetRotate(VpssGrp, VpssChn,  enRotate);

    /******************************************
    step 6: start vpss chn
    ******************************************/
    s32Ret = SAMPLE_COMM_VPSS_EnableChn(VpssGrp, VpssChn, &stVpssChnAttr, &stVpssChnMode, &stVpssExtChnAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start VPSS CHN failed!\n");
        goto END_1080P_1;
    }

    /******************************************
    step 7: start VO SD0 (bind * vi )
    ******************************************/
    stVoPubAttr.enIntfType = g_enVoIntfType;

    if (VO_INTF_BT656 == g_enVoIntfType)
    {
        stVoPubAttr.enIntfSync = VO_OUTPUT_PAL;
        gs_u32ViFrmRate = 50;
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

    stVoPubAttr.u32BgColor = 0x000000ff;

    /* In HD, this item should be set to HI_FALSE */
    s32Ret = SAMPLE_COMM_VO_StartDev(VoDev, &stVoPubAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartDev failed!\n");
        goto END_1080P_2;
    }

    stLayerAttr.bClusterMode = HI_FALSE;
    stLayerAttr.bDoubleFrame = HI_FALSE;
    stLayerAttr.enPixFormat  = PIXEL_FORMAT_YUV_SEMIPLANAR_420;

    stLayerAttr.stDispRect.s32X = 0;
    stLayerAttr.stDispRect.s32Y = 0;

    s32Ret = SAMPLE_COMM_VO_GetWH(stVoPubAttr.enIntfSync,
                                  &stLayerAttr.stDispRect.u32Width, &stLayerAttr.stDispRect.u32Height,
                                  &stLayerAttr.u32DispFrmRt);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_GetWH failed!\n");
        goto END_1080P_2;
    }

    stLayerAttr.stImageSize.u32Width  = stLayerAttr.stDispRect.u32Width;
    stLayerAttr.stImageSize.u32Height = stLayerAttr.stDispRect.u32Height;

    s32Ret = SAMPLE_COMM_VO_StartLayer(VoLayer, &stLayerAttr, HI_FALSE);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartChn failed!\n");
        goto END_1080P_3;
    }

    s32Ret = SAMPLE_COMM_VO_StartChn(VoDev, VO_MODE_1MUX);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartChn failed!\n");
        goto END_1080P_4;
    }

    s32Ret = SAMPLE_COMM_VO_BindVpss(VoDev, VoChn, VpssGrp, VpssChn);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_BindVpss(vo:%d)-(VpssChn:%d) failed with %#x!\n", VoDev, VoChn, s32Ret);
        goto END_1080P_5;
    }

    VI_PAUSE();
    /******************************************
     step 8: exit process
    ******************************************/
END_1080P_5:
    SAMPLE_COMM_VO_UnBindVpss(VoDev, VoChn, VpssGrp, VpssChn);
    SAMPLE_COMM_VO_StopChn(VoDev, enVoMode);
END_1080P_4:
    SAMPLE_COMM_VO_StopLayer(VoLayer);
END_1080P_3:
    SAMPLE_COMM_VO_StopDev(VoDev);
END_1080P_2:
    SAMPLE_COMM_VPSS_DisableChn(VpssGrp, VpssChn);
    SAMPLE_COMM_VPSS_StopGroup(VpssGrp);
END_1080P_1:
    SAMPLE_COMM_VI_StopVi(pstViConfig);
END_1080P_0:
    SAMPLE_COMM_SYS_Exit();
    return s32Ret;
}



/******************************************************************************
* function : vi/vpss: offline mode VI: DC,ROTATE;VO: SD0(BT656/LCD). Embeded isp, phychn channel preview.
******************************************************************************/
HI_S32 SAMPLE_VI_ROTATE_VO_PreView(SAMPLE_VI_CONFIG_S* pstViConfig)
{
    VB_CONF_S stVbConf;
    VO_DEV VoDev = SAMPLE_VO_DEV_DSD0;;
    VO_CHN VoChn = 0;
    VI_CHN ViChn = 0;
    VO_PUB_ATTR_S stVoPubAttr;
    SAMPLE_VO_MODE_E enVoMode = VO_MODE_1MUX;
    PIC_SIZE_E enPicSize = g_enPicSize;
    VO_LAYER VoLayer = 0;
    VO_VIDEO_LAYER_ATTR_S stLayerAttr;

    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32BlkSize;
    SIZE_S stSize;

    VO_CSC_S stCSC = {0};

    /******************************************
     step  1: init global  variable
    ******************************************/
    gs_u32ViFrmRate = (VIDEO_ENCODING_MODE_PAL == gs_enNorm) ? 25 : 30;
    memset(&stVbConf, 0, sizeof(VB_CONF_S));

    u32BlkSize = SAMPLE_COMM_SYS_CalcPicVbBlkSize(gs_enNorm, enPicSize,
                 SAMPLE_PIXEL_FORMAT, SAMPLE_SYS_ALIGN_WIDTH);
    stVbConf.u32MaxPoolCnt = 128;

    /*ddr0 video buffer*/
    stVbConf.astCommPool[0].u32BlkSize = u32BlkSize;
    stVbConf.astCommPool[0].u32BlkCnt = VB_CNT;

    /******************************************
     step 2: start vpss and vi bind vpss (subchn needn't bind vpss in this mode)
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_GetPicSize(gs_enNorm, enPicSize, &stSize);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_SYS_GetPicSize failed!\n");
        goto END_1080P_0;
    }

    /******************************************
     step 3: mpp system init.
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("system init failed with %d!\n", s32Ret);
        goto END_1080P_0;
    }

    /******************************************
     step 4: start vi dev & chn to capture
    ******************************************/
    s32Ret = SAMPLE_COMM_VI_StartVi(pstViConfig);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start vi failed!\n");
        goto END_1080P_0;
    }



    /******************************************
    step 7: start VO SD0 (bind * vi )
    ******************************************/
    stVoPubAttr.enIntfType = g_enVoIntfType;

    if (VO_INTF_BT656 == g_enVoIntfType)
    {
        stVoPubAttr.enIntfSync = VO_OUTPUT_PAL;
        gs_u32ViFrmRate = 50;
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
    stVoPubAttr.u32BgColor = 0x000000ff;

    /* In HD, this item should be set to HI_FALSE */
    s32Ret = SAMPLE_COMM_VO_StartDev(VoDev, &stVoPubAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartDev failed!\n");
        goto END_1080P_1;
    }

    stLayerAttr.bClusterMode = HI_FALSE;
    stLayerAttr.bDoubleFrame = HI_FALSE;
    stLayerAttr.enPixFormat = PIXEL_FORMAT_YUV_SEMIPLANAR_420;

    stLayerAttr.stDispRect.s32X = 0;
    stLayerAttr.stDispRect.s32Y = 0;

    s32Ret = SAMPLE_COMM_VO_GetWH(stVoPubAttr.enIntfSync,
                                  &stLayerAttr.stDispRect.u32Width, &stLayerAttr.stDispRect.u32Height,
                                  &stLayerAttr.u32DispFrmRt);
    printf("w = %d, h = %d\n", stLayerAttr.stDispRect.u32Width, stLayerAttr.stDispRect.u32Height);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_GetWH failed!\n");
        goto END_1080P_1;
    }

    stLayerAttr.stImageSize.u32Width  = stLayerAttr.stDispRect.u32Width;
    stLayerAttr.stImageSize.u32Height = stLayerAttr.stDispRect.u32Height;

    s32Ret = SAMPLE_COMM_VO_StartLayer(VoLayer, &stLayerAttr, HI_FALSE);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartChn failed!\n");
        goto END_1080P_2;
    }
    s32Ret = SAMPLE_COMM_VO_StartChn(VoDev, VO_MODE_1MUX);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartChn failed!\n");
        goto END_1080P_3;
    }

    //s32Ret = SAMPLE_COMM_VO_BindVpss(VoDev, VoChn, VpssGrp, VpssChn);
    s32Ret = SAMPLE_COMM_VO_BindVi(VoDev, VoChn, ViChn);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_BindVpss(vo:%d)-(VpssChn:%d) failed with %#x!\n", VoDev, VoChn, s32Ret);
        goto END_1080P_4;
    }

    VI_PAUSE();
    /******************************************
     step 8: exit process
    ******************************************/
END_1080P_4:
    SAMPLE_COMM_VO_UnBindVi(VoDev, VoChn);
    SAMPLE_COMM_VO_StopChn(VoDev, enVoMode);
END_1080P_3:
    SAMPLE_COMM_VO_StopLayer(VoLayer);
END_1080P_2:
    SAMPLE_COMM_VO_StopDev(VoDev);
END_1080P_1:
    SAMPLE_COMM_VI_StopVi(pstViConfig);
END_1080P_0:
    SAMPLE_COMM_SYS_Exit();

    return s32Ret;
}



/******************************************************************************
* function : vi/vpss: offline mode VI: DC,LDC;VO: SD0(BT656/LCD). Embeded isp, phychn channel preview.
******************************************************************************/
HI_S32 SAMPLE_VI_LDC_VO_PreView(SAMPLE_VI_CONFIG_S* pstViConfig)
{
    VB_CONF_S stVbConf;
    VO_DEV VoDev = SAMPLE_VO_DEV_DSD0;
    VO_CHN VoChn = 0;
    VI_CHN ViChn = 0;
    VO_PUB_ATTR_S stVoPubAttr;
    SAMPLE_VO_MODE_E enVoMode = VO_MODE_1MUX;
    PIC_SIZE_E enPicSize = g_enPicSize;
    VO_LAYER VoLayer = 0;
    VO_VIDEO_LAYER_ATTR_S stLayerAttr;

    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32BlkSize;
    SIZE_S stSize;
    VI_LDC_ATTR_S stLDCAttr;

    VO_CSC_S stCSC = {0};

    /******************************************
     step  1: init global  variable
    ******************************************/
    gs_u32ViFrmRate = (VIDEO_ENCODING_MODE_PAL == gs_enNorm) ? 25 : 30;
    memset(&stVbConf, 0, sizeof(VB_CONF_S));

    u32BlkSize = SAMPLE_COMM_SYS_CalcPicVbBlkSize(gs_enNorm, enPicSize,
                 SAMPLE_PIXEL_FORMAT, SAMPLE_SYS_ALIGN_WIDTH);
    stVbConf.u32MaxPoolCnt = 128;

    /*ddr0 video buffer*/
    stVbConf.astCommPool[0].u32BlkSize = u32BlkSize;
    stVbConf.astCommPool[0].u32BlkCnt = VB_CNT;

    /******************************************
     step 2: start vpss and vi bind vpss (subchn needn't bind vpss in this mode)
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_GetPicSize(gs_enNorm, enPicSize, &stSize);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_SYS_GetPicSize failed!\n");
        goto END_1080P_0;
    }

    /******************************************
     step 3: mpp system init.
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("system init failed with %d!\n", s32Ret);
        goto END_1080P_0;
    }

    /******************************************
     step 4: start vi dev & chn to capture
    ******************************************/
    s32Ret = SAMPLE_COMM_VI_StartVi(pstViConfig);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start vi failed!\n");
        goto END_1080P_0;
    }



    stLDCAttr.bEnable = HI_TRUE;
    stLDCAttr.stAttr.enViewType = LDC_VIEW_TYPE_ALL;
    stLDCAttr.stAttr.s32CenterXOffset = 0;
    stLDCAttr.stAttr.s32CenterYOffset = 0;
    stLDCAttr.stAttr.s32Ratio = 100;

    s32Ret =  HI_MPI_VI_SetLDCAttr(ViChn, &stLDCAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("set LDC failed!\n");
        goto END_1080P_1;
    }

    /******************************************
    step 7: start VO SD0 (bind * vi )
    ******************************************/
    stVoPubAttr.enIntfType = g_enVoIntfType;

    if (VO_INTF_BT656 == g_enVoIntfType)
    {
        stVoPubAttr.enIntfSync = VO_OUTPUT_PAL;
        gs_u32ViFrmRate = 50;
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

    stVoPubAttr.u32BgColor = 0x000000ff;

    /* In HD, this item should be set to HI_FALSE */
    s32Ret = SAMPLE_COMM_VO_StartDev(VoDev, &stVoPubAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartDev failed!\n");
        goto END_1080P_1;
    }

    stLayerAttr.bClusterMode = HI_FALSE;
    stLayerAttr.bDoubleFrame = HI_FALSE;
    stLayerAttr.enPixFormat = PIXEL_FORMAT_YUV_SEMIPLANAR_420;

    stLayerAttr.stDispRect.s32X = 0;
    stLayerAttr.stDispRect.s32Y = 0;
    s32Ret = SAMPLE_COMM_VO_GetWH(stVoPubAttr.enIntfSync,
                                  &stLayerAttr.stDispRect.u32Width, &stLayerAttr.stDispRect.u32Height,
                                  &stLayerAttr.u32DispFrmRt);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_GetWH failed!\n");
        goto END_1080P_1;
    }

    stLayerAttr.stImageSize.u32Width  = stLayerAttr.stDispRect.u32Width;
    stLayerAttr.stImageSize.u32Height = stLayerAttr.stDispRect.u32Height;

    s32Ret = SAMPLE_COMM_VO_StartLayer(VoLayer, &stLayerAttr, HI_FALSE);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartChn failed!\n");
        goto END_1080P_2;
    }


    s32Ret = SAMPLE_COMM_VO_StartChn(VoDev, VO_MODE_1MUX);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartChn failed!\n");
        goto END_1080P_3;
    }

    //s32Ret = SAMPLE_COMM_VO_BindVpss(VoDev, VoChn, VpssGrp, VpssChn);
    s32Ret = SAMPLE_COMM_VO_BindVi(VoDev, VoChn, ViChn);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_BindVpss(vo:%d)-(VpssChn:%d) failed with %#x!\n", VoDev, VoChn, s32Ret);
        goto END_1080P_4;
    }

    printf("\nplease hit the Enter key, disable LDC\n\n");
    VI_PAUSE();
    stLDCAttr.bEnable = HI_FALSE;

    s32Ret =  HI_MPI_VI_SetLDCAttr(ViChn, &stLDCAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("set LDC failed with %#x!\n", s32Ret);
        goto END_1080P_4;
    }

    printf("\nplease hit the Enter key, enable LDC\n");
    VI_PAUSE();

    stLDCAttr.bEnable = HI_TRUE;

    s32Ret =  HI_MPI_VI_SetLDCAttr(ViChn, &stLDCAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("set LDC failed with %#x!\n", s32Ret);
        goto END_1080P_4;
    }

    VI_PAUSE();
    /******************************************
     step 8: exit process
    ******************************************/
END_1080P_4:
    SAMPLE_COMM_VO_UnBindVi(VoDev, VoChn);
    SAMPLE_COMM_VO_StopChn(VoDev, enVoMode);
END_1080P_3:
    SAMPLE_COMM_VO_StopLayer(VoLayer);
END_1080P_2:
    SAMPLE_COMM_VO_StopDev(VoDev);
END_1080P_1:
    SAMPLE_COMM_VI_StopVi(pstViConfig);
END_1080P_0:
    SAMPLE_COMM_SYS_Exit();

    return HI_SUCCESS;
}


/******************************************************************************
* function : vi/vpss: offline mode VI: DC,DIS; VPSS:3DNR; VO: SD0(BT656/LCD). Embeded isp, phychn channel preview.
******************************************************************************/

HI_S32 SAMPLE_VI_DIS_VPSS_VO_PreView(SAMPLE_VI_CONFIG_S* pstViConfig)
{
    VB_CONF_S stVbConf;
    VO_DEV VoDev = SAMPLE_VO_DEV_DSD0;;
    VO_CHN VoChn = 0;
    VI_CHN ViChn = 0;
    VO_PUB_ATTR_S stVoPubAttr;
    SAMPLE_VO_MODE_E enVoMode = VO_MODE_2MUX;
    PIC_SIZE_E enPicSize = g_enPicSize;
    VO_LAYER VoLayer = 0;
    VO_VIDEO_LAYER_ATTR_S stLayerAttr;
    VPSS_GRP VpssGrp = 0;
    VPSS_CHN VpssChn = 1;
    VPSS_GRP_ATTR_S stVpssGrpAttr;
    VPSS_CHN_ATTR_S stVpssChnAttr;
    VPSS_CHN_MODE_S stVpssChnMode;
    VI_CHN_ATTR_S stChnAttr;
    VPSS_EXT_CHN_ATTR_S stVpssExtChnAttr;

    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32BlkSize;
    SIZE_S stSize;

    VO_CSC_S stCSC = {0};

    /******************************************
     step  1: init global  variable
    ******************************************/
    gs_u32ViFrmRate = (VIDEO_ENCODING_MODE_PAL == gs_enNorm) ? 25 : 30;
    memset(&stVbConf, 0, sizeof(VB_CONF_S));

    u32BlkSize = SAMPLE_COMM_SYS_CalcPicVbBlkSize(gs_enNorm, enPicSize,
                 SAMPLE_PIXEL_FORMAT, SAMPLE_SYS_ALIGN_WIDTH);
    stVbConf.u32MaxPoolCnt = 128;

    /*ddr0 video buffer*/
    stVbConf.astCommPool[0].u32BlkSize = u32BlkSize;
    stVbConf.astCommPool[0].u32BlkCnt = VB_CNT;

    /******************************************
     step 2: start vpss and vi bind vpss (subchn needn't bind vpss in this mode)
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_GetPicSize(gs_enNorm, enPicSize, &stSize);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_SYS_GetPicSize failed!\n");
        goto END_1080P_0;
    }

    /******************************************
     step 3: mpp system init.
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("system init failed with %d!\n", s32Ret);
        goto END_1080P_0;
    }

    /******************************************
     step 4: start vi dev & chn to capture
    ******************************************/
    s32Ret = SAMPLE_COMM_VI_StartVi(pstViConfig);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start vi failed!\n");
        goto END_1080P_0;
    }

    /* crop vi chn  */
    s32Ret =  HI_MPI_VI_DisableChn(ViChn);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("disable  vi chn failed!\n");
        goto END_1080P_0;
    }
    stChnAttr.stCapRect.s32X = 0;
    stChnAttr.stCapRect.s32Y = 0;
    stChnAttr.stCapRect.u32Width = 1180 + 100;
    stChnAttr.stCapRect.u32Height = 620 + 100;
    stChnAttr.bFlip = HI_FALSE;
    stChnAttr.bMirror = HI_FALSE;
    stChnAttr.enCapSel = VI_CAPSEL_BOTH;
    stChnAttr.enPixFormat = PIXEL_FORMAT_YUV_SEMIPLANAR_420;
    stChnAttr.enCompressMode = COMPRESS_MODE_NONE;
    stChnAttr.s32SrcFrameRate = 30;
    stChnAttr.s32DstFrameRate = 30;

    stChnAttr.stDestSize.u32Width = stChnAttr.stCapRect.u32Width;
    stChnAttr.stDestSize.u32Height = stChnAttr.stCapRect.u32Height;
    s32Ret =  HI_MPI_VI_SetChnAttr(ViChn, &stChnAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("set vi chn failed!\n");
        goto END_1080P_0;
    }

    s32Ret =  HI_MPI_VI_EnableChn(ViChn);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("enable vi chn failed!\n");
        goto END_1080P_0;
    }

    stVpssGrpAttr.bDciEn = HI_FALSE;
    stVpssGrpAttr.bHistEn = HI_FALSE;
    stVpssGrpAttr.bIeEn = HI_FALSE;
    stVpssGrpAttr.bNrEn = HI_FALSE;
    stVpssGrpAttr.enDieMode = VPSS_DIE_MODE_NODIE;
    stVpssGrpAttr.enPixFmt = PIXEL_FORMAT_YUV_SEMIPLANAR_420;
    stVpssGrpAttr.u32MaxH = 720 + 100;
    stVpssGrpAttr.u32MaxW = 1280 + 100;


    /******************************************
     step 5: start vpss group
    ******************************************/
    s32Ret = SAMPLE_COMM_VPSS_StartGroup(VpssGrp, &stVpssGrpAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start VPSS GROUP failed!\n");
        goto END_1080P_1;
    }

    stVpssChnAttr.bBorderEn = 0;
    stVpssChnAttr.bFlip = 0;
    stVpssChnAttr.bMirror = 0;
    stVpssChnAttr.bSpEn = 0 ;
    stVpssChnAttr.s32DstFrameRate = -1;
    stVpssChnAttr.s32SrcFrameRate = -1;
    stVpssChnMode.bDouble = HI_FALSE;
    stVpssChnMode.enChnMode = VPSS_CHN_MODE_USER;
    stVpssChnMode.enCompressMode = COMPRESS_MODE_NONE;
    stVpssChnMode.enPixelFormat = PIXEL_FORMAT_YUV_SEMIPLANAR_420;
    stVpssChnMode.u32Width  = 1180;
    stVpssChnMode.u32Height = 620;


    VPSS_CROP_INFO_S stVpssCropInfo;

    stVpssCropInfo.bEnable = HI_TRUE;
    stVpssCropInfo.enCropCoordinate = VPSS_CROP_ABS_COOR;
    stVpssCropInfo.stCropRect.s32X = 50;
    stVpssCropInfo.stCropRect.s32Y = 50;
    stVpssCropInfo.stCropRect.u32Width  = 1180;
    stVpssCropInfo.stCropRect.u32Height = 620;
    s32Ret = HI_MPI_VPSS_SetGrpCrop(VpssGrp, &stVpssCropInfo);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("set VPSS group crop failed!\n");
        goto END_1080P_1;
    }

    /******************************************
    step 6: start vpss chn
    ******************************************/
    s32Ret = SAMPLE_COMM_VPSS_EnableChn(VpssGrp, VpssChn, &stVpssChnAttr, &stVpssChnMode, &stVpssExtChnAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start VPSS CHN failed!\n");
        goto END_1080P_1;
    }

    /******************************************
    step 7: start VO SD0 (bind * vi )
    ******************************************/
    stVoPubAttr.enIntfType = g_enVoIntfType;

    if (VO_INTF_BT656 == g_enVoIntfType)
    {
        stVoPubAttr.enIntfSync = VO_OUTPUT_PAL;
        gs_u32ViFrmRate = 50;
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

    stVoPubAttr.u32BgColor = 0x000000ff;

    /* In HD, this item should be set to HI_FALSE */
    s32Ret = SAMPLE_COMM_VO_StartDev(VoDev, &stVoPubAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartDev failed!\n");
        goto END_1080P_2;
    }

    stLayerAttr.bClusterMode = HI_FALSE;
    stLayerAttr.bDoubleFrame = HI_FALSE;
    stLayerAttr.enPixFormat = PIXEL_FORMAT_YUV_SEMIPLANAR_420;

    stLayerAttr.stDispRect.s32X = 0;
    stLayerAttr.stDispRect.s32Y = 0;

    s32Ret = SAMPLE_COMM_VO_GetWH(stVoPubAttr.enIntfSync,
                                  &stLayerAttr.stDispRect.u32Width, &stLayerAttr.stDispRect.u32Height,
                                  &stLayerAttr.u32DispFrmRt);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_GetWH failed!\n");
        goto END_1080P_2;
    }
    stLayerAttr.stImageSize.u32Width  = stLayerAttr.stDispRect.u32Width;
    stLayerAttr.stImageSize.u32Height = stLayerAttr.stDispRect.u32Height;

    s32Ret = SAMPLE_COMM_VO_StartLayer(VoLayer, &stLayerAttr, HI_FALSE);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartChn failed!\n");
        goto END_1080P_3;
    }

    s32Ret = SAMPLE_COMM_VO_StartChn(VoDev, enVoMode);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartChn failed!\n");
        goto END_1080P_4;
    }

    s32Ret = SAMPLE_COMM_VI_BindVpss(pstViConfig->enViMode);

    s32Ret = SAMPLE_COMM_VO_BindVpss(VoDev, VoChn + 1, VpssGrp, VpssChn);
    s32Ret = SAMPLE_COMM_VO_BindVi(VoDev, VoChn, ViChn);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_BindVpss(vo:%d)-(VpssChn:%d) failed with %#x!\n", VoDev, VoChn, s32Ret);
        goto END_1080P_5;
    }


    ISP_DIS_ATTR_S stDisAttr;
    stDisAttr.bEnable = HI_TRUE;

    int IspDev = 0;
    s32Ret =  HI_MPI_ISP_SetDISAttr(IspDev, &stDisAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("set DIS atrr failed!\n");
        goto END_1080P_5;
    }

    printf("\n\npress key to disable dis ------\n");
    VI_PAUSE();

    s32Ret =  HI_MPI_ISP_GetDISAttr(IspDev, &stDisAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("get DIS atrr failed!\n");
        goto END_1080P_5;
    }
    stDisAttr.bEnable = HI_FALSE;
    s32Ret =  HI_MPI_ISP_SetDISAttr(IspDev, &stDisAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("set DIS atrr failed!\n");
        goto END_1080P_5;
    }
    printf("\n\npress key to enable dis ------\n");
    VI_PAUSE();

    s32Ret =  HI_MPI_ISP_GetDISAttr(IspDev, &stDisAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("get DIS atrr failed!\n");
        goto END_1080P_5;
    }

    stDisAttr.bEnable = HI_TRUE;
    s32Ret =  HI_MPI_ISP_SetDISAttr(IspDev, &stDisAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("set DIS atrr failed!\n");
        goto END_1080P_5;
    }
    VI_PAUSE();

    /******************************************
     step 8: exit process
    ******************************************/
END_1080P_5:
    SAMPLE_COMM_VO_UnBindVi(VoDev, VoChn);
    SAMPLE_COMM_VO_StopChn(VoDev, enVoMode);
END_1080P_4:
    SAMPLE_COMM_VO_StopLayer(VoLayer);
END_1080P_3:
    SAMPLE_COMM_VO_StopDev(VoDev);
END_1080P_2:
    SAMPLE_COMM_VPSS_DisableChn(VpssGrp, VpssChn);
    SAMPLE_COMM_VPSS_StopGroup(VpssGrp);
END_1080P_1:
    SAMPLE_COMM_VI_StopVi(pstViConfig);
END_1080P_0:
    SAMPLE_COMM_SYS_Exit();

    return s32Ret;
}



/******************************************************************************
* function :  VI: BT1120; VO: SD0(LCD/BT656). bypass isp, phychn channel preview.
******************************************************************************/
HI_S32 SAMPLE_VIO_BT1120_PreView(SAMPLE_VI_CONFIG_S* pstViConfig)
{
    VB_CONF_S stVbConf;
    VO_DEV VoDev = SAMPLE_VO_DEV_DSD0;;
    VO_CHN VoChn = 0;
    VI_CHN ViChn = 0;
    VO_PUB_ATTR_S stVoPubAttr;
    SAMPLE_VO_MODE_E enVoMode = VO_MODE_1MUX;
    PIC_SIZE_E enPicSize = g_enPicSize;

    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32BlkSize;
    SIZE_S stSize;
    VO_LAYER VoLayer = 0;
    VO_VIDEO_LAYER_ATTR_S stLayerAttr;

    VO_CSC_S stCSC = {0};

    /******************************************
     step  1: init global  variable
    ******************************************/
    gs_u32ViFrmRate = (VIDEO_ENCODING_MODE_PAL == gs_enNorm) ? 25 : 30;
    memset(&stVbConf, 0, sizeof(VB_CONF_S));

    u32BlkSize = SAMPLE_COMM_SYS_CalcPicVbBlkSize(gs_enNorm, enPicSize,
                 SAMPLE_PIXEL_FORMAT, SAMPLE_SYS_ALIGN_WIDTH);
    stVbConf.u32MaxPoolCnt = 128;

    /*ddr0 video buffer*/
    stVbConf.astCommPool[0].u32BlkSize = u32BlkSize;
    stVbConf.astCommPool[0].u32BlkCnt = VB_CNT;

    /******************************************
     step 2: mpp system init.
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("system init failed with %d!\n", s32Ret);
        goto END_1080P_0;
    }

    /******************************************
     step 3: start vi dev & chn to capture
    ******************************************/
    s32Ret = SAMPLE_COMM_VI_StartVi(pstViConfig);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start vi failed!\n");
        goto END_1080P_0;
    }

    /******************************************
     step 4: start vpss and vi bind vpss (subchn needn't bind vpss in this mode)
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_GetPicSize(gs_enNorm, enPicSize, &stSize);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_SYS_GetPicSize failed!\n");
        goto END_1080P_1;
    }

    /******************************************
    step 5: start VO SD0 (bind * vi )
    ******************************************/
    stVoPubAttr.enIntfType = g_enVoIntfType;

    if (VO_INTF_BT656 == g_enVoIntfType)
    {
        stVoPubAttr.enIntfSync = VO_OUTPUT_PAL;
        gs_u32ViFrmRate = 50;
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

    stVoPubAttr.u32BgColor = 0x000000ff;
    /* In HD, this item should be set to HI_FALSE */
    s32Ret = SAMPLE_COMM_VO_StartDev(VoDev, &stVoPubAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartDevLayer failed!\n");
        goto END_1080P_1;
    }

    stLayerAttr.bClusterMode = HI_FALSE;
    stLayerAttr.bDoubleFrame = HI_FALSE;
    stLayerAttr.enPixFormat = PIXEL_FORMAT_YUV_SEMIPLANAR_420;

    stLayerAttr.stDispRect.s32X = 0;
    stLayerAttr.stDispRect.s32Y = 0;
    s32Ret = SAMPLE_COMM_VO_GetWH(stVoPubAttr.enIntfSync,
                                  &stLayerAttr.stDispRect.u32Width, &stLayerAttr.stDispRect.u32Height,
                                  &stLayerAttr.u32DispFrmRt);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_GetWH failed!\n");
        goto END_1080P_1;
    }
    stLayerAttr.stImageSize.u32Width  = stLayerAttr.stDispRect.u32Width;
    stLayerAttr.stImageSize.u32Height = stLayerAttr.stDispRect.u32Height;

    s32Ret = SAMPLE_COMM_VO_StartLayer(VoLayer, &stLayerAttr, HI_FALSE);

    s32Ret = SAMPLE_COMM_VO_StartChn(VoDev, VO_MODE_1MUX);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartChn failed!\n");
        goto END_1080P_2;
    }

    s32Ret = SAMPLE_COMM_VO_BindVi(VoDev, VoChn, ViChn);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_BindVi(vo:%d)-(vichn:%d) failed with %#x!\n", VoDev, VoChn, s32Ret);
        goto END_1080P_3;
    }

    VI_PAUSE();
    /******************************************
     step 6: exit process
    ******************************************/
END_1080P_3:
    SAMPLE_COMM_VO_UnBindVi(VoDev, VoChn);
    SAMPLE_COMM_VO_StopChn(VoDev, enVoMode);
    SAMPLE_COMM_VO_StopLayer(VoLayer);
END_1080P_2:
    SAMPLE_COMM_VO_StopDev(VoDev);
END_1080P_1:
    SAMPLE_COMM_VI_StopVi(pstViConfig);
END_1080P_0:
    SAMPLE_COMM_SYS_Exit();

    return s32Ret;
}

/******************************************************************************
* function :  VI: DC(720P); VO: SD0(CVBS). Embeded isp, one ExtChn preview.
******************************************************************************/
HI_S32 SAMPLE_VIO_720P_Extchn_Preview(SAMPLE_VI_CONFIG_S* pstViConfig)
{
    VB_CONF_S stVbConf;
    VO_DEV VoDev = SAMPLE_VO_DEV_DSD0;;
    VO_CHN VoChn = 0;
    VI_CHN ViChn = 0;
    VO_PUB_ATTR_S stVoPubAttr;
    SAMPLE_VO_MODE_E enVoMode = VO_MODE_1MUX;

    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32BlkSize;
    SIZE_S stSize;
    VI_EXT_CHN_ATTR_S stExtChnAttr;
    VI_CHN ExtChn = 1;
    PIC_SIZE_E enPicSize = g_enPicSize;

    VO_CSC_S stCSC = {0};

    /******************************************
     step  1: init global  variable
    ******************************************/
    gs_u32ViFrmRate = (VIDEO_ENCODING_MODE_PAL == gs_enNorm) ? 25 : 30;
    memset(&stVbConf, 0, sizeof(VB_CONF_S));

    u32BlkSize = SAMPLE_COMM_SYS_CalcPicVbBlkSize(gs_enNorm, enPicSize,
                 SAMPLE_PIXEL_FORMAT, SAMPLE_SYS_ALIGN_WIDTH);
    stVbConf.u32MaxPoolCnt = 128;

    /*ddr0 video buffer*/
    stVbConf.astCommPool[0].u32BlkSize = u32BlkSize;
    stVbConf.astCommPool[0].u32BlkCnt = 5;

    u32BlkSize = SAMPLE_COMM_SYS_CalcPicVbBlkSize(gs_enNorm, PIC_D1,
                 SAMPLE_PIXEL_FORMAT, SAMPLE_SYS_ALIGN_WIDTH);
    stVbConf.astCommPool[1].u32BlkSize = u32BlkSize;
    stVbConf.astCommPool[1].u32BlkCnt = 2;


    /******************************************
     step 2: mpp system init.
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("system init failed with %d!\n", s32Ret);
        goto END_1080P_0;
    }

    /******************************************
     step 3: start vi dev & chn to capture
    ******************************************/
    s32Ret = SAMPLE_COMM_VI_StartVi(pstViConfig);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start vi failed!\n");
        goto END_1080P_0;
    }

    stExtChnAttr.enPixFormat = SAMPLE_PIXEL_FORMAT;
    stExtChnAttr.s32BindChn = ViChn;
    stExtChnAttr.stDestSize.u32Width = 720;
    stExtChnAttr.stDestSize.u32Height = 576;
    stExtChnAttr.s32DstFrameRate = -1;
    stExtChnAttr.s32SrcFrameRate = -1;

    s32Ret = HI_MPI_VI_SetExtChnAttr(ExtChn, &stExtChnAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_VI_SetExtChnAttr failed!\n");
        goto END_1080P_0;
    }
    s32Ret = HI_MPI_VI_EnableChn(ExtChn);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("HI_MPI_VI_EnableChn failed!\n");
        goto END_1080P_0;
    }

    /******************************************
     step 4: start vpss and vi bind vpss (subchn needn't bind vpss in this mode)
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_GetPicSize(gs_enNorm, enPicSize, &stSize);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_SYS_GetPicSize failed!\n");
        goto END_1080P_1;
    }

    /******************************************
    step 5: start VO SD0 (bind * vi )
    ******************************************/
    stVoPubAttr.enIntfType = g_enVoIntfType;
#ifndef HI_FPGA
    if (VO_INTF_BT656 == g_enVoIntfType)
    {
        stVoPubAttr.enIntfSync = VO_OUTPUT_PAL;
        gs_u32ViFrmRate = 50;
    }
#else
    if (VO_INTF_BT1120 == g_enVoIntfType)
    {
        stVoPubAttr.enIntfSync = VO_OUTPUT_1080P30;
        gs_u32ViFrmRate = 50;
    }
#endif

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

    stVoPubAttr.u32BgColor = 0x000000ff;
    /* In HD, this item should be set to HI_FALSE */
    s32Ret = SAMPLE_COMM_VO_StartDev(VoDev, &stVoPubAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartDevLayer failed!\n");
        goto END_1080P_1;
    }

    s32Ret = SAMPLE_COMM_VO_StartChn(VoDev, VO_MODE_1MUX);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartChn failed!\n");
        goto END_1080P_2;
    }

    s32Ret = SAMPLE_COMM_VO_BindVi(VoDev, VoChn, ExtChn);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_BindVi(vo:%d)-(vichn:%d) failed with %#x!\n", VoDev, VoChn, s32Ret);
        goto END_1080P_3;
    }

    VI_PAUSE();
    /******************************************
     step 6: exit process
    ******************************************/
END_1080P_3:
    SAMPLE_COMM_VO_UnBindVi(VoDev, VoChn);
    SAMPLE_COMM_VO_StopChn(VoDev, enVoMode);
END_1080P_2:
    SAMPLE_COMM_VO_StopDev(VoDev);
END_1080P_1:
    s32Ret = HI_MPI_VI_DisableChn(ExtChn);
    SAMPLE_COMM_VI_StopVi(pstViConfig);
END_1080P_0:
    SAMPLE_COMM_SYS_Exit();

    return s32Ret;
}

/******************************************************************************************************
* function :  VI: DC(720P); VO: SD0(CVBS). Embeded isp,  linear/wdr change, phychn channel preview.
******************************************************************************************************/
HI_S32 SAMPLE_VIO_720P_SensorModeChange_Preview(SAMPLE_VI_CONFIG_S* pstViConfig)
{
    VB_CONF_S stVbConf;
    VO_DEV VoDev = SAMPLE_VO_DEV_DSD0;;
    VO_CHN VoChn = 0;
    VI_CHN ViChn = 0;
    VO_PUB_ATTR_S stVoPubAttr;
    SAMPLE_VO_MODE_E enVoMode = VO_MODE_1MUX;
    PIC_SIZE_E enPicSize = g_enPicSize;

    HI_S32 s32Ret = HI_SUCCESS;
    HI_S32 s32GetC;
    HI_U8 u8Mode;
    HI_U32 u32BlkSize;
    SIZE_S stSize;
    ROTATE_E enRotate = ROTATE_NONE;

    VO_CSC_S stCSC = {0};

    /******************************************
     step  1: init global  variable
    ******************************************/
    gs_u32ViFrmRate = (VIDEO_ENCODING_MODE_PAL == gs_enNorm) ? 25 : 30;
    memset(&stVbConf, 0, sizeof(VB_CONF_S));

    u32BlkSize = SAMPLE_COMM_SYS_CalcPicVbBlkSize(gs_enNorm, enPicSize,
                 SAMPLE_PIXEL_FORMAT, SAMPLE_SYS_ALIGN_WIDTH);
    stVbConf.u32MaxPoolCnt = 128;

    /*ddr0 video buffer*/
    stVbConf.astCommPool[0].u32BlkSize = u32BlkSize;
    stVbConf.astCommPool[0].u32BlkCnt = 5;

    /******************************************
     step 2: start vpss and vi bind vpss (subchn needn't bind vpss in this mode)
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_GetPicSize(gs_enNorm, enPicSize, &stSize);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_SYS_GetPicSize failed!\n");
        goto END_1080P_1;
    }

    if (pstViConfig)
    {
        enRotate = pstViConfig->enRotate;
        if (ROTATE_90 == enRotate || ROTATE_270 == enRotate)
        {
            u32BlkSize = (CEILING_2_POWER(stSize.u32Width, SAMPLE_SYS_ALIGN_WIDTH) * \
                          CEILING_2_POWER(stSize.u32Height, SAMPLE_SYS_ALIGN_WIDTH) * \
                          ((PIXEL_FORMAT_YUV_SEMIPLANAR_422 == SAMPLE_PIXEL_FORMAT) ? 2 : 1.5));
            stVbConf.astCommPool[1].u32BlkSize = u32BlkSize;
            stVbConf.astCommPool[1].u32BlkCnt = 0;
        }
    }

    /******************************************
     step 3: mpp system init.
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("system init failed with %d!\n", s32Ret);
        goto END_1080P_0;
    }

    /******************************************
     step 4: start vi dev & chn to capture
    ******************************************/
    s32Ret = SAMPLE_COMM_VI_StartVi(pstViConfig);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start vi failed!\n");
        goto END_1080P_0;
    }

    /******************************************
    step 5: start VO SD0 (bind * vi )
    ******************************************/
    stVoPubAttr.enIntfType = g_enVoIntfType;
#ifndef HI_FPGA
    if (VO_INTF_BT656 == g_enVoIntfType)
    {
        stVoPubAttr.enIntfSync = VO_OUTPUT_PAL;
        gs_u32ViFrmRate = 50;
    }
#else
    if (VO_INTF_BT1120 == g_enVoIntfType)
    {
        stVoPubAttr.enIntfSync = VO_OUTPUT_1080P30;
        gs_u32ViFrmRate = 50;
    }
#endif

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

    stVoPubAttr.u32BgColor = 0x000000ff;

    /* In HD, this item should be set to HI_FALSE */
    s32Ret = SAMPLE_COMM_VO_StartDev(VoDev, &stVoPubAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartDevLayer failed!\n");
        goto END_1080P_1;
    }

    s32Ret = SAMPLE_COMM_VO_StartChn(VoDev, VO_MODE_1MUX);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartChn failed!\n");
        goto END_1080P_2;
    }

    s32Ret = SAMPLE_COMM_VO_BindVi(VoDev, VoChn, ViChn);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_BindVi(vo:%d)-(vichn:%d) failed with %#x!\n", VoDev, VoChn, s32Ret);
        goto END_1080P_3;
    }

    printf("---------quit(q)/linear(l)/wdr(w)---------\n");
    while ((s32GetC = (char)getchar()) != 'q')
    {
        if ('l' == s32GetC)
        {
            u8Mode = 0;
        }
        else if ('w' == s32GetC)
        {
            u8Mode = 1;
        }
        else
        {
            continue;
        }

        s32Ret = SAMPLE_COMM_ISP_ChangeSensorMode(u8Mode);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_PRT("SAMPLE_COMM_ISP_ChangeSensorMode failed!\n");
            break;
        }
    }


    /******************************************
     step 6: exit process
    ******************************************/
END_1080P_3:
    SAMPLE_COMM_VO_UnBindVi(VoDev, VoChn);
    SAMPLE_COMM_VO_StopChn(VoDev, enVoMode);
END_1080P_2:
    SAMPLE_COMM_VO_StopDev(VoDev);
END_1080P_1:
    SAMPLE_COMM_VI_StopVi(pstViConfig);
END_1080P_0:
    SAMPLE_COMM_SYS_Exit();

    return s32Ret;
}

HI_S32 SAMLE_VIO_ResolutionSwitch_Preview(SAMPLE_VI_CONFIG_S* pstViConfig)
{
    VB_CONF_S stVbConf;
    VO_DEV VoDev = SAMPLE_VO_DEV_DSD0;;
    VO_CHN VoChn = 0;
    VI_DEV ViDev = 0;
    VI_CHN ViChn = 0;
    ISP_DEV IspDev = 0;
    VO_PUB_ATTR_S stVoPubAttr;
    ISP_INNER_STATE_INFO_S stInnerStateInfo;

    SIZE_S stTargetSize;
    RECT_S stCapRect;

    SAMPLE_VO_MODE_E enVoMode = VO_MODE_1MUX;
    PIC_SIZE_E enPicSize = g_enPicSize;
    VO_LAYER VoLayer = 0;
    VO_VIDEO_LAYER_ATTR_S stLayerAttr;
    ISP_PUB_ATTR_S stPubAttr;

    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32BlkSize;

    VO_CSC_S stCSC = {0};

    if ( SONY_IMX222_DC_1080P_30FPS == pstViConfig->enViMode
         || SONY_IMX222_DC_720P_30FPS == pstViConfig->enViMode )
    {
        enPicSize = PIC_HD1080;
    }
    else
    {
        SAMPLE_PRT("Not support this sensor type!\n");
        goto END_1080P;
    }

    /******************************************
     step  1: init global  variable
    ******************************************/
    gs_u32ViFrmRate = (VIDEO_ENCODING_MODE_PAL == gs_enNorm) ? 25 : 30;
    memset(&stVbConf, 0, sizeof(VB_CONF_S));

    u32BlkSize = SAMPLE_COMM_SYS_CalcPicVbBlkSize(gs_enNorm, enPicSize,
                 SAMPLE_PIXEL_FORMAT, SAMPLE_SYS_ALIGN_WIDTH);
    stVbConf.u32MaxPoolCnt = 128;

    /*ddr0 video buffer*/
    stVbConf.astCommPool[0].u32BlkSize = u32BlkSize;
    stVbConf.astCommPool[0].u32BlkCnt = 5;

    /******************************************
     step 3: mpp system init.
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("system init failed with %d!\n", s32Ret);
        goto END_1080P_0;
    }

    /******************************************
     step 4: start vi dev & chn to capture
    ******************************************/
    s32Ret = SAMPLE_COMM_VI_StartVi(pstViConfig);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start vi failed!\n");
        goto END_1080P_0;
    }

    /******************************************
    step 5: start VO SD0 (bind * vi )
    ******************************************/
    stVoPubAttr.enIntfType = g_enVoIntfType;

    if (VO_INTF_BT656 == g_enVoIntfType)
    {
        stVoPubAttr.enIntfSync = VO_OUTPUT_PAL;
        gs_u32ViFrmRate = 50;
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

    stVoPubAttr.u32BgColor = 0x000000ff;

    /* In HD, this item should be set to HI_FALSE */
    s32Ret = SAMPLE_COMM_VO_StartDev(VoDev, &stVoPubAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartDev failed!\n");
        goto END_1080P_1;
    }

    stLayerAttr.bClusterMode = HI_FALSE;
    stLayerAttr.bDoubleFrame = HI_FALSE;
    stLayerAttr.enPixFormat = PIXEL_FORMAT_YUV_SEMIPLANAR_420;

    stLayerAttr.stDispRect.s32X = 0;
    stLayerAttr.stDispRect.s32Y = 0;

    s32Ret = SAMPLE_COMM_VO_GetWH(stVoPubAttr.enIntfSync,
                                  &stLayerAttr.stDispRect.u32Width, &stLayerAttr.stDispRect.u32Height,
                                  &stLayerAttr.u32DispFrmRt);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_GetWH failed!\n");
        goto END_1080P_1;
    }

    stLayerAttr.stImageSize.u32Width  = stLayerAttr.stDispRect.u32Width;
    stLayerAttr.stImageSize.u32Height = stLayerAttr.stDispRect.u32Height;

    s32Ret = SAMPLE_COMM_VO_StartLayer(VoLayer, &stLayerAttr, HI_TRUE);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartChn failed!\n");
        goto END_1080P_2;
    }
    s32Ret = SAMPLE_COMM_VO_StartChn(VoDev, VO_MODE_1MUX);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartChn failed!\n");
        goto END_1080P_3;
    }

    //s32Ret = SAMPLE_COMM_VO_BindVpss(VoDev, VoChn, VpssGrp, VpssChn);
    s32Ret = SAMPLE_COMM_VO_BindVi(VoDev, VoChn, ViChn);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_BindVpss(vo:%d)-(VpssChn:%d) failed with %#x!\n", VoDev, VoChn, s32Ret);
        goto END_1080P_4;
    }

    VI_PAUSE();

    s32Ret = HI_MPI_ISP_GetPubAttr(IspDev, &stPubAttr);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        goto END_1080P_4;
    }

    SAMPLE_COMM_VI_SwitchResParam(pstViConfig, &stPubAttr, &stCapRect);

    s32Ret = HI_MPI_VI_DisableChn(ViChn);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        goto END_1080P_4;
    }
    s32Ret = HI_MPI_VI_DisableDev(ViDev);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        goto END_1080P_4;
    }
    s32Ret = SAMPLE_COMM_VI_StartMIPI(pstViConfig);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        goto END_1080P_4;
    }

    s32Ret = HI_MPI_ISP_SetPubAttr(IspDev, &stPubAttr);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        goto END_1080P_4;
    }
    while (1)
    {
        HI_MPI_ISP_QueryInnerStateInfo(IspDev, &stInnerStateInfo);
        if (HI_TRUE == stInnerStateInfo.bResSwitchFinish)
        {
            SAMPLE_PRT("Res switch finish!\n");
            break;
        }
        usleep(1000);
    }

    s32Ret = SAMPLE_COMM_VI_StartDev(ViDev, pstViConfig->enViMode);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        goto END_1080P_4;
    }

    stTargetSize.u32Width = stCapRect.u32Width;
    stTargetSize.u32Height = stCapRect.u32Height;
    s32Ret = SAMPLE_COMM_VI_StartChn(ViChn, &stCapRect, &stTargetSize, pstViConfig);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        goto END_1080P_4;
    }


    VI_PAUSE();
    s32Ret = HI_MPI_ISP_GetPubAttr(IspDev, &stPubAttr);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        goto END_1080P_4;
    }

    SAMPLE_COMM_VI_SwitchResParam(pstViConfig, &stPubAttr, &stCapRect);

    s32Ret = HI_MPI_VI_DisableChn(ViChn);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        goto END_1080P_4;
    }
    s32Ret = HI_MPI_VI_DisableDev(ViDev);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        goto END_1080P_4;
    }
    s32Ret = SAMPLE_COMM_VI_StartMIPI(pstViConfig);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        goto END_1080P_4;
    }
    s32Ret = HI_MPI_ISP_SetPubAttr(IspDev, &stPubAttr);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        goto END_1080P_4;
    }

    while (1)
    {
        HI_MPI_ISP_QueryInnerStateInfo(IspDev, &stInnerStateInfo);
        if (HI_TRUE == stInnerStateInfo.bResSwitchFinish)
        {
            SAMPLE_PRT("Res switch finish!\n");
            break;
        }
        usleep(1000);
    }

    s32Ret = SAMPLE_COMM_VI_StartDev(ViDev, pstViConfig->enViMode);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        goto END_1080P_4;
    }

    stTargetSize.u32Width = stCapRect.u32Width;
    stTargetSize.u32Height = stCapRect.u32Height;
    s32Ret = SAMPLE_COMM_VI_StartChn(ViChn, &stCapRect, &stTargetSize, pstViConfig);
    if (s32Ret != HI_SUCCESS)
    {
        SAMPLE_PRT("failed with %#x!\n", s32Ret);
        goto END_1080P_4;
    }
    VI_PAUSE();

    /******************************************
     step 8: exit process
    ******************************************/
END_1080P_4:
    SAMPLE_COMM_VO_UnBindVi(VoDev, VoChn);
    SAMPLE_COMM_VO_StopChn(VoDev, enVoMode);
END_1080P_3:
    SAMPLE_COMM_VO_StopLayer(VoLayer);
END_1080P_2:
    SAMPLE_COMM_VO_StopDev(VoDev);
END_1080P_1:
    SAMPLE_COMM_VI_StopVi(pstViConfig);
END_1080P_0:
    SAMPLE_COMM_SYS_Exit();
END_1080P:
    return s32Ret;
}

/******************************************************************************
* function : vi/vpss: online mode VI: DC(1080P);VPSS(LDC) VO: SD0(CVBS). Embeded isp, phychn channel preview.
******************************************************************************/
HI_S32 SAMPLE_VI_VPSS_LDC_VO_1080P_PreView(SAMPLE_VI_CONFIG_S* pstViConfig)
{
    VB_CONF_S stVbConf;
    VO_DEV VoDev = SAMPLE_VO_DEV_DSD0;;
    VO_CHN VoChn = 0;
    VO_PUB_ATTR_S stVoPubAttr;
    SAMPLE_VO_MODE_E enVoMode = VO_MODE_1MUX;
    PIC_SIZE_E enPicSize = g_enPicSize;
    VO_LAYER VoLayer = 0;
    VO_VIDEO_LAYER_ATTR_S stLayerAttr;
    VPSS_GRP VpssGrp = 0;
    VPSS_CHN VpssChn = 0;
    VPSS_GRP_ATTR_S stVpssGrpAttr;
    VPSS_CHN_ATTR_S stVpssChnAttr;
    VPSS_CHN_MODE_S stVpssChnMode;
    VPSS_EXT_CHN_ATTR_S stVpssExtChnAttr;

    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32BlkSize;
    SIZE_S stSize;
    VPSS_LDC_ATTR_S stLDCAttr;

    VO_CSC_S stCSC = {0};

    /******************************************
     step  1: init global  variable
    ******************************************/
    gs_u32ViFrmRate = (VIDEO_ENCODING_MODE_PAL == gs_enNorm) ? 25 : 30;
    memset(&stVbConf, 0, sizeof(VB_CONF_S));

    u32BlkSize = SAMPLE_COMM_SYS_CalcPicVbBlkSize(gs_enNorm, enPicSize,
                 SAMPLE_PIXEL_FORMAT, SAMPLE_SYS_ALIGN_WIDTH);
    stVbConf.u32MaxPoolCnt = 128;

    /*ddr0 video buffer*/
    stVbConf.astCommPool[0].u32BlkSize = u32BlkSize;
    stVbConf.astCommPool[0].u32BlkCnt = VB_CNT;

    /******************************************
     step 2: start vpss and vi bind vpss (subchn needn't bind vpss in this mode)
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_GetPicSize(gs_enNorm, enPicSize, &stSize);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_SYS_GetPicSize failed!\n");
        goto END_1080P_0;
    }

    /******************************************
     step 3: mpp system init.
    ******************************************/
    s32Ret = SAMPLE_COMM_SYS_Init(&stVbConf);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("system init failed with %d!\n", s32Ret);
        goto END_1080P_0;
    }

    /******************************************
     step 4: start vi dev & chn to capture
    ******************************************/
    s32Ret = SAMPLE_COMM_VI_StartVi(pstViConfig);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start vi failed!\n");
        goto END_1080P_0;
    }

#if 0
    stViHLDCAttr.bEnable = HI_TRUE;
    stViHLDCAttr.stAttr.enViewType = LDC_VIEW_TYPE_ALL;
    stViHLDCAttr.stAttr.s32CenterXOffset = 0;
    stViHLDCAttr.stAttr.s32CenterYOffset = 0;
    stViHLDCAttr.stAttr.s32Ratio = 150;

    s32Ret =  HI_MPI_VI_SetLDCAttr(ViChn, &stViHLDCAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("set LDC failed!\n");
        goto END_1080P_1;
    }
#endif


    stVpssGrpAttr.bDciEn    = HI_FALSE;
    stVpssGrpAttr.bHistEn   = HI_FALSE;
    stVpssGrpAttr.bIeEn     = HI_FALSE;
    stVpssGrpAttr.bNrEn     = HI_TRUE;
    stVpssGrpAttr.enDieMode = VPSS_DIE_MODE_NODIE;
    stVpssGrpAttr.enPixFmt  = PIXEL_FORMAT_YUV_SEMIPLANAR_420;
    stVpssGrpAttr.u32MaxW   = stSize.u32Width;
    stVpssGrpAttr.u32MaxH   = stSize.u32Height;

    /******************************************
     step 5: start vpss group
    ******************************************/
    s32Ret = SAMPLE_COMM_VPSS_StartGroup(VpssGrp, &stVpssGrpAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start VPSS GROUP failed!\n");
        goto END_1080P_1;
    }

    stVpssChnAttr.bBorderEn       = 0;
    stVpssChnAttr.bFlip           = 0;
    stVpssChnAttr.bMirror         = 0;
    stVpssChnAttr.bSpEn           = 0 ;
    stVpssChnAttr.s32DstFrameRate = -1;
    stVpssChnAttr.s32SrcFrameRate = -1;
    stVpssChnMode.bDouble         = HI_FALSE;
    stVpssChnMode.enChnMode       = VPSS_CHN_MODE_USER;
    stVpssChnMode.enCompressMode  = COMPRESS_MODE_NONE;
    stVpssChnMode.enPixelFormat   = PIXEL_FORMAT_YUV_SEMIPLANAR_420;
    stVpssChnMode.u32Width        = stSize.u32Width;
    stVpssChnMode.u32Height       = stSize.u32Height;

    stLDCAttr.bEnable                 = HI_TRUE;
    stLDCAttr.stAttr.enViewType       = LDC_VIEW_TYPE_ALL;
    stLDCAttr.stAttr.s32CenterXOffset = 0;
    stLDCAttr.stAttr.s32CenterYOffset = 0;
    stLDCAttr.stAttr.s32Ratio         = 100;

    s32Ret =  HI_MPI_VPSS_SetLDCAttr(VpssGrp, VpssChn,  &stLDCAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("set LDC failed!\n");
        goto END_1080P_1;
    }

    /******************************************
    step 6: start vpss chn
    ******************************************/
    s32Ret = SAMPLE_COMM_VPSS_EnableChn(VpssGrp, VpssChn, &stVpssChnAttr, &stVpssChnMode, &stVpssExtChnAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("start VPSS CHN failed!\n");
        goto END_1080P_1;
    }

    /******************************************
    step 7: start VO SD0 (bind * vi )
    ******************************************/
    stVoPubAttr.enIntfType = g_enVoIntfType;

    if (VO_INTF_BT656 == g_enVoIntfType)
    {
        stVoPubAttr.enIntfSync = VO_OUTPUT_PAL;
        gs_u32ViFrmRate = 50;
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

    stVoPubAttr.u32BgColor = 0x000000ff;

    /* In HD, this item should be set to HI_FALSE */
    s32Ret = SAMPLE_COMM_VO_StartDev(VoDev, &stVoPubAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartDev failed!\n");
        goto END_1080P_1;
    }

    stLayerAttr.bClusterMode = HI_FALSE;
    stLayerAttr.bDoubleFrame = HI_FALSE;
    stLayerAttr.enPixFormat = PIXEL_FORMAT_YUV_SEMIPLANAR_420;

    stLayerAttr.stDispRect.s32X = 0;
    stLayerAttr.stDispRect.s32Y = 0;
    s32Ret = SAMPLE_COMM_VO_GetWH(stVoPubAttr.enIntfSync,
                                  &stLayerAttr.stDispRect.u32Width, &stLayerAttr.stDispRect.u32Height,
                                  &stLayerAttr.u32DispFrmRt);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_GetWH failed!\n");
        goto END_1080P_1;
    }

    stLayerAttr.stImageSize.u32Width  = stLayerAttr.stDispRect.u32Width;
    stLayerAttr.stImageSize.u32Height = stLayerAttr.stDispRect.u32Height;

    s32Ret = SAMPLE_COMM_VO_StartLayer(VoLayer, &stLayerAttr, HI_FALSE);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartChn failed!\n");
        goto END_1080P_2;
    }


    s32Ret = SAMPLE_COMM_VO_StartChn(VoDev, VO_MODE_1MUX);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_StartChn failed!\n");
        goto END_1080P_3;
    }

    s32Ret = SAMPLE_COMM_VO_BindVpss(VoDev, VoChn, VpssGrp, VpssChn);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("SAMPLE_COMM_VO_BindVpss(vo:%d)-(VpssChn:%d) failed with %#x!\n", VoDev, VoChn, s32Ret);
        goto END_1080P_4;
    }

    printf("\nplease hit the Enter key, disable LDC\n\n");
    VI_PAUSE();
    stLDCAttr.bEnable                 = HI_FALSE;
    stLDCAttr.stAttr.enViewType       = LDC_VIEW_TYPE_ALL;
    stLDCAttr.stAttr.s32CenterXOffset = 0;
    stLDCAttr.stAttr.s32CenterYOffset = 0;
    stLDCAttr.stAttr.s32Ratio         = 100;

    s32Ret =  HI_MPI_VPSS_SetLDCAttr(VpssGrp, VpssChn,  &stLDCAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("set LDC failed!\n");
        goto END_1080P_5;
    }

    printf("\nplease hit the Enter key, enable LDC\n\n");
    VI_PAUSE();
    stLDCAttr.bEnable                 = HI_TRUE;
    stLDCAttr.stAttr.enViewType       = LDC_VIEW_TYPE_ALL;
    stLDCAttr.stAttr.s32CenterXOffset = 0;
    stLDCAttr.stAttr.s32CenterYOffset = 0;
    stLDCAttr.stAttr.s32Ratio         = 100;

    s32Ret =  HI_MPI_VPSS_SetLDCAttr(VpssGrp, VpssChn,  &stLDCAttr);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_PRT("set LDC failed!\n");
        goto END_1080P_5;
    }
    VI_PAUSE();

    /******************************************
     step 8: exit process
    ******************************************/
END_1080P_5:
    SAMPLE_COMM_VO_UnBindVpss(VoDev, VoChn, VpssGrp, VpssChn);
    SAMPLE_COMM_VO_StopChn(VoDev, enVoMode);
END_1080P_4:
    SAMPLE_COMM_VO_StopLayer(VoLayer);
END_1080P_3:
    SAMPLE_COMM_VO_StopDev(VoDev);
END_1080P_2:
    SAMPLE_COMM_VPSS_DisableChn(VpssGrp, VpssChn);
    SAMPLE_COMM_VPSS_StopGroup(VpssGrp);
END_1080P_1:
    SAMPLE_COMM_VI_StopVi(pstViConfig);
END_1080P_0:
    SAMPLE_COMM_SYS_Exit();
    return s32Ret;

}


HI_S32 SAMPLE_VIO_SharpenTest()
{
    HI_S32 s32Ret = HI_SUCCESS;
    ISP_DEV IspDev = 0;
    HI_U32 i;
    ISP_SHARPEN_ATTR_S stSharpenAttr0;
    ISP_SHARPEN_ATTR_S stSharpenAttr255;
    HI_MPI_ISP_GetSharpenAttr(IspDev, &stSharpenAttr0);
    HI_MPI_ISP_GetSharpenAttr(IspDev, &stSharpenAttr255);

    stSharpenAttr0.bEnable = HI_TRUE;
    stSharpenAttr0.stAuto.au8SharpenD[0] = 0;

    stSharpenAttr255.bEnable = HI_TRUE;
    stSharpenAttr255.stAuto.au8SharpenD[0] = 255;

    for(i=0; i<100000;i++)
    {
        if(i%2)
        {
            printf("stSharpenAttr0.stAuto.au8SharpenD[0] = %d\n", stSharpenAttr0.stAuto.au8SharpenD[0]);
            HI_MPI_ISP_SetSharpenAttr(IspDev, &stSharpenAttr0);
        }
        else
        {
            printf("stSharpenAttr255.stAuto.au8SharpenD[0] = %d\n", stSharpenAttr255.stAuto.au8SharpenD[0]);
            HI_MPI_ISP_SetSharpenAttr(IspDev, &stSharpenAttr255);
        }
        sleep(5);
    }
    
    return s32Ret;
}


HI_S32 SAMPLE_VIO_Start16CV200(SAMPLE_VI_CONFIG_S *pstViConfig, char *pchPrgNm)
{
    HI_S32 s32Ret = HI_SUCCESS;
    
    switch (*pchPrgNm)
    {
            /* VI: DC(720P); VO: SD0(BT656). Embeded isp, phychn channel preview. */
        case '0':
            s32Ret = SAMPLE_VIO_PreView(&g_stViChnConfig);
            break;

        case '1':
            s32Ret = SAMPLE_VIO_FRAME_FPN_PreView(&g_stViChnConfig);
            break;

        case '2':
            s32Ret = SAMPLE_VI_VPSS_ROTATE_VO_PreView(&g_stViChnConfig);
            break;
			
        case '3':
            s32Ret = SAMPLE_VIO_RGBIR_PreView(&g_stViChnConfig);
            break;

        case '4':
            s32Ret = SAMPLE_VI_VPSS_LDC_VO_1080P_PreView(&g_stViChnConfig);
            break;
            
        case '5':
            g_stViChnConfig.enRotate = ROTATE_90;
            s32Ret = SAMPLE_VI_ROTATE_VO_PreView(&g_stViChnConfig);
            break;
			
        case '6':
            s32Ret = SAMPLE_VI_LDC_VO_PreView(&g_stViChnConfig);
            break;
			
        case '7':
            s32Ret = SAMPLE_VI_DIS_VPSS_VO_PreView(&g_stViChnConfig);
            break;

            /* VI: BT1120(1080I); VO:HD0(HDMI).  Isp bypass, phychn channel preview. */
        case '8':
            g_stViChnConfig.enViMode = SAMPLE_VI_MODE_BT1120_1080P;
            g_stViChnConfig.enNorm = VIDEO_ENCODING_MODE_PAL;
            s32Ret = SAMPLE_VIO_BT1120_PreView(&g_stViChnConfig);
            break;

        case '9':
            s32Ret = SAMLE_VIO_ResolutionSwitch_Preview(&g_stViChnConfig);
            break;
            
        default:
            SAMPLE_PRT("the index is invaild!\n");
            SAMPLE_VIO_Usage(pchPrgNm);
            return HI_FAILURE;
    }
    return s32Ret;
}

HI_S32 SAMPLE_VIO_Start18EV200(SAMPLE_VI_CONFIG_S *pstViConfig, char *pchPrgNm)
{
    HI_S32 s32Ret = HI_SUCCESS;
    
    switch (*pchPrgNm)
    {
            /* VI: DC(720P); VO: SD0(CVBS). Embeded isp, phychn channel preview. */
        case '0':
            s32Ret = SAMPLE_VIO_PreView(&g_stViChnConfig);
            break;

        case '1':
            s32Ret = SAMPLE_VIO_FRAME_FPN_PreView(&g_stViChnConfig);
            break;

        case '2':
            s32Ret = SAMPLE_VI_VPSS_ROTATE_VO_PreView(&g_stViChnConfig);
            break;

        case '3':
            s32Ret = SAMPLE_VIO_RGBIR_PreView(&g_stViChnConfig);
            break;

        case '4':
            s32Ret = SAMPLE_VI_VPSS_LDC_VO_1080P_PreView(&g_stViChnConfig);
            break;
            
        case '5':
            g_stViChnConfig.enRotate = ROTATE_90;
            s32Ret = SAMPLE_VI_ROTATE_VO_PreView(&g_stViChnConfig);
            break;
            
        case '6':
            s32Ret = SAMPLE_VI_LDC_VO_PreView(&g_stViChnConfig);
            break;
            
        case '7':
            s32Ret = SAMPLE_VI_DIS_VPSS_VO_PreView(&g_stViChnConfig);
            break;

            /* VI: BT1120(1080I); VO:HD0(HDMI).  Isp bypass, phychn channel preview. */
        case '8':
            g_stViChnConfig.enViMode = SAMPLE_VI_MODE_BT1120_1080P;
            g_stViChnConfig.enNorm = VIDEO_ENCODING_MODE_PAL;
            s32Ret = SAMPLE_VIO_BT1120_PreView(&g_stViChnConfig);
            break;
            
        case '9':
            s32Ret = SAMLE_VIO_ResolutionSwitch_Preview(&g_stViChnConfig);
            break;
            
        default:
            SAMPLE_PRT("the index is invaild!\n");
            SAMPLE_VIO_Usage(pchPrgNm);
            return HI_FAILURE;
    }
    
    return s32Ret;
}

HI_S32 SAMPLE_VIO_Start18EV201(SAMPLE_VI_CONFIG_S *pstViConfig, char *pchPrgNm)
{
    HI_S32 s32Ret = HI_SUCCESS;
    
    switch (*pchPrgNm)
    {
            /* VI: DC(720P); VO: SD0(CVBS). Embeded isp, phychn channel preview. */
        case '0':
            s32Ret = SAMPLE_VIO_PreView(&g_stViChnConfig);
            break;

        case '1':
            s32Ret = SAMPLE_VIO_FRAME_FPN_PreView(&g_stViChnConfig);
            break;

        case '2':
            s32Ret = SAMPLE_VI_VPSS_ROTATE_VO_PreView(&g_stViChnConfig);
            break;

        case '3':
            s32Ret = SAMPLE_VIO_RGBIR_PreView(&g_stViChnConfig);
            break;

        case '4':
            s32Ret = SAMPLE_VI_VPSS_LDC_VO_1080P_PreView(&g_stViChnConfig);
            break;
            
        case '5':
            g_stViChnConfig.enRotate = ROTATE_90;
            s32Ret = SAMPLE_VI_ROTATE_VO_PreView(&g_stViChnConfig);
            break;
            
        case '6':
            s32Ret = SAMPLE_VI_LDC_VO_PreView(&g_stViChnConfig);
            break;
            
        case '7':
            s32Ret = SAMPLE_VI_DIS_VPSS_VO_PreView(&g_stViChnConfig);
            break;

            /* VI: BT1120(1080I); VO:HD0(HDMI).  Isp bypass, phychn channel preview. */
        case '8':
            g_stViChnConfig.enViMode = SAMPLE_VI_MODE_BT1120_720P;
            g_stViChnConfig.enNorm = VIDEO_ENCODING_MODE_PAL;
            s32Ret = SAMPLE_VIO_BT1120_PreView(&g_stViChnConfig);
            break;

        default:
            SAMPLE_PRT("the index is invaild!\n");
            SAMPLE_VIO_Usage(pchPrgNm);
            return HI_FAILURE;
    }
    
    return s32Ret;
}


/******************************************************************************
* function    : main()
* Description : video preview sample
******************************************************************************/
int main(int argc, char* argv[])
{
    HI_S32 s32Ret = HI_FAILURE;

    if ( (argc < 2) || (1 != strlen(argv[1])))
    {
        SAMPLE_VIO_Usage(argv[0]);
        return HI_FAILURE;
    }

    signal(SIGINT, SAMPLE_VIO_HandleSig);
    signal(SIGTERM, SAMPLE_VIO_HandleSig);

    if ((argc > 2) && *argv[2] == '1')  /* '1': VO_INTF_BT656, else: VO_INTF_LCD */
    {
        g_enVoIntfType = VO_INTF_BT656;
    }

    g_stViChnConfig.enViMode = SENSOR_TYPE;
    SAMPLE_COMM_VI_GetSizeBySensor(&g_enPicSize);

    switch(CHIP_ID)
    {
        case CHIP_HI3516C_V200:
            s32Ret = SAMPLE_VIO_Start16CV200(&g_stViChnConfig, argv[1]);
            break;
        case CHIP_HI3518E_V200:
            s32Ret = SAMPLE_VIO_Start18EV200(&g_stViChnConfig, argv[1]);
            break;
        case CHIP_HI3518E_V201:
            s32Ret = SAMPLE_VIO_Start18EV201(&g_stViChnConfig, argv[1]);
            break;
        default :
            SAMPLE_PRT("Invalid chip id, please check!");
            s32Ret = HI_FAILURE;
            break;
    }

    if (HI_SUCCESS == s32Ret)
    { 
        SAMPLE_PRT("program exit normally!\n"); 
    }
    else
    { 
        SAMPLE_PRT("program exit abnormally!\n"); 
    }
    
    exit(s32Ret);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

