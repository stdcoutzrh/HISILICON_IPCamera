/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_statistics.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/05/07
  Description   : 
  History       :
  1.Date        : 2013/05/07
    Author      : n00168968
    Modification: Created file

******************************************************************************/
#ifndef __ISP_STATISTICS_H__
#define __ISP_STATISTICS_H__

#include "hi_comm_isp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

/* init the statistics buf */
HI_S32 ISP_StatisticsInit(ISP_DEV IspDev);
/* exit the statistics buf */
HI_S32 ISP_StatisticsExit(ISP_DEV IspDev);
/* get a statistics buf from kernel */
HI_S32 ISP_StatisticsGetBuf(ISP_DEV IspDev, HI_VOID **ppStat);
/* release a statistics buf to kernel */
HI_S32 ISP_StatisticsPutBuf(ISP_DEV IspDev);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
