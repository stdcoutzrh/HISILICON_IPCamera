/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_alg.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/04/23
  Description   : 
  History       :
  1.Date        : 2013/04/23
    Author      : n00168968
    Modification: Created file

******************************************************************************/
#ifndef __ISP_AF_H__
#define __ISP_AF_H__

#include <stdlib.h>
#include <string.h>
#include "hi_common.h"
#include "hi_comm_isp.h"
#include "isp_main.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

HI_S32 ISP_AlgRegisterAe(ISP_DEV IspDev);
HI_S32 ISP_AlgRegisterAwb(ISP_DEV IspDev);
HI_S32 ISP_AlgRegisterAf(ISP_DEV IspDev);
//HI_S32 ISP_AlgRegisterAntiFog(ISP_DEV IspDev);
HI_S32 ISP_AlgRegisterBlc(ISP_DEV IspDev);
HI_S32 ISP_AlgRegisterDp(ISP_DEV IspDev);
HI_S32 ISP_AlgRegisterDrc(ISP_DEV IspDev);
HI_S32 ISP_AlgRegisterCsc(ISP_DEV IspDev);
HI_S32 ISP_AlgRegisterDemosaic(ISP_DEV IspDev);
HI_S32 ISP_AlgRegisterGamma(ISP_DEV IspDev);
HI_S32 ISP_AlgRegisterCompander(ISP_DEV IspDev);
HI_S32 ISP_AlgRegisterGammaFe(ISP_DEV IspDev);
HI_S32 ISP_AlgRegisterGe(ISP_DEV IspDev);
//HI_S32 ISP_AlgRegisterNewAntiFog(ISP_DEV IspDev);
HI_S32 ISP_AlgRegisterNr(ISP_DEV IspDev);
HI_S32 ISP_AlgRegisterSharpen(ISP_DEV IspDev);
HI_S32 ISP_AlgRegisterFrameWDR(ISP_DEV IspDev);
HI_S32 ISP_AlgRegisterFPN(ISP_DEV IspDev);
HI_S32 ISP_AlgRegisterDehaze(ISP_DEV IspDev);
HI_S32 ISP_AlgRegisterAcm(ISP_DEV IspDev);
HI_S32 ISP_AlgRegisterCac(ISP_DEV IspDev);
HI_S32 ISP_AlgRegisterUvnr(ISP_DEV IspDev);
HI_S32 ISP_AlgRegisterComm(ISP_DEV IspDev);
HI_S32 ISP_AlgRegisterLsc(ISP_DEV IspDev);
HI_S32 ISP_AlgRegisterRgbir(ISP_DEV IspDev);


/* find the specified library */
static inline HI_S32 ISP_FindLib(const ISP_LIB_NODE_S *astLibs,
    const ALG_LIB_S *pstLib)
{
    HI_S32  i;

    for (i=0; i<MAX_REGISTER_ALG_LIB_NUM; i++)
    {
        /* can't use memcmp, if user not fill the string. */
        if ((0 == strcmp(astLibs[i].stAlgLib.acLibName, pstLib->acLibName))
            && (astLibs[i].stAlgLib.s32Id == pstLib->s32Id))
        {
            return i;
        }
    }

    return -1;
}

/* search the empty pos of libs */
static inline ISP_LIB_NODE_S * ISP_SearchLib(ISP_LIB_NODE_S *astLibs, const ALG_LIB_S *pstLib)
{
    HI_S32 i;
    
    for (i=0; i<MAX_REGISTER_ALG_LIB_NUM; i++)
    {
        if (!astLibs[i].bUsed)
        {
            return &astLibs[i];
        }
		else if (0 == strcmp(astLibs[i].stAlgLib.acLibName, pstLib->acLibName)&&(astLibs[i].stAlgLib.s32Id ==pstLib->s32Id))
		{
			
    		printf("can't register the same lib to isp.\n");
			return HI_NULL;
		}
    }
	
    printf("can't register lib to isp, there is too many libs.\n");
    return HI_NULL;
}


/* search the empty pos of algs */
static inline ISP_ALG_NODE_S * ISP_SearchAlg(ISP_ALG_NODE_S *astAlgs)
{
    HI_S32 i;
    
    for (i=0; i<ISP_MAX_ALGS_NUM; i++)
    {
        if (!astAlgs[i].bUsed)
        {
            return &astAlgs[i];
        }
    }

    return HI_NULL;
}

static inline HI_VOID ISP_AlgsInit(const ISP_ALG_NODE_S *astAlgs, ISP_DEV IspDev)
{
    HI_S32 i;
    
    for (i=0; i<ISP_MAX_ALGS_NUM; i++)
    {
        if (astAlgs[i].bUsed)
        {
            if (HI_NULL != astAlgs[i].stAlgFunc.pfn_alg_init)
            {
                astAlgs[i].stAlgFunc.pfn_alg_init(IspDev);
            }
        }
    }

    return;
}

