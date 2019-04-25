/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_gamma_rgb_mem_config.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/01/16
  Description   : 
  History       :
  1.Date        : 2013/01/16
    Author      : n00168968
    Modification: Created file

******************************************************************************/

#ifndef __ISP_GAMMA_RGB_MEM_CONFIG_H__
#define __ISP_GAMMA_RGB_MEM_CONFIG_H__

#include "hi_vreg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

// ------------------------------------------------------------------------------ //
// Instance 'gamma_rgb_mem' of module 'gamma_rgb_mem'
// ------------------------------------------------------------------------------ //

#define HI_GAMMA_RGB_MEM_BASE_ADDR (0x2800)
#define HI_GAMMA_RGB_MEM_SIZE      (0x400)

// ------------------------------------------------------------------------------ //
// Group: Array
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_gamma_rgb_mem_array_data
// ------------------------------------------------------------------------------ //
// Please see other documentation for a description of the contents of this array.
// ------------------------------------------------------------------------------ //

#define HI_GAMMA_RGB_MEM_ARRAY_DATA_DEFAULT (0x0)
#define HI_GAMMA_RGB_MEM_ARRAY_DATA_DATASIZE (32)

// args: index (0-256), data (32-bit)
static __inline void hi_gamma_rgb_mem_array_data_write(HI_U32 index, HI_U32 data) {
    HI_U32 addr = HI_GAMMA_RGB_MEM_BASE_ADDR + (index << 2);
    IOWR_32DIRECT(addr, data & 0xffffffff);
}

static __inline HI_U32 hi_gamma_rgb_mem_array_data_read(HI_U32 index) {
    HI_U32 addr = HI_GAMMA_RGB_MEM_BASE_ADDR + (index << 2);
    return (HI_U32) IORD_32DIRECT(addr);
}

// ------------------------------------------------------------------------------ //

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __ISP_GAMMA_RGB_MEM_CONFIG_H__ */

