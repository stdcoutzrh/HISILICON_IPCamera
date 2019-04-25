/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : sample_ae_adp.c
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
#include "sample_ae_adp.h"
#include "sample_ae_ext_config.h"
#include "sample_ae_mpi.h"


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
SAMPLE_AE_CTX_S g_astAeCtx_sample[MAX_AE_LIB_NUM] = {{0}};


/****************************************************************************
 * EXTERN FUNCTION DECLARATION                                              *
 ****************************************************************************/


/****************************************************************************
 * INTERNAL FUNCTION DECLARATION                                            *
 ****************************************************************************/


/****************************************************************************
 * AE FUNCTION                                                             *
 *            ---- Assistant Function Area                                  *
 ****************************************************************************/

HI_S32 SAMPLE_AE_ExtRegsInitialize(HI_S32 s32Handle)
{
    SAMPLE_AE_CTX_S *pstAeCtx = HI_NULL;
    HI_U8 u8Id = AE_GET_EXTREG_ID(s32Handle);
    
    pstAeCtx = AE_GET_CTX(s32Handle);

    // avoid warning
    pstAeCtx = pstAeCtx;
    
    /* set ext registers as a default value */
    hi_ext_system_ae_mode_write(u8Id, HI_EXT_SYSTEM_AE_MODE_DEFAULT);

    return HI_SUCCESS;
}

HI_S32 SAMPLE_AE_ReadExtRegs(HI_S32 s32Handle)
{
    SAMPLE_AE_CTX_S *pstAeCtx = HI_NULL;
    HI_U8 u8Id = AE_GET_EXTREG_ID(s32Handle);
    
    pstAeCtx = AE_GET_CTX(s32Handle);

    /* read the extregs to the global variables */
    pstAeCtx->u8AeMode = hi_ext_system_ae_mode_read(u8Id);

    return HI_SUCCESS;
}

HI_S32 SAMPLE_AE_UpdateExtRegs(HI_S32 s32Handle)
{
    SAMPLE_AE_CTX_S *pstAeCtx = HI_NULL;
    HI_U8 u8Id = AE_GET_EXTREG_ID(s32Handle);
        
    pstAeCtx = AE_GET_CTX(s32Handle);

    /* update the global variables to the extregs */
    HI_U32 u32Iso;

    u32Iso = pstAeCtx->stAeResult.u32Iso;

    hi_ext_system_query_exposure_again_write(u8Id, u32Iso);
    

    return HI_SUCCESS;
}

HI_S32 SAMPLE_AE_Calculate(HI_S32 s32Handle)
{
    SAMPLE_AE_CTX_S *pstAeCtx = HI_NULL;

    // avoid warning
    pstAeCtx = pstAeCtx;
                     
    pstAeCtx = AE_GET_CTX(s32Handle);

    /* user's AE alg implementor */    
    
    return HI_SUCCESS;
}


