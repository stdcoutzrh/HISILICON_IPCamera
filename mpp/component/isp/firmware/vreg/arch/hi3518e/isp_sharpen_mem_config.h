/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_sharpen_mem_config.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/01/16
  Description   : 
  History       :
  1.Date        : 2013/01/16
    Author      : n00168968
    Modification: Created file

******************************************************************************/

#ifndef __ISP_SHARPEN_MEM_CONFIG_H__
#define __ISP_SHARPEN_MEM_CONFIG_H__

#include "hi_vreg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

// ------------------------------------------------------------------------------ //
// Instance 'sharpen_mem' of module 'sharpen_mem'
// ------------------------------------------------------------------------------ //

#define HI_SHARPEN_MEM_BASE_ADDR (0x3000)
#define HI_SHARPEN_MEM_SIZE      (0x800)

// ------------------------------------------------------------------------------ //
// Group: Array
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_sharpen_mem_array_data
// ------------------------------------------------------------------------------ //
// Please see other documentation for a description of the contents of this array.
// ------------------------------------------------------------------------------ //

#define HI_SHARPEN_MEM_ARRAY_DATA_DEFAULT (0x0)
#define HI_SHARPEN_MEM_ARRAY_DATA_DATASIZE (32)

#define SHARP_LUT_SIZE 256


// args: index (0-511), data (32-bit)
static __inline void hi_sharpen_mem_array_data_write(HI_U32 index, HI_U32 data) {
    HI_U32 addr = HI_SHARPEN_MEM_BASE_ADDR + (index << 2);
    IOWR_32DIRECT(addr, data & 0xFFFFFFFF);
}
static __inline HI_U16 hi_sharpen_mem_array_data_read(HI_U32 index) {
    HI_U32 addr = HI_SHARPEN_MEM_BASE_ADDR + (index << 2);
    return (HI_U16) IORD_32DIRECT(addr);
}

// ------------------------------------------------------------------------------ //
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __ISP_SHARPEN_MEM_CONFIG_H__ */

