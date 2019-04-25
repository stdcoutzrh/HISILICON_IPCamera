/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : sample_awb_adp.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2012/12/19
  Description   : 
  History       :
  1.Date        : 2012/12/19
    Author      : n00168968
    Modification: Created file

******************************************************************************/
#include <stdio.h>
#include <string.h>

#include "mpi_isp.h"
#include "hi_vreg.h"
#include "sample_awb_adp.h"
#include "sample_awb_ext_config.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

/****************************************************************************
 * MACRO DEFINITION                                                         *
 ****************************************************************************/


/****************************************************************************
 * EXTERN VARIABLES                                                         *
 ****************************************************************************/


/****************************************************************************
 * GLOBAL VARIABLES                                                         *
 ****************************************************************************/
 
SAMPLE_AWB_CTX_S g_astAwbCtx[MAX_AWB_LIB_NUM] = {{0}};


HI_S32 SAMPLE_AWB_ExtRegsInitialize(HI_S32 s32Handle)
{
    HI_U8 u8Id;
    SAMPLE_AWB_CTX_S *pstAwbCtx = HI_NULL;
    pstAwbCtx = AWB_GET_CTX(s32Handle);
    u8Id = AWB_GET_EXTREG_ID(s32Handle);
    // avoid warning
    pstAwbCtx = pstAwbCtx;

    hi_ext_system_wb_type_write(u8Id, HI_EXT_SYSTEM_WB_TYPE_DEFAULT);
    
    return HI_SUCCESS;
}

HI_S32 SAMPLE_AWB_ReadExtRegs(HI_S32 s32Handle)
{
    SAMPLE_AWB_CTX_S *pstAwbCtx = HI_NULL;
    HI_U8 u8Id;
    HI_U8 u8WbType;

    pstAwbCtx = AWB_GET_CTX(s32Handle);
    u8Id = AWB_GET_EXTREG_ID(s32Handle);
    
    /* read the extregs to the global variables */
    
    u8WbType = hi_ext_system_wb_type_read(u8Id);

    pstAwbCtx->u8WbType = u8WbType;

    return HI_SUCCESS;
}

HI_S32 SAMPLE_AWB_UpdateExtRegs(HI_S32 s32Handle)
{
    SAMPLE_AWB_CTX_S *pstAwbCtx = HI_NULL;
    HI_U8 u8Id;
    
    pstAwbCtx = AWB_GET_CTX(s32Handle);
    u8Id = AWB_GET_EXTREG_ID(s32Handle);

    /* update the global variables to the extregs */

    hi_ext_system_wb_detect_temp_write(u8Id, pstAwbCtx->u16DetectTemp);

    return HI_SUCCESS;
}