HI_S32 SAMPLE_AE_IspRegsInit(HI_S32 s32Handle)
{
    SAMPLE_AE_CTX_S *pstAeCtx = HI_NULL;
    ISP_AE_STAT_ATTR_S *pstStatAttr = HI_NULL;
    
    HI_U8 au8WeightTable[15][17]={
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
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
    
    pstAeCtx = AE_GET_CTX(s32Handle);
    pstStatAttr = &pstAeCtx->stAeResult.stStatAttr;

    /* change the ae zone's weight table and histogram thresh, although there is
     * defalut value in isp. 
     */
    memcpy(pstStatAttr->au8WeightTable, au8WeightTable, sizeof(au8WeightTable));

    pstStatAttr->au8MeteringHistThresh[0] = pstAeCtx->stSnsDft.au8HistThresh[0];
    pstStatAttr->au8MeteringHistThresh[1] = pstAeCtx->stSnsDft.au8HistThresh[1];
    pstStatAttr->au8MeteringHistThresh[2] = pstAeCtx->stSnsDft.au8HistThresh[2];
    pstStatAttr->au8MeteringHistThresh[3] = pstAeCtx->stSnsDft.au8HistThresh[3];

    pstStatAttr->bChange = HI_TRUE;

    return HI_SUCCESS;
}


HI_S32 SAMPLE_AE_Init(HI_S32 s32Handle, const ISP_AE_PARAM_S *pstAeParam)
{
    HI_S32 s32Ret;
    SAMPLE_AE_CTX_S *pstAeCtx = HI_NULL;
    HI_U8 u8Id;

    // avoid warning
    pstAeCtx = pstAeCtx;

    AE_CHECK_HANDLE_ID(s32Handle);
    pstAeCtx = AE_GET_CTX(s32Handle);
    u8Id = AE_GET_EXTREG_ID(s32Handle);
    
    AE_CHECK_POINTER(pstAeParam);

    //printf("********************* AeInit Sample AE *********************\n");
    
    /* do something ... like check the sensor id, init the global variables, and so on */

    /* Commonly, create a virtual regs to communicate, also user can design the new commuincation style. */
    s32Ret = VReg_Init(AE_LIB_VREG_BASE(u8Id), ALG_LIB_VREG_SIZE);
    if (HI_SUCCESS != s32Ret)
    {
        printf("Ae lib(%d) vreg init failed!\n", s32Handle);
        return s32Ret;
    }

    /* maybe you need to init the virtual regs. */
    SAMPLE_AE_ExtRegsInitialize(s32Handle);

    return HI_SUCCESS;
}



HI_S32 SAMPLE_AE_Run(HI_S32 s32Handle, const ISP_AE_INFO_S *pstAeInfo,
    ISP_AE_RESULT_S *pstAeResult, HI_S32 s32Rsv)
{
    SAMPLE_AE_CTX_S *pstAeCtx = HI_NULL;
    
    AE_CHECK_HANDLE_ID(s32Handle);
    pstAeCtx = AE_GET_CTX(s32Handle);

    AE_CHECK_POINTER(pstAeInfo);
    AE_CHECK_POINTER(pstAeResult);

    AE_CHECK_POINTER(pstAeInfo->pstAeStat1);
    AE_CHECK_POINTER(pstAeInfo->pstAeStat2);

    //printf("********************* AeRun Sample AE *********************\n");
    
    pstAeCtx->u32FrameCnt = pstAeInfo->u32FrameCnt;

    /* init isp regs in the first frame. the regs in stStatAttr may need to be...
     * ...configed a few times. 
     */
     
    if (1 == pstAeCtx->u32FrameCnt)
    {
        SAMPLE_AE_IspRegsInit(s32Handle);
    }

    /* do AE alg per 2 frame */
    
    if (0 == pstAeCtx->u32FrameCnt % 2)
    {
        /* record the statistics in pstAeInfo, and then use the statistics...
         * ...to calculate, no need to call any other api 
         */

        memcpy(&pstAeCtx->stAeInfo, pstAeInfo, sizeof(ISP_AE_INFO_S));

        /* maybe need to read the virtual regs to check whether...
         * ...someone changes the configs. 
         */
        SAMPLE_AE_ReadExtRegs(s32Handle);
               
        SAMPLE_AE_Calculate(s32Handle);

        /* pls fill the result after calculate, the firmware will config...
         * ...the regs for awb algorithm. 
         */
         
        pstAeCtx->stAeResult.u32Iso = 0x100;
        pstAeCtx->stAeResult.u32IspDgain = 0x100;

        /* maybe need to write some configs to the virtual regs. */
        SAMPLE_AE_UpdateExtRegs(s32Handle);

        /* maybe need to call the sensor's register functions to config sensor */

        // update sensor gains
        if (HI_NULL != pstAeCtx->stSnsRegister.stSnsExp.pfn_cmos_gains_update)
        {
            HI_U32 u32Again = 0;
            HI_U32 u32Dgain = 0;
            
            pstAeCtx->stSnsRegister.stSnsExp.pfn_cmos_gains_update(u32Again, u32Dgain);
        }

        // update sensor exposure time etc.
        
    }
    
    /* record result */    
    memcpy(pstAeResult, &pstAeCtx->stAeResult, sizeof(ISP_AE_RESULT_S));
    
    pstAeCtx->stAeResult.stStatAttr.bChange = HI_FALSE;
    
    return HI_SUCCESS;
}

HI_S32 SAMPLE_AE_Ctrl(HI_S32 s32Handle, HI_U32 u32Cmd, HI_VOID *pValue)
{
    SAMPLE_AE_CTX_S *pstAeCtx = HI_NULL;

    // avoid warning
    pstAeCtx = pstAeCtx;
    
    AE_CHECK_HANDLE_ID(s32Handle);
    pstAeCtx = AE_GET_CTX(s32Handle);
    
    AE_CHECK_POINTER(pValue);
    
    switch (u32Cmd)
    {
        /* system ctrl */
        case ISP_WDR_MODE_SET :
            /* do something ... */
            break;
        case ISP_AE_FPS_BASE_SET :
            /* do something ... */
            break;
        /* ae ctrl, define the customer's ctrl cmd, if needed ... */
        default :
            break;
    }

    return HI_SUCCESS;
}

HI_S32 SAMPLE_AE_Exit(HI_S32 s32Handle)
{
    HI_S32 s32Ret;
    SAMPLE_AE_CTX_S *pstAeCtx = HI_NULL;
    HI_U8 u8Id;

    AE_CHECK_HANDLE_ID(s32Handle);
    pstAeCtx = AE_GET_CTX(s32Handle);
    u8Id = AE_GET_EXTREG_ID(s32Handle);

    // avoid warning
    pstAeCtx = pstAeCtx;
    
    /* do something ... */

    /* if created the virtual regs, need to destory virtual regs. */
    s32Ret = VReg_Exit(AE_LIB_VREG_BASE(u8Id), ALG_LIB_VREG_SIZE);
    if (HI_SUCCESS != s32Ret)
    {
        printf("Ae lib(%d) vreg exit failed!\n", s32Handle);
        return s32Ret;
    }

    return HI_SUCCESS;
}

HI_S32 SAMPLE_HI_MPI_AE_Register(ISP_DEV IspDev, ALG_LIB_S *pstAeLib)
{
    ISP_AE_REGISTER_S stRegister;
    HI_S32 s32Ret = HI_SUCCESS;
    
    AE_CHECK_DEV(IspDev);
    AE_CHECK_POINTER(pstAeLib);
    AE_CHECK_HANDLE_ID(pstAeLib->s32Id);
    AE_CHECK_LIB_NAME(pstAeLib->acLibName);


    stRegister.stAeExpFunc.pfn_ae_init  = SAMPLE_AE_Init;
    stRegister.stAeExpFunc.pfn_ae_run   = SAMPLE_AE_Run;
    stRegister.stAeExpFunc.pfn_ae_ctrl  = SAMPLE_AE_Ctrl;
    stRegister.stAeExpFunc.pfn_ae_exit  = SAMPLE_AE_Exit;
    s32Ret = HI_MPI_ISP_AELibRegCallBack(IspDev, pstAeLib, &stRegister);
    if (HI_SUCCESS != s32Ret)
    {
        printf("Hi_ae register failed!\n");
    }    
        
    return s32Ret;
}

HI_S32 SAMPLE_HI_MPI_AE_UnRegister(ISP_DEV IspDev, ALG_LIB_S *pstAeLib)
{
    HI_S32 s32Ret = HI_SUCCESS;

    AE_CHECK_DEV(IspDev);
    AE_CHECK_POINTER(pstAeLib);
    AE_CHECK_HANDLE_ID(pstAeLib->s32Id);
    AE_CHECK_LIB_NAME(pstAeLib->acLibName);
    
    s32Ret = HI_MPI_ISP_AELibUnRegCallBack(IspDev, pstAeLib);
    if (HI_SUCCESS != s32Ret)
    {
        printf("Hi_ae unregister failed!\n");
    }
    
    return s32Ret;
}

HI_S32 SAMPLE_HI_MPI_AE_SensorRegCallBack(ISP_DEV IspDev, ALG_LIB_S *pstAeLib, SENSOR_ID SensorId,
    AE_SENSOR_REGISTER_S *pstRegister)
{
    SAMPLE_AE_CTX_S *pstAeCtx = HI_NULL;
    HI_S32  s32Handle;

    AE_CHECK_DEV(IspDev);
    AE_CHECK_POINTER(pstAeLib);
    AE_CHECK_POINTER(pstRegister);
    
    s32Handle = pstAeLib->s32Id;
    AE_CHECK_HANDLE_ID(s32Handle);
    AE_CHECK_LIB_NAME(pstAeLib->acLibName);

    pstAeCtx = AE_GET_CTX(s32Handle);

    /* get default value */
    if (HI_NULL != pstRegister->stSnsExp.pfn_cmos_get_ae_default)
    {
        pstRegister->stSnsExp.pfn_cmos_get_ae_default(&pstAeCtx->stSnsDft);
    }

    /* record information */
    memcpy(&pstAeCtx->stSnsRegister, pstRegister, sizeof(AE_SENSOR_REGISTER_S));
    pstAeCtx->SensorId = SensorId;
    
    pstAeCtx->bSnsRegister = HI_TRUE;

    return HI_SUCCESS;
}

HI_S32 SAMPLE_HI_MPI_AE_SensorUnRegCallBack(ISP_DEV IspDev, ALG_LIB_S *pstAeLib, SENSOR_ID SensorId)
{
    SAMPLE_AE_CTX_S *pstAeCtx = HI_NULL;
    HI_S32  s32Handle;

    AE_CHECK_DEV(IspDev);
    AE_CHECK_POINTER(pstAeLib);
    
    s32Handle = pstAeLib->s32Id;
    AE_CHECK_HANDLE_ID(s32Handle);
    AE_CHECK_LIB_NAME(pstAeLib->acLibName);

    pstAeCtx = AE_GET_CTX(s32Handle);

    memset(&pstAeCtx->stSnsDft, 0, sizeof(AE_SENSOR_DEFAULT_S));
    memset(&pstAeCtx->stSnsRegister, 0, sizeof(AE_SENSOR_REGISTER_S));
    pstAeCtx->SensorId = 0;    
    pstAeCtx->bSnsRegister = HI_FALSE;

    return HI_SUCCESS;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