static inline HI_VOID ISP_AlgsRun(const ISP_ALG_NODE_S *astAlgs, ISP_DEV IspDev,
    const HI_VOID *pStatInfo, HI_VOID *pRegCfg, HI_S32 s32Rsv)
{
    HI_S32 i;
    
    for (i=0; i<ISP_MAX_ALGS_NUM; i++)
    {
        if (astAlgs[i].bUsed)
        {
            if (HI_NULL != astAlgs[i].stAlgFunc.pfn_alg_run)
            {
                astAlgs[i].stAlgFunc.pfn_alg_run(IspDev, pStatInfo, pRegCfg, s32Rsv);
            }
        }
    }

    return;
}

static inline HI_VOID ISP_AlgsCtrl(const ISP_ALG_NODE_S *astAlgs,
    ISP_DEV IspDev, HI_U32 u32Cmd, HI_VOID *pValue)
{
    HI_S32 i;
    
    for (i=0; i<ISP_MAX_ALGS_NUM; i++)
    {
        if (astAlgs[i].bUsed)
        {
            if (HI_NULL != astAlgs[i].stAlgFunc.pfn_alg_ctrl)
            {
                astAlgs[i].stAlgFunc.pfn_alg_ctrl(IspDev, u32Cmd, pValue);
            }
        }
    } 

    return;
}

static inline HI_VOID ISP_AlgsExit(const ISP_ALG_NODE_S *astAlgs, ISP_DEV IspDev)
{
    HI_S32 i;
    
    for (i=0; i<ISP_MAX_ALGS_NUM; i++)
    {
        if (astAlgs[i].bUsed)
        {
            if (HI_NULL != astAlgs[i].stAlgFunc.pfn_alg_exit)
            {
                astAlgs[i].stAlgFunc.pfn_alg_exit(IspDev);
            }
        }
    }

    return;
}

static inline HI_VOID ISP_AlgsRegister(ISP_DEV IspDev)
{
	ISP_AlgRegisterComm(IspDev);
    ISP_AlgRegisterDp(IspDev);
    ISP_AlgRegisterAe(IspDev);
    ISP_AlgRegisterAwb(IspDev);
    ISP_AlgRegisterAf(IspDev);
    ISP_AlgRegisterBlc(IspDev);
	ISP_AlgRegisterCompander(IspDev);
	ISP_AlgRegisterLsc(IspDev);
    ISP_AlgRegisterGammaFe(IspDev);
    ISP_AlgRegisterDrc(IspDev);
	//ISP_AlgRegisterCsc(IspDev);
    ISP_AlgRegisterDemosaic(IspDev);
    ISP_AlgRegisterGamma(IspDev);
    ISP_AlgRegisterGe(IspDev);
    ISP_AlgRegisterNr(IspDev);
    ISP_AlgRegisterSharpen(IspDev);
    //ISP_AlgRegisterFrameWDR(IspDev);
    ISP_AlgRegisterFPN(IspDev);
    ISP_AlgRegisterDehaze(IspDev);
    ISP_AlgRegisterAcm(IspDev);
    //ISP_AlgRegisterCac(IspDev);
	ISP_AlgRegisterUvnr(IspDev);	
    ISP_AlgRegisterRgbir(IspDev);

    return;
}

/* resolve problem: 
isp error: Null Pointer in ISP_AlgRegisterxxx when rerun isp_init without app exit */
static inline HI_VOID ISP_AlgsUnRegister(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    HI_S32 i;

    ISP_GET_CTX(IspDev, pstIspCtx);
    
    for (i=0; i<ISP_MAX_ALGS_NUM; i++)
    {
        if (pstIspCtx->astAlgs[i].bUsed)
        {
            pstIspCtx->astAlgs[i].bUsed = HI_FALSE;
        }
    }
}

/* resolve problem: 
isp error: HI_MPI_XX_Register failed when rerun 3a lib register without app exit */
static inline HI_VOID ISP_LibsUnRegister(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    HI_S32 i;

    ISP_GET_CTX(IspDev, pstIspCtx);
    
    for (i=0; i<MAX_REGISTER_ALG_LIB_NUM; i++)
    {
        pstIspCtx->stAeLibInfo.astLibs[i].bUsed  = HI_FALSE;
        pstIspCtx->stAwbLibInfo.astLibs[i].bUsed = HI_FALSE;
        pstIspCtx->stAfLibInfo.astLibs[i].bUsed  = HI_FALSE;
    }
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
