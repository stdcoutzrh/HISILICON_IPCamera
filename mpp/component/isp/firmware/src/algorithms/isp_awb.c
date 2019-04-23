/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_awb.c
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
#include "isp_config.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#ifndef MIN
#define MIN(a, b) (((a) > (b)) ?  (b) : (a))
#endif

static HI_VOID AwbRegsDefault(HI_VOID)
{
    /*Enable AWB, WB and CCM*/
    hi_isp_awb_cfg_enable_write(1);
    hi_isp_wb_enable_write(1);
    hi_isp_cc_enable_write(1);

    /* configure awb parameters */    
    hi_isp_metering_awb_nodes_used_horiz_write(HI_ISP_METERING_AWB_NODES_USED_HORIZ_DEFAULT);
    hi_isp_metering_awb_nodes_used_vert_write(HI_ISP_METERING_AWB_NODES_USED_VERT_DEFAULT);
    
    hi_isp_metering_max_threshold_write(0xFFF);
    hi_isp_metering_min_threshold_write(0x0);
    hi_isp_metering_cr_ref_max_awb_sum_write(0xFFF);
    hi_isp_metering_cr_ref_min_awb_sum_write(0x0);
    hi_isp_metering_cb_ref_max_awb_sum_write(0xFFF);
    hi_isp_metering_cb_ref_min_awb_sum_write(0x0);
    hi_isp_awb_top_hypotenuse_k_write(0x0);
    hi_isp_awb_top_hypotenuse_b_write(0xFFF);
    hi_isp_awb_bot_hypotenuse_k_write(0x0);
    hi_isp_awb_bot_hypotenuse_b_write(0x0);

    hi_isp_white_balance_gain_00_write(0x180);
    hi_isp_white_balance_gain_01_write(0x100);
    hi_isp_white_balance_gain_10_write(0x100);
    hi_isp_white_balance_gain_11_write(0x180);
    
    hi_isp_matrix_coefft_r_r_write(0x1000);
    hi_isp_matrix_coefft_r_g_write(0x0);
    hi_isp_matrix_coefft_r_b_write(0x0);
    hi_isp_matrix_coefft_g_r_write(0x0);
    hi_isp_matrix_coefft_g_g_write(0x1000);
    hi_isp_matrix_coefft_g_b_write(0x0);
    hi_isp_matrix_coefft_b_r_write(0x0);
    hi_isp_matrix_coefft_b_g_write(0x0);
    hi_isp_matrix_coefft_b_b_write(0x1000);    
        
    /*Config dc offset for ColorCorrectionMatrxi Module*/
    hi_isp_cc_in_dc0_write(HI_ISP_CC_IN_DC0_DEFAULT);
    hi_isp_cc_in_dc1_write(HI_ISP_CC_IN_DC1_DEFAULT);
    hi_isp_cc_in_dc2_write(HI_ISP_CC_IN_DC2_DEFAULT);
    hi_isp_cc_out_dc0_write(HI_ISP_CC_OUT_DC0_DEFAULT);
    hi_isp_cc_out_dc1_write(HI_ISP_CC_OUT_DC1_DEFAULT);
    hi_isp_cc_out_dc2_write(HI_ISP_CC_OUT_DC2_DEFAULT);

    return;
}

#if 0
static HI_U16 AwbSqrt32(HI_U32 u32Arg)
{
    HI_U32 u32Mask = (HI_U32)1 << 15;
    HI_U16 u16Res  = 0;
    HI_U32 i = 0;

    for(i=0; i<16; i++)
    {
        if((u16Res + (u32Mask >> i))*(u16Res + (u32Mask >> i)) <= u32Arg)
        {
            u16Res = u16Res + (u32Mask >> i);
        }
    }

    /* rounding */
    if (u16Res * u16Res + u16Res < u32Arg)
    {
        ++u16Res;
    }
    
    return u16Res;
}
#endif

