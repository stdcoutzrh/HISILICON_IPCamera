/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_main.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/01/16
  Description   : 
  History       :
  1.Date        : 2013/01/16
    Author      : n00168968
    Modification: Created file

******************************************************************************/

#include "isp_main.h"
#include "isp_ext_config.h"
#include "isp_defaults.h"
#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_statistics.h"
#include "isp_regcfg.h"
#include "isp_config.h"
#include "isp_proc.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#ifdef ENABLE_JPEGEDCF
HI_S32 ISP_UpdateDCFInfo(ISP_DEV IspDev)
{
    HI_S32 i;
    ISP_CTX_S *pstIspCtx = HI_NULL;

    ISP_GET_CTX(IspDev, pstIspCtx);
    
    ISP_CHECK_OPEN(IspDev);

    if (!pstIspCtx->stDcfInfoCtrl.bSupportDCF)
    {
        return HI_FAILURE;
    }
  
    if (HI_NULL == pstIspCtx->stDcfInfoCtrl.pstIspDCF)
    {
        printf("the IspDCF hasn't init!\n");
        return HI_FAILURE;
    }

    for (i=0; i<ISP_MAX_ALGS_NUM; i++)
    {
        if (pstIspCtx->astAlgs[i].bUsed)
        {
            if (HI_NULL != pstIspCtx->astAlgs[i].stAlgFunc.pfn_alg_ctrl)
            {
                pstIspCtx->astAlgs[i].stAlgFunc.pfn_alg_ctrl(IspDev, ISP_DCFINFO_GET, pstIspCtx->stDcfInfoCtrl.pstIspDCF);
            }
        }
    }
       
    return HI_SUCCESS;
}
#endif
HI_S32 ISP_ReadExtregs(ISP_DEV IspDev)
{
    //HI_U32 u32Value = 0;
    //HI_VOID *pValue = HI_NULL;
    ISP_CTX_S *pstIspCtx = HI_NULL;

    ISP_GET_CTX(IspDev, pstIspCtx);

    pstIspCtx->bFreezeFw = hi_ext_system_freeze_firmware_read();
    
    pstIspCtx->stIspDbg.bDebugEn = hi_ext_system_sys_debug_enable_read();
    pstIspCtx->stIspDbg.u32PhyAddr = hi_ext_system_sys_debug_addr_read();
    pstIspCtx->stIspDbg.u32Depth = hi_ext_system_sys_debug_depth_read();
    pstIspCtx->stIspDbg.u32Size= hi_ext_system_sys_debug_size_read();
#if 0
    pstIspCtx->stSnsImageMode.u16Width = hi_isp_top_active_width_read();
    pstIspCtx->stSnsImageMode.u16Height = hi_isp_top_active_height_read();
    u32Value = hi_ext_system_fps_base_read();
    pValue = (HI_VOID *)&u32Value;
    pstIspCtx->stSnsImageMode.f32Fps =  *(HI_FLOAT *)pValue;

    if((pstIspCtx->stSnsImageMode.u16Width == pstIspCtx->stPreSnsImageMode.u16Width )&&(pstIspCtx->stSnsImageMode.u16Height == pstIspCtx->stPreSnsImageMode.u16Height))
    {
        if (pstIspCtx->stPreSnsImageMode.f32Fps != pstIspCtx->stSnsImageMode.f32Fps)
        {
            pstIspCtx->stPreSnsImageMode.f32Fps = pstIspCtx->stSnsImageMode.f32Fps;
            ISP_AlgsCtrl(pstIspCtx->astAlgs, IspDev, ISP_AE_FPS_BASE_SET,
                (HI_VOID *)&pstIspCtx->stSnsImageMode.f32Fps);
        }
    }
#endif    
    return HI_SUCCESS;
}

