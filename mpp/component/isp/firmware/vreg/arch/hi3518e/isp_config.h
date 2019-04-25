/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_config.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/01/16
  Description   : 
  History       :
  1.Date        : 2013/01/16
    Author      : n00168968
    Modification: Created file

******************************************************************************/

#ifndef __ISP_CONFIG_H__
#define __ISP_CONFIG_H__

#include "hi_vreg.h"
#include "isp_config_ex.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

// ------------------------------------------------------------------------------ //
// Instance 'isp' of module 'ip_config_h'
// ------------------------------------------------------------------------------ //

#define HI_MINI_ISP
#ifndef HI_MINI_ISP
#define IOWR_32DIRECT_APICAL IOWR_32DIRECT
#define IOWR_16DIRECT_APICAL IOWR_16DIRECT
#define IOWR_8DIRECT_APICAL  IOWR_8DIRECT
#else
/*主要是为了消除告警*/
#define IOWR_32DIRECT_APICAL(BASE, DATA) do\
{\
	HI_U32 u32Invalid;\
	u32Invalid = (BASE)+(DATA);\
	u32Invalid += 1;\
}while(0)
#define IOWR_16DIRECT_APICAL(BASE, DATA) do\
{\
	HI_U32 u32Invalid;\
	u32Invalid = (BASE)+(DATA);\
	u32Invalid += 1;\
}while(0)
#define IOWR_8DIRECT_APICAL(BASE, DATA)  do\
{\
	HI_U32 u32Invalid;\
	u32Invalid = (BASE)+(DATA);\
	u32Invalid += 1;\
}while(0)
#endif

#define HI_ISP_AE_WEI_COMBIN_COUNT  4
#define HI_MINIISP_BITDEPTH 14

#define HI_ISP_BASE_ADDR (0x000)
#define HI_ISP_SIZE      (0x1000)

/*modify by q00214668*/
#define CCM_CONVERT_PRE(value) ((value & 0x8000) | (value << 3))
#define CCM_CONVERT(value) ((value & 0x8000)?((~(value & 0x7FFF)) + 1):(value))


// ------------------------------------------------------------------------------ //
// Group: ID
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_isp_id_api
// ------------------------------------------------------------------------------ //
static __inline HI_VOID hi_isp_blc_enable_write(HI_U8 data) {
#ifdef HI_MINI_ISP
	HI_U8 u8current = IORD_8DIRECT(0x1000);
	IOWR_8DIRECT(0x1000, (data & 0x1) | (u8current & 0xfe));
#endif
}
static __inline HI_U8 hi_isp_blc_enable_read(HI_VOID) {
#ifdef HI_MINI_ISP
    return (IORD_8DIRECT(0x1000) & 0x1);
#else
	return 1;
#endif
}

#define HI_ISP_ID_API_DEFAULT (0)
#define HI_ISP_ID_API_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_isp_id_api_read(HI_VOID) {
    return IORD_32DIRECT(0x0000);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_id_product
// ------------------------------------------------------------------------------ //

#define HI_ISP_ID_PRODUCT_DEFAULT (0x0)
#define HI_ISP_ID_PRODUCT_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_isp_id_product_read(HI_VOID) {
    return IORD_32DIRECT(0x0004);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_id_version
// ------------------------------------------------------------------------------ //

#define HI_ISP_ID_VERSION_DEFAULT (0x0)
#define HI_ISP_ID_VERSION_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_isp_id_version_read(HI_VOID) {
    return IORD_32DIRECT(0x0008);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_id_reserved
// ------------------------------------------------------------------------------ //

#define HI_ISP_ID_RESERVED_DEFAULT (0x0)
#define HI_ISP_ID_RESERVED_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_isp_id_reserved_read(HI_VOID) {
    return IORD_32DIRECT(0x000C);
}

// ------------------------------------------------------------------------------ //
// Group: Top
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_active_width
// ------------------------------------------------------------------------------ //
// Active video width in pixels
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_ACTIVE_WIDTH_DEFAULT (0x780)
#define HI_ISP_TOP_ACTIVE_WIDTH_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_top_active_width_write(HI_U16 data) {
#ifdef HI_MINI_ISP
	HI_U16 u16current = IORD_16DIRECT(0x01f0);
	IOWR_16DIRECT(0x01f0, (data & 0x1fff) | (u16current & 0xe000));
#else
    IOWR_16DIRECT(0x0010, data);
#endif
}
static __inline HI_U16 hi_isp_top_active_width_read(HI_VOID) {
#ifdef HI_MINI_ISP
    return (IORD_16DIRECT(0x01f0) & 0x1fff);
#else
	return IORD_16DIRECT(0x0010);
#endif
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_active_height
// ------------------------------------------------------------------------------ //
// Active video height in lines
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_ACTIVE_HEIGHT_DEFAULT (0x438)
#define HI_ISP_TOP_ACTIVE_HEIGHT_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_top_active_height_write(HI_U16 data) {
#ifdef HI_MINI_ISP
	HI_U16 u16current = IORD_16DIRECT(0x01f2);
	IOWR_16DIRECT(0x01f2, (data & 0x1fff) | (u16current & 0xe000));
#else
    IOWR_16DIRECT_APICAL(0x0014, data);
#endif
}
static __inline HI_U16 hi_isp_top_active_height_read(HI_VOID) {
#ifdef HI_MINI_ISP
    return (IORD_16DIRECT(0x01f2) & 0x1fff);
#else
	return IORD_16DIRECT(0x0014);
#endif
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_rggb_start
// ------------------------------------------------------------------------------ //
// Start color of the rggb pattern
//   0 = R Gr 
//  Gb B
//   1 = Gr R 
//  B Gb
//   2 = Gb B 
//  R Gr
//   3 = B Gb 
//  Gr R
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_RGGB_START_DEFAULT (0x0)
#define HI_ISP_TOP_RGGB_START_DATASIZE (2)

#define HI_ISP_TOP_RGGB_START_R_GR_GB_B (0)
#define HI_ISP_TOP_RGGB_START_GR_R_B_GB (1)
#define HI_ISP_TOP_RGGB_START_GB_B_R_GR (2)
#define HI_ISP_TOP_RGGB_START_B_GB_GR_R (3)

// args: data (2-bit)
static __inline HI_VOID hi_isp_top_rggb_start_write(HI_U8 data) {
#ifdef HI_MINI_ISP
	IOWR_8DIRECT(0x01E0, (data & 0x03));
#else
	IOWR_8DIRECT(0x0018, (data & 0x03));
#endif    
}
static __inline HI_U8 hi_isp_top_rggb_start_read(HI_VOID) {
#ifdef HI_MINI_ISP
	return (IORD_8DIRECT(0x01E0) & 0x03);
#else
	return (IORD_8DIRECT(0x0018) & 0x03);
#endif    
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_delay_line_transfer_select
// ------------------------------------------------------------------------------ //
// 
//   0 = Synchronize on ACL
//   1 = Synchronize on HSYNC
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_DELAY_LINE_TRANSFER_SELECT_DEFAULT (0x0)
#define HI_ISP_TOP_DELAY_LINE_TRANSFER_SELECT_DATASIZE (1)

#define HI_ISP_TOP_DELAY_LINE_TRANSFER_SELECT_SYNCHRONIZE_ON_ACL (0)
#define HI_ISP_TOP_DELAY_LINE_TRANSFER_SELECT_SYNCHRONIZE_ON_HSYNC (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_delay_line_transfer_select_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x0020, (data & 0x01));
}
static __inline HI_U8 hi_isp_top_delay_line_transfer_select_read(HI_VOID) {
    return (IORD_8DIRECT(0x0020) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_delay_line_jitter_correction
// ------------------------------------------------------------------------------ //
// Maximal allowed jitter of the horizontal blanking interval
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_DELAY_LINE_JITTER_CORRECTION_DEFAULT (0x0)
#define HI_ISP_TOP_DELAY_LINE_JITTER_CORRECTION_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_top_delay_line_jitter_correction_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0024, data);
}
static __inline HI_U16 hi_isp_top_delay_line_jitter_correction_read(HI_VOID) {
    return IORD_16DIRECT(0x0024);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_delay_line_regen_hblank
// ------------------------------------------------------------------------------ //
// Horizontal blanking interval during regeneration (0=measured input interval) 
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_DELAY_LINE_REGEN_HBLANK_DEFAULT (0x20)
#define HI_ISP_TOP_DELAY_LINE_REGEN_HBLANK_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_top_delay_line_regen_hblank_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0028, data);
}
static __inline HI_U16 hi_isp_top_delay_line_regen_hblank_read(HI_VOID) {
    return IORD_16DIRECT(0x0028);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_delay_line_extra_lines
// ------------------------------------------------------------------------------ //
// Extra lines to generate at the end of the frame to flush the pipeline 
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_DELAY_LINE_EXTRA_LINES_DEFAULT (0x18)
#define HI_ISP_TOP_DELAY_LINE_EXTRA_LINES_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_top_delay_line_extra_lines_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x002A, data);
}
static __inline HI_U8 hi_isp_top_delay_line_extra_lines_read(HI_VOID) {
    return IORD_8DIRECT(0x002A);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_delay_line_memory_configuration
// ------------------------------------------------------------------------------ //
// Memory configuration of nr delay lines.
// 00 = Maximum number of lines, shortest line length.
// 01 = 3/4 of maximum lines, 4/3 x shortest line.
// 1x = 1/2 of maximum lines, 2 x shortest line.
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_DELAY_LINE_MEMORY_CONFIGURATION_DEFAULT (0x2)
#define HI_ISP_TOP_DELAY_LINE_MEMORY_CONFIGURATION_DATASIZE (2)

// args: data (2-bit)
static __inline HI_VOID hi_isp_top_delay_line_memory_configuration_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x002C, (data & 0x03));
}
static __inline HI_U8 hi_isp_top_delay_line_memory_configuration_read(HI_VOID) {
    return (IORD_8DIRECT(0x002C) & 0x03);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_config_buffer_mode
// ------------------------------------------------------------------------------ //
// Select config double-buffering mode
//   0 = Disabled (config updates immediately)
//   1 = Blocked (config never updates)
//   2 = Local (module config updates during local vertical blanking)
//   3 = Global (all module config updated during ISP vertical blanking)
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_CONFIG_BUFFER_MODE_DEFAULT (0)
#define HI_ISP_TOP_CONFIG_BUFFER_MODE_DATASIZE (2)

#define HI_ISP_TOP_CONFIG_BUFFER_MODE_DISABLED_CONFIG_UPDATES_IMMEDIATELY_ (0)
#define HI_ISP_TOP_CONFIG_BUFFER_MODE_BLOCKED_CONFIG_NEVER_UPDATES_ (1)
#define HI_ISP_TOP_CONFIG_BUFFER_MODE_LOCAL_MODULE_CONFIG_UPDATES_DURING_LOCAL_VERTICAL_BLANKING_ (2)
#define HI_ISP_TOP_CONFIG_BUFFER_MODE_GLOBAL_ALL_MODULE_CONFIG_UPDATED_DURING_ISP_VERTICAL_BLANKING_ (3)

// args: data (2-bit)
static __inline HI_VOID hi_isp_top_config_buffer_mode_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x0030, (data & 0x03));
}
static __inline HI_U8 hi_isp_top_config_buffer_mode_read(HI_VOID) {
    return (IORD_8DIRECT(0x0030) & 0x03);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_bypass_flag
// ------------------------------------------------------------------------------ //
// Top Bypass Flag
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_BYPASS_FLAG_DEFAULT (0x40000)
#define HI_ISP_TOP_BYPASS_FlAG_DATASIZE (32)

// args: data (32-bit)
static __inline HI_VOID hi_isp_top_bypass_flag_write(HI_U32 data) {
    IOWR_32DIRECT_APICAL(0x0040, data);
}
static __inline HI_U32 hi_isp_top_bypass_flag_read(HI_VOID) {
    return IORD_32DIRECT(0x0040);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_bypass_video_test_gen
// ------------------------------------------------------------------------------ //
// Bypass video test generator
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_BYPASS_VIDEO_TEST_GEN_DEFAULT (0)
#define HI_ISP_TOP_BYPASS_VIDEO_TEST_GEN_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_bypass_video_test_gen_write(HI_U8 data) {
    //HI_U8 u8Current = IORD_8DIRECT(0x0040);
    //IOWR_8DIRECT_APICAL(0x0040, (data & 0x01) | (u8Current & 0xFE));
}
static __inline HI_U8 hi_isp_top_bypass_video_test_gen_read(HI_VOID) {
    return (IORD_8DIRECT(0x0040) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_bypass_balance_fe
// ------------------------------------------------------------------------------ //
// Bypass frontend black level adjustment
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_BYPASS_BALANCE_FE_DEFAULT (0)
#define HI_ISP_TOP_BYPASS_BALANCE_FE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_bypass_balance_fe_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0040);
    IOWR_8DIRECT_APICAL(0x0040, ((data & 0x01) << 1) | (u8Current & 0xFD));
}
static __inline HI_U8 hi_isp_top_bypass_balance_fe_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0040) & 0x02) >> 1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_bypass_digital_gain
// ------------------------------------------------------------------------------ //
// Bypass digital gain module
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_BYPASS_DIGITAL_GAIN_DEFAULT (0)
#define HI_ISP_TOP_BYPASS_DIGITAL_GAIN_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_bypass_digital_gain_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0040);
    IOWR_8DIRECT_APICAL(0x0040, ((data & 0x01) << 2) | (u8Current & 0xFB));
}
static __inline HI_U8 hi_isp_top_bypass_digital_gain_read(HI_VOID) {	
    return ((IORD_8DIRECT(0x0040) & 0x04) >> 2);
}


// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_bypass_gamma_fe
// ------------------------------------------------------------------------------ //
// Bypass WDR companded frontend lookup
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_BYPASS_GAMMA_FE_DEFAULT (0)
#define HI_ISP_TOP_BYPASS_GAMMA_FE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_bypass_gamma_fe_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0040);
    IOWR_8DIRECT_APICAL(0x0040, ((data & 0x01) << 3) | (u8Current & 0xF7));
}
static __inline HI_U8 hi_isp_top_bypass_gamma_fe_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0040) & 0x08) >> 3);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_bypass_green_equalize
// ------------------------------------------------------------------------------ //
// Bypass green_equalization
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_BYPASS_GREEN_EQUALIZE_DEFAULT (0)
#define HI_ISP_TOP_BYPASS_GREEN_EQUALIZE_DATASIZE (1)

// args: data (1-bit)
//1 hi_isp_top_bypass_raw_frontend_write
static __inline HI_VOID hi_isp_top_bypass_green_equalize_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0040);
    IOWR_8DIRECT_APICAL(0x0040, ((data & 0x01) << 4) | (u8Current & 0xEF));	
}
static __inline HI_U8 hi_isp_top_bypass_green_equalize_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0040) & 0x10) >> 4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_bypass_defect_pixel
// ------------------------------------------------------------------------------ //
// Bypass defect pixel
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_BYPASS_DEFECT_PIXEL_DEFAULT (0)
#define HI_ISP_TOP_BYPASS_DEFECT_PIXEL_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_bypass_defect_pixel_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0040);
    IOWR_8DIRECT_APICAL(0x0040, ((data & 0x01) << 5) | (u8Current & 0xDF));	
}
static __inline HI_U8 hi_isp_top_bypass_defect_pixel_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0040) & 0x20) >> 5);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_bypass_frame_switch_wdr
// ------------------------------------------------------------------------------ //
// Bypass frame switching WDR
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_BYPASS_FRAME_SWITCH_WDR_DEFAULT (1)
#define HI_ISP_TOP_BYPASS_FRAME_SWITCH_WDR_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_bypass_frame_switch_wdr_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0041);
    IOWR_8DIRECT_APICAL(0x0041, ((data & 0x01) << 7) | (u8Current & 0x7F));
}
static __inline HI_U8 hi_isp_top_bypass_frame_switch_wdr_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0041) & 0x80) >> 7);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_gamma_fe_position
// ------------------------------------------------------------------------------ //
// Bypass WDR companded frontend lookup
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_GAMMA_FE_POSITION_DEFAULT (0)
#define HI_ISP_TOP_GAMMA_FE_POSITION_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_gamma_fe_position_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0042);
    IOWR_8DIRECT_APICAL(0x0042, (data & 0x01) | (u8Current & 0xFE));	
}
static __inline HI_U8 hi_isp_top_gamma_fe_position_read(HI_VOID) {
    return (IORD_8DIRECT(0x0042) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_bypass_nr
// ------------------------------------------------------------------------------ //
// Bypass nr
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_BYPASS_NR_DEFAULT (0)
#define HI_ISP_TOP_BYPASS_NR_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_bypass_nr_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0040);
    IOWR_8DIRECT_APICAL(0x0040, ((data & 0x01) << 6) | (u8Current & 0xBF));	
}
static __inline HI_U8 hi_isp_top_bypass_nr_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0040) & 0x40) >> 6);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_bypass_frame_switch_buffer
// ------------------------------------------------------------------------------ //
// Bypass frame buffer for the Frame Swithch
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_BYPASS_FRAME_SWITCH_BUFFER_DEFAULT (1)
#define HI_ISP_TOP_BYPASS_FRAME_SWITCH_BUFFER_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_bypass_frame_switch_buffer_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0040);
    IOWR_8DIRECT_APICAL(0x0040, ((data & 0x01) << 7) | (u8Current & 0x7F));
}
static __inline HI_U8 hi_isp_top_bypass_frame_switch_buffer_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0040) & 0x80) >> 7);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_bypass_gain
// ------------------------------------------------------------------------------ //
// Bypass black level/gain
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_BYPASS_GAIN_DEFAULT (0)
#define HI_ISP_TOP_BYPASS_GAIN_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_bypass_gain_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0041);
    IOWR_8DIRECT_APICAL(0x0041, (data & 0x01) | (u8Current & 0xFE));
}
static __inline HI_U8 hi_isp_top_bypass_gain_read(HI_VOID) {
    return (IORD_8DIRECT(0x0041) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_bypass_radial_shading
// ------------------------------------------------------------------------------ //
// Bypass shading
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_BYPASS_RADIAL_SHADING_DEFAULT (0)
#define HI_ISP_TOP_BYPASS_RADIAL_SHADING_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_bypass_radial_shading_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0041);
    IOWR_8DIRECT_APICAL(0x0041, ((data & 0x01) << 1) | (u8Current & 0xFD));
}
static __inline HI_U8 hi_isp_top_bypass_radial_shading_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0041) & 0x02) >> 1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_bypass_drc
// ------------------------------------------------------------------------------ //
// Bypass Drc
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_BYPASS_DRC_DEFAULT (0)
#define HI_ISP_TOP_BYPASS_DRC_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_bypass_drc_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0041);
    IOWR_8DIRECT_APICAL(0x0041, ((data & 0x01) << 3) | (u8Current & 0xF7));
}
static __inline HI_U8 hi_isp_top_bypass_drc_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0041) & 0x08) >> 3);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_bypass_demosaic
// ------------------------------------------------------------------------------ //
// Bypass demosaic module (output RAW data)
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_BYPASS_DEMOSAIC_DEFAULT (0)
#define HI_ISP_TOP_BYPASS_DEMOSAIC_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_bypass_demosaic_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0041);
    IOWR_8DIRECT_APICAL(0x0041, ((data & 0x01) << 4) | (u8Current & 0xEF));
}
static __inline HI_U8 hi_isp_top_bypass_demosaic_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0041) & 0x10) >> 4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_bypass_color_matrix
// ------------------------------------------------------------------------------ //
// Bypass color matrix
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_BYPASS_COLOR_MATRIX_DEFAULT (0)
#define HI_ISP_TOP_BYPASS_COLOR_MATRIX_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_bypass_color_matrix_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0041);
    IOWR_8DIRECT_APICAL(0x0041, ((data & 0x01) << 5) | (u8Current & 0xDF));
}
static __inline HI_U8 hi_isp_top_bypass_color_matrix_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0041) & 0x20) >> 5);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_bypass_gamma_rgb
// ------------------------------------------------------------------------------ //
// Bypass gamma table
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_BYPASS_GAMMA_RGB_DEFAULT (0)
#define HI_ISP_TOP_BYPASS_GAMMA_RGB_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_bypass_gamma_rgb_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0041);
    IOWR_8DIRECT_APICAL(0x0041, ((data & 0x01) << 6) | (u8Current & 0xBF));	
}
static __inline HI_U8 hi_isp_top_bypass_gamma_rgb_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0041) & 0x40) >> 6);
}


// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_bypass_debuger
// ------------------------------------------------------------------------------ //
// Bypass Debuger
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_BYPASS_DEBUGER_DEFAULT (1)
#define HI_ISP_TOP_BYPASS_DEBUGER_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_bypass_debuger_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0042);
    IOWR_8DIRECT_APICAL(0x0042, ((data & 0x01) << 2) | (u8Current & 0xFB));
}
static __inline HI_U8 hi_isp_top_bypass_debuger_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0042) & 0x04) >> 2);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_bypass_sharpen
// ------------------------------------------------------------------------------ //
// Bypass Sharpening block 
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_BYPASS_SHARPEN_DEFAULT (0)
#define HI_ISP_TOP_BYPASS_SHARPEN_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_bypass_sharpen_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0042);
    IOWR_8DIRECT_APICAL(0x0042, ((data & 0x01) << 6) | (u8Current & 0xBF));	
}
static __inline HI_U8 hi_isp_top_bypass_sharpen_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0042) & 0x40) >> 6);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_bypass_cs_conv
// ------------------------------------------------------------------------------ //
// Bypass Colour Space Conversion
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_BYPASS_CS_CONV_DEFAULT (0)
#define HI_ISP_TOP_BYPASS_CS_CONV_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_bypass_cs_conv_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0042);
    IOWR_8DIRECT_APICAL(0x0042, ((data & 0x01) << 3) | (u8Current & 0xF7));	
}
static __inline HI_U8 hi_isp_top_bypass_cs_conv_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0042) & 0x08) >> 3);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_channel_switch
// ------------------------------------------------------------------------------ //
// Switch input channels 1 and 2
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_CHANNEL_SWITCH_DEFAULT (0)
#define HI_ISP_TOP_CHANNEL_SWITCH_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_channel_switch_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0042);
    IOWR_8DIRECT_APICAL(0x0042, ((data & 0x01) << 7) | (u8Current & 0x7F));
}
static __inline HI_U8 hi_isp_top_channel_switch_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0042) & 0x80) >> 7);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_isp_processing_bypass_mode
// ------------------------------------------------------------------------------ //
// 
//   0 = Full processing
//   1 = Bypass entire ISP processing (video input and output ports are still connected) and output raw sensor data
//   2 = Bypass entire ISP processing (video input and output ports are still connected) and output raw sensor data in most significant bits of channels 1 and 2
//   3 = Connect output to ground
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_ISP_PROCESSING_BYPASS_MODE_DEFAULT (0)
#define HI_ISP_TOP_ISP_PROCESSING_BYPASS_MODE_DATASIZE (2)

#define HI_ISP_TOP_ISP_PROCESSING_BYPASS_MODE_FULL_PROCESSING (0)
#define HI_ISP_TOP_ISP_PROCESSING_BYPASS_MODE_BYPASS_ENTIRE_ISP_PROCESSING_VIDEO_INPUT_AND_OUTPUT_PORTS_ARE_STILL_CONNECTED_AND_OUTPUT_RAW_SENSOR_DATA (1)
#define HI_ISP_TOP_ISP_PROCESSING_BYPASS_MODE_BYPASS_ENTIRE_ISP_PROCESSING_VIDEO_INPUT_AND_OUTPUT_PORTS_ARE_STILL_CONNECTED_AND_OUTPUT_RAW_SENSOR_DATA_IN_MOST_SIGNIFICANT_BITS_OF_CHANNELS_1_AND_2 (2)
#define HI_ISP_TOP_ISP_PROCESSING_BYPASS_MODE_CONNECT_OUTPUT_TO_GROUND (3)

// args: data (2-bit)
static __inline HI_VOID hi_isp_top_isp_processing_bypass_mode_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0043);
    IOWR_8DIRECT_APICAL(0x0043, (data & 0x03) | (u8Current & 0xFC));	
}
static __inline HI_U8 hi_isp_top_isp_processing_bypass_mode_read(HI_VOID) {
    return (IORD_8DIRECT(0x0043) & 0x03);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_isp_full_bypass_enable
// ------------------------------------------------------------------------------ //
// Output is connected directly to input
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_ISP_FULL_BYPASS_ENABLE_DEFAULT (0)
#define HI_ISP_TOP_ISP_FULL_BYPASS_ENABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_isp_full_bypass_enable_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0043);
    IOWR_8DIRECT_APICAL(0x0043, ((data & 0x01) << 2) | (u8Current & 0xFB));
}
static __inline HI_U8 hi_isp_top_isp_full_bypass_enable_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0043) & 0x04) >> 2);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_ae_switch
// ------------------------------------------------------------------------------ //
// AE tap in the pipeline
//   0 = After static WB
//   1 = Immediately from sensor, channel 1 (for WDR modes)
//   2 = After shading
//   3 = After frontend LUT, channel 1 (for WDR modes)
//   4 = After Drc
//   5 = Immediately from sensor, channel 2 (for WDR modes)
//   6 = After WDR stitch
//   7 = After frontend LUT, channel 2 (for WDR modes)
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_AE_SWITCH_DEFAULT (0)
#define HI_ISP_TOP_AE_SWITCH_DATASIZE (3)

#define HI_ISP_TOP_AE_SWITCH_AFTER_STATIC_WB (0)
#define HI_ISP_TOP_AE_SWITCH_IMMEDIATELY_FROM_SENSOR_CHANNEL_1_FOR_WDR_MODES_ (1)
#define HI_ISP_TOP_AE_SWITCH_AFTER_SHADING (2)
#define HI_ISP_TOP_AE_SWITCH_AFTER_FRONTEND_LUT_CHANNEL_1_FOR_WDR_MODES_ (3)
#define HI_ISP_TOP_AE_SWITCH_AFTER_DRC (4)
#define HI_ISP_TOP_AE_SWITCH_IMMEDIATELY_FROM_SENSOR_CHANNEL_2_FOR_WDR_MODES_ (5)
#define HI_ISP_TOP_AE_SWITCH_AFTER_WDR_STITCH (6)
#define HI_ISP_TOP_AE_SWITCH_AFTER_FRONTEND_LUT_CHANNEL_2_FOR_WDR_MODES_ (7)

// args: data (3-bit)
static __inline HI_VOID hi_isp_top_ae_switch_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x0044, (data & 0x07));
}
static __inline HI_U8 hi_isp_top_ae_switch_read(HI_VOID) {
    return (IORD_8DIRECT(0x0044) & 0x07);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_awb_switch
// ------------------------------------------------------------------------------ //
// AWB tap in the pipeline
//   0 = Immediately before Color Matrix
//   1 = Immediately after Color Matrix (for sensors with strong color channel crosstalk)
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_AWB_SWITCH_DEFAULT (0)
#define HI_ISP_TOP_AWB_SWITCH_DATASIZE (1)

#define HI_ISP_TOP_AWB_SWITCH_IMMEDIATELY_BEFORE_COLOR_MATRIX (0)
#define HI_ISP_TOP_AWB_SWITCH_IMMEDIATELY_AFTER_COLOR_MATRIX_FOR_SENSORS_WITH_STRONG_COLOR_CHANNEL_CROSSTALK_ (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_awb_switch_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x0048, (data & 0x01));
}
static __inline HI_U8 hi_isp_top_awb_switch_read(HI_VOID) {
    return (IORD_8DIRECT(0x0048) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_af_switch
// ------------------------------------------------------------------------------ //
// AF tap in the pipeline
//   0 = After demosaic
//   1 = After sharpen
//   2 = Disabled
//   3 = Disabled2
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_AF_SWITCH_DEFAULT (0)
#define HI_ISP_TOP_AF_SWITCH_DATASIZE (2)

#define HI_ISP_TOP_AF_SWITCH_AFTER_DEMOSAIC (0)
#define HI_ISP_TOP_AF_SWITCH_AFTER_SHARPEN (1)
#define HI_ISP_TOP_AF_SWITCH_DISABLED (2)
#define HI_ISP_TOP_AF_SWITCH_DISABLED2 (3)

// args: data (2-bit)
static __inline HI_VOID hi_isp_top_af_switch_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x004C, (data & 0x03));
}
static __inline HI_U8 hi_isp_top_af_switch_read(HI_VOID) {
    return (IORD_8DIRECT(0x004C) & 0x03);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_aesum_switch
// ------------------------------------------------------------------------------ //
// AE SUM tap in the pipeline
//   0 = After static WB
//   1 = Immediately from sensor, channel 1 (for WDR modes)
//   2 = After shading
//   3 = After frontend LUT, channel 1 (for WDR modes)
//   4 = After Drc
//   5 = Immediately from sensor, channel 2 (for WDR modes)
//   6 = After WDR stitch
//   7 = After frontend LUT, channel 2 (for WDR modes)
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_AESUM_SWITCH_DEFAULT (0)
#define HI_ISP_TOP_AESUM_SWITCH_DATASIZE (3)

#define HI_ISP_TOP_AESUM_SWITCH_AFTER_STATIC_WB (0)
#define HI_ISP_TOP_AESUM_SWITCH_IMMEDIATELY_FROM_SENSOR_CHANNEL_1_FOR_WDR_MODES_ (1)
#define HI_ISP_TOP_AESUM_SWITCH_AFTER_SHADING (2)
#define HI_ISP_TOP_AESUM_SWITCH_AFTER_FRONTEND_LUT_CHANNEL_1_FOR_WDR_MODES_ (3)
#define HI_ISP_TOP_AESUM_SWITCH_AFTER_DRC (4)
#define HI_ISP_TOP_AESUM_SWITCH_IMMEDIATELY_FROM_SENSOR_CHANNEL_2_FOR_WDR_MODES_ (5)
#define HI_ISP_TOP_AESUM_SWITCH_AFTER_WDR_STITCH (6)
#define HI_ISP_TOP_AESUM_SWITCH_AFTER_FRONTEND_LUT_CHANNEL_2_FOR_WDR_MODES_ (7)

// args: data (3-bit)
static __inline HI_VOID hi_isp_top_aesum_switch_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x0050, (data & 0x07));
}
static __inline HI_U8 hi_isp_top_aesum_switch_read(HI_VOID) {
    return (IORD_8DIRECT(0x0050) & 0x07);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_dis_switch
// ------------------------------------------------------------------------------ //
// DIS tap in the pipeline
//   0 = End of pipeline
//   1 = After Drc
//   2 = After Defect Pixel, channel 0
//   3 = After Defect Pixel, channel 1
//   4 = After static WB
//   5 = After RAdial Shading
//   6 = Switched off
//   7 = Switched off7
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_DIS_SWITCH_DEFAULT (0)
#define HI_ISP_TOP_DIS_SWITCH_DATASIZE (3)

#define HI_ISP_TOP_DIS_SWITCH_END_OF_PIPELINE (0)
#define HI_ISP_TOP_DIS_SWITCH_AFTER_DRC (1)
#define HI_ISP_TOP_DIS_SWITCH_AFTER_DEFECT_PIXEL_CHANNEL_0 (2)
#define HI_ISP_TOP_DIS_SWITCH_AFTER_DEFECT_PIXEL_CHANNEL_1 (3)
#define HI_ISP_TOP_DIS_SWITCH_AFTER_STATIC_WB (4)
#define HI_ISP_TOP_DIS_SWITCH_AFTER_RADIAL_SHADING (5)
#define HI_ISP_TOP_DIS_SWITCH_SWITCHED_OFF (6)
#define HI_ISP_TOP_DIS_SWITCH_SWITCHED_OFF7 (7)

// args: data (3-bit)
static __inline HI_VOID hi_isp_top_dis_switch_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x0052, (data & 0x07));
}
static __inline HI_U8 hi_isp_top_dis_switch_read(HI_VOID) {
    return (IORD_8DIRECT(0x0052) & 0x07);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_histogram_switch
// ------------------------------------------------------------------------------ //
// Histogram tap in the pipeline
//   0 = Same as AE
//   1 = Immediately from sensor, channel 1 (for WDR modes)
//   2 = After shading
//   3 = After frontend LUT, channel 1 (for WDR modes)
//   4 = After Drc
//   5 = Immediately from sensor, channel 2 (for WDR modes)
//   6 = After WDR stitch
//   7 = After frontend LUT, channel 2 (for WDR modes)
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_HISTOGRAM_SWITCH_DEFAULT (0)
#define HI_ISP_TOP_HISTOGRAM_SWITCH_DATASIZE (3)

#define HI_ISP_TOP_HISTOGRAM_SWITCH_SAME_AS_AE (0)
#define HI_ISP_TOP_HISTOGRAM_SWITCH_IMMEDIATELY_FROM_SENSOR_CHANNEL_1_FOR_WDR_MODES_ (1)
#define HI_ISP_TOP_HISTOGRAM_SWITCH_AFTER_SHADING (2)
#define HI_ISP_TOP_HISTOGRAM_SWITCH_AFTER_FRONTEND_LUT_CHANNEL_1_FOR_WDR_MODES_ (3)
#define HI_ISP_TOP_HISTOGRAM_SWITCH_AFTER_DRC (4)
#define HI_ISP_TOP_HISTOGRAM_SWITCH_IMMEDIATELY_FROM_SENSOR_CHANNEL_2_FOR_WDR_MODES_ (5)
#define HI_ISP_TOP_HISTOGRAM_SWITCH_AFTER_WDR_STITCH (6)
#define HI_ISP_TOP_HISTOGRAM_SWITCH_AFTER_FRONTEND_LUT_CHANNEL_2_FOR_WDR_MODES_ (7)

// args: data (3-bit)
static __inline HI_VOID hi_isp_top_histogram_switch_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x0054, (data & 0x07));
}
static __inline HI_U8 hi_isp_top_histogram_switch_read(HI_VOID) {
    return (IORD_8DIRECT(0x0054) & 0x07);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_field_status
// ------------------------------------------------------------------------------ //
// Status of field signal
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_FIELD_STATUS_DEFAULT (0)
#define HI_ISP_TOP_FIELD_STATUS_DATASIZE (1)

// args: data (1-bit)
static __inline HI_U8 hi_isp_top_field_status_read(HI_VOID) {
    return (IORD_8DIRECT(0x007C) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_top_global_fsm_reset
// ------------------------------------------------------------------------------ //
// 1= synchronous reset of FSMs in design (faster recovery after broken frame)
// ------------------------------------------------------------------------------ //

#define HI_ISP_TOP_GLOBAL_FSM_RESET_DEFAULT (0)
#define HI_ISP_TOP_GLOBAL_FSM_RESET_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_top_global_fsm_reset_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x0078, (data & 0x01));
}
static __inline HI_U8 hi_isp_top_global_fsm_reset_read(HI_VOID) {
    return (IORD_8DIRECT(0x0078) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_low_power_consumption_config
// ------------------------------------------------------------------------------ //
// 
// ------------------------------------------------------------------------------ //

#define HI_ISP_LOW_POWER_CONSUMPTION_CONFIG_DEFAULT (0x0)
#define HI_ISP_LOW_POWER_CONSUMPTION_CONFIG_DATASIZE (32)

// args: data (32-bit)
static __inline HI_VOID hi_isp_low_power_consumption_config_write(HI_U32 data) {
    IOWR_32DIRECT(0x0058, data);
}
static __inline HI_U8 hi_isp_low_power_consumption_config_read(HI_VOID) {
    return IORD_32DIRECT(0x0058);
}

// ------------------------------------------------------------------------------ //
// Group: Interrupts
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_isp_interrupts_interrupt0_source
// ------------------------------------------------------------------------------ //
// Interrupt source selector
// ------------------------------------------------------------------------------ //

#define HI_ISP_INTERRUPTS_INTERRUPT0_SOURCE_DEFAULT (0x0)
#define HI_ISP_INTERRUPTS_INTERRUPT0_SOURCE_DATASIZE (5)
#define HI_ISP_INTERRUPTS_INTERRUPT0_SOURCE_VALUE (10)

// args: data (5-bit)
static __inline HI_VOID hi_isp_interrupts_interrupt0_source_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x0080, (data & 0x1F));
}
static __inline HI_U8 hi_isp_interrupts_interrupt0_source_read(HI_VOID) {
    return (IORD_8DIRECT(0x0080) & 0x1F);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_interrupts_interrupt1_source
// ------------------------------------------------------------------------------ //
// Interrupt source selector
// ------------------------------------------------------------------------------ //

#define HI_ISP_INTERRUPTS_INTERRUPT1_SOURCE_DEFAULT (0x0)
#define HI_ISP_INTERRUPTS_INTERRUPT1_SOURCE_DATASIZE (5)

// args: data (5-bit)
static __inline HI_VOID hi_isp_interrupts_interrupt1_source_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x0084, (data & 0x1F));
}
static __inline HI_U8 hi_isp_interrupts_interrupt1_source_read(HI_VOID) {
    return (IORD_8DIRECT(0x0084) & 0x1F);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_interrupts_interrupt2_source
// ------------------------------------------------------------------------------ //
// Interrupt source selector
// ------------------------------------------------------------------------------ //

#define HI_ISP_INTERRUPTS_INTERRUPT2_SOURCE_DEFAULT (0x0)
#define HI_ISP_INTERRUPTS_INTERRUPT2_SOURCE_DATASIZE (5)

// args: data (5-bit)
static __inline HI_VOID hi_isp_interrupts_interrupt2_source_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x0088, (data & 0x1F));
}
static __inline HI_U8 hi_isp_interrupts_interrupt2_source_read(HI_VOID) {
    return (IORD_8DIRECT(0x0088) & 0x1F);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_interrupts_interrupt3_source
// ------------------------------------------------------------------------------ //
// Interrupt source selector
// ------------------------------------------------------------------------------ //

#define HI_ISP_INTERRUPTS_INTERRUPT3_SOURCE_DEFAULT (0x0)
#define HI_ISP_INTERRUPTS_INTERRUPT3_SOURCE_DATASIZE (5)

// args: data (5-bit)
static __inline HI_VOID hi_isp_interrupts_interrupt3_source_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x008C, (data & 0x1F));
}
static __inline HI_U8 hi_isp_interrupts_interrupt3_source_read(HI_VOID) {
    return (IORD_8DIRECT(0x008C) & 0x1F);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_interrupts_interrupt4_source
// ------------------------------------------------------------------------------ //
// Interrupt source selector
// ------------------------------------------------------------------------------ //

#define HI_ISP_INTERRUPTS_INTERRUPT4_SOURCE_DEFAULT (0x0)
#define HI_ISP_INTERRUPTS_INTERRUPT4_SOURCE_DATASIZE (5)

// args: data (5-bit)
static __inline HI_VOID hi_isp_interrupts_interrupt4_source_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x0090, (data & 0x1F));
}
static __inline HI_U8 hi_isp_interrupts_interrupt4_source_read(HI_VOID) {
    return (IORD_8DIRECT(0x0090) & 0x1F);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_interrupts_interrupt5_source
// ------------------------------------------------------------------------------ //
// Interrupt source selector
// ------------------------------------------------------------------------------ //

#define HI_ISP_INTERRUPTS_INTERRUPT5_SOURCE_DEFAULT (0x0)
#define HI_ISP_INTERRUPTS_INTERRUPT5_SOURCE_DATASIZE (5)

// args: data (5-bit)
static __inline HI_VOID hi_isp_interrupts_interrupt5_source_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x0094, (data & 0x1F));
}
static __inline HI_U8 hi_isp_interrupts_interrupt5_source_read(HI_VOID) {
    return (IORD_8DIRECT(0x0094) & 0x1F);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_interrupts_interrupt6_source
// ------------------------------------------------------------------------------ //
// Interrupt source selector
// ------------------------------------------------------------------------------ //

#define HI_ISP_INTERRUPTS_INTERRUPT6_SOURCE_DEFAULT (0x0)
#define HI_ISP_INTERRUPTS_INTERRUPT6_SOURCE_DATASIZE (5)

// args: data (5-bit)
static __inline HI_VOID hi_isp_interrupts_interrupt6_source_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x0098, (data & 0x1F));
}
static __inline HI_U8 hi_isp_interrupts_interrupt6_source_read(HI_VOID) {
    return (IORD_8DIRECT(0x0098) & 0x1F);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_interrupts_interrupt7_source
// ------------------------------------------------------------------------------ //
// Interrupt source selector
// ------------------------------------------------------------------------------ //

#define HI_ISP_INTERRUPTS_INTERRUPT7_SOURCE_DEFAULT (0x0)
#define HI_ISP_INTERRUPTS_INTERRUPT7_SOURCE_DATASIZE (5)

// args: data (5-bit)
static __inline HI_VOID hi_isp_interrupts_interrupt7_source_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x009C, (data & 0x1F));
}
static __inline HI_U8 hi_isp_interrupts_interrupt7_source_read(HI_VOID) {
    return (IORD_8DIRECT(0x009C) & 0x1F);
}

// ------------------------------------------------------------------------------ //
// Group: Input port
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_preset
// ------------------------------------------------------------------------------ //
// 0-14 frequently used presets, 15 - full control
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_PRESET_DEFAULT (2)
#define HI_ISP_INPUT_PORT_PRESET_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_isp_input_port_preset_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x0100, (data & 0x0F));
}
static __inline HI_U8 hi_isp_input_port_preset_read(HI_VOID) {
    return (IORD_8DIRECT(0x0100) & 0x0F);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_vs_use_field
// ------------------------------------------------------------------------------ //
// 
//   0 = use vsync_i port for vertical sync
//   1 = use field_i port for vertical sync
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_VS_USE_FIELD_DEFAULT (0)
#define HI_ISP_INPUT_PORT_VS_USE_FIELD_DATASIZE (1)

#define HI_ISP_INPUT_PORT_VS_USE_FIELD_USE_VSYNC_I_PORT_FOR_VERTICAL_SYNC (0)
#define HI_ISP_INPUT_PORT_VS_USE_FIELD_USE_FIELD_I_PORT_FOR_VERTICAL_SYNC (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_vs_use_field_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0101);
    IOWR_8DIRECT_APICAL(0x0101, (data & 0x01) | (u8Current & 0xFE));
}
static __inline HI_U8 hi_isp_input_port_vs_use_field_read(HI_VOID) {
    return (IORD_8DIRECT(0x0101) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_vs_toggle
// ------------------------------------------------------------------------------ //
// 
//   0 = vsync is pulse-type
//   1 = vsync is toggle-type (field signal)
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_VS_TOGGLE_DEFAULT (0)
#define HI_ISP_INPUT_PORT_VS_TOGGLE_DATASIZE (1)

#define HI_ISP_INPUT_PORT_VS_TOGGLE_VSYNC_IS_PULSE_TYPE (0)
#define HI_ISP_INPUT_PORT_VS_TOGGLE_VSYNC_IS_TOGGLE_TYPE_FIELD_SIGNAL_ (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_vs_toggle_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0101);
    IOWR_8DIRECT_APICAL(0x0101, ((data & 0x01) << 1) | (u8Current & 0xFD));
}
static __inline HI_U8 hi_isp_input_port_vs_toggle_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0101) & 0x02) >> 1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_vs_polarity
// ------------------------------------------------------------------------------ //
// 
//   0 = horizontal counter reset on rising edge
//   1 = horizontal counter reset on falling edge
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_VS_POLARITY_DEFAULT (0)
#define HI_ISP_INPUT_PORT_VS_POLARITY_DATASIZE (1)

#define HI_ISP_INPUT_PORT_VS_POLARITY_HORIZONTAL_COUNTER_RESET_ON_RISING_EDGE (0)
#define HI_ISP_INPUT_PORT_VS_POLARITY_HORIZONTAL_COUNTER_RESET_ON_FALLING_EDGE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_vs_polarity_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0101);
    IOWR_8DIRECT_APICAL(0x0101, ((data & 0x01) << 2) | (u8Current & 0xFB));
}
static __inline HI_U8 hi_isp_input_port_vs_polarity_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0101) & 0x04) >> 2);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_vs_polarity_acl
// ------------------------------------------------------------------------------ //
// 
//   0 = don't invert polarity for ACL gate
//   1 = invert polarity for ACL gate
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_VS_POLARITY_ACL_DEFAULT (0)
#define HI_ISP_INPUT_PORT_VS_POLARITY_ACL_DATASIZE (1)

#define HI_ISP_INPUT_PORT_VS_POLARITY_ACL_DON_T_INVERT_POLARITY_FOR_ACL_GATE (0)
#define HI_ISP_INPUT_PORT_VS_POLARITY_ACL_INVERT_POLARITY_FOR_ACL_GATE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_vs_polarity_acl_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0101);
    IOWR_8DIRECT_APICAL(0x0101, ((data & 0x01) << 3) | (u8Current & 0xF7));
}
static __inline HI_U8 hi_isp_input_port_vs_polarity_acl_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0101) & 0x08) >> 3);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_hs_use_acl
// ------------------------------------------------------------------------------ //
// 
//   0 = use hsync_i port for active-line
//   1 = use acl_i port for active-line
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_HS_USE_ACL_DEFAULT (0)
#define HI_ISP_INPUT_PORT_HS_USE_ACL_DATASIZE (1)

#define HI_ISP_INPUT_PORT_HS_USE_ACL_USE_HSYNC_I_PORT_FOR_ACTIVE_LINE (0)
#define HI_ISP_INPUT_PORT_HS_USE_ACL_USE_ACL_I_PORT_FOR_ACTIVE_LINE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_hs_use_acl_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0101);
    IOWR_8DIRECT_APICAL(0x0101, ((data & 0x01) << 4) | (u8Current & 0xEF));
}
static __inline HI_U8 hi_isp_input_port_hs_use_acl_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0101) & 0x10) >> 4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_vc_c_select
// ------------------------------------------------------------------------------ //
// 
//   0 = vertical counter counts on hs
//   1 = vertical counter counts on horizontal counter overflow or reset
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_VC_C_SELECT_DEFAULT (0)
#define HI_ISP_INPUT_PORT_VC_C_SELECT_DATASIZE (1)

#define HI_ISP_INPUT_PORT_VC_C_SELECT_VERTICAL_COUNTER_COUNTS_ON_HS (0)
#define HI_ISP_INPUT_PORT_VC_C_SELECT_VERTICAL_COUNTER_COUNTS_ON_HORIZONTAL_COUNTER_OVERFLOW_OR_RESET (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_vc_c_select_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0101);
    IOWR_8DIRECT_APICAL(0x0101, ((data & 0x01) << 6) | (u8Current & 0xBF));
}
static __inline HI_U8 hi_isp_input_port_vc_c_select_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0101) & 0x40) >> 6);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_vc_r_select
// ------------------------------------------------------------------------------ //
// 
//   0 = vertical counter is reset on edge of vs
//   1 = vertical counter is reset after timeout on hs
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_VC_R_SELECT_DEFAULT (0)
#define HI_ISP_INPUT_PORT_VC_R_SELECT_DATASIZE (1)

#define HI_ISP_INPUT_PORT_VC_R_SELECT_VERTICAL_COUNTER_IS_RESET_ON_EDGE_OF_VS (0)
#define HI_ISP_INPUT_PORT_VC_R_SELECT_VERTICAL_COUNTER_IS_RESET_AFTER_TIMEOUT_ON_HS (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_vc_r_select_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0101);
    IOWR_8DIRECT_APICAL(0x0101, ((data & 0x01) << 7) | (u8Current & 0x7F));
}
static __inline HI_U8 hi_isp_input_port_vc_r_select_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0101) & 0x80) >> 7);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_hs_xor_vs
// ------------------------------------------------------------------------------ //
// 
//   0 = normal mode
//   1 = hvalid = hsync XOR vsync
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_HS_XOR_VS_DEFAULT (0)
#define HI_ISP_INPUT_PORT_HS_XOR_VS_DATASIZE (1)

#define HI_ISP_INPUT_PORT_HS_XOR_VS_NORMAL_MODE (0)
#define HI_ISP_INPUT_PORT_HS_XOR_VS_HVALID_HSYNC_XOR_VSYNC (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_hs_xor_vs_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0102);
    IOWR_8DIRECT_APICAL(0x0102, (data & 0x01) | (u8Current & 0xFE));
}
static __inline HI_U8 hi_isp_input_port_hs_xor_vs_read(HI_VOID) {
    return (IORD_8DIRECT(0x0102) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_hs_polarity
// ------------------------------------------------------------------------------ //
// 
//   0 = don't invert polarity of HS for ACL gate
//   1 = invert polarity of HS for ACL gate
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_HS_POLARITY_DEFAULT (0)
#define HI_ISP_INPUT_PORT_HS_POLARITY_DATASIZE (1)

#define HI_ISP_INPUT_PORT_HS_POLARITY_DON_T_INVERT_POLARITY_OF_HS_FOR_ACL_GATE (0)
#define HI_ISP_INPUT_PORT_HS_POLARITY_INVERT_POLARITY_OF_HS_FOR_ACL_GATE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_hs_polarity_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0102);
    IOWR_8DIRECT_APICAL(0x0102, ((data & 0x01) << 1) | (u8Current & 0xFD));
}
static __inline HI_U8 hi_isp_input_port_hs_polarity_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0102) & 0x02) >> 1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_hs_polarity_acl
// ------------------------------------------------------------------------------ //
// 
//   0 = don't invert polarity of HS for HS gate
//   1 = invert polarity of HS for HS gate
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_HS_POLARITY_ACL_DEFAULT (0)
#define HI_ISP_INPUT_PORT_HS_POLARITY_ACL_DATASIZE (1)

#define HI_ISP_INPUT_PORT_HS_POLARITY_ACL_DON_T_INVERT_POLARITY_OF_HS_FOR_HS_GATE (0)
#define HI_ISP_INPUT_PORT_HS_POLARITY_ACL_INVERT_POLARITY_OF_HS_FOR_HS_GATE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_hs_polarity_acl_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0102);
    IOWR_8DIRECT_APICAL(0x0102, ((data & 0x01) << 2) | (u8Current & 0xFB));
}
static __inline HI_U8 hi_isp_input_port_hs_polarity_acl_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0102) & 0x04) >> 2);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_hs_polarity_hs
// ------------------------------------------------------------------------------ //
// 
//   0 = horizontal counter is reset on rising edge of hs
//   1 = horizontal counter is reset on vsync (e.g. when hsync is not available)
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_HS_POLARITY_HS_DEFAULT (0)
#define HI_ISP_INPUT_PORT_HS_POLARITY_HS_DATASIZE (1)

#define HI_ISP_INPUT_PORT_HS_POLARITY_HS_HORIZONTAL_COUNTER_IS_RESET_ON_RISING_EDGE_OF_HS (0)
#define HI_ISP_INPUT_PORT_HS_POLARITY_HS_HORIZONTAL_COUNTER_IS_RESET_ON_VSYNC_E_G_WHEN_HSYNC_IS_NOT_AVAILABLE_ (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_hs_polarity_hs_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0102);
    IOWR_8DIRECT_APICAL(0x0102, ((data & 0x01) << 3) | (u8Current & 0xF7));
}
static __inline HI_U8 hi_isp_input_port_hs_polarity_hs_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0102) & 0x08) >> 3);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_hs_polarity_vc
// ------------------------------------------------------------------------------ //
// 
//   0 = vertical counter increments on rising edge of HS
//   1 = vertical counter increments on falling edge of HS
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_HS_POLARITY_VC_DEFAULT (1)
#define HI_ISP_INPUT_PORT_HS_POLARITY_VC_DATASIZE (1)

#define HI_ISP_INPUT_PORT_HS_POLARITY_VC_VERTICAL_COUNTER_INCREMENTS_ON_RISING_EDGE_OF_HS (0)
#define HI_ISP_INPUT_PORT_HS_POLARITY_VC_VERTICAL_COUNTER_INCREMENTS_ON_FALLING_EDGE_OF_HS (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_hs_polarity_vc_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0102);
    IOWR_8DIRECT_APICAL(0x0102, ((data & 0x01) << 4) | (u8Current & 0xEF));
}
static __inline HI_U8 hi_isp_input_port_hs_polarity_vc_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0102) & 0x10) >> 4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_hc_r_select
// ------------------------------------------------------------------------------ //
// 
//   0 = vertical counter is reset on rising edge of hs
//   1 = vertical counter is reset on rising edge of vs
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_HC_R_SELECT_DEFAULT (0)
#define HI_ISP_INPUT_PORT_HC_R_SELECT_DATASIZE (1)

#define HI_ISP_INPUT_PORT_HC_R_SELECT_VERTICAL_COUNTER_IS_RESET_ON_RISING_EDGE_OF_HS (0)
#define HI_ISP_INPUT_PORT_HC_R_SELECT_VERTICAL_COUNTER_IS_RESET_ON_RISING_EDGE_OF_VS (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_hc_r_select_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0102);
    IOWR_8DIRECT_APICAL(0x0102, ((data & 0x01) << 7) | (u8Current & 0x7F));
}
static __inline HI_U8 hi_isp_input_port_hc_r_select_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0102) & 0x80) >> 7);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_acl_polarity
// ------------------------------------------------------------------------------ //
// 
//   0 = don't invert acl_i for acl gate
//   1 = invert acl_i for acl gate
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_ACL_POLARITY_DEFAULT (0)
#define HI_ISP_INPUT_PORT_ACL_POLARITY_DATASIZE (1)

#define HI_ISP_INPUT_PORT_ACL_POLARITY_DON_T_INVERT_ACL_I_FOR_ACL_GATE (0)
#define HI_ISP_INPUT_PORT_ACL_POLARITY_INVERT_ACL_I_FOR_ACL_GATE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_acl_polarity_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x0103, (data & 0x01));
}
static __inline HI_U8 hi_isp_input_port_acl_polarity_read(HI_VOID) {
    return (IORD_8DIRECT(0x0103) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_signal_flag
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_SIGNAL_FLAG_DEFAULT (0)
#define HI_ISP_INPUT_PORT_SIGNAL_FLAG_DATASIZE (32)

// args: data (32-bit)
static __inline HI_VOID hi_isp_input_port_signal_flag_write(HI_U32 data) {
    IOWR_32DIRECT_APICAL(0x0104, data);
}
static __inline HI_U32 hi_isp_input_port_signal_flag_read(HI_VOID) {
    return IORD_32DIRECT(0x0104);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_field_polarity
// ------------------------------------------------------------------------------ //
// 
//   0 = don't invert field_i for field gate
//   1 = invert field_i for field gate
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_FIELD_POLARITY_DEFAULT (0)
#define HI_ISP_INPUT_PORT_FIELD_POLARITY_DATASIZE (1)

#define HI_ISP_INPUT_PORT_FIELD_POLARITY_DON_T_INVERT_FIELD_I_FOR_FIELD_GATE (0)
#define HI_ISP_INPUT_PORT_FIELD_POLARITY_INVERT_FIELD_I_FOR_FIELD_GATE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_field_polarity_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0104);
    IOWR_8DIRECT_APICAL(0x0104, (data & 0x01) | (u8Current & 0xFE));
}
static __inline HI_U8 hi_isp_input_port_field_polarity_read(HI_VOID) {
    return (IORD_8DIRECT(0x0104) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_field_toggle
// ------------------------------------------------------------------------------ //
// 
//   0 = field is pulse-type
//   1 = field is toggle-type
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_FIELD_TOGGLE_DEFAULT (0)
#define HI_ISP_INPUT_PORT_FIELD_TOGGLE_DATASIZE (1)

#define HI_ISP_INPUT_PORT_FIELD_TOGGLE_FIELD_IS_PULSE_TYPE (0)
#define HI_ISP_INPUT_PORT_FIELD_TOGGLE_FIELD_IS_TOGGLE_TYPE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_field_toggle_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0104);
    IOWR_8DIRECT_APICAL(0x0104, ((data & 0x01) << 1) | (u8Current & 0xFD));
}
static __inline HI_U8 hi_isp_input_port_field_toggle_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0104) & 0x02) >> 1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_aclg_window0
// ------------------------------------------------------------------------------ //
// 
//   0 = exclude window0 signal in ACL gate
//   1 = include window0 signal in ACL gate
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_ACLG_WINDOW0_DEFAULT (0)
#define HI_ISP_INPUT_PORT_ACLG_WINDOW0_DATASIZE (1)

#define HI_ISP_INPUT_PORT_ACLG_WINDOW0_EXCLUDE_WINDOW0_SIGNAL_IN_ACL_GATE (0)
#define HI_ISP_INPUT_PORT_ACLG_WINDOW0_INCLUDE_WINDOW0_SIGNAL_IN_ACL_GATE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_aclg_window0_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0105);
    IOWR_8DIRECT_APICAL(0x0105, (data & 0x01) | (u8Current & 0xFE));
}
static __inline HI_U8 hi_isp_input_port_aclg_window0_read(HI_VOID) {
    return (IORD_8DIRECT(0x0105) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_aclg_hsync
// ------------------------------------------------------------------------------ //
// 
//   0 = exclude hsync signal in ACL gate
//   1 = include hsync signal in ACL gate
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_ACLG_HSYNC_DEFAULT (0)
#define HI_ISP_INPUT_PORT_ACLG_HSYNC_DATASIZE (1)

#define HI_ISP_INPUT_PORT_ACLG_HSYNC_EXCLUDE_HSYNC_SIGNAL_IN_ACL_GATE (0)
#define HI_ISP_INPUT_PORT_ACLG_HSYNC_INCLUDE_HSYNC_SIGNAL_IN_ACL_GATE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_aclg_hsync_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0105);
    IOWR_8DIRECT_APICAL(0x0105, ((data & 0x01) << 1) | (u8Current & 0xFD));
}
static __inline HI_U8 hi_isp_input_port_aclg_hsync_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0105) & 0x02) >> 1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_aclg_window2
// ------------------------------------------------------------------------------ //
// 
//   0 = exclude window2 signal in ACL gate
//   1 = include window2 signal in ACL gate
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_ACLG_WINDOW2_DEFAULT (0)
#define HI_ISP_INPUT_PORT_ACLG_WINDOW2_DATASIZE (1)

#define HI_ISP_INPUT_PORT_ACLG_WINDOW2_EXCLUDE_WINDOW2_SIGNAL_IN_ACL_GATE (0)
#define HI_ISP_INPUT_PORT_ACLG_WINDOW2_INCLUDE_WINDOW2_SIGNAL_IN_ACL_GATE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_aclg_window2_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0105);
    IOWR_8DIRECT_APICAL(0x0105, ((data & 0x01) << 2) | (u8Current & 0xFB));
}
static __inline HI_U8 hi_isp_input_port_aclg_window2_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0105) & 0x04) >> 2);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_aclg_acl
// ------------------------------------------------------------------------------ //
// 
//   0 = exclude acl_i signal in ACL gate
//   1 = include acl_i signal in ACL gate
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_ACLG_ACL_DEFAULT (0)
#define HI_ISP_INPUT_PORT_ACLG_ACL_DATASIZE (1)

#define HI_ISP_INPUT_PORT_ACLG_ACL_EXCLUDE_ACL_I_SIGNAL_IN_ACL_GATE (0)
#define HI_ISP_INPUT_PORT_ACLG_ACL_INCLUDE_ACL_I_SIGNAL_IN_ACL_GATE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_aclg_acl_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0105);
    IOWR_8DIRECT_APICAL(0x0105, ((data & 0x01) << 3) | (u8Current & 0xF7));
}
static __inline HI_U8 hi_isp_input_port_aclg_acl_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0105) & 0x08) >> 3);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_aclg_vsync
// ------------------------------------------------------------------------------ //
// 
//   0 = exclude vsync signal in ACL gate
//   1 = include vsync signal in ACL gate
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_ACLG_VSYNC_DEFAULT (0)
#define HI_ISP_INPUT_PORT_ACLG_VSYNC_DATASIZE (1)

#define HI_ISP_INPUT_PORT_ACLG_VSYNC_EXCLUDE_VSYNC_SIGNAL_IN_ACL_GATE (0)
#define HI_ISP_INPUT_PORT_ACLG_VSYNC_INCLUDE_VSYNC_SIGNAL_IN_ACL_GATE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_aclg_vsync_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0105);
    IOWR_8DIRECT_APICAL(0x0105, ((data & 0x01) << 4) | (u8Current & 0xEF));
}
static __inline HI_U8 hi_isp_input_port_aclg_vsync_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0105) & 0x10) >> 4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_hsg_window1
// ------------------------------------------------------------------------------ //
// 
//   0 = exclude window1 signal in HS gate
//   1 = include window1 signal in HS gate
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_HSG_WINDOW1_DEFAULT (0)
#define HI_ISP_INPUT_PORT_HSG_WINDOW1_DATASIZE (1)

#define HI_ISP_INPUT_PORT_HSG_WINDOW1_EXCLUDE_WINDOW1_SIGNAL_IN_HS_GATE (0)
#define HI_ISP_INPUT_PORT_HSG_WINDOW1_INCLUDE_WINDOW1_SIGNAL_IN_HS_GATE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_hsg_window1_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0106);
    IOWR_8DIRECT_APICAL(0x0106, (data & 0x01) | (u8Current & 0xFE));
}
static __inline HI_U8 hi_isp_input_port_hsg_window1_read(HI_VOID) {
    return (IORD_8DIRECT(0x0106) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_hsg_hsync
// ------------------------------------------------------------------------------ //
// 
//   0 = exclude hsync signal in HS gate
//   1 = include hsync signal in HS gate
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_HSG_HSYNC_DEFAULT (0)
#define HI_ISP_INPUT_PORT_HSG_HSYNC_DATASIZE (1)

#define HI_ISP_INPUT_PORT_HSG_HSYNC_EXCLUDE_HSYNC_SIGNAL_IN_HS_GATE (0)
#define HI_ISP_INPUT_PORT_HSG_HSYNC_INCLUDE_HSYNC_SIGNAL_IN_HS_GATE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_hsg_hsync_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0106);
    IOWR_8DIRECT_APICAL(0x0106, ((data & 0x01) << 1) | (u8Current & 0xFD));
}
static __inline HI_U8 hi_isp_input_port_hsg_hsync_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0106) & 0x02) >> 1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_hsg_vsync
// ------------------------------------------------------------------------------ //
// 
//   0 = exclude vsync signal in HS gate
//   1 = include vsync signal in HS gate
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_HSG_VSYNC_DEFAULT (0)
#define HI_ISP_INPUT_PORT_HSG_VSYNC_DATASIZE (1)

#define HI_ISP_INPUT_PORT_HSG_VSYNC_EXCLUDE_VSYNC_SIGNAL_IN_HS_GATE (0)
#define HI_ISP_INPUT_PORT_HSG_VSYNC_INCLUDE_VSYNC_SIGNAL_IN_HS_GATE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_hsg_vsync_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0106);
    IOWR_8DIRECT_APICAL(0x0106, ((data & 0x01) << 2) | (u8Current & 0xFB));
}
static __inline HI_U8 hi_isp_input_port_hsg_vsync_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0106) & 0x04) >> 2);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_hsg_window2
// ------------------------------------------------------------------------------ //
// 
//   0 = exclude window2 signal in HS gate
//   1 = include window2 signal in HS gate (mask out spurious hs during blank)
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_HSG_WINDOW2_DEFAULT (0)
#define HI_ISP_INPUT_PORT_HSG_WINDOW2_DATASIZE (1)

#define HI_ISP_INPUT_PORT_HSG_WINDOW2_EXCLUDE_WINDOW2_SIGNAL_IN_HS_GATE (0)
#define HI_ISP_INPUT_PORT_HSG_WINDOW2_INCLUDE_WINDOW2_SIGNAL_IN_HS_GATE_MASK_OUT_SPURIOUS_HS_DURING_BLANK_ (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_hsg_window2_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0106);
    IOWR_8DIRECT_APICAL(0x0106, ((data & 0x01) << 3) | (u8Current & 0xF7));
}
static __inline HI_U8 hi_isp_input_port_hsg_window2_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0106) & 0x08) >> 3);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_fieldg_vsync
// ------------------------------------------------------------------------------ //
// 
//   0 = exclude vsync signal in Field gate
//   1 = include vsync signal in Field gate
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_FIELDG_VSYNC_DEFAULT (0)
#define HI_ISP_INPUT_PORT_FIELDG_VSYNC_DATASIZE (1)

#define HI_ISP_INPUT_PORT_FIELDG_VSYNC_EXCLUDE_VSYNC_SIGNAL_IN_FIELD_GATE (0)
#define HI_ISP_INPUT_PORT_FIELDG_VSYNC_INCLUDE_VSYNC_SIGNAL_IN_FIELD_GATE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_fieldg_vsync_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0107);
    IOWR_8DIRECT_APICAL(0x0107, (data & 0x01) | (u8Current & 0xFE));
}
static __inline HI_U8 hi_isp_input_port_fieldg_vsync_read(HI_VOID) {
    return (IORD_8DIRECT(0x0107) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_fieldg_window2
// ------------------------------------------------------------------------------ //
// 
//   0 = exclude window2 signal in Field gate
//   1 = include window2 signal in Field gate
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_FIELDG_WINDOW2_DEFAULT (0)
#define HI_ISP_INPUT_PORT_FIELDG_WINDOW2_DATASIZE (1)

#define HI_ISP_INPUT_PORT_FIELDG_WINDOW2_EXCLUDE_WINDOW2_SIGNAL_IN_FIELD_GATE (0)
#define HI_ISP_INPUT_PORT_FIELDG_WINDOW2_INCLUDE_WINDOW2_SIGNAL_IN_FIELD_GATE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_fieldg_window2_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0107);
    IOWR_8DIRECT_APICAL(0x0107, ((data & 0x01) << 1) | (u8Current & 0xFD));
}
static __inline HI_U8 hi_isp_input_port_fieldg_window2_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0107) & 0x02) >> 1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_fieldg_field
// ------------------------------------------------------------------------------ //
// 
//   0 = exclude field_i signal in Field gate
//   1 = include field_i signal in Field gate
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_FIELDG_FIELD_DEFAULT (0)
#define HI_ISP_INPUT_PORT_FIELDG_FIELD_DATASIZE (1)

#define HI_ISP_INPUT_PORT_FIELDG_FIELD_EXCLUDE_FIELD_I_SIGNAL_IN_FIELD_GATE (0)
#define HI_ISP_INPUT_PORT_FIELDG_FIELD_INCLUDE_FIELD_I_SIGNAL_IN_FIELD_GATE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_fieldg_field_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0107);
    IOWR_8DIRECT_APICAL(0x0107, ((data & 0x01) << 2) | (u8Current & 0xFB));
}
static __inline HI_U8 hi_isp_input_port_fieldg_field_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0107) & 0x04) >> 2);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_field_mode
// ------------------------------------------------------------------------------ //
// 
//   0 = pulse field
//   1 = toggle field
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_FIELD_MODE_DEFAULT (0)
#define HI_ISP_INPUT_PORT_FIELD_MODE_DATASIZE (1)

#define HI_ISP_INPUT_PORT_FIELD_MODE_PULSE_FIELD (0)
#define HI_ISP_INPUT_PORT_FIELD_MODE_TOGGLE_FIELD (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_field_mode_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0107);
    IOWR_8DIRECT_APICAL(0x0107, ((data & 0x01) << 3) | (u8Current & 0xF7));
}
static __inline HI_U8 hi_isp_input_port_field_mode_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0107) & 0x08) >> 3);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_hc_limit
// ------------------------------------------------------------------------------ //
// horizontal counter limit value (counts: 0,1,...hc_limit-1,hc_limit,0,1,...)
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_HC_LIMIT_DEFAULT (0xFFFF)
#define HI_ISP_INPUT_PORT_HC_LIMIT_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_input_port_hc_limit_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0108, data);
}
static __inline HI_U16 hi_isp_input_port_hc_limit_read(HI_VOID) {
    return IORD_16DIRECT(0x0108);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_hc_start0
// ------------------------------------------------------------------------------ //
// window0 start for ACL gate
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_HC_START0_DEFAULT (0)
#define HI_ISP_INPUT_PORT_HC_START0_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_input_port_hc_start0_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x010C, data);
}
static __inline HI_U16 hi_isp_input_port_hc_start0_read(HI_VOID) {
    return IORD_16DIRECT(0x010C);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_hc_size0
// ------------------------------------------------------------------------------ //
// window0 size for ACL gate
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_HC_SIZE0_DEFAULT (0)
#define HI_ISP_INPUT_PORT_HC_SIZE0_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_input_port_hc_size0_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0110, data);
}
static __inline HI_U16 hi_isp_input_port_hc_size0_read(HI_VOID) {
    return IORD_16DIRECT(0x0110);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_hc_start1
// ------------------------------------------------------------------------------ //
// window1 start for HS gate
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_HC_START1_DEFAULT (0)
#define HI_ISP_INPUT_PORT_HC_START1_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_input_port_hc_start1_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0114, data);
}
static __inline HI_U16 hi_isp_input_port_hc_start1_read(HI_VOID) {
    return IORD_16DIRECT(0x0114);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_hc_size1
// ------------------------------------------------------------------------------ //
// window1 size for HS gate
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_HC_SIZE1_DEFAULT (0)
#define HI_ISP_INPUT_PORT_HC_SIZE1_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_input_port_hc_size1_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0118, data);
}
static __inline HI_U16 hi_isp_input_port_hc_size1_read(HI_VOID) {
    return IORD_16DIRECT(0x0118);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_vc_limit
// ------------------------------------------------------------------------------ //
// vertical counter limit value (counts: 0,1,...vc_limit-1,vc_limit,0,1,...)
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_VC_LIMIT_DEFAULT (0xFFFF)
#define HI_ISP_INPUT_PORT_VC_LIMIT_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_input_port_vc_limit_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x011C, data);
}
static __inline HI_U16 hi_isp_input_port_vc_limit_read(HI_VOID) {
    return IORD_16DIRECT(0x011C);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_vc_start
// ------------------------------------------------------------------------------ //
// window2 start for ACL gate
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_VC_START_DEFAULT (0)
#define HI_ISP_INPUT_PORT_VC_START_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_input_port_vc_start_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0120, data);
}
static __inline HI_U16 hi_isp_input_port_vc_start_read(HI_VOID) {
    return IORD_16DIRECT(0x0120);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_vc_size
// ------------------------------------------------------------------------------ //
// window2 size for ACL gate
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_VC_SIZE_DEFAULT (0)
#define HI_ISP_INPUT_PORT_VC_SIZE_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_input_port_vc_size_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0124, data);
}
static __inline HI_U16 hi_isp_input_port_vc_size_read(HI_VOID) {
    return IORD_16DIRECT(0x0124);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_frame_width
// ------------------------------------------------------------------------------ //
// detected frame width
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_FRAME_WIDTH_DEFAULT (0)
#define HI_ISP_INPUT_PORT_FRAME_WIDTH_DATASIZE (16)

// args: data (16-bit)
static __inline HI_U16 hi_isp_input_port_frame_width_read(HI_VOID) {
    return IORD_16DIRECT(0x0128);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_frame_height
// ------------------------------------------------------------------------------ //
// detected frame height
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_FRAME_HEIGHT_DEFAULT (0)
#define HI_ISP_INPUT_PORT_FRAME_HEIGHT_DATASIZE (16)

// args: data (16-bit)
static __inline HI_U16 hi_isp_input_port_frame_height_read(HI_VOID) {
    return IORD_16DIRECT(0x012C);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_freeze_config
// ------------------------------------------------------------------------------ //
// 
//   0 = normal operation
//   1 = hold previous config state
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_FREEZE_CONFIG_DEFAULT (0)
#define HI_ISP_INPUT_PORT_FREEZE_CONFIG_DATASIZE (1)

#define HI_ISP_INPUT_PORT_FREEZE_CONFIG_NORMAL_OPERATION (0)
#define HI_ISP_INPUT_PORT_FREEZE_CONFIG_HOLD_PREVIOUS_CONFIG_STATE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_input_port_freeze_config_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0130);
    IOWR_8DIRECT_APICAL(0x0130, ((data & 0x01) << 7) | (u8Current & 0x7F));
}
static __inline HI_U8 hi_isp_input_port_freeze_config_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0130) & 0x80) >> 7);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_mode_request
// ------------------------------------------------------------------------------ //
// 
//   0 = safe stop
//   1 = safe start
//   2 = urgent stop
//   3 = urgent start
//   4 = reserved4
//   5 = safer start
//   6 = reserved6
//   7 = reserved7
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_MODE_REQUEST_DEFAULT (0)
#define HI_ISP_INPUT_PORT_MODE_REQUEST_DATASIZE (3)

#define HI_ISP_INPUT_PORT_MODE_REQUEST_SAFE_STOP (0)
#define HI_ISP_INPUT_PORT_MODE_REQUEST_SAFE_START (1)
#define HI_ISP_INPUT_PORT_MODE_REQUEST_URGENT_STOP (2)
#define HI_ISP_INPUT_PORT_MODE_REQUEST_URGENT_START (3)
#define HI_ISP_INPUT_PORT_MODE_REQUEST_RESERVED4 (4)
#define HI_ISP_INPUT_PORT_MODE_REQUEST_SAFER_START (5)
#define HI_ISP_INPUT_PORT_MODE_REQUEST_RESERVED6 (6)
#define HI_ISP_INPUT_PORT_MODE_REQUEST_RESERVED7 (7)

// args: data (3-bit)
static __inline HI_VOID hi_isp_input_port_mode_request_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0130);
    IOWR_8DIRECT_APICAL(0x0130, (data & 0x07) | (u8Current & 0xF8));
}
static __inline HI_U8 hi_isp_input_port_mode_request_read(HI_VOID) {
    return (IORD_8DIRECT(0x0130) & 0x07);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_input_port_mode_status
// ------------------------------------------------------------------------------ //
// bit 0: 1=started, 0=stopped, bits 1,2-reserved
// ------------------------------------------------------------------------------ //

#define HI_ISP_INPUT_PORT_MODE_STATUS_DEFAULT (0)
#define HI_ISP_INPUT_PORT_MODE_STATUS_DATASIZE (3)

// args: data (3-bit)
static __inline HI_U8 hi_isp_input_port_mode_status_read(HI_VOID) {
    return (IORD_8DIRECT(0x0134) & 0x07);
}

#define VIDIEO_TEST_GEN 0xC0

// ------------------------------------------------------------------------------ //
// Group: video test gen
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_isp_video_test_gen_test_pattern_off_on
// ------------------------------------------------------------------------------ //
// Test pattern off-on: 0=off, 1=on
// ------------------------------------------------------------------------------ //

#define HI_ISP_VIDEO_TEST_GEN_TEST_PATTERN_OFF_ON_DEFAULT (0)
#define HI_ISP_VIDEO_TEST_GEN_TEST_PATTERN_OFF_ON_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_video_test_gen_test_pattern_off_on_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(VIDIEO_TEST_GEN + 0x0);
    IOWR_8DIRECT_APICAL(VIDIEO_TEST_GEN + 0x0, (data & 0x01) | (u8Current & 0xFE));
}
static __inline HI_U8 hi_isp_video_test_gen_test_pattern_off_on_read(HI_VOID) {
    return (IORD_8DIRECT(VIDIEO_TEST_GEN + 0x0) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_video_test_gen_bayer_rgb_i_sel
// ------------------------------------------------------------------------------ //
// Bayer or rgb select for input video: 0=bayer, 1=rgb
// ------------------------------------------------------------------------------ //

#define HI_ISP_VIDEO_TEST_GEN_BAYER_RGB_I_SEL_DEFAULT (0)
#define HI_ISP_VIDEO_TEST_GEN_BAYER_RGB_I_SEL_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_video_test_gen_bayer_rgb_i_sel_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(VIDIEO_TEST_GEN + 0x0);
    IOWR_8DIRECT_APICAL(VIDIEO_TEST_GEN + 0x0, ((data & 0x01) << 1) | (u8Current & 0xFD));
}
static __inline HI_U8 hi_isp_video_test_gen_bayer_rgb_i_sel_read(HI_VOID) {
    return ((IORD_8DIRECT(VIDIEO_TEST_GEN + 0x0) & 0x02) >> 1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_video_test_gen_bayer_rgb_o_sel
// ------------------------------------------------------------------------------ //
// Bayer or rgb select for output video: 0=bayer, 1=rgb
// ------------------------------------------------------------------------------ //

#define HI_ISP_VIDEO_TEST_GEN_BAYER_RGB_O_SEL_DEFAULT (0)
#define HI_ISP_VIDEO_TEST_GEN_BAYER_RGB_O_SEL_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_video_test_gen_bayer_rgb_o_sel_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(VIDIEO_TEST_GEN + 0x0);
    IOWR_8DIRECT_APICAL(VIDIEO_TEST_GEN + 0x0, ((data & 0x01) << 2) | (u8Current & 0xFB));
}
static __inline HI_U8 hi_isp_video_test_gen_bayer_rgb_o_sel_read(HI_VOID) {
    return ((IORD_8DIRECT(VIDIEO_TEST_GEN + 0x0) & 0x04) >> 2);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_video_test_gen_pattern_type
// ------------------------------------------------------------------------------ //
// Pattern type select: 0=Flat field,1=Horizontal gradient,2=Vertical Gradient,3=Vertical Bars,4=Rectangle,5-255=Default white frame on black
// ------------------------------------------------------------------------------ //

#define HI_ISP_VIDEO_TEST_GEN_PATTERN_TYPE_DEFAULT (0x03)
#define HI_ISP_VIDEO_TEST_GEN_PATTERN_TYPE_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_video_test_gen_pattern_type_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(VIDIEO_TEST_GEN + 0x4, data);
}
static __inline HI_U8 hi_isp_video_test_gen_pattern_type_read(HI_VOID) {
    return IORD_8DIRECT(VIDIEO_TEST_GEN + 0x4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_video_test_gen_r_backgnd
// ------------------------------------------------------------------------------ //
// Red background  value 16bit, MSB aligned to used width
// ------------------------------------------------------------------------------ //

#define HI_ISP_VIDEO_TEST_GEN_R_BACKGND_DEFAULT (0xFFFF)
#define HI_ISP_VIDEO_TEST_GEN_R_BACKGND_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_video_test_gen_r_backgnd_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(VIDIEO_TEST_GEN + 0x8, data);
}
static __inline HI_U16 hi_isp_video_test_gen_r_backgnd_read(HI_VOID) {
    return IORD_16DIRECT(VIDIEO_TEST_GEN + 0x8);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_video_test_gen_g_backgnd
// ------------------------------------------------------------------------------ //
// Green background value 16bit, MSB aligned to used width
// ------------------------------------------------------------------------------ //

#define HI_ISP_VIDEO_TEST_GEN_G_BACKGND_DEFAULT (0xFFFF)
#define HI_ISP_VIDEO_TEST_GEN_G_BACKGND_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_video_test_gen_g_backgnd_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(VIDIEO_TEST_GEN + 0xc, data);
}
static __inline HI_U16 hi_isp_video_test_gen_g_backgnd_read(HI_VOID) {
    return IORD_16DIRECT(VIDIEO_TEST_GEN + 0xc);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_video_test_gen_b_backgnd
// ------------------------------------------------------------------------------ //
// Blue background value 16bit, MSB aligned to used width
// ------------------------------------------------------------------------------ //

#define HI_ISP_VIDEO_TEST_GEN_B_BACKGND_DEFAULT (0xFFFF)
#define HI_ISP_VIDEO_TEST_GEN_B_BACKGND_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_video_test_gen_b_backgnd_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(VIDIEO_TEST_GEN + 0x10, data);
}
static __inline HI_U16 hi_isp_video_test_gen_b_backgnd_read(HI_VOID) {
    return IORD_16DIRECT(VIDIEO_TEST_GEN + 0x10);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_video_test_gen_r_foregnd
// ------------------------------------------------------------------------------ //
// Red foreground  value 16bit, MSB aligned to used width
// ------------------------------------------------------------------------------ //

#define HI_ISP_VIDEO_TEST_GEN_R_FOREGND_DEFAULT (0x8FFF)
#define HI_ISP_VIDEO_TEST_GEN_R_FOREGND_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_video_test_gen_r_foregnd_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(VIDIEO_TEST_GEN + 0x14, data);
}
static __inline HI_U16 hi_isp_video_test_gen_r_foregnd_read(HI_VOID) {
    return IORD_16DIRECT(VIDIEO_TEST_GEN + 0x14);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_video_test_gen_g_foregnd
// ------------------------------------------------------------------------------ //
// Green foreground value 16bit, MSB aligned to used width
// ------------------------------------------------------------------------------ //

#define HI_ISP_VIDEO_TEST_GEN_G_FOREGND_DEFAULT (0x8FFF)
#define HI_ISP_VIDEO_TEST_GEN_G_FOREGND_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_video_test_gen_g_foregnd_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(VIDIEO_TEST_GEN + 0x18, data);
}
static __inline HI_U16 hi_isp_video_test_gen_g_foregnd_read(HI_VOID) {
    return IORD_16DIRECT(VIDIEO_TEST_GEN + 0x18);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_video_test_gen_b_foregnd
// ------------------------------------------------------------------------------ //
// Blue foreground value 16bit, MSB aligned to used width
// ------------------------------------------------------------------------------ //

#define HI_ISP_VIDEO_TEST_GEN_B_FOREGND_DEFAULT (0x8FFF)
#define HI_ISP_VIDEO_TEST_GEN_B_FOREGND_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_video_test_gen_b_foregnd_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(VIDIEO_TEST_GEN + 0x1c, data);
}
static __inline HI_U16 hi_isp_video_test_gen_b_foregnd_read(HI_VOID) {
    return IORD_16DIRECT(VIDIEO_TEST_GEN + 0x1c);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_video_test_gen_rgb_gradient
// ------------------------------------------------------------------------------ //
// RGB gradient increment per pixel (0-15)
// ------------------------------------------------------------------------------ //

#define HI_ISP_VIDEO_TEST_GEN_RGB_GRADIENT_DEFAULT (0x3CAA)
#define HI_ISP_VIDEO_TEST_GEN_RGB_GRADIENT_DATASIZE (16)

// args: data (16-bit)
// data format: unsigned 4.12-bit fixed-point
static __inline HI_VOID hi_isp_video_test_gen_rgb_gradient_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(VIDIEO_TEST_GEN + 0x20, data);
}
static __inline HI_U16 hi_isp_video_test_gen_rgb_gradient_read(HI_VOID) {
    return IORD_16DIRECT(VIDIEO_TEST_GEN + 0x20);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_video_test_gen_rgb_gradient_start
// ------------------------------------------------------------------------------ //
// RGB gradient start value 16bit, MSB aligned to used width
// ------------------------------------------------------------------------------ //

#define HI_ISP_VIDEO_TEST_GEN_RGB_GRADIENT_START_DEFAULT (0x0000)
#define HI_ISP_VIDEO_TEST_GEN_RGB_GRADIENT_START_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_video_test_gen_rgb_gradient_start_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(VIDIEO_TEST_GEN + 0x24, data);
}
static __inline HI_U16 hi_isp_video_test_gen_rgb_gradient_start_read(HI_VOID) {
    return IORD_16DIRECT(VIDIEO_TEST_GEN + 0x24);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_video_test_gen_rect_top
// ------------------------------------------------------------------------------ //
// Rectangle top line number 1-n
// ------------------------------------------------------------------------------ //

#define HI_ISP_VIDEO_TEST_GEN_RECT_TOP_DEFAULT (0x001)
#define HI_ISP_VIDEO_TEST_GEN_RECT_TOP_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_video_test_gen_rect_top_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(VIDIEO_TEST_GEN + 0x28, (data & 0x0FFF));
}
static __inline HI_U16 hi_isp_video_test_gen_rect_top_read(HI_VOID) {
    return (IORD_16DIRECT(VIDIEO_TEST_GEN + 0x28) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_video_test_gen_rect_bot
// ------------------------------------------------------------------------------ //
// Rectangle bottom line number 1-n
// ------------------------------------------------------------------------------ //

#define HI_ISP_VIDEO_TEST_GEN_RECT_BOT_DEFAULT (0x100)
#define HI_ISP_VIDEO_TEST_GEN_RECT_BOT_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_video_test_gen_rect_bot_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(VIDIEO_TEST_GEN + 0x2c, (data & 0x0FFF));
}
static __inline HI_U16 hi_isp_video_test_gen_rect_bot_read(HI_VOID) {
    return (IORD_16DIRECT(VIDIEO_TEST_GEN + 0x2c) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_video_test_gen_rect_left
// ------------------------------------------------------------------------------ //
// Rectangle left pixel number 1-n
// ------------------------------------------------------------------------------ //

#define HI_ISP_VIDEO_TEST_GEN_RECT_LEFT_DEFAULT (0x001)
#define HI_ISP_VIDEO_TEST_GEN_RECT_LEFT_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_video_test_gen_rect_left_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(VIDIEO_TEST_GEN + 0x30, (data & 0x0FFF));
}
static __inline HI_U16 hi_isp_video_test_gen_rect_left_read(HI_VOID) {
    return (IORD_16DIRECT(VIDIEO_TEST_GEN + 0x30) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_video_test_gen_rect_right
// ------------------------------------------------------------------------------ //
// Rectangle right pixel number 1-n
// ------------------------------------------------------------------------------ //

#define HI_ISP_VIDEO_TEST_GEN_RECT_RIGHT_DEFAULT (0x100)
#define HI_ISP_VIDEO_TEST_GEN_RECT_RIGHT_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_video_test_gen_rect_right_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(VIDIEO_TEST_GEN + 0x38, (data & 0x0FFF));
}
static __inline HI_U16 hi_isp_video_test_gen_rect_right_read(HI_VOID) {
    return (IORD_16DIRECT(VIDIEO_TEST_GEN + 0x38) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Group: Sensor Offset
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_isp_sensor_offset_black_00
// ------------------------------------------------------------------------------ //
// Black offset for color channel 00 (R)
// ------------------------------------------------------------------------------ //

#define HI_ISP_SENSOR_OFFSET_BLACK_00_DEFAULT (0x00)
#define HI_ISP_SENSOR_OFFSET_BLACK_00_DATASIZE (14)

// args: data (14-bit)
static __inline HI_VOID hi_isp_sensor_offset_black_00_write(HI_U16 data) {
#ifdef HI_MINI_ISP
	HI_U16 u16current = IORD_16DIRECT(0x1012);
    IOWR_16DIRECT(0x1012, (data & 0xFFF) | (u16current & 0xF000));
#else
	IOWR_16DIRECT(0x0140, (data & 0x3FFF));
#endif
}
static __inline HI_U16 hi_isp_sensor_offset_black_00_read(HI_VOID) {
#ifdef HI_MINI_ISP	
    return (IORD_16DIRECT(0x1012) & 0xFFF);
#else
	return (IORD_16DIRECT(0x0140) & 0x3FFF);
#endif
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_sensor_offset_black_01
// ------------------------------------------------------------------------------ //
// Black offset for color channel 01 (Gr)
// ------------------------------------------------------------------------------ //

#define HI_ISP_SENSOR_OFFSET_BLACK_01_DEFAULT (0x00)
#define HI_ISP_SENSOR_OFFSET_BLACK_01_DATASIZE (14)

// args: data (14-bit)
static __inline HI_VOID hi_isp_sensor_offset_black_01_write(HI_U16 data) {
#ifdef HI_MINI_ISP
	HI_U16 u16current = IORD_16DIRECT(0x1010);
	IOWR_16DIRECT(0x1010, (data & 0xFFF) | (u16current & 0xF000));
#else
    IOWR_16DIRECT(0x0144, (data & 0x3FFF));
#endif
}
static __inline HI_U16 hi_isp_sensor_offset_black_01_read(HI_VOID) {
#ifdef HI_MINI_ISP	
    return (IORD_16DIRECT(0x1010) & 0xFFF);
#else
	return (IORD_16DIRECT(0x0144) & 0x3FFF);
#endif
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_sensor_offset_black_10
// ------------------------------------------------------------------------------ //
// Black offset for color channel 10 (Gb)
// ------------------------------------------------------------------------------ //

#define HI_ISP_SENSOR_OFFSET_BLACK_10_DEFAULT (0x00)
#define HI_ISP_SENSOR_OFFSET_BLACK_10_DATASIZE (14)

// args: data (14-bit)
static __inline HI_VOID hi_isp_sensor_offset_black_10_write(HI_U16 data) {
#ifdef HI_MINI_ISP
	HI_U16 u16current = IORD_16DIRECT(0x1014);
	IOWR_16DIRECT(0x1014, (data & 0xFFF) | (u16current & 0xF000));
#else
    IOWR_16DIRECT(0x0148, (data & 0x3FFF));
#endif
}
static __inline HI_U16 hi_isp_sensor_offset_black_10_read(HI_VOID) {
#ifdef HI_MINI_ISP	
    return (IORD_16DIRECT(0x1014) & 0xFFF);
#else
	return (IORD_16DIRECT(0x0148) & 0x3FFF);
#endif
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_sensor_offset_black_11
// ------------------------------------------------------------------------------ //
// Black offset for color channel 11 (B)
// ------------------------------------------------------------------------------ //

#define HI_ISP_SENSOR_OFFSET_BLACK_11_DEFAULT (0x00)
#define HI_ISP_SENSOR_OFFSET_BLACK_11_DATASIZE (14)

// args: data (14-bit)
static __inline HI_VOID hi_isp_sensor_offset_black_11_write(HI_U16 data) {
#ifdef HI_MINI_ISP
	HI_U16 u16current = IORD_16DIRECT(0x1016);
	IOWR_16DIRECT(0x1016, (data & 0xFFF) | (u16current & 0xF000));
#else
    IOWR_16DIRECT(0x014C, (data & 0x3FFF));
#endif
}
static __inline HI_U16 hi_isp_sensor_offset_black_11_read(HI_VOID) {
#ifdef HI_MINI_ISP	
    return (IORD_16DIRECT(0x1016) & 0xFFF);
#else
	return (IORD_16DIRECT(0x014C) & 0x3FFF);
#endif
}

// ------------------------------------------------------------------------------ //
// Group: Digital Gain
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_isp_digital_gain_gain
// ------------------------------------------------------------------------------ //
// Gain applied to data in 4.8 format
// ------------------------------------------------------------------------------ //

#define HI_ISP_DIGITAL_GAIN_GAIN_DEFAULT (0x100)
#define HI_ISP_DIGITAL_GAIN_GAIN_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_digital_gain_gain_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x01A0, (data & 0x0FFF));
}
static __inline HI_U16 hi_isp_digital_gain_gain_read(HI_VOID) {
    return (IORD_16DIRECT(0x01A0) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_digital_gain_offset
// ------------------------------------------------------------------------------ //
// Data black level
// ------------------------------------------------------------------------------ //

#define HI_ISP_DIGITAL_GAIN_OFFSET_DEFAULT (0x000)
#define HI_ISP_DIGITAL_GAIN_OFFSET_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_digital_gain_offset_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x01A4, data);
}
static __inline HI_U16 hi_isp_digital_gain_offset_read(HI_VOID) {
    return IORD_16DIRECT(0x01A4);
}

// ------------------------------------------------------------------------------ //
// Group: Frontend LUT
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_isp_gammafe_enable0
// ------------------------------------------------------------------------------ //
// Frontend lookup0 enable: 0=off 1=on
// ------------------------------------------------------------------------------ //

#define HI_ISP_GAMMAFE_ENABLE_DEFAULT (0)
#define HI_ISP_GAMMAFE_ENABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_gammafe_enable_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0188);
    IOWR_8DIRECT_APICAL(0x0188, (data & 0x01) | (u8Current & 0xFE));
}
static __inline HI_U8 hi_isp_gammafe_enable_read(HI_VOID) {
    return (IORD_8DIRECT(0x0188) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_gammafe_enable1
// ------------------------------------------------------------------------------ //
// Frontend lookup1 enable: 0=off 1=on
// ------------------------------------------------------------------------------ //

#define HI_ISP_GAMMAFE_ENABLE1_DEFAULT (0)
#define HI_ISP_GAMMAFE_ENABLE1_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_gammafe_enable1_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0188);
    IOWR_8DIRECT_APICAL(0x0188, ((data & 0x01) << 1) | (u8Current & 0xFD));
}
static __inline HI_U8 hi_isp_gammafe_enable1_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0188) & 0x02) >> 1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_gammafe_mcu_priority
// ------------------------------------------------------------------------------ //
// Priority of CPU port 0=low, 1=high
// ------------------------------------------------------------------------------ //

#define HI_ISP_GAMMAFE_MCU_PRIORITY_DEFAULT (0)
#define HI_ISP_GAMMAFE_MCU_PRIORITY_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_gammafe_mcu_priority_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0188);
    IOWR_8DIRECT_APICAL(0x0188, ((data & 0x01) << 3) | (u8Current & 0xF7));
}
static __inline HI_U8 hi_isp_gammafe_mcu_priority_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0188) & 0x08) >> 3);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_gammafe_mcu_ready0
// ------------------------------------------------------------------------------ //
// LUT0 is ready to receive the data from CPU
// ------------------------------------------------------------------------------ //

#define HI_ISP_GAMMAFE_MCU_READY0_DEFAULT (0)
#define HI_ISP_GAMMAFE_MCU_READY0_DATASIZE (1)

// args: data (1-bit)
static __inline HI_U8 hi_isp_gammafe_mcu_ready0_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0188) & 0x10) >> 4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_gammafe_mcu_ready1
// ------------------------------------------------------------------------------ //
// LUT1 is ready to receive the data from CPU
// ------------------------------------------------------------------------------ //

#define HI_ISP_GAMMAFE_MCU_READY1_DEFAULT (0)
#define HI_ISP_GAMMAFE_MCU_READY1_DATASIZE (1)

// args: data (1-bit)
static __inline HI_U8 hi_isp_gammafe_mcu_ready1_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0188) & 0x20) >> 5);
}
//1 Green Equalize change name to raw frontend
// ------------------------------------------------------------------------------ //
// Group: Green Equalize
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_isp_green_equalize_ge_enable
// ------------------------------------------------------------------------------ //
// Green equalization enable: 0=off, 1=on
// ------------------------------------------------------------------------------ //

#define HI_ISP_GREEN_EQUALIZE_GE_ENABLE_DEFAULT (0)
#define HI_ISP_GREEN_EQUALIZE_GE_ENABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_green_equalize_ge_enable_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x01C0);
    IOWR_8DIRECT_APICAL(0x01C0, (data & 0x01) | (u8Current & 0xFE));
}
static __inline HI_U8 hi_isp_green_equalize_ge_enable_read(HI_VOID) {
    return (IORD_8DIRECT(0x01C0) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_green_equalize_dp_enable
// ------------------------------------------------------------------------------ //
// Defect Pixel enable: 0=off, 1=on
// ------------------------------------------------------------------------------ //

#define HI_ISP_GREEN_EQUALIZE_DP_ENABLE_DEFAULT (0)
#define HI_ISP_GREEN_EQUALIZE_DP_ENABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_green_equalize_dp_enable_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x01C0);
    IOWR_8DIRECT_APICAL(0x01C0, ((data & 0x01) << 2) | (u8Current & 0xFB));
}
static __inline HI_U8 hi_isp_green_equalize_dp_enable_read(HI_VOID) {
    return ((IORD_8DIRECT(0x01C0) & 0x04) >> 2);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_green_equalize_show_dynamic_defect_pixel
// ------------------------------------------------------------------------------ //
// Show Defect Pixel: 0=off, 1=on
// ------------------------------------------------------------------------------ //

#define HI_ISP_GREEN_EQUALIZE_SHOW_DYNAMIC_DEFECT_PIXEL_DEFAULT (0)
#define HI_ISP_GREEN_EQUALIZE_SHOW_DYNAMIC_DEFECT_PIXEL_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_green_equalize_show_dynamic_defect_pixel_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x01C0);
    IOWR_8DIRECT_APICAL(0x01C0, ((data & 0x01) << 3) | (u8Current & 0xF7));
}
static __inline HI_U8 hi_isp_green_equalize_show_dynamic_defect_pixel_read(HI_VOID) {
    return ((IORD_8DIRECT(0x01C0) & 0x08) >> 3);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_green_equalize_dark_disable
// ------------------------------------------------------------------------------ //
// Disable detection of dark pixels 
// ------------------------------------------------------------------------------ //

#define HI_ISP_GREEN_EQUALIZE_DARK_DISABLE_DEFAULT (0)
#define HI_ISP_GREEN_EQUALIZE_DARK_DISABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_green_equalize_dark_disable_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x01C0);
    IOWR_8DIRECT_APICAL(0x01C0, ((data & 0x01) << 6) | (u8Current & 0xBF));
}
static __inline HI_U8 hi_isp_green_equalize_dark_disable_read(HI_VOID) {
    return ((IORD_8DIRECT(0x01C0) & 0x40) >> 6);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_green_equalize_bright_disable
// ------------------------------------------------------------------------------ //
// Disable detection of bright pixels 
// ------------------------------------------------------------------------------ //

#define HI_ISP_GREEN_EQUALIZE_BRIGHT_DISABLE_DEFAULT (0)
#define HI_ISP_GREEN_EQUALIZE_BRIGHT_DISABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_green_equalize_bright_disable_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x01C0);
    IOWR_8DIRECT_APICAL(0x01C0, ((data & 0x01) << 7) | (u8Current & 0x7F));
}
static __inline HI_U8 hi_isp_green_equalize_bright_disable_read(HI_VOID) {
    return ((IORD_8DIRECT(0x01C0) & 0x80) >> 7);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_green_equalize_ge_strength
// ------------------------------------------------------------------------------ //
// Controls strength of Green equalization
// ------------------------------------------------------------------------------ //

#define HI_ISP_GREEN_EQUALIZE_GE_STRENGTH_DEFAULT (0x00)
#define HI_ISP_GREEN_EQUALIZE_GE_STRENGTH_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_green_equalize_ge_strength_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x01C4, data);
}
static __inline HI_U8 hi_isp_green_equalize_ge_strength_read(HI_VOID) {
    return IORD_8DIRECT(0x01C4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_green_equalize_debug_sel
// ------------------------------------------------------------------------------ //
// Debug selection port
// ------------------------------------------------------------------------------ //

#define HI_ISP_GREEN_EQUALIZE_DEBUG_SEL_DEFAULT (0x0)
#define HI_ISP_GREEN_EQUALIZE_DEBUG_SEL_DATASIZE (16)

// args: data (12-bit)
static __inline HI_VOID hi_isp_green_equalize_debug_sel_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x01C8, data);
}
static __inline HI_U16 hi_isp_green_equalize_debug_sel_read(HI_VOID) {
    return IORD_16DIRECT(0x01C8);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_green_equalize_dp_threshold
// ------------------------------------------------------------------------------ //
// Controls Config of green equalization
// ------------------------------------------------------------------------------ //

#define HI_ISP_GREEN_EQUALIZE_DP_THRESHOLD_DEFAULT (0x040)
#define HI_ISP_GREEN_EQUALIZE_DP_THRESHOLD_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_green_equalize_dp_threshold_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x01CA, (data & 0x0FFF));
}
static __inline HI_U16 hi_isp_green_equalize_dp_threshold_read(HI_VOID) {
    return (IORD_16DIRECT(0x01CA) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_green_equalize_ge_threshold
// ------------------------------------------------------------------------------ //
// Controls Config of green equalization
// ------------------------------------------------------------------------------ //

#define HI_ISP_GREEN_EQUALIZE_GE_THRESHOLD_DEFAULT (0x400)
#define HI_ISP_GREEN_EQUALIZE_GE_THRESHOLD_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_green_equalize_ge_threshold_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x01CC, (data & 0x0FFF));
}
static __inline HI_U16 hi_isp_green_equalize_ge_threshold_read(HI_VOID) {
    return (IORD_16DIRECT(0x01CC) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_green_equalize_dp_slope
// ------------------------------------------------------------------------------ //
// Slope for HP Mask function
// ------------------------------------------------------------------------------ //

#define HI_ISP_GREEN_EQUALIZE_DP_SLOPE_DEFAULT (0x200)
#define HI_ISP_GREEN_EQUALIZE_DP_SLOPE_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_green_equalize_dp_slope_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x01D0, (data & 0x0FFF));
}
static __inline HI_U16 hi_isp_green_equalize_dp_slope_read(HI_VOID) {
    return (IORD_16DIRECT(0x01D0) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_green_equalize_ge_slope
// ------------------------------------------------------------------------------ //
// Slope for GE Mask function
// ------------------------------------------------------------------------------ //

#define HI_ISP_GREEN_EQUALIZE_GE_SLOPE_DEFAULT (0x0AA)
#define HI_ISP_GREEN_EQUALIZE_GE_SLOPE_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_green_equalize_ge_slope_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x01D4, (data & 0x0FFF));
}
static __inline HI_U16 hi_isp_green_equalize_ge_slope_read(HI_VOID) {
    return (IORD_16DIRECT(0x01D4) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_green_equalize_ge_sens
// ------------------------------------------------------------------------------ //
// Controls the sensitivity of STD-DEV
// ------------------------------------------------------------------------------ //

#define HI_ISP_GREEN_EQUALIZE_GE_SENS_DEFAULT (0x80)
#define HI_ISP_GREEN_EQUALIZE_GE_SENS_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_green_equalize_ge_sens_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x01D8, data);
}
static __inline HI_U8 hi_isp_green_equalize_ge_sens_read(HI_VOID) {
    return IORD_8DIRECT(0x01D8);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_raw_frontend_hpdev_threshold
// ------------------------------------------------------------------------------ //
// Threshold for Deviation of other colors for HP
// ------------------------------------------------------------------------------ //

#define HI_ISP_RAW_FRONTEND_HPDEV_THRESHOLD_DEFAULT (0x266)
#define HI_ISP_RAW_FRONTEND_HPDEV_THRESHOLD_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_raw_frontend_hpdev_threshold_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x01DC, (data & 0x0FFF));
}
static __inline HI_U16 hi_isp_raw_frontend_hpdev_threshold_read(HI_VOID) {
    return (IORD_16DIRECT(0x01DC) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_raw_frontend_line_thresh
// ------------------------------------------------------------------------------ //
// Line Threshold
// ------------------------------------------------------------------------------ //

#define HI_ISP_RAW_FRONTEND_LINE_THRESH_DEFAULT (0x150)
#define HI_ISP_RAW_FRONTEND_LINE_THRESH_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_raw_frontend_line_thresh_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x01B0, (data & 0x0FFF));
}
static __inline HI_U16 hi_isp_raw_frontend_line_thresh_read(HI_VOID) {
    return (IORD_16DIRECT(0x01B0) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_raw_frontend_hp_blend
// ------------------------------------------------------------------------------ //
// HP Blend
// ------------------------------------------------------------------------------ //

#define HI_ISP_RAW_FRONTEND_HP_BLEND_DEFAULT (0x00)
#define HI_ISP_RAW_FRONTEND_HP_BLEND_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_raw_frontend_hp_blend_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x01B4, data);
}
static __inline HI_U8 hi_isp_raw_frontend_hp_blend_read(HI_VOID) {
    return IORD_8DIRECT(0x01B4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_raw_frontend_sigma_in
// ------------------------------------------------------------------------------ //
// Manual override of noise estimation
// ------------------------------------------------------------------------------ //

#define HI_ISP_RAW_FRONTEND_SIGMA_IN_DEFAULT (0x00)
#define HI_ISP_RAW_FRONTEND_SIGMA_IN_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_raw_frontend_sigma_in_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x01B8, (data & 0x0FFF));
}
static __inline HI_U16 hi_isp_raw_frontend_sigma_in_read(HI_VOID) {
    return (IORD_16DIRECT(0x01B8) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_raw_frontend_thresh_short
// ------------------------------------------------------------------------------ //
// Noise threshold for short exposure data
// ------------------------------------------------------------------------------ //

#define HI_ISP_RAW_FRONTEND_THRESH_SHORT_DEFAULT (0x00)
#define HI_ISP_RAW_FRONTEND_THRESH_SHORT_DATASIZE (8)

// args: data (8-bit)
// data format: unsigned 4.4-bit floating-point
static __inline HI_VOID hi_isp_raw_frontend_thresh_short_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x01E0, data);
}
static __inline HI_U8 hi_isp_raw_frontend_thresh_short_read(HI_VOID) {
    return IORD_8DIRECT(0x01E0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_raw_frontend_thresh_long
// ------------------------------------------------------------------------------ //
// Noise threshold for long exposure data
// ------------------------------------------------------------------------------ //

#define HI_ISP_RAW_FRONTEND_THRESH_LONG_DEFAULT (0x30)
#define HI_ISP_RAW_FRONTEND_THRESH_LONG_DATASIZE (8)

// args: data (8-bit)
// data format: unsigned 4.4-bit floating-point
static __inline HI_VOID hi_isp_raw_frontend_thresh_long_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x01E2, data);
}
static __inline HI_U8 hi_isp_raw_frontend_thresh_long_read(HI_VOID) {
    return IORD_8DIRECT(0x01E2);
}

// ------------------------------------------------------------------------------ //
// Group: Defect Pixel
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_isp_defect_pixel_pointer_reset
// ------------------------------------------------------------------------------ //
// Reset defect-pixel table pointer each frame - set this when defect-pixel table has been written from mcu
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEFECT_PIXEL_POINTER_RESET_DEFAULT (0)
#define HI_ISP_DEFECT_PIXEL_POINTER_RESET_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_defect_pixel_pointer_reset_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x01E8);
    IOWR_8DIRECT_APICAL(0x01E8, (data & 0x01) | (u8Current & 0xFE));
}
static __inline HI_U8 hi_isp_defect_pixel_pointer_reset_read(HI_VOID) {
    return (IORD_8DIRECT(0x01E8) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_defect_pixel_show_reference
// ------------------------------------------------------------------------------ //
// Show reference values which are compared with actual values to detect bad pixels
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEFECT_PIXEL_SHOW_REFERENCE_DEFAULT (0)
#define HI_ISP_DEFECT_PIXEL_SHOW_REFERENCE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_defect_pixel_show_reference_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x01E8);
    IOWR_8DIRECT_APICAL(0x01E8, ((data & 0x01) << 1) | (u8Current & 0xFD));
}
static __inline HI_U8 hi_isp_defect_pixel_show_reference_read(HI_VOID) {
    return ((IORD_8DIRECT(0x01E8) & 0x02) >> 1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_defect_pixel_enable
// ------------------------------------------------------------------------------ //
// Correction enable: 0=off 1=on
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEFECT_PIXEL_ENABLE_DEFAULT (0)
#define HI_ISP_DEFECT_PIXEL_ENABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_defect_pixel_enable_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x01E8);
    IOWR_8DIRECT_APICAL(0x01E8, ((data & 0x01) << 2) | (u8Current & 0xFB));
}
static __inline HI_U8 hi_isp_defect_pixel_enable_read(HI_VOID) {
    return ((IORD_8DIRECT(0x01E8) & 0x04) >> 2);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_defect_pixel_overflow
// ------------------------------------------------------------------------------ //
// Table overflow flag
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEFECT_PIXEL_OVERFLOW_DEFAULT (0)
#define HI_ISP_DEFECT_PIXEL_OVERFLOW_DATASIZE (1)

// args: data (1-bit)
static __inline HI_U8 hi_isp_defect_pixel_overflow_read(HI_VOID) {
    return ((IORD_8DIRECT(0x01E8) & 0x40) >> 6);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_defect_pixel_show_static_defect_pixels
// ------------------------------------------------------------------------------ //
// Show which pixels have been detected as bad
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEFECT_PIXEL_SHOW_STATIC_DEFECT_PIXELS_DEFAULT (0)
#define HI_ISP_DEFECT_PIXEL_SHOW_STATIC_DEFECT_PIXELS_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_defect_pixel_show_static_defect_pixels_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x01E8);
    IOWR_8DIRECT_APICAL(0x01E8, ((data & 0x01) << 3) | (u8Current & 0xF7));
}
static __inline HI_U8 hi_isp_defect_pixel_show_static_defect_pixels_read(HI_VOID) {
    return ((IORD_8DIRECT(0x01E8) & 0x08) >> 3);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_defect_pixel_detection_trigger
// ------------------------------------------------------------------------------ //
// Starts detection on 0-1 transition
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEFECT_PIXEL_DETECTION_TRIGGER_DEFAULT (0)
#define HI_ISP_DEFECT_PIXEL_DETECTION_TRIGGER_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_defect_pixel_detection_trigger_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x01E8);
    IOWR_8DIRECT_APICAL(0x01E8, ((data & 0x01) << 4) | (u8Current & 0xEF));
}
static __inline HI_U8 hi_isp_defect_pixel_detection_trigger_read(HI_VOID) {
    return ((IORD_8DIRECT(0x01E8) & 0x10) >> 4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_defect_pixel_defect_pixel_count
// ------------------------------------------------------------------------------ //
// Number of defect-pixels detected
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEFECT_PIXEL_DEFECT_PIXEL_COUNT_DEFAULT (0x0)
#define HI_ISP_DEFECT_PIXEL_DEFECT_PIXEL_COUNT_DATASIZE (10)

// args: data (10-bit)
static __inline HI_U16 hi_isp_defect_pixel_defect_pixel_count_read(HI_VOID) {
    return (IORD_16DIRECT(0x01EC) & 0x03FF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_defect_pixel_table_start
// ------------------------------------------------------------------------------ //
// Address of first defect-pixel in defect-pixel store
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEFECT_PIXEL_TABLE_START_DEFAULT (0x0)
#define HI_ISP_DEFECT_PIXEL_TABLE_START_DATASIZE (10)

// args: data (10-bit)
static __inline HI_U16 hi_isp_defect_pixel_table_start_read(HI_VOID) {
    return (IORD_16DIRECT(0x01F0) & 0x03FF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_defect_pixel_defect_pixel_count_in
// ------------------------------------------------------------------------------ //
// Number of defect-pixels in the written table
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEFECT_PIXEL_DEFECT_PIXEL_COUNT_IN_DEFAULT (0x0)
#define HI_ISP_DEFECT_PIXEL_DEFECT_PIXEL_COUNT_IN_DATASIZE (10)

// args: data (10-bit)
static __inline HI_VOID hi_isp_defect_pixel_defect_pixel_count_in_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x01F4, (data & 0x03FF));
}
static __inline HI_U16 hi_isp_defect_pixel_defect_pixel_count_in_read(HI_VOID) {
    return (IORD_16DIRECT(0x01F4) & 0x03FF);
}

// ------------------------------------------------------------------------------ //
// LUT: hi_isp_defect_pixel_table_lut
// ------------------------------------------------------------------------------ //
// Static defect-pixel table.
// Data format: [23:12] = pixel line, [11:0] = pixel column.
// Note: a LUT read requires at least 2 configuration clock cycles between
//       writing the LUT address and reading the LUT data register
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEFECT_PIXEL_TABLE_LUT_NODES    (1024)
#define HI_ISP_DEFECT_PIXEL_TABLE_LUT_ADDRBITS (10)
#define HI_ISP_DEFECT_PIXEL_TABLE_LUT_DATASIZE (24)

// args: index (0-1023), data (24-bit)
static __inline HI_VOID hi_isp_defect_pixel_table_lut_write_addr(HI_U16 index) {
    IOWR_32DIRECT_APICAL(0x0850, index);
}
static __inline HI_VOID hi_isp_defect_pixel_table_lut_write_data(HI_U32 data) {
    IOWR_32DIRECT_APICAL(0x0854, data);
}
static __inline HI_VOID hi_isp_defect_pixel_table_lut_write(HI_U16 index, HI_U32 data) {
    IOWR_32DIRECT_APICAL(0x0850, index);
    IOWR_32DIRECT_APICAL(0x0854, data);
}
static __inline HI_U32 hi_isp_defect_pixel_table_lut_read_data(HI_VOID) {
    HI_U32 result = 0;
    result = ((HI_U32) IORD_32DIRECT(0x0858));
    return result;
}
static __inline HI_U32 hi_isp_defect_pixel_table_lut_read(HI_U16 index, int delay) {
    HI_U32 result = 0;
    IOWR_32DIRECT_APICAL(0x0850, index);
    while (delay-- > 0) {
        IORD_32DIRECT(0x0850);     // dummy_read of address reg
        IOWR_32DIRECT_APICAL(0x0858, 0); // dummy write to read-only data reg
    };
    result = ((HI_U32) IORD_32DIRECT(0x0858));
    return result;
}

// ------------------------------------------------------------------------------ //
// Group: Noise Profile RAW Frontend
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_isp_noise_profile_raw_frontend_weight_lut
// ------------------------------------------------------------------------------ //
// Noise profile LUT
// ------------------------------------------------------------------------------ //

#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_WEIGHT_LUT_DEFAULT (0x0)
#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_WEIGHT_LUT_DATASIZE (8)

// args: index (0-127), data (8-bit)
static __inline HI_VOID hi_isp_noise_profile_raw_frontend_weight_lut_write(HI_U32 index, HI_U8 data) {
    HI_U32 addr = 0x0880 + (index << 0);
    IOWR_8DIRECT_APICAL(addr, data & 0xFF);
}
static __inline HI_U8 hi_isp_noise_profile_raw_frontend_weight_lut_read(HI_U32 index) {
    HI_U32 addr = 0x0880 + (index << 0);
    return (HI_U8) IORD_8DIRECT(addr);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_noise_profile_raw_frontend_exp_thresh
// ------------------------------------------------------------------------------ //
// Threshold for determining long/short exposure data
// ------------------------------------------------------------------------------ //

#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_EXP_THRESH_DEFAULT (0xFFFF)
#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_EXP_THRESH_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_noise_profile_raw_frontend_exp_thresh_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0260, data);
}
static __inline HI_U16 hi_isp_noise_profile_raw_frontend_exp_thresh_read(HI_VOID) {
    return IORD_16DIRECT(0x0260);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_noise_profile_raw_frontend_short_ratio
// ------------------------------------------------------------------------------ //
// Multiplier applied to short exposure data for noise profile calculation
// ------------------------------------------------------------------------------ //

#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_SHORT_RATIO_DEFAULT (0x20)
#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_SHORT_RATIO_DATASIZE (8)

// args: data (8-bit)
// data format: unsigned 6.2-bit fixed-point
static __inline HI_VOID hi_isp_noise_profile_raw_frontend_short_ratio_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x0264, data);
}
static __inline HI_U8 hi_isp_noise_profile_raw_frontend_short_ratio_read(HI_VOID) {
    return IORD_8DIRECT(0x0264);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_noise_profile_raw_frontend_long_ratio
// ------------------------------------------------------------------------------ //
// Multiplier applied to long exposure data for noise profile calculation
// ------------------------------------------------------------------------------ //

#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_LONG_RATIO_DEFAULT (0x04)
#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_LONG_RATIO_DATASIZE (8)

// args: data (8-bit)
// data format: unsigned 6.2-bit fixed-point
static __inline HI_VOID hi_isp_noise_profile_raw_frontend_long_ratio_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x0268, data);
}
static __inline HI_U8 hi_isp_noise_profile_raw_frontend_long_ratio_read(HI_VOID) {
    return IORD_8DIRECT(0x0268);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_noise_profile_raw_frontend_np_off
// ------------------------------------------------------------------------------ //

#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_NP_OFF_DEFAULT (0)
#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_NP_OFF_DATASIZE (7)

// args: data (7-bit)
static __inline HI_VOID hi_isp_noise_profile_raw_frontend_np_off_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x026C);
    IOWR_8DIRECT_APICAL(0x026C, (data & 0x7F) | (u8Current & 0x80));
}
static __inline HI_U8 hi_isp_noise_profile_raw_frontend_np_off_read(HI_VOID) {
    return (IORD_8DIRECT(0x026C) & 0x7F);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_noise_profile_raw_frontend_np_off_reflect
// ------------------------------------------------------------------------------ //

#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_NP_OFF_REFLECT_DEFAULT (0)
#define HI_ISP_NOISE_PROFILE_RAW_FRONTEND_NP_OFF_REFLECT_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_noise_profile_raw_frontend_np_off_reflect_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x026C);
    IOWR_8DIRECT_APICAL(0x026C, ((data & 0x01) << 7) | (u8Current & 0x7F));
}
static __inline HI_U8 hi_isp_noise_profile_raw_frontend_np_off_reflect_read(HI_VOID) {
    return ((IORD_8DIRECT(0x026C) & 0x80) >> 7);
}

// ------------------------------------------------------------------------------ //
// Group: WDR
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_isp_wdr_wdr_mode
// ------------------------------------------------------------------------------ //
// Selects WDR mode
//   0 = WDR Disabled
//   1 = Frame-Switching
// ------------------------------------------------------------------------------ //

#define HI_ISP_WDR_WDR_MODE_DEFAULT (0)
#define HI_ISP_WDR_WDR_MODE_DATASIZE (1)

#define HI_ISP_WDR_WDR_MODE_WDR_DISABLED (0)
#define HI_ISP_WDR_WDR_MODE_FRAME_SWITCHING (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_wdr_wdr_mode_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0150);
    IOWR_8DIRECT_APICAL(0x0150, ((data & 0x01) << 6) | (u8Current & 0xBF));
}
static __inline HI_U8 hi_isp_wdr_wdr_mode_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0150) & 0x40) >> 6);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_wdr_long_first
// ------------------------------------------------------------------------------ //
// Indicates exposure of first pixel in image: 0=short, 1=long.
// This applies to even fields (field input is zero).
// ------------------------------------------------------------------------------ //

#define HI_ISP_WDR_LONG_FIRST_DEFAULT (1)
#define HI_ISP_WDR_LONG_FIRST_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_wdr_long_first_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0150);
    IOWR_8DIRECT_APICAL(0x0150, (data & 0x01) | (u8Current & 0xFE));
}
static __inline HI_U8 hi_isp_wdr_long_first_read(HI_VOID) {
    return (IORD_8DIRECT(0x0150) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_wdr_use_stitching_error
// ------------------------------------------------------------------------------ //

#define HI_ISP_WDR_USE_STITCHING_ERROR_DEFAULT (0)
#define HI_ISP_WDR_USE_STITCHING_ERROR_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_wdr_use_stitching_error_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0150);
    IOWR_8DIRECT_APICAL(0x0150, ((data & 0x01) << 1) | (u8Current & 0xFD));
}
static __inline HI_U8 hi_isp_wdr_use_stitching_error_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0150) & 0x02) >> 1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_wdr_blur_edges
// ------------------------------------------------------------------------------ //

#define HI_ISP_WDR_BLUR_EDGES_DEFAULT (1)
#define HI_ISP_WDR_BLUR_EDGES_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_wdr_blur_edges_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0150);
    IOWR_8DIRECT_APICAL(0x0150, ((data & 0x01) << 2) | (u8Current & 0xFB));
}
static __inline HI_U8 hi_isp_wdr_blur_edges_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0150) & 0x04) >> 2);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_wdr_resolution_recovery
// ------------------------------------------------------------------------------ //
// Enable use of short-exposure data to recover resolution
// ------------------------------------------------------------------------------ //

#define HI_ISP_WDR_RESOLUTION_RECOVERY_DEFAULT (1)
#define HI_ISP_WDR_RESOLUTION_RECOVERY_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_wdr_resolution_recovery_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0150);
    IOWR_8DIRECT_APICAL(0x0150, ((data & 0x01) << 4) | (u8Current & 0xEF));
}
static __inline HI_U8 hi_isp_wdr_resolution_recovery_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0150) & 0x10) >> 4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_wdr_use_long_override
// ------------------------------------------------------------------------------ //

#define HI_ISP_WDR_USE_LONG_OVERRIDE_DEFAULT (0)
#define HI_ISP_WDR_USE_LONG_OVERRIDE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_wdr_use_long_override_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0150);
    IOWR_8DIRECT_APICAL(0x0150, ((data & 0x01) << 5) | (u8Current & 0xDF));
}
static __inline HI_U8 hi_isp_wdr_use_long_override_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0150) & 0x20) >> 5);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_wdr_use_max1_intensity
// ------------------------------------------------------------------------------ //

#define HI_ISP_WDR_USE_MAX1_INTENSITY_DEFAULT (1)
#define HI_ISP_WDR_USE_MAX1_INTENSITY_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_wdr_use_max1_intensity_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0151);
    IOWR_8DIRECT_APICAL(0x0151, ((data & 0x01) << 7) | (u8Current & 0x7F));
}
static __inline HI_U8 hi_isp_wdr_use_max1_intensity_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0151) & 0x80) >> 7);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_wdr_use_max2_intensity
// ------------------------------------------------------------------------------ //

#define HI_ISP_WDR_USE_MAX2_INTENSITY_DEFAULT (1)
#define HI_ISP_WDR_USE_MAX2_INTENSITY_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_wdr_use_max2_intensity_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0151);
    IOWR_8DIRECT_APICAL(0x0151, ((data & 0x01) << 6) | (u8Current & 0xBF));
}
static __inline HI_U8 hi_isp_wdr_use_max2_intensity_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0151) & 0x40) >> 6);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_wdr_use_log
// ------------------------------------------------------------------------------ //

#define HI_ISP_WDR_USE_LOG_DEFAULT (0)
#define HI_ISP_WDR_USE_LOG_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_wdr_use_log_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0151);
    IOWR_8DIRECT_APICAL(0x0151, ((data & 0x01) << 5) | (u8Current & 0xDF));
}
static __inline HI_U8 hi_isp_wdr_use_log_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0151) & 0x20) >> 5);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_wdr_short_thresh
// ------------------------------------------------------------------------------ //
// Data above this threshold will be taken from short exposure only
// ------------------------------------------------------------------------------ //

#define HI_ISP_WDR_SHORT_THRESH_DEFAULT (0x0F00)
#define HI_ISP_WDR_SHORT_THRESH_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_wdr_short_thresh_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0152, (data & 0x0FFF));
}
static __inline HI_U16 hi_isp_wdr_short_thresh_read(HI_VOID) {
    return (IORD_16DIRECT(0x0152) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_wdr_long_thresh
// ------------------------------------------------------------------------------ //
// Data below this threshold will be taken from long exposure only
// ------------------------------------------------------------------------------ //

#define HI_ISP_WDR_LONG_THRESH_DEFAULT (0x0C00)
#define HI_ISP_WDR_LONG_THRESH_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_wdr_long_thresh_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0154, (data & 0x0FFF));
}
static __inline HI_U16 hi_isp_wdr_long_thresh_read(HI_VOID) {
    return (IORD_16DIRECT(0x0154) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_wdr_lines_thresh
// ------------------------------------------------------------------------------ //
// Controls level at which resolution recovery will activate for detected lines
// ------------------------------------------------------------------------------ //

#define HI_ISP_WDR_LINES_THRESH_DEFAULT (0x0040)
#define HI_ISP_WDR_LINES_THRESH_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_wdr_lines_thresh_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0156, data);
}
static __inline HI_U16 hi_isp_wdr_lines_thresh_read(HI_VOID) {
    return IORD_16DIRECT(0x0156);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_wdr_edges_thresh
// ------------------------------------------------------------------------------ //
// Controls level at which resolution recovery will activate for detected edges
// ------------------------------------------------------------------------------ //

#define HI_ISP_WDR_EDGES_THRESH_DEFAULT (0x0040)
#define HI_ISP_WDR_EDGES_THRESH_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_wdr_edges_thresh_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0158, data);
}
static __inline HI_U16 hi_isp_wdr_edges_thresh_read(HI_VOID) {
    return IORD_16DIRECT(0x0158);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_wdr_exposure_ratio
// ------------------------------------------------------------------------------ //
// Sets ratio between long and short exposures - this must match the actual exposure ratio on the sensor
// ------------------------------------------------------------------------------ //

#define HI_ISP_WDR_EXPOSURE_RATIO_DEFAULT (0x100)
#define HI_ISP_WDR_EXPOSURE_RATIO_DATASIZE (12)

// args: data (12-bit)
// data format: unsigned 6.6-bit fixed-point
static __inline HI_VOID hi_isp_wdr_exposure_ratio_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x015A, (data & 0x0FFF));
}
static __inline HI_U16 hi_isp_wdr_exposure_ratio_read(HI_VOID) {
    return (IORD_16DIRECT(0x015A) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_wdr_stitch_correct
// ------------------------------------------------------------------------------ //
// Allows adjustment for error in sensor exposure ratio for stitching area
// ------------------------------------------------------------------------------ //

#define HI_ISP_WDR_STITCH_CORRECT_DEFAULT (0x80)
#define HI_ISP_WDR_STITCH_CORRECT_DATASIZE (8)

// args: data (8-bit)
// data format: unsigned 1.7-bit fixed-point
static __inline HI_VOID hi_isp_wdr_stitch_correct_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x0160, data);
}
static __inline HI_U8 hi_isp_wdr_stitch_correct_read(HI_VOID) {
    return IORD_8DIRECT(0x0160);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_wdr_stitch_error_thresh
// ------------------------------------------------------------------------------ //
// Sets level for detection of stitching errors due to motion
// ------------------------------------------------------------------------------ //

#define HI_ISP_WDR_STITCH_ERROR_THRESH_DEFAULT (0x0040)
#define HI_ISP_WDR_STITCH_ERROR_THRESH_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_wdr_stitch_error_thresh_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x015C, data);
}
static __inline HI_U16 hi_isp_wdr_stitch_error_thresh_read(HI_VOID) {
    return IORD_16DIRECT(0x015C);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_wdr_stitch_error_limit
// ------------------------------------------------------------------------------ //
// Sets intensity level for long exposure below which stitching error detection is disabled
// ------------------------------------------------------------------------------ //

#define HI_ISP_WDR_STITCH_ERROR_LIMIT_DEFAULT (0x0200)
#define HI_ISP_WDR_STITCH_ERROR_LIMIT_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_wdr_stitch_error_limit_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x016E, data);
}
static __inline HI_U16 hi_isp_wdr_stitch_error_limit_read(HI_VOID) {
    return IORD_16DIRECT(0x016E);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_wdr_black_level_long
// ------------------------------------------------------------------------------ //
// Black level for long exposure input
// ------------------------------------------------------------------------------ //

#define HI_ISP_WDR_BLACK_LEVEL_LONG_DEFAULT (0x000)
#define HI_ISP_WDR_BLACK_LEVEL_LONG_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_wdr_black_level_long_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0170, (data & 0x0FFF));
}
static __inline HI_U16 hi_isp_wdr_black_level_long_read(HI_VOID) {
    return (IORD_16DIRECT(0x0170) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_wdr_black_level_short
// ------------------------------------------------------------------------------ //
// Black level for short exposure input
// ------------------------------------------------------------------------------ //

#define HI_ISP_WDR_BLACK_LEVEL_SHORT_DEFAULT (0x00)
#define HI_ISP_WDR_BLACK_LEVEL_SHORT_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_wdr_black_level_short_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0174, (data & 0x0FFF));
}
static __inline HI_U16 hi_isp_wdr_black_level_short_read(HI_VOID) {
    return (IORD_16DIRECT(0x0174) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_wdr_black_level_out
// ------------------------------------------------------------------------------ //
// Black level for module output
// ------------------------------------------------------------------------------ //

#define HI_ISP_WDR_BLACK_LEVEL_OUT_DEFAULT (0x000)
#define HI_ISP_WDR_BLACK_LEVEL_OUT_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_wdr_black_level_out_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0178, data);
}
static __inline HI_U16 hi_isp_wdr_black_level_out_read(HI_VOID) {
    return IORD_16DIRECT(0x0178);
}

// ------------------------------------------------------------------------------ //
// Group: Sinter
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_isp_nr_config
// ------------------------------------------------------------------------------ //

#define HI_ISP_NR_CONFIG_DEFAULT (0x1C)
#define HI_ISP_NR_CONFIG_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_nr_config_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x0200, data);
}
static __inline HI_U8 hi_isp_nr_config_read(HI_VOID) {
    return IORD_8DIRECT(0x0200);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_nr_enable
// ------------------------------------------------------------------------------ //
// Sinter enable: 0=off 1=on
// ------------------------------------------------------------------------------ //

#define HI_ISP_NR_ENABLE_DEFAULT (1)
#define HI_ISP_NR_ENABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_nr_enable_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0200);
    IOWR_8DIRECT_APICAL(0x0200, ((data & 0x01) << 4) | (u8Current & 0xEF));
}
static __inline HI_U8 hi_isp_nr_enable_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0200) & 0x10) >> 4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_nr_view_filter
// ------------------------------------------------------------------------------ //
// For debug purposes only. Set to zero for normal operation
// ------------------------------------------------------------------------------ //

#define HI_ISP_NR_VIEW_FILTER_DEFAULT (0)
#define HI_ISP_NR_VIEW_FILTER_DATASIZE (2)

// args: data (2-bit)
static __inline HI_VOID hi_isp_nr_view_filter_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0200);
    IOWR_8DIRECT_APICAL(0x0200, (data & 0x03) | (u8Current & 0xFC));
}
static __inline HI_U8 hi_isp_nr_view_filter_read(HI_VOID) {
    return (IORD_8DIRECT(0x0200) & 0x03);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_nr_scale_mode
// ------------------------------------------------------------------------------ //
// For debug purposes only. Set to 3 for normal operation
//   0 = Use filter 0 only
//   1 = Use filters 0 and 2 only
//   2 = Use filters 0, 2 and 4 only
//   3 = Use all filters
// ------------------------------------------------------------------------------ //

#define HI_ISP_NR_SCALE_MODE_DEFAULT (3)
#define HI_ISP_NR_SCALE_MODE_DATASIZE (2)

#define HI_ISP_NR_SCALE_MODE_USE_FILTER_0_ONLY (0)
#define HI_ISP_NR_SCALE_MODE_USE_FILTERS_0_AND_2_ONLY (1)
#define HI_ISP_NR_SCALE_MODE_USE_FILTERS_0_2_AND_4_ONLY (2)
#define HI_ISP_NR_SCALE_MODE_USE_ALL_FILTERS (3)

// args: data (2-bit)
static __inline HI_VOID hi_isp_nr_scale_mode_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0200);
    IOWR_8DIRECT_APICAL(0x0200, ((data & 0x03) << 2) | (u8Current & 0xF3));
}
static __inline HI_U8 hi_isp_nr_scale_mode_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0200) & 0x0C) >> 2);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_nr_filter_select
// ------------------------------------------------------------------------------ //
// Sinter filter fine tuning
// ------------------------------------------------------------------------------ //

#define HI_ISP_NR_FILTER_SELECT_DEFAULT (0)
#define HI_ISP_NR_FILTER_SELECT_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_nr_filter_select_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0200);
    IOWR_8DIRECT_APICAL(0x0200, ((data & 0x01) << 5) | (u8Current & 0xDF));
}
static __inline HI_U8 hi_isp_nr_filter_select_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0200) & 0x20) >> 5);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_nr_int_select
// ------------------------------------------------------------------------------ //
// Select intensity filter
// ------------------------------------------------------------------------------ //

#define HI_ISP_NR_INT_SELECT_DEFAULT (0)
#define HI_ISP_NR_INT_SELECT_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_nr_int_select_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0200);
    IOWR_8DIRECT_APICAL(0x0200, ((data & 0x01) << 6) | (u8Current & 0xBF));
}
static __inline HI_U8 hi_isp_nr_int_select_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0200) & 0x40) >> 6);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_nr_rm_enable
// ------------------------------------------------------------------------------ //
// Lens shading correction enable: 0=off, 1=on
// ------------------------------------------------------------------------------ //

#define HI_ISP_NR_RM_ENABLE_DEFAULT (0)
#define HI_ISP_NR_RM_ENABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_nr_rm_enable_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0200);
    IOWR_8DIRECT_APICAL(0x0200, ((data & 0x01) << 7) | (u8Current & 0x7F));
}
static __inline HI_U8 hi_isp_nr_rm_enable_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0200) & 0x80) >> 7);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_nr_int_config
// ------------------------------------------------------------------------------ //
// Intensity blending with mosaic raw
// ------------------------------------------------------------------------------ //

#define HI_ISP_NR_INT_CONFIG_DEFAULT (0x4)
#define HI_ISP_NR_INT_CONFIG_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_isp_nr_int_config_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x0201, (data & 0x0F));
}
static __inline HI_U8 hi_isp_nr_int_config_read(HI_VOID) {
    return (IORD_8DIRECT(0x0201) & 0x0F);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_nr_rm_center_x
// ------------------------------------------------------------------------------ //
// x coordinates of shading map
// ------------------------------------------------------------------------------ //

#define HI_ISP_NR_RM_CENTER_X_DEFAULT (0x280)
#define HI_ISP_NR_RM_CENTER_X_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_nr_rm_center_x_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0210, data);
}
static __inline HI_U16 hi_isp_nr_rm_center_x_read(HI_VOID) {
    return IORD_16DIRECT(0x0210);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_nr_rm_center_y
// ------------------------------------------------------------------------------ //
// y coordinates of shading map
// ------------------------------------------------------------------------------ //

#define HI_ISP_NR_RM_CENTER_Y_DEFAULT (0x168)
#define HI_ISP_NR_RM_CENTER_Y_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_nr_rm_center_y_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0214, data);
}
static __inline HI_U16 hi_isp_nr_rm_center_y_read(HI_VOID) {
    return IORD_16DIRECT(0x0214);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_nr_rm_off_center_mult
// ------------------------------------------------------------------------------ //
// normalizing factor for sum of squares
// ------------------------------------------------------------------------------ //

#define HI_ISP_NR_RM_OFF_CENTER_MULT_DEFAULT (0x0100)
#define HI_ISP_NR_RM_OFF_CENTER_MULT_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_nr_rm_off_center_mult_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0218, data);
}
static __inline HI_U16 hi_isp_nr_rm_off_center_mult_read(HI_VOID) {
    return IORD_16DIRECT(0x0218);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_nr_thresh_0h
// ------------------------------------------------------------------------------ //
// Noise threshold for high horizontal spatial frequencies
// ------------------------------------------------------------------------------ //

#define HI_ISP_NR_THRESH_0H_DEFAULT (0x00)
#define HI_ISP_NR_THRESH_0H_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_nr_thresh_0h_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x021C, data);
}
static __inline HI_U8 hi_isp_nr_thresh_0h_read(HI_VOID) {
    return IORD_8DIRECT(0x021C);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_nr_thresh_1h
// ------------------------------------------------------------------------------ //
// Noise threshold for horizontal green-only filter
// ------------------------------------------------------------------------------ //

#define HI_ISP_NR_THRESH_1H_DEFAULT (0x00)
#define HI_ISP_NR_THRESH_1H_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_nr_thresh_1h_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x021D, data);
}
static __inline HI_U8 hi_isp_nr_thresh_1h_read(HI_VOID) {
    return IORD_8DIRECT(0x021D);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_nr_thresh_2h
// ------------------------------------------------------------------------------ //
// Noise threshold for medium horizontal spatial frequencies
// ------------------------------------------------------------------------------ //

#define HI_ISP_NR_THRESH_2H_DEFAULT (0x00)
#define HI_ISP_NR_THRESH_2H_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_nr_thresh_2h_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x021E, data);
}
static __inline HI_U8 hi_isp_nr_thresh_2h_read(HI_VOID) {
    return IORD_8DIRECT(0x021E);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_nr_thresh_4h
// ------------------------------------------------------------------------------ //
// Noise threshold for low horizontal spatial frequencies
// ------------------------------------------------------------------------------ //

#define HI_ISP_NR_THRESH_4H_DEFAULT (0x00)
#define HI_ISP_NR_THRESH_4H_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_nr_thresh_4h_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x021F, data);
}
static __inline HI_U8 hi_isp_nr_thresh_4h_read(HI_VOID) {
    return IORD_8DIRECT(0x021F);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_nr_thresh_0v
// ------------------------------------------------------------------------------ //
// Noise threshold for high vertical spatial frequencies
// ------------------------------------------------------------------------------ //

#define HI_ISP_NR_THRESH_0V_DEFAULT (0x00)
#define HI_ISP_NR_THRESH_0V_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_nr_thresh_0v_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x0220, data);
}
static __inline HI_U8 hi_isp_nr_thresh_0v_read(HI_VOID) {
    return IORD_8DIRECT(0x0220);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_nr_thresh_1v
// ------------------------------------------------------------------------------ //
// Noise threshold for vertical green-only filter
// ------------------------------------------------------------------------------ //

#define HI_ISP_NR_THRESH_1V_DEFAULT (0x00)
#define HI_ISP_NR_THRESH_1V_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_nr_thresh_1v_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x0221, data);
}
static __inline HI_U8 hi_isp_nr_thresh_1v_read(HI_VOID) {
    return IORD_8DIRECT(0x0221);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_nr_thresh_2v
// ------------------------------------------------------------------------------ //
// Noise threshold for medium vertical spatial frequencies
// ------------------------------------------------------------------------------ //

#define HI_ISP_NR_THRESH_2V_DEFAULT (0x00)
#define HI_ISP_NR_THRESH_2V_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_nr_thresh_2v_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x0222, data);
}
static __inline HI_U8 hi_isp_nr_thresh_2v_read(HI_VOID) {
    return IORD_8DIRECT(0x0222);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_nr_thresh_4v
// ------------------------------------------------------------------------------ //
// Noise threshold for low vertical spatial frequencies
// ------------------------------------------------------------------------------ //

#define HI_ISP_NR_THRESH_4V_DEFAULT (0x00)
#define HI_ISP_NR_THRESH_4V_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_nr_thresh_4v_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x0223, data);
}
static __inline HI_U8 hi_isp_nr_thresh_4v_read(HI_VOID) {
    return IORD_8DIRECT(0x0223);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_nr_thresh_short
// ------------------------------------------------------------------------------ //
// Noise threshold adjustment for short exposure data
// ------------------------------------------------------------------------------ //

#define HI_ISP_NR_THRESH_SHORT_DEFAULT (0x00)
#define HI_ISP_NR_THRESH_SHORT_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_nr_thresh_short_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x0224, data);
}
static __inline HI_U8 hi_isp_nr_thresh_short_read(HI_VOID) {
    return IORD_8DIRECT(0x0224);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_nr_thresh_long
// ------------------------------------------------------------------------------ //
// Noise threshold adjustment for long exposure data
// ------------------------------------------------------------------------------ //

#define HI_ISP_NR_THRESH_LONG_DEFAULT (0x30)
#define HI_ISP_NR_THRESH_LONG_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_nr_thresh_long_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x0228, data);
}
static __inline HI_U8 hi_isp_nr_thresh_long_read(HI_VOID) {
    return IORD_8DIRECT(0x0228);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_nr_strength_0
// ------------------------------------------------------------------------------ //
// Noise reduction effect for high spatial frequencies
// ------------------------------------------------------------------------------ //

#define HI_ISP_NR_STRENGTH_0_DEFAULT (0xFF)
#define HI_ISP_NR_STRENGTH_0_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_nr_strength_0_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x022C, data);
}
static __inline HI_U8 hi_isp_nr_strength_0_read(HI_VOID) {
    return IORD_8DIRECT(0x022C);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_nr_strength_1
// ------------------------------------------------------------------------------ //
// Noise reduction effect for green-only filter
// ------------------------------------------------------------------------------ //

#define HI_ISP_NR_STRENGTH_1_DEFAULT (0xFF)
#define HI_ISP_NR_STRENGTH_1_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_nr_strength_1_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x0230, data);
}
static __inline HI_U8 hi_isp_nr_strength_1_read(HI_VOID) {
    return IORD_8DIRECT(0x0230);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_nr_strength_2
// ------------------------------------------------------------------------------ //
// Noise reduction effect for medium spatial frequencies
// ------------------------------------------------------------------------------ //

#define HI_ISP_NR_STRENGTH_2_DEFAULT (0xFF)
#define HI_ISP_NR_STRENGTH_2_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_nr_strength_2_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x0234, data);
}
static __inline HI_U8 hi_isp_nr_strength_2_read(HI_VOID) {
    return IORD_8DIRECT(0x0234);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_nr_strength_4
// ------------------------------------------------------------------------------ //
// Noise reduction effect for low spatial frequencies
// ------------------------------------------------------------------------------ //

#define HI_ISP_NR_STRENGTH_4_DEFAULT (0xFF)
#define HI_ISP_NR_STRENGTH_4_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_nr_strength_4_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x0238, data);
}
static __inline HI_U8 hi_isp_nr_strength_4_read(HI_VOID) {
    return IORD_8DIRECT(0x0238);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_nr_rm_shading_lut
// ------------------------------------------------------------------------------ //
// Radial LUT
// ------------------------------------------------------------------------------ //

#define HI_ISP_NR_RM_SHADING_LUT_DEFAULT (16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16)
#define HI_ISP_NR_RM_SHADING_LUT_DATASIZE (8)

// args: index (0-32), data (8-bit)
static __inline HI_VOID hi_isp_nr_rm_shading_lut_write(HI_U32 index, HI_U8 data) {
    HI_U32 addr = 0x0980 + (index << 0);
    IOWR_8DIRECT_APICAL(addr, data & 0xFF);
}
static __inline HI_U8 hi_isp_nr_rm_shading_lut_read(HI_U32 index) {
    HI_U32 addr = 0x0980 + (index << 0);
    return (HI_U8) IORD_8DIRECT(addr);
}

// ------------------------------------------------------------------------------ //
// Group: Noise Profile
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_isp_noise_profile_weight_lut
// ------------------------------------------------------------------------------ //
// Noise profile LUT
// ------------------------------------------------------------------------------ //

#define HI_ISP_NOISE_PROFILE_WEIGHT_LUT_DEFAULT (0x0)
#define HI_ISP_NOISE_PROFILE_WEIGHT_LUT_DATASIZE (8)

// args: index (0-127), data (8-bit)
static __inline HI_VOID hi_isp_noise_profile_weight_lut_write(HI_U32 index, HI_U8 data) {
    HI_U32 addr = 0x0900 + (index << 0);
    IOWR_8DIRECT_APICAL(addr, data & 0xFF);
}
static __inline HI_U8 hi_isp_noise_profile_weight_lut_read(HI_U32 index) {
    HI_U32 addr = 0x0900 + (index << 0);
    return (HI_U8) IORD_8DIRECT(addr);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_noise_profile_exp_thresh
// ------------------------------------------------------------------------------ //
// Threshold for determining long/short exposure data
// ------------------------------------------------------------------------------ //

#define HI_ISP_NOISE_PROFILE_EXP_THRESH_DEFAULT (0xFFFF)
#define HI_ISP_NOISE_PROFILE_EXP_THRESH_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_noise_profile_exp_thresh_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0270, data);
}
static __inline HI_U16 hi_isp_noise_profile_exp_thresh_read(HI_VOID) {
    return IORD_16DIRECT(0x0270);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_noise_profile_short_ratio
// ------------------------------------------------------------------------------ //
// Multiplier applied to short exposure data for noise profile calculation
// ------------------------------------------------------------------------------ //

#define HI_ISP_NOISE_PROFILE_SHORT_RATIO_DEFAULT (0x20)
#define HI_ISP_NOISE_PROFILE_SHORT_RATIO_DATASIZE (8)

// args: data (8-bit)
// data format: unsigned 6.2-bit fixed-point
static __inline HI_VOID hi_isp_noise_profile_short_ratio_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x0274, data);
}
static __inline HI_U8 hi_isp_noise_profile_short_ratio_read(HI_VOID) {
    return IORD_8DIRECT(0x0274);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_noise_profile_long_ratio
// ------------------------------------------------------------------------------ //
// Multiplier applied to long exposure data for noise profile calculation
// ------------------------------------------------------------------------------ //

#define HI_ISP_NOISE_PROFILE_LONG_RATIO_DEFAULT (0x04)
#define HI_ISP_NOISE_PROFILE_LONG_RATIO_DATASIZE (8)

// args: data (8-bit)
// data format: unsigned 6.2-bit fixed-point
static __inline HI_VOID hi_isp_noise_profile_long_ratio_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x0278, data);
}
static __inline HI_U8 hi_isp_noise_profile_long_ratio_read(HI_VOID) {
    return IORD_8DIRECT(0x0278);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_noise_profile_np_off
// ------------------------------------------------------------------------------ //

#define HI_ISP_NOISE_PROFILE_NP_OFF_DEFAULT (0)
#define HI_ISP_NOISE_PROFILE_NP_OFF_DATASIZE (7)

// args: data (7-bit)
static __inline HI_VOID hi_isp_noise_profile_np_off_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x027C);
    IOWR_8DIRECT_APICAL(0x027C, (data & 0x7F) | (u8Current & 0x80));
}
static __inline HI_U8 hi_isp_noise_profile_np_off_read(HI_VOID) {
    return (IORD_8DIRECT(0x027C) & 0x7F);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_noise_profile_np_off_reflect
// ------------------------------------------------------------------------------ //

#define HI_ISP_NOISE_PROFILE_NP_OFF_REFLECT_DEFAULT (0)
#define HI_ISP_NOISE_PROFILE_NP_OFF_REFLECT_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_noise_profile_np_off_reflect_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x027C);
    IOWR_8DIRECT_APICAL(0x027C, ((data & 0x01) << 7) | (u8Current & 0x7F));
}
static __inline HI_U8 hi_isp_noise_profile_np_off_reflect_read(HI_VOID) {
    return ((IORD_8DIRECT(0x027C) & 0x80) >> 7);
}

// ------------------------------------------------------------------------------ //
// Group: White Balance
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_isp_white_balance_gain_00
// ------------------------------------------------------------------------------ //
// Multiplier for color channel 00 (R)
// ------------------------------------------------------------------------------ //

#define HI_ISP_WHITE_BALANCE_GAIN_00_DEFAULT (0x100)
#define HI_ISP_WHITE_BALANCE_GAIN_00_DATASIZE (12)

// args: data (12-bit)
// data format: unsigned 4.8-bit fixed-point
static __inline HI_VOID hi_isp_white_balance_gain_00_write(HI_U16 data) {
#ifdef HI_MINI_ISP
    IOWR_16DIRECT(0x1112, (data & 0x0FFF));
#else
	IOWR_16DIRECT(0x02C0, (data & 0x0FFF));
#endif
}
static __inline HI_U16 hi_isp_white_balance_gain_00_read(HI_VOID) {
#ifdef HI_MINI_ISP
    return (IORD_16DIRECT(0x1112) & 0x0FFF);
#else
	return (IORD_16DIRECT(0x02C0) & 0x0FFF);
#endif
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_white_balance_gain_01
// ------------------------------------------------------------------------------ //
// Multiplier for color channel 01 (Gr)
// ------------------------------------------------------------------------------ //

#define HI_ISP_WHITE_BALANCE_GAIN_01_DEFAULT (0x100)
#define HI_ISP_WHITE_BALANCE_GAIN_01_DATASIZE (12)

// args: data (12-bit)
// data format: unsigned 4.8-bit fixed-point
static __inline HI_VOID hi_isp_white_balance_gain_01_write(HI_U16 data) {
#ifdef HI_MINI_ISP
    IOWR_16DIRECT(0x1110, (data & 0x0FFF));
#else
	IOWR_16DIRECT(0x02C4, (data & 0x0FFF));
#endif
}
static __inline HI_U16 hi_isp_white_balance_gain_01_read(HI_VOID) {
#ifdef HI_MINI_ISP	
    return (IORD_16DIRECT(0x1110) & 0x0FFF);
#else
	return (IORD_16DIRECT(0x02C4) & 0x0FFF);
#endif
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_white_balance_gain_10
// ------------------------------------------------------------------------------ //
// Multiplier for color channel 10 (Gb)
// ------------------------------------------------------------------------------ //

#define HI_ISP_WHITE_BALANCE_GAIN_10_DEFAULT (0x100)
#define HI_ISP_WHITE_BALANCE_GAIN_10_DATASIZE (12)

// args: data (12-bit)
// data format: unsigned 4.8-bit fixed-point
static __inline HI_VOID hi_isp_white_balance_gain_10_write(HI_U16 data) {
#ifdef HI_MINI_ISP
    IOWR_16DIRECT(0x1114, (data & 0x0FFF));
#else
	IOWR_16DIRECT(0x02C8, (data & 0x0FFF));
#endif
}
static __inline HI_U16 hi_isp_white_balance_gain_10_read(HI_VOID) {
#ifdef HI_MINI_ISP	
    return (IORD_16DIRECT(0x1114) & 0x0FFF);
#else
	return (IORD_16DIRECT(0x02C8) & 0x0FFF);
#endif
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_white_balance_gain_11
// ------------------------------------------------------------------------------ //
// Multiplier for color channel 11 (B)
// ------------------------------------------------------------------------------ //

#define HI_ISP_WHITE_BALANCE_GAIN_11_DEFAULT (0x100)
#define HI_ISP_WHITE_BALANCE_GAIN_11_DATASIZE (12)

// args: data (12-bit)
// data format: unsigned 4.8-bit fixed-point
static __inline HI_VOID hi_isp_white_balance_gain_11_write(HI_U16 data) {
#ifdef HI_MINI_ISP	
    IOWR_16DIRECT(0x1116, (data & 0x0FFF));
#else
	IOWR_16DIRECT(0x02CC, (data & 0x0FFF));
#endif
}
static __inline HI_U16 hi_isp_white_balance_gain_11_read(HI_VOID) {
#ifdef HI_MINI_ISP	
    return (IORD_16DIRECT(0x1116) & 0x0FFF);
#else
	return (IORD_16DIRECT(0x02CC) & 0x0FFF);
#endif
}

// ------------------------------------------------------------------------------ //
// Group: Offset
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_isp_offset_black_00
// ------------------------------------------------------------------------------ //
// Black offset for color channel 00 (R)
// ------------------------------------------------------------------------------ //

#define HI_ISP_OFFSET_BLACK_00_DEFAULT (0x00)
#define HI_ISP_OFFSET_BLACK_00_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_offset_black_00_write(HI_U16 data) {
#ifdef HI_MINI_ISP
	HI_U16 u16current = IORD_16DIRECT(0x1012);
	IOWR_16DIRECT(0x1012, (data & 0x3FFF) | (u16current & 0xc000));
#else
    IOWR_16DIRECT(0x02D0, (data & 0x0FFF));
#endif
}
static __inline HI_U16 hi_isp_offset_black_00_read(HI_VOID) {
#ifdef HI_MINI_ISP	
	return (IORD_16DIRECT(0x1012) & 0x3FFF);
#else
    return (IORD_16DIRECT(0x02D0) & 0x0FFF);
#endif
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_offset_black_01
// ------------------------------------------------------------------------------ //
// Black offset for color channel 01 (Gr)
// ------------------------------------------------------------------------------ //

#define HI_ISP_OFFSET_BLACK_01_DEFAULT (0x00)
#define HI_ISP_OFFSET_BLACK_01_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_offset_black_01_write(HI_U16 data) {
#ifdef HI_MINI_ISP
    HI_U16 u16current = IORD_16DIRECT(0x1010);
	IOWR_16DIRECT(0x1010, (data & 0x3FFF) | (u16current & 0xc000));
#else
	IOWR_16DIRECT(0x02D4, (data & 0x0FFF));
#endif
}
static __inline HI_U16 hi_isp_offset_black_01_read(HI_VOID) {
#ifdef HI_MINI_ISP
	return (IORD_16DIRECT(0x1010) & 0x3FFF);
#else
    return (IORD_16DIRECT(0x02D4) & 0x0FFF);
#endif
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_offset_black_10
// ------------------------------------------------------------------------------ //
// Black offset for color channel 10 (Gb)
// ------------------------------------------------------------------------------ //

#define HI_ISP_OFFSET_BLACK_10_DEFAULT (0x00)
#define HI_ISP_OFFSET_BLACK_10_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_offset_black_10_write(HI_U16 data) {
#ifdef HI_MINI_ISP
	HI_U16 u16current = IORD_16DIRECT(0x1014);
	IOWR_16DIRECT(0x1014, (data & 0x3FFF) | (u16current & 0xc000));
#else
    IOWR_16DIRECT(0x02D8, (data & 0x0FFF));
#endif
}
static __inline HI_U16 hi_isp_offset_black_10_read(HI_VOID) {
#ifdef HI_MINI_ISP
	return (IORD_16DIRECT(0x1014) & 0x3FFF);
#else
    return (IORD_16DIRECT(0x02D8) & 0x0FFF);
#endif
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_offset_black_11
// ------------------------------------------------------------------------------ //
// Black offset for color channel 11 (B)
// ------------------------------------------------------------------------------ //

#define HI_ISP_OFFSET_BLACK_11_DEFAULT (0x00)
#define HI_ISP_OFFSET_BLACK_11_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_offset_black_11_write(HI_U16 data) {
#ifdef HI_MINI_ISP
		HI_U16 u16current = IORD_16DIRECT(0x1016);
		IOWR_16DIRECT(0x1016, (data & 0x3FFF) | (u16current & 0xc000));
#else
    IOWR_16DIRECT(0x02DC, (data & 0x0FFF));
#endif
}
static __inline HI_U16 hi_isp_offset_black_11_read(HI_VOID) {
#ifdef HI_MINI_ISP
    return (IORD_16DIRECT(0x1016) & 0x3FFF);
#else
	return (IORD_16DIRECT(0x02DC) & 0x0FFF);
#endif
}

// ------------------------------------------------------------------------------ //
// Group: Radial Shading
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_isp_radial_shading_enable
// ------------------------------------------------------------------------------ //
// Lens shading correction enable: 0=off, 1=on
// ------------------------------------------------------------------------------ //

#define HI_ISP_RADIAL_SHADING_ENABLE_DEFAULT (0)
#define HI_ISP_RADIAL_SHADING_ENABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_radial_shading_enable_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0300);
    IOWR_8DIRECT_APICAL(0x0300, (data & 0x01) | (u8Current & 0xFE));
}
static __inline HI_U8 hi_isp_radial_shading_enable_read(HI_VOID) {
    return (IORD_8DIRECT(0x0300) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_radial_shading_mcu_priority
// ------------------------------------------------------------------------------ //
// Priority of CPU port 0=low, 1=high
// ------------------------------------------------------------------------------ //

#define HI_ISP_RADIAL_SHADING_MCU_PRIORITY_DEFAULT (0)
#define HI_ISP_RADIAL_SHADING_MCU_PRIORITY_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_radial_shading_mcu_priority_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0300);
    IOWR_8DIRECT_APICAL(0x0300, ((data & 0x01) << 1) | (u8Current & 0xFD));
}
static __inline HI_U8 hi_isp_radial_shading_mcu_priority_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0300) & 0x02) >> 1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_radial_shading_mcu_ready
// ------------------------------------------------------------------------------ //
// LUT is ready to receive the data from CPU
// ------------------------------------------------------------------------------ //

#define HI_ISP_RADIAL_SHADING_MCU_READY_DEFAULT (0)
#define HI_ISP_RADIAL_SHADING_MCU_READY_DATASIZE (1)

// args: data (1-bit)
static __inline HI_U8 hi_isp_radial_shading_mcu_ready_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0300) & 0x04) >> 2);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_radial_shading_centerr_x
// ------------------------------------------------------------------------------ //
// Rx coordinates of shading map
// ------------------------------------------------------------------------------ //

#define HI_ISP_RADIAL_SHADING_CENTERR_X_DEFAULT (0x3C0)
#define HI_ISP_RADIAL_SHADING_CENTERR_X_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_radial_shading_centerr_x_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0304, data);
}
static __inline HI_U16 hi_isp_radial_shading_centerr_x_read(HI_VOID) {
    return IORD_16DIRECT(0x0304);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_radial_shading_centerr_y
// ------------------------------------------------------------------------------ //
// Ry coordinates of shading map
// ------------------------------------------------------------------------------ //

#define HI_ISP_RADIAL_SHADING_CENTERR_Y_DEFAULT (0x21C)
#define HI_ISP_RADIAL_SHADING_CENTERR_Y_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_radial_shading_centerr_y_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0308, data);
}
static __inline HI_U16 hi_isp_radial_shading_centerr_y_read(HI_VOID) {
    return IORD_16DIRECT(0x0308);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_radial_shading_centerg_x
// ------------------------------------------------------------------------------ //
// Gx coordinates of shading map
// ------------------------------------------------------------------------------ //

#define HI_ISP_RADIAL_SHADING_CENTERG_X_DEFAULT (0x3C0)
#define HI_ISP_RADIAL_SHADING_CENTERG_X_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_radial_shading_centerg_x_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x030C, data);
}
static __inline HI_U16 hi_isp_radial_shading_centerg_x_read(HI_VOID) {
    return IORD_16DIRECT(0x030C);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_radial_shading_centerg_y
// ------------------------------------------------------------------------------ //
// Gy coordinates of shading map
// ------------------------------------------------------------------------------ //

#define HI_ISP_RADIAL_SHADING_CENTERG_Y_DEFAULT (0x21C)
#define HI_ISP_RADIAL_SHADING_CENTERG_Y_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_radial_shading_centerg_y_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0310, data);
}
static __inline HI_U16 hi_isp_radial_shading_centerg_y_read(HI_VOID) {
    return IORD_16DIRECT(0x0310);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_radial_shading_centerb_x
// ------------------------------------------------------------------------------ //
// Bx coordinates of shading map
// ------------------------------------------------------------------------------ //

#define HI_ISP_RADIAL_SHADING_CENTERB_X_DEFAULT (0x3C0)
#define HI_ISP_RADIAL_SHADING_CENTERB_X_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_radial_shading_centerb_x_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0314, data);
}
static __inline HI_U16 hi_isp_radial_shading_centerb_x_read(HI_VOID) {
    return IORD_16DIRECT(0x0314);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_radial_shading_centerb_y
// ------------------------------------------------------------------------------ //
// By coordinates of shading map
// ------------------------------------------------------------------------------ //

#define HI_ISP_RADIAL_SHADING_CENTERB_Y_DEFAULT (0x21C)
#define HI_ISP_RADIAL_SHADING_CENTERB_Y_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_radial_shading_centerb_y_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0318, data);
}
static __inline HI_U16 hi_isp_radial_shading_centerb_y_read(HI_VOID) {
    return IORD_16DIRECT(0x0318);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_radial_shading_off_center_multr
// ------------------------------------------------------------------------------ //
// normalizing factor for sum of squares
// ------------------------------------------------------------------------------ //

#define HI_ISP_RADIAL_SHADING_OFF_CENTER_MULTR_DEFAULT (0x06EA)
#define HI_ISP_RADIAL_SHADING_OFF_CENTER_MULTR_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_radial_shading_off_center_multr_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x031C, data);
}
static __inline HI_U16 hi_isp_radial_shading_off_center_multr_read(HI_VOID) {
    return IORD_16DIRECT(0x031C);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_radial_shading_off_center_multg
// ------------------------------------------------------------------------------ //
// normalizing factor for sum of squares
// ------------------------------------------------------------------------------ //

#define HI_ISP_RADIAL_SHADING_OFF_CENTER_MULTG_DEFAULT (0x06EA)
#define HI_ISP_RADIAL_SHADING_OFF_CENTER_MULTG_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_radial_shading_off_center_multg_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0320, data);
}
static __inline HI_U16 hi_isp_radial_shading_off_center_multg_read(HI_VOID) {
    return IORD_16DIRECT(0x0320);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_radial_shading_off_center_multb
// ------------------------------------------------------------------------------ //
// normalizing factor for sum of squares
// ------------------------------------------------------------------------------ //

#define HI_ISP_RADIAL_SHADING_OFF_CENTER_MULTB_DEFAULT (0x06EA)
#define HI_ISP_RADIAL_SHADING_OFF_CENTER_MULTB_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_radial_shading_off_center_multb_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0324, data);
}
static __inline HI_U16 hi_isp_radial_shading_off_center_multb_read(HI_VOID) {
    return IORD_16DIRECT(0x0324);
}

// ------------------------------------------------------------------------------ //
// Group: Drc
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_isp_drc_control_0
// ------------------------------------------------------------------------------ //

#define HI_ISP_DRC_CONTROL_0_DEFAULT (0x29)
#define HI_ISP_DRC_CONTROL_0_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_drc_control_0_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x0380, data);
}
static __inline HI_U8 hi_isp_drc_control_0_read(HI_VOID) {
    return IORD_8DIRECT(0x0380);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_drc_enable
// ------------------------------------------------------------------------------ //
// Drc enable: 0=off 1=on
// ------------------------------------------------------------------------------ //

#define HI_ISP_DRC_ENABLE_DEFAULT (1)
#define HI_ISP_DRC_ENABLE_DATASIZE (1)

// args: data (1-bit)
#if 0
static __inline HI_VOID hi_isp_drc_enable_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0380);
    IOWR_8DIRECT_APICAL(0x0380, (data & 0x01) | (u8Current & 0xFE));
}
static __inline HI_U8 hi_isp_drc_enable_read(HI_VOID) {
    return (IORD_8DIRECT(0x0380) & 0x01);
}
#endif
// ------------------------------------------------------------------------------ //
// Register: hi_isp_drc_max_type
// ------------------------------------------------------------------------------ //

#define HI_ISP_DRC_MAX_TYPE_DEFAULT (1)
#define HI_ISP_DRC_MAX_TYPE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_drc_max_type_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0380);
    IOWR_8DIRECT_APICAL(0x0380, ((data & 0x01) << 3) | (u8Current & 0xF7));
}
static __inline HI_U8 hi_isp_drc_max_type_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0380) & 0x08) >> 3);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_drc_rev_percept_lut_position
// ------------------------------------------------------------------------------ //

#define HI_ISP_DRC_REV_PERCEPT_LUT_POSITION_DEFAULT (0)
#define HI_ISP_DRC_REV_PERCEPT_LUT_POSITION_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_drc_rev_percept_lut_position_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0380);
    IOWR_8DIRECT_APICAL(0x0380, ((data & 0x01) << 6) | (u8Current & 0xBF));
}
static __inline HI_U8 hi_isp_drc_rev_percept_lut_position_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0380) & 0x40) >> 6);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_drc_control_1
// ------------------------------------------------------------------------------ //

#define HI_ISP_DRC_CONTROL_1_DEFAULT (0x46)
#define HI_ISP_DRC_CONTROL_1_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_drc_control_1_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x0381, data);
}
static __inline HI_U8 hi_isp_drc_control_1_read(HI_VOID) {
    return IORD_8DIRECT(0x0381);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_drc_collect_ovl
// ------------------------------------------------------------------------------ //

#define HI_ISP_DRC_COLLECT_OVL_DEFAULT (0)
#define HI_ISP_DRC_COLLECT_OVL_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_drc_collect_ovl_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0381);
    IOWR_8DIRECT_APICAL(0x0381, (data & 0x01) | (u8Current & 0xFE));
}
static __inline HI_U8 hi_isp_drc_collect_ovl_read(HI_VOID) {
    return (IORD_8DIRECT(0x0381) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_drc_collect_rnd
// ------------------------------------------------------------------------------ //

#define HI_ISP_DRC_COLLECT_RND_DEFAULT (1)
#define HI_ISP_DRC_COLLECT_RND_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_drc_collect_rnd_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0381);
    IOWR_8DIRECT_APICAL(0x0381, ((data & 0x01) << 1) | (u8Current & 0xFD));
}
static __inline HI_U8 hi_isp_drc_collect_rnd_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0381) & 0x02) >> 1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_drc_stat_norm
// ------------------------------------------------------------------------------ //

#define HI_ISP_DRC_STAT_NORM_DEFAULT (1)
#define HI_ISP_DRC_STAT_NORM_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_drc_stat_norm_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0381);
    IOWR_8DIRECT_APICAL(0x0381, ((data & 0x01) << 2) | (u8Current & 0xFB));
}
static __inline HI_U8 hi_isp_drc_stat_norm_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0381) & 0x04) >> 2);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_drc_stat_mult
// ------------------------------------------------------------------------------ //

#define HI_ISP_DRC_STAT_MULT_DEFAULT (1)
#define HI_ISP_DRC_STAT_MULT_DATASIZE (2)

// args: data (2-bit)
static __inline HI_VOID hi_isp_drc_stat_mult_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0381);
    IOWR_8DIRECT_APICAL(0x0381, ((data & 0x03) << 6) | (u8Current & 0x3F));
}
static __inline HI_U8 hi_isp_drc_stat_mult_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0381) & 0xC0) >> 6);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_drc_strength
// ------------------------------------------------------------------------------ //
// Strength of dynamic range compression. With other parameters at defaults, increases visibility of shadows
// ------------------------------------------------------------------------------ //

#define HI_ISP_DRC_STRENGTH_DEFAULT (0x80)
#define HI_ISP_DRC_STRENGTH_DATASIZE (8)

// args: data (8-bit)
#if 0
static __inline HI_VOID hi_isp_drc_strength_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x0384, data);
}
static __inline HI_U8 hi_isp_drc_strength_read(HI_VOID) {
    return IORD_8DIRECT(0x0384);
}
#endif
// ------------------------------------------------------------------------------ //
// Register: hi_isp_drc_variance
// ------------------------------------------------------------------------------ //

#define HI_ISP_DRC_VARIANCE_DEFAULT (0x12)
#define HI_ISP_DRC_VARIANCE_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_drc_variance_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x0388, data);
}
static __inline HI_U8 hi_isp_drc_variance_read(HI_VOID) {
    return IORD_8DIRECT(0x0388);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_drc_variance_space
// ------------------------------------------------------------------------------ //
// Sets the degree of spatial sensitivity of the algorithm
// ------------------------------------------------------------------------------ //

#define HI_ISP_DRC_VARIANCE_SPACE_DEFAULT (0x2)
#define HI_ISP_DRC_VARIANCE_SPACE_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_isp_drc_variance_space_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0388);
    IOWR_8DIRECT_APICAL(0x0388, (data & 0x0F) | (u8Current & 0xF0));
}
static __inline HI_U8 hi_isp_drc_variance_space_read(HI_VOID) {
    return (IORD_8DIRECT(0x0388) & 0x0F);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_drc_variance_intensity
// ------------------------------------------------------------------------------ //
// Sets the degree of luminance sensitivity of the algorithm
// ------------------------------------------------------------------------------ //

#define HI_ISP_DRC_VARIANCE_INTENSITY_DEFAULT (0x1)
#define HI_ISP_DRC_VARIANCE_INTENSITY_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_isp_drc_variance_intensity_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0388);
    IOWR_8DIRECT_APICAL(0x0388, ((data & 0x0F) << 4) | (u8Current & 0x0F));
}
static __inline HI_U8 hi_isp_drc_variance_intensity_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0388) & 0xF0) >> 4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_drc_slope_max
// ------------------------------------------------------------------------------ //
// Restricts the maximum slope (gain) which can be generated by the adaptive algorithm
// ------------------------------------------------------------------------------ //

#define HI_ISP_DRC_SLOPE_MAX_DEFAULT (0x80)
#define HI_ISP_DRC_SLOPE_MAX_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_drc_slope_max_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x0389, data);
}
static __inline HI_U8 hi_isp_drc_slope_max_read(HI_VOID) {
    return IORD_8DIRECT(0x0389);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_drc_slope_min
// ------------------------------------------------------------------------------ //
// Restricts the minimum slope (gain) which can be generated by the adaptive algorithm
// ------------------------------------------------------------------------------ //

#define HI_ISP_DRC_SLOPE_MIN_DEFAULT (0x40)
#define HI_ISP_DRC_SLOPE_MIN_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_drc_slope_min_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x038A, data);
}
static __inline HI_U8 hi_isp_drc_slope_min_read(HI_VOID) {
    return IORD_8DIRECT(0x038A);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_drc_black_level
// ------------------------------------------------------------------------------ //

#define HI_ISP_DRC_BLACK_LEVEL_DEFAULT (0x000)
#define HI_ISP_DRC_BLACK_LEVEL_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_drc_black_level_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0390, (data & 0x0FFF));
}
static __inline HI_U16 hi_isp_drc_black_level_read(HI_VOID) {
    return (IORD_16DIRECT(0x0390) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_drc_white_level
// ------------------------------------------------------------------------------ //

#define HI_ISP_DRC_WHITE_LEVEL_DEFAULT (0xFFF)
#define HI_ISP_DRC_WHITE_LEVEL_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_drc_white_level_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0394, (data & 0x0FFF));
}
static __inline HI_U16 hi_isp_drc_white_level_read(HI_VOID) {
    return (IORD_16DIRECT(0x0394) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_drc_limit_ampl
// ------------------------------------------------------------------------------ //

#define HI_ISP_DRC_LIMIT_AMPL_DEFAULT (0x00)
#define HI_ISP_DRC_LIMIT_AMPL_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_drc_limit_ampl_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x0398, data);
}
static __inline HI_U8 hi_isp_drc_limit_ampl_read(HI_VOID) {
    return IORD_8DIRECT(0x0398);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_drc_collection_correction
// ------------------------------------------------------------------------------ //

#define HI_ISP_DRC_COLLECTION_CORRECTION_DEFAULT (0x100)
#define HI_ISP_DRC_COLLECTION_CORRECTION_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_drc_collection_correction_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x039C, (data & 0x0FFF));
}
static __inline HI_U16 hi_isp_drc_collection_correction_read(HI_VOID) {
    return (IORD_16DIRECT(0x039C) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_drc_rev_percept_enable
// ------------------------------------------------------------------------------ //
// Drc lookup 1 enable: 0=off 1=on
// ------------------------------------------------------------------------------ //

#define HI_ISP_DRC_REV_PERCEPT_ENABLE_DEFAULT (0)
#define HI_ISP_DRC_REV_PERCEPT_ENABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_drc_rev_percept_enable_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x03C8, ((data & 0x01) << 1));
}
static __inline HI_U8 hi_isp_drc_rev_percept_enable_read(HI_VOID) {
    return ((IORD_8DIRECT(0x03C8) & 0x02) >> 1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_drc_fwd_percept_enable
// ------------------------------------------------------------------------------ //
// Drc lookup 2 enable: 0=off 1=on
// ------------------------------------------------------------------------------ //

#define HI_ISP_DRC_FWD_PERCEPT_ENABLE_DEFAULT (0)
#define HI_ISP_DRC_FWD_PERCEPT_ENABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_drc_fwd_percept_enable_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x03E8, ((data & 0x01) << 1));
}
static __inline HI_U8 hi_isp_drc_fwd_percept_enable_read(HI_VOID) {
    return ((IORD_8DIRECT(0x03E8) & 0x02) >> 1);
}

// ------------------------------------------------------------------------------ //
// LUT: hi_isp_drc_asymmetry_lut
// ------------------------------------------------------------------------------ //

#define HI_ISP_DRC_ASYMMETRY_LUT_NODES    (33)
#define HI_ISP_DRC_ASYMMETRY_LUT_ADDRBITS (6)
#define HI_ISP_DRC_ASYMMETRY_LUT_DATASIZE (16)
//1 no this register
// args: index (0-32), data (16-bit)
static __inline HI_VOID hi_isp_drc_asymmetry_lut_write_addr(HI_U8 index) {
    IOWR_8DIRECT_APICAL(0x0800, index);
}
static __inline HI_VOID hi_isp_drc_asymmetry_lut_write_data(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0804, data);
}
static __inline HI_VOID hi_isp_drc_asymmetry_lut_write(HI_U8 index, HI_U16 data) {
    IOWR_8DIRECT_APICAL(0x0800, index);
    IOWR_16DIRECT_APICAL(0x0804, data);
}

// ------------------------------------------------------------------------------ //
// LUT: hi_isp_drc_rev_percept_lut
// ------------------------------------------------------------------------------ //
// Drc look-up-table 1
// ------------------------------------------------------------------------------ //

#define HI_ISP_DRC_REV_PERCEPT_LUT_NODES    (65)
#define HI_ISP_DRC_REV_PERCEPT_LUT_ADDRBITS (7)
#define HI_ISP_DRC_REV_PERCEPT_LUT_DATASIZE (16)

// args: index (0-64), data (16-bit)
static __inline HI_VOID hi_isp_drc_rev_percept_lut_write_addr(HI_U8 index) {
    IOWR_8DIRECT_APICAL(0x0820, index);
}
static __inline HI_VOID hi_isp_drc_rev_percept_lut_write_data(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0824, data);
}
static __inline HI_VOID hi_isp_drc_rev_percept_lut_write(HI_U8 index, HI_U16 data) {
    IOWR_8DIRECT_APICAL(0x0820, index);
    IOWR_16DIRECT_APICAL(0x0824, data);
}

// ------------------------------------------------------------------------------ //
// LUT: hi_isp_drc_fwd_percept_lut
// ------------------------------------------------------------------------------ //
// Drc look-up-table 2
// ------------------------------------------------------------------------------ //

#define HI_ISP_DRC_FWD_PERCEPT_LUT_NODES    (65)
#define HI_ISP_DRC_FWD_PERCEPT_LUT_ADDRBITS (7)
#define HI_ISP_DRC_FWD_PERCEPT_LUT_DATASIZE (12)

// args: index (0-64), data (12-bit)
static __inline HI_VOID hi_isp_drc_fwd_percept_lut_write_addr(HI_U8 index) {
    IOWR_8DIRECT_APICAL(0x0830, index);
}
static __inline HI_VOID hi_isp_drc_fwd_percept_lut_write_data(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0834, (data & 0x0FFF));
}
static __inline HI_VOID hi_isp_drc_fwd_percept_lut_write(HI_U8 index, HI_U16 data) {
    IOWR_8DIRECT_APICAL(0x0830, index);
    IOWR_16DIRECT_APICAL(0x0834, (data & 0x0FFF));
}

// ------------------------------------------------------------------------------ //
// Group: Demosaic
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_isp_demosaic_vh_slope
// ------------------------------------------------------------------------------ //
// Slope of vertical/horizontal blending threshold in 4.4 logarithmic format
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_VH_SLOPE_DEFAULT (0xC0)
#define HI_ISP_DEMOSAIC_VH_SLOPE_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_demosaic_vh_slope_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x0400, data);
}
static __inline HI_U8 hi_isp_demosaic_vh_slope_read(HI_VOID) {
    return IORD_8DIRECT(0x0400);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_demosaic_aa_slope
// ------------------------------------------------------------------------------ //
// Slope of angular blending threshold in 4.4 logarithmic format
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_AA_SLOPE_DEFAULT (0xC0)
#define HI_ISP_DEMOSAIC_AA_SLOPE_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_demosaic_aa_slope_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x0404, data);
}
static __inline HI_U8 hi_isp_demosaic_aa_slope_read(HI_VOID) {
    return IORD_8DIRECT(0x0404);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_demosaic_va_slope
// ------------------------------------------------------------------------------ //
// Slope of VH-AA (VA) blending threshold in 4.4 logarithmic format
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_VA_SLOPE_DEFAULT (0xAA)
#define HI_ISP_DEMOSAIC_VA_SLOPE_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_demosaic_va_slope_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x0408, data);
}
static __inline HI_U8 hi_isp_demosaic_va_slope_read(HI_VOID) {
    return IORD_8DIRECT(0x0408);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_demosaic_uu_slope
// ------------------------------------------------------------------------------ //
// Slope of undefined blending threshold in 4.4 logarithmic format
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_UU_SLOPE_DEFAULT (0xAD)
#define HI_ISP_DEMOSAIC_UU_SLOPE_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_demosaic_uu_slope_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x040C, data);
}
static __inline HI_U8 hi_isp_demosaic_uu_slope_read(HI_VOID) {
    return IORD_8DIRECT(0x040C);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_demosaic_sat_slope
// ------------------------------------------------------------------------------ //
// Slope of saturation blending threshold in linear format 2.6
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_SAT_SLOPE_DEFAULT (0x5D)
#define HI_ISP_DEMOSAIC_SAT_SLOPE_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_demosaic_sat_slope_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x0410, data);
}
static __inline HI_U8 hi_isp_demosaic_sat_slope_read(HI_VOID) {
    return IORD_8DIRECT(0x0410);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_demosaic_vh_thresh
// ------------------------------------------------------------------------------ //
// Threshold for the range of vertical/horizontal blending in 0.12 format
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_VH_THRESH_DEFAULT (0x131)
#define HI_ISP_DEMOSAIC_VH_THRESH_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_demosaic_vh_thresh_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0414, (data & 0x0FFF));
}
static __inline HI_U16 hi_isp_demosaic_vh_thresh_read(HI_VOID) {
    return (IORD_16DIRECT(0x0414) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_demosaic_aa_thresh
// ------------------------------------------------------------------------------ //
// Threshold for the range of angular blending  in 0.12 format
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_AA_THRESH_DEFAULT (0xA0)
#define HI_ISP_DEMOSAIC_AA_THRESH_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_demosaic_aa_thresh_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0418, (data & 0x0FFF));
}
static __inline HI_U16 hi_isp_demosaic_aa_thresh_read(HI_VOID) {
    return (IORD_16DIRECT(0x0418) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_demosaic_va_thresh
// ------------------------------------------------------------------------------ //
// Threshold for the range of VA blending  in 0.12 format
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_VA_THRESH_DEFAULT (0x70)
#define HI_ISP_DEMOSAIC_VA_THRESH_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_demosaic_va_thresh_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x041C, (data & 0x0FFF));
}
static __inline HI_U16 hi_isp_demosaic_va_thresh_read(HI_VOID) {
    return (IORD_16DIRECT(0x041C) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_demosaic_uu_thresh
// ------------------------------------------------------------------------------ //
// Threshold for the range of undefined blending  in 0.12 format
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_UU_THRESH_DEFAULT (0x171)
#define HI_ISP_DEMOSAIC_UU_THRESH_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_demosaic_uu_thresh_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0420, (data & 0x0FFF));
}
static __inline HI_U16 hi_isp_demosaic_uu_thresh_read(HI_VOID) {
    return (IORD_16DIRECT(0x0420) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_demosaic_sat_thresh
// ------------------------------------------------------------------------------ //
// Threshold for the range of saturation blending  in signed 2.9 format
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_SAT_THRESH_DEFAULT (0x171)
#define HI_ISP_DEMOSAIC_SAT_THRESH_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_demosaic_sat_thresh_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0424, (data & 0x0FFF));
}
static __inline HI_U16 hi_isp_demosaic_sat_thresh_read(HI_VOID) {
    return (IORD_16DIRECT(0x0424) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_demosaic_vh_offset
// ------------------------------------------------------------------------------ //
// Offset for vertical/horizontal blending threshold in 0.12 format
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_VH_OFFSET_DEFAULT (0x800)
#define HI_ISP_DEMOSAIC_VH_OFFSET_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_demosaic_vh_offset_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0428, (data & 0x0FFF));
}
static __inline HI_U16 hi_isp_demosaic_vh_offset_read(HI_VOID) {
    return (IORD_16DIRECT(0x0428) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_demosaic_aa_offset
// ------------------------------------------------------------------------------ //
// Offset for angular blending threshold in 0.12 format
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_AA_OFFSET_DEFAULT (0x800)
#define HI_ISP_DEMOSAIC_AA_OFFSET_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_demosaic_aa_offset_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x042C, (data & 0x0FFF));
}
static __inline HI_U16 hi_isp_demosaic_aa_offset_read(HI_VOID) {
    return (IORD_16DIRECT(0x042C) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_demosaic_va_offset
// ------------------------------------------------------------------------------ //
// Offset for VA blending threshold in 0.12 format
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_VA_OFFSET_DEFAULT (0x800)
#define HI_ISP_DEMOSAIC_VA_OFFSET_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_demosaic_va_offset_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0430, (data & 0x0FFF));
}
static __inline HI_U16 hi_isp_demosaic_va_offset_read(HI_VOID) {
    return (IORD_16DIRECT(0x0430) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_demosaic_uu_offset
// ------------------------------------------------------------------------------ //
// Offset for undefined blending threshold in 0.12 format
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_UU_OFFSET_DEFAULT (0x000)
#define HI_ISP_DEMOSAIC_UU_OFFSET_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_demosaic_uu_offset_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0434, (data & 0x0FFF));
}
static __inline HI_U16 hi_isp_demosaic_uu_offset_read(HI_VOID) {
    return (IORD_16DIRECT(0x0434) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_demosaic_sat_offset
// ------------------------------------------------------------------------------ //
// Offset for saturation blending threshold in signed 2.9 format
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_SAT_OFFSET_DEFAULT (0x000)
#define HI_ISP_DEMOSAIC_SAT_OFFSET_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_demosaic_sat_offset_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0438, (data & 0x0FFF));
}
static __inline HI_U16 hi_isp_demosaic_sat_offset_read(HI_VOID) {
    return (IORD_16DIRECT(0x0438) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_demosaic_sharp_alt_d
// ------------------------------------------------------------------------------ //
// Directional sharp mask strength in signed 4.4 format
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_SHARP_ALT_D_DEFAULT (0x30)
#define HI_ISP_DEMOSAIC_SHARP_ALT_D_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_demosaic_sharp_alt_d_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x043C, data);
}
static __inline HI_U8 hi_isp_demosaic_sharp_alt_d_read(HI_VOID) {
    return IORD_8DIRECT(0x043C);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_demosaic_sharp_alt_ud
// ------------------------------------------------------------------------------ //
// Non-directional sharp mask strength in signed 4.4 format
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_SHARP_ALT_UD_DEFAULT (0x20)
#define HI_ISP_DEMOSAIC_SHARP_ALT_UD_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_demosaic_sharp_alt_ud_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x0440, data);
}
static __inline HI_U8 hi_isp_demosaic_sharp_alt_ud_read(HI_VOID) {
    return IORD_8DIRECT(0x0440);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_demosaic_lum_thresh
// ------------------------------------------------------------------------------ //
// Luminance threshold for directional sharpening
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_LUM_THRESH_DEFAULT (0x060)
#define HI_ISP_DEMOSAIC_LUM_THRESH_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_demosaic_lum_thresh_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0444, (data & 0x0FFF));
}
static __inline HI_U16 hi_isp_demosaic_lum_thresh_read(HI_VOID) {
    return (IORD_16DIRECT(0x0444) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_demosaic_np_offset
// ------------------------------------------------------------------------------ //
// Noise profile offset in logarithmic 4.4 format
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_NP_OFFSET_DEFAULT (0x00)
#define HI_ISP_DEMOSAIC_NP_OFFSET_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_demosaic_np_offset_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x0448, data);
}
static __inline HI_U8 hi_isp_demosaic_np_offset_read(HI_VOID) {
    return IORD_8DIRECT(0x0448);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_demosaic_dmsc_config
// ------------------------------------------------------------------------------ //
// Debug output select. Set to 0x00 for normal operation.
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_DMSC_CONFIG_DEFAULT (0x00)
#define HI_ISP_DEMOSAIC_DMSC_CONFIG_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_demosaic_dmsc_config_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x044C, data);
}
static __inline HI_U8 hi_isp_demosaic_dmsc_config_read(HI_VOID) {
    return IORD_8DIRECT(0x044C);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_demosaic_ac_thresh
// ------------------------------------------------------------------------------ //
// Threshold for the range of AC blending in signed 2.9 format
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_AC_THRESH_DEFAULT (0x1B3)
#define HI_ISP_DEMOSAIC_AC_THRESH_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_demosaic_ac_thresh_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0450, (data & 0x0FFF));
}
static __inline HI_U16 hi_isp_demosaic_ac_thresh_read(HI_VOID) {
    return (IORD_16DIRECT(0x0450) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_demosaic_ac_slope
// ------------------------------------------------------------------------------ //
// Slope of AC blending threshold in linear format 2.6
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_AC_SLOPE_DEFAULT (0xCF)
#define HI_ISP_DEMOSAIC_AC_SLOPE_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_demosaic_ac_slope_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x0454, data);
}
static __inline HI_U8 hi_isp_demosaic_ac_slope_read(HI_VOID) {
    return IORD_8DIRECT(0x0454);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_demosaic_ac_offset
// ------------------------------------------------------------------------------ //
// Offset for AC blending threshold in signed 2.9 format
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_AC_OFFSET_DEFAULT (0x000)
#define HI_ISP_DEMOSAIC_AC_OFFSET_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_demosaic_ac_offset_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0458, (data & 0x0FFF));
}
static __inline HI_U16 hi_isp_demosaic_ac_offset_read(HI_VOID) {
    return (IORD_16DIRECT(0x0458) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_demosaic_fc_slope
// ------------------------------------------------------------------------------ //
// Slope (strength) of false color correction
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_FC_SLOPE_DEFAULT (0x80)
#define HI_ISP_DEMOSAIC_FC_SLOPE_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_demosaic_fc_slope_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x045C, data);
}
static __inline HI_U8 hi_isp_demosaic_fc_slope_read(HI_VOID) {
    return IORD_8DIRECT(0x045C);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_demosaic_fc_alias_slope
// ------------------------------------------------------------------------------ //
// Slope (strength) of false colour correction after blending with saturation value in 2.6 unsigned format
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_FC_ALIAS_SLOPE_DEFAULT (0x55)
#define HI_ISP_DEMOSAIC_FC_ALIAS_SLOPE_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_demosaic_fc_alias_slope_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x0460, data);
}
static __inline HI_U8 hi_isp_demosaic_fc_alias_slope_read(HI_VOID) {
    return IORD_8DIRECT(0x0460);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_demosaic_fc_alias_thresh
// ------------------------------------------------------------------------------ //
// Threshold of false colour correction after blending with saturation valuet in in 0.8 unsigned format
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_FC_ALIAS_THRESH_DEFAULT (0x00)
#define HI_ISP_DEMOSAIC_FC_ALIAS_THRESH_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_demosaic_fc_alias_thresh_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x0464, data);
}
static __inline HI_U8 hi_isp_demosaic_fc_alias_thresh_read(HI_VOID) {
    return IORD_8DIRECT(0x0464);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_demosaic_weight_lut
// ------------------------------------------------------------------------------ //
// Noise profile LUT
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_WEIGHT_LUT_DEFAULT (0x0)
#define HI_ISP_DEMOSAIC_WEIGHT_LUT_DATASIZE (8)

// args: index (0-127), data (8-bit)
static __inline HI_VOID hi_isp_demosaic_weight_lut_write(HI_U32 index, HI_U8 data) {
    HI_U32 addr = 0x0C00 + (index << 0);
    IOWR_8DIRECT_APICAL(addr, data & 0xFF);
}
static __inline HI_U8 hi_isp_demosaic_weight_lut_read(HI_U32 index) {
    HI_U32 addr = 0x0C00 + (index << 0);
    return (HI_U8) IORD_8DIRECT(addr);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_demosaic_np_off
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_NP_OFF_DEFAULT (0)
#define HI_ISP_DEMOSAIC_NP_OFF_DATASIZE (7)

// args: data (7-bit)
static __inline HI_VOID hi_isp_demosaic_np_off_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x046C);
    IOWR_8DIRECT_APICAL(0x046C, (data & 0x7F) | (u8Current & 0x80));
}
static __inline HI_U8 hi_isp_demosaic_np_off_read(HI_VOID) {
    return (IORD_8DIRECT(0x046C) & 0x7F);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_demosaic_np_off_reflect
// ------------------------------------------------------------------------------ //

#define HI_ISP_DEMOSAIC_NP_OFF_REFLECT_DEFAULT (0)
#define HI_ISP_DEMOSAIC_NP_OFF_REFLECT_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_demosaic_np_off_reflect_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x046C);
    IOWR_8DIRECT_APICAL(0x046C, ((data & 0x01) << 7) | (u8Current & 0x7F));
}
static __inline HI_U8 hi_isp_demosaic_np_off_reflect_read(HI_VOID) {
    return ((IORD_8DIRECT(0x046C) & 0x80) >> 7);
}

// ------------------------------------------------------------------------------ //
// Group: Color Matrix
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_isp_matrix_enable
// ------------------------------------------------------------------------------ //
// Color matrix enable: 0=off 1=on
// ------------------------------------------------------------------------------ //

#define HI_ISP_MATRIX_ENABLE_DEFAULT (1)
#define HI_ISP_MATRIX_ENABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_matrix_enable_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x04A4, (data & 0x01));
}
static __inline HI_U8 hi_isp_matrix_enable_read(HI_VOID) {
    return (IORD_8DIRECT(0x04A4) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_matrix_coefft_r_r
// ------------------------------------------------------------------------------ //
// Matrix coefficient for red-red multiplier
// ------------------------------------------------------------------------------ //

#define HI_ISP_MATRIX_COEFFT_R_R_DEFAULT (0x0100)
#define HI_ISP_MATRIX_COEFFT_R_R_DATASIZE (16)

// args: data (16-bit)
// data format: sign/magnitude 8.8-bit fixed-point
static __inline HI_VOID hi_isp_matrix_coefft_r_r_write(HI_U16 data) {
#ifdef HI_MINI_ISP
    IOWR_16DIRECT(0x1210, data);
#else
	IOWR_16DIRECT(0x0480, data);
#endif
}
static __inline HI_U16 hi_isp_matrix_coefft_r_r_read(HI_VOID) {
#ifdef HI_MINI_ISP
    return IORD_16DIRECT(0x1210);
#else
	return IORD_16DIRECT(0x0480);
#endif
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_matrix_coefft_r_g
// ------------------------------------------------------------------------------ //
// Matrix coefficient for red-green multiplier
// ------------------------------------------------------------------------------ //

#define HI_ISP_MATRIX_COEFFT_R_G_DEFAULT (0x0000)
#define HI_ISP_MATRIX_COEFFT_R_G_DATASIZE (16)

// args: data (16-bit)
// data format: sign/magnitude 8.8-bit fixed-point
static __inline HI_VOID hi_isp_matrix_coefft_r_g_write(HI_U16 data) {
#ifdef HI_MINI_ISP
    IOWR_16DIRECT(0x1212, data);
#else
	IOWR_16DIRECT(0x0484, data);
#endif
}
static __inline HI_U16 hi_isp_matrix_coefft_r_g_read(HI_VOID) {
#ifdef HI_MINI_ISP
    return IORD_16DIRECT(0x1212);
#else
	return IORD_16DIRECT(0x0484);
#endif
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_matrix_coefft_r_b
// ------------------------------------------------------------------------------ //
// Matrix coefficient for red-blue multiplier
// ------------------------------------------------------------------------------ //

#define HI_ISP_MATRIX_COEFFT_R_B_DEFAULT (0x0000)
#define HI_ISP_MATRIX_COEFFT_R_B_DATASIZE (16)

// args: data (16-bit)
// data format: sign/magnitude 8.8-bit fixed-point
static __inline HI_VOID hi_isp_matrix_coefft_r_b_write(HI_U16 data) {
#ifdef HI_MINI_ISP
    IOWR_16DIRECT(0x1214, data);
#else
	IOWR_16DIRECT(0x0488, data);
#endif
}
static __inline HI_U16 hi_isp_matrix_coefft_r_b_read(HI_VOID) {
#ifdef HI_MINI_ISP	
    return IORD_16DIRECT(0x1214);
#else
	return IORD_16DIRECT(0x0488);
#endif
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_matrix_coefft_g_r
// ------------------------------------------------------------------------------ //
// Matrix coefficient for green-red multiplier
// ------------------------------------------------------------------------------ //

#define HI_ISP_MATRIX_COEFFT_G_R_DEFAULT (0x0000)
#define HI_ISP_MATRIX_COEFFT_G_R_DATASIZE (16)

// args: data (16-bit)
// data format: sign/magnitude 8.8-bit fixed-point
static __inline HI_VOID hi_isp_matrix_coefft_g_r_write(HI_U16 data) {
#ifdef HI_MINI_ISP	
    IOWR_16DIRECT(0x1216, data);
#else
	IOWR_16DIRECT(0x048C, data);
#endif
}
static __inline HI_U16 hi_isp_matrix_coefft_g_r_read(HI_VOID) {
#ifdef HI_MINI_ISP	
    return IORD_16DIRECT(0x1216);
#else
	return IORD_16DIRECT(0x048C);
#endif
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_matrix_coefft_g_g
// ------------------------------------------------------------------------------ //
// Matrix coefficient for green-green multiplier
// ------------------------------------------------------------------------------ //

#define HI_ISP_MATRIX_COEFFT_G_G_DEFAULT (0x0100)
#define HI_ISP_MATRIX_COEFFT_G_G_DATASIZE (16)

// args: data (16-bit)
// data format: sign/magnitude 8.8-bit fixed-point
static __inline HI_VOID hi_isp_matrix_coefft_g_g_write(HI_U16 data) {
#ifdef HI_MINI_ISP
    IOWR_16DIRECT(0x1218, data);
#else
	IOWR_16DIRECT(0x0490, data);
#endif
}
static __inline HI_U16 hi_isp_matrix_coefft_g_g_read(HI_VOID) {
#ifdef HI_MINI_ISP	
    return IORD_16DIRECT(0x1218);
#else
	return IORD_16DIRECT(0x0490);
#endif
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_matrix_coefft_g_b
// ------------------------------------------------------------------------------ //
// Matrix coefficient for green-blue multiplier
// ------------------------------------------------------------------------------ //

#define HI_ISP_MATRIX_COEFFT_G_B_DEFAULT (0x0000)
#define HI_ISP_MATRIX_COEFFT_G_B_DATASIZE (16)

// args: data (16-bit)
// data format: sign/magnitude 8.8-bit fixed-point
static __inline HI_VOID hi_isp_matrix_coefft_g_b_write(HI_U16 data) {
#ifdef HI_MINI_ISP	
    IOWR_16DIRECT(0x121a, data);
#else
	IOWR_16DIRECT(0x0494, data);
#endif
}
static __inline HI_U16 hi_isp_matrix_coefft_g_b_read(HI_VOID) {
#ifdef HI_MINI_ISP	
    return IORD_16DIRECT(0x121a);
#else
	return IORD_16DIRECT(0x0494);
#endif
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_matrix_coefft_b_r
// ------------------------------------------------------------------------------ //
// Matrix coefficient for blue-red multiplier
// ------------------------------------------------------------------------------ //

#define HI_ISP_MATRIX_COEFFT_B_R_DEFAULT (0x0000)
#define HI_ISP_MATRIX_COEFFT_B_R_DATASIZE (16)

// args: data (16-bit)
// data format: sign/magnitude 8.8-bit fixed-point
static __inline HI_VOID hi_isp_matrix_coefft_b_r_write(HI_U16 data) {
#ifdef HI_MINI_ISP
    IOWR_16DIRECT(0x121c, data);
#else
	IOWR_16DIRECT(0x0498, data);
#endif
}
static __inline HI_U16 hi_isp_matrix_coefft_b_r_read(HI_VOID) {
#ifdef HI_MINI_ISP	
    return IORD_16DIRECT(0x121c);
#else
	return IORD_16DIRECT(0x0498);
#endif
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_matrix_coefft_b_g
// ------------------------------------------------------------------------------ //
// Matrix coefficient for blue-green multiplier
// ------------------------------------------------------------------------------ //

#define HI_ISP_MATRIX_COEFFT_B_G_DEFAULT (0x0000)
#define HI_ISP_MATRIX_COEFFT_B_G_DATASIZE (16)

// args: data (16-bit)
// data format: sign/magnitude 8.8-bit fixed-point
static __inline HI_VOID hi_isp_matrix_coefft_b_g_write(HI_U16 data) {
#ifdef HI_MINI_ISP	
    IOWR_16DIRECT(0x121e, data);
#else
	IOWR_16DIRECT(0x049C, data);
#endif
}
static __inline HI_U16 hi_isp_matrix_coefft_b_g_read(HI_VOID) {
#ifdef HI_MINI_ISP	
    return IORD_16DIRECT(0x121e);
#else
	return IORD_16DIRECT(0x049C);
#endif
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_matrix_coefft_b_b
// ------------------------------------------------------------------------------ //
// Matrix coefficient for blue-blue multiplier
// ------------------------------------------------------------------------------ //

#define HI_ISP_MATRIX_COEFFT_B_B_DEFAULT (0x0100)
#define HI_ISP_MATRIX_COEFFT_B_B_DATASIZE (16)

// args: data (16-bit)
// data format: sign/magnitude 8.8-bit fixed-point
static __inline HI_VOID hi_isp_matrix_coefft_b_b_write(HI_U16 data) {
#ifdef HI_MINI_ISP
    IOWR_16DIRECT(0x1220, data);
#else
	IOWR_16DIRECT(0x04A0, data);
#endif
}
static __inline HI_U16 hi_isp_matrix_coefft_b_b_read(HI_VOID) {
#ifdef HI_MINI_ISP	
    return IORD_16DIRECT(0x1220);
#else
	return IORD_16DIRECT(0x04A0);
#endif
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_matrix_coefft_wb_r
// ------------------------------------------------------------------------------ //
// White balance gain for Red
// ------------------------------------------------------------------------------ //

#define HI_ISP_MATRIX_COEFFT_WB_R_DEFAULT (0x0100)
#define HI_ISP_MATRIX_COEFFT_WB_R_DATASIZE (16)

// args: data (16-bit)
// data format: sign/magnitude 8.8-bit fixed-point
static __inline HI_VOID hi_isp_matrix_coefft_wb_r_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x04A8, data);
}
static __inline HI_U16 hi_isp_matrix_coefft_wb_r_read(HI_VOID) {
    return IORD_16DIRECT(0x04A8);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_matrix_coefft_wb_g
// ------------------------------------------------------------------------------ //
// White balance gain for Green
// ------------------------------------------------------------------------------ //

#define HI_ISP_MATRIX_COEFFT_WB_G_DEFAULT (0x0100)
#define HI_ISP_MATRIX_COEFFT_WB_G_DATASIZE (16)

// args: data (16-bit)
// data format: sign/magnitude 8.8-bit fixed-point
static __inline HI_VOID hi_isp_matrix_coefft_wb_g_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x04AC, data);
}
static __inline HI_U16 hi_isp_matrix_coefft_wb_g_read(HI_VOID) {
    return IORD_16DIRECT(0x04AC);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_matrix_coefft_wb_b
// ------------------------------------------------------------------------------ //
// White balance gain for Blue
// ------------------------------------------------------------------------------ //

#define HI_ISP_MATRIX_COEFFT_WB_B_DEFAULT (0x0100)
#define HI_ISP_MATRIX_COEFFT_WB_B_DATASIZE (16)

// args: data (16-bit)
// data format: sign/magnitude 8.8-bit fixed-point
static __inline HI_VOID hi_isp_matrix_coefft_wb_b_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x04B0, data);
}
static __inline HI_U16 hi_isp_matrix_coefft_wb_b_read(HI_VOID) {
    return IORD_16DIRECT(0x04B0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_matrix_coefft_fog_offset_r
// ------------------------------------------------------------------------------ //
// Offset R for antifog function
// ------------------------------------------------------------------------------ //

#define HI_ISP_MATRIX_COEFFT_FOG_OFFSET_R_DEFAULT (0x0000)
#define HI_ISP_MATRIX_COEFFT_FOG_OFFSET_R_DATASIZE (12)

// args: data (12-bit)
// data format: unsigned 12-bit integer
static __inline HI_VOID hi_isp_matrix_coefft_fog_offset_r_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x04B4, (data & 0x0FFF));
}
static __inline HI_U16 hi_isp_matrix_coefft_fog_offset_r_read(HI_VOID) {
    return (IORD_16DIRECT(0x04B4) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_matrix_coefft_fog_offset_g
// ------------------------------------------------------------------------------ //
// Offset G for antifog function
// ------------------------------------------------------------------------------ //

#define HI_ISP_MATRIX_COEFFT_FOG_OFFSET_G_DEFAULT (0x0000)
#define HI_ISP_MATRIX_COEFFT_FOG_OFFSET_G_DATASIZE (12)

// args: data (12-bit)
// data format: unsigned 12-bit integer
static __inline HI_VOID hi_isp_matrix_coefft_fog_offset_g_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x04B8, (data & 0x0FFF));
}
static __inline HI_U16 hi_isp_matrix_coefft_fog_offset_g_read(HI_VOID) {
    return (IORD_16DIRECT(0x04B8) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_matrix_coefft_fog_offset_b
// ------------------------------------------------------------------------------ //
// Offset B for antifog function
// ------------------------------------------------------------------------------ //

#define HI_ISP_MATRIX_COEFFT_FOG_OFFSET_B_DEFAULT (0x0000)
#define HI_ISP_MATRIX_COEFFT_FOG_OFFSET_B_DATASIZE (12)

// args: data (12-bit)
// data format: unsigned 12-bit integer
static __inline HI_VOID hi_isp_matrix_coefft_fog_offset_b_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x04BC, (data & 0x0FFF));
}
static __inline HI_U16 hi_isp_matrix_coefft_fog_offset_b_read(HI_VOID) {
    return (IORD_16DIRECT(0x04BC) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Group: Sharpen
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_isp_sharpen_enable
// ------------------------------------------------------------------------------ //
// Sharpening enable: 0=off, 1=on
// ------------------------------------------------------------------------------ //

#define HI_ISP_SHARPEN_ENABLE_DEFAULT (0)
#define HI_ISP_SHARPEN_ENABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_sharpen_enable_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x04D0);
    IOWR_8DIRECT_APICAL(0x04D0, (data & 0x01) | (u8Current & 0xFE));
}
static __inline HI_U8 hi_isp_sharpen_enable_read(HI_VOID) {
    return (IORD_8DIRECT(0x04D0) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_sharpen_coring
// ------------------------------------------------------------------------------ //
// Selects LUT memory bank, value 00 connects bank 0 to sharpening and bank 1 to programming, value 01 swap banks
// ------------------------------------------------------------------------------ //

#define HI_ISP_SHARPEN_CORING_DEFAULT (1)
#define HI_ISP_SHARPEN_CORING_DATASIZE (2)

// args: data (2-bit)
static __inline HI_VOID hi_isp_sharpen_coring_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x04D0);
    IOWR_8DIRECT_APICAL(0x04D0, ((data & 0x03) << 2) | (u8Current & 0xF3));
}
static __inline HI_U8 hi_isp_sharpen_coring_read(HI_VOID) {
    return ((IORD_8DIRECT(0x04D0) & 0x0C) >> 2);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_sharpen_strength
// ------------------------------------------------------------------------------ //
// Controls strength of sharpening effect
// ------------------------------------------------------------------------------ //

#define HI_ISP_SHARPEN_STRENGTH_DEFAULT (0x30)
#define HI_ISP_SHARPEN_STRENGTH_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_sharpen_strength_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x04D4, data);
}
static __inline HI_U8 hi_isp_sharpen_strength_read(HI_VOID) {
    return IORD_8DIRECT(0x04D4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_sharpen_control_r
// ------------------------------------------------------------------------------ //

#define HI_ISP_SHARPEN_CONTROL_R_DEFAULT (0x60)
#define HI_ISP_SHARPEN_CONTROL_R_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_sharpen_control_r_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x04D8, data);
}
static __inline HI_U8 hi_isp_sharpen_control_r_read(HI_VOID) {
    return IORD_8DIRECT(0x04D8);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_sharpen_control_b
// ------------------------------------------------------------------------------ //

#define HI_ISP_SHARPEN_CONTROL_B_DEFAULT (0x40)
#define HI_ISP_SHARPEN_CONTROL_B_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_sharpen_control_b_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x04D9, data);
}
static __inline HI_U8 hi_isp_sharpen_control_b_read(HI_VOID) {
    return IORD_8DIRECT(0x04D9);
}

// ------------------------------------------------------------------------------ //
// Group: Logo
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_isp_logo_logo_left
// ------------------------------------------------------------------------------ //
// Sets x ordinate of logo (in 16-pixel steps)
// ------------------------------------------------------------------------------ //

#define HI_ISP_LOGO_LOGO_LEFT_DEFAULT (0x08)
#define HI_ISP_LOGO_LOGO_LEFT_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_logo_logo_left_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x05A0, data);
}
static __inline HI_U8 hi_isp_logo_logo_left_read(HI_VOID) {
    return IORD_8DIRECT(0x05A0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_logo_logo_top
// ------------------------------------------------------------------------------ //
// Sets y ordinate of logo (in 16-pixel steps)
// ------------------------------------------------------------------------------ //

#define HI_ISP_LOGO_LOGO_TOP_DEFAULT (0x08)
#define HI_ISP_LOGO_LOGO_TOP_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_logo_logo_top_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x05A4, data);
}
static __inline HI_U8 hi_isp_logo_logo_top_read(HI_VOID) {
    return IORD_8DIRECT(0x05A4);
}

// ------------------------------------------------------------------------------ //
// Group: Crop
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_isp_crop_enable_crop
// ------------------------------------------------------------------------------ //
// Crop enable: 0=off 1=on
// ------------------------------------------------------------------------------ //

#define HI_ISP_CROP_ENABLE_CROP_DEFAULT (0)
#define HI_ISP_CROP_ENABLE_CROP_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_crop_enable_crop_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x05C0, (data & 0x01));
}
static __inline HI_U8 hi_isp_crop_enable_crop_read(HI_VOID) {
    return (IORD_8DIRECT(0x05C0) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_crop_start_x
// ------------------------------------------------------------------------------ //
// left column of output crop window
// ------------------------------------------------------------------------------ //

#define HI_ISP_CROP_START_X_DEFAULT (0x0000)
#define HI_ISP_CROP_START_X_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_crop_start_x_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x05C4, data);
}
static __inline HI_U16 hi_isp_crop_start_x_read(HI_VOID) {
    return IORD_16DIRECT(0x05C4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_crop_start_y
// ------------------------------------------------------------------------------ //
// top row of output crop window
// ------------------------------------------------------------------------------ //

#define HI_ISP_CROP_START_Y_DEFAULT (0x0000)
#define HI_ISP_CROP_START_Y_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_crop_start_y_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x05C8, data);
}
static __inline HI_U16 hi_isp_crop_start_y_read(HI_VOID) {
    return IORD_16DIRECT(0x05C8);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_crop_size_x
// ------------------------------------------------------------------------------ //
// width of output crop window
// ------------------------------------------------------------------------------ //

#define HI_ISP_CROP_SIZE_X_DEFAULT (0xffff)
#define HI_ISP_CROP_SIZE_X_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_crop_size_x_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x05CC, data);
}
static __inline HI_U16 hi_isp_crop_size_x_read(HI_VOID) {
    return IORD_16DIRECT(0x05CC);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_crop_size_y
// ------------------------------------------------------------------------------ //
// height of output crop window
// ------------------------------------------------------------------------------ //

#define HI_ISP_CROP_SIZE_Y_DEFAULT (0xffff)
#define HI_ISP_CROP_SIZE_Y_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_isp_crop_size_y_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x05D0, data);
}
static __inline HI_U16 hi_isp_crop_size_y_read(HI_VOID) {
    return IORD_16DIRECT(0x05D0);
}

// ------------------------------------------------------------------------------ //
// Group: Statistics
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_hist_thresh_0_1
// ------------------------------------------------------------------------------ //
// Histogram threshold for bin 0/1 boundary
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_HIST_THRESH_0_1_DEFAULT (0x10)
#define HI_ISP_METERING_HIST_THRESH_0_1_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_metering_hist_thresh_0_1_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x0600, data);
}
static __inline HI_U8 hi_isp_metering_hist_thresh_0_1_read(HI_VOID) {
    return IORD_8DIRECT(0x0600);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_hist_thresh_1_2
// ------------------------------------------------------------------------------ //
// Histogram threshold for bin 1/2 boundary
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_HIST_THRESH_1_2_DEFAULT (0x20)
#define HI_ISP_METERING_HIST_THRESH_1_2_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_metering_hist_thresh_1_2_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x0604, data);
}
static __inline HI_U8 hi_isp_metering_hist_thresh_1_2_read(HI_VOID) {
    return IORD_8DIRECT(0x0604);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_hist_thresh_3_4
// ------------------------------------------------------------------------------ //
// Histogram threshold for bin 2/3 boundary
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_HIST_THRESH_3_4_DEFAULT (0xD0)
#define HI_ISP_METERING_HIST_THRESH_3_4_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_metering_hist_thresh_3_4_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x0608, data);
}
static __inline HI_U8 hi_isp_metering_hist_thresh_3_4_read(HI_VOID) {
    return IORD_8DIRECT(0x0608);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_hist_thresh_4_5
// ------------------------------------------------------------------------------ //
// Histogram threshold for bin 3/4 boundary
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_HIST_THRESH_4_5_DEFAULT (0xE0)
#define HI_ISP_METERING_HIST_THRESH_4_5_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_metering_hist_thresh_4_5_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x060C, data);
}
static __inline HI_U8 hi_isp_metering_hist_thresh_4_5_read(HI_VOID) {
    return IORD_8DIRECT(0x060C);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_hist_0
// ------------------------------------------------------------------------------ //
// Normalized histogram results for bin 0
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_HIST_0_DEFAULT (0x0)
#define HI_ISP_METERING_HIST_0_DATASIZE (16)

// args: data (16-bit)
static __inline HI_U16 hi_isp_metering_hist_0_read(HI_VOID) {
    return IORD_16DIRECT(0x0620);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_hist_1
// ------------------------------------------------------------------------------ //
// Normalized histogram results for bin 1
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_HIST_1_DEFAULT (0x0)
#define HI_ISP_METERING_HIST_1_DATASIZE (16)

// args: data (16-bit)
static __inline HI_U16 hi_isp_metering_hist_1_read(HI_VOID) {
    return IORD_16DIRECT(0x0624);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_hist_3
// ------------------------------------------------------------------------------ //
// Normalized histogram results for bin 3
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_HIST_3_DEFAULT (0x0)
#define HI_ISP_METERING_HIST_3_DATASIZE (16)

// args: data (16-bit)
static __inline HI_U16 hi_isp_metering_hist_3_read(HI_VOID) {
    return IORD_16DIRECT(0x0628);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_hist_4
// ------------------------------------------------------------------------------ //
// Normalized histogram results for bin 4
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_HIST_4_DEFAULT (0x0)
#define HI_ISP_METERING_HIST_4_DATASIZE (16)

// args: data (16-bit)
static __inline HI_U16 hi_isp_metering_hist_4_read(HI_VOID) {
    return IORD_16DIRECT(0x062C);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_aexp_nodes_used_horiz
// ------------------------------------------------------------------------------ //
// Number of active zones horizontally
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_AEXP_NODES_USED_HORIZ_DEFAULT (17)
#define HI_ISP_METERING_AEXP_NODES_USED_HORIZ_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_metering_aexp_nodes_used_horiz_write(HI_U8 data) {
#ifdef HI_MINI_ISP
	//水平方向分窗个数,默认17
    HI_U8 u8current = IORD_8DIRECT(0x2010);
    IOWR_8DIRECT(0x2010, ((data & 0x1f) | (u8current & 0xe0)));
#else
	IOWR_8DIRECT(0x0630, data);
#endif
}
static __inline HI_U8 hi_isp_metering_aexp_nodes_used_horiz_read(HI_VOID) {
#ifdef HI_MINI_ISP
	return (IORD_8DIRECT(0x2010) & 0x1f);
#else
    return IORD_8DIRECT(0x0630);
#endif
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_aexp_nodes_used_vert
// ------------------------------------------------------------------------------ //
// Number of active zones vertically
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_AEXP_NODES_USED_VERT_DEFAULT (15)
#define HI_ISP_METERING_AEXP_NODES_USED_VERT_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_metering_aexp_nodes_used_vert_write(HI_U8 data) {
#ifdef HI_MINI_ISP
	//垂直方向分窗个数,默认为15
    HI_U8 u8current = IORD_8DIRECT(0x2011);
    IOWR_8DIRECT(0x2011, ((data & 0x1f) | (u8current & 0xe0)));
#else
	IOWR_8DIRECT(0x0631, data);
#endif
}
static __inline HI_U8 hi_isp_metering_aexp_nodes_used_vert_read(HI_VOID) {
#ifdef HI_MINI_ISP
	return (IORD_8DIRECT(0x2011) & 0x1f);
#else
    return IORD_8DIRECT(0x0631);
#endif
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_awb_stats_mode
// ------------------------------------------------------------------------------ //
// Statistics mode: 0 - legacy(G/R,B/R), 1 - u8Current (R/G, B/G)
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_AWB_STATS_MODE_DEFAULT (0)
#define HI_ISP_METERING_AWB_STATS_MODE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_metering_awb_stats_mode_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x0668, (data & 0x01));
}
static __inline HI_U8 hi_isp_metering_awb_stats_mode_read(HI_VOID) {
    return (IORD_8DIRECT(0x0668) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_white_level_awb
// ------------------------------------------------------------------------------ //
// Upper limit of valid data for AWB
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_WHITE_LEVEL_AWB_DEFAULT (0x3FF)
#define HI_ISP_METERING_WHITE_LEVEL_AWB_DATASIZE (10)

// args: data (10-bit)
static __inline HI_VOID hi_isp_metering_white_level_awb_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0640, (data & 0x03FF));
}
static __inline HI_U16 hi_isp_metering_white_level_awb_read(HI_VOID) {
    return (IORD_16DIRECT(0x0640) & 0x03FF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_black_level_awb
// ------------------------------------------------------------------------------ //
// Lower limit of valid data for AWB
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_BLACK_LEVEL_AWB_DEFAULT (0x000)
#define HI_ISP_METERING_BLACK_LEVEL_AWB_DATASIZE (10)

// args: data (10-bit)
static __inline HI_VOID hi_isp_metering_black_level_awb_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0644, (data & 0x03FF));
}
static __inline HI_U16 hi_isp_metering_black_level_awb_read(HI_VOID) {
    return (IORD_16DIRECT(0x0644) & 0x03FF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_cr_ref_max_awb
// ------------------------------------------------------------------------------ //
// Maximum value of R/G for white region
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_CR_REF_MAX_AWB_DEFAULT (0x1FF)
#define HI_ISP_METERING_CR_REF_MAX_AWB_DATASIZE (12)

// args: data (12-bit)
// data format: unsigned 4.8-bit fixed-point
static __inline HI_VOID hi_isp_metering_cr_ref_max_awb_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0648, (data & 0x0FFF));
}
static __inline HI_U16 hi_isp_metering_cr_ref_max_awb_read(HI_VOID) {
    return (IORD_16DIRECT(0x0648) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_cr_ref_min_awb
// ------------------------------------------------------------------------------ //
// Minimum value of R/G for white region
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_CR_REF_MIN_AWB_DEFAULT (0x040)
#define HI_ISP_METERING_CR_REF_MIN_AWB_DATASIZE (12)

// args: data (12-bit)
// data format: unsigned 4.8-bit fixed-point
static __inline HI_VOID hi_isp_metering_cr_ref_min_awb_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x064C, (data & 0x0FFF));
}
static __inline HI_U16 hi_isp_metering_cr_ref_min_awb_read(HI_VOID) {
    return (IORD_16DIRECT(0x064C) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_cb_ref_max_awb
// ------------------------------------------------------------------------------ //
// Maximum value of B/G for white region
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_CB_REF_MAX_AWB_DEFAULT (0x1FF)
#define HI_ISP_METERING_CB_REF_MAX_AWB_DATASIZE (12)

// args: data (12-bit)
// data format: unsigned 4.8-bit fixed-point
static __inline HI_VOID hi_isp_metering_cb_ref_max_awb_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0650, (data & 0x0FFF));
}
static __inline HI_U16 hi_isp_metering_cb_ref_max_awb_read(HI_VOID) {
    return (IORD_16DIRECT(0x0650) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_cb_ref_min_awb
// ------------------------------------------------------------------------------ //
// Minimum value of B/G for white region
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_CB_REF_MIN_AWB_DEFAULT (0x040)
#define HI_ISP_METERING_CB_REF_MIN_AWB_DATASIZE (12)

// args: data (12-bit)
// data format: unsigned 4.8-bit fixed-point
static __inline HI_VOID hi_isp_metering_cb_ref_min_awb_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0654, (data & 0x0FFF));
}
static __inline HI_U16 hi_isp_metering_cb_ref_min_awb_read(HI_VOID) {
    return (IORD_16DIRECT(0x0654) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_awb_rg
// ------------------------------------------------------------------------------ //
// AWB output R/G
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_AWB_RG_DEFAULT (0x0)
#define HI_ISP_METERING_AWB_RG_DATASIZE (12)

// args: data (12-bit)
// data format: unsigned 4.8-bit fixed-point
static __inline HI_U16 hi_isp_metering_awb_rg_read(HI_VOID) {
    return (IORD_16DIRECT(0x0658) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_awb_bg
// ------------------------------------------------------------------------------ //
// AWB output B/G
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_AWB_BG_DEFAULT (0x0)
#define HI_ISP_METERING_AWB_BG_DATASIZE (12)

// args: data (12-bit)
// data format: unsigned 4.8-bit fixed-point
static __inline HI_U16 hi_isp_metering_awb_bg_read(HI_VOID) {
    return (IORD_16DIRECT(0x065C) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_awb_sum
// ------------------------------------------------------------------------------ //
// AWB output population
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_AWB_SUM_DEFAULT (0x0)
#define HI_ISP_METERING_AWB_SUM_DATASIZE (32)

// args: data (32-bit)
// data format: unsigned 32-bit integer
static __inline HI_U32 hi_isp_metering_awb_sum_read(HI_VOID) {
    return IORD_32DIRECT(0x0660);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_awb_nodes_used_horiz
// ------------------------------------------------------------------------------ //
// Number of active zones horizontally
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_AWB_NODES_USED_HORIZ_DEFAULT (17)
#define HI_ISP_METERING_AWB_NODES_USED_HORIZ_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_metering_awb_nodes_used_horiz_write(HI_U8 data) {
#ifdef HI_MINI_ISP
	//水平方向分窗个数,默认17
	HI_U8 u8current = IORD_8DIRECT(0x2110);
    IOWR_8DIRECT(0x2110, ((data & 0x1f) | (u8current & 0xe0)));
#else
    IOWR_8DIRECT(0x066C, data);
#endif
}
static __inline HI_U8 hi_isp_metering_awb_nodes_used_horiz_read(HI_VOID) {
#ifdef HI_MINI_ISP
    return (IORD_8DIRECT(0x2110) & 0x1f);
#else
	return IORD_8DIRECT(0x066C);
#endif
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_awb_nodes_used_vert
// ------------------------------------------------------------------------------ //
// Number of active zones vertically
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_AWB_NODES_USED_VERT_DEFAULT (15)
#define HI_ISP_METERING_AWB_NODES_USED_VERT_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_metering_awb_nodes_used_vert_write(HI_U8 data) {
#ifdef HI_MINI_ISP
	//垂直方向分窗个数,默认15
	HI_U8 u8current = IORD_8DIRECT(0x2111);
    IOWR_8DIRECT(0x2111, ((data & 0x1f) | (u8current & 0xe0)));
#else
    IOWR_8DIRECT(0x066D, data);
#endif
}
static __inline HI_U8 hi_isp_metering_awb_nodes_used_vert_read(HI_VOID) {
#ifdef HI_MINI_ISP	
    return (IORD_8DIRECT(0x2111) & 0x1f);
#else
	return IORD_8DIRECT(0x066D);
#endif
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_cr_ref_high_awb
// ------------------------------------------------------------------------------ //
// Maximum value of R/G for white region
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_CR_REF_HIGH_AWB_DEFAULT (0xFFF)
#define HI_ISP_METERING_CR_REF_HIGH_AWB_DATASIZE (12)

// args: data (12-bit)
// data format: unsigned 4.8-bit fixed-point
static __inline HI_VOID hi_isp_metering_cr_ref_high_awb_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0670, (data & 0x0FFF));
}
static __inline HI_U16 hi_isp_metering_cr_ref_high_awb_read(HI_VOID) {
    return (IORD_16DIRECT(0x0670) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_cr_ref_low_awb
// ------------------------------------------------------------------------------ //
// Minimum value of R/G for white region
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_CR_REF_LOW_AWB_DEFAULT (0x000)
#define HI_ISP_METERING_CR_REF_LOW_AWB_DATASIZE (12)

// args: data (12-bit)
// data format: unsigned 4.8-bit fixed-point
static __inline HI_VOID hi_isp_metering_cr_ref_low_awb_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0674, (data & 0x0FFF));
}
static __inline HI_U16 hi_isp_metering_cr_ref_low_awb_read(HI_VOID) {
    return (IORD_16DIRECT(0x0674) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_cb_ref_high_awb
// ------------------------------------------------------------------------------ //
// Maximum value of B/G for white region
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_CB_REF_HIGH_AWB_DEFAULT (0xFFF)
#define HI_ISP_METERING_CB_REF_HIGH_AWB_DATASIZE (12)

// args: data (12-bit)
// data format: unsigned 4.8-bit fixed-point
static __inline HI_VOID hi_isp_metering_cb_ref_high_awb_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0678, (data & 0x0FFF));
}
static __inline HI_U16 hi_isp_metering_cb_ref_high_awb_read(HI_VOID) {
    return (IORD_16DIRECT(0x0678) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_cb_ref_low_awb
// ------------------------------------------------------------------------------ //
// Minimum value of B/G for white region
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_CB_REF_LOW_AWB_DEFAULT (0x000)
#define HI_ISP_METERING_CB_REF_LOW_AWB_DATASIZE (12)

// args: data (12-bit)
// data format: unsigned 4.8-bit fixed-point
static __inline HI_VOID hi_isp_metering_cb_ref_low_awb_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x067C, (data & 0x0FFF));
}
static __inline HI_U16 hi_isp_metering_cb_ref_low_awb_read(HI_VOID) {
    return (IORD_16DIRECT(0x067C) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_af_metrics_shift
// ------------------------------------------------------------------------------ //
// Metrics scaling factor, 0x03 is default
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_AF_METRICS_SHIFT_DEFAULT (0x3)
#define HI_ISP_METERING_AF_METRICS_SHIFT_DATASIZE (4)

// args: data (4-bit)
// data format: unsigned 4-bit integer
static __inline HI_VOID hi_isp_metering_af_metrics_shift_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x069C, (data & 0x0F));
}
static __inline HI_U8 hi_isp_metering_af_metrics_shift_read(HI_VOID) {
    return (IORD_8DIRECT(0x069C) & 0x0F);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_af_metrics
// ------------------------------------------------------------------------------ //
// The integrated and normalized measure of contrast
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_AF_METRICS_DEFAULT (0x0)
#define HI_ISP_METERING_AF_METRICS_DATASIZE (16)

// args: data (16-bit)
// data format: unsigned 16-bit integer
static __inline HI_U16 hi_isp_metering_af_metrics_read(HI_VOID) {
    return IORD_16DIRECT(0x0680);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_af_metrics_alt
// ------------------------------------------------------------------------------ //
// The integrated and normalized measure of contrast - with alternative threshold
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_AF_METRICS_ALT_DEFAULT (0x0)
#define HI_ISP_METERING_AF_METRICS_ALT_DATASIZE (16)

// args: data (16-bit)
// data format: unsigned 16-bit integer
static __inline HI_U16 hi_isp_metering_af_metrics_alt_read(HI_VOID) {
    return IORD_16DIRECT(0x0684);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_af_threshold_write
// ------------------------------------------------------------------------------ //
// The suggested value of AF threshold (or 0 to use threshold from previous frame)
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_AF_THRESHOLD_WRITE_DEFAULT (0x0)
#define HI_ISP_METERING_AF_THRESHOLD_WRITE_DATASIZE (16)

// args: data (16-bit)
// data format: unsigned 16-bit integer
static __inline HI_VOID hi_isp_metering_af_threshold_write_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0688, data);
}
static __inline HI_U16 hi_isp_metering_af_threshold_write_read(HI_VOID) {
    return IORD_16DIRECT(0x0688);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_af_threshold_alt_write
// ------------------------------------------------------------------------------ //
// The suggested value of alternative AF threshold (or 0 to use threshold from previous frame) 
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_AF_THRESHOLD_ALT_WRITE_DEFAULT (0x0)
#define HI_ISP_METERING_AF_THRESHOLD_ALT_WRITE_DATASIZE (16)

// args: data (16-bit)
// data format: unsigned 16-bit integer
static __inline HI_VOID hi_isp_metering_af_threshold_alt_write_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x068C, data);
}
static __inline HI_U16 hi_isp_metering_af_threshold_alt_write_read(HI_VOID) {
    return IORD_16DIRECT(0x068C);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_af_threshold_read
// ------------------------------------------------------------------------------ //
// The calculated value of AF threshold
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_AF_THRESHOLD_READ_DEFAULT (0x0)
#define HI_ISP_METERING_AF_THRESHOLD_READ_DATASIZE (16)

// args: data (16-bit)
// data format: unsigned 16-bit integer
static __inline HI_U16 hi_isp_metering_af_threshold_read_read(HI_VOID) {
    return IORD_16DIRECT(0x0690);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_af_intensity_read
// ------------------------------------------------------------------------------ //
// The calculated value of AF intensity
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_AF_INTENSITY_READ_DEFAULT (0x0)
#define HI_ISP_METERING_AF_INTENSITY_READ_DATASIZE (16)

// args: data (16-bit)
// data format: unsigned 16-bit integer
static __inline HI_U16 hi_isp_metering_af_intensity_read_read(HI_VOID) {
    return IORD_16DIRECT(0x0694);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_af_intensity_zone_read
// ------------------------------------------------------------------------------ //
// The calculated value of AF intensity
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_AF_INTENSITY_ZONE_READ_DEFAULT (0x0)
#define HI_ISP_METERING_AF_INTENSITY_ZONE_READ_DATASIZE (16)

// args: data (16-bit)
// data format: unsigned 16-bit integer
static __inline HI_U16 hi_isp_metering_af_intensity_zone_read_read(HI_VOID) {
    return IORD_16DIRECT(0x0698);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_af_nodes_used_horiz
// ------------------------------------------------------------------------------ //
// Number of active zones horizontally
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_AF_NODES_USED_HORIZ_DEFAULT (17)
#define HI_ISP_METERING_AF_NODES_USED_HORIZ_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_metering_af_nodes_used_horiz_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x06A0, data);
}
static __inline HI_U8 hi_isp_metering_af_nodes_used_horiz_read(HI_VOID) {
    return IORD_8DIRECT(0x06A0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_af_nodes_used_vert
// ------------------------------------------------------------------------------ //
// Number of active zones vertically
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_AF_NODES_USED_VERT_DEFAULT (15)
#define HI_ISP_METERING_AF_NODES_USED_VERT_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_metering_af_nodes_used_vert_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x06A1, data);
}
static __inline HI_U8 hi_isp_metering_af_nodes_used_vert_read(HI_VOID) {
    return IORD_8DIRECT(0x06A1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_af_np_offset
// ------------------------------------------------------------------------------ //
// AF noise profile offset
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_AF_NP_OFFSET_DEFAULT (0x9f)
#define HI_ISP_METERING_AF_NP_OFFSET_DATASIZE (8)

// args: data (8-bit)
// data format: unsigned 4.4-bit fixed-point
static __inline HI_VOID hi_isp_metering_af_np_offset_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x06A4, data);
}
static __inline HI_U8 hi_isp_metering_af_np_offset_read(HI_VOID) {
    return IORD_8DIRECT(0x06A4);
}
// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_af_intensity_norm_mode
// ------------------------------------------------------------------------------ //
// AF intensity normalization mode
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_AF_INTENSITY_NORM_MODE_DEFAULT (0x0)
#define HI_ISP_METERING_AF_INTENSITY_NORM_MODE_DATASIZE (3)

// args: data (2-bit)
// data format: unsigned 8-bit integer
static __inline HI_VOID hi_isp_metering_af_intensity_norm_mode_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x06A2, (data & 0x07));
}
static __inline HI_U8 hi_isp_metering_af_intensity_norm_mode_read(HI_VOID) {
    return (IORD_8DIRECT(0x06A2) & 0x07);
}


// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_skip_x
// ------------------------------------------------------------------------------ //
// Histogram decimation in horizontal direction: 0=every 2nd pixel; 1=every 3rd pixel; 2=every 4th pixel; 3=every 5th pixel; 4=every 8th pixel ; 5+=every 9th pixel
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_SKIP_X_DEFAULT (0)
#define HI_ISP_METERING_SKIP_X_DATASIZE (3)

// args: data (3-bit)0x2014
static __inline HI_VOID hi_isp_metering_skip_x_write(HI_U8 data) {
#ifdef HI_MINI_ISP
	HI_U8 u8Current = IORD_8DIRECT(0x2014);
    IOWR_8DIRECT(0x2014, (data & 0x07) | (u8Current & 0xF8));
#else
	HI_U8 u8Current = IORD_8DIRECT(0x06C0);
    IOWR_8DIRECT(0x06C0, (data & 0x07) | (u8Current & 0xF8));
#endif
}
static __inline HI_U8 hi_isp_metering_skip_x_read(HI_VOID) {
#ifdef HI_MINI_ISP	
    return (IORD_8DIRECT(0x2014) & 0x07);
#else
	return (IORD_8DIRECT(0x06C0) & 0x07);
#endif
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_skip_y
// ------------------------------------------------------------------------------ //
// Histogram decimation in vertical direction: 0=every pixel; 1=every 2nd pixel; 2=every 3rd pixel; 3=every 4th pixel; 4=every 5th pixel; 5=every 8th pixel ; 6+=every 9th pixel
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_SKIP_Y_DEFAULT (0)
#define HI_ISP_METERING_SKIP_Y_DATASIZE (3)

// args: data (3-bit)
static __inline HI_VOID hi_isp_metering_skip_y_write(HI_U8 data) {
#ifdef HI_MINI_ISP
    HI_U8 u8Current = IORD_8DIRECT(0x2014);
    IOWR_8DIRECT(0x2014, ((data & 0x07) << 4) | (u8Current & 0x8F));
#else
	HI_U8 u8Current = IORD_8DIRECT(0x06C0);
	IOWR_8DIRECT(0x06C0, ((data & 0x07) << 4) | (u8Current & 0x8F));
#endif
}
static __inline HI_U8 hi_isp_metering_skip_y_read(HI_VOID) {
#ifdef HI_MINI_ISP	
    return ((IORD_8DIRECT(0x2014) & 0x70) >> 4);
#else
	return ((IORD_8DIRECT(0x06C0) & 0x70) >> 4);
#endif
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_offset_x
// ------------------------------------------------------------------------------ //
// 0= start from the first column;  1=start from second column
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_OFFSET_X_DEFAULT (0)
#define HI_ISP_METERING_OFFSET_X_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_metering_offset_x_write(HI_U8 data) {
#ifdef HI_MINI_ISP
    HI_U8 u8Current = IORD_8DIRECT(0x2014);
    IOWR_8DIRECT(0x2014, ((data & 0x01) << 3) | (u8Current & 0xF7));
#else
	HI_U8 u8Current = IORD_8DIRECT(0x06C0);
	IOWR_8DIRECT(0x06C0, ((data & 0x01) << 3) | (u8Current & 0xF7));
#endif
}
static __inline HI_U8 hi_isp_metering_offset_x_read(HI_VOID) {
#ifdef HI_MINI_ISP	
    return ((IORD_8DIRECT(0x2014) & 0x08) >> 3);
#else
	return ((IORD_8DIRECT(0x06C0) & 0x08) >> 3);
#endif
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_offset_y
// ------------------------------------------------------------------------------ //
// 0= start from the first row; 1= start from second row
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_OFFSET_Y_DEFAULT (0)
#define HI_ISP_METERING_OFFSET_Y_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_metering_offset_y_write(HI_U8 data) {
#ifdef HI_MINI_ISP	
    HI_U8 u8Current = IORD_8DIRECT(0x2014);
    IOWR_8DIRECT(0x2014, ((data & 0x01) << 7) | (u8Current & 0x7F));
#else
	HI_U8 u8Current = IORD_8DIRECT(0x06C0);
	IOWR_8DIRECT(0x06C0, ((data & 0x01) << 7) | (u8Current & 0x7F));
#endif
}
static __inline HI_U8 hi_isp_metering_offset_y_read(HI_VOID) {
#ifdef HI_MINI_ISP		
    return ((IORD_8DIRECT(0x2014) & 0x80) >> 7);
#else
	return ((IORD_8DIRECT(0x06C0) & 0x80) >> 7);
#endif
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_scale_bottom
// ------------------------------------------------------------------------------ //
// scale of bottom half of the range: 0=1x ,1=2x, 2=4x, 4=8x, 4=16x
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_SCALE_BOTTOM_DEFAULT (0)
#define HI_ISP_METERING_SCALE_BOTTOM_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_isp_metering_scale_bottom_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x06C4);
    IOWR_8DIRECT_APICAL(0x06C4, (data & 0x0F) | (u8Current & 0xF0));
}
static __inline HI_U8 hi_isp_metering_scale_bottom_read(HI_VOID) {
    return (IORD_8DIRECT(0x06C4) & 0x0F);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_scale_top
// ------------------------------------------------------------------------------ //
// scale of top half of the range: 0=1x ,1=2x, 2=4x, 4=8x, 4=16x
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_SCALE_TOP_DEFAULT (0)
#define HI_ISP_METERING_SCALE_TOP_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_isp_metering_scale_top_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x06C4);
    IOWR_8DIRECT_APICAL(0x06C4, ((data & 0x0F) << 4) | (u8Current & 0x0F));
}
static __inline HI_U8 hi_isp_metering_scale_top_read(HI_VOID) {
    return ((IORD_8DIRECT(0x06C4) & 0xF0) >> 4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_total_pixels
// ------------------------------------------------------------------------------ //
// Total number of pixels processed (skip x and skip y are taken into account)
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_TOTAL_PIXELS_DEFAULT (0)
#define HI_ISP_METERING_TOTAL_PIXELS_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_isp_metering_total_pixels_read(HI_VOID) {
    return IORD_32DIRECT(0x06C8);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_counted_pixels
// ------------------------------------------------------------------------------ //
// Number of pixels accumulated (with nonzero weight)
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_COUNTED_PIXELS_DEFAULT (0)
#define HI_ISP_METERING_COUNTED_PIXELS_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_isp_metering_counted_pixels_read(HI_VOID) {
    return IORD_32DIRECT(0x06CC);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_sum_r
// ------------------------------------------------------------------------------ //
// Normalized sum of Red pixels
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_SUM_R_DEFAULT (0x0)
#define HI_ISP_METERING_SUM_R_DATASIZE (16)

// args: data (16-bit)
static __inline HI_U16 hi_isp_metering_sum_r_read(HI_VOID) {
    return IORD_16DIRECT(0x0700);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_sum_rg
// ------------------------------------------------------------------------------ //
// Normalized sum of Green (red row) pixels
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_SUM_RG_DEFAULT (0x0)
#define HI_ISP_METERING_SUM_RG_DATASIZE (16)

// args: data (16-bit)
static __inline HI_U16 hi_isp_metering_sum_rg_read(HI_VOID) {
    return IORD_16DIRECT(0x0704);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_sum_bg
// ------------------------------------------------------------------------------ //
// Normalized sum of Green (blue row) pixels
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_SUM_BG_DEFAULT (0x0)
#define HI_ISP_METERING_SUM_BG_DATASIZE (16)

// args: data (16-bit)
static __inline HI_U16 hi_isp_metering_sum_bg_read(HI_VOID) {
    return IORD_16DIRECT(0x0708);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_sum_b
// ------------------------------------------------------------------------------ //
// Normalized sum of Blue pixels
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_SUM_B_DEFAULT (0x0)
#define HI_ISP_METERING_SUM_B_DATASIZE (16)

// args: data (16-bit)
static __inline HI_U16 hi_isp_metering_sum_b_read(HI_VOID) {
    return IORD_16DIRECT(0x070C);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_ae_sum_nodes_used_horiz
// ------------------------------------------------------------------------------ //
// Number of active zones horizontally
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_AE_SUM_NODES_USED_HORIZ_DEFAULT (17)
#define HI_ISP_METERING_AE_SUM_NODES_USED_HORIZ_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_metering_ae_sum_nodes_used_horiz_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x0710, data);
}
static __inline HI_U8 hi_isp_metering_ae_sum_nodes_used_horiz_read(HI_VOID) {
    return IORD_8DIRECT(0x0710);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_ae_sum_nodes_used_vert
// ------------------------------------------------------------------------------ //
// Number of active zones vertically
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_AE_SUM_NODES_USED_VERT_DEFAULT (15)
#define HI_ISP_METERING_AE_SUM_NODES_USED_VERT_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_metering_ae_sum_nodes_used_vert_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x0711, data);
}
static __inline HI_U8 hi_isp_metering_ae_sum_nodes_used_vert_read(HI_VOID) {
    return IORD_8DIRECT(0x0711);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_avg_r
// ------------------------------------------------------------------------------ //
// Normalized sum of Red component
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_AVG_R_DEFAULT (0x0)
#define HI_ISP_METERING_AVG_R_DATASIZE (16)

// args: data (16-bit)
static __inline HI_U16 hi_isp_metering_avg_r_read(HI_VOID) {
#ifdef HI_MINI_ISP
    return IORD_16DIRECT(0x2140);
#else
	return IORD_16DIRECT(0x0740);
#endif
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_avg_g
// ------------------------------------------------------------------------------ //
// Normalized sum of Green component
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_AVG_G_DEFAULT (0x0)
#define HI_ISP_METERING_AVG_G_DATASIZE (16)

// args: data (16-bit)
static __inline HI_U16 hi_isp_metering_avg_g_read(HI_VOID) {
#ifdef HI_MINI_ISP
    return IORD_16DIRECT(0x2144);
#else
	return IORD_16DIRECT(0x0744);
#endif
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_avg_b
// ------------------------------------------------------------------------------ //
// Normalized sum of Blue component
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_AVG_B_DEFAULT (0x0)
#define HI_ISP_METERING_AVG_B_DATASIZE (16)

// args: data (16-bit)
static __inline HI_U16 hi_isp_metering_avg_b_read(HI_VOID) {
#ifdef HI_MINI_ISP
    return IORD_16DIRECT(0x2148);
#else
	return IORD_16DIRECT(0x0748);
#endif
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_count_all
// ------------------------------------------------------------------------------ //
// Normalized count of pixels which were averaged in Avg R, Avg G and Avg B
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_COUNT_ALL_DEFAULT (0x0)
#define HI_ISP_METERING_COUNT_ALL_DATASIZE (16)

// args: data (16-bit)
static __inline HI_U16 hi_isp_metering_count_all_read(HI_VOID) {
#ifdef HI_MINI_ISP
    return IORD_16DIRECT(0x214C);
#else
	return IORD_16DIRECT(0x074C);
#endif
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_count_min
// ------------------------------------------------------------------------------ //
// Normalized count of pixels below min threshold 
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_COUNT_MIN_DEFAULT (0x0)
#define HI_ISP_METERING_COUNT_MIN_DATASIZE (16)

// args: data (16-bit)
static __inline HI_U16 hi_isp_metering_count_min_read(HI_VOID) {
#ifdef HI_MINI_ISP
    return IORD_16DIRECT(0x2150);
#else
	return IORD_16DIRECT(0x0750);
#endif
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_count_max
// ------------------------------------------------------------------------------ //
// Normalized count of pixels above max threshold
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_COUNT_MAX_DEFAULT (0x0)
#define HI_ISP_METERING_COUNT_MAX_DATASIZE (16)

// args: data (16-bit)
static __inline HI_U16 hi_isp_metering_count_max_read(HI_VOID) {
#ifdef HI_MINI_ISP
    return IORD_16DIRECT(0x2154);
#else
	return IORD_16DIRECT(0x0754);
#endif
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_awb_sum_nodes_used_horiz
// ------------------------------------------------------------------------------ //
// Number of active zones horizontally
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_AWB_SUM_NODES_USED_HORIZ_DEFAULT (17)
#define HI_ISP_METERING_AWB_SUM_NODES_USED_HORIZ_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_metering_awb_sum_nodes_used_horiz_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x0760, data);
}
static __inline HI_U8 hi_isp_metering_awb_sum_nodes_used_horiz_read(HI_VOID) {
    return IORD_8DIRECT(0x0760);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_awb_sum_nodes_used_vert
// ------------------------------------------------------------------------------ //
// Number of active zones vertically
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_AWB_SUM_NODES_USED_VERT_DEFAULT (15)
#define HI_ISP_METERING_AWB_SUM_NODES_USED_VERT_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_metering_awb_sum_nodes_used_vert_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x0761, data);
}
static __inline HI_U8 hi_isp_metering_awb_sum_nodes_used_vert_read(HI_VOID) {
    return IORD_8DIRECT(0x0761);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_min_threshold
// ------------------------------------------------------------------------------ //
// Minimal threshold for AWB sums 
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_MIN_THRESHOLD_DEFAULT (0x10)
#define HI_ISP_METERING_MIN_THRESHOLD_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_metering_min_threshold_write(HI_U16 data) {
#ifdef HI_MINI_ISP
    IOWR_16DIRECT(0x2118, (data & 0x0FFF));
#else
	IOWR_16DIRECT(0x0764, (data & 0x0FFF));
#endif
}
static __inline HI_U16 hi_isp_metering_min_threshold_read(HI_VOID) {
#ifdef HI_MINI_ISP	
    return (IORD_16DIRECT(0x2118) & 0x0FFF);
#else
	return (IORD_16DIRECT(0x0764) & 0x0FFF);
#endif
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_max_threshold
// ------------------------------------------------------------------------------ //
// Maximal threshold for AWB sums 
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_MAX_THRESHOLD_DEFAULT (0xff0)
#define HI_ISP_METERING_MAX_THRESHOLD_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_isp_metering_max_threshold_write(HI_U16 data) {
#ifdef HI_MINI_ISP
    IOWR_16DIRECT(0x211C, (data & 0x0FFF));
#else
	IOWR_16DIRECT(0x0768, (data & 0x0FFF));
#endif
}
static __inline HI_U16 hi_isp_metering_max_threshold_read(HI_VOID) {
#ifdef HI_MINI_ISP	
    return (IORD_16DIRECT(0x211C) & 0x0FFF);
#else
	return (IORD_16DIRECT(0x0768) & 0x0FFF);
#endif
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_min_clip
// ------------------------------------------------------------------------------ //
// Clip pixels below minimal threshold for AWB sums
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_MIN_CLIP_DEFAULT (0)
#define HI_ISP_METERING_MIN_CLIP_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_metering_min_clip_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x076C);
    IOWR_8DIRECT_APICAL(0x076C, (data & 0x01) | (u8Current & 0xFE));
}
static __inline HI_U8 hi_isp_metering_min_clip_read(HI_VOID) {
    return (IORD_8DIRECT(0x076C) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_max_clip
// ------------------------------------------------------------------------------ //
// Clip pixels ablove maximal threshold for AWB sums
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_MAX_CLIP_DEFAULT (0)
#define HI_ISP_METERING_MAX_CLIP_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_metering_max_clip_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x076C);
    IOWR_8DIRECT_APICAL(0x076C, ((data & 0x01) << 1) | (u8Current & 0xFD));
}
static __inline HI_U8 hi_isp_metering_max_clip_read(HI_VOID) {
    return ((IORD_8DIRECT(0x076C) & 0x02) >> 1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_cr_ref_max_awb_sum
// ------------------------------------------------------------------------------ //
// Maximum value of R/G for white region
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_CR_REF_MAX_AWB_SUM_DEFAULT (0x1FF)
#define HI_ISP_METERING_CR_REF_MAX_AWB_SUM_DATASIZE (12)

// args: data (12-bit)
// data format: unsigned 4.8-bit fixed-point
static __inline HI_VOID hi_isp_metering_cr_ref_max_awb_sum_write(HI_U16 data) {
#ifdef HI_MINI_ISP
    IOWR_16DIRECT(0x2120, (data & 0x0FFF));
#else
	IOWR_16DIRECT(0x0770, (data & 0x0FFF));
#endif
}
static __inline HI_U16 hi_isp_metering_cr_ref_max_awb_sum_read(HI_VOID) {
#ifdef HI_MINI_ISP	
    return (IORD_16DIRECT(0x2120) & 0x0FFF);
#else
	return (IORD_16DIRECT(0x0770) & 0x0FFF);
#endif
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_cr_ref_min_awb_sum
// ------------------------------------------------------------------------------ //
// Minimum value of R/G for white region
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_CR_REF_MIN_AWB_SUM_DEFAULT (0x040)
#define HI_ISP_METERING_CR_REF_MIN_AWB_SUM_DATASIZE (12)

// args: data (12-bit)
// data format: unsigned 4.8-bit fixed-point
static __inline HI_VOID hi_isp_metering_cr_ref_min_awb_sum_write(HI_U16 data) {
#ifdef HI_MINI_ISP
    IOWR_16DIRECT(0x2122, (data & 0x0FFF));
#else
	IOWR_16DIRECT(0x0772, (data & 0x0FFF));
#endif
}
static __inline HI_U16 hi_isp_metering_cr_ref_min_awb_sum_read(HI_VOID) {
#ifdef HI_MINI_ISP	
    return (IORD_16DIRECT(0x2122) & 0x0FFF);
#else
	return (IORD_16DIRECT(0x0772) & 0x0FFF);
#endif
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_cb_ref_max_awb_sum
// ------------------------------------------------------------------------------ //
// Maximum value of B/G for white region
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_CB_REF_MAX_AWB_SUM_DEFAULT (0x1FF)
#define HI_ISP_METERING_CB_REF_MAX_AWB_SUM_DATASIZE (12)

// args: data (12-bit)
// data format: unsigned 4.8-bit fixed-point
static __inline HI_VOID hi_isp_metering_cb_ref_max_awb_sum_write(HI_U16 data) {
#ifdef HI_MINI_ISP
    IOWR_16DIRECT(0x2124, (data & 0x0FFF));
#else
	IOWR_16DIRECT(0x0774, (data & 0x0FFF));
#endif
}
static __inline HI_U16 hi_isp_metering_cb_ref_max_awb_sum_read(HI_VOID) {
#ifdef HI_MINI_ISP	
    return (IORD_16DIRECT(0x2124) & 0x0FFF);
#else
	return (IORD_16DIRECT(0x0774) & 0x0FFF);
#endif
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_cb_ref_min_awb_sum
// ------------------------------------------------------------------------------ //
// Minimum value of B/G for white region
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_CB_REF_MIN_AWB_SUM_DEFAULT (0x040)
#define HI_ISP_METERING_CB_REF_MIN_AWB_SUM_DATASIZE (12)

// args: data (12-bit)
// data format: unsigned 4.8-bit fixed-point
static __inline HI_VOID hi_isp_metering_cb_ref_min_awb_sum_write(HI_U16 data) {
#ifdef HI_MINI_ISP
    IOWR_16DIRECT(0x2126, (data & 0x0FFF));
#else
	IOWR_16DIRECT(0x0776, (data & 0x0FFF));
#endif
}
static __inline HI_U16 hi_isp_metering_cb_ref_min_awb_sum_read(HI_VOID) {
#ifdef HI_MINI_ISP	
    return (IORD_16DIRECT(0x2126) & 0x0FFF);
#else
	return (IORD_16DIRECT(0x0776) & 0x0FFF);
#endif
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_cr_ref_high_awb_sum
// ------------------------------------------------------------------------------ //
// Maximum value of R/G for white region
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_CR_REF_HIGH_AWB_SUM_DEFAULT (0xFFF)
#define HI_ISP_METERING_CR_REF_HIGH_AWB_SUM_DATASIZE (12)

// args: data (12-bit)
// data format: unsigned 4.8-bit fixed-point
static __inline HI_VOID hi_isp_metering_cr_ref_high_awb_sum_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0778, (data & 0x0FFF));
}
static __inline HI_U16 hi_isp_metering_cr_ref_high_awb_sum_read(HI_VOID) {
    return (IORD_16DIRECT(0x0778) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_cr_ref_low_awb_sum
// ------------------------------------------------------------------------------ //
// Minimum value of R/G for white region
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_CR_REF_LOW_AWB_SUM_DEFAULT (0x000)
#define HI_ISP_METERING_CR_REF_LOW_AWB_SUM_DATASIZE (12)

// args: data (12-bit)
// data format: unsigned 4.8-bit fixed-point
static __inline HI_VOID hi_isp_metering_cr_ref_low_awb_sum_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x077A, (data & 0x0FFF));
}
static __inline HI_U16 hi_isp_metering_cr_ref_low_awb_sum_read(HI_VOID) {
    return (IORD_16DIRECT(0x077A) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_cb_ref_high_awb_sum
// ------------------------------------------------------------------------------ //
// Maximum value of B/G for white region
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_CB_REF_HIGH_AWB_SUM_DEFAULT (0xFFF)
#define HI_ISP_METERING_CB_REF_HIGH_AWB_SUM_DATASIZE (12)

// args: data (12-bit)
// data format: unsigned 4.8-bit fixed-point
static __inline HI_VOID hi_isp_metering_cb_ref_high_awb_sum_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x077C, (data & 0x0FFF));
}
static __inline HI_U16 hi_isp_metering_cb_ref_high_awb_sum_read(HI_VOID) {
    return (IORD_16DIRECT(0x077C) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_metering_cb_ref_low_awb_sum
// ------------------------------------------------------------------------------ //
// Minimum value of B/G for white region
// ------------------------------------------------------------------------------ //

#define HI_ISP_METERING_CB_REF_LOW_AWB_SUM_DEFAULT (0x000)
#define HI_ISP_METERING_CB_REF_LOW_AWB_SUM_DATASIZE (12)

// args: data (12-bit)
// data format: unsigned 4.8-bit fixed-point
static __inline HI_VOID hi_isp_metering_cb_ref_low_awb_sum_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x077E, (data & 0x0FFF));
}
static __inline HI_U16 hi_isp_metering_cb_ref_low_awb_sum_read(HI_VOID) {
    return (IORD_16DIRECT(0x077E) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Group: Zones
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_isp_zones_aexp_weight
// ------------------------------------------------------------------------------ //
// Sets zone weighting for auto exposure. Index is (row,col) where (0,0) is top-left zone
// ------------------------------------------------------------------------------ //

#define HI_ISP_ZONES_AEXP_WEIGHT_DEFAULT (0xF)
#define HI_ISP_ZONES_AEXP_WEIGHT_DATASIZE (4)

// args: index1 (0-16), index2 (0-14), data (4-bit)
static __inline HI_VOID hi_isp_zones_aexp_weight_write(HI_U32 index1, HI_U32 index2, HI_U8 data) {
    HI_U32 addr = 0x0A00 + (((index1 * 17) + index2) << 0);
    IOWR_8DIRECT_APICAL(addr, data & 0x0F);
}
static __inline HI_U8 hi_isp_zones_aexp_weight_read(HI_U32 index1, HI_U32 index2) {
    HI_U32 addr = 0x0A00 + (((index1 * 17) + index2) << 0);
    return (HI_U8) IORD_8DIRECT(addr);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_zones_awb_weight
// ------------------------------------------------------------------------------ //
// Sets zone weighting for auto white balance. Index is (row,col) where (0,0) is top-left zone
// ------------------------------------------------------------------------------ //

#define HI_ISP_ZONES_AWB_WEIGHT_DEFAULT (0xF)
#define HI_ISP_ZONES_AWB_WEIGHT_DATASIZE (4)

// args: index1 (0-14), index2 (0-16), data (4-bit)
static __inline HI_VOID hi_isp_zones_awb_weight_write(HI_U32 index1, HI_U32 index2, HI_U8 data) {
    //HI_U32 addr = 0x0B00 + (((index1 * 17) + index2) << 0);
    //IOWR_8DIRECT_APICAL(addr, data & 0x0F);
}
static __inline HI_U8 hi_isp_zones_awb_weight_read(HI_U32 index1, HI_U32 index2) {
    HI_U32 addr = 0x0B00 + (((index1 * 17) + index2) << 0);
    return (HI_U8) IORD_8DIRECT(addr);
}

// ------------------------------------------------------------------------------ //
// Group: Colour space conv
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_isp_cs_conv_enable_matrix
// ------------------------------------------------------------------------------ //
// Color matrix enable: 0=off 1=on
// ------------------------------------------------------------------------------ //

#define HI_ISP_CS_CONV_ENABLE_MATRIX_DEFAULT (0)
#define HI_ISP_CS_CONV_ENABLE_MATRIX_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_cs_conv_enable_matrix_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0530);
    IOWR_8DIRECT_APICAL(0x0530, (data & 0x01) | (u8Current & 0xFE));
}
static __inline HI_U8 hi_isp_cs_conv_enable_matrix_read(HI_VOID) {
    return (IORD_8DIRECT(0x0530) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_cs_conv_enable_filter
// ------------------------------------------------------------------------------ //
// Filter enable: 0=off 1=on
// ------------------------------------------------------------------------------ //

#define HI_ISP_CS_CONV_ENABLE_FILTER_DEFAULT (0)
#define HI_ISP_CS_CONV_ENABLE_FILTER_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_cs_conv_enable_filter_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0530);
    IOWR_8DIRECT_APICAL(0x0530, ((data & 0x01) << 1) | (u8Current & 0xFD));
}
static __inline HI_U8 hi_isp_cs_conv_enable_filter_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0530) & 0x02) >> 1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_cs_conv_enable_downsample
// ------------------------------------------------------------------------------ //
// Downsample enable: 0=off 1=on
// ------------------------------------------------------------------------------ //

#define HI_ISP_CS_CONV_ENABLE_DOWNSAMPLE_DEFAULT (0)
#define HI_ISP_CS_CONV_ENABLE_DOWNSAMPLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_cs_conv_enable_downsample_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0530);
    IOWR_8DIRECT_APICAL(0x0530, ((data & 0x01) << 2) | (u8Current & 0xFB));
}
static __inline HI_U8 hi_isp_cs_conv_enable_downsample_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0530) & 0x04) >> 2);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_cs_conv_coefft_11
// ------------------------------------------------------------------------------ //
// Matrix coefficient for R-Y multiplier. Format: sign/magnitude 8.8-bit fixed-point 
// ------------------------------------------------------------------------------ //

#define HI_ISP_CS_CONV_COEFFT_11_DEFAULT (0x002f)
#define HI_ISP_CS_CONV_COEFFT_11_DATASIZE (16)

// args: data (16-bit)
// data format: sign/magnitude 8.8-bit fixed-point
static __inline HI_VOID hi_isp_cs_conv_coefft_11_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0500, data);
}
static __inline HI_U16 hi_isp_cs_conv_coefft_11_read(HI_VOID) {
    return IORD_16DIRECT(0x0500);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_cs_conv_coefft_12
// ------------------------------------------------------------------------------ //
// Matrix coefficient for G-Y multiplier. Format: sign/magnitude 8.8-bit fixed-point 
// ------------------------------------------------------------------------------ //

#define HI_ISP_CS_CONV_COEFFT_12_DEFAULT (0x009d)
#define HI_ISP_CS_CONV_COEFFT_12_DATASIZE (16)

// args: data (16-bit)
// data format: sign/magnitude 8.8-bit fixed-point
static __inline HI_VOID hi_isp_cs_conv_coefft_12_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0504, data);
}
static __inline HI_U16 hi_isp_cs_conv_coefft_12_read(HI_VOID) {
    return IORD_16DIRECT(0x0504);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_cs_conv_coefft_13
// ------------------------------------------------------------------------------ //
// Matrix coefficient for B-Y multiplier. Format: sign/magnitude 8.8-bit fixed-point 
// ------------------------------------------------------------------------------ //

#define HI_ISP_CS_CONV_COEFFT_13_DEFAULT (0x0010)
#define HI_ISP_CS_CONV_COEFFT_13_DATASIZE (16)

// args: data (16-bit)
// data format: sign/magnitude 8.8-bit fixed-point
static __inline HI_VOID hi_isp_cs_conv_coefft_13_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0508, data);
}
static __inline HI_U16 hi_isp_cs_conv_coefft_13_read(HI_VOID) {
    return IORD_16DIRECT(0x0508);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_cs_conv_coefft_21
// ------------------------------------------------------------------------------ //
// Matrix coefficient for R-Cb multiplier. Format: sign/magnitude 8.8-bit fixed-point 
// ------------------------------------------------------------------------------ //

#define HI_ISP_CS_CONV_COEFFT_21_DEFAULT (0x801a)
#define HI_ISP_CS_CONV_COEFFT_21_DATASIZE (16)

// args: data (16-bit)
// data format: sign/magnitude 8.8-bit fixed-point
static __inline HI_VOID hi_isp_cs_conv_coefft_21_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x050C, data);
}
static __inline HI_U16 hi_isp_cs_conv_coefft_21_read(HI_VOID) {
    return IORD_16DIRECT(0x050C);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_cs_conv_coefft_22
// ------------------------------------------------------------------------------ //
// Matrix coefficient for G-Cb multiplier. Format: sign/magnitude 8.8-bit fixed-point 
// ------------------------------------------------------------------------------ //

#define HI_ISP_CS_CONV_COEFFT_22_DEFAULT (0x8057)
#define HI_ISP_CS_CONV_COEFFT_22_DATASIZE (16)

// args: data (16-bit)
// data format: sign/magnitude 8.8-bit fixed-point
static __inline HI_VOID hi_isp_cs_conv_coefft_22_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0510, data);
}
static __inline HI_U16 hi_isp_cs_conv_coefft_22_read(HI_VOID) {
    return IORD_16DIRECT(0x0510);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_cs_conv_coefft_23
// ------------------------------------------------------------------------------ //
// Matrix coefficient for B-Cb multiplier. Format: sign/magnitude 8.8-bit fixed-point 
// ------------------------------------------------------------------------------ //

#define HI_ISP_CS_CONV_COEFFT_23_DEFAULT (0x0070)
#define HI_ISP_CS_CONV_COEFFT_23_DATASIZE (16)

// args: data (16-bit)
// data format: sign/magnitude 8.8-bit fixed-point
static __inline HI_VOID hi_isp_cs_conv_coefft_23_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0514, data);
}
static __inline HI_U16 hi_isp_cs_conv_coefft_23_read(HI_VOID) {
    return IORD_16DIRECT(0x0514);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_cs_conv_coefft_31
// ------------------------------------------------------------------------------ //
// Matrix coefficient for R-Cr multiplier. Format: sign/magnitude 8.8-bit fixed-point 
// ------------------------------------------------------------------------------ //

#define HI_ISP_CS_CONV_COEFFT_31_DEFAULT (0x0070)
#define HI_ISP_CS_CONV_COEFFT_31_DATASIZE (16)

// args: data (16-bit)
// data format: sign/magnitude 8.8-bit fixed-point
static __inline HI_VOID hi_isp_cs_conv_coefft_31_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0518, data);
}
static __inline HI_U16 hi_isp_cs_conv_coefft_31_read(HI_VOID) {
    return IORD_16DIRECT(0x0518);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_cs_conv_coefft_32
// ------------------------------------------------------------------------------ //
// Matrix coefficient for G-Cr multiplier. Format: sign/magnitude 8.8-bit fixed-point 
// ------------------------------------------------------------------------------ //

#define HI_ISP_CS_CONV_COEFFT_32_DEFAULT (0x8066)
#define HI_ISP_CS_CONV_COEFFT_32_DATASIZE (16)

// args: data (16-bit)
// data format: sign/magnitude 8.8-bit fixed-point
static __inline HI_VOID hi_isp_cs_conv_coefft_32_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x051C, data);
}
static __inline HI_U16 hi_isp_cs_conv_coefft_32_read(HI_VOID) {
    return IORD_16DIRECT(0x051C);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_cs_conv_coefft_33
// ------------------------------------------------------------------------------ //
// Matrix coefficient for B-Cr multiplier. Format: sign/magnitude 8.8-bit fixed-point 
// ------------------------------------------------------------------------------ //

#define HI_ISP_CS_CONV_COEFFT_33_DEFAULT (0x800a)
#define HI_ISP_CS_CONV_COEFFT_33_DATASIZE (16)

// args: data (16-bit)
// data format: sign/magnitude 8.8-bit fixed-point
static __inline HI_VOID hi_isp_cs_conv_coefft_33_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0520, data);
}
static __inline HI_U16 hi_isp_cs_conv_coefft_33_read(HI_VOID) {
    return IORD_16DIRECT(0x0520);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_cs_conv_coefft_o1
// ------------------------------------------------------------------------------ //
// Offset for Y  
// ------------------------------------------------------------------------------ //

#define HI_ISP_CS_CONV_COEFFT_O1_DEFAULT (0x000)
#define HI_ISP_CS_CONV_COEFFT_O1_DATASIZE (11)

// args: data (11-bit)
// data format: signed (2's complement) 11-bit integer
static __inline HI_VOID hi_isp_cs_conv_coefft_o1_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0524, (data & 0x07FF));
}
static __inline HI_U16 hi_isp_cs_conv_coefft_o1_read(HI_VOID) {
    return (IORD_16DIRECT(0x0524) & 0x07FF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_cs_conv_coefft_o2
// ------------------------------------------------------------------------------ //
// Offset for Cb
// ------------------------------------------------------------------------------ //

#define HI_ISP_CS_CONV_COEFFT_O2_DEFAULT (0x200)
#define HI_ISP_CS_CONV_COEFFT_O2_DATASIZE (11)

// args: data (11-bit)
// data format: signed (2's complement) 11-bit integer
static __inline HI_VOID hi_isp_cs_conv_coefft_o2_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0528, (data & 0x07FF));
}
static __inline HI_U16 hi_isp_cs_conv_coefft_o2_read(HI_VOID) {
    return (IORD_16DIRECT(0x0528) & 0x07FF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_cs_conv_coefft_o3
// ------------------------------------------------------------------------------ //
// Offset for Cr
// ------------------------------------------------------------------------------ //

#define HI_ISP_CS_CONV_COEFFT_O3_DEFAULT (0x200)
#define HI_ISP_CS_CONV_COEFFT_O3_DATASIZE (11)

// args: data (11-bit)
// data format: signed (2's complement) 11-bit integer
static __inline HI_VOID hi_isp_cs_conv_coefft_o3_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x052C, (data & 0x07FF));
}
static __inline HI_U16 hi_isp_cs_conv_coefft_o3_read(HI_VOID) {
    return (IORD_16DIRECT(0x052C) & 0x07FF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_cs_conv_clip_min_y
// ------------------------------------------------------------------------------ //
// Minimal value for Y
// ------------------------------------------------------------------------------ //

#define HI_ISP_CS_CONV_CLIP_MIN_Y_DEFAULT (0x000)
#define HI_ISP_CS_CONV_CLIP_MIN_Y_DATASIZE (10)

// args: data (10-bit)
// data format: signed (2's complement) 10-bit integer
static __inline HI_VOID hi_isp_cs_conv_clip_min_y_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0538, (data & 0x03FF));
}
static __inline HI_U16 hi_isp_cs_conv_clip_min_y_read(HI_VOID) {
    return (IORD_16DIRECT(0x0538) & 0x03FF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_cs_conv_clip_max_y
// ------------------------------------------------------------------------------ //
// Maximal value for Y
// ------------------------------------------------------------------------------ //

#define HI_ISP_CS_CONV_CLIP_MAX_Y_DEFAULT (0x3FF)
#define HI_ISP_CS_CONV_CLIP_MAX_Y_DATASIZE (10)

// args: data (10-bit)
// data format: signed (2's complement) 10-bit integer
static __inline HI_VOID hi_isp_cs_conv_clip_max_y_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x053A, (data & 0x03FF));
}
static __inline HI_U16 hi_isp_cs_conv_clip_max_y_read(HI_VOID) {
    return (IORD_16DIRECT(0x053A) & 0x03FF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_cs_conv_clip_min_uv
// ------------------------------------------------------------------------------ //
// Minimal value for Cb, Cr
// ------------------------------------------------------------------------------ //

#define HI_ISP_CS_CONV_CLIP_MIN_UV_DEFAULT (0x000)
#define HI_ISP_CS_CONV_CLIP_MIN_UV_DATASIZE (10)

// args: data (10-bit)
// data format: signed (2's complement) 10-bit integer
static __inline HI_VOID hi_isp_cs_conv_clip_min_uv_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x053C, (data & 0x03FF));
}
static __inline HI_U16 hi_isp_cs_conv_clip_min_uv_read(HI_VOID) {
    return (IORD_16DIRECT(0x053C) & 0x03FF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_cs_conv_clip_max_uv
// ------------------------------------------------------------------------------ //
// Maximal value for Cb, Cr
// ------------------------------------------------------------------------------ //

#define HI_ISP_CS_CONV_CLIP_MAX_UV_DEFAULT (0x3FF)
#define HI_ISP_CS_CONV_CLIP_MAX_UV_DATASIZE (10)

// args: data (10-bit)
// data format: signed (2's complement) 10-bit integer
static __inline HI_VOID hi_isp_cs_conv_clip_max_uv_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x053E, (data & 0x03FF));
}
static __inline HI_U16 hi_isp_cs_conv_clip_max_uv_read(HI_VOID) {
    return (IORD_16DIRECT(0x053E) & 0x03FF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_cs_conv_data_mask_ry
// ------------------------------------------------------------------------------ //
// Data mask for channel 1 (R or Y)
// ------------------------------------------------------------------------------ //

#define HI_ISP_CS_CONV_DATA_MASK_RY_DEFAULT (0x3FF)
#define HI_ISP_CS_CONV_DATA_MASK_RY_DATASIZE (10)

// args: data (10-bit)
static __inline HI_VOID hi_isp_cs_conv_data_mask_ry_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0540, (data & 0x03FF));
}
static __inline HI_U16 hi_isp_cs_conv_data_mask_ry_read(HI_VOID) {
    return (IORD_16DIRECT(0x0540) & 0x03FF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_cs_conv_data_mask_gu
// ------------------------------------------------------------------------------ //
// Data mask for channel 2 (G or U)
// ------------------------------------------------------------------------------ //

#define HI_ISP_CS_CONV_DATA_MASK_GU_DEFAULT (0x3FF)
#define HI_ISP_CS_CONV_DATA_MASK_GU_DATASIZE (10)

// args: data (10-bit)
static __inline HI_VOID hi_isp_cs_conv_data_mask_gu_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0544, (data & 0x03FF));
}
static __inline HI_U16 hi_isp_cs_conv_data_mask_gu_read(HI_VOID) {
    return (IORD_16DIRECT(0x0544) & 0x03FF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_cs_conv_data_mask_bv
// ------------------------------------------------------------------------------ //
// Data mask for channel 3 (B or V)
// ------------------------------------------------------------------------------ //

#define HI_ISP_CS_CONV_DATA_MASK_BV_DEFAULT (0x3FF)
#define HI_ISP_CS_CONV_DATA_MASK_BV_DATASIZE (10)

// args: data (10-bit)
static __inline HI_VOID hi_isp_cs_conv_data_mask_bv_write(HI_U16 data) {
    IOWR_16DIRECT_APICAL(0x0548, (data & 0x03FF));
}
static __inline HI_U16 hi_isp_cs_conv_data_mask_bv_read(HI_VOID) {
    return (IORD_16DIRECT(0x0548) & 0x03FF);
}

// ------------------------------------------------------------------------------ //
// Group: Dither
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_isp_dither_enable_dither
// ------------------------------------------------------------------------------ //
// Enables dithering module
// ------------------------------------------------------------------------------ //

#define HI_ISP_DITHER_ENABLE_DITHER_DEFAULT (0x0)
#define HI_ISP_DITHER_ENABLE_DITHER_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_dither_enable_dither_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0580);
    IOWR_8DIRECT_APICAL(0x0580, (data & 0x01) | (u8Current & 0xFE));
}
static __inline HI_U8 hi_isp_dither_enable_dither_read(HI_VOID) {
    return (IORD_8DIRECT(0x0580) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_dither_dither_amount
// ------------------------------------------------------------------------------ //
// 0= dither to 9 bits; 1=dither to 8 bits; 2=dither to 7 bits; 3=dither to 6 bits
// ------------------------------------------------------------------------------ //

#define HI_ISP_DITHER_DITHER_AMOUNT_DEFAULT (0x0)
#define HI_ISP_DITHER_DITHER_AMOUNT_DATASIZE (2)

// args: data (2-bit)
static __inline HI_VOID hi_isp_dither_dither_amount_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0580);
    IOWR_8DIRECT_APICAL(0x0580, ((data & 0x03) << 1) | (u8Current & 0xF9));
}
static __inline HI_U8 hi_isp_dither_dither_amount_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0580) & 0x06) >> 1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_dither_shift_mode
// ------------------------------------------------------------------------------ //
// 0= output is LSB aligned; 1=output is MSB aligned
// ------------------------------------------------------------------------------ //

#define HI_ISP_DITHER_SHIFT_MODE_DEFAULT (0x0)
#define HI_ISP_DITHER_SHIFT_MODE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_dither_shift_mode_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x0580);
    IOWR_8DIRECT_APICAL(0x0580, ((data & 0x01) << 4) | (u8Current & 0xEF));
}
static __inline HI_U8 hi_isp_dither_shift_mode_read(HI_VOID) {
    return ((IORD_8DIRECT(0x0580) & 0x10) >> 4);
}

// ------------------------------------------------------------------------------ //
// Group: DIS
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_isp_dis_manual_control
// ------------------------------------------------------------------------------ //
// Disables DIS and enables manual output offset controls
// ------------------------------------------------------------------------------ //

#define HI_ISP_DIS_MANUAL_CONTROL_DEFAULT (0)
#define HI_ISP_DIS_MANUAL_CONTROL_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_dis_manual_control_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x07E0);
    IOWR_8DIRECT_APICAL(0x07E0, (data & 0x01) | (u8Current & 0xFE));
}
static __inline HI_U8 hi_isp_dis_manual_control_read(HI_VOID) {
    return (IORD_8DIRECT(0x07E0) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_dis_bypass_motion_filter
// ------------------------------------------------------------------------------ //
// Disables motion filter (for debug purposes)
// ------------------------------------------------------------------------------ //

#define HI_ISP_DIS_BYPASS_MOTION_FILTER_DEFAULT (0)
#define HI_ISP_DIS_BYPASS_MOTION_FILTER_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_dis_bypass_motion_filter_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x07E0);
    IOWR_8DIRECT_APICAL(0x07E0, ((data & 0x01) << 1) | (u8Current & 0xFD));
}
static __inline HI_U8 hi_isp_dis_bypass_motion_filter_read(HI_VOID) {
    return ((IORD_8DIRECT(0x07E0) & 0x02) >> 1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_dis_raw_filter
// ------------------------------------------------------------------------------ //
// Enables filtering of raw input data
// ------------------------------------------------------------------------------ //

#define HI_ISP_DIS_RAW_FILTER_DEFAULT (0)
#define HI_ISP_DIS_RAW_FILTER_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_dis_raw_filter_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x07E0);
    IOWR_8DIRECT_APICAL(0x07E0, ((data & 0x01) << 2) | (u8Current & 0xFB));
}
static __inline HI_U8 hi_isp_dis_raw_filter_read(HI_VOID) {
    return ((IORD_8DIRECT(0x07E0) & 0x04) >> 2);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_dis_shift_mux
// ------------------------------------------------------------------------------ //
// Scaling factor for the internal accumulators.
//   0 = no shift
//   1 = internal results are multiplied by 4
//   2 = internal results are multiplied by 16
//   3 = internal results are multiplied by 64
// ------------------------------------------------------------------------------ //

#define HI_ISP_DIS_SHIFT_MUX_DEFAULT (0x00)
#define HI_ISP_DIS_SHIFT_MUX_DATASIZE (2)

#define HI_ISP_DIS_SHIFT_MUX_NO_SHIFT (0)
#define HI_ISP_DIS_SHIFT_MUX_INTERNAL_RESULTS_ARE_MULTIPLIED_BY_4 (1)
#define HI_ISP_DIS_SHIFT_MUX_INTERNAL_RESULTS_ARE_MULTIPLIED_BY_16 (2)
#define HI_ISP_DIS_SHIFT_MUX_INTERNAL_RESULTS_ARE_MULTIPLIED_BY_64 (3)

// args: data (2-bit)
static __inline HI_VOID hi_isp_dis_shift_mux_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x07E1, (data & 0x03));
}
static __inline HI_U8 hi_isp_dis_shift_mux_read(HI_VOID) {
    return (IORD_8DIRECT(0x07E1) & 0x03);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_dis_manual_offset_x
// ------------------------------------------------------------------------------ //
// Manual X offset control
// ------------------------------------------------------------------------------ //

#define HI_ISP_DIS_MANUAL_OFFSET_X_DEFAULT (0x80)
#define HI_ISP_DIS_MANUAL_OFFSET_X_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_dis_manual_offset_x_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x07E8, data);
}
static __inline HI_U8 hi_isp_dis_manual_offset_x_read(HI_VOID) {
    return IORD_8DIRECT(0x07E8);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_dis_manual_offset_y
// ------------------------------------------------------------------------------ //
// Manual Y offset control
// ------------------------------------------------------------------------------ //

#define HI_ISP_DIS_MANUAL_OFFSET_Y_DEFAULT (0x80)
#define HI_ISP_DIS_MANUAL_OFFSET_Y_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_dis_manual_offset_y_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x07EC, data);
}
static __inline HI_U8 hi_isp_dis_manual_offset_y_read(HI_VOID) {
    return IORD_8DIRECT(0x07EC);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_dis_offset_x
// ------------------------------------------------------------------------------ //
// Calculated X offset
// ------------------------------------------------------------------------------ //

#define HI_ISP_DIS_OFFSET_X_DEFAULT (0x0)
#define HI_ISP_DIS_OFFSET_X_DATASIZE (8)

// args: data (8-bit)
static __inline HI_U8 hi_isp_dis_offset_x_read(HI_VOID) {
    return IORD_8DIRECT(0x07F0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_dis_offset_y
// ------------------------------------------------------------------------------ //
// Calculated Y offset
// ------------------------------------------------------------------------------ //

#define HI_ISP_DIS_OFFSET_Y_DEFAULT (0x0)
#define HI_ISP_DIS_OFFSET_Y_DATASIZE (8)

// args: data (8-bit)
static __inline HI_U8 hi_isp_dis_offset_y_read(HI_VOID) {
    return IORD_8DIRECT(0x07F4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_dis_corr_run_times
// ------------------------------------------------------------------------------ //
// Multiplier for correlation block size to get correlation range in +-8 increments
// ------------------------------------------------------------------------------ //

#define HI_ISP_DIS_CORR_RUN_TIMES_DEFAULT (0x04)
#define HI_ISP_DIS_CORR_RUN_TIMES_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_isp_dis_corr_run_times_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x07E2, data);
}
static __inline HI_U8 hi_isp_dis_corr_run_times_read(HI_VOID) {
    return IORD_8DIRECT(0x07E2);
}

// ------------------------------------------------------------------------------ //
// Group: Frame Stats
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_isp_frame_stats_stats_reset
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STATS_STATS_RESET_DEFAULT (0)
#define HI_ISP_FRAME_STATS_STATS_RESET_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_frame_stats_stats_reset_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x0780, (data & 0x01));
}
static __inline HI_U8 hi_isp_frame_stats_stats_reset_read(HI_VOID) {
    return (IORD_8DIRECT(0x0780) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_frame_stats_stats_hold
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STATS_STATS_HOLD_DEFAULT (0)
#define HI_ISP_FRAME_STATS_STATS_HOLD_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_isp_frame_stats_stats_hold_write(HI_U8 data) {
    IOWR_8DIRECT_APICAL(0x0784, (data & 0x01));
}
static __inline HI_U8 hi_isp_frame_stats_stats_hold_read(HI_VOID) {
    return (IORD_8DIRECT(0x0784) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_frame_stats_active_width_min
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STATS_ACTIVE_WIDTH_MIN_DEFAULT (0x0)
#define HI_ISP_FRAME_STATS_ACTIVE_WIDTH_MIN_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_isp_frame_stats_active_width_min_read(HI_VOID) {
    return IORD_32DIRECT(0x0790);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_frame_stats_active_width_max
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STATS_ACTIVE_WIDTH_MAX_DEFAULT (0x0)
#define HI_ISP_FRAME_STATS_ACTIVE_WIDTH_MAX_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_isp_frame_stats_active_width_max_read(HI_VOID) {
    return IORD_32DIRECT(0x0794);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_frame_stats_active_width_sum
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STATS_ACTIVE_WIDTH_SUM_DEFAULT (0x0)
#define HI_ISP_FRAME_STATS_ACTIVE_WIDTH_SUM_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_isp_frame_stats_active_width_sum_read(HI_VOID) {
    return IORD_32DIRECT(0x0798);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_frame_stats_active_width_num
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STATS_ACTIVE_WIDTH_NUM_DEFAULT (0x0)
#define HI_ISP_FRAME_STATS_ACTIVE_WIDTH_NUM_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_isp_frame_stats_active_width_num_read(HI_VOID) {
    return IORD_32DIRECT(0x079C);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_frame_stats_active_height_min
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STATS_ACTIVE_HEIGHT_MIN_DEFAULT (0x0)
#define HI_ISP_FRAME_STATS_ACTIVE_HEIGHT_MIN_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_isp_frame_stats_active_height_min_read(HI_VOID) {
    return IORD_32DIRECT(0x07A0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_frame_stats_active_height_max
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STATS_ACTIVE_HEIGHT_MAX_DEFAULT (0x0)
#define HI_ISP_FRAME_STATS_ACTIVE_HEIGHT_MAX_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_isp_frame_stats_active_height_max_read(HI_VOID) {
    return IORD_32DIRECT(0x07A4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_frame_stats_active_height_sum
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STATS_ACTIVE_HEIGHT_SUM_DEFAULT (0x0)
#define HI_ISP_FRAME_STATS_ACTIVE_HEIGHT_SUM_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_isp_frame_stats_active_height_sum_read(HI_VOID) {
    return IORD_32DIRECT(0x07A8);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_frame_stats_active_height_num
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STATS_ACTIVE_HEIGHT_NUM_DEFAULT (0x0)
#define HI_ISP_FRAME_STATS_ACTIVE_HEIGHT_NUM_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_isp_frame_stats_active_height_num_read(HI_VOID) {
    return IORD_32DIRECT(0x07AC);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_frame_stats_hblank_min
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STATS_HBLANK_MIN_DEFAULT (0x0)
#define HI_ISP_FRAME_STATS_HBLANK_MIN_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_isp_frame_stats_hblank_min_read(HI_VOID) {
    return IORD_32DIRECT(0x07B0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_frame_stats_hblank_max
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STATS_HBLANK_MAX_DEFAULT (0x0)
#define HI_ISP_FRAME_STATS_HBLANK_MAX_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_isp_frame_stats_hblank_max_read(HI_VOID) {
    return IORD_32DIRECT(0x07B4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_frame_stats_hblank_sum
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STATS_HBLANK_SUM_DEFAULT (0x0)
#define HI_ISP_FRAME_STATS_HBLANK_SUM_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_isp_frame_stats_hblank_sum_read(HI_VOID) {
    return IORD_32DIRECT(0x07B8);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_frame_stats_hblank_num
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STATS_HBLANK_NUM_DEFAULT (0x0)
#define HI_ISP_FRAME_STATS_HBLANK_NUM_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_isp_frame_stats_hblank_num_read(HI_VOID) {
    return IORD_32DIRECT(0x07BC);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_frame_stats_vblank_min
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STATS_VBLANK_MIN_DEFAULT (0x0)
#define HI_ISP_FRAME_STATS_VBLANK_MIN_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_isp_frame_stats_vblank_min_read(HI_VOID) {
    return IORD_32DIRECT(0x07C0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_frame_stats_vblank_max
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STATS_VBLANK_MAX_DEFAULT (0x0)
#define HI_ISP_FRAME_STATS_VBLANK_MAX_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_isp_frame_stats_vblank_max_read(HI_VOID) {
    return IORD_32DIRECT(0x07C4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_frame_stats_vblank_sum
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STATS_VBLANK_SUM_DEFAULT (0x0)
#define HI_ISP_FRAME_STATS_VBLANK_SUM_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_isp_frame_stats_vblank_sum_read(HI_VOID) {
    return IORD_32DIRECT(0x07C8);
}

// ------------------------------------------------------------------------------ //
// Register: hi_isp_frame_stats_vblank_num
// ------------------------------------------------------------------------------ //

#define HI_ISP_FRAME_STATS_VBLANK_NUM_DEFAULT (0x0)
#define HI_ISP_FRAME_STATS_VBLANK_NUM_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_isp_frame_stats_vblank_num_read(HI_VOID) {
    return IORD_32DIRECT(0x07CC);
}

#if 1
// ------------------------------------------------------------------------------ //

/*--------------------------------------------------------------------------------------*/
/* write or read vi reg */

// ------------------------------------------------------------------------------ //
// Register: hi_vi_top_set_blc
// ------------------------------------------------------------------------------ //
// BLC enable/disable, r/gr/b/gb offset config
// ------------------------------------------------------------------------------ //
static __inline HI_VOID hi_vi_top_set_blc_offset(const ISP_BLC_ATTR_S *pstBLCAttr) {
    HI_U32 u32BLCOffset1 = (pstBLCAttr->u32ROffset << 16) | pstBLCAttr->u32GROffset;
    HI_U32 u32BLCOffset2 = (pstBLCAttr->u32BOffset << 16) | pstBLCAttr->u32GBOffset;
    ISP_BAYER_FORMAT_E enBayerFormat = hi_isp_top_rggb_start_read();

    IOWR_32DIRECT_VI(0x0550, pstBLCAttr->bEnable);
    IOWR_32DIRECT_VI(0x0554, u32BLCOffset1);
    IOWR_32DIRECT_VI(0x0558, u32BLCOffset2);
    IOWR_32DIRECT_VI(0x055C, enBayerFormat);
    /* reg new */
    IOWR_32DIRECT_VI(0x0504, 0x1);
}

static __inline HI_VOID hi_vi_top_get_blc_offset(ISP_BLC_ATTR_S *pstBLCAttr) {
    HI_U32 u32BLCOffset1 = IORD_32DIRECT_VI(0x0554);
    HI_U32 u32BLCOffset2 = IORD_32DIRECT_VI(0x0558);

    pstBLCAttr->bEnable = IORD_32DIRECT_VI(0x0550);

    pstBLCAttr->u32ROffset = u32BLCOffset1 >> 16;
    pstBLCAttr->u32GROffset = u32BLCOffset1 & 0xFFFF;
    pstBLCAttr->u32BOffset = u32BLCOffset2 >> 16;
    pstBLCAttr->u32GBOffset = u32BLCOffset2 & 0xFFFF;
}

HI_S32 ISP_SetCalibrateAttr(VI_DEV ViDev, ISP_FPN_CALIBRATE_ATTR_S *pstCalibrate);
HI_S32 ISP_SetCorrectionAttr(VI_DEV ViDev, const ISP_FPN_ATTR_S *pstCorrection);
HI_S32 ISP_GetCorrectionAttr(VI_DEV ViDev, ISP_FPN_ATTR_S *pstCorrection);
HI_S32 ISP_SetBLCAttr(ISP_DEV IspDev, const ISP_BLC_ATTR_S *pstBLCAttr);
HI_S32 ISP_GetBLCAttr(ISP_DEV IspDev, ISP_BLC_ATTR_S *pstBLCAttr);


// ------------------------------------------------------------------------------ //
// Register: hi_vi_top_chn_switch_enable
// ------------------------------------------------------------------------------ //
// 1: enable, 0: disable
// ------------------------------------------------------------------------------ //

#define HI_VI_TOP_CHN_SWITCH_ENABLE_DEFAULT (0)
#define HI_VI_TOP_CHN_SWITCH_ENABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_top_chn_switch_enble_write(HI_U32 data) {
    HI_U32 u32Current = IORD_32DIRECT_VI(0x10028);
    IOWR_32DIRECT_VI(0x10028, ((data & 0x1) << 0) | (u32Current & 0xFFFFFFFE));
}
static __inline HI_U32 hi_vi_top_chn_switch_enble_read(HI_VOID) {
    return ((IORD_32DIRECT_VI(0x10028) & 0x1) >> 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_top_chn_switch0
// ------------------------------------------------------------------------------ //
// select vi input for isp channel0 
// ------------------------------------------------------------------------------ //

#define HI_VI_TOP_CHN_SWITCH0_DEFAULT (0)
#define HI_VI_TOP_CHN_SWITCH0_DATASIZE (2)

// args: data (2-bit)
static __inline HI_VOID hi_vi_top_chn_switch0_write(HI_U32 data) {
    HI_U32 u32Current = IORD_32DIRECT_VI(0x10028);
    IOWR_32DIRECT_VI(0x10028, ((data & 0x3) << 4) | (u32Current & 0xFFFFFFCF));
}
static __inline HI_U32 hi_vi_top_chn_switch0_read(HI_VOID) {
    return ((IORD_32DIRECT_VI(0x10028) & 0x30) >> 4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_top_chn_switch1
// ------------------------------------------------------------------------------ //
// select vi input for isp channel1 
// ------------------------------------------------------------------------------ //

#define HI_VI_TOP_CHN_SWITCH1_DEFAULT (1)
#define HI_VI_TOP_CHN_SWITCH1_DATASIZE (2)

// args: data (2-bit)
static __inline HI_VOID hi_vi_top_chn_switch1_write(HI_U32 data) {
    HI_U32 u32Current = IORD_32DIRECT_VI(0x10028);
    IOWR_32DIRECT_VI(0x10028, ((data & 0x3) << 8) | (u32Current & 0xFFFFFCFF));
}
static __inline HI_U32 hi_vi_top_chn_switch1_read(HI_VOID) {
    return ((IORD_32DIRECT_VI(0x10028) & 0x300) >> 8);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_top_chn_switch2
// ------------------------------------------------------------------------------ //
// select vi input for isp channel2 
// ------------------------------------------------------------------------------ //

#define HI_VI_TOP_CHN_SWITCH2_DEFAULT (2)
#define HI_VI_TOP_CHN_SWITCH2_DATASIZE (2)

// args: data (2-bit)
static __inline HI_VOID hi_vi_top_chn_switch2_write(HI_U32 data) {
    HI_U32 u32Current = IORD_32DIRECT_VI(0x10028);
    IOWR_32DIRECT_VI(0x10028, ((data & 0x3) << 12) | (u32Current & 0xFFFFCFFF));
}
static __inline HI_U32 hi_vi_top_chn_switch2_read(HI_VOID) {
    return ((IORD_32DIRECT_VI(0x10028) & 0x3000) >> 12);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_top_chn_switch3
// ------------------------------------------------------------------------------ //
// select vi input for isp channel3
// ------------------------------------------------------------------------------ //

#define HI_VI_TOP_CHN_SWITCH3_DEFAULT (3)
#define HI_VI_TOP_CHN_SWITCH3_DATASIZE (2)

// args: data (2-bit)
static __inline HI_VOID hi_vi_top_chn_switch3_write(HI_U32 data) {
    HI_U32 u32Current = IORD_32DIRECT_VI(0x10028);
    IOWR_32DIRECT_VI(0x10028, ((data & 0x3) << 16) | (u32Current & 0xFFFCFFFF));
}
static __inline HI_U32 hi_vi_top_chn_switch3_read(HI_VOID) {
    return ((IORD_32DIRECT_VI(0x10028) & 0x30000) >> 16);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_top_channel_switch
// ------------------------------------------------------------------------------ //
// Switch input channels 1 and 2
// ------------------------------------------------------------------------------ //

#define HI_VI_TOP_CHANNEL_SWITCH_DEFAULT (0)
#define HI_VI_TOP_CHANNEL_SWITCH_DATASIZE (2)

// args: data (2-bit)
static __inline HI_VOID hi_vi_top_channel_switch_write(HI_U32 data) {
    IOWR_32DIRECT_VI(0x10028, (data & 0x03));
}
static __inline HI_U32 hi_vi_top_channel_switch_read(HI_VOID) {
    return (IORD_32DIRECT_VI(0x10028) & 0x03);
}


// ------------------------------------------------------------------------------ //
// Register: hi_vi_top_vc_enable
// ------------------------------------------------------------------------------ //
// Enable VC function
// ------------------------------------------------------------------------------ //

#define HI_VI_TOP_VC_ENABLE_DEFAULT (0)
#define HI_VI_TOP_VC_ENABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_top_vc_enable_write(HI_U32 data) {
    HI_U32 u32Current = IORD_32DIRECT_VI(0x01A4);
    IOWR_32DIRECT_VI(0x01A4, ((data & 0x1) << 31) | (u32Current & 0x7FFFFFFF));
}
static __inline HI_U32 hi_vi_top_vc_enable_read(HI_VOID) {
    return ((IORD_32DIRECT_VI(0x01A4) & 0x80000000) >> 31);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_top_vc_mode
// ------------------------------------------------------------------------------ //
// 0: auto, 1: manual
// ------------------------------------------------------------------------------ //

#define HI_VI_TOP_VC_MODE_DEFAULT (0)
#define HI_VI_TOP_VC_MODE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_top_vc_mode_write(HI_U32 data) {
    HI_U32 u32Current = IORD_32DIRECT_VI(0x01A4);
    IOWR_32DIRECT_VI(0x01A4, ((data & 0x1) << 30) | (u32Current & 0xBFFFFFFF));
}
static __inline HI_U32 hi_vi_top_vc_mode_read(HI_VOID) {
    return ((IORD_32DIRECT_VI(0x01A4) & 0x40000000) >> 30);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_top_vc_reset
// ------------------------------------------------------------------------------ //
// reset vc
// ------------------------------------------------------------------------------ //

#define HI_VI_TOP_VC_RESET_DEFAULT (0)
#define HI_VI_TOP_VC_RESET_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_top_vc_reset_write(HI_U32 data) {
    HI_U32 u32Current = IORD_32DIRECT_VI(0x01A4);
    IOWR_32DIRECT_VI(0x01A4, ((data & 0x1) << 29) | (u32Current & 0xDFFFFFFF));
}
static __inline HI_U32 hi_vi_top_vc_reset_read(HI_VOID) {
    return ((IORD_32DIRECT_VI(0x01A4) & 0x20000000) >> 29);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_top_vc_num
// ------------------------------------------------------------------------------ //
// vc number, when mode = auto, RO
// ------------------------------------------------------------------------------ //

#define HI_VI_TOP_VC_NUM_DEFAULT (0)
#define HI_VI_TOP_VC_NUM_DATASIZE (2)

// args: data (2-bit)
static __inline HI_VOID hi_vi_top_vc_num_write(HI_U32 data) {
    HI_U32 u32Current = IORD_32DIRECT_VI(0x01A4);
    IOWR_32DIRECT_VI(0x01A4, ((data & 0x3) << 4) | (u32Current & 0xFFFFFFCF));
}
static __inline HI_U32 hi_vi_top_vc_num_read(HI_VOID) {
    return ((IORD_32DIRECT_VI(0x01A4) & 0x30) >> 4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_top_vc_max
// ------------------------------------------------------------------------------ //
// the max of vc number
// ------------------------------------------------------------------------------ //

#define HI_VI_TOP_VC_MAX_DEFAULT (0)
#define HI_VI_TOP_VC_MAX_DATASIZE (2)

// args: data (2-bit)
static __inline HI_VOID hi_vi_top_vc_max_write(HI_U32 data) {
    HI_U32 u32Current = IORD_32DIRECT_VI(0x01A4);
    IOWR_32DIRECT_VI(0x01A4, ((data & 0x3) << 2) | (u32Current & 0xFFFFFFF3));
}
static __inline HI_U32 hi_vi_top_vc_max_read(HI_VOID) {
    return ((IORD_32DIRECT_VI(0x01A4) & 0xC) >> 2);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_top_vc_init_num
// ------------------------------------------------------------------------------ //
// the init value of vc number
// ------------------------------------------------------------------------------ //

#define HI_VI_TOP_VC_INIT_NUM_DEFAULT (0)
#define HI_VI_TOP_VC_INIT_NUM_DATASIZE (2)

// args: data (2-bit)
static __inline HI_VOID hi_vi_top_vc_init_num_write(HI_U32 data) {
    HI_U32 u32Current = IORD_32DIRECT_VI(0x01A4);
    IOWR_32DIRECT_VI(0x01A4, ((data & 0x3) << 0) | (u32Current & 0xFFFFFFFC));
}
static __inline HI_U32 hi_vi_top_vc_init_num_read(HI_VOID) {
    return ((IORD_32DIRECT_VI(0x01A4) & 0x3) >> 0);
}


// ------------------------------------------------------------------------------ //
// Register: hi_vi_dehaze_enable
// ------------------------------------------------------------------------------ //
// Correction enable: 0=off 1=on
// ------------------------------------------------------------------------------ //

#define HI_VI_DEHAZE_ENABLE_DEFAULT (0)
#define HI_VI_DEHAZE_ENABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_dehaze_enable_write(HI_U32 data) {
    IOWR_32DIRECT_VI(0x19000, data);
}
static __inline HI_U32 hi_vi_dehaze_enable_read(HI_VOID) {
    return IORD_32DIRECT_VI(0x19000);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_dehaze_update
// ------------------------------------------------------------------------------ //
// dehaze update
// ------------------------------------------------------------------------------ //

#define HI_VI_DEHAZE_UPDATE_DEFAULT (0)
#define HI_VI_DEHAZE_UPDATE_DATASIZE (32)

// args: data (1-bit)
static __inline HI_VOID hi_vi_dehaze_update_write(HI_U32 data) {
    IOWR_32DIRECT_VI(0x19010, data);
}
static __inline HI_U32 hi_vi_dehaze_update_read(HI_VOID) {
    return IORD_32DIRECT_VI(0x19010);
}


// ------------------------------------------------------------------------------ //
// Register: hi_vi_dehaze_air
// ------------------------------------------------------------------------------ //
// dehaze air
// ------------------------------------------------------------------------------ //

#define HI_VI_DEHAZE_AIR_DEFAULT (0)
#define HI_VI_DEHAZE_AIR_DATASIZE (32)

// args: data (1-bit)
static __inline HI_VOID hi_vi_dehaze_air_write(HI_U32 data) {
    IOWR_32DIRECT_VI(0x19030, data);
}
static __inline HI_U32 hi_vi_dehaze_air_read(HI_VOID) {
    return IORD_32DIRECT_VI(0x19030);
}


// ------------------------------------------------------------------------------ //
// Register: hi_vi_dehaze_blk_size
// ------------------------------------------------------------------------------ //
// Block size for dehaze hist
// ------------------------------------------------------------------------------ //

#define HI_VI_DEHAZE_BLK_SIZE_DEFAULT (0)
#define HI_VI_DEHAZE_BLK_SIZE_DATASIZE (32)

// args: data (32-bit)
static __inline HI_VOID hi_vi_dehaze_blk_size_write(HI_U32 data) {   
    IOWR_32DIRECT_VI(0x19014, data);
}
static __inline HI_U32 hi_vi_dehaze_blk_size_read(HI_VOID) {
    return IORD_32DIRECT_VI(0x19014);
}


// ------------------------------------------------------------------------------ //
// Register: hi_vi_dehaze_blk_num
// ------------------------------------------------------------------------------ //
// Block number for dehaze hist
// ------------------------------------------------------------------------------ //

#define HI_VI_DEHAZE_BLK_NUM_DEFAULT (0)
#define HI_VI_DEHAZE_BLK_NUM_DATASIZE (32)

// args: data (32-bit)
static __inline HI_VOID hi_vi_dehaze_blk_num_write(HI_U32 data) {   
    IOWR_32DIRECT_VI(0x19018, data);
}
static __inline HI_U32 hi_vi_dehaze_blk_num_read(HI_VOID) {
    return IORD_32DIRECT_VI(0x19018);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_dehaze_x
// ------------------------------------------------------------------------------ //
//  dehaze x 
// ------------------------------------------------------------------------------ //
#define HI_VI_DEHAZE_X_DEFAULT (0)
#define HI_VI_DEHAZE_X_DATASIZE (32)

// args: data (32-bit)
static __inline HI_VOID hi_vi_dehaze_x_write(HI_U32 data) {   
    IOWR_32DIRECT_VI(0x19020, data);
}
static __inline HI_U32 hi_vi_dehaze_x_read(HI_VOID) {
    return IORD_32DIRECT_VI(0x19020);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_dehaze_y
// ------------------------------------------------------------------------------ //
//  dehaze y 
// ------------------------------------------------------------------------------ //
#define HI_VI_DEHAZE_Y_DEFAULT (0)
#define HI_VI_DEHAZE_Y_DATASIZE (32)

// args: data (32-bit)
static __inline HI_VOID hi_vi_dehaze_y_write(HI_U32 data) {   
    IOWR_32DIRECT_VI(0x19024, data);
}
static __inline HI_U32 hi_vi_dehaze_y_read(HI_VOID) {
    return IORD_32DIRECT_VI(0x19024);
}


// ------------------------------------------------------------------------------ //
// Register: hi_vi_dehaze_statmode
// ------------------------------------------------------------------------------ //
//  dehaze statmode
// ------------------------------------------------------------------------------ //
#define HI_VI_DEHAZE_STATMODE_DEFAULT (0)
#define HI_VI_DEHAZE_STATMODE_DATASIZE (32)

// args: data (32-bit)
static __inline HI_VOID hi_vi_dehaze_statmode_write(HI_U32 data) {   
    IOWR_32DIRECT_VI(0x19028, data);
}
static __inline HI_U32 hi_vi_dehaze_statmode_read(HI_VOID) {
    return IORD_32DIRECT_VI(0x19028);
}
// ------------------------------------------------------------------------------ //
// Register: hi_vi_dehaze_thld
// ------------------------------------------------------------------------------ //
//  dehaze threshold
// ------------------------------------------------------------------------------ //
#define HI_VI_DEHAZE_THLD_DEFAULT (0)
#define HI_VI_DEHAZE_THLD_DATASIZE (32)

// args: data (32-bit)
static __inline HI_VOID hi_vi_dehaze_thld_write(HI_U32 data) {   
    IOWR_32DIRECT_VI(0x19034, data);
}
static __inline HI_U32 hi_vi_dehaze_thld_read(HI_VOID) {
    return IORD_32DIRECT_VI(0x19034);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_dehaze_gstrength
// ------------------------------------------------------------------------------ //
//  dehaze goble strength
// ------------------------------------------------------------------------------ //
#define HI_VI_DEHAZE_GSTRTH_DEFAULT (0)
#define HI_VI_DEHAZE_GSTRTH_DATASIZE (32)

// args: data (32-bit)
static __inline HI_VOID hi_vi_dehaze_gstrth_write(HI_U32 data) {   
    IOWR_32DIRECT_VI(0x19038, data);
}
static __inline HI_U32 hi_vi_dehaze_gstrth_read(HI_VOID) {
    return IORD_32DIRECT_VI(0x19038);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_dehaze_blthld
// ------------------------------------------------------------------------------ //
//  dehaze bl threshold
// ------------------------------------------------------------------------------ //
#define HI_VI_DEHAZE_BLTHLD_DEFAULT (0)
#define HI_VI_DEHAZE_BLTHLD_DATASIZE (32)

// args: data (32-bit)
static __inline HI_VOID hi_vi_dehaze_blthld_write(HI_U32 data) {   
    IOWR_32DIRECT_VI(0x1903c, data);
}
static __inline HI_U32 hi_vi_dehaze_blthld_read(HI_VOID) {
    return IORD_32DIRECT_VI(0x1903c);
}

//CH_DEHAZE_NEG_MODE
// ------------------------------------------------------------------------------ //
// Register: hi_vi_dehaze_neg_mode
// ------------------------------------------------------------------------------ //
//  dehaze negative mode
// ------------------------------------------------------------------------------ //
#define HI_VI_DEHAZE_NEG_MODE_DEFAULT (0)
#define HI_VI_DEHAZE_NEG_MODE_DATASIZE (32)

// args: data (32-bit)
static __inline HI_VOID hi_vi_dehaze_neg_mode_write(HI_U32 data) {   
    IOWR_32DIRECT_VI(0x1902C, data);
}
static __inline HI_U32 hi_vi_dehaze_neg_mode_read(HI_VOID) {
    return IORD_32DIRECT_VI(0x1902C);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_dehaze_dc_size
// ------------------------------------------------------------------------------ //
//  dehaze dc size
// ------------------------------------------------------------------------------ //
#define HI_VI_DEHAZE_DC_SIZE_DEFAULT (0)
#define HI_VI_DEHAZE_DC_SIZE_DATASIZE (32)

// args: data (32-bit)
static __inline HI_VOID hi_vi_dehaze_dc_size_write(HI_U32 data) {   
    IOWR_32DIRECT_VI(0x1901c, data);
}
static __inline HI_U32 hi_vi_dehaze_dc_size_read(HI_VOID) {
    return IORD_32DIRECT_VI(0x1901c);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_dehaze_max_stat
// ------------------------------------------------------------------------------ //
//  dehaze max_stat
// ------------------------------------------------------------------------------ //
#define HI_VI_DEHAZE_MAX_STAT_DEFAULT (0)
#define HI_VI_DEHAZE_MAX_STAT_DATASIZE (32)

// args: data (32-bit)
static __inline HI_VOID hi_vi_dehaze_max_stat_write(HI_U32 index,HI_U32 data) {   
    IOWR_32DIRECT_VI(0x19400+(index<<2), data);
}
static __inline HI_U32 hi_vi_dehaze_max_stat_read(HI_U32 index) {
    return IORD_32DIRECT_VI(0x19400+(index<<2));
}


// ------------------------------------------------------------------------------ //
// Register: hi_vi_dehaze_min_stat
// ------------------------------------------------------------------------------ //
//  dehaze min_stat
// ------------------------------------------------------------------------------ //
#define HI_VI_DEHAZE_MIN_STAT_DEFAULT (0)
#define HI_VI_DEHAZE_MIN_STAT_DATASIZE (32)

// args: data (32-bit)
static __inline HI_VOID hi_vi_dehaze_min_stat_write(HI_U32 index,HI_U32 data) {   
    IOWR_32DIRECT_VI(0x19100+(index<<2), data);
}
static __inline HI_U32 hi_vi_dehaze_min_stat_read(HI_U32 index) {
    return IORD_32DIRECT_VI(0x19100+(index<<2));
}


// ------------------------------------------------------------------------------ //
// Register: hi_vi_dehaze_prev_dc
// ------------------------------------------------------------------------------ //
//  dehaze prev dc
// ------------------------------------------------------------------------------ //
#define HI_VI_DEHAZE_PREV_DC_DEFAULT (0)
#define HI_VI_DEHAZE_PREV_DC_DATASIZE (32)

// args: data (32-bit)
static __inline HI_VOID hi_vi_dehaze_prev_dc_write(HI_U32 index,HI_U32 data) {   
    IOWR_32DIRECT_VI(0x19200+(index<<2), data);
}
static __inline HI_U32 hi_vi_dehaze_prev_dc_read(HI_U32 index) {
    return IORD_32DIRECT_VI(0x19200+(index<<2));
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_dehaze_lut
// ------------------------------------------------------------------------------ //
//  dehaze lut
// ------------------------------------------------------------------------------ //
#define HI_VI_DEHAZE_LUT_DEFAULT (0)
#define HI_VI_DEHAZE_LUT_DATASIZE (32)

// args: data (32-bit)
static __inline HI_VOID hi_vi_dehaze_lut_write(HI_U32 index,HI_U32 data) {   
    IOWR_32DIRECT_VI(0x19800+(index<<2), data);
}
static __inline HI_U32 hi_vi_dehaze_lut_read(HI_U32 index) {
    return IORD_32DIRECT_VI(0x19800+(index<<2));
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_dehaze_size
// ------------------------------------------------------------------------------ //
//  dehaze size
// ------------------------------------------------------------------------------ //
#define HI_VI_DEHAZE_SIZE_DEFAULT (0)
#define HI_VI_DEHAZE_SIZE_DATASIZE (32)

// args: data (32-bit)
static __inline HI_VOID hi_vi_dehaze_size_write(HI_U32 data) {   
    IOWR_32DIRECT_VI(0x190F0, data);
}
static __inline HI_U32 hi_vi_dehaze_size_read(HI_VOID) {
    return IORD_32DIRECT_VI(0x190F0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_fpn_enalbe_read
// ------------------------------------------------------------------------------ //
//  enalbe,gain,offset
// ------------------------------------------------------------------------------ //


// args: data (32-bit)
static __inline HI_U32 hi_vi_fpn_en_read(HI_VOID)
{ 
    HI_U32 u32FpnEnable;
    // fpn en && correction mode
    u32FpnEnable =  IORD_32DIRECT_VI(0x10100);    
    return (u32FpnEnable&0x1)&&(!(u32FpnEnable&0x100));
}
// args: data (32-bit)
static __inline HI_U32 hi_vi_fpn_read_gain_offset(HI_U32 index)
{     
    return  IORD_32DIRECT_VI(0x10130+(index<<2)); 
}

static __inline HI_VOID hi_vi_fpn_write_gain_offset(HI_U32 index, HI_U32 data)
{     
   IOWR_32DIRECT_VI(0x10130+(index<<2), data);
}

/******************* ACM *******************************************/
static __inline HI_U32 hi_vi_acm_read_enable(HI_VOID)
{     
    HI_U32 u32Cfg;
    u32Cfg = IORD_32DIRECT_VI(0x1400); 
    return (u32Cfg >>31);
}

static __inline HI_VOID hi_vi_acm_write_enable(HI_BOOL en)
{     
    HI_U32 u32Cfg;
    u32Cfg = IORD_32DIRECT_VI(0x1400); 
    u32Cfg &= ~(1 << 31);
    u32Cfg |= (en << 31);
    IOWR_32DIRECT_VI(0x1400, u32Cfg);
}

static __inline HI_U32 hi_vi_acm_read_dbg_enable(HI_VOID)
{     
    HI_U32 u32Cfg;
    u32Cfg = IORD_32DIRECT_VI(0x1400); 
    return (u32Cfg >>30) & 1;
}

static __inline HI_VOID hi_vi_acm_write_dbg_enable(HI_BOOL en)
{     
    HI_U32 u32Cfg;
    u32Cfg = IORD_32DIRECT_VI(0x1400); 
    u32Cfg &= ~(1 << 30);
    u32Cfg |= (en << 30);
    IOWR_32DIRECT_VI(0x1400, u32Cfg);
}

static __inline HI_U32 hi_vi_acm_read_stretch(HI_VOID)
{     
    HI_U32 u32Cfg;
    u32Cfg = IORD_32DIRECT_VI(0x1400); 
    return (u32Cfg >>29) & 1;
}

static __inline HI_VOID hi_vi_acm_write_stretch(HI_BOOL en)
{     
    HI_U32 u32Cfg;
    u32Cfg = IORD_32DIRECT_VI(0x1400); 
    u32Cfg &= ~(1 << 29);
    u32Cfg |= (en << 29);
    IOWR_32DIRECT_VI(0x1400, u32Cfg);
}

static __inline HI_U32 hi_vi_acm_read_cliprange(HI_VOID)
{     
    HI_U32 u32Cfg;
    u32Cfg = IORD_32DIRECT_VI(0x1400); 
    return (u32Cfg >>28) & 1;
}

static __inline HI_VOID hi_vi_acm_write_cliprange(HI_BOOL en)
{     
    HI_U32 u32Cfg;
    u32Cfg = IORD_32DIRECT_VI(0x1400); 
    u32Cfg &= ~(1 << 28);
    u32Cfg |= (en << 28);
    IOWR_32DIRECT_VI(0x1400, u32Cfg);
}

static __inline HI_U32 hi_vi_acm_read_cliporwrap(HI_VOID)
{     
    HI_U32 u32Cfg;
    u32Cfg = IORD_32DIRECT_VI(0x1400); 
    return (u32Cfg >>27) & 1;
}

static __inline HI_VOID hi_vi_acm_write_cliporwrap(HI_BOOL en)
{     
    HI_U32 u32Cfg;
    u32Cfg = IORD_32DIRECT_VI(0x1400); 
    u32Cfg &= ~(1 << 27);
    u32Cfg |= (en << 27);
    IOWR_32DIRECT_VI(0x1400, u32Cfg);
}

static __inline HI_U32 hi_vi_acm_read_cbcrth(HI_VOID)
{     
    HI_U32 u32Cfg;
    u32Cfg = IORD_32DIRECT_VI(0x1400); 
    return (u32Cfg & 0xFF);
}

static __inline HI_VOID hi_vi_acm_write_cbcrth(HI_U8 u8Thr)
{     
    HI_U32 u32Cfg;
    u32Cfg = IORD_32DIRECT_VI(0x1400); 
    u32Cfg &= ~(0xFF);
    u32Cfg |= (u8Thr & 0xFF);
    IOWR_32DIRECT_VI(0x1400, u32Cfg);
}

static __inline HI_U32 hi_vi_acm_read_luma_gain(HI_VOID)
{     
    HI_U32 u32Cfg;
    u32Cfg = IORD_32DIRECT_VI(0x1404); 
    return ((u32Cfg >> 20 ) & 0x3FF );
}

static __inline HI_VOID hi_vi_acm_write_luma_gain(HI_U32 u32Gain)
{     
    HI_U32 u32Cfg;
    u32Cfg = IORD_32DIRECT_VI(0x1404); 
    u32Cfg &= ~(0x3FF<<20);
    u32Cfg |= ((u32Gain & 0x3FF) << 20);
    IOWR_32DIRECT_VI(0x1404, u32Cfg);
}

static __inline HI_U32 hi_vi_acm_read_hue_gain(HI_VOID)
{     
    HI_U32 u32Cfg;
    u32Cfg = IORD_32DIRECT_VI(0x1404); 
    return ((u32Cfg >> 10 ) & 0x3FF );
}

static __inline HI_VOID hi_vi_acm_write_hue_gain(HI_U32 u32Gain)
{     
    HI_U32 u32Cfg;
    u32Cfg = IORD_32DIRECT_VI(0x1404); 
    u32Cfg &= ~(0x3FF<<10);
    u32Cfg |= ((u32Gain & 0x3FF) << 10);
    IOWR_32DIRECT_VI(0x1404, u32Cfg);
}

static __inline HI_U32 hi_vi_acm_read_sat_gain(HI_VOID)
{     
    HI_U32 u32Cfg;
    u32Cfg = IORD_32DIRECT_VI(0x1404); 
    return (u32Cfg & 0x3FF );
}

static __inline HI_VOID hi_vi_acm_write_sat_gain(HI_U32 u32Gain)
{     
    HI_U32 u32Cfg;
    u32Cfg = IORD_32DIRECT_VI(0x1404); 
    u32Cfg &= ~(0x3FF);
    u32Cfg |= (u32Gain & 0x3FF) ;
    IOWR_32DIRECT_VI(0x1404, u32Cfg);
}

static __inline HI_U32 hi_vi_acm_read_coeff_addr(HI_VOID)
{     
    return IORD_32DIRECT_VI(0x1414); 
}

static __inline HI_VOID hi_vi_acm_write_coeff_addr(HI_U32 u32Addr)
{     
    IOWR_32DIRECT_VI(0x1414, u32Addr);
}

static __inline HI_U32 hi_vi_acm_read_para_up(HI_VOID)
{     
    return IORD_32DIRECT_VI(0x1420); 
}

static __inline HI_VOID hi_vi_acm_write_para_up(HI_BOOL bEn)
{     
    IOWR_32DIRECT_VI(0x1420, bEn);
}

static __inline HI_U32 hi_vi_acm_read_width(HI_VOID)
{    
    HI_U32 u32Cfg;
    u32Cfg = IORD_32DIRECT_VI(0x1410); 
    return (u32Cfg & 0x1FFF );
}

static __inline HI_VOID hi_vi_acm_write_width(HI_U32 w)
{     
    HI_U32 u32Cfg;
    u32Cfg = IORD_32DIRECT_VI(0x1410); 
    u32Cfg &= ~(0x1FFF);
    u32Cfg |= (w & 0x1FFF) ;
    IOWR_32DIRECT_VI(0x1410, u32Cfg);
}

static __inline HI_U32 hi_vi_acm_read_height(HI_VOID)
{     
    HI_U32 u32Cfg;
    u32Cfg = IORD_32DIRECT_VI(0x1410); 
    return ((u32Cfg>>16) & 0x1FFF );
}

static __inline HI_VOID hi_vi_acm_write_height(HI_U32 h)
{     
    HI_U32 u32Cfg;
    u32Cfg = IORD_32DIRECT_VI(0x1410); 
    u32Cfg &= ~(0x1FFFF<<16);
    u32Cfg |= (h & 0x1FFF) <<16;
    IOWR_32DIRECT_VI(0x1410, u32Cfg);
}
static __inline HI_VOID hi_vi_acm_write_size(HI_VOID)
{
    HI_U32 u32Cfg;
    u32Cfg = IORD_32DIRECT_VI(0x0234); 
    IOWR_32DIRECT_VI(0x1410, u32Cfg);
}

extern HI_S32 ISP_ACM_SetAttr(ISP_ACM_ATTR_S *pstAcmAttr);
extern HI_S32 ISP_ACM_GetAttr(ISP_ACM_ATTR_S *pstAcmAttr);
extern HI_S32 ISP_ACM_SetCoeff(ISP_ACM_LUT_S *pstAcmCoeff,ISP_ACM_MODE_E enMode);
extern HI_S32 ISP_ACM_GetCoeff(ISP_ACM_LUT_S *pstAcmCoeff,ISP_ACM_MODE_E enMode);

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_enable
// ------------------------------------------------------------------------------ //
//  af enable
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_ENABLE_DEFAULT (1)
#define HI_VI_AF_ENABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_enable_write(HI_U32 data) {   
    HI_U32 u32Cfg = IORD_32DIRECT_VI(0x2000);
    IOWR_32DIRECT_VI(0x2000, ((data & 0x1) | (u32Cfg & 0x03FE)));
}
static __inline HI_U32 hi_vi_af_enable_read(HI_VOID) {
    return (IORD_32DIRECT_VI(0x2000) & 0x1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_en0
// ------------------------------------------------------------------------------ //
//  iir0 enable0
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_EN0_DEFAULT (1)
#define HI_VI_AF_IIR0_EN0_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_iir0_en0_write(HI_U32 data) {   
    HI_U32 u32Cfg = IORD_32DIRECT_VI(0x2000);
    IOWR_32DIRECT_VI(0x2000, (((data & 0x1) << 1) | (u32Cfg & 0x03FD)));
}
static __inline HI_U32 hi_vi_af_iir0_en0_read(HI_VOID) {
    return ((IORD_32DIRECT_VI(0x2000) & 0x2) >> 1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_en1
// ------------------------------------------------------------------------------ //
//  iir0 enable1
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_EN1_DEFAULT (0)
#define HI_VI_AF_IIR0_EN1_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_iir0_en1_write(HI_U32 data) {   
    HI_U32 u32Cfg = IORD_32DIRECT_VI(0x2000);
    IOWR_32DIRECT_VI(0x2000, (((data & 0x1) << 2) | (u32Cfg & 0x03FB)));
}
static __inline HI_U32 hi_vi_af_iir0_en1_read(HI_VOID) {
    return ((IORD_32DIRECT_VI(0x2000) & 0x4) >> 2);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_en2
// ------------------------------------------------------------------------------ //
//  iir0 enable2
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_EN2_DEFAULT (1)
#define HI_VI_AF_IIR0_EN2_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_iir0_en2_write(HI_U32 data) {   
    HI_U32 u32Cfg = IORD_32DIRECT_VI(0x2000);
    IOWR_32DIRECT_VI(0x2000, (((data & 0x1) << 3) | (u32Cfg & 0x03F7)));
}
static __inline HI_U32 hi_vi_af_iir0_en2_read(HI_VOID) {
    return ((IORD_32DIRECT_VI(0x2000) & 0x8) >> 3);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_en0
// ------------------------------------------------------------------------------ //
//  iir1 enable0
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_EN0_DEFAULT (1)
#define HI_VI_AF_IIR1_EN0_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_iir1_en0_write(HI_U32 data) {   
    HI_U32 u32Cfg = IORD_32DIRECT_VI(0x2000);
    IOWR_32DIRECT_VI(0x2000, (((data & 0x1) << 4) | (u32Cfg & 0x03EF)));
}
static __inline HI_U32 hi_vi_af_iir1_en0_read(HI_VOID) {
    return ((IORD_32DIRECT_VI(0x2000) & 0x10) >> 4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_en1
// ------------------------------------------------------------------------------ //
//  iir1 enable1
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_EN1_DEFAULT (0)
#define HI_VI_AF_IIR1_EN1_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_iir1_en1_write(HI_U32 data) {   
    HI_U32 u32Cfg = IORD_32DIRECT_VI(0x2000);
    IOWR_32DIRECT_VI(0x2000, (((data & 0x1) << 5) | (u32Cfg & 0x03DF)));
}
static __inline HI_U32 hi_vi_af_iir1_en1_read(HI_VOID) {
    return ((IORD_32DIRECT_VI(0x2000) & 0x20) >> 5);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_en2
// ------------------------------------------------------------------------------ //
//  iir1 enable2
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_EN2_DEFAULT (1)
#define HI_VI_AF_IIR1_EN2_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_iir1_en2_write(HI_U32 data) {   
    HI_U32 u32Cfg = IORD_32DIRECT_VI(0x2000);
    IOWR_32DIRECT_VI(0x2000, (((data & 0x1) << 6) | (u32Cfg & 0x03BF)));
}
static __inline HI_U32 hi_vi_af_iir1_en2_read(HI_VOID) {
    return ((IORD_32DIRECT_VI(0x2000) & 0x40) >> 6);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fvmode_peak
// ------------------------------------------------------------------------------ //
//  af fvmode peak
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FVMODE_PEAK_DEFAULT (1)
#define HI_VI_AF_FVMODE_PEAK_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_fvmode_peak_write(HI_U32 data) {   
    HI_U32 u32Cfg = IORD_32DIRECT_VI(0x2000);
    IOWR_32DIRECT_VI(0x2000, (((data & 0x1) << 7) | (u32Cfg & 0x037F)));
}
static __inline HI_U32 hi_vi_af_fvmode_peak_read(HI_VOID) {
    return ((IORD_32DIRECT_VI(0x2000) & 0x80) >> 7);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fvmode_squ
// ------------------------------------------------------------------------------ //
//  af fvmode square
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FVMODE_SQU_DEFAULT (1)
#define HI_VI_AF_FVMODE_SQU_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_vi_af_fvmode_squ_write(HI_U32 data) {   
    HI_U32 u32Cfg = IORD_32DIRECT_VI(0x2000);
    IOWR_32DIRECT_VI(0x2000, (((data & 0x1) << 8) | (u32Cfg & 0x02FF)));
}
static __inline HI_U32 hi_vi_af_fvmode_squ_read(HI_VOID) {
    return ((IORD_32DIRECT_VI(0x2000) & 0x100) >> 8);
}


// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_Hwnd
// ------------------------------------------------------------------------------ //
//  af Horizontal window number
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_HWND_DEFAULT (0x11)
#define HI_VI_AF_HWND_DATASIZE (5)

// args: data (5-bit)
static __inline HI_VOID hi_vi_af_hwnd_write(HI_U32 data) {   
    HI_U32 u32Cfg = IORD_32DIRECT_VI(0x2010);
    IOWR_32DIRECT_VI(0x2010, ((data & 0x1F) | (u32Cfg & 0x1F00)));
}
static __inline HI_U32 hi_vi_af_hwnd_read(HI_VOID) {
    return (IORD_32DIRECT_VI(0x2010) & 0x1F);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_Vwnd
// ------------------------------------------------------------------------------ //
//  af Vertical window number
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_VWND_DEFAULT (0xF)
#define HI_VI_AF_VWND_DATASIZE (5)

// args: data (5-bit)
static __inline HI_VOID hi_vi_af_vwnd_write(HI_U32 data) {   
    HI_U32 u32Cfg = IORD_32DIRECT_VI(0x2010);
    IOWR_32DIRECT_VI(0x2010, (((data & 0x1F) << 8) | (u32Cfg & 0x1F)));
}
static __inline HI_U32 hi_vi_af_vwnd_read(HI_VOID) {
    return ((IORD_32DIRECT_VI(0x2010) & 0x1F00) >> 8);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_gain0
// ------------------------------------------------------------------------------ //
//  af iir0 gain0
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_GAIN0_DEFAULT (0xA0)
#define HI_VI_AF_IIR0_GAIN0_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_vi_af_iir0_gain0_write(HI_U32 data) {   
    HI_U32 u32Cfg = IORD_32DIRECT_VI(0x2020);
    IOWR_32DIRECT_VI(0x2020, ((data & 0xFF) | (u32Cfg & 0xFF0000)));
}
static __inline HI_U32 hi_vi_af_iir0_gain0_read(HI_VOID) {
    return (IORD_32DIRECT_VI(0x2020) & 0xFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_gain0
// ------------------------------------------------------------------------------ //
//  af iir1 gain0
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_GAIN0_DEFAULT (0xA1)
#define HI_VI_AF_IIR1_GAIN0_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_vi_af_iir1_gain0_write(HI_U32 data) {   
    HI_U32 u32Cfg = IORD_32DIRECT_VI(0x2020);
    IOWR_32DIRECT_VI(0x2020, (((data & 0xFF) << 16) | (u32Cfg & 0x0000FF)));
}
static __inline HI_U32 hi_vi_af_iir1_gain0_read(HI_VOID) {
    return ((IORD_32DIRECT_VI(0x2020) & 0xFF0000) >> 16);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_gain1
// ------------------------------------------------------------------------------ //
//  af iir0 gain1
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_GAIN1_DEFAULT (0x1BC)
#define HI_VI_AF_IIR0_GAIN1_DATASIZE (10)

// args: data (10-bit)
static __inline HI_VOID hi_vi_af_iir0_gain1_write(HI_U32 data) {   
    HI_U32 u32Cfg = IORD_32DIRECT_VI(0x2024);
    IOWR_32DIRECT_VI(0x2024, ((data & 0x3FF) | (u32Cfg & 0x3FF0000)));
}
static __inline HI_U32 hi_vi_af_iir0_gain1_read(HI_VOID) {
    HI_U32 u32Tmp;
    
    u32Tmp = IORD_32DIRECT_VI(0x2024) & 0x3FF;
    if (u32Tmp & 0x200)
    {
        u32Tmp |= 0xFFFFFC00;
    }
    else
    {
    }
    return u32Tmp;
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_gain1
// ------------------------------------------------------------------------------ //
//  af iir1 gain1
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_GAIN1_DEFAULT (0x244)
#define HI_VI_AF_IIR1_GAIN1_DATASIZE (10)

// args: data (10-bit)
static __inline HI_VOID hi_vi_af_iir1_gain1_write(HI_U32 data) {   
    HI_U32 u32Cfg = IORD_32DIRECT_VI(0x2024);
    IOWR_32DIRECT_VI(0x2024, (((data & 0x3FF) << 16) | (u32Cfg & 0x00003FF)));
}
static __inline HI_U32 hi_vi_af_iir1_gain1_read(HI_VOID) {
    HI_U32 u32Tmp;
    
    u32Tmp = (IORD_32DIRECT_VI(0x2024) & 0x3FF0000) >> 16;
    if (u32Tmp & 0x200)
    {
        u32Tmp |= 0xFFFFFC00;
    }
    else
    {
    }
    return u32Tmp;
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_gain2
// ------------------------------------------------------------------------------ //
//  af iir0 gain2
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_GAIN2_DEFAULT (0x328)
#define HI_VI_AF_IIR0_GAIN2_DATASIZE (10)

// args: data (10-bit)
static __inline HI_VOID hi_vi_af_iir0_gain2_write(HI_U32 data) {   
    HI_U32 u32Cfg = IORD_32DIRECT_VI(0x2028);
    IOWR_32DIRECT_VI(0x2028, ((data & 0x3FF) | (u32Cfg & 0x3FF0000)));
}
static __inline HI_U32 hi_vi_af_iir0_gain2_read(HI_VOID) {
    HI_U32 u32Tmp;
    
    u32Tmp = IORD_32DIRECT_VI(0x2028) & 0x3FF;
    if (u32Tmp & 0x200)
    {
        u32Tmp |= 0xFFFFFC00;
    }
    else
    {
    }
    return u32Tmp;
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_gain2
// ------------------------------------------------------------------------------ //
//  af iir1 gain2
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_GAIN2_DEFAULT (0x328)
#define HI_VI_AF_IIR1_GAIN2_DATASIZE (10)

// args: data (10-bit)
static __inline HI_VOID hi_vi_af_iir1_gain2_write(HI_U32 data) {   
    HI_U32 u32Cfg = IORD_32DIRECT_VI(0x2028);
    IOWR_32DIRECT_VI(0x2028, (((data & 0x3FF) << 16) | (u32Cfg & 0x00003FF)));
}
static __inline HI_U32 hi_vi_af_iir1_gain2_read(HI_VOID) {
    HI_U32 u32Tmp;
    
    u32Tmp = (IORD_32DIRECT_VI(0x2028) & 0x3FF0000) >> 16;
    if (u32Tmp & 0x200)
    {
        u32Tmp |= 0xFFFFFC00;
    }
    else
    {
    }
    return u32Tmp;
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_gain3
// ------------------------------------------------------------------------------ //
//  af iir0 gain3
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_GAIN3_DEFAULT (0)
#define HI_VI_AF_IIR0_GAIN3_DATASIZE (10)

// args: data (10-bit)
static __inline HI_VOID hi_vi_af_iir0_gain3_write(HI_U32 data) {   
    HI_U32 u32Cfg = IORD_32DIRECT_VI(0x202C);
    IOWR_32DIRECT_VI(0x202C, ((data & 0x3FF) | (u32Cfg & 0x3FF0000)));
}
static __inline HI_U32 hi_vi_af_iir0_gain3_read(HI_VOID) {
    HI_U32 u32Tmp;
    
    u32Tmp = IORD_32DIRECT_VI(0x202C) & 0x3FF;
    if (u32Tmp & 0x200)
    {
        u32Tmp |= 0xFFFFFC00;
    }
    else
    {
    }
    return u32Tmp;
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_gain3
// ------------------------------------------------------------------------------ //
//  af iir1 gain3
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_GAIN3_DEFAULT (0)
#define HI_VI_AF_IIR1_GAIN3_DATASIZE (10)

// args: data (10-bit)
static __inline HI_VOID hi_vi_af_iir1_gain3_write(HI_U32 data) {   
    HI_U32 u32Cfg = IORD_32DIRECT_VI(0x202C);
    IOWR_32DIRECT_VI(0x202C, (((data & 0x3FF) << 16) | (u32Cfg & 0x00003FF)));
}
static __inline HI_U32 hi_vi_af_iir1_gain3_read(HI_VOID) {
    HI_U32 u32Tmp;
    
    u32Tmp = (IORD_32DIRECT_VI(0x202C) & 0x3FF0000) >> 16;
    if (u32Tmp & 0x200)
    {
        u32Tmp |= 0xFFFFFC00;
    }
    else
    {
    }
    return u32Tmp;
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_gain4
// ------------------------------------------------------------------------------ //
//  af iir0 gain4
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_GAIN4_DEFAULT (0)
#define HI_VI_AF_IIR0_GAIN4_DATASIZE (10)

// args: data (10-bit)
static __inline HI_VOID hi_vi_af_iir0_gain4_write(HI_U32 data) {   
    HI_U32 u32Cfg = IORD_32DIRECT_VI(0x2030);
    IOWR_32DIRECT_VI(0x2030, ((data & 0x3FF) | (u32Cfg & 0x3FF0000)));
}
static __inline HI_U32 hi_vi_af_iir0_gain4_read(HI_VOID) {
    HI_U32 u32Tmp;
    
    u32Tmp = IORD_32DIRECT_VI(0x2030) & 0x3FF;
    if (u32Tmp & 0x200)
    {
        u32Tmp |= 0xFFFFFC00;
    }
    else
    {
    }
    return u32Tmp;
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_gain4
// ------------------------------------------------------------------------------ //
//  af iir1 gain4
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_GAIN4_DEFAULT (0)
#define HI_VI_AF_IIR1_GAIN4_DATASIZE (10)

// args: data (10-bit)
static __inline HI_VOID hi_vi_af_iir1_gain4_write(HI_U32 data) {   
    HI_U32 u32Cfg = IORD_32DIRECT_VI(0x2030);
    IOWR_32DIRECT_VI(0x2030, (((data & 0x3FF) << 16) | (u32Cfg & 0x00003FF)));
}
static __inline HI_U32 hi_vi_af_iir1_gain4_read(HI_VOID) {
    HI_U32 u32Tmp;
    
    u32Tmp = (IORD_32DIRECT_VI(0x2030) & 0x3FF0000) >> 16;
    if (u32Tmp & 0x200)
    {
        u32Tmp |= 0xFFFFFC00;
    }
    else
    {
    }
    return u32Tmp;
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_gain5
// ------------------------------------------------------------------------------ //
//  af iir0 gain5
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_GAIN5_DEFAULT (0x17C)
#define HI_VI_AF_IIR0_GAIN5_DATASIZE (10)

// args: data (10-bit)
static __inline HI_VOID hi_vi_af_iir0_gain5_write(HI_U32 data) {   
    HI_U32 u32Cfg = IORD_32DIRECT_VI(0x2034);
    IOWR_32DIRECT_VI(0x2034, ((data & 0x3FF) | (u32Cfg & 0x3FF0000)));
}
static __inline HI_U32 hi_vi_af_iir0_gain5_read(HI_VOID) {
    HI_U32 u32Tmp;
    
    u32Tmp = IORD_32DIRECT_VI(0x2034) & 0x3FF;
    if (u32Tmp & 0x200)
    {
        u32Tmp |= 0xFFFFFC00;
    }
    else
    {
    }
    return u32Tmp;
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_gain5
// ------------------------------------------------------------------------------ //
//  af iir1 gain5
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_GAIN5_DEFAULT (0x284)
#define HI_VI_AF_IIR1_GAIN5_DATASIZE (10)

// args: data (10-bit)
static __inline HI_VOID hi_vi_af_iir1_gain5_write(HI_U32 data) {   
    HI_U32 u32Cfg = IORD_32DIRECT_VI(0x2034);
    IOWR_32DIRECT_VI(0x2034, (((data & 0x3FF) << 16) | (u32Cfg & 0x00003FF)));
}
static __inline HI_U32 hi_vi_af_iir1_gain5_read(HI_VOID) {
    HI_U32 u32Tmp;
    
    u32Tmp = (IORD_32DIRECT_VI(0x2034) & 0x3FF0000) >> 16;
    if (u32Tmp & 0x200)
    {
        u32Tmp |= 0xFFFFFC00;
    }
    else
    {
    }
    return u32Tmp;
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_gain6
// ------------------------------------------------------------------------------ //
//  af iir0 gain6
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_GAIN6_DEFAULT (0x33C)
#define HI_VI_AF_IIR0_GAIN6_DATASIZE (10)

// args: data (10-bit)
static __inline HI_VOID hi_vi_af_iir0_gain6_write(HI_U32 data) {   
    HI_U32 u32Cfg = IORD_32DIRECT_VI(0x2038);
    IOWR_32DIRECT_VI(0x2038, ((data & 0x3FF) | (u32Cfg & 0x3FF0000)));
}
static __inline HI_U32 hi_vi_af_iir0_gain6_read(HI_VOID) {
    HI_U32 u32Tmp;
    
    u32Tmp = IORD_32DIRECT_VI(0x2038) & 0x3FF;
    if (u32Tmp & 0x200)
    {
        u32Tmp |= 0xFFFFFC00;
    }
    else
    {
    }
    return u32Tmp;
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_gain6
// ------------------------------------------------------------------------------ //
//  af iir1 gain6
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_GAIN6_DEFAULT (0x33C)
#define HI_VI_AF_IIR1_GAIN6_DATASIZE (10)

// args: data (10-bit)
static __inline HI_VOID hi_vi_af_iir1_gain6_write(HI_U32 data) {   
    HI_U32 u32Cfg = IORD_32DIRECT_VI(0x2038);
    IOWR_32DIRECT_VI(0x2038, (((data & 0x3FF) << 16) | (u32Cfg & 0x00003FF)));
}
static __inline HI_U32 hi_vi_af_iir1_gain6_read(HI_VOID) {
    HI_U32 u32Tmp;
    
    u32Tmp = (IORD_32DIRECT_VI(0x2038) & 0x3FF0000) >> 16;
    if (u32Tmp & 0x200)
    {
        u32Tmp |= 0xFFFFFC00;
    }
    else
    {
    }
    return u32Tmp;
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_shift0
// ------------------------------------------------------------------------------ //
//  af iir0 shift0
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_SHIFT0_DEFAULT (0x7)
#define HI_VI_AF_IIR0_SHIFT0_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_iir0_shift0_write(HI_U32 data) {   
    HI_U32 u32Cfg = IORD_32DIRECT_VI(0x2040);
    IOWR_32DIRECT_VI(0x2040, ((data & 0xF) | (u32Cfg & 0xFFFFFFF0)));
}
static __inline HI_U32 hi_vi_af_iir0_shift0_read(HI_VOID) {
    return (IORD_32DIRECT_VI(0x2040) & 0xF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_shift1
// ------------------------------------------------------------------------------ //
//  af iir0 shift1
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_SHIFT1_DEFAULT (0x2)
#define HI_VI_AF_IIR0_SHIFT1_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_iir0_shift1_write(HI_U32 data) {   
    HI_U32 u32Cfg = IORD_32DIRECT_VI(0x2040);
    IOWR_32DIRECT_VI(0x2040, (((data & 0xF) << 4) | (u32Cfg & 0xFFFFFF0F)));
}
static __inline HI_U32 hi_vi_af_iir0_shift1_read(HI_VOID) {
    return ((IORD_32DIRECT_VI(0x2040) & 0xF0)>> 4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_shift2
// ------------------------------------------------------------------------------ //
//  af iir0 shift2
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_SHIFT2_DEFAULT (0x0)
#define HI_VI_AF_IIR0_SHIFT2_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_iir0_shift2_write(HI_U32 data) {   
    HI_U32 u32Cfg = IORD_32DIRECT_VI(0x2040);
    IOWR_32DIRECT_VI(0x2040, (((data & 0xF) << 8) | (u32Cfg & 0xFFFFF0FF)));
}
static __inline HI_U32 hi_vi_af_iir0_shift2_read(HI_VOID) {
    return ((IORD_32DIRECT_VI(0x2040) & 0xF00)>> 8);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_shift3
// ------------------------------------------------------------------------------ //
//  af iir0 shift3
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_SHIFT3_DEFAULT (0x1)
#define HI_VI_AF_IIR0_SHIFT3_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_iir0_shift3_write(HI_U32 data) {   
    HI_U32 u32Cfg = IORD_32DIRECT_VI(0x2040);
    IOWR_32DIRECT_VI(0x2040, (((data & 0xF) << 12) | (u32Cfg & 0xFFFF0FFF)));
}
static __inline HI_U32 hi_vi_af_iir0_shift3_read(HI_VOID) {
    return ((IORD_32DIRECT_VI(0x2040) & 0xF000)>> 12);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_shift0
// ------------------------------------------------------------------------------ //
//  af iir1 shift0
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_SHIFT0_DEFAULT (0x7)
#define HI_VI_AF_IIR0_SHIFT0_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_iir1_shift0_write(HI_U32 data) {   
    HI_U32 u32Cfg = IORD_32DIRECT_VI(0x2040);
    IOWR_32DIRECT_VI(0x2040, (((data & 0xF) << 16) | (u32Cfg & 0xFFF0FFFF)));
}
static __inline HI_U32 hi_vi_af_iir1_shift0_read(HI_VOID) {
    return ((IORD_32DIRECT_VI(0x2040) & 0xF0000) >> 16);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_shift1
// ------------------------------------------------------------------------------ //
//  af iir1 shift1
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_SHIFT1_DEFAULT (0x2)
#define HI_VI_AF_IIR1_SHIFT1_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_iir1_shift1_write(HI_U32 data) {   
    HI_U32 u32Cfg = IORD_32DIRECT_VI(0x2040);
    IOWR_32DIRECT_VI(0x2040, (((data & 0xF) << 20) | (u32Cfg & 0xFF0FFFFF)));
}
static __inline HI_U32 hi_vi_af_iir1_shift1_read(HI_VOID) {
    return ((IORD_32DIRECT_VI(0x2040) & 0xF00000)>> 20);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_shift2
// ------------------------------------------------------------------------------ //
//  af iir1 shift2
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_SHIFT2_DEFAULT (0x0)
#define HI_VI_AF_IIR1_SHIFT2_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_iir1_shift2_write(HI_U32 data) {   
    HI_U32 u32Cfg = IORD_32DIRECT_VI(0x2040);
    IOWR_32DIRECT_VI(0x2040, (((data & 0xF) << 24) | (u32Cfg & 0xF0FFFFFF)));
}
static __inline HI_U32 hi_vi_af_iir1_shift2_read(HI_VOID) {
    return ((IORD_32DIRECT_VI(0x2040) & 0xF000000)>> 24);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_shift3
// ------------------------------------------------------------------------------ //
//  af iir1 shift3
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_SHIFT3_DEFAULT (0)
#define HI_VI_AF_IIR1_SHIFT3_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_iir1_shift3_write(HI_U32 data) {   
    HI_U32 u32Cfg = IORD_32DIRECT_VI(0x2040);
    IOWR_32DIRECT_VI(0x2040, (((data & 0xF) << 28) | (u32Cfg & 0x0FFFFFFF)));
}
static __inline HI_U32 hi_vi_af_iir1_shift3_read(HI_VOID) {
    return ((IORD_32DIRECT_VI(0x2040) & 0xF0000000)>> 28);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir0_h0
// ------------------------------------------------------------------------------ //
//  af fir0 h0
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR0_H0_DEFAULT (0x30)
#define HI_VI_AF_FIR0_H0_DATASIZE (6)

// args: data (6-bit)
static __inline HI_VOID hi_vi_af_fir0_h0_write(HI_U32 data) {   
    HI_U32 u32Cfg = IORD_32DIRECT_VI(0x2050);
    IOWR_32DIRECT_VI(0x2050, ((data & 0x3F) | (u32Cfg & 0x3F0000)));
}
static __inline HI_U32 hi_vi_af_fir0_h0_read(HI_VOID) {
    HI_U32 u32Tmp;
    
    u32Tmp = IORD_32DIRECT_VI(0x2050) & 0x3F;
    if (u32Tmp & 0x20)
    {
        u32Tmp |= 0xFFFFFFC0;
    }
    else
    {
    }
    return u32Tmp;
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir1_h0
// ------------------------------------------------------------------------------ //
//  af fir1 h0
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR1_H0_DEFAULT (0x30)
#define HI_VI_AF_FIR1_H0_DATASIZE (6)

// args: data (6-bit)
static __inline HI_VOID hi_vi_af_fir1_h0_write(HI_U32 data) {   
    HI_U32 u32Cfg = IORD_32DIRECT_VI(0x2050);
    IOWR_32DIRECT_VI(0x2050, (((data & 0x3F) << 16) | (u32Cfg & 0x3F)));
}
static __inline HI_U32 hi_vi_af_fir1_h0_read(HI_VOID) {
    HI_U32 u32Tmp;
    
    u32Tmp = (IORD_32DIRECT_VI(0x2050) & 0x3F0000) >> 16;
    if (u32Tmp & 0x20)
    {
        u32Tmp |= 0xFFFFFFC0;
    }
    else
    {
    }
    return u32Tmp;
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir0_h1
// ------------------------------------------------------------------------------ //
//  af fir0 h1
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR0_H1_DEFAULT (0x2B)
#define HI_VI_AF_FIR0_H1_DATASIZE (6)

// args: data (6-bit)
static __inline HI_VOID hi_vi_af_fir0_h1_write(HI_U32 data) {   
    HI_U32 u32Cfg = IORD_32DIRECT_VI(0x2054);
    IOWR_32DIRECT_VI(0x2054, ((data & 0x3F) | (u32Cfg & 0x3F0000)));
}
static __inline HI_U32 hi_vi_af_fir0_h1_read(HI_VOID) {
    HI_U32 u32Tmp;
    
    u32Tmp = IORD_32DIRECT_VI(0x2054) & 0x3F;
    if (u32Tmp & 0x20)
    {
        u32Tmp |= 0xFFFFFFC0;
    }
    else
    {
    }
    return u32Tmp;
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir1_h1
// ------------------------------------------------------------------------------ //
//  af fir1 h1
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR1_H1_DEFAULT (0x15)
#define HI_VI_AF_FIR1_H1_DATASIZE (6)

// args: data (6-bit)
static __inline HI_VOID hi_vi_af_fir1_h1_write(HI_U32 data) {   
    HI_U32 u32Cfg = IORD_32DIRECT_VI(0x2054);
    IOWR_32DIRECT_VI(0x2054, (((data & 0x3F) << 16) | (u32Cfg & 0x3F)));
}
static __inline HI_U32 hi_vi_af_fir1_h1_read(HI_VOID) {
    HI_U32 u32Tmp;
    
    u32Tmp = (IORD_32DIRECT_VI(0x2054) & 0x3F0000) >> 16;
    if (u32Tmp & 0x20)
    {
        u32Tmp |= 0xFFFFFFC0;
    }
    else
    {
    }
    return u32Tmp;
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir0_h2
// ------------------------------------------------------------------------------ //
//  af fir0 h2
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR0_H2_DEFAULT (0x0)
#define HI_VI_AF_FIR0_H2_DATASIZE (6)

// args: data (6-bit)
static __inline HI_VOID hi_vi_af_fir0_h2_write(HI_U32 data) {   
    HI_U32 u32Cfg = IORD_32DIRECT_VI(0x2058);
    IOWR_32DIRECT_VI(0x2058, ((data & 0x3F) | (u32Cfg & 0x3F0000)));
}
static __inline HI_U32 hi_vi_af_fir0_h2_read(HI_VOID) {
    HI_U32 u32Tmp;
    
    u32Tmp = IORD_32DIRECT_VI(0x2058) & 0x3F;
    if (u32Tmp & 0x20)
    {
        u32Tmp |= 0xFFFFFFC0;
    }
    else
    {
    }
    return u32Tmp;
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir1_h2
// ------------------------------------------------------------------------------ //
//  af fir1 h2
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR1_H2_DEFAULT (0)
#define HI_VI_AF_FIR1_H2_DATASIZE (6)

// args: data (6-bit)
static __inline HI_VOID hi_vi_af_fir1_h2_write(HI_U32 data) {   
    HI_U32 u32Cfg = IORD_32DIRECT_VI(0x2058);
    IOWR_32DIRECT_VI(0x2058, (((data & 0x3F) << 16) | (u32Cfg & 0x3F)));
}
static __inline HI_U32 hi_vi_af_fir1_h2_read(HI_VOID) {
    HI_U32 u32Tmp;
    
    u32Tmp = (IORD_32DIRECT_VI(0x2058) & 0x3F0000) >> 16;
    if (u32Tmp & 0x20)
    {
        u32Tmp |= 0xFFFFFFC0;
    }
    else
    {
    }
    return u32Tmp;
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir0_h3
// ------------------------------------------------------------------------------ //
//  af fir0 h3
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR0_H3_DEFAULT (0x15)
#define HI_VI_AF_FIR0_H3_DATASIZE (6)

// args: data (6-bit)
static __inline HI_VOID hi_vi_af_fir0_h3_write(HI_U32 data) {   
    HI_U32 u32Cfg = IORD_32DIRECT_VI(0x205C);
    IOWR_32DIRECT_VI(0x205C, ((data & 0x3F) | (u32Cfg & 0x3F0000)));
}
static __inline HI_U32 hi_vi_af_fir0_h3_read(HI_VOID) {
    HI_U32 u32Tmp;
    
    u32Tmp = IORD_32DIRECT_VI(0x205C) & 0x3F;
    if (u32Tmp & 0x20)
    {
        u32Tmp |= 0xFFFFFFC0;
    }
    else
    {
    }
    return u32Tmp;
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir1_h3
// ------------------------------------------------------------------------------ //
//  af fir1 h3
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR1_H3_DEFAULT (0x2B)
#define HI_VI_AF_FIR1_H3_DATASIZE (6)

// args: data (6-bit)
static __inline HI_VOID hi_vi_af_fir1_h3_write(HI_U32 data) {   
    HI_U32 u32Cfg = IORD_32DIRECT_VI(0x205C);
    IOWR_32DIRECT_VI(0x205C, (((data & 0x3F) << 16) | (u32Cfg & 0x3F)));
}
static __inline HI_U32 hi_vi_af_fir1_h3_read(HI_VOID) {
    HI_U32 u32Tmp;
    
    u32Tmp = (IORD_32DIRECT_VI(0x205C) & 0x3F0000) >> 16;
    if (u32Tmp & 0x20)
    {
        u32Tmp |= 0xFFFFFFC0;
    }
    else
    {
    }
    return u32Tmp;
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir0_h4
// ------------------------------------------------------------------------------ //
//  af fir0 h4
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR0_H4_DEFAULT (0x10)
#define HI_VI_AF_FIR0_H4_DATASIZE (6)

// args: data (6-bit)
static __inline HI_VOID hi_vi_af_fir0_h4_write(HI_U32 data) {   
    HI_U32 u32Cfg = IORD_32DIRECT_VI(0x2060);
    IOWR_32DIRECT_VI(0x2060, ((data & 0x3F) | (u32Cfg & 0x3F0000)));
}
static __inline HI_U32 hi_vi_af_fir0_h4_read(HI_VOID) {
    HI_U32 u32Tmp;
    
    u32Tmp = IORD_32DIRECT_VI(0x2060) & 0x3F;
    if (u32Tmp & 0x20)
    {
        u32Tmp |= 0xFFFFFFC0;
    }
    else
    {
    }
    return u32Tmp;
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir1_h4
// ------------------------------------------------------------------------------ //
//  af fir1 h4
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR1_H4_DEFAULT (0x10)
#define HI_VI_AF_FIR1_H4_DATASIZE (6)

// args: data (6-bit)
static __inline HI_VOID hi_vi_af_fir1_h4_write(HI_U32 data) {   
    HI_U32 u32Cfg = IORD_32DIRECT_VI(0X2060);
    IOWR_32DIRECT_VI(0x2060, (((data & 0x3F) << 16) | (u32Cfg & 0x3F)));
}
static __inline HI_U32 hi_vi_af_fir1_h4_read(HI_VOID) {
    HI_U32 u32Tmp;
    
    u32Tmp = (IORD_32DIRECT_VI(0x2060) & 0x3F0000) >> 16;
    if (u32Tmp & 0x20)
    {
        u32Tmp |= 0xFFFFFFC0;
    }
    else
    {
    }
    return u32Tmp;
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir0_thd_x
// ------------------------------------------------------------------------------ //
//  af iir0 threshhold x
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR0_THD_X_DEFAULT (0x64)
#define HI_VI_AF_IIR0_THD_X_DATASIZE (11)

// args: data (11-bit)
static __inline HI_VOID hi_vi_af_iir0_thd_x_write(HI_U32 data) {   
    HI_U32 u32Cfg = IORD_32DIRECT_VI(0X2070);
    IOWR_32DIRECT_VI(0x2070, ((data & 0x7FF) | (u32Cfg & 0x7FF0000)));
}
static __inline HI_U32 hi_vi_af_iir0_thd_x_read(HI_VOID) {
    return (IORD_32DIRECT_VI(0x2070) & 0x7FF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_iir1_thd_x
// ------------------------------------------------------------------------------ //
//  af iir1 threshhold x
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_IIR1_THD_X_DEFAULT (0x14)
#define HI_VI_AF_IIR1_THD_X_DATASIZE (11)

// args: data (11-bit)
static __inline HI_VOID hi_vi_af_iir1_thd_x_write(HI_U32 data) {   
    HI_U32 u32Cfg = IORD_32DIRECT_VI(0X2070);
    IOWR_32DIRECT_VI(0x2070, (((data & 0x7FF) << 16) | (u32Cfg & 0x7FF)));
}
static __inline HI_U32 hi_vi_af_iir1_thd_x_read(HI_VOID) {
    return ((IORD_32DIRECT_VI(0x2070) & 0x7FF0000) >> 16);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir0_thd_y
// ------------------------------------------------------------------------------ //
//  af fir0 threshhold y
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR0_THD_Y_DEFAULT (0x32)
#define HI_VI_AF_FIR0_THD_Y_DATASIZE (11)

// args: data (11-bit)
static __inline HI_VOID hi_vi_af_fir0_thd_y_write(HI_U32 data) {   
    HI_U32 u32Cfg = IORD_32DIRECT_VI(0X2074);
    IOWR_32DIRECT_VI(0x2074, ((data & 0x7FF) | (u32Cfg & 0x7FF0000)));
}
static __inline HI_U32 hi_vi_af_fir0_thd_y_read(HI_VOID) {
    return (IORD_32DIRECT_VI(0x2074) & 0x7FF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_fir1_thd_y
// ------------------------------------------------------------------------------ //
//  af fir1 threshhold y
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_FIR1_THD_Y_DEFAULT (0xA)
#define HI_VI_AF_FIR1_THD_Y_DATASIZE (11)

// args: data (11-bit)
static __inline HI_VOID hi_vi_af_fir1_thd_y_write(HI_U32 data) {   
    HI_U32 u32Cfg = IORD_32DIRECT_VI(0X2074);
    IOWR_32DIRECT_VI(0x2074, (((data & 0x7FF) << 16) | (u32Cfg & 0x7FF)));
}
static __inline HI_U32 hi_vi_af_fir1_thd_y_read(HI_VOID) {
    return ((IORD_32DIRECT_VI(0x2074) & 0x7FF0000) >> 16);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_acc_shift_h0
// ------------------------------------------------------------------------------ //
//  af iir0 acc shift h
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_ACC_SHIFT_H0_DEFAULT (0)
#define HI_VI_AF_ACC_SHIFT_H0_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_acc_shift_h0_write(HI_U32 data) {   
    HI_U32 u32Cfg = IORD_32DIRECT_VI(0X2078);
    IOWR_32DIRECT_VI(0x2078, ((data & 0xF) | (u32Cfg & 0xFFFF0)));
}
static __inline HI_U32 hi_vi_af_acc_shift_h0_read(HI_VOID) {
    return (IORD_32DIRECT_VI(0x2078) & 0xF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_acc_shift_h1
// ------------------------------------------------------------------------------ //
//  af iir1 acc shift h
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_ACC_SHIFT_H1_DEFAULT (0)
#define HI_VI_AF_ACC_SHIFT_H1_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_acc_shift_h1_write(HI_U32 data) {   
    HI_U32 u32Cfg = IORD_32DIRECT_VI(0X2078);
    IOWR_32DIRECT_VI(0x2078, (((data & 0xF) << 4) | (u32Cfg & 0xFFF0F)));
}
static __inline HI_U32 hi_vi_af_acc_shift_h1_read(HI_VOID) {
    return ((IORD_32DIRECT_VI(0x2078) & 0xF0) >> 4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_acc_shift_V0
// ------------------------------------------------------------------------------ //
//  af fir0 acc shift V
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_ACC_SHIFT_V0_DEFAULT (2)
#define HI_VI_AF_ACC_SHIFT_V0_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_acc_shift_v0_write(HI_U32 data) {   
    HI_U32 u32Cfg = IORD_32DIRECT_VI(0X2078);
    IOWR_32DIRECT_VI(0x2078, (((data & 0xF) << 8) | (u32Cfg & 0xFF0FF)));
}
static __inline HI_U32 hi_vi_af_acc_shift_v0_read(HI_VOID) {
    return ((IORD_32DIRECT_VI(0x2078) & 0xF00) >> 8);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_acc_shift_V1
// ------------------------------------------------------------------------------ //
//  af fir1 acc shift V
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_ACC_SHIFT_V1_DEFAULT (0)
#define HI_VI_AF_ACC_SHIFT_V1_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_acc_shift_v1_write(HI_U32 data) {   
    HI_U32 u32Cfg = IORD_32DIRECT_VI(0X2078);
    IOWR_32DIRECT_VI(0x2078, (((data & 0xF) << 12) | (u32Cfg & 0xF0FFF)));
}
static __inline HI_U32 hi_vi_af_acc_shift_v1_read(HI_VOID) {
    return ((IORD_32DIRECT_VI(0x2078) & 0xF000) >> 12);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_acc_shift_y
// ------------------------------------------------------------------------------ //
//  af acc shift y
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_ACC_SHIFT_Y_DEFAULT (2)
#define HI_VI_AF_ACC_SHIFT_Y_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_acc_shift_y_write(HI_U32 data) {   
    HI_U32 u32Cfg = IORD_32DIRECT_VI(0X2078);
    IOWR_32DIRECT_VI(0x2078, (((data & 0xF) << 16) | (u32Cfg & 0x0FFFF)));
}
static __inline HI_U32 hi_vi_af_acc_shift_y_read(HI_VOID) {
    return ((IORD_32DIRECT_VI(0x2078) & 0xF0000) >> 16);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_cnt_shift_h0
// ------------------------------------------------------------------------------ //
//  af iir0 cnt shift h
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_CNT_SHIFT_H0_DEFAULT (0)
#define HI_VI_AF_CNT_SHIFT_H0_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_cnt_shift_h0_write(HI_U32 data) {   
    HI_U32 u32Cfg = IORD_32DIRECT_VI(0X207C);
    IOWR_32DIRECT_VI(0x207C, ((data & 0xF) | (u32Cfg & 0xFFF0)));
}
static __inline HI_U32 hi_vi_af_cnt_shift_h0_read(HI_VOID) {
    return (IORD_32DIRECT_VI(0x207C) & 0xF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_cnt_shift_h1
// ------------------------------------------------------------------------------ //
//  af iir1 cnt shift h
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_CNT_SHIFT_H1_DEFAULT (0)
#define HI_VI_AF_CNT_SHIFT_H1_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_cnt_shift_h1_write(HI_U32 data) {   
    HI_U32 u32Cfg = IORD_32DIRECT_VI(0X207C);
    IOWR_32DIRECT_VI(0x207C, (((data & 0xF) << 4) | (u32Cfg & 0xFF0F)));
}
static __inline HI_U32 hi_vi_af_cnt_shift_h1_read(HI_VOID) {
    return ((IORD_32DIRECT_VI(0x207C) & 0xF0) >> 4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_cnt_shift_v0
// ------------------------------------------------------------------------------ //
//  af fir0 cnt shift v
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_CNT_SHIFT_V0_DEFAULT (2)
#define HI_VI_AF_CNT_SHIFT_V0_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_cnt_shift_v0_write(HI_U32 data) {   
    HI_U32 u32Cfg = IORD_32DIRECT_VI(0X207C);
    IOWR_32DIRECT_VI(0x207C, (((data & 0xF) << 8) | (u32Cfg & 0xF0FF)));
}
static __inline HI_U32 hi_vi_af_cnt_shift_v0_read(HI_VOID) {
    return ((IORD_32DIRECT_VI(0x207C) & 0xF00) >> 8);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_cnt_shift_v1
// ------------------------------------------------------------------------------ //
//  af fir1 cnt shift v
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_CNT_SHIFT_V1_DEFAULT (0)
#define HI_VI_AF_CNT_SHIFT_V1_DATASIZE (4)

// args: data (4-bit)
static __inline HI_VOID hi_vi_af_cnt_shift_v1_write(HI_U32 data) {   
    HI_U32 u32Cfg = IORD_32DIRECT_VI(0X207C);
    IOWR_32DIRECT_VI(0x207C, (((data & 0xF) << 12) | (u32Cfg & 0x0FFF)));
}
static __inline HI_U32 hi_vi_af_cnt_shift_v1_read(HI_VOID) {
    return ((IORD_32DIRECT_VI(0x207C) & 0xF000) >> 12);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_Hsize
// ------------------------------------------------------------------------------ //
//  af Hsize
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_HSIZE_DEFAULT (0x77E)
#define HI_VI_AF_HSIZE_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_vi_af_hsize_write(HI_U32 data) {   
    HI_U32 u32Cfg = IORD_32DIRECT_VI(0X20E0);
    IOWR_32DIRECT_VI(0x20E0, ((data & 0xFFF) | (u32Cfg & 0xFFF0000)));
}
static __inline HI_U32 hi_vi_af_hsize_read(HI_VOID) {
    return (IORD_32DIRECT_VI(0x20E0) & 0xFFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_vi_af_Vsize
// ------------------------------------------------------------------------------ //
//  af Vsize
// ------------------------------------------------------------------------------ //
#define HI_VI_AF_VSIZE_DEFAULT (0x437)
#define HI_VI_AF_VSIZE_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_vi_af_vsize_write(HI_U32 data) {   
    HI_U32 u32Cfg = IORD_32DIRECT_VI(0X20E0);
    IOWR_32DIRECT_VI(0x20E0, (((data & 0xFFF) << 16) | (u32Cfg & 0xFFF)));
}
static __inline HI_U32 hi_vi_af_vsize_read(HI_VOID) {
    return ((IORD_32DIRECT_VI(0x20E0) & 0xFFF0000) >> 16);
}



#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __ISP_CONFIG_H__ */

