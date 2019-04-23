/******************************************************************************

  Copyright (C), 2004-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_common.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2014/08/08
  Description   : 
  History       :
  1.Date        : 2014/08/08
    Author      : q00214668
    Modification: Created file

******************************************************************************/

#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_config.h"
#include "isp_ext_config.h"
#include "isp_proc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#define HI_ISP_DMNR_DITHER_OUTBITS (10)
#define HI_ISP_DRC_DITHER_OUTBITS  (12)
#define HI_ISP_DCI_DITHER_OUTBITS  (8)

typedef enum ISP_DITHER_ROUND_MODE
{
	ISP_DITHER_ROUND_CUT = 0,
	ISP_DITHER_ROUND_OFF,
	ISP_DITHER_ROUND_RANDOM,
	ISP_DITHER_ROUND_BUTT,
}ISP_DITHER_ROUND_MODE_EN;

static HI_VOID CommRegsInitialize(ISP_DEV IspDev)
{	
	hi_isp_dmnr_dither_en_write(HI_TRUE);
	hi_isp_drc_moudle_dither_en_write(HI_FALSE);
	hi_isp_dci_dither_en_write(HI_FALSE);

	hi_isp_dmnr_dither_out_bits_write(HI_ISP_DMNR_DITHER_OUTBITS);
	hi_isp_drc_dither_out_bits_write(HI_ISP_DRC_DITHER_OUTBITS);
	hi_isp_dci_dither_out_bits_write(HI_ISP_DCI_DITHER_OUTBITS);

	hi_isp_dmnr_dither_round_write(ISP_DITHER_ROUND_OFF);
	hi_isp_drc_dither_round_write(ISP_DITHER_ROUND_OFF);
	hi_isp_dci_dither_round_write(ISP_DITHER_ROUND_OFF);

	hi_isp_dmnr_dither_spatial_mode_write(HI_FALSE);
	hi_isp_drc_dither_spatial_mode_write(HI_FALSE);
	hi_isp_dci_dither_spatial_mode_write(HI_FALSE);
	
    return;
}

#if 0
static HI_S32 CommReadExtregs(ISP_DEV IspDev)
{      
    return 0;
}
#endif

HI_S32 CommProcWrite(ISP_DEV IspDev, ISP_CTRL_PROC_WRITE_S *pstProc)
{   
    return HI_SUCCESS;
}


HI_S32 ISP_CommInit(ISP_DEV IspDev)
{
    CommRegsInitialize(IspDev);
    
    return HI_SUCCESS;
}

HI_S32 ISP_CommRun(ISP_DEV IspDev, const HI_VOID *pStatInfo,
    HI_VOID *pRegCfg, HI_S32 s32Rsv)
{
	//CommReadExtregs(IspDev);
    return HI_SUCCESS;
}

HI_S32 ISP_CommCtrl(ISP_DEV IspDev, HI_U32 u32Cmd, HI_VOID *pValue)
{
    switch (u32Cmd)
    {
        case ISP_WDR_MODE_SET :
            ISP_CommInit(IspDev);
            break;
        case ISP_PROC_WRITE:
            CommProcWrite(IspDev, (ISP_CTRL_PROC_WRITE_S *)pValue);
            break;
        default :
            break;
    }
    return HI_SUCCESS;
}

HI_S32 ISP_CommExit(ISP_DEV IspDev)
{
    return HI_SUCCESS;
}

HI_S32 ISP_AlgRegisterComm(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_ALG_NODE_S *pstAlgs = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->enAlgType = ISP_ALG_COMM;
    pstAlgs->stAlgFunc.pfn_alg_init = ISP_CommInit;
    pstAlgs->stAlgFunc.pfn_alg_run  = ISP_CommRun;
    pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_CommCtrl;
    pstAlgs->stAlgFunc.pfn_alg_exit = ISP_CommExit;
    pstAlgs->bUsed = HI_TRUE;

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