HI_S32 AwbCfgReg(ISP_AWB_RESULT_S *pstAwbResult, HI_U8 u8WDRMode,
    HI_U32 u32IspDgain, HI_U32 u32IspDgainShift, ISP_REG_CFG_S *pstRegCfg)
{
    HI_S32 i;
    HI_U32 au32WbGain[4];
    
    if (pstAwbResult->stRawStatAttr.bChange)
    {
        if (IS_LINEAR_MODE(u8WDRMode)||IS_WDR_MODE(u8WDRMode))
        {
#if 0        
            pstRegCfg->stAwbRegCfg2.u16MeteringWhiteLevelAwb = pstAwbResult->stStatAttr.u16MeteringWhiteLevelAwb;
            pstRegCfg->stAwbRegCfg2.u16MeteringBlackLevelAwb = pstAwbResult->stStatAttr.u16MeteringBlackLevelAwb;
            pstRegCfg->stAwbRegCfg2.u16MeteringCrRefMaxAwb = pstAwbResult->stStatAttr.u16MeteringCrRefMaxAwb;
            pstRegCfg->stAwbRegCfg2.u16MeteringCrRefMinAwb = pstAwbResult->stStatAttr.u16MeteringCrRefMinAwb;
            pstRegCfg->stAwbRegCfg2.u16MeteringCbRefMaxAwb = pstAwbResult->stStatAttr.u16MeteringCbRefMaxAwb;
            pstRegCfg->stAwbRegCfg2.u16MeteringCbRefMinAwb = pstAwbResult->stStatAttr.u16MeteringCbRefMinAwb;
            pstRegCfg->stAwbRegCfg2.u16MeteringCrRefHighAwb = pstAwbResult->stStatAttr.u16MeteringCrRefHighAwb;
            pstRegCfg->stAwbRegCfg2.u16MeteringCrRefLowAwb = pstAwbResult->stStatAttr.u16MeteringCrRefHighAwb;
            pstRegCfg->stAwbRegCfg2.u16MeteringCbRefHighAwb = pstAwbResult->stStatAttr.u16MeteringCbRefHighAwb;
            pstRegCfg->stAwbRegCfg2.u16MeteringCbRefLowAwb = pstAwbResult->stStatAttr.u16MeteringCbRefLowAwb;
#endif
            pstRegCfg->stAwbRegCfg3.bAboveWhiteLevelClip = pstAwbResult->stRawStatAttr.bAboveWhiteLevelClip;
            pstRegCfg->stAwbRegCfg3.bBelowBlackLevelClip = pstAwbResult->stRawStatAttr.bBelowBlackLevelClip;
            pstRegCfg->stAwbRegCfg3.u16MeteringWhiteLevelAwb = MIN(pstAwbResult->stRawStatAttr.u16MeteringWhiteLevelAwb, 0xFFF);
            pstRegCfg->stAwbRegCfg3.u16MeteringBlackLevelAwb = MIN(pstAwbResult->stRawStatAttr.u16MeteringBlackLevelAwb, 0xFFF);
            pstRegCfg->stAwbRegCfg3.u16MeteringCrRefMaxAwb = MIN(pstAwbResult->stRawStatAttr.u16MeteringCrRefMaxAwb, 0xFFF);
            pstRegCfg->stAwbRegCfg3.u16MeteringCrRefMinAwb = MIN(pstAwbResult->stRawStatAttr.u16MeteringCrRefMinAwb, 0xFFF);
            pstRegCfg->stAwbRegCfg3.u16MeteringCbRefMaxAwb = MIN(pstAwbResult->stRawStatAttr.u16MeteringCbRefMaxAwb, 0xFFF);
            pstRegCfg->stAwbRegCfg3.u16MeteringCbRefMinAwb = MIN(pstAwbResult->stRawStatAttr.u16MeteringCbRefMinAwb, 0xFFF);
#if 0            
            pstRegCfg->stAwbRegCfg3.u16MeteringCrRefHighAwb = MIN(pstAwbResult->stRawStatAttr.u16MeteringCrRefHighAwb, 0xFFF);
            pstRegCfg->stAwbRegCfg3.u16MeteringCrRefLowAwb = MIN(pstAwbResult->stRawStatAttr.u16MeteringCrRefLowAwb, 0xFFF);
            pstRegCfg->stAwbRegCfg3.u16MeteringCbRefHighAwb = MIN(pstAwbResult->stRawStatAttr.u16MeteringCbRefHighAwb, 0xFFF);
            pstRegCfg->stAwbRegCfg3.u16MeteringCbRefLowAwb = MIN(pstAwbResult->stRawStatAttr.u16MeteringCbRefLowAwb, 0xFFF);
#endif            
        }
#if 0
		else if (IS_WDR_MODE(u8WDRMode))
        {
            pstRegCfg->stAwbRegCfg2.u16MeteringWhiteLevelAwb = AwbSqrt32((HI_U32)pstAwbResult->stStatAttr.u16MeteringWhiteLevelAwb << 10);
            pstRegCfg->stAwbRegCfg2.u16MeteringBlackLevelAwb = AwbSqrt32((HI_U32)pstAwbResult->stStatAttr.u16MeteringBlackLevelAwb << 10);
            pstRegCfg->stAwbRegCfg2.u16MeteringCrRefMaxAwb = AwbSqrt32((HI_U32)pstAwbResult->stStatAttr.u16MeteringCrRefMaxAwb << 8);
            pstRegCfg->stAwbRegCfg2.u16MeteringCrRefMinAwb = AwbSqrt32((HI_U32)pstAwbResult->stStatAttr.u16MeteringCrRefMinAwb << 8);
            pstRegCfg->stAwbRegCfg2.u16MeteringCbRefMaxAwb = AwbSqrt32((HI_U32)pstAwbResult->stStatAttr.u16MeteringCbRefMaxAwb << 8);
            pstRegCfg->stAwbRegCfg2.u16MeteringCbRefMinAwb = AwbSqrt32((HI_U32)pstAwbResult->stStatAttr.u16MeteringCbRefMinAwb << 8);
            pstRegCfg->stAwbRegCfg2.u16MeteringCrRefHighAwb = AwbSqrt32((HI_U32)pstAwbResult->stStatAttr.u16MeteringCrRefHighAwb << 8);
            pstRegCfg->stAwbRegCfg2.u16MeteringCrRefLowAwb = AwbSqrt32((HI_U32)pstAwbResult->stStatAttr.u16MeteringCrRefLowAwb << 8);
            pstRegCfg->stAwbRegCfg2.u16MeteringCbRefHighAwb = AwbSqrt32((HI_U32)pstAwbResult->stStatAttr.u16MeteringCbRefHighAwb << 8);
            pstRegCfg->stAwbRegCfg2.u16MeteringCbRefLowAwb = AwbSqrt32((HI_U32)pstAwbResult->stStatAttr.u16MeteringCbRefLowAwb << 8);

            pstRegCfg->stAwbRegCfg3.bAboveWhiteLevelClip = pstAwbResult->stRawStatAttr.bAboveWhiteLevelClip;
            pstRegCfg->stAwbRegCfg3.bBelowBlackLevelClip = pstAwbResult->stRawStatAttr.bBelowBlackLevelClip;
            pstRegCfg->stAwbRegCfg3.u16MeteringWhiteLevelAwb = AwbSqrt32((HI_U32)pstAwbResult->stRawStatAttr.u16MeteringWhiteLevelAwb << 12);
            pstRegCfg->stAwbRegCfg3.u16MeteringBlackLevelAwb = pstAwbResult->stRawStatAttr.u16MeteringBlackLevelAwb;   /*offset of black level*/
            pstRegCfg->stAwbRegCfg3.u16MeteringCrRefMaxAwb = AwbSqrt32((HI_U32)pstAwbResult->stRawStatAttr.u16MeteringCrRefMaxAwb << 8);
            pstRegCfg->stAwbRegCfg3.u16MeteringCrRefMinAwb = AwbSqrt32((HI_U32)pstAwbResult->stRawStatAttr.u16MeteringCrRefMinAwb << 8);
            pstRegCfg->stAwbRegCfg3.u16MeteringCbRefMaxAwb = AwbSqrt32((HI_U32)pstAwbResult->stRawStatAttr.u16MeteringCbRefMaxAwb << 8);
            pstRegCfg->stAwbRegCfg3.u16MeteringCbRefMinAwb = AwbSqrt32((HI_U32)pstAwbResult->stRawStatAttr.u16MeteringCbRefMinAwb << 8);
            pstRegCfg->stAwbRegCfg3.u16MeteringCrRefHighAwb = AwbSqrt32((HI_U32)pstAwbResult->stRawStatAttr.u16MeteringCrRefHighAwb << 8);
            pstRegCfg->stAwbRegCfg3.u16MeteringCrRefLowAwb = AwbSqrt32((HI_U32)pstAwbResult->stRawStatAttr.u16MeteringCrRefLowAwb << 8);
            pstRegCfg->stAwbRegCfg3.u16MeteringCbRefHighAwb = AwbSqrt32((HI_U32)pstAwbResult->stRawStatAttr.u16MeteringCbRefHighAwb << 8);
            pstRegCfg->stAwbRegCfg3.u16MeteringCbRefLowAwb = AwbSqrt32((HI_U32)pstAwbResult->stRawStatAttr.u16MeteringCbRefLowAwb << 8);

        }
#endif
        else
        {
            /* unknow u8WDRMode */
        }
        //pstRegCfg->stAwbRegCfg3.u16MeteringBlackLevelAwb += u16BlackLevel;
        pstRegCfg->unKey.bit1AwbCfg2 = 1;
        pstRegCfg->unKey.bit1AwbCfg3 = 1;
    }

    for (i=0; i<9; i++)
    {
        pstRegCfg->stAwbRegCfg1.au16ColorMatrix[i] = pstAwbResult->au16ColorMatrix[i];
    }

    //if (IS_LINEAR_MODE(u8WDRMode))
    {
        for (i=0; i<4; i++)
        {
            au32WbGain[i] = pstAwbResult->au32WhiteBalanceGain[i];           
            au32WbGain[i] = (au32WbGain[i] + 128) >> 8;
            /*avoid overflow*/
            au32WbGain[i] = (au32WbGain[i] > 0xFFF) ? 0xFFF : au32WbGain[i];

            pstRegCfg->stAwbRegCfg1.au32WhiteBalanceGain[i] = (HI_U16)au32WbGain[i];
        }
    }
#if 0    
    else if (IS_WDR_MODE(u8WDRMode))
    {
        for (i=0; i<4; i++)
        {
            au32WbGain[i] = pstAwbResult->au32WhiteBalanceGain[i];
            /*avoid overflow*/
            au32WbGain[i] = (au32WbGain[i] > 0xFFE000) ? 0xFFE000 : au32WbGain[i];

            pstRegCfg->stAwbRegCfg1.au32WhiteBalanceGain[i] = AwbSqrt32(au32WbGain[i]);
        }
    }
    else
    {
        /* unknow u8WDRMode */
    }
#endif

    pstRegCfg->unKey.bit1AwbCfg1 = 1;

    return HI_SUCCESS;
}
HI_S32 ISP_AwbCtrl(ISP_DEV IspDev, HI_U32 u32Cmd, HI_VOID *pValue);
HI_S32 ISP_AwbInit(ISP_DEV IspDev)
{
    HI_S32 i;
    ISP_AWB_PARAM_S stAwbParam;
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_LIB_NODE_S *pstLib = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    AwbRegsDefault();
    
    stAwbParam.SensorId = pstIspCtx->stBindAttr.SensorId;
    stAwbParam.u8WDRMode = pstIspCtx->u8SnsWDRMode;

    for (i=0; i<MAX_REGISTER_ALG_LIB_NUM; i++)
    {
        if (pstIspCtx->stAwbLibInfo.astLibs[i].bUsed)
        {
            pstLib = &pstIspCtx->stAwbLibInfo.astLibs[i];
            if (HI_NULL != pstLib->stAwbRegsiter.stAwbExpFunc.pfn_awb_init)
            {
                pstLib->stAwbRegsiter.stAwbExpFunc.pfn_awb_init(
                    pstLib->stAlgLib.s32Id, &stAwbParam);
            }
        }
    }
   
    return HI_SUCCESS;
}

