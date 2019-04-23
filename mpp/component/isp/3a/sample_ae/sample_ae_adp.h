/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : sample_ae_adp.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2012/12/19
  Description   : 
  History       :
  1.Date        : 2012/12/19
    Author      : n00168968
    Modification: Created file

******************************************************************************/
#ifndef __SAMPLE_AE_ADP_H__
#define __SAMPLE_AE_ADP_H__

#include <string.h>
#include "hi_type.h"
#include "hi_comm_3a.h"
#include "hi_ae_comm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

typedef struct hiSAMPLE_AE_CTX_S
{
    /* usr var */
    HI_U8                   u8AeMode;
    
    /* communicate with isp */
    ISP_AE_PARAM_S          stAeParam;
    HI_U32                  u32FrameCnt;
    ISP_AE_INFO_S           stAeInfo;
    ISP_AE_RESULT_S         stAeResult;

    /* communicate with sensor, defined by user. */
    HI_BOOL                 bSnsRegister;
    SENSOR_ID               SensorId;
    AE_SENSOR_DEFAULT_S     stSnsDft;
    AE_SENSOR_REGISTER_S    stSnsRegister;

    /* global variables of ae algorithm */
} SAMPLE_AE_CTX_S;

#define MAX_AE_REGISTER_SNS_NUM    1
#define MAX_AE_LIB_NUM              2
#define GAIN_USER_LINEAR_SHIFT     10

extern SAMPLE_AE_CTX_S g_astAeCtx_sample[MAX_AE_LIB_NUM];
extern HI_U8 g_u8AeExtRegId;

/* we assumed that the different lib instance have different id, 
 * hisi use the id 0 & 1.
 */
#define AE_GET_EXTREG_ID(s32Handle)   ((0 == (s32Handle)) ? 0x4 : 0x5)

#define AE_GET_CTX(s32Handle)           (&g_astAeCtx_sample[s32Handle])

#define AE_CHECK_HANDLE_ID(s32Handle)\
do {\
    if (((s32Handle) < 0) || ((s32Handle) >= MAX_AE_LIB_NUM))\
    {\
        printf("Illegal handle id %d in %s!\n", (s32Handle), __FUNCTION__);\
        return HI_FAILURE;\
    }\
}while(0)

#define AE_CHECK_LIB_NAME(acName)\
do {\
    if (0 != strcmp((acName), HI_AE_LIB_NAME))\
    {\
        printf("Illegal lib name %s in %s!\n", (acName), __FUNCTION__);\
        return HI_FAILURE;\
    }\
}while(0)

#define AE_CHECK_POINTER(ptr)\
do {\
    if (HI_NULL == ptr)\
    {\
        printf("Null Pointer in %s!\n", __FUNCTION__);\
        return HI_FAILURE;\
    }\
}while(0)


#define AE_CHECK_DEV(dev)\
    do {\
        if (0 != dev)\
        {\
            ISP_TRACE(HI_DBG_ERR, "Err AE dev %d in %s!\n", dev, __FUNCTION__);\
            return HI_ERR_ISP_ILLEGAL_PARAM;\
        }\
    }while(0) 

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif

