/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : mpi_isp.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2011/01/05
  Description   : 
  History       :
  1.Date        : 2011/01/05
    Author      : n00168968
    Modification: Created file

******************************************************************************/

#include <stdio.h>
#include <string.h>
#if 0
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif
#include <sys/ioctl.h>
#include <signal.h>

#include "mpi_sys.h"
#include "mkp_isp.h"
#include "hi_isp_debug.h"
#include "isp_debug.h"
#include "isp_defaults.h"
#include "isp_main.h"
#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_statistics.h"
#include "isp_regcfg.h"
#include "isp_proc.h"
#include "hi_vreg.h"
#include "isp_config.h"
#include "isp_ext_config.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

/* Isp Version Proc will be Hi3518_ISP_V1.0.4.x, ISP_LIB_VERSION stands for x */
#define ISP_LIB_VERSION "0"    //[0~F]

/****************************************************************************
 * GLOBAL VARIABLES                                                         *
 ****************************************************************************/

ISP_CTX_S  g_astIspCtx[ISP_MAX_DEV_NUM] = {{0}};
HI_S32     g_as32IspFd[ISP_MAX_DEV_NUM] = {-1};


static ISP_VERSION_S gs_stVersion = 
{
    .au8MppVersion = ISP_LIB_VERSION,
    .au8Date = __DATE__,
    .au8Time = __TIME__,
    .u32Magic = 0,
};

/****************************************************************************
 * MACRO DEFINITION                                                         *
 ****************************************************************************/

#define ISP_CHECK_SENSOR_REGISTER(dev)\
do {\
    if (g_astIspCtx[dev].bSnsReg != HI_TRUE)\
    {\
        ISP_TRACE(HI_DBG_ERR, "Sensor doesn't register to ISP[%d]!\n", dev);\
        return HI_ERR_ISP_SNS_UNREGISTER;\
    }\
}while(0)

#define ISP_VERSION_MAGIC    20130305

/****************************************************************************
 * static functions 
 ****************************************************************************/
static HI_BOOL ISP_GetVersion(HI_VOID)
{
    HI_S32 s32Ret;
    ISP_DEV IspDev = 0;
    
    ISP_CHECK_OPEN(IspDev);

    s32Ret = ioctl(g_as32IspFd[IspDev], ISP_GET_VERSION, &gs_stVersion);
    if (s32Ret)
    {
        ISP_TRACE(HI_DBG_ERR, "register ISP[%d] lib info ec %x!\n", IspDev, s32Ret);
        return HI_FALSE;
    }
    
    /* 20130314, disable isp magic version check */
    #if 0
    if (gs_stVersion.u32Magic != ISP_VERSION_MAGIC)
    {
        printf("isp lib version doesn't match with sdk!\n");
        return HI_FALSE;
    }
    #endif

    return HI_TRUE;
}