HI_S32 ISP_SwitchImageMode(ISP_DEV IspDev)
{
    HI_U32 u32Value = 0;
    HI_VOID *pValue = HI_NULL;
    ISP_CMOS_SENSOR_IMAGE_MODE_S  stSnsImageMode;
    ISP_RES_SWITCH_MODE_S stResSwitchMode;
    HI_U8 u8ChangeSnsImageMode;
    HI_S32 s32Ret1 = 0, s32Ret2 = 0, s32Ret3 = 0;
    
    ISP_CTX_S *pstIspCtx = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    //pstIspCtx->stSnsImageMode.u16Width = hi_isp_top_active_width_read();
    //pstIspCtx->stSnsImageMode.u16Height = hi_isp_top_active_height_read();

    pstIspCtx->stSnsImageMode.u16Width = hi_ext_sync_total_width_read();
    pstIspCtx->stSnsImageMode.u16Height = hi_ext_sync_total_height_read();
    
    u32Value = hi_ext_system_fps_base_read();
    pValue = (HI_VOID *)&u32Value;
    pstIspCtx->stSnsImageMode.f32Fps = *(HI_FLOAT *)pValue;

    if (0 == pstIspCtx->u8IspImageModeFlag)
    {
        ISP_AlgsCtrl(pstIspCtx->astAlgs, IspDev, ISP_AE_FPS_BASE_SET,
            (HI_VOID *)&pstIspCtx->stSnsImageMode.f32Fps);

        pstIspCtx->stPreSnsImageMode.u16Width  = pstIspCtx->stSnsImageMode.u16Width;
        pstIspCtx->stPreSnsImageMode.u16Height = pstIspCtx->stSnsImageMode.u16Height;
        pstIspCtx->stPreSnsImageMode.f32Fps  = pstIspCtx->stSnsImageMode.f32Fps;
        pstIspCtx->u8IspImageModeFlag = 1;
    }
    else
    {
        if((pstIspCtx->stSnsImageMode.u16Width != pstIspCtx->stPreSnsImageMode.u16Width )||(pstIspCtx->stSnsImageMode.u16Height != pstIspCtx->stPreSnsImageMode.u16Height)||(pstIspCtx->stSnsImageMode.f32Fps != pstIspCtx->stPreSnsImageMode.f32Fps))
        {
            stSnsImageMode.u16Width  = pstIspCtx->stSnsImageMode.u16Width;
            stSnsImageMode.u16Height = pstIspCtx->stSnsImageMode.u16Height;
            stSnsImageMode.f32Fps = pstIspCtx->stSnsImageMode.f32Fps;

            pstIspCtx->stPreSnsImageMode.u16Width  = pstIspCtx->stSnsImageMode.u16Width;
            pstIspCtx->stPreSnsImageMode.u16Height = pstIspCtx->stSnsImageMode.u16Height;
            pstIspCtx->stPreSnsImageMode.f32Fps  = pstIspCtx->stSnsImageMode.f32Fps;

            s32Ret1 = ISP_SensorSetImageMode(IspDev, &stSnsImageMode);
            if(HI_SUCCESS == s32Ret1)    /* need to init sensor */
            {
                stResSwitchMode.f32Fps = pstIspCtx->stSnsImageMode.f32Fps;
                stResSwitchMode.u16Height = pstIspCtx->stSnsImageMode.u16Height;
                stResSwitchMode.u16Width = pstIspCtx->stSnsImageMode.u16Width;
                
                s32Ret3 = ioctl(g_as32IspFd[IspDev], ISP_RES_SWITCH_SET, &stResSwitchMode);
                if (HI_SUCCESS != s32Ret3)
                {
                    printf("Config ISP Res Switch Failed with ec %#x!\n", s32Ret3);
                    return s32Ret3;
                }   
                
                s32Ret2 = ISP_SensorInit(IspDev);
                if (HI_SUCCESS != s32Ret2)
                {
                    printf("ISP[%d] init sensor failed!\n", IspDev);
                    return HI_FAILURE;
                }

                u8ChangeSnsImageMode = 1;
                ISP_AlgsCtrl(pstIspCtx->astAlgs, IspDev, ISP_CHANGE_IMAGE_MODE_SET, (HI_VOID *)&u8ChangeSnsImageMode);
            }

            ISP_AlgsCtrl(pstIspCtx->astAlgs, IspDev, ISP_AE_FPS_BASE_SET,
                (HI_VOID *)&pstIspCtx->stSnsImageMode.f32Fps);

            hi_ext_top_res_switch_write(HI_TRUE);
        }
    }

    return HI_SUCCESS;
    

}


HI_S32 ISP_WDRCfgSet(ISP_DEV IspDev)
{
    HI_S32 s32Ret;
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_WDR_CFG_S stWDRCfg;

    ISP_GET_CTX(IspDev, pstIspCtx);

    memset(&stWDRCfg, 0, sizeof(ISP_WDR_CFG_S));

    stWDRCfg.u8WDRMode = pstIspCtx->u8SnsWDRMode;

    if (IS_LINE_WDR_MODE(stWDRCfg.u8WDRMode))
    {
        hi_isp_interrupts_interrupt1_source_write(24);
    }
    else
    {
        hi_isp_interrupts_interrupt1_source_write(0);
    }
    
    s32Ret = ioctl(g_as32IspFd[IspDev], ISP_WDR_CFG_SET, &stWDRCfg);
    if (HI_SUCCESS != s32Ret)
    {
        printf("Config WDR register Failed with ec %#x!\n", s32Ret);
        return s32Ret;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_SwitchWDRMode(ISP_DEV IspDev)
{
    HI_U8   u8SensorWDRMode;
    HI_S32 s32Ret = 0;
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_GET_CTX(IspDev, pstIspCtx);

#if 0
    hi_isp_input_port_mode_request_write(HI_ISP_INPUT_PORT_MODE_REQUEST_SAFE_STOP);
    while(hi_isp_input_port_mode_status_read() != 0)
    {
        ;
    }
#endif
    ISP_WDRCfgSet(IspDev);
    
    /* 1. switch sensor to WDR mode */
    u8SensorWDRMode = pstIspCtx->u8SnsWDRMode;
    ISP_SensorSetWDRMode(IspDev, u8SensorWDRMode);

    s32Ret = ISP_SensorInit(IspDev);
    if (HI_SUCCESS != s32Ret)
    {
        printf("ISP[%d] init sensor failed!\n", IspDev);
        return HI_FAILURE;
    }

    /* 2. get new sensor default param */
    ISP_SensorUpdateDefault(IspDev);
    ISP_SensorUpdateBlc(IspDev);
    
    /* 3. init the common part of extern registers and real registers */
    //ISP_ExtRegsDefault();
    //ISP_RegsDefault();
    ISP_ExtRegsInitialize(IspDev);
    ISP_RegsInitialize(IspDev);
    
    /* 4. isp algs global variable initialize */
    //ISP_GlobalInitialize(IspDev);
#if 0
    /* 5. reinit all algorithms. */
    ISP_AlgsExit(pstIspCtx->astAlgs, IspDev);
    ISP_AlgsInit(pstIspCtx->astAlgs, IspDev);
#endif
    /* 6. notify algs to switch WDR mode */
    ISP_AlgsCtrl(pstIspCtx->astAlgs, IspDev, ISP_WDR_MODE_SET, (HI_VOID *)&u8SensorWDRMode);

    pstIspCtx->u8PreSnsWDRMode = pstIspCtx->u8SnsWDRMode;

    //hi_isp_input_port_mode_request_write(HI_ISP_INPUT_PORT_MODE_REQUEST_SAFE_START);

    hi_ext_top_wdr_switch_write(HI_TRUE);

    return HI_SUCCESS;
}

HI_S32 ISP_Run(ISP_DEV IspDev)
{
    HI_S32 s32Ret;
    HI_VOID *pStat = HI_NULL;
    HI_VOID *pRegCfg = HI_NULL;
    ISP_CTX_S *pstIspCtx = HI_NULL;

    ISP_GET_CTX(IspDev, pstIspCtx);

    /*  get statistics buf info. */
    s32Ret = ISP_StatisticsGetBuf(IspDev, &pStat);
    if (s32Ret)
    {
        return s32Ret;
    }

    /* get regcfg */
    s32Ret = ISP_RegCfgInit(IspDev, &pRegCfg);
    if (s32Ret)
    {
        return s32Ret;
    }

    ISP_ReadExtregs(IspDev);

    if (pstIspCtx->bFreezeFw)
    {
        ISP_RegCfgSet(IspDev);
        
        return HI_SUCCESS;
    }
    
    pstIspCtx->u32FrameCnt++;

    ISP_DbgRunBgn(&pstIspCtx->stIspDbg, pstIspCtx->u32FrameCnt);

    ISP_AlgsRun(pstIspCtx->astAlgs, IspDev, pStat, pRegCfg, 0);

    ISP_ProcWrite(pstIspCtx->astAlgs, IspDev);

    /*更新DCF信息*/
    
#ifdef ENABLE_JPEGEDCF
    ISP_UpdateDCFInfo(IspDev);
#endif

    ISP_DbgRunEnd(&pstIspCtx->stIspDbg, pstIspCtx->u32FrameCnt);

    /* release statistics buf info. */
    ISP_StatisticsPutBuf(IspDev);

    /* record the register config infomation to kernel,and be valid in next frame. */
    ISP_RegCfgSet(IspDev);

    if (0 == pstIspCtx->u32FrameCnt % DIV_0_TO_1(pstIspCtx->stLinkage.u8AERunInterval))   //h00191408
    {
        if (!pstIspCtx->stLinkage.bDefectPixel)
        {
            ISP_SyncCfgSet(IspDev);
        }
    }

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

