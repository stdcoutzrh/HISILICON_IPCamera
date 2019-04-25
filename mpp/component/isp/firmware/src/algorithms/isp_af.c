/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_af.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/01/05
  Description   : 
  History       :
  1.Date        : 2013/01/05
    Author      : n00168968
    Modification: Created file

******************************************************************************/

#include <stdio.h>
#include "isp_alg.h"
#include "isp_ext_config.h"
#include "isp_config_ex.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#define BAYER_MODE	(1)	/* 0: rggb/bggr	1: grbg/rbrg*/
#define RAW_MODE	(1) /* 0: yuv		1: raw */

static HI_VOID AfRegsDefault(HI_VOID)
{
	HI_U8 u8Rggb = 0;

	hi_isp_af_lpf_en_write(HI_TRUE);
	hi_isp_af_median_en_write(HI_TRUE);
	hi_isp_af_gamma_en_write(HI_TRUE);
	hi_isp_af_crop_en_write(HI_FALSE);
	//hi_isp_af_bayer_mode_write(BAYER_MODE);
	hi_isp_af_raw_mode_write(RAW_MODE);
	hi_isp_af_data_mode_write(0x0);
	hi_isp_af_median_thres_write(25600);
	hi_isp_af_peak_mode_write(0x1);
	hi_isp_af_squ_mode_write(0x1);
	hi_isp_af_hnum_write(AF_ZONE_COLUMN);
	hi_isp_af_vnum_write(AF_ZONE_ROW);

	u8Rggb = hi_isp_yuv422_rggb_start_read();
	if ((u8Rggb == BAYER_RGGB)||(u8Rggb == BAYER_BGGR))
	{
	    hi_isp_af_bayer_mode_write(0);
	}
	else if ((u8Rggb == BAYER_GRBG)||(u8Rggb == BAYER_GBRG))
	{
	    hi_isp_af_bayer_mode_write(1);
	}
	
	hi_isp_af_enable_write(HI_TRUE);
    return;
}

HI_S32 ISP_AfInit(ISP_DEV IspDev)
{
    HI_S32 i;
    ISP_AF_PARAM_S stAfParam;
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_LIB_NODE_S *pstLib = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

	AfRegsDefault();
    
    stAfParam.SensorId = pstIspCtx->stBindAttr.SensorId;
    for (i=0; i<MAX_REGISTER_ALG_LIB_NUM; i++)
    {
        if (pstIspCtx->stAfLibInfo.astLibs[i].bUsed)
        {
            pstLib = &pstIspCtx->stAfLibInfo.astLibs[i];
            if (HI_NULL != pstLib->stAfRegsiter.stAfExpFunc.pfn_af_init)
            {
                pstLib->stAfRegsiter.stAfExpFunc.pfn_af_init(
                    pstLib->stAlgLib.s32Id, &stAfParam);
            }
        }
    }
    
    return HI_SUCCESS;
}

HI_S32 ISP_AfRun(ISP_DEV IspDev, const HI_VOID *pStatInfo,
    HI_VOID *pRegCfg, HI_S32 s32Rsv)
{
    HI_S32 s32Ret = HI_FAILURE;
    ISP_AF_INFO_S       stAfInfo = {0};
    ISP_AF_RESULT_S     stAfResult = {0};
    ISP_CTX_S          *pstIspCtx   = HI_NULL;
    ISP_LIB_NODE_S     *pstLib      = HI_NULL;
    ISP_AF_STATISTICS_POS_E enPos;
    
    ISP_GET_CTX(IspDev, pstIspCtx);
    pstLib = &pstIspCtx->stAfLibInfo.astLibs[pstIspCtx->stAfLibInfo.u32ActiveLib];

    if (pstIspCtx->stLinkage.bDefectPixel)
    {
        return HI_SUCCESS;
    }

    stAfInfo.u32FrameCnt = pstIspCtx->u32FrameCnt;

    stAfInfo.stAfStat = &((ISP_STAT_S *)pStatInfo)->stAfStat;
    
    if (HI_NULL != pstLib->stAfRegsiter.stAfExpFunc.pfn_af_run)
    {
        s32Ret = pstLib->stAfRegsiter.stAfExpFunc.pfn_af_run(
            pstLib->stAlgLib.s32Id, &stAfInfo, &stAfResult, 0);
        if (HI_SUCCESS != s32Ret)
        {
            printf("WARNING!! run af lib err 0x%x!\n", s32Ret);
        }
    }
    /* some paramers shoud be changed synchronous */
    enPos = hi_isp_af_raw_mode_read();

    if (ISP_AF_STATISTICS_YUV == enPos )
    {
        hi_vi_af_pos_write(0x1);
        hi_isp_af_gamma_en_write(HI_FALSE);
        
    }
    else
    {
        hi_vi_af_pos_write(0x0);
        /* set sqrt bit as enable since chose bayer domain */ 
        hi_isp_af_gamma_en_write(HI_TRUE);
    }

    return s32Ret;
}

HI_S32 ISP_AfCtrl(ISP_DEV IspDev, HI_U32 u32Cmd, HI_VOID *pValue)
{    
    HI_S32  i, s32Ret = HI_FAILURE;
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_LIB_NODE_S *pstLib = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    for (i=0; i<MAX_REGISTER_ALG_LIB_NUM; i++)
    {
        if (pstIspCtx->stAfLibInfo.astLibs[i].bUsed)
        {
            pstLib = &pstIspCtx->stAfLibInfo.astLibs[i];
            if (HI_NULL != pstLib->stAfRegsiter.stAfExpFunc.pfn_af_ctrl)
            {
                s32Ret = pstLib->stAfRegsiter.stAfExpFunc.pfn_af_ctrl(
                    pstLib->stAlgLib.s32Id, u32Cmd, pValue);
            }
        }
    }

    return s32Ret;
}

HI_S32 ISP_AfExit(ISP_DEV IspDev)
{
    HI_S32 i;
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_LIB_NODE_S *pstLib = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    for (i=0; i<MAX_REGISTER_ALG_LIB_NUM; i++)
    {
        if (pstIspCtx->stAfLibInfo.astLibs[i].bUsed)
        {
            pstLib = &pstIspCtx->stAfLibInfo.astLibs[i];
            if (HI_NULL != pstLib->stAfRegsiter.stAfExpFunc.pfn_af_exit)
            {
                pstLib->stAfRegsiter.stAfExpFunc.pfn_af_exit(
                    pstLib->stAlgLib.s32Id);
            }
        }
    }

    return HI_SUCCESS;
}

HI_S32 ISP_AlgRegisterAf(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_ALG_NODE_S *pstAlgs = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->enAlgType = ISP_ALG_AF;
    pstAlgs->stAlgFunc.pfn_alg_init = ISP_AfInit;
    pstAlgs->stAlgFunc.pfn_alg_run  = ISP_AfRun;
    pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_AfCtrl;
    pstAlgs->stAlgFunc.pfn_alg_exit = ISP_AfExit;
    pstAlgs->bUsed = HI_TRUE;

    return HI_SUCCESS;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