/*****************************************************************************
 Prototype       : HI_MPI_ISP_MemInit
 Description     : isp initial extent memory
 Input           : I_VOID  **
 Output          : None
 Return Value    : 
 Process         : 
 Note            : 

  History         
  1.Date         : 2014/8/13
    Author       : p00272155
    Modification : Created function

*****************************************************************************/
HI_S32 HI_MPI_ISP_MemInit(ISP_DEV IspDev)
{
    HI_S32 s32Ret;
    ISP_CTX_S *pstIspCtx = HI_NULL;

    /* 1. check status */
    ISP_CHECK_DEV(IspDev);
    ISP_GET_CTX(IspDev, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);
    ISP_CHECK_OPEN(IspDev);    
    ISP_CHECK_SENSOR_REGISTER(IspDev);
    
    if (ioctl(g_as32IspFd[IspDev], ISP_MEM_INFO_GET, &pstIspCtx->bMemInit))
    {
        ISP_TRACE(HI_DBG_ERR, "ISP[%d] get Mem info failed!\n", IspDev);
        return HI_ERR_ISP_MEM_NOT_INIT;
    }
    if (HI_TRUE == pstIspCtx->bMemInit)
    {
#if 1   /* support re-init */
        VReg_Exit(ISP_VREG_BASE, ISP_VREG_SIZE);;
        pstIspCtx->bMemInit = HI_FALSE;
        if (ioctl(g_as32IspFd[IspDev], ISP_MEM_INFO_SET, &pstIspCtx->bMemInit))
        {
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
#else   /* NOT support re-init */
        ISP_TRACE(HI_DBG_ERR, "ISP[%d] MemInit failed!\n", IspDev);
        return HI_ERR_ISP_ILLEGAL_PARAM;
#endif
    }

    if (HI_TRUE != ISP_GetVersion())
    {
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    /* 2. creat extern registers */
    s32Ret = VReg_Init(ISP_VREG_BASE, ISP_VREG_SIZE);
    if (HI_SUCCESS != s32Ret)
    {
        ISP_TRACE(HI_DBG_ERR, "ISP[%d] init Mem failed\n", IspDev);
        return s32Ret;
    }

    pstIspCtx->bMemInit = HI_TRUE;
    s32Ret = ioctl(g_as32IspFd[IspDev], ISP_MEM_INFO_SET, &pstIspCtx->bMemInit);
    if (s32Ret)
    {
        pstIspCtx->bMemInit = HI_FALSE;
        VReg_Exit(ISP_VREG_BASE, ISP_VREG_SIZE);
        
        ISP_TRACE(HI_DBG_ERR, "ISP[%d] set Mem info failed!\n", IspDev);
        return s32Ret;
    }

    memset(&pstIspCtx->stIspParaRec, 0, sizeof(ISP_PARA_REC_S));
    hi_ext_top_wdr_cfg_write(pstIspCtx->stIspParaRec.bWDRCfg);
    hi_ext_top_pub_attr_cfg_write(pstIspCtx->stIspParaRec.bPubCfg);

    hi_ext_top_wdr_switch_write(HI_FALSE);
    hi_ext_top_res_switch_write(HI_FALSE);

    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype       : HI_MPI_ISP_Init
 Description     : isp initial process, include extent memory, top structure, 
                    default value, etc.
 Input           : I_VOID  **
 Output          : None
 Return Value    : 
 Process         : 
 Note             : 

  History         
  1.Date         : 2011/1/13
    Author       : x00100808
    Modification : Created function

*****************************************************************************/
HI_S32 HI_MPI_ISP_Init(ISP_DEV IspDev)
{
    HI_S32 s32Ret;
    ISP_CTX_S *pstIspCtx = HI_NULL;

    HI_U8 u8WDRMode;
    HI_U32 u32Value = 0;
    HI_VOID *pValue = HI_NULL;
    ISP_CMOS_SENSOR_IMAGE_MODE_S stSnsImageMode;

    /* 1. check status */
    ISP_CHECK_DEV(IspDev);
    ISP_GET_CTX(IspDev, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);
    ISP_CHECK_OPEN(IspDev);    
    ISP_CHECK_SENSOR_REGISTER(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    pstIspCtx->stIspParaRec.bWDRCfg = hi_ext_top_wdr_cfg_read();
    ISP_CHECK_ISP_WDR_CFG(IspDev);
    
    pstIspCtx->stIspParaRec.bPubCfg = hi_ext_top_pub_attr_cfg_read();
    ISP_CHECK_ISP_PUB_ATTR_CFG(IspDev);

    if (HI_TRUE == pstIspCtx->stIspParaRec.bInit)
    {
        ISP_TRACE(HI_DBG_ERR, "ISP[%d] Init failed!\n", IspDev);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    /* 2. init statistics bufs. */
    s32Ret = ISP_StatisticsInit(IspDev);
    if (HI_SUCCESS != s32Ret)
    {
        ISP_TRACE(HI_DBG_ERR, "ISP[%d] Init statistics failed\n", IspDev);
        goto fail0;
    }

    /* 3. init proc bufs. */
    s32Ret = ISP_ProcInit(IspDev);
    if (HI_SUCCESS != s32Ret)
    {
        goto fail1;
    }

#ifdef ENABLE_JPEGEDCF
    /* 4. init dcfinfo bufs. */
    s32Ret = ISP_DCFInfoInit(IspDev, &pstIspCtx->stDcfInfoCtrl);
    if (HI_SUCCESS != s32Ret)
    {
        goto fail2;
    }
#endif

    /* 5. set WDR mode to sensor and update isp param from sensor and sensor init */
    u8WDRMode = hi_ext_system_sensor_wdr_mode_read();
    s32Ret = ISP_SensorSetWDRMode(IspDev, u8WDRMode);
    if (HI_SUCCESS != s32Ret)
    {
        ISP_TRACE(HI_DBG_ERR, "ISP[%d] set sensor WDR mode failed!\n", IspDev);
        s32Ret = HI_ERR_ISP_NOT_INIT;
        goto fail3;
    }

    stSnsImageMode.u16Width = hi_isp_top_active_width_read();
    stSnsImageMode.u16Height = hi_isp_top_active_height_read();
    u32Value = hi_ext_system_fps_base_read();
    pValue = (HI_VOID *)&u32Value;
    stSnsImageMode.f32Fps = *(HI_FLOAT *)pValue;
    s32Ret = ISP_SensorSetImageMode(IspDev, &stSnsImageMode);
    if (HI_SUCCESS != s32Ret)
    {
        ISP_TRACE(HI_DBG_ERR, "ISP[%d] set sensor image mode failed!\n", IspDev);
        s32Ret = HI_ERR_ISP_NOT_INIT;
        goto fail3;
    }

    s32Ret = ISP_SensorInit(IspDev);
    if (HI_SUCCESS != s32Ret)
    {
        ISP_TRACE(HI_DBG_ERR, "ISP[%d] init sensor failed!\n", IspDev);
        s32Ret = HI_ERR_ISP_NOT_INIT;
        goto fail3;
    }

    s32Ret = ISP_SensorUpdateAll(IspDev);
    if (HI_SUCCESS != s32Ret)
    {
        ISP_TRACE(HI_DBG_ERR, "ISP[%d] update sensor all failed!\n", IspDev);
        s32Ret = HI_ERR_ISP_NOT_INIT;
        goto fail3;
    }
    
    /* 6. init the common part of extern registers and real registers */
    ISP_ExtRegsDefault();
    ISP_RegsDefault();
    ISP_ExtRegsInitialize(IspDev);
    ISP_RegsInitialize(IspDev);
    
    /* 7. isp algs global variable initialize */
    ISP_GlobalInitialize(IspDev);

    /* 8. register all algorithm to isp, and init them. */
    ISP_AlgsRegister(IspDev);
    ISP_AlgsInit(pstIspCtx->astAlgs, IspDev);
    
    /* 9. set WDR mode to kernel. */
    s32Ret = ISP_WDRCfgSet(IspDev);
    if (HI_SUCCESS != s32Ret)
    {
        ISP_TRACE(HI_DBG_ERR, "ISP[%d] set WDR mode to kernel failed!\n", IspDev);
        goto fail3;
    }

    /* 10. init isp global variables */
    if (pthread_mutex_init(&pstIspCtx->stLock, HI_NULL) != 0)
    {
        s32Ret = HI_FAILURE;
        goto fail3;
    }
    
    pthread_mutex_lock(&pstIspCtx->stLock);
    pstIspCtx->stIspParaRec.bInit = HI_TRUE;
    pstIspCtx->stIspParaRec.bRunEn = HI_TRUE;
    pthread_mutex_unlock(&pstIspCtx->stLock);

    return HI_SUCCESS;
fail3:  
#ifdef ENABLE_JPEGEDCF
    ISP_DCFInfoExit(IspDev, &pstIspCtx->stDcfInfoCtrl);

fail2:
#endif
    ISP_ProcExit(IspDev);
fail1:
    ISP_StatisticsExit(IspDev);
fail0:
    //VReg_Exit(ISP_VREG_BASE, ISP_VREG_SIZE);
    
    return s32Ret;
}

/*****************************************************************************
 Prototype       : HI_MPI_ISP_Run
 Description     : isp firmware recurrent task, always run in a single thread.
 Input           : I_VOID  **
 Output          : None
 Return Value    : 
 Process         : 
 Note             : 

  History         
  1.Date         : 2011/1/13
    Author       : x00100808
    Modification : Created function

*****************************************************************************/
HI_S32 HI_MPI_ISP_Run(ISP_DEV IspDev)
{
    HI_S32 s32Ret;
    HI_U32 u32IntStatus = 0;
    HI_BOOL bEn;
    HI_U32 u32WDRmode;
    ISP_CTX_S *pstIspCtx = HI_NULL;

    /* 1. check status */
    ISP_CHECK_DEV(IspDev);
    ISP_GET_CTX(IspDev, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);    
    ISP_CHECK_OPEN(IspDev);
    ISP_CHECK_SENSOR_REGISTER(IspDev);
    ISP_CHECK_MEM_INIT(IspDev);

    ISP_CHECK_ISP_INIT(IspDev);

    if (HI_TRUE == pstIspCtx->stIspParaRec.bRun)
    {
        ISP_TRACE(HI_DBG_ERR, "ISP[%d] Run failed!\n", IspDev);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    pthread_mutex_lock(&pstIspCtx->stLock);

    /* Sometimes HI_MPI_ISP_Run thread is not scheduled to run before calling HI_MPI_ISP_Exit. */
    if (HI_FALSE == pstIspCtx->stIspParaRec.bRunEn)
    {
        pthread_mutex_unlock(&pstIspCtx->stLock);
        return HI_SUCCESS;
    }

    /* 2. enable interrupt */
    bEn = HI_TRUE;
    if (ioctl(g_as32IspFd[IspDev], ISP_SET_INT_ENABLE, &bEn) < 0)
    {
        ISP_TRACE(HI_DBG_ERR, "Enable ISP[%d] interrupt failed!\n", IspDev);
        pthread_mutex_unlock(&pstIspCtx->stLock);
        return -1;
    }

    pstIspCtx->stIspParaRec.bRun = HI_TRUE;
    pthread_mutex_unlock(&pstIspCtx->stLock);

    while (1)
    {
        pthread_mutex_lock(&pstIspCtx->stLock);
        if (HI_FALSE == pstIspCtx->stIspParaRec.bRunEn)
        {
            pthread_mutex_unlock(&pstIspCtx->stLock);
            break;
        }
        
       /*change  resolution  */
        ISP_SwitchImageMode(IspDev);

        u32WDRmode = hi_ext_system_sensor_wdr_mode_read();
		
		
        /* swtich linear/WDR mode, width/height, fps  */
        if (pstIspCtx->u8SnsWDRMode != u32WDRmode)
        {
            pstIspCtx->u8SnsWDRMode = u32WDRmode;
            ISP_SwitchWDRMode(IspDev);
        }

        {
            u32IntStatus = 0;
            /* 3. waked up by the interrupt */
            s32Ret = ioctl(g_as32IspFd[IspDev], ISP_GET_FRAME_EDGE, &u32IntStatus);
            if (s32Ret)
            {

            }
            else
            {
                /* 4.isp firmware calculate, include AE/AWB, etc. */
                if (ISP_1ST_INT & u32IntStatus)
                {
                    ISP_Run(IspDev);
                }
            }
        }

        pthread_mutex_unlock(&pstIspCtx->stLock);
        usleep(10);
    }

    /* 8. disable interrupt */
    bEn = HI_FALSE;
    if (ioctl(g_as32IspFd[IspDev], ISP_SET_INT_ENABLE, &bEn) < 0)
    {
        ISP_TRACE(HI_DBG_ERR, "Disable ISP[%d] interrupt failed!\n", IspDev);
    }

    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype       : HI_MPI_ISP_Exit
 Description     : control isp to exit recurrent task, always run in main process.
 Input           : I_VOID  **
 Output          : None
 Return Value    : 
 Process         : 
 Note             : 

  History         
  1.Date         : 2011/1/13
    Author       : x00100808
    Modification : Created function

*****************************************************************************/
HI_S32 HI_MPI_ISP_Exit(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    
    /* 1. Check status */
    ISP_CHECK_DEV(IspDev);
    ISP_GET_CTX(IspDev, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);    
    ISP_CHECK_OPEN(IspDev);
    //ISP_CHECK_MEM_INIT(IspDev);

    /* 2. exit global variables */
    pthread_mutex_lock(&pstIspCtx->stLock);
    memset(&pstIspCtx->stIspParaRec, 0, sizeof(ISP_PARA_REC_S));
    hi_ext_top_wdr_cfg_write(pstIspCtx->stIspParaRec.bWDRCfg);
    hi_ext_top_pub_attr_cfg_write(pstIspCtx->stIspParaRec.bPubCfg);

    /* 3. exit algorthmics */
    ISP_AlgsExit(pstIspCtx->astAlgs, IspDev);

    /* 4. unregister algorthmics, 3a libs */
    ISP_AlgsUnRegister(IspDev);
    ISP_LibsUnRegister(IspDev);

    /* 5. sensor exit */
    ISP_SensorExit(IspDev);

    /* 6. release proc bufs. */
    ISP_ProcExit(IspDev);

#ifdef ENABLE_JPEGEDCF
    /* 7. exit dcf bufs. */
    ISP_DCFInfoExit(IspDev, &pstIspCtx->stDcfInfoCtrl);
#endif

    /* 7. exit statistics bufs. */
    ISP_StatisticsExit(IspDev);

    /* 8. release vregs */
    VReg_Munmap();
    VReg_Exit(ISP_VREG_BASE, ISP_VREG_SIZE);

    pthread_mutex_unlock(&pstIspCtx->stLock);

    pstIspCtx->bMemInit = HI_FALSE;
    if (ioctl(g_as32IspFd[IspDev], ISP_MEM_INFO_SET, &pstIspCtx->bMemInit))
    {
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype       : HI_MPI_ISP_SetRegister
 Description     : set isp register, include extent memory. 
 Input           : u32Addr   **
                   u32Value  **
 Output          : None
 Return Value    : 
 Process         : 
 Note             : 

  History         
  1.Date         : 2011/1/13
    Author       : x00100808
    Modification : Created function

*****************************************************************************/
HI_S32 HI_MPI_ISP_SetRegister(ISP_DEV IspDev, HI_U32 u32Addr, HI_U32 u32Value)
{
    ISP_CHECK_DEV(IspDev);
    IO_WRITE32(u32Addr, u32Value);

    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype       : HI_MPI_ISP_GetRegister
 Description     : get isp register, include extent memory.
 Input           : u32Addr    **
                   pu32Value  **
 Output          : None
 Return Value    : 
 Process         : 
 Note             : 

  History         
  1.Date         : 2011/1/13
    Author       : x00100808
    Modification : Created function

*****************************************************************************/
HI_S32 HI_MPI_ISP_GetRegister(ISP_DEV IspDev, HI_U32 u32Addr, HI_U32 *pu32Value)
{
    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pu32Value);
    *pu32Value = IO_READ32(u32Addr);

    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype       : HI_MPI_ISP_SensorRegister
 Description     : sensor register callback function to isp firmware
 Input           : pstIspSensorRegister  **
 Output          : None
 Return Value    : 
 Process         : 
 Note             : 

  History         
  1.Date         : 2011/1/21
    Author       : x00100808
    Modification : Created function

*****************************************************************************/
HI_S32 HI_MPI_ISP_SensorRegCallBack(ISP_DEV IspDev, SENSOR_ID SensorId, ISP_SENSOR_REGISTER_S *pstRegister)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;

    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_POINTER(pstRegister);

    ISP_GET_CTX(IspDev, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);

    if (HI_TRUE == pstIspCtx->bSnsReg)
    {
        ISP_TRACE(HI_DBG_ERR, "Reg ERR! Sensor have registered to ISP[%d]!\n", IspDev);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    ISP_SensorRegCallBack(IspDev, SensorId, pstRegister);

    pstIspCtx->stBindAttr.SensorId = SensorId;
    pstIspCtx->bSnsReg = HI_TRUE;
    
    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_AELibRegCallBack(ISP_DEV IspDev, ALG_LIB_S *pstAeLib,
        ISP_AE_REGISTER_S *pstRegister)
{
    HI_S32 s32Ret = HI_SUCCESS;
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_LIB_NODE_S *pstAeLibNode = HI_NULL;

    ISP_CHECK_DEV(IspDev);
    ISP_GET_CTX(IspDev, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);

    /* check null point */
    ISP_CHECK_POINTER(pstAeLib);
    ISP_CHECK_POINTER(pstRegister);

    ISP_CHECK_POINTER(pstRegister->stAeExpFunc.pfn_ae_init);
    ISP_CHECK_POINTER(pstRegister->stAeExpFunc.pfn_ae_run);
    ISP_CHECK_POINTER(pstRegister->stAeExpFunc.pfn_ae_ctrl);
    ISP_CHECK_POINTER(pstRegister->stAeExpFunc.pfn_ae_exit);

    /* whether the lib have been registered */
    s32Ret = ISP_FindLib(pstIspCtx->stAeLibInfo.astLibs, pstAeLib);
    if (-1 != s32Ret)
    {
        ISP_TRACE(HI_DBG_ERR, "Reg ERR! aelib have registered to ISP[%d].\n", IspDev);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    /* whether can we register a new lib  */
    pstAeLibNode = ISP_SearchLib(pstIspCtx->stAeLibInfo.astLibs, pstAeLib);
    if (HI_NULL == pstAeLibNode)
    {
        ISP_TRACE(HI_DBG_ERR, "can't register aelib to ISP[%d], there is too many libs.\n", IspDev);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    /* record register info */
    memcpy(&pstAeLibNode->stAlgLib, pstAeLib, sizeof(ALG_LIB_S));
    memcpy(&pstAeLibNode->stAeRegsiter, pstRegister, sizeof(ISP_AE_REGISTER_S));
    pstAeLibNode->bUsed = HI_TRUE;

    /* set active lib */
    pstIspCtx->stAeLibInfo.u32ActiveLib = ISP_FindLib(pstIspCtx->stAeLibInfo.astLibs, pstAeLib);
    memcpy(&pstIspCtx->stBindAttr.stAeLib, pstAeLib, sizeof(ALG_LIB_S));

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_AWBLibRegCallBack(ISP_DEV IspDev, ALG_LIB_S *pstAwbLib,
        ISP_AWB_REGISTER_S *pstRegister)
{
    HI_S32 s32Ret = HI_SUCCESS;
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_LIB_NODE_S *pstAwbLibNode = HI_NULL;

    ISP_CHECK_DEV(IspDev);
    ISP_GET_CTX(IspDev, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);

    /* check null point */
    ISP_CHECK_POINTER(pstAwbLib);
    ISP_CHECK_POINTER(pstRegister);

    ISP_CHECK_POINTER(pstRegister->stAwbExpFunc.pfn_awb_init);
    ISP_CHECK_POINTER(pstRegister->stAwbExpFunc.pfn_awb_run);
    ISP_CHECK_POINTER(pstRegister->stAwbExpFunc.pfn_awb_ctrl);
    ISP_CHECK_POINTER(pstRegister->stAwbExpFunc.pfn_awb_exit);

    /* whether the lib have been registered */
    s32Ret = ISP_FindLib(pstIspCtx->stAwbLibInfo.astLibs, pstAwbLib);
    if (-1 != s32Ret)
    {
        ISP_TRACE(HI_DBG_ERR, "Reg ERR! awblib have registered to ISP[%d].\n", IspDev);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    /* whether can we register a new lib  */
    pstAwbLibNode = ISP_SearchLib(pstIspCtx->stAwbLibInfo.astLibs, pstAwbLib);
    if (HI_NULL == pstAwbLibNode)
    {        
        ISP_TRACE(HI_DBG_ERR, "can't register awblib to ISP[%d], there is too many libs.\n", IspDev);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    /* record register info */
    memcpy(&pstAwbLibNode->stAlgLib, pstAwbLib, sizeof(ALG_LIB_S));
    memcpy(&pstAwbLibNode->stAwbRegsiter, pstRegister, sizeof(ISP_AWB_REGISTER_S));
    pstAwbLibNode->bUsed = HI_TRUE;

    /* set active lib */
    pstIspCtx->stAwbLibInfo.u32ActiveLib = ISP_FindLib(pstIspCtx->stAwbLibInfo.astLibs, pstAwbLib);
    memcpy(&pstIspCtx->stBindAttr.stAwbLib, pstAwbLib, sizeof(ALG_LIB_S));

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_AFLibRegCallBack(ISP_DEV IspDev, ALG_LIB_S *pstAfLib,
        ISP_AF_REGISTER_S *pstRegister)
{
    HI_S32 s32Ret = HI_SUCCESS;
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_LIB_NODE_S *pstAfLibNode = HI_NULL;

    ISP_CHECK_DEV(IspDev);
    ISP_GET_CTX(IspDev, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);

    /* check null point */
    ISP_CHECK_POINTER(pstAfLib);
    ISP_CHECK_POINTER(pstRegister);

    ISP_CHECK_POINTER(pstRegister->stAfExpFunc.pfn_af_init);
    ISP_CHECK_POINTER(pstRegister->stAfExpFunc.pfn_af_run);
    ISP_CHECK_POINTER(pstRegister->stAfExpFunc.pfn_af_ctrl);
    ISP_CHECK_POINTER(pstRegister->stAfExpFunc.pfn_af_exit);

    /* whether the lib have been registered */
    s32Ret = ISP_FindLib(pstIspCtx->stAfLibInfo.astLibs, pstAfLib);
    if (-1 != s32Ret)
    {
        ISP_TRACE(HI_DBG_ERR, "Reg ERR! aflib have registered to ISP[%d].\n", IspDev);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    /* whether can we register a new lib  */
    pstAfLibNode = ISP_SearchLib(pstIspCtx->stAfLibInfo.astLibs, pstAfLib);
    if (HI_NULL == pstAfLibNode)
    {        
        ISP_TRACE(HI_DBG_ERR, "can't register aflib to ISP[%d], there is too many libs.\n", IspDev);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    /* record register info */
    memcpy(&pstAfLibNode->stAlgLib, pstAfLib, sizeof(ALG_LIB_S));
    memcpy(&pstAfLibNode->stAfRegsiter, pstRegister, sizeof(ISP_AF_REGISTER_S));
    pstAfLibNode->bUsed = HI_TRUE;

    /* set active lib */
    pstIspCtx->stAfLibInfo.u32ActiveLib = ISP_FindLib(pstIspCtx->stAfLibInfo.astLibs, pstAfLib);
    memcpy(&pstIspCtx->stBindAttr.stAfLib, pstAfLib, sizeof(ALG_LIB_S));

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SensorUnRegCallBack(ISP_DEV IspDev, SENSOR_ID SensorId)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;

    ISP_CHECK_DEV(IspDev);
    ISP_GET_CTX(IspDev, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);
    ISP_CHECK_SENSOR_REGISTER(IspDev);

    /* check sensor id */
    if (pstIspCtx->stBindAttr.SensorId != SensorId)
    {
        ISP_TRACE(HI_DBG_ERR, "UnReg ERR! ISP[%d] Registered sensor is %d, present sensor is %d.\n",
            IspDev, pstIspCtx->stBindAttr.SensorId, SensorId);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    pstIspCtx->stBindAttr.SensorId = 0;
    pstIspCtx->bSnsReg = HI_FALSE;
    
    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_AELibUnRegCallBack(ISP_DEV IspDev, ALG_LIB_S *pstAeLib)
{
    HI_S32 s32SearchId;
    ISP_CTX_S *pstIspCtx = HI_NULL;

    ISP_CHECK_DEV(IspDev);
    ISP_GET_CTX(IspDev, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);

    /* check null point */
    ISP_CHECK_POINTER(pstAeLib);

    s32SearchId = ISP_FindLib(pstIspCtx->stAeLibInfo.astLibs, pstAeLib);
    if (-1 == s32SearchId)
    {
        ISP_TRACE(HI_DBG_ERR, "can't find ae lib in ISP[%d].\n", IspDev);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }


    memset(&pstIspCtx->stAeLibInfo.astLibs[s32SearchId], 0, sizeof(ISP_LIB_NODE_S));

    /* set active lib */
    pstIspCtx->stAeLibInfo.u32ActiveLib = 0;

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_AWBLibUnRegCallBack(ISP_DEV IspDev, ALG_LIB_S *pstAwbLib)
{
    HI_S32 s32SearchId;
    ISP_CTX_S *pstIspCtx = HI_NULL;

    ISP_CHECK_DEV(IspDev);
    ISP_GET_CTX(IspDev, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);

    /* check null point */
    ISP_CHECK_POINTER(pstAwbLib);

    s32SearchId = ISP_FindLib(pstIspCtx->stAwbLibInfo.astLibs, pstAwbLib);
    if (-1 == s32SearchId)
    {
        ISP_TRACE(HI_DBG_ERR, "can't find awb lib in ISP[%d].\n", IspDev);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    memset(&pstIspCtx->stAwbLibInfo.astLibs[s32SearchId], 0, sizeof(ISP_LIB_NODE_S));

    /* set active lib */
    pstIspCtx->stAwbLibInfo.u32ActiveLib = 0;

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_AFLibUnRegCallBack(ISP_DEV IspDev, ALG_LIB_S *pstAfLib)
{
    HI_S32 s32SearchId;
    ISP_CTX_S *pstIspCtx = HI_NULL;

    ISP_CHECK_DEV(IspDev);
    ISP_GET_CTX(IspDev, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);

    /* check null point */
    ISP_CHECK_POINTER(pstAfLib);

    s32SearchId = ISP_FindLib(pstIspCtx->stAfLibInfo.astLibs, pstAfLib);
    if (-1 == s32SearchId)
    {
        ISP_TRACE(HI_DBG_ERR, "can't find af lib in ISP[%d].\n", IspDev);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    memset(&pstIspCtx->stAfLibInfo.astLibs[s32SearchId], 0, sizeof(ISP_LIB_NODE_S));

    /* set active lib */
    pstIspCtx->stAfLibInfo.u32ActiveLib = 0;

    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_SetBindAttr(ISP_DEV IspDev, const ISP_BIND_ATTR_S *pstBindAttr)
{
    SENSOR_ID SensorId;
    HI_S32    s32SearchId;
    ISP_CTX_S *pstIspCtx = HI_NULL;

    ISP_CHECK_DEV(IspDev);
    ISP_GET_CTX(IspDev, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);
    
    ISP_CHECK_POINTER(pstBindAttr);

    ISP_SensorGetId(IspDev, &SensorId);
    /* check sensor id */
    if (pstBindAttr->SensorId != SensorId)
    {
        ISP_TRACE(HI_DBG_ERR, "ISP[%d] Register sensor is %d, present sensor is %d.\n",
            IspDev, SensorId, pstBindAttr->SensorId);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    /* check ae lib */
    s32SearchId = ISP_FindLib(pstIspCtx->stAeLibInfo.astLibs, &pstBindAttr->stAeLib);
    if (-1 != s32SearchId)
    {
        pstIspCtx->stAeLibInfo.u32ActiveLib = s32SearchId;
    }
    else
    {
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    /* check awb lib */
    s32SearchId = ISP_FindLib(pstIspCtx->stAwbLibInfo.astLibs, &pstBindAttr->stAwbLib);
    if (-1 != s32SearchId)
    {
        pstIspCtx->stAwbLibInfo.u32ActiveLib = s32SearchId;
    }
    else
    {
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    /* check af lib */
    s32SearchId = ISP_FindLib(pstIspCtx->stAfLibInfo.astLibs, &pstBindAttr->stAfLib);
    if (-1 != s32SearchId)
    {
        pstIspCtx->stAfLibInfo.u32ActiveLib = s32SearchId;
    }
    else
    {
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    /* save global variable */
    memcpy(&pstIspCtx->stBindAttr, pstBindAttr, sizeof(ISP_BIND_ATTR_S));
    
    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetBindAttr(ISP_DEV IspDev, ISP_BIND_ATTR_S *pstBindAttr)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;

    ISP_CHECK_DEV(IspDev);
    ISP_GET_CTX(IspDev, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);
    
    ISP_CHECK_POINTER(pstBindAttr);

    /* get global variable */
    memcpy(pstBindAttr, &pstIspCtx->stBindAttr, sizeof(ISP_BIND_ATTR_S));
    
    return HI_SUCCESS;
}

HI_S32 HI_MPI_ISP_GetVDTimeOut(ISP_DEV IspDev, ISP_VD_INFO_S *pstIspVdInfo, HI_U32 u32MilliSec)
{
    ISP_VD_TIMEOUT_S stIspVdTimeOut;
    HI_S32 s32Ret;

    ISP_CHECK_DEV(IspDev);
    ISP_CHECK_OPEN(IspDev);

    stIspVdTimeOut.u32MilliSec = u32MilliSec;
    stIspVdTimeOut.s32IntStatus = 0x0;

    s32Ret = ioctl(g_as32IspFd[IspDev], ISP_GET_VD_TIMEOUT, &stIspVdTimeOut);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    if (HI_NULL != pstIspVdInfo)
    {
        memcpy(pstIspVdInfo, &stIspVdTimeOut.stVdInfo, sizeof(ISP_VD_INFO_S));
    }

    if (0 == (0x1 & stIspVdTimeOut.s32IntStatus))
    {
       return -1;
    }

   return HI_SUCCESS;
}
#ifdef ENABLE_JPEGEDCF
/*****************************************************************************
 Prototype       : HI_MPI_ISP_SetDCFInfo
 Description     : set dcf info to isp firmware
 Input           : ISP_DCF_S *pstIspDCF
 Output          : None
 Return Value    : 
 Process         : 
 Note            : 

  History         
  1.Date         : 2014/6/13
    Author       : z00183560
    Modification : Created function

*****************************************************************************/
HI_S32 HI_MPI_ISP_SetDCFInfo(const ISP_DCF_INFO_S *pstIspDCF)
{
    ISP_DEV IspDev = 0;
    HI_S32 s32Ret;
    ISP_CTX_S *pstIspCtx = HI_NULL;

    ISP_GET_CTX(IspDev, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx->stDcfInfoCtrl.pstIspDCF);
    ISP_CHECK_POINTER(pstIspDCF);
    ISP_CHECK_OPEN(IspDev);

    s32Ret = ioctl(g_as32IspFd[IspDev], ISP_DCF_INFO_SET, pstIspDCF);
    if (s32Ret)
    {
        return s32Ret;
    }

    memcpy(pstIspCtx->stDcfInfoCtrl.pstIspDCF,pstIspDCF,sizeof(ISP_DCF_INFO_S));
    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype       : HI_MPI_ISP_GetDCFInfo
 Description     : get dcf info from isp firmware
 Input           : ISP_DCF_S *pstIspDCF
 Output          : None
 Return Value    : 
 Process         : 
 Note            : 

  History         
  1.Date         : 2014/6/16
    Author       : z00183560
    Modification : Created function

*****************************************************************************/
HI_S32 HI_MPI_ISP_GetDCFInfo(ISP_DCF_INFO_S *pstIspDCF)
{
    ISP_DEV IspDev = 0;
    ISP_CTX_S *pstIspCtx = HI_NULL;

    ISP_GET_CTX(IspDev, pstIspCtx);
    ISP_CHECK_POINTER(pstIspCtx);    
    ISP_CHECK_POINTER(pstIspDCF);
    ISP_CHECK_OPEN(IspDev);

    if (ioctl(g_as32IspFd[IspDev], ISP_DCF_INFO_GET, pstIspDCF))
    {
        return HI_ERR_ISP_NOMEM;
    }

    return HI_SUCCESS;
}
#endif
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