HI_S32 ISP_AwbRun(ISP_DEV IspDev, const HI_VOID *pStatInfo,
    HI_VOID *pRegCfg, HI_S32 s32Rsv)
{
    HI_S32 i, s32Ret = HI_FAILURE;
    ISP_AWB_INFO_S   stAwbInfo = {0};
    ISP_AWB_RESULT_S stAwbResult = {{0}};
    ISP_CTX_S       *pstIspCtx   = HI_NULL;
    ISP_LIB_NODE_S  *pstLib      = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);
    pstLib = &pstIspCtx->stAwbLibInfo.astLibs[pstIspCtx->stAwbLibInfo.u32ActiveLib];

    if (pstIspCtx->stLinkage.bDefectPixel)
    {
        return HI_SUCCESS;
    }

    /* get statistics */
    stAwbInfo.u32FrameCnt = pstIspCtx->u32FrameCnt;

    stAwbInfo.pstAwbStat1 = &((ISP_STAT_S *)pStatInfo)->stAwbStat1;
    stAwbInfo.pstAwbStat2 = &((ISP_STAT_S *)pStatInfo)->stAwbStat2;
    stAwbInfo.pstAwbStat3 = &((ISP_STAT_S *)pStatInfo)->stAwbStat3;
    stAwbInfo.pstAwbStat4 = &((ISP_STAT_S *)pStatInfo)->stAwbStat4;

    /* linkage with the iso of ae */
    for (i=0; i<MAX_REGISTER_ALG_LIB_NUM; i++)
    {
        if (pstIspCtx->stAwbLibInfo.astLibs[i].bUsed)
        {
            pstLib = &pstIspCtx->stAwbLibInfo.astLibs[i];
            if (HI_NULL != pstLib->stAwbRegsiter.stAwbExpFunc.pfn_awb_ctrl)
            {
                pstLib->stAwbRegsiter.stAwbExpFunc.pfn_awb_ctrl(pstLib->stAlgLib.s32Id,
                    ISP_AWB_ISO_SET, (HI_VOID *)&pstIspCtx->stLinkage.u32Iso);
                pstLib->stAwbRegsiter.stAwbExpFunc.pfn_awb_ctrl(pstLib->stAlgLib.s32Id,
                    ISP_AWB_INTTIME_SET, (HI_VOID *)&pstIspCtx->stLinkage.u32Inttime);
                pstLib->stAwbRegsiter.stAwbExpFunc.pfn_awb_ctrl(pstLib->stAlgLib.s32Id,
                    ISP_AWB_PIRIS_SET, (HI_VOID *)&pstIspCtx->stLinkage.u32PirisGain);
            }
        }
    }

    if (HI_NULL != pstLib->stAwbRegsiter.stAwbExpFunc.pfn_awb_run)
    {
        s32Ret = pstLib->stAwbRegsiter.stAwbExpFunc.pfn_awb_run(
            pstLib->stAlgLib.s32Id, &stAwbInfo, &stAwbResult, 0);
        if (HI_SUCCESS != s32Ret)
        {
            printf("WARNING!! run awb lib err 0x%x!\n", s32Ret);
        }
    }
    
    AwbCfgReg(&stAwbResult, pstIspCtx->u8SnsWDRMode, pstIspCtx->stLinkage.u32IspDgain,
        pstIspCtx->stLinkage.u32IspDgainShift, (ISP_REG_CFG_S *)pRegCfg);
    
    return s32Ret;
}

