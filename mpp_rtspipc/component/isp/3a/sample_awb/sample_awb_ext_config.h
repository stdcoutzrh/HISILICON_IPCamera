/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_awb_ext_config.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/01/09
  Description   : 
  History       :
  1.Date        : 2013/01/09
    Author      : n00168968
    Modification: Created file

******************************************************************************/
#ifndef __HI_AWB_EXT_CONFIG_H__
#define __HI_AWB_EXT_CONFIG_H__

#include "hi_vreg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */



/*
 *------------------------------------------------------------------------------
 *          these two entity illustrate how to use a ext register
 *
 * 1. Awb Mode
 * 2. Detected color temperature
 *
 *------------------------------------------------------------------------------
 */


// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_WB_type
// ------------------------------------------------------------------------------ //
// white balance type, 0 auto, 1 manual
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_WB_TYPE_DEFAULT (0x0)
#define HI_EXT_SYSTEM_WB_TYPE_DATASIZE (1)
    
// args: data (2-bit)
static __inline HI_VOID hi_ext_system_wb_type_write(HI_U8 id, HI_U8 data){
    IOWR_8DIRECT((AWB_LIB_VREG_BASE(id)), data);
}
static __inline HI_U8 hi_ext_system_wb_type_read(HI_U8 id) {
    return (IORD_8DIRECT(AWB_LIB_VREG_BASE(id)) & 0x1);
}


// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_wb_detect_temp
// ------------------------------------------------------------------------------ //
// the detected color temperature 
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_WB_DETECT_TEMP_DEFAULT (5000)
#define HI_EXT_SYSTEM_WB_DETECT_TEMP_DATASIZE (16)
    
// args: data (16-bit)
static __inline HI_VOID hi_ext_system_wb_detect_temp_write(HI_U8 id, HI_U16 data){
    IOWR_16DIRECT((AWB_LIB_VREG_BASE(id)+0x1), data);
}

static __inline HI_U16 hi_ext_system_wb_detect_temp_read(HI_U8 id){
    return IORD_16DIRECT(AWB_LIB_VREG_BASE(id)+0x1);
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif
