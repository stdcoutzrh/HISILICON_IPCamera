/******************************************************************************

  Copyright (C), 2004-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_csc.c
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
#include "isp_proc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

/*复用3516a的参数*/
typedef struct hiHI_ISP_CSC_ATTR
{
    HI_S32 s32CSCIdc[3];   
	HI_S32 s32CSCOdc[3];
    HI_S32 s32CSCCoef[9];
}ISP_CSC_ATTR;

/* RGB->YUV709 常量系数矩阵 */
ISP_CSC_ATTR g_stCSCAttr_709 =
{
     /*csc输入直流(IDC)*/
     {0, 0, 0},
     /*csc输出直流(ODC)*/
     {0, 128, 128},
     /*csc系数*/
     {213, 715, 72, -117, -394, 511, 511, -464, -47},
};

ISP_CSC_ATTR g_astCscCtx[ISP_MAX_DEV_NUM];

#define CSC_GET_CTX(dev, pstCtx)   pstCtx = &g_astCscCtx[dev]

static HI_VOID CscRegsInitialize(ISP_DEV IspDev)
{
	ISP_CSC_ATTR *pstCscCtx = HI_NULL;
    
    CSC_GET_CTX(IspDev, pstCscCtx);

	memcpy(pstCscCtx, &g_stCSCAttr_709, sizeof(ISP_CSC_ATTR));
		
	hi_isp_csc_coef00_write((pstCscCtx->s32CSCCoef[0]*1024/1000));
	hi_isp_csc_coef01_write((pstCscCtx->s32CSCCoef[1]*1024/1000));
	hi_isp_csc_coef02_write((pstCscCtx->s32CSCCoef[2]*1024/1000));
	hi_isp_csc_coef10_write((pstCscCtx->s32CSCCoef[3]*1024/1000));
	hi_isp_csc_coef11_write((pstCscCtx->s32CSCCoef[4]*1024/1000));
	hi_isp_csc_coef12_write((pstCscCtx->s32CSCCoef[5]*1024/1000));
	hi_isp_csc_coef20_write((pstCscCtx->s32CSCCoef[6]*1024/1000));
	hi_isp_csc_coef21_write((pstCscCtx->s32CSCCoef[7]*1024/1000));
	hi_isp_csc_coef22_write((pstCscCtx->s32CSCCoef[8]*1024/1000));

	hi_isp_csc_in_dc0_write(pstCscCtx->s32CSCIdc[0] << 2);
	hi_isp_csc_in_dc1_write(pstCscCtx->s32CSCIdc[1] << 2);
	hi_isp_csc_in_dc2_write(pstCscCtx->s32CSCIdc[2] << 2);

	hi_isp_csc_out_dc0_write(pstCscCtx->s32CSCOdc[0] << 2);
	hi_isp_csc_out_dc1_write(pstCscCtx->s32CSCOdc[1] << 2);
	hi_isp_csc_out_dc2_write(pstCscCtx->s32CSCOdc[2] << 2);

	hi_isp_csc_enable_write(HI_TRUE);


    return;
}

static HI_S32 CscReadExtregs(ISP_DEV IspDev)
{   
	ISP_CSC_ATTR *pstCscCtx = HI_NULL;
    
    CSC_GET_CTX(IspDev, pstCscCtx);
		
	pstCscCtx->s32CSCCoef[0] = hi_isp_csc_coef00_read();
	pstCscCtx->s32CSCCoef[1] = hi_isp_csc_coef01_read();
	pstCscCtx->s32CSCCoef[2] = hi_isp_csc_coef02_read();
	pstCscCtx->s32CSCCoef[3] = hi_isp_csc_coef10_read();
	pstCscCtx->s32CSCCoef[4] = hi_isp_csc_coef11_read();
	pstCscCtx->s32CSCCoef[5] = hi_isp_csc_coef12_read();
	pstCscCtx->s32CSCCoef[6] = hi_isp_csc_coef20_read();
	pstCscCtx->s32CSCCoef[7] = hi_isp_csc_coef21_read();
	pstCscCtx->s32CSCCoef[8] = hi_isp_csc_coef22_read();
	
	pstCscCtx->s32CSCIdc[0] = hi_isp_csc_in_dc0_read();
	pstCscCtx->s32CSCIdc[1] = hi_isp_csc_in_dc1_read();
	pstCscCtx->s32CSCIdc[2] = hi_isp_csc_in_dc2_read();
	pstCscCtx->s32CSCOdc[0] = hi_isp_csc_out_dc0_read();
	pstCscCtx->s32CSCOdc[1] = hi_isp_csc_out_dc1_read();
	pstCscCtx->s32CSCOdc[2] = hi_isp_csc_out_dc2_read();
    
    return 0;
}

HI_S32 CscProcWrite(ISP_DEV IspDev, ISP_CTRL_PROC_WRITE_S *pstProc)
{
    ISP_CTRL_PROC_WRITE_S stProcTmp;
    ISP_CSC_ATTR *pstCsc = HI_NULL;
    
    CSC_GET_CTX(IspDev, pstCsc);
 
    if ((HI_NULL == pstProc->pcProcBuff)
        || (0 == pstProc->u32BuffLen))
    {
        return HI_FAILURE;
    }

	CscReadExtregs(IspDev);
		
    stProcTmp.pcProcBuff = pstProc->pcProcBuff;
    stProcTmp.u32BuffLen = pstProc->u32BuffLen;
    
    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
        "-----CSC INFO-------------------------------------------------------------\n");

    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
            "%8s"   "%8s"   "%8s"   "%8s"   "%8s"   "%8s"      "%8s"   "%8s"   "%8s\n",
            "coef00", "coef01", "coef02", "coef10", "coef11", "coef12", "coef20", "coef21", "coef22");
    
    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
            "%8d" "%8d" "%8d" "%8d" "%8d" "%8d" "%8d" "%8d" "%8d\n\n",
            pstCsc->s32CSCCoef[0],
            pstCsc->s32CSCCoef[1],
            pstCsc->s32CSCCoef[2],
            pstCsc->s32CSCCoef[3],
            pstCsc->s32CSCCoef[4],
            pstCsc->s32CSCCoef[5],
            pstCsc->s32CSCCoef[6],
            pstCsc->s32CSCCoef[7],
            pstCsc->s32CSCCoef[8]
            );

	ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
            "%8s"    "%8s"    "%8s"    "%8s"    "%8s"    "%8s\n",
            "in_dc0", "in_dc1", "in_dc2", "out_dc0", "out_dc1", "out_dc2");
    
	ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
            "%8d" "%8d" "%8d" "%8d" "%8d"  "%8d\n",
            pstCsc->s32CSCIdc[0],
            pstCsc->s32CSCIdc[1],
            pstCsc->s32CSCIdc[2],
            pstCsc->s32CSCOdc[0], 
            pstCsc->s32CSCOdc[1],
            pstCsc->s32CSCOdc[2]
            );

    pstProc->u32WriteLen += 1;
    
    return HI_SUCCESS;
}


HI_S32 ISP_CscInit(ISP_DEV IspDev)
{
    CscRegsInitialize(IspDev);
    
    return HI_SUCCESS;
}

HI_S32 ISP_CscRun(ISP_DEV IspDev, const HI_VOID *pStatInfo,
    HI_VOID *pRegCfg, HI_S32 s32Rsv)
{
    return HI_SUCCESS;
}

HI_S32 ISP_CscCtrl(ISP_DEV IspDev, HI_U32 u32Cmd, HI_VOID *pValue)
{
    switch (u32Cmd)
    {
        case ISP_WDR_MODE_SET :
            ISP_CscInit(IspDev);
            break;
        case ISP_PROC_WRITE:
            CscProcWrite(IspDev, (ISP_CTRL_PROC_WRITE_S *)pValue);
            break;
        default :
            break;
    }
    return HI_SUCCESS;
}

HI_S32 ISP_CscExit(ISP_DEV IspDev)
{
#if 1
	/*add by q00214668 for hi_mini_isp*/
	//hi_isp_csc_enable_write(HI_FALSE);
	//hi_isp_midfilter_en_write(HI_FALSE);
	//hi_isp_uv2c_en_write(HI_FALSE);
	/*end*/
#endif	
    return HI_SUCCESS;
}

HI_S32 ISP_AlgRegisterCsc(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_ALG_NODE_S *pstAlgs = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->enAlgType = ISP_ALG_CSC;
    pstAlgs->stAlgFunc.pfn_alg_init = ISP_CscInit;
    pstAlgs->stAlgFunc.pfn_alg_run  = ISP_CscRun;
    pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_CscCtrl;
    pstAlgs->stAlgFunc.pfn_alg_exit = ISP_CscExit;
    pstAlgs->bUsed = HI_TRUE;

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


