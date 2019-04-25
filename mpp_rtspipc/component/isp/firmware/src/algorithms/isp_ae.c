/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_ae.c
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
#include <string.h>
#include "isp_alg.h"
#include "isp_ext_config.h"
#include "isp_config.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

static HI_VOID AeRegsDefault(HI_VOID)
{
    /* still need to set ae default value */    
    /* init the ae weighting table . */
    HI_U8 i,j;
	HI_U32 u32CombinWeight = 0;
	HI_U32 u32CombinWeightNum = 0;

    HI_U8 u8Weighttable[15][17] =
        {{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
         {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
         {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
         {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
         {1,1,1,1,1,2,2,2,2,2,2,2,1,1,1,1,1},
         {1,1,1,1,1,2,2,2,2,2,2,2,1,1,1,1,1},
         {1,1,1,1,2,2,2,2,2,2,2,2,2,1,1,1,1},
         {1,1,1,1,2,2,2,2,2,2,2,2,2,1,1,1,1},
         {1,1,1,1,2,2,2,2,2,2,2,2,2,1,1,1,1},
         {1,1,1,1,1,2,2,2,2,2,2,2,1,1,1,1,1},
         {1,1,1,1,1,2,2,2,2,2,2,2,1,1,1,1,1},
         {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
         {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
         {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
         {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}};
	
	hi_isp_ae_mem_wei_waddr_write(0);
    for (i=0; i<15; i++)
    {
        for (j=0; j<17; j++)
        {
        	u32CombinWeight |= (u8Weighttable[i][j] << (8*u32CombinWeightNum));
			u32CombinWeightNum++;

			if (u32CombinWeightNum == HI_ISP_AE_WEI_COMBIN_COUNT)
			{
				u32CombinWeightNum = 0;
				hi_isp_ae_mem_wei_wdata_write(u32CombinWeight);
				u32CombinWeight = 0;
			}			
        }
    }
	
	if (u32CombinWeightNum != HI_ISP_AE_WEI_COMBIN_COUNT
		&&u32CombinWeightNum != 0)
	{
		hi_isp_ae_mem_wei_wdata_write(u32CombinWeight);
	}

	/*init aexp_nodes_used_horiz*/
	#ifdef HI_MINI_ISP
	hi_isp_metering_aexp_nodes_used_horiz_write(HI_ISP_METERING_AEXP_NODES_USED_HORIZ_DEFAULT);
	hi_isp_metering_aexp_nodes_used_vert_write(HI_ISP_METERING_AEXP_NODES_USED_VERT_DEFAULT);	
	hi_isp_ae_version_write(HI_ISP_AE_VERSION_V100);
	hi_isp_ae_cfg_enable_write(HI_TRUE);
	hi_isp_dg_enable_write(HI_TRUE);
	#endif
	
    return;
}
HI_S32 ISP_AeCtrl(ISP_DEV IspDev, HI_U32 u32Cmd, HI_VOID *pValue);
HI_S32 ISP_AeInit(ISP_DEV IspDev)
{
    HI_S32 i;
    ISP_AE_PARAM_S stAeParam;
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_LIB_NODE_S *pstLib = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    AeRegsDefault();
    
    stAeParam.SensorId = pstIspCtx->stBindAttr.SensorId;
    stAeParam.u8WDRMode = pstIspCtx->u8SnsWDRMode;
    stAeParam.f32Fps = pstIspCtx->stSnsImageMode.f32Fps;

    /* AE statistics info use sqrt data in WDR mode */
	if(IS_BUILT_IN_WDR_MODE(pstIspCtx->u8SnsWDRMode))
	{
		hi_isp_ae_bit_move_gamma_write(0x100);
	}
	else
    {
	    hi_isp_ae_bit_move_gamma_write(0x0);
	}

    /* init all registered ae libs */
    for (i=0; i<MAX_REGISTER_ALG_LIB_NUM; i++)
    {
        if (pstIspCtx->stAeLibInfo.astLibs[i].bUsed)
        {
            pstLib = &pstIspCtx->stAeLibInfo.astLibs[i];
            if (HI_NULL != pstLib->stAeRegsiter.stAeExpFunc.pfn_ae_init)
            {
                pstLib->stAeRegsiter.stAeExpFunc.pfn_ae_init(
                    pstLib->stAlgLib.s32Id, &stAeParam);
            }
        }
    }
   
    return HI_SUCCESS;
}

HI_S32 ISP_AeRun(ISP_DEV IspDev, const HI_VOID *pStatInfo,
    HI_VOID *pRegCfg, HI_S32 s32Rsv)
{
    HI_S32 i, j, s32Ret = HI_FAILURE;
    ISP_AE_INFO_S       stAeInfo    = {0};
    ISP_AE_RESULT_S     stAeResult  = {{0}};
    ISP_CTX_S          *pstIspCtx   = HI_NULL;
    ISP_LIB_NODE_S     *pstLib      = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);
    pstLib = &pstIspCtx->stAeLibInfo.astLibs[pstIspCtx->stAeLibInfo.u32ActiveLib];

    if (pstIspCtx->stLinkage.bDefectPixel)
    {
        return HI_SUCCESS;
    }

    stAeInfo.u32FrameCnt = pstIspCtx->u32FrameCnt;

    stAeInfo.pstAeStat1 = &((ISP_STAT_S *)pStatInfo)->stAeStat1;   /* not support */
    stAeInfo.pstAeStat2 = &((ISP_STAT_S *)pStatInfo)->stAeStat2;   /* not support */
    stAeInfo.pstAeStat3 = &((ISP_STAT_S *)pStatInfo)->stAeStat3;
    stAeInfo.pstAeStat4 = &((ISP_STAT_S *)pStatInfo)->stAeStat4; 
    stAeInfo.pstAeStat5 = &((ISP_STAT_S *)pStatInfo)->stAeStat5;

    if (HI_NULL != pstLib->stAeRegsiter.stAeExpFunc.pfn_ae_run)
    {
        s32Ret = pstLib->stAeRegsiter.stAeExpFunc.pfn_ae_run(
            pstLib->stAlgLib.s32Id, &stAeInfo, &stAeResult, 0);
        if (HI_SUCCESS != s32Ret)
        {
            printf("WARNING!! run ae lib err 0x%x!\n", s32Ret);
        }
    }
    
    if (stAeResult.stStatAttr.bChange)
    {
        for (i = 0; i < 15; i++)
        {
            for (j = 0; j < 17; j++)
            {
                ((ISP_REG_CFG_S *)pRegCfg)->stAeRegCfg1.au8WeightTable[i][j] =
                    stAeResult.stStatAttr.au8WeightTable[i][j];
            }
        }

        for (i = 0; i < 4; i++)
        {
            ((ISP_REG_CFG_S *)pRegCfg)->stAeRegCfg1.au8MeteringHistThresh[i] =
                stAeResult.stStatAttr.au8MeteringHistThresh[i];
        }

        ((ISP_REG_CFG_S *)pRegCfg)->unKey.bit1AeCfg1 = 1;
    }    
    
    ((ISP_REG_CFG_S *)pRegCfg)->stAeRegCfg2.u32IntTime[0] = stAeResult.u32IntTime[0];
    ((ISP_REG_CFG_S *)pRegCfg)->stAeRegCfg2.u32IntTime[1] = stAeResult.u32IntTime[1];
    ((ISP_REG_CFG_S *)pRegCfg)->stAeRegCfg2.u32IntTime[2] = stAeResult.u32IntTime[2];
    ((ISP_REG_CFG_S *)pRegCfg)->stAeRegCfg2.u32IntTime[3] = stAeResult.u32IntTime[3];
    ((ISP_REG_CFG_S *)pRegCfg)->stAeRegCfg2.u32IspDgain = stAeResult.u32IspDgain;
    ((ISP_REG_CFG_S *)pRegCfg)->unKey.bit1AeCfg2 = 1;

    ((ISP_REG_CFG_S *)pRegCfg)->stAeRegCfg2.bPirisValid = stAeResult.bPirisValid;
    ((ISP_REG_CFG_S *)pRegCfg)->stAeRegCfg2.s32PirisPos = stAeResult.s32PirisPos;
    /* be careful avoid overflow */
    if(stAeResult.bPirisValid == HI_TRUE)
    {
        ((ISP_REG_CFG_S *)pRegCfg)->stAeRegCfg2.u64Exposure = ((HI_U64)stAeResult.u32IntTime[0] * (stAeResult.u32Iso>>4) >> 6) * stAeResult.u32PirisGain;
    }
    else
    {
        ((ISP_REG_CFG_S *)pRegCfg)->stAeRegCfg2.u64Exposure = (HI_U64)stAeResult.u32IntTime[0] * (stAeResult.u32Iso>>4) >> 6;
    }
    
	
    pstIspCtx->stLinkage.u32IspDgain = stAeResult.u32IspDgain;
    pstIspCtx->stLinkage.u32IspDgainShift = 8;
    pstIspCtx->stLinkage.u32Iso = stAeResult.u32Iso;
    pstIspCtx->stLinkage.u32SensorIso = ((HI_U64)stAeResult.u32Iso << 8) / DIV_0_TO_1(stAeResult.u32IspDgain);  // IspDgain is 8bit precision
    pstIspCtx->stLinkage.u32SensorIso = (pstIspCtx->stLinkage.u32SensorIso < 100) ? 100 : pstIspCtx->stLinkage.u32SensorIso;
    pstIspCtx->stLinkage.u32ExpRatio = (stAeResult.u32IntTime[1] << 6) / DIV_0_TO_1(stAeResult.u32IntTime[0]);  // WDR exposure ratio is 6bit precision
    pstIspCtx->stLinkage.u32Inttime = stAeResult.u32IntTime[0];
    if (HI_TRUE == stAeResult.bPirisValid)
    {
        pstIspCtx->stLinkage.u32PirisGain = stAeResult.u32PirisGain;
    }
    else
    {
        pstIspCtx->stLinkage.u32PirisGain = 0;
    }
    pstIspCtx->stLinkage.u8AERunInterval = stAeResult.u8AERunInterval;

    return s32Ret;
}

HI_S32 ISP_AeCtrl(ISP_DEV IspDev, HI_U32 u32Cmd, HI_VOID *pValue)
{    
    HI_S32  i, s32Ret = HI_FAILURE;
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_LIB_NODE_S *pstLib = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    for (i=0; i<MAX_REGISTER_ALG_LIB_NUM; i++)
    {
        if (pstIspCtx->stAeLibInfo.astLibs[i].bUsed)
        {
            pstLib = &pstIspCtx->stAeLibInfo.astLibs[i];
            if (HI_NULL != pstLib->stAeRegsiter.stAeExpFunc.pfn_ae_ctrl)
            {
                s32Ret = pstLib->stAeRegsiter.stAeExpFunc.pfn_ae_ctrl(
                    pstLib->stAlgLib.s32Id, u32Cmd, pValue);
            }
        }
    }

    return s32Ret;
}

HI_S32 ISP_AeExit(ISP_DEV IspDev)
{
    HI_S32 i;
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_LIB_NODE_S *pstLib = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    for (i=0; i<MAX_REGISTER_ALG_LIB_NUM; i++)
    {
        if (pstIspCtx->stAeLibInfo.astLibs[i].bUsed)
        {
            pstLib = &pstIspCtx->stAeLibInfo.astLibs[i];
            if (HI_NULL != pstLib->stAeRegsiter.stAeExpFunc.pfn_ae_exit)
            {
                pstLib->stAeRegsiter.stAeExpFunc.pfn_ae_exit(
                    pstLib->stAlgLib.s32Id);
            }
        }
    }

	#ifdef HI_MINI_ISP
	hi_isp_ae_cfg_enable_write(HI_FALSE);
	#endif
	/*modify end*/

    return HI_SUCCESS;
}

HI_S32 ISP_AlgRegisterAe(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_ALG_NODE_S *pstAlgs = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->enAlgType = ISP_ALG_AE;
    pstAlgs->stAlgFunc.pfn_alg_init = ISP_AeInit;
    pstAlgs->stAlgFunc.pfn_alg_run  = ISP_AeRun;
    pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_AeCtrl;
    pstAlgs->stAlgFunc.pfn_alg_exit = ISP_AeExit;
    pstAlgs->bUsed = HI_TRUE;

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

