/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_defaults.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/01/16
  Description   : 
  History       :
  1.Date        : 2013/01/16
    Author      : n00168968
    Modification: Created file

******************************************************************************/

#ifndef __ISP_DEFAULTS_H_
#define __ISP_DEFAULTS_H_

#include "isp_main.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */


/* ext register default value */
HI_VOID ISP_ExtRegsDefault(HI_VOID);

/* initialize ext register */
HI_VOID ISP_ExtRegsInitialize(ISP_DEV IspDev);

/* isp register default value */
HI_VOID ISP_RegsDefault(HI_VOID);

/* initialize isp register */
HI_VOID ISP_RegsInitialize(ISP_DEV IspDev);

/* This function initialises an instance of ISP_CTX_S */
HI_VOID ISP_GlobalInitialize(ISP_DEV IspDev);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __ISP_DEFAULTS_H_ */