HI_S32 SAMPLE_AWB_IspRegsInit(HI_S32 s32Handle)
{
    SAMPLE_AWB_CTX_S *pstAwbCtx = HI_NULL;
    HI_S32 i;
    ISP_AWB_STAT_ATTR_S *pstStatAttr = HI_NULL;
    ISP_AWB_RAW_STAT_ATTR_S *pstRawStatAttr = HI_NULL;

    pstAwbCtx = AWB_GET_CTX(s32Handle);

    pstStatAttr = &pstAwbCtx->stAwbResult.stStatAttr;
    pstStatAttr->u16MeteringWhiteLevelAwb = 0x3ac;
    pstStatAttr->u16MeteringBlackLevelAwb = 0x10;
    pstStatAttr->u16MeteringCrRefMaxAwb = 256*2;
    pstStatAttr->u16MeteringCrRefMinAwb = 256/2;
    pstStatAttr->u16MeteringCbRefMaxAwb = 256*2;
    pstStatAttr->u16MeteringCbRefMinAwb = 256/2;
    pstStatAttr->u16MeteringCrRefHighAwb = 256*2;
    pstStatAttr->u16MeteringCrRefLowAwb = 256/2;
    pstStatAttr->u16MeteringCbRefHighAwb = 256*2;
    pstStatAttr->u16MeteringCbRefLowAwb = 256/2;
    pstStatAttr->bChange = HI_TRUE;

    pstRawStatAttr = &pstAwbCtx->stAwbResult.stRawStatAttr;
    pstRawStatAttr->u16MeteringWhiteLevelAwb = 0xFFF;
    pstRawStatAttr->u16MeteringBlackLevelAwb = 0x08;
    pstRawStatAttr->u16MeteringCrRefMaxAwb = 0x130;
    pstRawStatAttr->u16MeteringCrRefMinAwb = 0x40;
    pstRawStatAttr->u16MeteringCbRefMaxAwb = 0x120;
    pstRawStatAttr->u16MeteringCbRefMinAwb = 0x40;
    pstRawStatAttr->u16MeteringCrRefHighAwb = pstRawStatAttr->u16MeteringCrRefMaxAwb ;
    pstRawStatAttr->u16MeteringCrRefLowAwb = pstRawStatAttr->u16MeteringCrRefMinAwb ;
    pstRawStatAttr->u16MeteringCbRefHighAwb = pstRawStatAttr->u16MeteringCbRefMaxAwb ;
    pstRawStatAttr->u16MeteringCbRefLowAwb = pstRawStatAttr->u16MeteringCbRefMinAwb ;
    pstRawStatAttr->bChange = HI_TRUE;
    pstRawStatAttr->bAboveWhiteLevelClip = HI_TRUE;
    pstRawStatAttr->bBelowBlackLevelClip= HI_TRUE;

    for (i=0; i<4; i++)
    {
        pstAwbCtx->stAwbResult.au32WhiteBalanceGain[i] =
            pstAwbCtx->stSnsDft.au16GainOffset[i] << 8;
    }

    return HI_SUCCESS;
}

HI_S32 SAMPLE_AWB_Calculate(HI_S32 s32Handle)
{
    SAMPLE_AWB_CTX_S *pstAwbCtx = HI_NULL;

    pstAwbCtx = AWB_GET_CTX(s32Handle);

    pstAwbCtx = pstAwbCtx;

     /* user's AWB alg implementor */  
    
    return HI_SUCCESS;
}

HI_S32 SAMPLE_AWB_Init(HI_S32 s32Handle, const ISP_AWB_PARAM_S *pstAwbParam)
{
    HI_S32 s32Ret;
    SAMPLE_AWB_CTX_S *pstAwbCtx = HI_NULL;
    HI_U8 u8Id;
    
    AWB_CHECK_HANDLE_ID(s32Handle);
    pstAwbCtx = AWB_GET_CTX(s32Handle);
    u8Id = AWB_GET_EXTREG_ID(s32Handle);
    
    AWB_CHECK_POINTER(pstAwbParam);

    pstAwbCtx = pstAwbCtx;
    
    /* do something ... like check the sensor id, init the global variables...
     * and so on 
     */

    /* Commonly, create a virtual regs to communicate, also user can design...
     * the new commuincation style. 
     */
     
    s32Ret = VReg_Init(AWB_LIB_VREG_BASE(u8Id), ALG_LIB_VREG_SIZE);
    if (HI_SUCCESS != s32Ret)
    {
        printf("Awb lib(%d) vreg init failed!\n", s32Handle);
        return s32Ret;
    }

    SAMPLE_AWB_ExtRegsInitialize(s32Handle);

    /* maybe you need to init the virtual regs. */

    return HI_SUCCESS;
}

HI_S32 SAMPLE_AWB_Run(HI_S32 s32Handle, const ISP_AWB_INFO_S *pstAwbInfo,
    ISP_AWB_RESULT_S *pstAwbResult, HI_S32 s32Rsv)
{
    HI_S32  i;
    SAMPLE_AWB_CTX_S *pstAwbCtx = HI_NULL;
    
    AWB_CHECK_HANDLE_ID(s32Handle);
    pstAwbCtx = AWB_GET_CTX(s32Handle);

    AWB_CHECK_POINTER(pstAwbInfo);
    AWB_CHECK_POINTER(pstAwbResult);

    AWB_CHECK_POINTER(pstAwbInfo->pstAwbStat1);
    AWB_CHECK_POINTER(pstAwbInfo->pstAwbStat2);
    
    pstAwbCtx->u32FrameCnt = pstAwbInfo->u32FrameCnt;

    /* init isp regs in the first frame. the regs in stStatAttr may need to be configed a few times. */
    if (1 == pstAwbCtx->u32FrameCnt)
    {
        SAMPLE_AWB_IspRegsInit(s32Handle);
    }

    /* do something ... */
    if (0 == pstAwbCtx->u32FrameCnt % 2)
    {
        /* record the statistics in pstAwbInfo, and then use the statistics to calculate, no need to call any other api */
        memcpy(&pstAwbCtx->stAwbInfo, pstAwbInfo, sizeof(ISP_AWB_INFO_S));

        /* maybe need to read the virtual regs to check whether someone changes the configs. */
        SAMPLE_AWB_ReadExtRegs(s32Handle);
        
        SAMPLE_AWB_Calculate(s32Handle);

        /* maybe need to write some configs to the virtual regs. */
        SAMPLE_AWB_UpdateExtRegs(s32Handle);

        /* pls fill the result after calculate, the firmware will config the regs for awb algorithm. */
        for (i=0; i<9; i++)
        {
            pstAwbCtx->stAwbResult.au16ColorMatrix[i] = 0x0;
        }
        
        /* the unit of the result is not fixed, just map with the isp_awb.c, modify the unit together. */
        pstAwbCtx->stAwbResult.au16ColorMatrix[0] = 0x100;
        pstAwbCtx->stAwbResult.au16ColorMatrix[4] = 0x100;
        pstAwbCtx->stAwbResult.au16ColorMatrix[8] = 0x100;

        pstAwbCtx->stAwbResult.au32WhiteBalanceGain[0] = 0x100 << 8;
        pstAwbCtx->stAwbResult.au32WhiteBalanceGain[1] = 0x100 << 8;
        pstAwbCtx->stAwbResult.au32WhiteBalanceGain[2] = 0x100 << 8;
        pstAwbCtx->stAwbResult.au32WhiteBalanceGain[3] = 0x100 << 8;
    }
    
    /* record result */    
    memcpy(pstAwbResult, &pstAwbCtx->stAwbResult, sizeof(ISP_AWB_RESULT_S));
    
    pstAwbCtx->stAwbResult.stStatAttr.bChange = HI_FALSE;
    
    return HI_SUCCESS;
}

HI_S32 SAMPLE_AWB_Ctrl(HI_S32 s32Handle, HI_U32 u32Cmd, HI_VOID *pValue)
{
    SAMPLE_AWB_CTX_S *pstAwbCtx = HI_NULL;
    
    AWB_CHECK_HANDLE_ID(s32Handle);
    pstAwbCtx = AWB_GET_CTX(s32Handle);
    
    AWB_CHECK_POINTER(pValue);
    
    pstAwbCtx = pstAwbCtx;
    
    switch (u32Cmd)
    {
        /* system ctrl */
        case ISP_WDR_MODE_SET :
            /* do something ... */
            break;
        case ISP_AWB_ISO_SET :
            /* do something ... */
            break;
        /* awb ctrl, define the customer's ctrl cmd, if needed ... */
        default :
            break;
    }

    return HI_SUCCESS;
}

HI_S32 SAMPLE_AWB_Exit(HI_S32 s32Handle)
{
    HI_S32 s32Ret;
    HI_U8 u8Id;
    SAMPLE_AWB_CTX_S *pstAwbCtx = HI_NULL;

    AWB_CHECK_HANDLE_ID(s32Handle);
    pstAwbCtx = AWB_GET_CTX(s32Handle);
    u8Id = AWB_GET_EXTREG_ID(s32Handle);

    pstAwbCtx = pstAwbCtx;

    /* do something ... */

    /* if created the virtual regs, need to destory virtual regs. */
    s32Ret = VReg_Exit(AWB_LIB_VREG_BASE(u8Id), ALG_LIB_VREG_SIZE);
    if (HI_SUCCESS != s32Ret)
    {
        printf("Awb lib(%d) vreg exit failed!\n", s32Handle);
        return s32Ret;
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_AWB_Register(ISP_DEV IspDev, ALG_LIB_S *pstAwbLib)
{
    ISP_AWB_REGISTER_S stRegister;
    HI_S32 s32Ret = HI_SUCCESS;

    AWB_CHECK_DEV(IspDev);
    AWB_CHECK_POINTER(pstAwbLib);
    AWB_CHECK_HANDLE_ID(pstAwbLib->s32Id);
    AWB_CHECK_LIB_NAME(pstAwbLib->acLibName);

    stRegister.stAwbExpFunc.pfn_awb_init  = SAMPLE_AWB_Init;
    stRegister.stAwbExpFunc.pfn_awb_run   = SAMPLE_AWB_Run;
    stRegister.stAwbExpFunc.pfn_awb_ctrl  = SAMPLE_AWB_Ctrl;
    stRegister.stAwbExpFunc.pfn_awb_exit  = SAMPLE_AWB_Exit;
    s32Ret = HI_MPI_ISP_AWBLibRegCallBack(IspDev, pstAwbLib, &stRegister);
    if (HI_SUCCESS != s32Ret)
    {
        printf("Hi_awb register failed!\n");
    }
    
    return s32Ret;
}

HI_S32 HI_MPI_AWB_UnRegister(ISP_DEV IspDev, ALG_LIB_S *pstAwbLib)
{
    HI_S32 s32Ret = HI_SUCCESS;

    AWB_CHECK_DEV(IspDev);
    AWB_CHECK_POINTER(pstAwbLib);
    AWB_CHECK_HANDLE_ID(pstAwbLib->s32Id);
    AWB_CHECK_LIB_NAME(pstAwbLib->acLibName);
    
    s32Ret = HI_MPI_ISP_AWBLibUnRegCallBack(IspDev, pstAwbLib);
    if (HI_SUCCESS != s32Ret)
    {
        printf("Hi_awb unregister failed!\n");
    }
    
    return s32Ret;
}


HI_S32 HI_MPI_AWB_SensorRegCallBack(ISP_DEV IspDev, ALG_LIB_S *pstAwbLib, SENSOR_ID SensorId,
    AWB_SENSOR_REGISTER_S *pstRegister)
{
    SAMPLE_AWB_CTX_S *pstAwbCtx = HI_NULL;
    HI_S32  s32Handle;

    AWB_CHECK_DEV(IspDev);
    AWB_CHECK_POINTER(pstAwbLib);
    AWB_CHECK_POINTER(pstRegister);
    
    s32Handle = pstAwbLib->s32Id;
    AWB_CHECK_HANDLE_ID(s32Handle);
    AWB_CHECK_LIB_NAME(pstAwbLib->acLibName);

    pstAwbCtx = AWB_GET_CTX(s32Handle);

    if (HI_NULL != pstRegister->stSnsExp.pfn_cmos_get_awb_default)
    {
        pstRegister->stSnsExp.pfn_cmos_get_awb_default(&pstAwbCtx->stSnsDft);
    }
    
    memcpy(&pstAwbCtx->stSnsRegister, pstRegister, sizeof(AWB_SENSOR_REGISTER_S));
    pstAwbCtx->SensorId = SensorId;
    
    pstAwbCtx->bSnsRegister = HI_TRUE;

    return HI_SUCCESS;
}

HI_S32 HI_MPI_AWB_SensorUnRegCallBack(ISP_DEV IspDev, ALG_LIB_S *pstAwbLib, SENSOR_ID SensorId)
{
    SAMPLE_AWB_CTX_S *pstAwbCtx = HI_NULL;
    HI_S32  s32Handle;

    AWB_CHECK_DEV(IspDev);
    AWB_CHECK_POINTER(pstAwbLib);
    
    s32Handle = pstAwbLib->s32Id;
    AWB_CHECK_HANDLE_ID(s32Handle);
    AWB_CHECK_LIB_NAME(pstAwbLib->acLibName);

    pstAwbCtx = AWB_GET_CTX(s32Handle);

    memset(&pstAwbCtx->stSnsDft, 0, sizeof(AWB_SENSOR_DEFAULT_S));
    memset(&pstAwbCtx->stSnsRegister, 0, sizeof(AWB_SENSOR_REGISTER_S));
    pstAwbCtx->SensorId = 0;    
    pstAwbCtx->bSnsRegister = HI_FALSE;

    return HI_SUCCESS;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