HI_S32 ISP_AwbCtrl(ISP_DEV IspDev, HI_U32 u32Cmd, HI_VOID *pValue)
{    
    HI_S32  i, s32Ret = HI_FAILURE;
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_LIB_NODE_S *pstLib = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    for (i=0; i<MAX_REGISTER_ALG_LIB_NUM; i++)
    {
        if (pstIspCtx->stAwbLibInfo.astLibs[i].bUsed)
        {
            pstLib = &pstIspCtx->stAwbLibInfo.astLibs[i];
            if (HI_NULL != pstLib->stAwbRegsiter.stAwbExpFunc.pfn_awb_ctrl)
            {
                s32Ret = pstLib->stAwbRegsiter.stAwbExpFunc.pfn_awb_ctrl(
                    pstLib->stAlgLib.s32Id, u32Cmd, pValue);
            }
        }
    }

    return s32Ret;
}

HI_S32 ISP_AwbExit(ISP_DEV IspDev)
{
    HI_S32 i;
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_LIB_NODE_S *pstLib = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    for (i=0; i<MAX_REGISTER_ALG_LIB_NUM; i++)
    {
        if (pstIspCtx->stAwbLibInfo.astLibs[i].bUsed)
        {
            pstLib = &pstIspCtx->stAwbLibInfo.astLibs[i];
            if (HI_NULL != pstLib->stAwbRegsiter.stAwbExpFunc.pfn_awb_exit)
            {
                pstLib->stAwbRegsiter.stAwbExpFunc.pfn_awb_exit(
                    pstLib->stAlgLib.s32Id);
            }
        }
    }

    return HI_SUCCESS;
}

HI_S32 ISP_AlgRegisterAwb(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_ALG_NODE_S *pstAlgs = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->enAlgType = ISP_ALG_AWB;
    pstAlgs->stAlgFunc.pfn_alg_init = ISP_AwbInit;
    pstAlgs->stAlgFunc.pfn_alg_run  = ISP_AwbRun;
    pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_AwbCtrl;
    pstAlgs->stAlgFunc.pfn_alg_exit = ISP_AwbExit;
    pstAlgs->bUsed = HI_TRUE;

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

