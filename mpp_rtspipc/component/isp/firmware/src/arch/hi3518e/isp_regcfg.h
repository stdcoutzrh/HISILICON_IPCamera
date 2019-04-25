/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_regcfg.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/05/07
  Description   : 
  History       :
  1.Date        : 2013/05/07
    Author      : n00168968
    Modification: Created file

******************************************************************************/
#ifndef __ISP_REGCFG_H__
#define __ISP_REGCFG_H__

#include "hi_comm_isp.h"
#include "mpi_isp.h"
#include "isp_sensor.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

HI_S32 ISP_RegCfgInit(ISP_DEV IspDev, HI_VOID **ppCfg);
HI_S32 ISP_RegCfgSet(ISP_DEV IspDev);
HI_S32 ISP_SyncCfgSet(ISP_DEV IspDev);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
