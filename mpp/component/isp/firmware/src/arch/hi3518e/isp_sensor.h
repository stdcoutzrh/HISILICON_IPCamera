/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_sensor.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/05/06
  Description   : 
  History       :
  1.Date        : 2013/05/06
    Author      : n00168968
    Modification: Created file

******************************************************************************/
#ifndef __ISP_SENSOR_H__
#define __ISP_SENSOR_H__

#include "hi_comm_sns.h"
#include "hi_comm_isp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

HI_S32 ISP_SensorRegCallBack(ISP_DEV IspDev, SENSOR_ID SensorId,
    ISP_SENSOR_REGISTER_S *pstRegister);

HI_S32 ISP_SensorUpdateAll(ISP_DEV IspDev);

HI_S32 ISP_SensorGetId(ISP_DEV IspDev, SENSOR_ID *pSensorId);
HI_S32 ISP_SensorGetBlc(ISP_DEV IspDev, ISP_CMOS_BLACK_LEVEL_S **ppstSnsBlackLevel);
HI_S32 ISP_SensorGetDefault(ISP_DEV IspDev, ISP_CMOS_DEFAULT_S **ppstSnsDft);
HI_S32 ISP_SensorGetSnsReg(ISP_DEV IspDev, ISP_SNS_REGS_INFO_S **ppstSnsRegInfo);
HI_S32 ISP_SensorGetMaxResolution(ISP_DEV IspDev, ISP_CMOS_SENSOR_MAX_RESOLUTION_S ** ppstSnsMaxResolution);

HI_S32 ISP_SensorInit(ISP_DEV IspDev);
HI_S32 ISP_SensorExit(ISP_DEV IspDev);
HI_S32 ISP_SensorUpdateBlc(ISP_DEV IspDev);
HI_S32 ISP_SensorUpdateDefault(ISP_DEV IspDev);
HI_S32 ISP_SensorSetWDRMode(ISP_DEV IspDev, HI_U8 u8Mode);
HI_S32 ISP_SensorSetResolutionMode(ISP_DEV IspDev, HI_U32 u32Mode);
HI_S32 ISP_SensorSetPixelDetect(ISP_DEV IspDev, HI_BOOL bEnable);
HI_S32 ISP_SensorUpdateSnsReg(ISP_DEV IspDev);
HI_S32 ISP_SensorSetImageMode(ISP_DEV IspDev, ISP_CMOS_SENSOR_IMAGE_MODE_S *pstSnsImageMode);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
