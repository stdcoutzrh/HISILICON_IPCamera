/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_sensor.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/05/06
  Description   : 
  History       :
  1.Date        : 2013/05/06
    Author      : n00168968
    Modification: Created file

******************************************************************************/

#include <stdio.h>
#include <string.h>
#include "isp_sensor.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

typedef struct hiISP_SENSOR_S
{
    SENSOR_ID               SensorId;
    ISP_SENSOR_REGISTER_S   stRegister;
    ISP_CMOS_DEFAULT_S      stSnsDft;
    ISP_CMOS_BLACK_LEVEL_S  stSnsBlackLevel;    /* some sensors's black level will be changed with iso */
    ISP_SNS_REGS_INFO_S     stSnsRegInfo;
    ISP_CMOS_SENSOR_IMAGE_MODE_S      stSnsImageMode;
} ISP_SENSOR_S;

ISP_SENSOR_S g_astSensorCtx[ISP_MAX_DEV_NUM] = {{0}};
#define SENSOR_GET_CTX(dev, pstCtx)   pstCtx = &g_astSensorCtx[dev]

HI_S32 ISP_SensorRegCallBack(ISP_DEV IspDev, SENSOR_ID SensorId, ISP_SENSOR_REGISTER_S *pstRegister)
{
    ISP_SENSOR_S *pstSensor = HI_NULL;
    
    SENSOR_GET_CTX(IspDev, pstSensor);

    pstSensor->SensorId = SensorId;
    memcpy(&pstSensor->stRegister, pstRegister, sizeof(ISP_SENSOR_REGISTER_S));

    if(HI_NULL != pstSensor->stRegister.stSnsExp.pfn_cmos_sensor_global_init)
    {
       pstSensor->stRegister.stSnsExp.pfn_cmos_sensor_global_init();
    }

    return HI_SUCCESS;
}

HI_S32 ISP_SensorUpdateAll(ISP_DEV IspDev)
{
    ISP_SENSOR_S *pstSensor = HI_NULL;
    
    SENSOR_GET_CTX(IspDev, pstSensor);

    if (HI_NULL != pstSensor->stRegister.stSnsExp.pfn_cmos_get_isp_default)
    {
        pstSensor->stRegister.stSnsExp.pfn_cmos_get_isp_default(&pstSensor->stSnsDft);
    }
    else
    {
        printf("Get isp default value error!\n");
        return HI_FAILURE;
    }

    if (HI_NULL != pstSensor->stRegister.stSnsExp.pfn_cmos_get_isp_black_level)
    {
        pstSensor->stRegister.stSnsExp.pfn_cmos_get_isp_black_level(&pstSensor->stSnsBlackLevel);
    }

    return HI_SUCCESS;
}

HI_S32 ISP_SensorGetId(ISP_DEV IspDev, SENSOR_ID *pSensorId)
{
    *pSensorId = g_astSensorCtx[IspDev].SensorId;
    
    return HI_SUCCESS;
}

HI_S32 ISP_SensorGetBlc(ISP_DEV IspDev, ISP_CMOS_BLACK_LEVEL_S **ppstSnsBlackLevel)
{
    *ppstSnsBlackLevel = &g_astSensorCtx[IspDev].stSnsBlackLevel;
    
    return HI_SUCCESS;
}

HI_S32 ISP_SensorGetDefault(ISP_DEV IspDev, ISP_CMOS_DEFAULT_S **ppstSnsDft)
{
    *ppstSnsDft = &g_astSensorCtx[IspDev].stSnsDft;
    
    return HI_SUCCESS;
}


HI_S32 ISP_SensorGetMaxResolution(ISP_DEV IspDev, ISP_CMOS_SENSOR_MAX_RESOLUTION_S **ppstSnsMaxResolution)
{
   *ppstSnsMaxResolution = &g_astSensorCtx[IspDev].stSnsDft.stSensorMaxResolution;

   return HI_SUCCESS;
}

HI_S32 ISP_SensorGetSnsReg(ISP_DEV IspDev, ISP_SNS_REGS_INFO_S **ppstSnsRegInfo)
{
    *ppstSnsRegInfo = &g_astSensorCtx[IspDev].stSnsRegInfo;
    
    return HI_SUCCESS;
}

HI_S32 ISP_SensorInit(ISP_DEV IspDev)
{
    ISP_SENSOR_S *pstSensor = HI_NULL;
    
    SENSOR_GET_CTX(IspDev, pstSensor);
    
    if (HI_NULL != pstSensor->stRegister.stSnsExp.pfn_cmos_sensor_init)
    {
        pstSensor->stRegister.stSnsExp.pfn_cmos_sensor_init();
    }
    else
    {
        return HI_FAILURE;
    }
    
    return HI_SUCCESS;
}

HI_S32 ISP_SensorExit(ISP_DEV IspDev)
{
    ISP_SENSOR_S *pstSensor = HI_NULL;
    
    SENSOR_GET_CTX(IspDev, pstSensor);
    
    if (HI_NULL != pstSensor->stRegister.stSnsExp.pfn_cmos_sensor_exit)
    {
        pstSensor->stRegister.stSnsExp.pfn_cmos_sensor_exit();
    }
    else
    {
        return HI_FAILURE;
    }
    
    return HI_SUCCESS;
}

HI_S32 ISP_SensorUpdateBlc(ISP_DEV IspDev)
{
    ISP_SENSOR_S *pstSensor = HI_NULL;
    
    SENSOR_GET_CTX(IspDev, pstSensor);
    
    if (HI_NULL != pstSensor->stRegister.stSnsExp.pfn_cmos_get_isp_black_level)
    {
        /* sensor should record the present iso, and calculate new black level. */
        pstSensor->stRegister.stSnsExp.pfn_cmos_get_isp_black_level(&pstSensor->stSnsBlackLevel);
    }
    else
    {
        return HI_FAILURE;
    }
    
    return HI_SUCCESS;
}

HI_S32 ISP_SensorUpdateDefault(ISP_DEV IspDev)
{
    ISP_SENSOR_S *pstSensor = HI_NULL;
    
    SENSOR_GET_CTX(IspDev, pstSensor);
    
    if (HI_NULL != pstSensor->stRegister.stSnsExp.pfn_cmos_get_isp_default)
    {
        pstSensor->stRegister.stSnsExp.pfn_cmos_get_isp_default(&pstSensor->stSnsDft);
    }
    else
    {
        return HI_FAILURE;
    }
    
    return HI_SUCCESS;
}

HI_S32 ISP_SensorSetWDRMode(ISP_DEV IspDev, HI_U8 u8Mode)
{
    ISP_SENSOR_S *pstSensor = HI_NULL;
    
    SENSOR_GET_CTX(IspDev, pstSensor);
    
    if (HI_NULL != pstSensor->stRegister.stSnsExp.pfn_cmos_set_wdr_mode)
    {
        pstSensor->stRegister.stSnsExp.pfn_cmos_set_wdr_mode(u8Mode);
    }
    
    return HI_SUCCESS;
}

HI_S32 ISP_SensorSetImageMode(ISP_DEV IspDev, ISP_CMOS_SENSOR_IMAGE_MODE_S *pstSnsImageMode)
{
    ISP_SENSOR_S *pstSensor = NULL;

    SENSOR_GET_CTX(IspDev, pstSensor);
    if (HI_NULL != pstSensor->stRegister.stSnsExp.pfn_cmos_set_image_mode)
    {
        if (HI_SUCCESS != pstSensor->stRegister.stSnsExp.pfn_cmos_set_image_mode(pstSnsImageMode))
        {
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}

HI_S32 ISP_SensorSetPixelDetect(ISP_DEV IspDev, HI_BOOL bEnable)
{
    ISP_SENSOR_S *pstSensor = HI_NULL;
    
    SENSOR_GET_CTX(IspDev, pstSensor);
    
    if (HI_NULL != pstSensor->stRegister.stSnsExp.pfn_cmos_set_pixel_detect)
    {
        pstSensor->stRegister.stSnsExp.pfn_cmos_set_pixel_detect(bEnable);
    }
    else
    {
        return HI_FAILURE;
    }
    
    return HI_SUCCESS;
}

HI_S32 ISP_SensorUpdateSnsReg(ISP_DEV IspDev)
{
    ISP_SENSOR_S *pstSensor = HI_NULL;
    
    SENSOR_GET_CTX(IspDev, pstSensor);
    
    if (HI_NULL != pstSensor->stRegister.stSnsExp.pfn_cmos_get_sns_reg_info)
    {
        pstSensor->stRegister.stSnsExp.pfn_cmos_get_sns_reg_info(&pstSensor->stSnsRegInfo);
    }
    else
    {
        return HI_FAILURE;
    }
    
    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

