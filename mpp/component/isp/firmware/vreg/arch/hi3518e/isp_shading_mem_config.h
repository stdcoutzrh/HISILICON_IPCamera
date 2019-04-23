/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_shading_mem_config.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/01/16
  Description   : 
  History       :
  1.Date        : 2013/01/16
    Author      : n00168968
    Modification: Created file

******************************************************************************/

#ifndef __ISP_SHADING_MEM_CONFIG_H__
#define __ISP_SHADING_MEM_CONFIG_H__

#include "hi_vreg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

// ------------------------------------------------------------------------------ //
// Instance 'radial_shading_mem' of module 'radial_shading_mem'
// ------------------------------------------------------------------------------ //

#define HI_RADIAL_SHADING_MEM_BASE_ADDR (0x1000)
#define HI_RADIAL_SHADING_MEM_SIZE      (0x200)

// ------------------------------------------------------------------------------ //
// Group: luts
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_radial_shading_mem_luts_shading_lutr
// ------------------------------------------------------------------------------ //
// Lookup table for the red channel
// ------------------------------------------------------------------------------ //
#define HI_RADIAL_SHADING_MEM_LUTS_SHADING_LUTR_DEFAULT (4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096)
#define HI_RADIAL_SHADING_MEM_LUTS_SHADING_LUTR_DATASIZE (16)
// args: index (0-128), data (16-bit)
static __inline void hi_radial_shading_mem_luts_shading_lutr_write(HI_U32 index, HI_U16 data) { 
    HI_U32 addr = HI_RADIAL_SHADING_MEM_BASE_ADDR + (index << 2);    
    IOWR_16DIRECT(addr, data & 0xFFFF);
}

static __inline HI_U16 hi_radial_shading_mem_luts_shading_lutr_read(HI_U32 index) { 
    HI_U32 addr = HI_RADIAL_SHADING_MEM_BASE_ADDR + (index << 2);    
    return (HI_U16) IORD_16DIRECT(addr);
}

// ------------------------------------------------------------------------------ //
// Register: hi_radial_shading_mem_luts_shading_lutg
// ------------------------------------------------------------------------------ //
// Lookup table for the green channel
// ------------------------------------------------------------------------------ //

#define HI_RADIAL_SHADING_MEM_LUTS_SHADING_LUTG_DEFAULT (4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096)
#define HI_RADIAL_SHADING_MEM_LUTS_SHADING_LUTG_DATASIZE (16)

// args: index (0-128), data (16-bit)
static __inline void hi_radial_shading_mem_luts_shading_lutg_write(HI_U32 index, HI_U16 data) {
    HI_U32 addr = HI_RADIAL_SHADING_MEM_BASE_ADDR + 0x400 + (index << 2);    
    IOWR_16DIRECT(addr, data & 0xFFFF);
}

static __inline HI_U16 hi_radial_shading_mem_luts_shading_lutg_read(HI_U32 index) {
    HI_U32 addr = HI_RADIAL_SHADING_MEM_BASE_ADDR + 0x400 + (index << 2);   
    return (HI_U16) IORD_16DIRECT(addr);
}

// ------------------------------------------------------------------------------ //
// Register: hi_radial_shading_mem_luts_shading_lutb
// ------------------------------------------------------------------------------ //
// Lookup table for the blue channel
// ------------------------------------------------------------------------------ //
#define HI_RADIAL_SHADING_MEM_LUTS_SHADING_LUTB_DEFAULT (4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096,4096)
#define HI_RADIAL_SHADING_MEM_LUTS_SHADING_LUTB_DATASIZE (16)

// args: index (0-128), data (16-bit)
static __inline void hi_radial_shading_mem_luts_shading_lutb_write(HI_U32 index, HI_U16 data) {
    HI_U32 addr = HI_RADIAL_SHADING_MEM_BASE_ADDR + 0x800 + (index << 2);    
    IOWR_16DIRECT(addr, data & 0xFFFF);
}

static __inline HI_U16 hi_radial_shading_mem_luts_shading_lutb_read(HI_U32 index) { 
    HI_U32 addr = HI_RADIAL_SHADING_MEM_BASE_ADDR + 0x800 + (index << 2); 
    return (HI_U16) IORD_16DIRECT(addr);
}
// ------------------------------------------------------------------------------ //

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __ISP_SHADING_MEM_CONFIG_H__ */

