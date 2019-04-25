/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_ext_config.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/01/16
  Description   : 
  History       :
  1.Date        : 2013/01/16
    Author      : n00168968
    Modification: Created file

******************************************************************************/

#ifndef __ISP_EXT_CONFIG_H__
#define __ISP_EXT_CONFIG_H__

#include "hi_vreg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

// ------------------------------------------------------------------------------ //
// Instance 'ext' of module 'ext_config'
// ------------------------------------------------------------------------------ //

#define HI_EXT_BASE_ADDR (0x10000)
#define HI_EXT_SIZE      (0x200)

// ------------------------------------------------------------------------------ //
// Group: Top
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_ext_top_mem_init
// ------------------------------------------------------------------------------ //
// HI_TRUE: The state of ISP memory(ext register) is initialized; 
// HI_FALSE: The state of ISP memory(ext register) is NOT initialized
// ------------------------------------------------------------------------------ //

#define HI_EXT_TOP_MEM_INIT_DEFAULT (0x0)
#define HI_EXT_TOP_MEM_INIT_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_top_mem_init_write(HI_U8 data) {
    IOWR_8DIRECT(0x10000, (data & 0x01));
}
static __inline HI_U8 hi_ext_top_mem_init_read(HI_VOID) {
    return (IORD_8DIRECT(0x10000) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_top_wdr_cfg
// ------------------------------------------------------------------------------ //
// HI_TRUE: The state of WDR config is initialized; 
// HI_FALSE: The state of WDR config is NOT initialized
// ------------------------------------------------------------------------------ //

#define HI_EXT_TOP_WDR_CFG_DEFAULT (0x0)
#define HI_EXT_TOP_WDR_CFG_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_top_wdr_cfg_write(HI_U8 data) {
    IOWR_8DIRECT(0x10001, (data & 0x01));
}
static __inline HI_U8 hi_ext_top_wdr_cfg_read(HI_VOID) {
    return (IORD_8DIRECT(0x10001) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_top_pub_attr_cfg
// ------------------------------------------------------------------------------ //
// HI_TRUE: The state of PubAttr config is initialized; 
// HI_FALSE: The state of PubAttr config is NOT initialized
// ------------------------------------------------------------------------------ //

#define HI_EXT_TOP_PUB_ATTR_CFG_DEFAULT (0x0)
#define HI_EXT_TOP_PUB_ATTR_CFG_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_top_pub_attr_cfg_write(HI_U8 data) {
    IOWR_8DIRECT(0x10002, (data & 0x01));
}
static __inline HI_U8 hi_ext_top_pub_attr_cfg_read(HI_VOID) {
    return (IORD_8DIRECT(0x10002) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_top_isp_init
// ------------------------------------------------------------------------------ //
// HI_TRUE: The state of ISP is initialized; 
// HI_FALSE: The state of ISP is NOT initialized
// ------------------------------------------------------------------------------ //

#define HI_EXT_TOP_ISP_INIT_DEFAULT (0x0)
#define HI_EXT_TOP_ISP_INIT_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_top_isp_init_write(HI_U8 data) {
    IOWR_8DIRECT(0x10003, (data & 0x01));
}
static __inline HI_U8 hi_ext_top_isp_init_read(HI_VOID) {
    return (IORD_8DIRECT(0x10003) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_top_isp_run
// ------------------------------------------------------------------------------ //
// HI_TRUE: The state of ISP is running; 
// HI_FALSE: The state of ISP is NOT running
// ------------------------------------------------------------------------------ //

#define HI_EXT_TOP_ISP_RUN_DEFAULT (0x0)
#define HI_EXT_TOP_ISP_RUN_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_top_isp_run_write(HI_U8 data) {
    IOWR_8DIRECT(0x10004, (data & 0x01));
}
static __inline HI_U8 hi_ext_top_isp_run_read(HI_VOID) {
    return (IORD_8DIRECT(0x10004) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_top_isp_run_enable
// ------------------------------------------------------------------------------ //
// HI_TRUE: ISP_run is enable; 
// HI_FALSE: ISP_run is disable
// ------------------------------------------------------------------------------ //

#define HI_EXT_TOP_ISP_RUN_ENABLE_DEFAULT (0x0)
#define HI_EXT_TOP_ISP_RUN_ENABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_top_isp_run_enable_write(HI_U8 data) {
    IOWR_8DIRECT(0x10005, (data & 0x01));
}
static __inline HI_U8 hi_ext_top_isp_run_enable_read(HI_VOID) {
    return (IORD_8DIRECT(0x10005) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_top_wdr_switch
// ------------------------------------------------------------------------------ //
// HI_TRUE: WDR switch is complete; 
// HI_FALSE: WDR switch is NOT complete
// ------------------------------------------------------------------------------ //

#define HI_EXT_TOP_WDR_SWITCH_DEFAULT (0x0)
#define HI_EXT_TOP_WDR_SWITCH_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_top_wdr_switch_write(HI_U8 data) {
    IOWR_8DIRECT(0x10006, (data & 0x01));
}
static __inline HI_U8 hi_ext_top_wdr_switch_read(HI_VOID) {
    return (IORD_8DIRECT(0x10006) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_top_res_switch
// ------------------------------------------------------------------------------ //
// HI_TRUE: Resolution switch is complete; 
// HI_FALSE: Resolution switch is NOT complete
// ------------------------------------------------------------------------------ //

#define HI_EXT_TOP_RES_SWITCH_DEFAULT (0x0)
#define HI_EXT_TOP_RES_SWITCH_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_top_res_switch_write(HI_U8 data) {
    IOWR_8DIRECT(0x10007, (data & 0x01));
}
static __inline HI_U8 hi_ext_top_res_switch_read(HI_VOID) {
    return (IORD_8DIRECT(0x10007) & 0x01);
}


// ------------------------------------------------------------------------------ //
// Group: Sync
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_offset_x
// ------------------------------------------------------------------------------ //
// Offset in pixels from v-sync reference to start of active video
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_OFFSET_X_DEFAULT (0x000)
#define HI_EXT_SYNC_OFFSET_X_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_ext_sync_offset_x_write(HI_U16 data) {
    IOWR_16DIRECT(0x10200, (data & 0x0FFF));
}
static __inline HI_U16 hi_ext_sync_offset_x_read(HI_VOID) {
    return (IORD_16DIRECT(0x10200) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_offset_y
// ------------------------------------------------------------------------------ //
// Offset in lines from v-sync reference to start of active video
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_OFFSET_Y_DEFAULT (0x000)
#define HI_EXT_SYNC_OFFSET_Y_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_ext_sync_offset_y_write(HI_U16 data) {
    IOWR_16DIRECT(0x10202, (data & 0x0FFF));
}
static __inline HI_U16 hi_ext_sync_offset_y_read(HI_VOID) {
    return (IORD_16DIRECT(0x10202) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_total_width
// ------------------------------------------------------------------------------ //
// Total frame width including horizontal blanking
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_TOTAL_WIDTH_DEFAULT (0x672)
#define HI_EXT_SYNC_TOTAL_WIDTH_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_ext_sync_total_width_write(HI_U16 data) {
    IOWR_16DIRECT(0x10204, data);
}
static __inline HI_U16 hi_ext_sync_total_width_read(HI_VOID) {
    return IORD_16DIRECT(0x10204);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_total_height
// ------------------------------------------------------------------------------ //
// Total frame height including vertical blanking
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_TOTAL_HEIGHT_DEFAULT (0x2EE)
#define HI_EXT_SYNC_TOTAL_HEIGHT_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_ext_sync_total_height_write(HI_U16 data) {
    IOWR_16DIRECT(0x10206, data);
}
static __inline HI_U16 hi_ext_sync_total_height_read(HI_VOID) {
    return IORD_16DIRECT(0x10206);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_active_width
// ------------------------------------------------------------------------------ //
// Active video width in pixels
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_ACTIVE_WIDTH_DEFAULT (0x500)
#define HI_EXT_SYNC_ACTIVE_WIDTH_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_ext_sync_active_width_write(HI_U16 data) {
    IOWR_16DIRECT(0x10208, data);
}
static __inline HI_U16 hi_ext_sync_active_width_read(HI_VOID) {
    return IORD_16DIRECT(0x10208);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_active_height
// ------------------------------------------------------------------------------ //
// Active video height in lines
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_ACTIVE_HEIGHT_DEFAULT (0x2D0)
#define HI_EXT_SYNC_ACTIVE_HEIGHT_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_ext_sync_active_height_write(HI_U16 data) {
    IOWR_16DIRECT(0x1020A, data);
}
static __inline HI_U16 hi_ext_sync_active_height_read(HI_VOID) {
    return IORD_16DIRECT(0x1020A);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_h_front_porch
// ------------------------------------------------------------------------------ //
// Horizontal sync front porch (for DVI out)
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_H_FRONT_PORCH_DEFAULT (0x048)
#define HI_EXT_SYNC_H_FRONT_PORCH_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_ext_sync_h_front_porch_write(HI_U16 data) {
    IOWR_16DIRECT(0x1020C, (data & 0x0FFF));
}
static __inline HI_U16 hi_ext_sync_h_front_porch_read(HI_VOID) {
    return (IORD_16DIRECT(0x1020C) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_v_front_porch
// ------------------------------------------------------------------------------ //
// Vertical sync front porch (for DVI out)
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_V_FRONT_PORCH_DEFAULT (0x003)
#define HI_EXT_SYNC_V_FRONT_PORCH_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_ext_sync_v_front_porch_write(HI_U16 data) {
    IOWR_16DIRECT(0x1020E, (data & 0x0FFF));
}
static __inline HI_U16 hi_ext_sync_v_front_porch_read(HI_VOID) {
    return (IORD_16DIRECT(0x1020E) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_h_sync_width
// ------------------------------------------------------------------------------ //
// Horizontal sync width (for DVI out)
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_H_SYNC_WIDTH_DEFAULT (0x50)
#define HI_EXT_SYNC_H_SYNC_WIDTH_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_ext_sync_h_sync_width_write(HI_U16 data) {
    IOWR_16DIRECT(0x10210, (data & 0x0FFF));
}
static __inline HI_U16 hi_ext_sync_h_sync_width_read(HI_VOID) {
    return (IORD_16DIRECT(0x10210) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_v_sync_width
// ------------------------------------------------------------------------------ //
// Vertical sync width (for DVI out)
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_V_SYNC_WIDTH_DEFAULT (0x5)
#define HI_EXT_SYNC_V_SYNC_WIDTH_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_ext_sync_v_sync_width_write(HI_U16 data) {
    IOWR_16DIRECT(0x10212, (data & 0x0FFF));
}
static __inline HI_U16 hi_ext_sync_v_sync_width_read(HI_VOID) {
    return (IORD_16DIRECT(0x10212) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_control
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_CONTROL_DEFAULT (0x8D)
#define HI_EXT_SYNC_CONTROL_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_sync_control_write(HI_U8 data) {
    IOWR_8DIRECT(0x10214, data);
}
static __inline HI_U8 hi_ext_sync_control_read(HI_VOID) {
    return IORD_8DIRECT(0x10214);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_h_sync_out_pol
// ------------------------------------------------------------------------------ //
// Horizontal sync out polarity (for DVI out)
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_H_SYNC_OUT_POL_DEFAULT (1)
#define HI_EXT_SYNC_H_SYNC_OUT_POL_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_sync_h_sync_out_pol_write(HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(0x10214);
    IOWR_8DIRECT(0x10214, ((data & 0x01) << 2) | (u32Current & 0xFB));
}
static __inline HI_U8 hi_ext_sync_h_sync_out_pol_read(HI_VOID) {
    return ((IORD_8DIRECT(0x10214) & 0x04) >> 2);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_v_sync_out_pol
// ------------------------------------------------------------------------------ //
// Vertical sync out polarity (for DVI out)
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_V_SYNC_OUT_POL_DEFAULT (1)
#define HI_EXT_SYNC_V_SYNC_OUT_POL_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_sync_v_sync_out_pol_write(HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(0x10214);
    IOWR_8DIRECT(0x10214, ((data & 0x01) << 3) | (u32Current & 0xF7));
}
static __inline HI_U8 hi_ext_sync_v_sync_out_pol_read(HI_VOID) {
    return ((IORD_8DIRECT(0x10214) & 0x08) >> 3);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_dvi_tx_clock_edge
// ------------------------------------------------------------------------------ //
// DVI output clock edge: 0=falling 1=rising
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_DVI_TX_CLOCK_EDGE_DEFAULT (0)
#define HI_EXT_SYNC_DVI_TX_CLOCK_EDGE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_sync_dvi_tx_clock_edge_write(HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(0x10220);
    IOWR_8DIRECT(0x10220, (data & 0x01) | (u32Current & 0xFE));
}
static __inline HI_U8 hi_ext_sync_dvi_tx_clock_edge_read(HI_VOID) {
    return (IORD_8DIRECT(0x10220) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_dvi_rx_clock_edge
// ------------------------------------------------------------------------------ //
// Video input clock phase: 0=0 1=90 2=180 3=270
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_DVI_RX_CLOCK_EDGE_DEFAULT (0)
#define HI_EXT_SYNC_DVI_RX_CLOCK_EDGE_DATASIZE (2)

// args: data (2-bit)
static __inline HI_VOID hi_ext_sync_dvi_rx_clock_edge_write(HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(0x10220);
    IOWR_8DIRECT(0x10220, ((data & 0x03) << 1) | (u32Current & 0xF9));
}
static __inline HI_U8 hi_ext_sync_dvi_rx_clock_edge_read(HI_VOID) {
    return ((IORD_8DIRECT(0x10220) & 0x06) >> 1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_h_sync_in_pol
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_H_SYNC_IN_POL_DEFAULT (0)
#define HI_EXT_SYNC_H_SYNC_IN_POL_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_sync_h_sync_in_pol_write(HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(0x10221);
    IOWR_8DIRECT(0x10221, (data & 0x01) | (u32Current & 0xFE));
}
static __inline HI_U8 hi_ext_sync_h_sync_in_pol_read(HI_VOID) {
    return (IORD_8DIRECT(0x10221) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_v_sync_in_pol
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_V_SYNC_IN_POL_DEFAULT (0)
#define HI_EXT_SYNC_V_SYNC_IN_POL_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_sync_v_sync_in_pol_write(HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(0x10221);
    IOWR_8DIRECT(0x10221, ((data & 0x01) << 1) | (u32Current & 0xFD));
}
static __inline HI_U8 hi_ext_sync_v_sync_in_pol_read(HI_VOID) {
    return ((IORD_8DIRECT(0x10221) & 0x02) >> 1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_field_mode
// ------------------------------------------------------------------------------ //
// 
//   0 = Same as external
//   1 = Adjust with active-line
//   2 = Adjust with V-sync
//   3 = Reserved
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_FIELD_MODE_DEFAULT (0)
#define HI_EXT_SYNC_FIELD_MODE_DATASIZE (2)

#define HI_EXT_SYNC_FIELD_MODE_SAME_AS_EXTERNAL (0)
#define HI_EXT_SYNC_FIELD_MODE_ADJUST_WITH_ACTIVE_LINE (1)
#define HI_EXT_SYNC_FIELD_MODE_ADJUST_WITH_V_SYNC (2)
#define HI_EXT_SYNC_FIELD_MODE_RESERVED (3)

// args: data (2-bit)
static __inline HI_VOID hi_ext_sync_field_mode_write(HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(0x10221);
    IOWR_8DIRECT(0x10221, ((data & 0x03) << 4) | (u32Current & 0xCF));
}
static __inline HI_U8 hi_ext_sync_field_mode_read(HI_VOID) {
    return ((IORD_8DIRECT(0x10221) & 0x30) >> 4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_auto_pos
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_AUTO_POS_DEFAULT (0)
#define HI_EXT_SYNC_AUTO_POS_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_sync_auto_pos_write(HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(0x10221);
    IOWR_8DIRECT(0x10221, ((data & 0x01) << 6) | (u32Current & 0xBF));
}
static __inline HI_U8 hi_ext_sync_auto_pos_read(HI_VOID) {
    return ((IORD_8DIRECT(0x10221) & 0x40) >> 6);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_auto_size
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_AUTO_SIZE_DEFAULT (0)
#define HI_EXT_SYNC_AUTO_SIZE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_sync_auto_size_write(HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(0x10221);
    IOWR_8DIRECT(0x10221, ((data & 0x01) << 7) | (u32Current & 0x7F));
}
static __inline HI_U8 hi_ext_sync_auto_size_read(HI_VOID) {
    return ((IORD_8DIRECT(0x10221) & 0x80) >> 7);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_max_line_length
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_MAX_LINE_LENGTH_DEFAULT (0xfff)
#define HI_EXT_SYNC_MAX_LINE_LENGTH_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_ext_sync_max_line_length_write(HI_U16 data) {
    IOWR_16DIRECT(0x10230, (data & 0x0FFF));
}
static __inline HI_U16 hi_ext_sync_max_line_length_read(HI_VOID) {
    return (IORD_16DIRECT(0x10230) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_max_line_length_dis_y
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_MAX_LINE_LENGTH_DIS_Y_DEFAULT (0xfff)
#define HI_EXT_SYNC_MAX_LINE_LENGTH_DIS_Y_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_ext_sync_max_line_length_dis_y_write(HI_U16 data) {
    IOWR_16DIRECT(0x1023C, (data & 0x0FFF));
}
static __inline HI_U16 hi_ext_sync_max_line_length_dis_y_read(HI_VOID) {
    return (IORD_16DIRECT(0x1023C) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_max_line_length_nr1
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_MAX_LINE_LENGTH_NR1_DEFAULT (1407)
#define HI_EXT_SYNC_MAX_LINE_LENGTH_NR1_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_ext_sync_max_line_length_nr1_write(HI_U16 data) {
    IOWR_16DIRECT(0x10250, (data & 0x0FFF));
}
static __inline HI_U16 hi_ext_sync_max_line_length_nr1_read(HI_VOID) {
    return (IORD_16DIRECT(0x10250) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_max_line_length_nr2
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_MAX_LINE_LENGTH_NR2_DEFAULT (703)
#define HI_EXT_SYNC_MAX_LINE_LENGTH_NR2_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_ext_sync_max_line_length_nr2_write(HI_U16 data) {
    IOWR_16DIRECT(0x10254, (data & 0x0FFF));
}
static __inline HI_U16 hi_ext_sync_max_line_length_nr2_read(HI_VOID) {
    return (IORD_16DIRECT(0x10254) & 0x0FFF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_input_field_toggle
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_INPUT_FIELD_TOGGLE_DEFAULT (0)
#define HI_EXT_SYNC_INPUT_FIELD_TOGGLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_sync_input_field_toggle_write(HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(0x10232);
    IOWR_8DIRECT(0x10232, (data & 0x01) | (u32Current & 0xFE));
}
static __inline HI_U8 hi_ext_sync_input_field_toggle_read(HI_VOID) {
    return (IORD_8DIRECT(0x10232) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_input_field_polarity
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_INPUT_FIELD_POLARITY_DEFAULT (0)
#define HI_EXT_SYNC_INPUT_FIELD_POLARITY_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_sync_input_field_polarity_write(HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(0x10232);
    IOWR_8DIRECT(0x10232, ((data & 0x01) << 1) | (u32Current & 0xFD));
}
static __inline HI_U8 hi_ext_sync_input_field_polarity_read(HI_VOID) {
    return ((IORD_8DIRECT(0x10232) & 0x02) >> 1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_output_field_manual
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_OUTPUT_FIELD_MANUAL_DEFAULT (0)
#define HI_EXT_SYNC_OUTPUT_FIELD_MANUAL_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_sync_output_field_manual_write(HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(0x10232);
    IOWR_8DIRECT(0x10232, ((data & 0x01) << 4) | (u32Current & 0xEF));
}
static __inline HI_U8 hi_ext_sync_output_field_manual_read(HI_VOID) {
    return ((IORD_8DIRECT(0x10232) & 0x10) >> 4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_output_field_value
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_OUTPUT_FIELD_VALUE_DEFAULT (0)
#define HI_EXT_SYNC_OUTPUT_FIELD_VALUE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_sync_output_field_value_write(HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(0x10232);
    IOWR_8DIRECT(0x10232, ((data & 0x01) << 5) | (u32Current & 0xDF));
}
static __inline HI_U8 hi_ext_sync_output_field_value_read(HI_VOID) {
    return ((IORD_8DIRECT(0x10232) & 0x20) >> 5);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_jumbo_frame_mode
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_JUMBO_FRAME_MODE_DEFAULT (0)
#define HI_EXT_SYNC_JUMBO_FRAME_MODE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_sync_jumbo_frame_mode_write(HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(0x10232);
    IOWR_8DIRECT(0x10232, ((data & 0x01) << 3) | (u32Current & 0xF7));
}
static __inline HI_U8 hi_ext_sync_jumbo_frame_mode_read(HI_VOID) {
    return ((IORD_8DIRECT(0x10232) & 0x08) >> 3);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_mask_enable
// ------------------------------------------------------------------------------ //
// Enable masking of frame edges based on the values of Mask x and Mask y
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_MASK_ENABLE_DEFAULT (0)
#define HI_EXT_SYNC_MASK_ENABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_sync_mask_enable_write(HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(0x10232);
    IOWR_8DIRECT(0x10232, ((data & 0x01) << 6) | (u32Current & 0xBF));
}
static __inline HI_U8 hi_ext_sync_mask_enable_read(HI_VOID) {
    return ((IORD_8DIRECT(0x10232) & 0x40) >> 6);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_mask_auto
// ------------------------------------------------------------------------------ //
// Calculate mask size based on offsets
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_MASK_AUTO_DEFAULT (0)
#define HI_EXT_SYNC_MASK_AUTO_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_sync_mask_auto_write(HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(0x10232);
    IOWR_8DIRECT(0x10232, ((data & 0x01) << 7) | (u32Current & 0x7F));
}
static __inline HI_U8 hi_ext_sync_mask_auto_read(HI_VOID) {
    return ((IORD_8DIRECT(0x10232) & 0x80) >> 7);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_dis_offset_x
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_DIS_OFFSET_X_DEFAULT (0)
#define HI_EXT_SYNC_DIS_OFFSET_X_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_sync_dis_offset_x_write(HI_U8 data) {
    IOWR_8DIRECT(0x10234, data);
}
static __inline HI_U8 hi_ext_sync_dis_offset_x_read(HI_VOID) {
    return IORD_8DIRECT(0x10234);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_dis_offset_y
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_DIS_OFFSET_Y_DEFAULT (0)
#define HI_EXT_SYNC_DIS_OFFSET_Y_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_sync_dis_offset_y_write(HI_U8 data) {
    IOWR_8DIRECT(0x10235, data);
}
static __inline HI_U8 hi_ext_sync_dis_offset_y_read(HI_VOID) {
    return IORD_8DIRECT(0x10235);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_scale_y
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_SCALE_Y_DEFAULT (0)
#define HI_EXT_SYNC_SCALE_Y_DATASIZE (16)

// args: data (16-bit)
// data format: sign/magnitude 2.14-bit fixed-point
static __inline HI_VOID hi_ext_sync_scale_y_write(HI_U16 data) {
    IOWR_16DIRECT(0x10236, data);
}
static __inline HI_U16 hi_ext_sync_scale_y_read(HI_VOID) {
    return IORD_16DIRECT(0x10236);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_skew_x
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_SKEW_X_DEFAULT (0)
#define HI_EXT_SYNC_SKEW_X_DATASIZE (16)

// args: data (16-bit)
// data format: sign/magnitude 2.14-bit fixed-point
static __inline HI_VOID hi_ext_sync_skew_x_write(HI_U16 data) {
    IOWR_16DIRECT(0x10238, data);
}
static __inline HI_U16 hi_ext_sync_skew_x_read(HI_VOID) {
    return IORD_16DIRECT(0x10238);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_mask_x
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_MASK_X_DEFAULT (0)
#define HI_EXT_SYNC_MASK_X_DATASIZE (16)

// args: data (16-bit)
// data format: unsigned 16-bit integer
static __inline HI_VOID hi_ext_sync_mask_x_write(HI_U16 data) {
    IOWR_16DIRECT(0x1023A, data);
}
static __inline HI_U16 hi_ext_sync_mask_x_read(HI_VOID) {
    return IORD_16DIRECT(0x1023A);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_mask_y
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_MASK_Y_DEFAULT (0)
#define HI_EXT_SYNC_MASK_Y_DATASIZE (16)

// args: data (16-bit)
// data format: unsigned 16-bit integer
static __inline HI_VOID hi_ext_sync_mask_y_write(HI_U16 data) {
    IOWR_16DIRECT(0x1023E, data);
}
static __inline HI_U16 hi_ext_sync_mask_y_read(HI_VOID) {
    return IORD_16DIRECT(0x1023E);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_tg_width
// ------------------------------------------------------------------------------ //
// sensor TG full line length
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_TG_WIDTH_DEFAULT (2401)
#define HI_EXT_SYNC_TG_WIDTH_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_ext_sync_tg_width_write(HI_U16 data) {
    IOWR_16DIRECT(0x10240, data);
}
static __inline HI_U16 hi_ext_sync_tg_width_read(HI_VOID) {
    return IORD_16DIRECT(0x10240);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_tg_height
// ------------------------------------------------------------------------------ //
// sensor TG full frame height
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_TG_HEIGHT_DEFAULT (1125)
#define HI_EXT_SYNC_TG_HEIGHT_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_ext_sync_tg_height_write(HI_U16 data) {
    IOWR_16DIRECT(0x10242, data);
}
static __inline HI_U16 hi_ext_sync_tg_height_read(HI_VOID) {
    return IORD_16DIRECT(0x10242);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_3dnr_active_width
// ------------------------------------------------------------------------------ //
// Active video width in pixels
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_3DNR_ACTIVE_WIDTH_DEFAULT (0x500)
#define HI_EXT_SYNC_3DNR_ACTIVE_WIDTH_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_ext_sync_3dnr_active_width_write(HI_U16 data) {
    IOWR_16DIRECT(0x10248, data);
}
static __inline HI_U16 hi_ext_sync_3dnr_active_width_read(HI_VOID) {
    return IORD_16DIRECT(0x10248);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sync_3dnr_active_height
// ------------------------------------------------------------------------------ //
// Active video height in lines
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYNC_3DNR_ACTIVE_HEIGHT_DEFAULT (0x2D0)
#define HI_EXT_SYNC_3DNR_ACTIVE_HEIGHT_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_ext_sync_3dnr_active_height_write(HI_U16 data) {
    IOWR_16DIRECT(0x1024A, data);
}
static __inline HI_U16 hi_ext_sync_3dnr_active_height_read(HI_VOID) {
    return IORD_16DIRECT(0x1024A);
}

// ------------------------------------------------------------------------------ //
// Group: Exposure Status
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_ext_exposure_status_analog_gain_status
// ------------------------------------------------------------------------------ //
// Analog gain status
// ------------------------------------------------------------------------------ //

#define HI_EXT_EXPOSURE_STATUS_ANALOG_GAIN_STATUS_DEFAULT (0x0)
#define HI_EXT_EXPOSURE_STATUS_ANALOG_GAIN_STATUS_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_ext_exposure_status_analog_gain_status_write(HI_U16 data) {
    IOWR_16DIRECT(0x102AA, data);
}
static __inline HI_U16 hi_ext_exposure_status_analog_gain_status_read(HI_VOID) {
    return IORD_16DIRECT(0x102AA);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_exposure_status_coarse_digital_gain_status
// ------------------------------------------------------------------------------ //
// Coarse digital gain status
// ------------------------------------------------------------------------------ //

#define HI_EXT_EXPOSURE_STATUS_COARSE_DIGITAL_GAIN_STATUS_DEFAULT (0x0)
#define HI_EXT_EXPOSURE_STATUS_COARSE_DIGITAL_GAIN_STATUS_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_exposure_status_coarse_digital_gain_status_write(HI_U8 data) {
    IOWR_8DIRECT(0x102A1, data);
}
static __inline HI_U8 hi_ext_exposure_status_coarse_digital_gain_status_read(HI_VOID) {
    return IORD_8DIRECT(0x102A1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_exposure_status_fine_digital_gain_status
// ------------------------------------------------------------------------------ //
// Fine digital gain status
// ------------------------------------------------------------------------------ //

#define HI_EXT_EXPOSURE_STATUS_FINE_DIGITAL_GAIN_STATUS_DEFAULT (0x0)
#define HI_EXT_EXPOSURE_STATUS_FINE_DIGITAL_GAIN_STATUS_DATASIZE (10)

// args: data (10-bit)
static __inline HI_VOID hi_ext_exposure_status_fine_digital_gain_status_write(HI_U16 data) {
    IOWR_16DIRECT(0x102A2, (data & 0x03FF));
}
static __inline HI_U16 hi_ext_exposure_status_fine_digital_gain_status_read(HI_VOID) {
    return (IORD_16DIRECT(0x102A2) & 0x03FF);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_exposure_status_long_integration_time_status
// ------------------------------------------------------------------------------ //
// Long integration time status
// ------------------------------------------------------------------------------ //

#define HI_EXT_EXPOSURE_STATUS_LONG_INTEGRATION_TIME_STATUS_DEFAULT (0x0)
#define HI_EXT_EXPOSURE_STATUS_LONG_INTEGRATION_TIME_STATUS_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_ext_exposure_status_long_integration_time_status_write(HI_U16 data) {
    IOWR_16DIRECT(0x102A4, data);
}
static __inline HI_U16 hi_ext_exposure_status_long_integration_time_status_read(HI_VOID) {
    return IORD_16DIRECT(0x102A4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_exposure_status_short_integration_time_status
// ------------------------------------------------------------------------------ //
// Short integration time status
// ------------------------------------------------------------------------------ //

#define HI_EXT_EXPOSURE_STATUS_SHORT_INTEGRATION_TIME_STATUS_DEFAULT (0x0)
#define HI_EXT_EXPOSURE_STATUS_SHORT_INTEGRATION_TIME_STATUS_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_ext_exposure_status_short_integration_time_status_write(HI_U16 data) {
    IOWR_16DIRECT(0x102A6, data);
}
static __inline HI_U16 hi_ext_exposure_status_short_integration_time_status_read(HI_VOID) {
    return IORD_16DIRECT(0x102A6);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_exposure_status_average_brightness_status
// ------------------------------------------------------------------------------ //
// Average brightness status
// ------------------------------------------------------------------------------ //

#define HI_EXT_EXPOSURE_STATUS_AVERAGE_BRIGHTNESS_STATUS_DEFAULT (0x0)
#define HI_EXT_EXPOSURE_STATUS_AVERAGE_BRIGHTNESS_STATUS_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_ext_exposure_status_average_brightness_status_write(HI_U16 data) {
    IOWR_16DIRECT(0x102A8, data);
}
static __inline HI_U16 hi_ext_exposure_status_average_brightness_status_read(HI_VOID) {
    return IORD_16DIRECT(0x102A8);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_exposure_status_illumination_status
// ------------------------------------------------------------------------------ //
// Sensor Illumination status
// ------------------------------------------------------------------------------ //

#define HI_EXT_EXPOSURE_STATUS_ILLUMINATION_STATUS_DEFAULT (0x0)
#define HI_EXT_EXPOSURE_STATUS_ILLUMINATION_STATUS_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_ext_exposure_status_illumination_status_write(HI_U16 data) {
    IOWR_16DIRECT(0x102AC, data);
}
static __inline HI_U16 hi_ext_exposure_status_illumination_status_read(HI_VOID) {
    return IORD_16DIRECT(0x102AC);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_exposure_status_exposure_ratio_status
// ------------------------------------------------------------------------------ //
// Sensor Illumination status
// ------------------------------------------------------------------------------ //

#define HI_EXT_EXPOSURE_STATUS_EXPOSURE_RATIO_STATUS_DEFAULT (0x0)
#define HI_EXT_EXPOSURE_STATUS_EXPOSURE_RATIO_STATUS_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_ext_exposure_status_exposure_ratio_status_write(HI_U16 data) {
    IOWR_16DIRECT(0x102AE, data);
}
static __inline HI_U16 hi_ext_exposure_status_exposure_ratio_status_read(HI_VOID) {
    return IORD_16DIRECT(0x102AE);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_exposure_status_anti_flicker_status
// ------------------------------------------------------------------------------ //

#define HI_EXT_EXPOSURE_STATUS_ANTI_FLICKER_STATUS_DEFAULT (0x0)
#define HI_EXT_EXPOSURE_STATUS_ANTI_FLICKER_STATUS_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_exposure_status_anti_flicker_status_write(HI_U8 data) {
    IOWR_8DIRECT(0x102A0, data);
}
static __inline HI_U8 hi_ext_exposure_status_anti_flicker_status_read(HI_VOID) {
    return IORD_8DIRECT(0x102A0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_exposure_status_short_overexposed
// ------------------------------------------------------------------------------ //
// Short overexposed
// ------------------------------------------------------------------------------ //

#define HI_EXT_EXPOSURE_STATUS_SHORT_OVEREXPOSED_DEFAULT (0)
#define HI_EXT_EXPOSURE_STATUS_SHORT_OVEREXPOSED_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_exposure_status_short_overexposed_write(HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(0x102A0);
    IOWR_8DIRECT(0x102A0, (data & 0x01) | (u32Current & 0xFE));
}
static __inline HI_U8 hi_ext_exposure_status_short_overexposed_read(HI_VOID) {
    return (IORD_8DIRECT(0x102A0) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_exposure_status_short_underexposed
// ------------------------------------------------------------------------------ //
// Short underexposed
// ------------------------------------------------------------------------------ //

#define HI_EXT_EXPOSURE_STATUS_SHORT_UNDEREXPOSED_DEFAULT (0)
#define HI_EXT_EXPOSURE_STATUS_SHORT_UNDEREXPOSED_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_exposure_status_short_underexposed_write(HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(0x102A0);
    IOWR_8DIRECT(0x102A0, ((data & 0x01) << 1) | (u32Current & 0xFD));
}
static __inline HI_U8 hi_ext_exposure_status_short_underexposed_read(HI_VOID) {
    return ((IORD_8DIRECT(0x102A0) & 0x02) >> 1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_exposure_status_long_overexposed
// ------------------------------------------------------------------------------ //
// Long overexposed
// ------------------------------------------------------------------------------ //

#define HI_EXT_EXPOSURE_STATUS_LONG_OVEREXPOSED_DEFAULT (0)
#define HI_EXT_EXPOSURE_STATUS_LONG_OVEREXPOSED_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_exposure_status_long_overexposed_write(HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(0x102A0);
    IOWR_8DIRECT(0x102A0, ((data & 0x01) << 2) | (u32Current & 0xFB));
}
static __inline HI_U8 hi_ext_exposure_status_long_overexposed_read(HI_VOID) {
    return ((IORD_8DIRECT(0x102A0) & 0x04) >> 2);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_exposure_status_long_underexposed
// ------------------------------------------------------------------------------ //
// Long underexposed
// ------------------------------------------------------------------------------ //

#define HI_EXT_EXPOSURE_STATUS_LONG_UNDEREXPOSED_DEFAULT (0)
#define HI_EXT_EXPOSURE_STATUS_LONG_UNDEREXPOSED_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_exposure_status_long_underexposed_write(HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(0x102A0);
    IOWR_8DIRECT(0x102A0, ((data & 0x01) << 3) | (u32Current & 0xF7));
}
static __inline HI_U8 hi_ext_exposure_status_long_underexposed_read(HI_VOID) {
    return ((IORD_8DIRECT(0x102A0) & 0x08) >> 3);
}

// ------------------------------------------------------------------------------ //
// Group: Sensor Access
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sensor_access_custom_sensor_addr0
// ------------------------------------------------------------------------------ //
// Sensor address register 0
// ------------------------------------------------------------------------------ //

#define HI_EXT_SENSOR_ACCESS_CUSTOM_SENSOR_ADDR0_DEFAULT (0xFFFF)
#define HI_EXT_SENSOR_ACCESS_CUSTOM_SENSOR_ADDR0_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_ext_sensor_access_custom_sensor_addr0_write(HI_U16 data) {
    IOWR_16DIRECT(0x102B0, data);
}
static __inline HI_U16 hi_ext_sensor_access_custom_sensor_addr0_read(HI_VOID) {
    return IORD_16DIRECT(0x102B0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sensor_access_custom_sensor_data0
// ------------------------------------------------------------------------------ //
// Sensor data register 0
// ------------------------------------------------------------------------------ //

#define HI_EXT_SENSOR_ACCESS_CUSTOM_SENSOR_DATA0_DEFAULT (0x0)
#define HI_EXT_SENSOR_ACCESS_CUSTOM_SENSOR_DATA0_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_ext_sensor_access_custom_sensor_data0_write(HI_U16 data) {
    IOWR_16DIRECT(0x102B2, data);
}
static __inline HI_U16 hi_ext_sensor_access_custom_sensor_data0_read(HI_VOID) {
    return IORD_16DIRECT(0x102B2);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sensor_access_custom_sensor_addr1
// ------------------------------------------------------------------------------ //
// Sensor address register 0
// ------------------------------------------------------------------------------ //

#define HI_EXT_SENSOR_ACCESS_CUSTOM_SENSOR_ADDR1_DEFAULT (0xFFFF)
#define HI_EXT_SENSOR_ACCESS_CUSTOM_SENSOR_ADDR1_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_ext_sensor_access_custom_sensor_addr1_write(HI_U16 data) {
    IOWR_16DIRECT(0x102B4, data);
}
static __inline HI_U16 hi_ext_sensor_access_custom_sensor_addr1_read(HI_VOID) {
    return IORD_16DIRECT(0x102B4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sensor_access_custom_sensor_data1
// ------------------------------------------------------------------------------ //
// Sensor data register 0
// ------------------------------------------------------------------------------ //

#define HI_EXT_SENSOR_ACCESS_CUSTOM_SENSOR_DATA1_DEFAULT (0x0)
#define HI_EXT_SENSOR_ACCESS_CUSTOM_SENSOR_DATA1_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_ext_sensor_access_custom_sensor_data1_write(HI_U16 data) {
    IOWR_16DIRECT(0x102B6, data);
}
static __inline HI_U16 hi_ext_sensor_access_custom_sensor_data1_read(HI_VOID) {
    return IORD_16DIRECT(0x102B6);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sensor_access_custom_sensor_addr2
// ------------------------------------------------------------------------------ //
// Sensor address register 0
// ------------------------------------------------------------------------------ //

#define HI_EXT_SENSOR_ACCESS_CUSTOM_SENSOR_ADDR2_DEFAULT (0xFFFF)
#define HI_EXT_SENSOR_ACCESS_CUSTOM_SENSOR_ADDR2_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_ext_sensor_access_custom_sensor_addr2_write(HI_U16 data) {
    IOWR_16DIRECT(0x102B8, data);
}
static __inline HI_U16 hi_ext_sensor_access_custom_sensor_addr2_read(HI_VOID) {
    return IORD_16DIRECT(0x102B8);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_sensor_access_custom_sensor_data2
// ------------------------------------------------------------------------------ //
// Sensor data register 0
// ------------------------------------------------------------------------------ //

#define HI_EXT_SENSOR_ACCESS_CUSTOM_SENSOR_DATA2_DEFAULT (0x0)
#define HI_EXT_SENSOR_ACCESS_CUSTOM_SENSOR_DATA2_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_ext_sensor_access_custom_sensor_data2_write(HI_U16 data) {
    IOWR_16DIRECT(0x102BA, data);
}
static __inline HI_U16 hi_ext_sensor_access_custom_sensor_data2_read(HI_VOID) {
    return IORD_16DIRECT(0x102BA);
}

// ------------------------------------------------------------------------------ //
// Group: General Purpose
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_ext_general_purpose_gpi
// ------------------------------------------------------------------------------ //
// Status of push buttons
// ------------------------------------------------------------------------------ //

#define HI_EXT_GENERAL_PURPOSE_GPI_DEFAULT (0x0)
#define HI_EXT_GENERAL_PURPOSE_GPI_DATASIZE (4)

// args: data (4-bit)
static __inline HI_U8 hi_ext_general_purpose_gpi_read(HI_VOID) {
    return (IORD_8DIRECT(0x102C0) & 0x0F);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_general_purpose_illumination_target
// ------------------------------------------------------------------------------ //
// debug register 1
// ------------------------------------------------------------------------------ //

#define HI_EXT_GENERAL_PURPOSE_ILLUMINATION_TARGET_DEFAULT (0xFF)
#define HI_EXT_GENERAL_PURPOSE_ILLUMINATION_TARGET_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_general_purpose_illumination_target_write(HI_U8 data) {
    IOWR_8DIRECT(0x102C4, data);
}
static __inline HI_U8 hi_ext_general_purpose_illumination_target_read(HI_VOID) {
    return IORD_8DIRECT(0x102C4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_general_purpose_equilibrium_point
// ------------------------------------------------------------------------------ //
// debug register 2
// ------------------------------------------------------------------------------ //

#define HI_EXT_GENERAL_PURPOSE_EQUILIBRIUM_POINT_DEFAULT (0x0)
#define HI_EXT_GENERAL_PURPOSE_EQUILIBRIUM_POINT_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_general_purpose_equilibrium_point_write(HI_U8 data) {
    IOWR_8DIRECT(0x102C5, data);
}
static __inline HI_U8 hi_ext_general_purpose_equilibrium_point_read(HI_VOID) {
    return IORD_8DIRECT(0x102C5);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_general_purpose_iris_rate
// ------------------------------------------------------------------------------ //
// debug register 3
// ------------------------------------------------------------------------------ //

#define HI_EXT_GENERAL_PURPOSE_IRIS_RATE_DEFAULT (0xFF)
#define HI_EXT_GENERAL_PURPOSE_IRIS_RATE_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_general_purpose_iris_rate_write(HI_U8 data) {
    IOWR_8DIRECT(0x102C8, data);
}
static __inline HI_U8 hi_ext_general_purpose_iris_rate_read(HI_VOID) {
    return IORD_8DIRECT(0x102C8);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_general_purpose_debug_4
// ------------------------------------------------------------------------------ //
// debug register 4
// ------------------------------------------------------------------------------ //

#define HI_EXT_GENERAL_PURPOSE_DEBUG_4_DEFAULT (0xFF)
#define HI_EXT_GENERAL_PURPOSE_DEBUG_4_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_general_purpose_debug_4_write(HI_U8 data) {
    IOWR_8DIRECT(0x102C9, data);
}
static __inline HI_U8 hi_ext_general_purpose_debug_4_read(HI_VOID) {
    return IORD_8DIRECT(0x102C9);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_general_purpose_debug_5
// ------------------------------------------------------------------------------ //
// debug register 5
// ------------------------------------------------------------------------------ //

#define HI_EXT_GENERAL_PURPOSE_DEBUG_5_DEFAULT (0xFF)
#define HI_EXT_GENERAL_PURPOSE_DEBUG_5_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_general_purpose_debug_5_write(HI_U8 data) {
    IOWR_8DIRECT(0x102CA, data);
}
static __inline HI_U8 hi_ext_general_purpose_debug_5_read(HI_VOID) {
    return IORD_8DIRECT(0x102CA);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_general_purpose_debug_6
// ------------------------------------------------------------------------------ //
// debug register 6
// ------------------------------------------------------------------------------ //

#define HI_EXT_GENERAL_PURPOSE_DEBUG_6_DEFAULT (0xFF)
#define HI_EXT_GENERAL_PURPOSE_DEBUG_6_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_general_purpose_debug_6_write(HI_U8 data) {
    IOWR_8DIRECT(0x102CB, data);
}
static __inline HI_U8 hi_ext_general_purpose_debug_6_read(HI_VOID) {
    return IORD_8DIRECT(0x102CB);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_general_purpose_misc_control
// ------------------------------------------------------------------------------ //
// 32 bit control output
// ------------------------------------------------------------------------------ //

#define HI_EXT_GENERAL_PURPOSE_MISC_CONTROL_DEFAULT (0x04650000)
#define HI_EXT_GENERAL_PURPOSE_MISC_CONTROL_DATASIZE (32)

// args: data (32-bit)
static __inline HI_VOID hi_ext_general_purpose_misc_control_write(HI_U32 data) {
    IOWR_32DIRECT(0x102CC, data);
}
static __inline HI_U32 hi_ext_general_purpose_misc_control_read(HI_VOID) {
    return IORD_32DIRECT(0x102CC);
}

// ------------------------------------------------------------------------------ //
// Group: Flash interface
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_ext_flash_interface_trigger_select
// ------------------------------------------------------------------------------ //
// Xenon flash trigger mode: 0 - Manual, 1 - from sensor, 2 - from timing generator, 3 - reserved
// ------------------------------------------------------------------------------ //

#define HI_EXT_FLASH_INTERFACE_TRIGGER_SELECT_DEFAULT (00)
#define HI_EXT_FLASH_INTERFACE_TRIGGER_SELECT_DATASIZE (2)

// args: data (2-bit)
static __inline HI_VOID hi_ext_flash_interface_trigger_select_write(HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(0x102D0);
    IOWR_8DIRECT(0x102D0, (data & 0x03) | (u32Current & 0xFC));
}
static __inline HI_U8 hi_ext_flash_interface_trigger_select_read(HI_VOID) {
    return (IORD_8DIRECT(0x102D0) & 0x03);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_flash_interface_sensor_strobe_polarity
// ------------------------------------------------------------------------------ //
// 1 - invert sensor strobe
// ------------------------------------------------------------------------------ //

#define HI_EXT_FLASH_INTERFACE_SENSOR_STROBE_POLARITY_DEFAULT (0)
#define HI_EXT_FLASH_INTERFACE_SENSOR_STROBE_POLARITY_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_flash_interface_sensor_strobe_polarity_write(HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(0x102D0);
    IOWR_8DIRECT(0x102D0, ((data & 0x01) << 4) | (u32Current & 0xEF));
}
static __inline HI_U8 hi_ext_flash_interface_sensor_strobe_polarity_read(HI_VOID) {
    return ((IORD_8DIRECT(0x102D0) & 0x10) >> 4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_flash_interface_af_led_enable
// ------------------------------------------------------------------------------ //
// Turn on AF LED
// ------------------------------------------------------------------------------ //

#define HI_EXT_FLASH_INTERFACE_AF_LED_ENABLE_DEFAULT (00)
#define HI_EXT_FLASH_INTERFACE_AF_LED_ENABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_flash_interface_af_led_enable_write(HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(0x102D2);
    IOWR_8DIRECT(0x102D2, (data & 0x01) | (u32Current & 0xFE));
}
static __inline HI_U8 hi_ext_flash_interface_af_led_enable_read(HI_VOID) {
    return (IORD_8DIRECT(0x102D2) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_flash_interface_charge
// ------------------------------------------------------------------------------ //
// Charge the flash module
// ------------------------------------------------------------------------------ //

#define HI_EXT_FLASH_INTERFACE_CHARGE_DEFAULT (00)
#define HI_EXT_FLASH_INTERFACE_CHARGE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_flash_interface_charge_write(HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(0x102D2);
    IOWR_8DIRECT(0x102D2, ((data & 0x01) << 1) | (u32Current & 0xFD));
}
static __inline HI_U8 hi_ext_flash_interface_charge_read(HI_VOID) {
    return ((IORD_8DIRECT(0x102D2) & 0x02) >> 1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_flash_interface_flash_trigger
// ------------------------------------------------------------------------------ //
// 1 - invert sensor strobe
// ------------------------------------------------------------------------------ //

#define HI_EXT_FLASH_INTERFACE_FLASH_TRIGGER_DEFAULT (00)
#define HI_EXT_FLASH_INTERFACE_FLASH_TRIGGER_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_flash_interface_flash_trigger_write(HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(0x102D2);
    IOWR_8DIRECT(0x102D2, ((data & 0x01) << 4) | (u32Current & 0xEF));
}
static __inline HI_U8 hi_ext_flash_interface_flash_trigger_read(HI_VOID) {
    return ((IORD_8DIRECT(0x102D2) & 0x10) >> 4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_flash_interface_flash_ready
// ------------------------------------------------------------------------------ //
// 1 - flash is charged
// ------------------------------------------------------------------------------ //

#define HI_EXT_FLASH_INTERFACE_FLASH_READY_DEFAULT (0)
#define HI_EXT_FLASH_INTERFACE_FLASH_READY_DATASIZE (1)

// args: data (1-bit)
static __inline HI_U8 hi_ext_flash_interface_flash_ready_read(HI_VOID) {
    return (IORD_8DIRECT(0x102D4) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_flash_interface_flash_strobe
// ------------------------------------------------------------------------------ //
// strobe signal from sensor module
// ------------------------------------------------------------------------------ //

#define HI_EXT_FLASH_INTERFACE_FLASH_STROBE_DEFAULT (0)
#define HI_EXT_FLASH_INTERFACE_FLASH_STROBE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_U8 hi_ext_flash_interface_flash_strobe_read(HI_VOID) {
    return ((IORD_8DIRECT(0x102D4) & 0x02) >> 1);
}

// ------------------------------------------------------------------------------ //
// Group: System
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_flags1
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_FLAGS1_DEFAULT (0x0)
#define HI_EXT_SYSTEM_FLAGS1_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_flags1_write(HI_U8 data) {
    IOWR_8DIRECT(0x10300, data);
}
static __inline HI_U8 hi_ext_system_flags1_read(HI_VOID) {
    return IORD_8DIRECT(0x10300);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_freeze_firmware
// ------------------------------------------------------------------------------ //
// Disables firmware and stops updating the ISP
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_FREEZE_FIRMWARE_DEFAULT (0)
#define HI_EXT_SYSTEM_FREEZE_FIRMWARE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_system_freeze_firmware_write(HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(0x10300);
    IOWR_8DIRECT(0x10300, (data & 0x01) | (u32Current & 0xFE));
}
static __inline HI_U8 hi_ext_system_freeze_firmware_read(HI_VOID) {
    return (IORD_8DIRECT(0x10300) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_directional_sharpening
// ------------------------------------------------------------------------------ //
// Enables manual control of directional sharpening strength: 0=auto 1=manual
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_MANUAL_DIRECTIONAL_SHARPENING_DEFAULT (0)
#define HI_EXT_SYSTEM_MANUAL_DIRECTIONAL_SHARPENING_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_system_manual_directional_sharpening_write(HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(0x10300);
    IOWR_8DIRECT(0x10300, ((data & 0x01) << 7) | (u32Current & 0x7F));
}
static __inline HI_U8 hi_ext_system_manual_directional_sharpening_read(HI_VOID) {
    return ((IORD_8DIRECT(0x10300) & 0x80) >> 7);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_flags2
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_FLAGS2_DEFAULT (0xC0)
#define HI_EXT_SYSTEM_FLAGS2_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_flags2_write(HI_U8 data) {
    IOWR_8DIRECT(0x10301, data);
}
static __inline HI_U8 hi_ext_system_flags2_read(HI_VOID) {
    return IORD_8DIRECT(0x10301);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_un_directional_sharpening
// ------------------------------------------------------------------------------ //
// Enables manual control of un-directional sharpening strength: 0=auto 1=manual
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_MANUAL_UN_DIRECTIONAL_SHARPENING_DEFAULT (0)
#define HI_EXT_SYSTEM_MANUAL_UN_DIRECTIONAL_SHARPENING_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_system_manual_un_directional_sharpening_write(HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(0x10301);
    IOWR_8DIRECT(0x10301, (data & 0x01) | (u32Current & 0xFE));
}
static __inline HI_U8 hi_ext_system_manual_un_directional_sharpening_read(HI_VOID) {
    return (IORD_8DIRECT(0x10301) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_drc
// ------------------------------------------------------------------------------ //
// Enables manual drc control - drc strength target controls drc strength directly
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_MANUAL_DRC_DEFAULT (0)
#define HI_EXT_SYSTEM_MANUAL_DRC_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_system_manual_drc_write(HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(0x10301);
    IOWR_8DIRECT(0x10301, ((data & 0x01) << 1) | (u32Current & 0xFD));
}
static __inline HI_U8 hi_ext_system_manual_drc_read(HI_VOID) {
    return ((IORD_8DIRECT(0x10301) & 0x02) >> 1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_3dnr
// ------------------------------------------------------------------------------ //
// Enables manual 3dnr control - 3dnr strength target controls 3dnr strength directly
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_MANUAL_3DNR_DEFAULT (0)
#define HI_EXT_SYSTEM_MANUAL_3DNR_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_system_manual_3dnr_write(HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(0x10301);
    IOWR_8DIRECT(0x10301, ((data & 0x01) << 3) | (u32Current & 0xF7));
}
static __inline HI_U8 hi_ext_system_manual_3dnr_read(HI_VOID) {
    return ((IORD_8DIRECT(0x10301) & 0x08) >> 3);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_calibrate_flag
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_CALIBRATE_FLAG_DEFAULT (0x0)
#define HI_EXT_SYSTEM_CALIBRATE_FLAG_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_calibrate_flag_write(HI_U8 data) {
    IOWR_8DIRECT(0x10308, data);
}
static __inline HI_U8 hi_ext_system_calibrate_flag_read(HI_VOID) {
    return IORD_8DIRECT(0x10308);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_calibrate_bad_pixels
// ------------------------------------------------------------------------------ //
// Start calibration algorithm for defect pixel correction, bit will be cleared by FW when finished
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_CALIBRATE_BAD_PIXELS_DEFAULT (0)
#define HI_EXT_SYSTEM_CALIBRATE_BAD_PIXELS_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_system_calibrate_bad_pixels_write(HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(0x10308);
    IOWR_8DIRECT(0x10308, (data & 0x01) | (u32Current & 0xFE));
}
static __inline HI_U8 hi_ext_system_calibrate_bad_pixels_read(HI_VOID) {
    return (IORD_8DIRECT(0x10308) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_frame_rates
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_FRAME_RATES_DEFAULT (0x0)
#define HI_EXT_SYSTEM_FRAME_RATES_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_frame_rates_write(HI_U8 data) {
    IOWR_8DIRECT(0x10306, data);
}
static __inline HI_U8 hi_ext_system_frame_rates_read(HI_VOID) {
    return IORD_8DIRECT(0x10306);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_set_1001_rate_divider
// ------------------------------------------------------------------------------ //
// Set 1/1.001 rate multiplier to achieve 29.97 FPS
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_SET_1001_RATE_DIVIDER_DEFAULT (0)
#define HI_EXT_SYSTEM_SET_1001_RATE_DIVIDER_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_system_set_1001_rate_divider_write(HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(0x10304);
    IOWR_8DIRECT(0x10304, ((data & 0x01) << 4) | (u32Current & 0xEF));
}
static __inline HI_U8 hi_ext_system_set_1001_rate_divider_read(HI_VOID) {
    return ((IORD_8DIRECT(0x10304) & 0x10) >> 4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_half_pixel_clock
// ------------------------------------------------------------------------------ //
// Corrects internal interface dividers to match the case when pixel clock is slowed down
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_HALF_PIXEL_CLOCK_DEFAULT (0)
#define HI_EXT_SYSTEM_HALF_PIXEL_CLOCK_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_system_half_pixel_clock_write(HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(0x10304);
    IOWR_8DIRECT(0x10304, ((data & 0x01) << 5) | (u32Current & 0xDF));
}
static __inline HI_U8 hi_ext_system_half_pixel_clock_read(HI_VOID) {
    return ((IORD_8DIRECT(0x10304) & 0x20) >> 5);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_directional_sharpening_target
// ------------------------------------------------------------------------------ //
// Directional sharpening target. In manual mode this directly sets the directional sharpening strength.
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_DIRECTIONAL_SHARPENING_TARGET_DEFAULT (0x08)
#define HI_EXT_SYSTEM_DIRECTIONAL_SHARPENING_TARGET_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_directional_sharpening_target_write(HI_U8 data) {
    IOWR_8DIRECT(0x10324, data);
}
static __inline HI_U8 hi_ext_system_directional_sharpening_target_read(HI_VOID) {
    return IORD_8DIRECT(0x10324);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_maximum_directional_sharpening
// ------------------------------------------------------------------------------ //
// Maximum directional sharpening in all modes.
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_MAXIMUM_DIRECTIONAL_SHARPENING_DEFAULT (0x08)
#define HI_EXT_SYSTEM_MAXIMUM_DIRECTIONAL_SHARPENING_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_maximum_directional_sharpening_write(HI_U8 data) {
    IOWR_8DIRECT(0x10326, data);
}
static __inline HI_U8 hi_ext_system_maximum_directional_sharpening_read(HI_VOID) {
    return IORD_8DIRECT(0x10326);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_minimum_directional_sharpening
// ------------------------------------------------------------------------------ //
// Minimum directional sharpening in all modes.
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_MINIMUM_DIRECTIONAL_SHARPENING_DEFAULT (0x08)
#define HI_EXT_SYSTEM_MINIMUM_DIRECTIONAL_SHARPENING_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_minimum_directional_sharpening_write(HI_U8 data) {
    IOWR_8DIRECT(0x10327, data);
}
static __inline HI_U8 hi_ext_system_minimum_directional_sharpening_read(HI_VOID) {
    return IORD_8DIRECT(0x10327);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_un_directional_sharpening_target
// ------------------------------------------------------------------------------ //
// Un-directional sharpening target. In manual mode this directly sets the un-directional sharpening strength.
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_UN_DIRECTIONAL_SHARPENING_TARGET_DEFAULT (0x08)
#define HI_EXT_SYSTEM_UN_DIRECTIONAL_SHARPENING_TARGET_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_un_directional_sharpening_target_write(HI_U8 data) {
    IOWR_8DIRECT(0x10328, data);
}
static __inline HI_U8 hi_ext_system_un_directional_sharpening_target_read(HI_VOID) {
    return IORD_8DIRECT(0x10328);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_maximum_un_directional_sharpening
// ------------------------------------------------------------------------------ //
// Maximum un-directional sharpening in all modes.
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_MAXIMUM_UN_DIRECTIONAL_SHARPENING_DEFAULT (0x08)
#define HI_EXT_SYSTEM_MAXIMUM_UN_DIRECTIONAL_SHARPENING_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_maximum_un_directional_sharpening_write(HI_U8 data) {
    IOWR_8DIRECT(0x1032A, data);
}
static __inline HI_U8 hi_ext_system_maximum_un_directional_sharpening_read(HI_VOID) {
    return IORD_8DIRECT(0x1032A);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_minimum_un_directional_sharpening
// ------------------------------------------------------------------------------ //
// Minimum un-directional sharpening in all modes.
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_MINIMUM_UN_DIRECTIONAL_SHARPENING_DEFAULT (0x08)
#define HI_EXT_SYSTEM_MINIMUM_UN_DIRECTIONAL_SHARPENING_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_minimum_un_directional_sharpening_write(HI_U8 data) {
    IOWR_8DIRECT(0x1032B, data);
}
static __inline HI_U8 hi_ext_system_minimum_un_directional_sharpening_read(HI_VOID) {
    return IORD_8DIRECT(0x1032B);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_drc_manual_strength
// ------------------------------------------------------------------------------ //
// Manual setting for DRC strength (for manual DRC)
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_DRC_MANUAL_STRENGTH_DEFAULT (0x80)
#define HI_EXT_SYSTEM_DRC_MANUAL_STRENGTH_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_drc_manual_strength_write(HI_U8 data) {
    IOWR_8DIRECT(0x1032C, data);
}
static __inline HI_U8 hi_ext_system_drc_manual_strength_read(HI_VOID) {
    return IORD_8DIRECT(0x1032C);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_drc_auto_strength
// ------------------------------------------------------------------------------ //
// Manual setting for DRC strength (for auto DRC)
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_DRC_AUTO_STRENGTH_DEFAULT (0x80)
#define HI_EXT_SYSTEM_DRC_AUTO_STRENGTH_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_drc_auto_strength_write(HI_U8 data) {
    IOWR_8DIRECT(0x1032D, data);
}
static __inline HI_U8 hi_ext_system_drc_auto_strength_read(HI_VOID) {
    return IORD_8DIRECT(0x1032D);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_drc_asymmetry
// ------------------------------------------------------------------------------ //
// Asymmetry of asymmetry LUT.
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_DRC_ASYMMETRY_DEFAULT (0x14)
#define HI_EXT_SYSTEM_DRC_ASYMMETRY_DATASIZE (8)

// args: data (8-bit)
#if 0
static __inline HI_VOID hi_ext_system_drc_asymmetry_write(HI_U8 data) {
    IOWR_8DIRECT(0x1032E, data);
}
static __inline HI_U8 hi_ext_system_drc_asymmetry_read(HI_VOID) {
    return IORD_8DIRECT(0x1032E);
}
#endif
// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_drc_bright_enhance
// ------------------------------------------------------------------------------ //
// Bright Enhance of asymmetry LUT.
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_DRC_BRIGHT_ENHANCE_DEFAULT (0xA0)
#define HI_EXT_SYSTEM_DRC_BRIGHT_ENHANCE_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_drc_bright_enhance_write(HI_U8 data) {
    IOWR_8DIRECT(0x1032F, data);
}
static __inline HI_U8 hi_ext_system_drc_bright_enhance_read(HI_VOID) {
    return IORD_8DIRECT(0x1032F);
}

/* not used */
#if 0
// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_maximum_drc_strength
// ------------------------------------------------------------------------------ //
// Maximum drc strength in all modes.
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_MAXIMUM_DRC_STRENGTH_DEFAULT (0x80)
#define HI_EXT_SYSTEM_MAXIMUM_DRC_STRENGTH_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_maximum_drc_strength_write(HI_U8 data) {
    IOWR_8DIRECT(0x1032E, data);
}
static __inline HI_U8 hi_ext_system_maximum_drc_strength_read(HI_VOID) {
    return IORD_8DIRECT(0x1032E);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_minimum_drc_strength
// ------------------------------------------------------------------------------ //
// Minimum drc strength in all modes.
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_MINIMUM_DRC_STRENGTH_DEFAULT (0x80)
#define HI_EXT_SYSTEM_MINIMUM_DRC_STRENGTH_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_minimum_drc_strength_write(HI_U8 data) {
    IOWR_8DIRECT(0x1032F, data);
}
static __inline HI_U8 hi_ext_system_minimum_drc_strength_read(HI_VOID) {
    return IORD_8DIRECT(0x1032F);
}
#endif

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_nr_threshold_target
// ------------------------------------------------------------------------------ //
// Overall strength of Sinter noise-reduction effect for short exposure
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_NR_THRESHOLD_TARGET_DEFAULT (0x18)
#define HI_EXT_SYSTEM_NR_THRESHOLD_TARGET_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_nr_threshold_target_write(HI_U8 data) {
    IOWR_8DIRECT(0x10330, data);
}
static __inline HI_U8 hi_ext_system_nr_threshold_target_read(HI_VOID) {
    return IORD_8DIRECT(0x10330);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_nr_threshold_long_target
// ------------------------------------------------------------------------------ //
// Overall strength of Sinter noise-reduction effect for long exposure
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_NR_THRESHOLD_LONG_TARGET_DEFAULT (0x18)
#define HI_EXT_SYSTEM_NR_THRESHOLD_LONG_TARGET_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_nr_threshold_long_target_write(HI_U8 data) {
    IOWR_8DIRECT(0x10331, data);
}
static __inline HI_U8 hi_ext_system_nr_threshold_long_target_read(HI_VOID) {
    return IORD_8DIRECT(0x10331);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_maximum_nr_strength
// ------------------------------------------------------------------------------ //
// Maximum nr strength in all modes.
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_MAXIMUM_NR_STRENGTH_DEFAULT (0x18)
#define HI_EXT_SYSTEM_MAXIMUM_NR_STRENGTH_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_maximum_nr_strength_write(HI_U8 data) {
    IOWR_8DIRECT(0x10332, data);
}
static __inline HI_U8 hi_ext_system_maximum_nr_strength_read(HI_VOID) {
    return IORD_8DIRECT(0x10332);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_minimum_nr_strength
// ------------------------------------------------------------------------------ //
// Minimum nr strength in all modes.
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_MINIMUM_NR_STRENGTH_DEFAULT (0x18)
#define HI_EXT_SYSTEM_MINIMUM_NR_STRENGTH_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_minimum_nr_strength_write(HI_U8 data) {
    IOWR_8DIRECT(0x10333, data);
}
static __inline HI_U8 hi_ext_system_minimum_nr_strength_read(HI_VOID) {
    return IORD_8DIRECT(0x10333);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_3dnr_threshold_target
// ------------------------------------------------------------------------------ //
// Overall strength of Temper noise-reduction effect
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_3DNR_THRESHOLD_TARGET_DEFAULT (0x24)
#define HI_EXT_SYSTEM_3DNR_THRESHOLD_TARGET_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_3dnr_threshold_target_write(HI_U8 data) {
    IOWR_8DIRECT(0x10334, data);
}
static __inline HI_U8 hi_ext_system_3dnr_threshold_target_read(HI_VOID) {
    return IORD_8DIRECT(0x10334);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_maximum_3dnr_strength
// ------------------------------------------------------------------------------ //
// Maximum 3dnr strength in all modes.
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_MAXIMUM_3DNR_STRENGTH_DEFAULT (0x24)
#define HI_EXT_SYSTEM_MAXIMUM_3DNR_STRENGTH_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_maximum_3dnr_strength_write(HI_U8 data) {
    IOWR_8DIRECT(0x10336, data);
}
static __inline HI_U8 hi_ext_system_maximum_3dnr_strength_read(HI_VOID) {
    return IORD_8DIRECT(0x10336);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_minimum_3dnr_strength
// ------------------------------------------------------------------------------ //
// Minimum 3dnr strength in all modes.
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_MINIMUM_3DNR_STRENGTH_DEFAULT (0x24)
#define HI_EXT_SYSTEM_MINIMUM_3DNR_STRENGTH_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_minimum_3dnr_strength_write(HI_U8 data) {
    IOWR_8DIRECT(0x10337, data);
}
static __inline HI_U8 hi_ext_system_minimum_3dnr_strength_read(HI_VOID) {
    return IORD_8DIRECT(0x10337);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_debug_0
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_DEBUG_0_DEFAULT (0x00)
#define HI_EXT_SYSTEM_DEBUG_0_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_debug_0_write(HI_U8 data) {
    IOWR_8DIRECT(0x103F0, data);
}
static __inline HI_U8 hi_ext_system_debug_0_read(HI_VOID) {
    return IORD_8DIRECT(0x103F0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_debug_1
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_DEBUG_1_DEFAULT (0x00)
#define HI_EXT_SYSTEM_DEBUG_1_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_debug_1_write(HI_U8 data) {
    IOWR_8DIRECT(0x103F1, data);
}
static __inline HI_U8 hi_ext_system_debug_1_read(HI_VOID) {
    return IORD_8DIRECT(0x103F1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_debug_2
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_DEBUG_2_DEFAULT (0x00)
#define HI_EXT_SYSTEM_DEBUG_2_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_debug_2_write(HI_U8 data) {
    IOWR_8DIRECT(0x103F2, data);
}
static __inline HI_U8 hi_ext_system_debug_2_read(HI_VOID) {
    return IORD_8DIRECT(0x103F2);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_debug_3
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_DEBUG_3_DEFAULT (0x00)
#define HI_EXT_SYSTEM_DEBUG_3_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_debug_3_write(HI_U8 data) {
    IOWR_8DIRECT(0x103F3, data);
}
static __inline HI_U8 hi_ext_system_debug_3_read(HI_VOID) {
    return IORD_8DIRECT(0x103F3);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_debug_4
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_DEBUG_4_DEFAULT (0x00)
#define HI_EXT_SYSTEM_DEBUG_4_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_debug_4_write(HI_U8 data) {
    IOWR_8DIRECT(0x103F4, data);
}
static __inline HI_U8 hi_ext_system_debug_4_read(HI_VOID) {
    return IORD_8DIRECT(0x103F4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_debug_5
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_DEBUG_5_DEFAULT (0x00)
#define HI_EXT_SYSTEM_DEBUG_5_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_debug_5_write(HI_U8 data) {
    IOWR_8DIRECT(0x103F5, data);
}
static __inline HI_U8 hi_ext_system_debug_5_read(HI_VOID) {
    return IORD_8DIRECT(0x103F5);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_debug_6
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_DEBUG_6_DEFAULT (0x00)
#define HI_EXT_SYSTEM_DEBUG_6_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_debug_6_write(HI_U8 data) {
    IOWR_8DIRECT(0x103F6, data);
}
static __inline HI_U8 hi_ext_system_debug_6_read(HI_VOID) {
    return IORD_8DIRECT(0x103F6);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_debug_7
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_DEBUG_7_DEFAULT (0x00)
#define HI_EXT_SYSTEM_DEBUG_7_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_debug_7_write(HI_U8 data) {
    IOWR_8DIRECT(0x103F7, data);
}
static __inline HI_U8 hi_ext_system_debug_7_read(HI_VOID) {
    return IORD_8DIRECT(0x103F7);
}

//==Address=========================================================//
//AE:               0x11400~0x115FF
//AWB:              0x11600~0x117FF
//AF:               0x11800~0x119FF
//NR:               0x11A00~0x11AFF
//DRC:              0x11B00~0x11BFF
//sharpen_ex:       0x11c00~0x11cFF
//sharpening:       0x12000~0x120FF
//other ISP module: 0x12100~0x121FF
//==Address=========================================================//
#define HI_EXT_SYSTEM_FPS_BASE_DEFAULT (30)
#define HI_EXT_SYSTEM_FPS_BASE_DATASIZE (32)

// args: data (32-bit)
static __inline HI_VOID hi_ext_system_fps_base_write(HI_U32 data) {
    IOWR_32DIRECT(0x11400, data);
}
static __inline HI_U32 hi_ext_system_fps_base_read(HI_VOID) {
    return IORD_32DIRECT(0x11400);
}

//==============================================================//
//NR: 0x11A00~0x11AFF
//==============================================================//


// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_nr
// ------------------------------------------------------------------------------ //
// Enables manual nr control - nr threshold target controls drc strength
// ------------------------------------------------------------------------------ //

// args: data (1-bit)
//manu_mode 0x11A00 
static __inline HI_VOID hi_ext_system_nr_manu_mode_write(HI_U8 data){
    IOWR_8DIRECT(0x11A00, data);
}
static __inline HI_U8 hi_ext_system_nr_manu_mode_read(HI_VOID) {
    return (IORD_8DIRECT(0x11A00));
}

//varstrength 0x11A04
#define HI_EXT_SYSTEM_NR_MANUAL_VAR_STRENGTH_WRITE_DEFAULT (160)

static __inline HI_VOID hi_ext_system_nr_manual_varstrength_write(HI_U8 data) {
    IOWR_8DIRECT(0x11A04, data);
}

static __inline HI_U8 hi_ext_system_nr_manual_varstrength_read(HI_VOID) {
    return (IORD_8DIRECT(0x11A04));
} 

//fixstrength 0x11A05
#define HI_EXT_SYSTEM_NR_MANUAL_FIXSTRENGTH_WRITE_DEFAULT (0)

static __inline HI_VOID hi_ext_system_nr_manual_fixstrength_write(HI_U8 data) {
    IOWR_8DIRECT(0x11A05, data);
}

static __inline HI_U8 hi_ext_system_nr_manual_fixstrength_read(HI_VOID) {
    return (IORD_8DIRECT(0x11A05));
} 

//lowfreqslope 0x11A06
#define HI_EXT_SYSTEM_NR_MANUAL_LOWFREQSLOPE_WRITE_DEFAULT (0)

static __inline HI_VOID hi_ext_system_nr_manual_lowfreqslope_write(HI_U8 data) {
    IOWR_8DIRECT(0x11A06, data);
}

static __inline HI_U8 hi_ext_system_nr_manual_lowfreqslope_read(HI_VOID) {
    return (IORD_8DIRECT(0x11A06));
} 

//threshold 0x11A08
#define HI_EXT_SYSTEM_NR_MANUAL_THRESHOLD_WRITE_DEFAULT (1500)

static __inline HI_VOID hi_ext_system_nr_manual_threshold_write(HI_U16 data) {
    IOWR_16DIRECT(0x11A08, data);
}

static __inline HI_U16 hi_ext_system_nr_manual_threshold_read(HI_VOID) {
    return (IORD_16DIRECT(0x11A08));
} 

//varstrength[16]  0x11A10
static __inline HI_VOID hi_ext_system_nr_varstrength_write(HI_U8 u8Index, HI_U8 data){
    IOWR_8DIRECT((0x11A10 + u8Index*sizeof(HI_U8)), data);
}
static __inline HI_U8 hi_ext_system_nr_varstrength_read(HI_U8 u8Index) {
    return (IORD_8DIRECT((0x11A10 + u8Index*sizeof(HI_U8))));
}

//threshold[16]  0x11A20---0x11A3f
static __inline HI_VOID hi_ext_system_nr_threshold_write(HI_U8 u8Index, HI_U16 data){
    IOWR_16DIRECT((0x11A20 + u8Index*sizeof(HI_U16)), data);
}
static __inline HI_U16 hi_ext_system_nr_threshold_read(HI_U8 u8Index) {
    return (IORD_16DIRECT((0x11A20 + u8Index*sizeof(HI_U16))));
}

//fixstrength[16]  0x11A40
static __inline HI_VOID hi_ext_system_nr_fixstrength_write(HI_U8 u8Index, HI_U8 data){
    IOWR_8DIRECT((0x11A40 + u8Index*sizeof(HI_U8)), data);
}
static __inline HI_U8 hi_ext_system_nr_fixstrength_read(HI_U8 u8Index) {
    return (IORD_8DIRECT((0x11A40 + u8Index*sizeof(HI_U8))));
}

//lowfreqslope[16]  0x11A50
static __inline HI_VOID hi_ext_system_nr_lowfreqslope_write(HI_U8 u8Index, HI_U8 data){
    IOWR_8DIRECT((0x11A50 + u8Index*sizeof(HI_U8)), data);
}
static __inline HI_U8 hi_ext_system_nr_lowfreqslope_read(HI_U8 u8Index) {
    return (IORD_8DIRECT((0x11A50 + u8Index*sizeof(HI_U8))));
}

//==============================================================//
//DRC: 0x11B00~0x11BFF
//==============================================================//

/*****************************************
enable            0x11B00~0x11B00
Asymmetry     0x11B01~0x11B01
SecondPole    0x11B02~0x11B02
*****************************************/
static __inline HI_VOID hi_ext_system_drc_enable_write(HI_U8 data){
	IOWR_8DIRECT((0x11B00), data);
}
static __inline HI_U8 hi_ext_system_drc_enable_read(HI_VOID){
	return (IORD_8DIRECT(0x11B00));
}

static __inline HI_VOID hi_ext_system_drc_asymmetry_write(HI_U8 data){
    IOWR_8DIRECT((0x11B01), data);
}
static __inline HI_U8 hi_ext_system_drc_asymmetry_read(HI_VOID){
    return (IORD_8DIRECT(0x11B01));
}

static __inline HI_VOID hi_ext_system_drc_secondpole_write(HI_U8 data){
    IOWR_8DIRECT((0x11B02), data);
}
static __inline HI_U8 hi_ext_system_drc_secondpole_read(HI_VOID){
    return (IORD_8DIRECT(0x11B02));
}

static __inline HI_VOID hi_ext_system_drc_manual_mode_write(HI_U8 data){
	IOWR_8DIRECT((0x11B03), data);
}
static __inline HI_U8 hi_ext_system_drc_manual_mode_read(HI_VOID){
	return (IORD_8DIRECT(0x11B03));
}

static __inline HI_VOID hi_ext_system_drc_stretch_write(HI_U32 data){
    IOWR_32DIRECT((0x11B04), data);
}
static __inline HI_U32 hi_ext_system_drc_stretch_read(HI_VOID){
    return (IORD_32DIRECT(0x11B04));    
}



static __inline HI_VOID hi_ext_system_gammafe_k_write(HI_U8 data){
    IOWR_8DIRECT((0x11B08), data);
}
static __inline HI_U8 hi_ext_system_gammafe_k_read(HI_VOID){
    return (IORD_8DIRECT(0x11B08));    
}

static __inline HI_VOID hi_ext_system_gammafe_s_write(HI_U8 data){
    IOWR_8DIRECT((0x11B09), data);
}
static __inline HI_U8 hi_ext_system_gammafe_s_read(HI_VOID){
    return (IORD_8DIRECT(0x11B09));    
}

static __inline HI_VOID hi_ext_system_drc_strength_target_write(HI_U8 data){
    IOWR_8DIRECT((0x11B0A), data);
}
static __inline HI_U8 hi_ext_system_drc_strength_target_read(HI_VOID){
    return (IORD_8DIRECT(0x11B0A));    
}

static __inline HI_VOID hi_ext_system_drc_bright_gain_lmt_write(HI_U16 data){
    IOWR_16DIRECT((0x11B0C), data);
}
static __inline HI_U16 hi_ext_system_drc_bright_gain_lmt_read(HI_VOID){
    return (IORD_16DIRECT(0x11B0C));    
}
static __inline HI_VOID hi_ext_system_drc_dark_gain_lmt_c_write(HI_U16 data){
    IOWR_16DIRECT((0x11B0E), data);
}
static __inline HI_U16 hi_ext_system_drc_dark_gain_lmt_c_read(HI_VOID){
    return (IORD_16DIRECT(0x11B0E));    
}
static __inline HI_VOID hi_ext_system_drc_dark_gain_lmt_y_write(HI_U16 data){
    IOWR_16DIRECT((0x11B10), data);
}
static __inline HI_U16 hi_ext_system_drc_dark_gain_lmt_y_read(HI_VOID){
    return (IORD_16DIRECT(0x11B10));    
}


static __inline HI_VOID hi_ext_system_drc_manual_localmixingbrigtht_write(HI_U8 data) {
    IOWR_8DIRECT(0x11B12, data);
}
static __inline HI_U8 hi_ext_system_drc_manual_localmixingbrigtht_read(HI_VOID) {
    return IORD_8DIRECT(0x11B12);
}

static __inline HI_VOID hi_ext_system_drc_auto_localmixingbrigtht_write(HI_U8 data) {
    IOWR_8DIRECT(0x11B13, data);
}
static __inline HI_U8 hi_ext_system_drc_auto_localmixingbrigtht_read(HI_VOID) {
    return IORD_8DIRECT(0x11B13);
}

static __inline HI_VOID hi_ext_system_drc_manual_localmixingdark_write(HI_U8 data) {
    IOWR_8DIRECT(0x11B14, data);
}
static __inline HI_U8 hi_ext_system_drc_manual_localmixingdark_read(HI_VOID) {
    return IORD_8DIRECT(0x11B14);
}

static __inline HI_VOID hi_ext_system_drc_auto_localmixingdark_write(HI_U8 data) {
    IOWR_8DIRECT(0x11B15, data);
}
static __inline HI_U8 hi_ext_system_drc_auto_localmixingdark_read(HI_VOID) {
    return IORD_8DIRECT(0x11B15);
}



//0x11B20 for sharpen


//==============================================================//
//sharpening: 0x12000~0x120FF
//==============================================================//

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sharpening_core
// ------------------------------------------------------------------------------ //
// sharpening core
// ------------------------------------------------------------------------------ //

//manu_mode 0x12000 

#define HI_EXT_SYSTEM_SHARPEN_MANU_MODE_WRITE_DEFAULT (0)

static __inline HI_VOID hi_ext_system_sharpen_manu_mode_write(HI_U8 data){
    IOWR_8DIRECT(0x12000, data);
}
static __inline HI_U8 hi_ext_system_sharpen_manu_mode_read(HI_VOID) {
    return (IORD_8DIRECT(0x12000));
}

//SharpenUd 0x12004
#define HI_EXT_SYSTEM_MANUAL_SHARPEN_UD_WRITE_DEFAULT (50)

static __inline HI_VOID hi_ext_system_manual_sharpen_Ud_write(HI_U8 data){
	IOWR_8DIRECT(0x12004, data);
}
static __inline HI_U8 hi_ext_system_manual_sharpen_Ud_read(HI_VOID) {
    return (IORD_8DIRECT(0x12004));
}

//SharpenD 0x12005
#define HI_EXT_SYSTEM_MANUAL_SHARPEN_D_WRITE_DEFAULT (32)

static __inline HI_VOID hi_ext_system_manual_sharpen_D_write(HI_U8 data){
	IOWR_8DIRECT(0x12005, data);
}
static __inline HI_U8 hi_ext_system_manual_sharpen_D_read(HI_VOID) {
    return (IORD_8DIRECT(0x12005));
}

//overshootAmt 0x12006
#define HI_EXT_SYSTEM_MANUAL_SHARPEN_OVERSHOOTAMT_WRITE_DEFAULT (100)

static __inline HI_VOID hi_ext_system_manual_sharpen_overshootAmt_write(HI_U8 data){
    IOWR_8DIRECT(0x12006 , data);
}
static __inline HI_U8 hi_ext_system_manual_sharpen_overshootAmt_read(HI_VOID) {
    return IORD_8DIRECT(0x12006);
}

//undershootAmt 0x12007
#define HI_EXT_SYSTEM_MANUAL_SHARPEN_UNDERSHOOTAMT_WRITE_DEFAULT (100)

static __inline HI_VOID hi_ext_system_manual_sharpen_undershootAmt_write(HI_U8 data){
    IOWR_8DIRECT(0x12007 , data);
}
static __inline HI_U8 hi_ext_system_manual_sharpen_undershootAmt_read(HI_VOID) {
    return IORD_8DIRECT(0x12007);
}

/*
sharpen_Ud[16]
*/
static __inline HI_VOID hi_ext_system_sharpen_Ud_write(HI_U8 u8Index, HI_U8 data){
    IOWR_8DIRECT((0x12010 + u8Index*sizeof(HI_U8)), data);
}
static __inline HI_U8 hi_ext_system_sharpen_Ud_read(HI_U8 u8Index) {
    return (IORD_8DIRECT((0x12010 + u8Index*sizeof(HI_U8))));
}

/*
sharpen_D[16]
*/
static __inline HI_VOID hi_ext_system_sharpen_D_write(HI_U8 u8Index, HI_U8 data){
    IOWR_8DIRECT((0x12020 + u8Index*sizeof(HI_U8)), data);
}
static __inline HI_U8 hi_ext_system_sharpen_D_read(HI_U8 u8Index) {
    return (IORD_8DIRECT((0x12020 + u8Index*sizeof(HI_U8))));
}

/*
overshootAmt[16]
*/
static __inline HI_VOID hi_ext_system_sharpen_overshootAmt_write(HI_U8 u8Index, HI_U8 data){
    IOWR_8DIRECT((0x12030 + u8Index*sizeof(HI_U8)), data);
}
static __inline HI_U8 hi_ext_system_sharpen_overshootAmt_read(HI_U8 u8Index) {
    return (IORD_8DIRECT((0x12030 + u8Index*sizeof(HI_U8))));
}

/*
undershootAmt[16]
*/
static __inline HI_VOID hi_ext_system_sharpen_undershootAmt_write(HI_U8 u8Index, HI_U8 data){
    IOWR_8DIRECT((0x12040 + u8Index*sizeof(HI_U8)), data);
}
static __inline HI_U8 hi_ext_system_sharpen_undershootAmt_read(HI_U8 u8Index) {
    return (IORD_8DIRECT((0x12040 + u8Index*sizeof(HI_U8))));
}

// EnLowLumaShoot (1-bit)	0x12050

#define HI_EXT_SYSTEM_MANUAL_SHARPEN_ENLOWLUMASHOOT_WRITE_DEFAULT (0)

static __inline HI_VOID hi_ext_system_manual_sharpen_EnLowLumaShoot_write(HI_U8 data)
{
    HI_U8 u8Current = IORD_8DIRECT(0x12050);
    IOWR_8DIRECT(0x12050, (data & 0x01) | (u8Current & 0xFE));
}
static __inline HI_U8 hi_ext_system_manual_sharpen_EnLowLumaShoot_read(HI_VOID)
{
    return (IORD_8DIRECT(0x12050) & 0x01);
}

//TextureNoiseThd 0x12051
#define HI_EXT_SYSTEM_MANUAL_SHARPEN_TEXTURENOISETHD_WRITE_DEFAULT (0)

static __inline HI_VOID hi_ext_system_manual_sharpen_TextureNoiseThd_write(HI_U8 data){
	IOWR_8DIRECT(0x12051, data);
}
static __inline HI_U8 hi_ext_system_manual_sharpen_TextureNoiseThd_read(HI_VOID) {
    return (IORD_8DIRECT(0x12051));
}

//EdgeNoiseThd 0x12052
#define HI_EXT_SYSTEM_MANUAL_SHARPEN_EDGENOISETHD_WRITE_DEFAULT (0)

static __inline HI_VOID hi_ext_system_manual_sharpen_EdgeNoiseThd_write(HI_U8 data){
	IOWR_8DIRECT(0x12052, data);
}
static __inline HI_U8 hi_ext_system_manual_sharpen_EdgeNoiseThd_read(HI_VOID) {
    return (IORD_8DIRECT(0x12052));
}

// EnLowLumaShoot[16] (1-bit) 0x12054

static __inline HI_VOID hi_ext_system_sharpen_EnLowLumaShoot_write(HI_U8 u8Index, HI_U8 data)
{
    IOWR_8DIRECT((0x12054 + u8Index*sizeof(HI_U8)),  (data & 0x01));
}
static __inline HI_U8 hi_ext_system_sharpen_EnLowLumaShoot_read(HI_U8 u8Index)
{
    return (IORD_8DIRECT((0x12054 + u8Index*sizeof(HI_U8))) & 0x01);
}

/*
TextureNoiseThd[16] 0x12064
*/
static __inline HI_VOID hi_ext_system_sharpen_TextureNoiseThd_write(HI_U8 u8Index, HI_U8 data){
    IOWR_8DIRECT((0x12064 + u8Index*sizeof(HI_U8)), data);
}
static __inline HI_U8 hi_ext_system_sharpen_TextureNoiseThd_read(HI_U8 u8Index) {
    return (IORD_8DIRECT((0x12064 + u8Index*sizeof(HI_U8))));
}

/*
EdgeNoiseThd[16] 0x12074
*/
static __inline HI_VOID hi_ext_system_sharpen_EdgeNoiseThd_write(HI_U8 u8Index, HI_U8 data){
    IOWR_8DIRECT((0x12074 + u8Index*sizeof(HI_U8)), data);
}
static __inline HI_U8 hi_ext_system_sharpen_EdgeNoiseThd_read(HI_U8 u8Index) {
    return (IORD_8DIRECT((0x12074 + u8Index*sizeof(HI_U8))));
}


//=============================================================//
//other ISP module: 0x12100~0x121FF
//=============================================================//

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_window_mode
// ------------------------------------------------------------------------------ //
// window mode
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_WINDOW_MODE_DEFAULT (0)
#define HI_EXT_SYSTEM_WINDOW_MODE_DATASIZE (8)
    
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_window_mode_write(HI_U8 data)
{
    HI_U16 u32Current = IORD_16DIRECT(0x12100);
    IOWR_16DIRECT(0x12100, (((HI_U16) data) << 8) | (u32Current & 0x00FF));
}
static __inline HI_U8 hi_ext_system_window_mode_read(HI_VOID) 
{
    return (HI_U8) ((IORD_16DIRECT(0x12100) & 0xFF00) >> 8);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_bad_pixel_thresh
// ------------------------------------------------------------------------------ //
// define the start thresh of defect pixel detection
// ------------------------------------------------------------------------------ //

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_bad_pixel_manual_mode_write(HI_U8 data) {
    IOWR_8DIRECT(0x12101, data);
}
static __inline HI_U8 hi_ext_system_bad_pixel_manual_mode_read(HI_VOID) {
    return (IORD_8DIRECT(0x12101) & 0x1);
}

//0x12102 ~ 0x12114
#define HI_EXT_SYSTEM_BAD_PIXEL_START_THRESH_DEFAULT (0x1f)
#define HI_EXT_SYSTEM_BAD_PIXEL_START_THRESH_DATASIZE (8)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_bad_pixel_start_thresh_write(HI_U8 data) {
    HI_U16 u32Current = IORD_16DIRECT(0x12106);
    IOWR_16DIRECT(0x12106, ((HI_U16) data) | (u32Current & 0xFF00));
}
static __inline HI_U8 hi_ext_system_bad_pixel_start_thresh_read(HI_VOID) {
    return (HI_U8) (IORD_16DIRECT(0x12106) & 0x00FF);
}


// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_bad_pixel_trigger_status
// ------------------------------------------------------------------------------ //
// the static bad pixel trigger status 
// 0:Initial status
// 1:Finished
// 2:time out
//
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_BAD_PIXEL_TRIGGER_STATUS_DEFAULT (0x0)
#define HI_EXT_SYSTEM_BAD_PIXEL_TRIGGER_STATUS_DATASIZE (2)

// args: data (8-bit)
static __inline HI_VOID hi_ext_system_bad_pixel_trigger_status_write(HI_U8 data) {
    HI_U16 u32Current = IORD_16DIRECT(0x12106);
    IOWR_16DIRECT(0x12106, (((HI_U16) data) << 8) | (u32Current & 0xFCFF));
}
static __inline HI_U8 hi_ext_system_bad_pixel_trigger_status_read(HI_VOID) {
    return (HI_U8) ((IORD_16DIRECT(0x12106) & 0x0300) >> 8);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_bad_pixel_trigger_time
// ------------------------------------------------------------------------------ //
// the bad pixel trigger frame cnt of time out 
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_BAD_PIXEL_TRIGGER_TIME_DEFAULT (0x640)
#define HI_EXT_SYSTEM_BAD_PIXEL_TRIGGER_TIME_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_ext_system_bad_pixel_trigger_time_write(HI_U16 data) {
    IOWR_16DIRECT(0x12108, data);
}
static __inline HI_U16 hi_ext_system_bad_pixel_trigger_time_read(HI_VOID) {
    return IORD_16DIRECT(0x12108) ;
}


// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_bad_pixel_count_max
// ------------------------------------------------------------------------------ //
// bad pixel calibration  the maxmum bad pixel count
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_BAD_PIXEL_COUNT_MAX_DEFAULT (0x100)
#define HI_EXT_SYSTEM_BAD_PIXEL_COUNT_MAX_DATASIZE (16)

// args: data (8-bit)
// data format: unsigned 1.7-bit fixed-point
static __inline HI_VOID hi_ext_system_bad_pixel_count_max_write(HI_U16 data) {
    IOWR_16DIRECT(0x12110, data);
}
static __inline HI_U16 hi_ext_system_bad_pixel_count_max_read(HI_VOID) {
    return IORD_16DIRECT(0x12110);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_bad_pixel_count_min
// ------------------------------------------------------------------------------ //
// bad pixel calibration  the minmum bad pixel count
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_BAD_PIXEL_COUNT_MIN_DEFAULT (0x40)
#define HI_EXT_SYSTEM_BAD_PIXEL_COUNT_MIN_DATASIZE (16)

// args: data (8-bit)
// data format: unsigned 1.7-bit fixed-point
static __inline HI_VOID hi_ext_system_bad_pixel_count_min_write(HI_U16 data) {
    IOWR_16DIRECT(0x12112, data);
}
static __inline HI_U16 hi_ext_system_bad_pixel_count_min_read(HI_VOID) {
    return IORD_16DIRECT(0x12112);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_low_light_enable
// ------------------------------------------------------------------------------ //
// 
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_LOW_LIGHT_ENABLE_DEFAULT (0x0)
#define HI_EXT_SYSTEM_LOW_LIGHT_ENABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_U8 hi_ext_system_low_light_enable_read(HI_VOID) {
    return (HI_U8) ((IORD_16DIRECT(0x12114) & 0x0001));
}
static __inline HI_VOID hi_ext_system_low_light_enable_write(HI_U8 data) {
    HI_U16 u32Current = IORD_16DIRECT(0x12114);
    IOWR_16DIRECT(0x12114, ((HI_U16) (data & 0x0001)) | (u32Current & 0xFFFE));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_manual_exp_ratio_enable
// ------------------------------------------------------------------------------ //
// 0:auto exposure ratio, 1:manual exposure ratio. only used in frame switching mode.
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_MANUAL_EXP_RATIO_ENABLE_DEFAULT (0x0)
#define HI_EXT_SYSTEM_MANUAL_EXP_RATIO_ENABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_system_manual_exp_ratio_enable_write(HI_U8 data) {
    HI_U16 u32Current = IORD_16DIRECT(0x12114);
    IOWR_16DIRECT(0x12114, (((HI_U16) (data & 0x0001)) << 1) | (u32Current & 0xFFFD));
}
static __inline HI_U8 hi_ext_system_manual_exp_ratio_enable_read(HI_VOID) {
    return (HI_U8) ((IORD_16DIRECT(0x12114) & 0x0002) >> 1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sensor_wdr_mode
// ------------------------------------------------------------------------------ //
// WDR mode: 0=linear 1=built-in wdr 2=2to1 full frame wdr  ...
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_SENSOR_WDR_MODE_DEFAULT (0x0)
#define HI_EXT_SYSTEM_SENSOR_WDR_MODE_DATASIZE (6)

// args: data (6-bit)
static __inline HI_VOID hi_ext_system_sensor_wdr_mode_write(HI_U8 data) {
    HI_U16 u32Current = IORD_16DIRECT(0x12114);
    IOWR_16DIRECT(0x12114, (((HI_U16) (data & 0x003F)) << 4) | (u32Current & 0xFC0F));
}
static __inline HI_U8 hi_ext_system_sensor_wdr_mode_read(HI_VOID) {
    return (HI_U8) ((IORD_16DIRECT(0x12114) & 0x03F0) >> 4);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_static_defect_pixel_detect_type
// ------------------------------------------------------------------------------ //
// ISP_STATIC_DP_BRIGHT:0x0,ISP_STATIC_DP_DARK:0x1
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_BAD_PIXEL_DETECT_TYPE_DEFAULT (0x0)
#define HI_EXT_SYSTEM_BAD_PIXEL_DETECT_TYPE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_system_bad_pixel_detect_type_write(HI_U8 data) {
    IOWR_8DIRECT(0x12116, data&0x1);
}
static __inline HI_U8 hi_ext_system_bad_pixel_detect_type_read(HI_VOID) {
    return  (IORD_8DIRECT(0x12116) & 0x1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_gamma_curve_type
// ------------------------------------------------------------------------------ //
// the type of gamma curve
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_GAMMA_CURVE_TYPE_DEFAULT (0)
#define HI_EXT_SYSTEM_GAMMA_CURVE_TYPE_DATASIZE (8)
    
// args: data (8-bit)
static __inline HI_VOID hi_ext_system_gamma_curve_type_write(HI_U8 data)
{
    IOWR_8DIRECT(0x12120, data);
}
static __inline HI_U8 hi_ext_system_gamma_curve_type_read(HI_VOID) 
{
    return IORD_8DIRECT(0x12120);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sys_debug_enable
// ------------------------------------------------------------------------------ //
// 0:disable system debug information
// 1:enable system debug information
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_SYS_DEBUG_ENABLE_DEFAULT (0x0)
#define HI_EXT_SYSTEM_SYS_DEBUG_ENABLE_DATASIZE (1)
    
// args: data (1-bit)
static __inline HI_VOID hi_ext_system_sys_debug_enable_write(HI_U16 data)
{
    HI_U16 u32Current = IORD_16DIRECT(0x1218C);
    IOWR_16DIRECT(0x1218C, (((HI_U16) data) << 0) | (u32Current & 0xFFFE));
}

static __inline HI_U16 hi_ext_system_sys_debug_enable_read(HI_VOID) 
{
    return (HI_U16) ((IORD_16DIRECT(0x1218C) & 0x0001) >> 0);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sys_debug_addr
// ------------------------------------------------------------------------------ //
// address of system debug information
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_SYS_DEBUG_ADDR_DEFAULT (0x0)
#define HI_EXT_SYSTEM_SYS_DEBUG_ADDR_DATASIZE (32)
    
// args: data (32-bit)
static __inline HI_VOID hi_ext_system_sys_debug_addr_write(HI_U32 data)
{
    IOWR_16DIRECT(0x12190, (HI_U16)(data & 0xFFFF) );
    IOWR_16DIRECT(0x12192, (HI_U16)((data & 0xFFFF0000) >> 16) );    
}

static __inline HI_U32 hi_ext_system_sys_debug_addr_read(HI_VOID) 
{
    return (HI_U32)IORD_16DIRECT(0x12190) + ((HI_U32)IORD_16DIRECT(0x12192) << 16);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sys_debug_size
// ------------------------------------------------------------------------------ //
// size of sys debug struct
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_SYS_DEBUG_SIZE_DEFAULT (0x0)
#define HI_EXT_SYSTEM_SYS_DEBUG_SIZE_DATASIZE (32)
    
// args: data (32-bit)
static __inline HI_VOID hi_ext_system_sys_debug_size_write(HI_U32 data)
{
    IOWR_16DIRECT(0x12194, (HI_U16)(data & 0xFFFF) );
    IOWR_16DIRECT(0x12196, (HI_U16)((data & 0xFFFF0000) >> 16) );    
}

static __inline HI_U32 hi_ext_system_sys_debug_size_read(HI_VOID) 
{
    return (HI_U32)IORD_16DIRECT(0x12194) + ((HI_U32)IORD_16DIRECT(0x12196) << 16);
}


// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_debug_depth
// ------------------------------------------------------------------------------ //
// the depth of all debug information
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_DEBUG_DEPTH_DEFAULT (0x0)
#define HI_EXT_SYSTEM_DEBUG_DEPTH_DATASIZE (16)
    
// args: data (16-bit)
static __inline HI_VOID hi_ext_system_sys_debug_depth_write(HI_U16 data)
{
    IOWR_16DIRECT(0x12198, data);
}

static __inline HI_U16 hi_ext_system_sys_debug_depth_read(HI_VOID) 
{
    return IORD_16DIRECT(0x12198);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_shading_table_node_number
// ------------------------------------------------------------------------------ //
// the node number of shading table
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_SHADING_TABLE_NODE_NUMBER_DEFAULT (0x0081)
#define HI_EXT_SYSTEM_SHADING_TABLE_NODE_NUMBER_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_ext_system_shading_table_node_number_write(HI_U16 data) {
    IOWR_16DIRECT(0x1219a, data);
}
static __inline HI_U16 hi_ext_system_shading_table_node_number_read(HI_VOID) {
    return IORD_16DIRECT(0x1219a);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dynamic_defect_pixel_slope
// ------------------------------------------------------------------------------ //
// slope of dynamic defect pixel
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_DYNAMIC_DEFECT_PIXEL_SLOPE_DEFAULT (0x200)
#define HI_EXT_SYSTEM_DYNAMIC_DEFECT_PIXEL_SLOPE_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_ext_system_dynamic_defect_pixel_slope_write(HI_U16 data) {
    IOWR_16DIRECT(0x1219c, data);
}
static __inline HI_U16 hi_ext_system_dynamic_defect_pixel_slope_read(HI_VOID) {
    return IORD_16DIRECT(0x1219c);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dynamic_defect_pixel_thresh
// ------------------------------------------------------------------------------ //
// thresh of dynamic defect pixel
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_DYNAMIC_DEFECT_PIXEL_THRESH_DEFAULT (0x40)
#define HI_EXT_SYSTEM_DYNAMIC_DEFECT_PIXEL_THRESH_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_ext_system_dynamic_defect_pixel_thresh_write(HI_U16 data) {
    IOWR_16DIRECT(0x1219e, data);
}
static __inline HI_U16 hi_ext_system_dynamic_defect_pixel_thresh_read(HI_VOID) {
    return IORD_16DIRECT(0x1219e);
}

// ------------------------------------------------------------------------------ //


//==============================================================//
//Frame stitch WDR: 0x121A0~0x121BF
//==============================================================//
// ------------------------------------------------------------------------------ //
// Register: hi_ext_wdr_manual_exp_ratio_enable
// ------------------------------------------------------------------------------ //
// 0: auto exposure ratio; 1: manual exposure ratio; 
// ------------------------------------------------------------------------------ //

#define HI_EXT_WDR_MANUAL_EXP_RATIO_ENABLE_DEFAULT (0x0)
#define HI_EXT_WDR_MANUAL_EXP_RATIO_ENABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_ext_wdr_manual_exp_ratio_enable_write(HI_U8 data) {
    HI_U8 u32Current = IORD_8DIRECT(0x121A0);
    IOWR_8DIRECT(0x121A0, ((data & 0x0001) << 0) | (u32Current & 0xFE));
}
static __inline HI_U8 hi_ext_wdr_manual_exp_ratio_enable_read(HI_VOID) {
    return (IORD_8DIRECT(0x121A0) & 0x1) >> 0 ;
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_wdr_exp_ratio_target
// ------------------------------------------------------------------------------ //
// exposure ratio target 
// ------------------------------------------------------------------------------ //

#define HI_EXT_WDR_EXP_RATIO_TARGET_DEFAULT (0x400)
#define HI_EXT_WDR_EXP_RATIO_TARGET_DATASIZE (12)

// args: data (12-bit)
static __inline HI_VOID hi_ext_wdr_exp_ratio_target_write(HI_U16 data) {
    IOWR_16DIRECT(0x121A2, data & 0xFFF);
}
static __inline HI_U16 hi_ext_wdr_exp_ratio_target_read(HI_VOID) {
    return IORD_16DIRECT(0x121A2) & 0xFFF;
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_wdr_compress_mode
// ------------------------------------------------------------------------------ //
// buffer number in WDR compress mode
// ------------------------------------------------------------------------------ //

#define HI_EXT_WDR_COMPRESS_DEFAULT (0x1)
#define HI_EXT_WDR_COMPRESS_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_ext_wdr_compress_write(HI_U16 data) {
    IOWR_16DIRECT(0x121A4, data & 0xFFFF);
}
static __inline HI_U16 hi_ext_wdr_compress_read(HI_VOID) {
    return IORD_16DIRECT(0x121A4) & 0xFFFF;
}

//==============================================================//
//Dehaze: 0x12300~0x123FF
//==============================================================//

// ------------------------------------------------------------------------------ //
// Register: isp_ext_system_manual_dehaze_enable
// ------------------------------------------------------------------------------ //
// Enables manual dehaze: 0=auto 1=manual
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_MANUAL_DEHAZE_ENABLE_DEFAULT (1)
#define ISP_EXT_SYSTEM_MANUAL_DEHAZE_ENABLE_DATASIZE (1)

// args: data (8-bit)
static __inline void isp_ext_system_manual_dehaze_enable_write(HI_U8 data) {
    IOWR_8DIRECT(0x12300, data);
}
static __inline HI_U8 isp_ext_system_manual_dehaze_enable_read(HI_VOID) {
    return IORD_8DIRECT(0x12300);
}

// ------------------------------------------------------------------------------ //
// Register: isp_ext_system_manual_dehaze_hblk
// ------------------------------------------------------------------------------ //
// manual dehaze hblk number
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_MANUAL_DEHAZE_HBLK_DEFAULT (32)
#define ISP_EXT_SYSTEM_MANUAL_DEHAZE_HBLK_DATASIZE (8)

// args: data (8-bit)
static __inline void isp_ext_system_manual_dehaze_hblk_write(HI_U8 data) {
    IOWR_8DIRECT(0x12301, data);
}
static __inline HI_U8 isp_ext_system_manual_dehaze_hblk_read(HI_VOID) {
    return IORD_8DIRECT(0x12301);
}

// ------------------------------------------------------------------------------ //
// Register: isp_ext_system_manual_dehaze_vblk
// ------------------------------------------------------------------------------ //
// manual dehaze vblk number
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_MANUAL_DEHAZE_VBLK_DEFAULT (30)
#define ISP_EXT_SYSTEM_MANUAL_DEHAZE_VBLK_DATASIZE (8)

// args: data (8-bit)
static __inline void isp_ext_system_manual_dehaze_vblk_write(HI_U8 data) {
    IOWR_8DIRECT(0x12302, data);
}
static __inline HI_U8 isp_ext_system_manual_dehaze_vblk_read(HI_VOID) {
    return IORD_8DIRECT(0x12302);
}

// ------------------------------------------------------------------------------ //
// Register: isp_ext_system_manual_dehaze_ctp
// ------------------------------------------------------------------------------ //
// manual dehaze curve turn point
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_DEHAZE_CTP_DEFAULT (250)
#define ISP_EXT_SYSTEM_DEHAZE_CTP_DATASIZE (8)

// args: data (8-bit)
static __inline void isp_ext_system_dehaze_ctp_write(HI_U8 data) {
    IOWR_8DIRECT(0x12303, data);
}
static __inline HI_U8 isp_ext_system_dehaze_ctp_read(HI_VOID) {
    return IORD_8DIRECT(0x12303);
}

// ------------------------------------------------------------------------------ //
// Register: isp_ext_system_manual_dehaze_mft
// ------------------------------------------------------------------------------ //
// manual dehaze minumum filter threshold
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_DEHAZE_MFT_DEFAULT (128)
#define ISP_EXT_SYSTEM_MANUAL_DEHAZE_MFT_DATASIZE (8)

// args: data (8-bit)
static __inline void isp_ext_system_dehaze_mft_write(HI_U8 data) {
    IOWR_8DIRECT(0x12304, data);
}
static __inline HI_U8 isp_ext_system_dehaze_mft_read(HI_VOID) {
    return IORD_8DIRECT(0x12304);
}

// ------------------------------------------------------------------------------ //
// Register: isp_ext_system_manual_dehaze_mfs
// ------------------------------------------------------------------------------ //
// manual dehaze minumum filter size
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_DEHAZE_MFS_DEFAULT (7)
#define ISP_EXT_SYSTEM_DEHAZE_MFS_DATASIZE (8)

// args: data (8-bit)
static __inline void isp_ext_system_dehaze_mfs_write(HI_U8 data) {
    IOWR_8DIRECT(0x12305, data);
}
static __inline HI_U8 isp_ext_system_dehaze_mfs_read(HI_VOID) {
    return IORD_8DIRECT(0x12305);
}

// ------------------------------------------------------------------------------ //
// Register: isp_ext_system_manual_dehaze_ct
// ------------------------------------------------------------------------------ //
// manual dehaze change threshold
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_DEHAZE_CT_DEFAULT (255)
#define ISP_EXT_SYSTEM_DEHAZE_CT_DATASIZE (8)

// args: data (8-bit)
static __inline void isp_ext_system_dehaze_ct_write(HI_U8 data) {
    IOWR_8DIRECT(0x12306, data);
}
static __inline HI_U8 isp_ext_system_dehaze_ct_read(HI_VOID) {
    return IORD_8DIRECT(0x12306);
}

// ------------------------------------------------------------------------------ //
// Register: isp_ext_system_manual_dehaze_tfic
// ------------------------------------------------------------------------------ //
// manual dehaze temper filter increase ceofficient
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_DEHAZE_TFIC_DEFAULT (4)
#define ISP_EXT_SYSTEM_DEHAZE_TFIC_DATASIZE (8)

// args: data (16-bit)
static __inline void isp_ext_system_dehaze_tfic_write(HI_U16 data) {
    IOWR_16DIRECT(0x12308, data);
}
static __inline HI_U16 isp_ext_system_dehaze_tfic_read(HI_VOID) {
    return IORD_16DIRECT(0x12308);
}

// ------------------------------------------------------------------------------ //
// Register: isp_ext_system_manual_dehaze_tfdc
// ------------------------------------------------------------------------------ //
// manual dehaze temper filter decrease ceofficient
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_DEHAZE_TFDC_DEFAULT (16)
#define ISP_EXT_SYSTEM_DEHAZE_TFDC_DATASIZE (16)

// args: data (16-bit)
static __inline void isp_ext_system_dehaze_tfdc_write(HI_U16 data) {
    IOWR_16DIRECT(0x1230A, data);
}
static __inline HI_U16 isp_ext_system_dehaze_tfdc_read(HI_VOID) {
    return IORD_16DIRECT(0x1230A);
}

// ------------------------------------------------------------------------------ //
// Register: isp_ext_system_dehaze_debug_enable
// ------------------------------------------------------------------------------ //
// dehaze debug enable
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_DEHAZE_DEBUG_ENABLE_DEFAULT (0)
#define ISP_EXT_SYSTEM_DEHAZE_DEBUG_ENABLE_DATASIZE (8)

// args: data (8-bit)
static __inline void isp_ext_system_dehaze_debug_enable_write(HI_U8 data) {
    IOWR_8DIRECT(0x1230C, data);
}
static __inline HI_U8 isp_ext_system_dehaze_debug_enable_read(HI_VOID) {
    return IORD_8DIRECT(0x1230C);
}

// ------------------------------------------------------------------------------ //
// Register: isp_ext_system_manual_dehaze_strength
// ------------------------------------------------------------------------------ //
// dehaze strength
// ------------------------------------------------------------------------------ //

#define ISP_EXT_SYSTEM_MANUAL_DEHAZE_STRENGTH_DEFAULT  (0xD2)
#define ISP_EXT_SYSTEM_MANUAL_DEHAZE_STRENGTH_DATASIZE (8)

// args: data (8-bit)
static __inline void isp_ext_system_manual_dehaze_strength_write(HI_U8 data) {
    IOWR_8DIRECT(0x1230D, data);
}
static __inline HI_U8 isp_ext_system_manual_dehaze_strength_read(HI_VOID) {
    return IORD_8DIRECT(0x1230D);
}

// ------------------------------------------------------------------------------ //
// Register: isp_ext_system_manual_fpn_strength
// ------------------------------------------------------------------------------ //
#define ISP_EXT_SYSTEM_FPN_STRENGTH_DEFAULT  (0)
#define ISP_EXT_SYSTEM_FPN_STRENGTH_DATASIZE (16)

static __inline void isp_ext_system_manual_fpn_strength_write(HI_U16 data)
{
    IOWR_16DIRECT(0x1230E, data);
}
static __inline HI_U16 isp_ext_system_manual_fpn_strength_read(HI_VOID)
{
    return IORD_16DIRECT(0x1230E);
}

// ------------------------------------------------------------------------------ //
// Register: isp_ext_system_manual_fpn_opmode
// ------------------------------------------------------------------------------ //
static __inline void isp_ext_system_manual_fpn_opmode_write(HI_U8 data)
{
     HI_U16 u32Current = IORD_16DIRECT(0x12310);
     IOWR_16DIRECT(0x12310, ((HI_U16)data) |(u32Current & 0xFF00));
}

static __inline HI_U8 isp_ext_system_manual_fpn_opmode_read(HI_VOID)
{
    return (HI_U8) (IORD_16DIRECT(0x12310) & 0x00FF);
}


// ------------------------------------------------------------------------------ //
// Register: isp_ext_system_manual_fpn_update
// ------------------------------------------------------------------------------ //

static __inline void isp_ext_system_manual_fpn_update_write(HI_U8 data)
{
     HI_U16 u32Current = IORD_16DIRECT(0x12312);
     IOWR_16DIRECT(0x12312, ((HI_U16)data) |(u32Current & 0xFF00));
}

static __inline HI_U8 isp_ext_system_manual_fpn_update_read(HI_VOID)
{
    return (HI_U8) (IORD_16DIRECT(0x12312) & 0x00FF);
}

// ------------------------------------------------------------------------------ //
// Register: isp_ext_system_manual_fpn_CorrCfg
// ------------------------------------------------------------------------------ //

static __inline void isp_ext_system_manual_fpn_CorrCfg_write(HI_U8 data)
{
    HI_U16 u32Current = IORD_16DIRECT(0x12314);
    IOWR_16DIRECT(0x12314, ((HI_U16)data) |(u32Current & 0xFF00));
}

static __inline HI_U8 isp_ext_system_manual_fpn_CorrCfg_read(HI_VOID)
{
    return (HI_U8) (IORD_16DIRECT(0x12314) & 0x00FF);
}

#define ISP_EXT_SYSTEM_MANUAL_DEHAZE_AUTOSTRENGTH_DEFAULT  (0xf0)
#define ISP_EXT_SYSTEM_MANUAL_DEHAZE_AUTOSTRENGTH_DATASIZE (8)

// args: data (8-bit)
static __inline void isp_ext_system_manual_dehaze_autostrength_write(HI_U8 data) {
    IOWR_8DIRECT(0x12316, data);
}
static __inline HI_U8 isp_ext_system_manual_dehaze_autostrength_read(HI_VOID) {
    return IORD_8DIRECT(0x12316);
}

/* 把标定时候的ISO存储起来 */
static __inline void isp_ext_system_manual_FPN_ISO_write(HI_U32 data) 
{
    IOWR_32DIRECT(0x12318, data);
}
/* 把标定时候的ISO读出来 */
static __inline HI_U32 isp_ext_system_manual_FPN_ISO_read(HI_VOID) 
{
    return IORD_32DIRECT(0x12318);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_agc_table_sharp_alt_d
// ------------------------------------------------------------------------------ //
//args: data(8-bits)

static __inline HI_VOID hi_ext_system_agc_table_sharp_alt_d_write(HI_U8 index, HI_U8 data){
    IOWR_8DIRECT(0x12326 + index, data);
}

static __inline HI_U8 hi_ext_system_agc_table_sharp_alt_d_read(HI_U8 index){
  return IORD_8DIRECT(0x12326 + index);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_agc_table_sharp_alt_ud
// ------------------------------------------------------------------------------ //
//args: data(8-bits)

static __inline HI_VOID hi_ext_system_agc_table_sharp_alt_ud_write(HI_U8 index, HI_U8 data){
    IOWR_8DIRECT(0x12336 + index, data);
}

static __inline HI_U8 hi_ext_system_agc_table_sharp_alt_ud_read(HI_U8 index){
  return IORD_8DIRECT(0x12336 + index);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_agc_table_RGBsharp_strength
// ------------------------------------------------------------------------------ //
//args: data(8-bits)

static __inline HI_VOID hi_ext_system_agc_table_RGBsharp_strength_write(HI_U8 index,HI_U8 data){
    IOWR_8DIRECT(0x12346 + index, data);
}

static __inline HI_U8 hi_ext_system_agc_table_RGBsharp_strength_read(HI_U8 index){
  return IORD_8DIRECT(0x12346 + index);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_agc_table_snr_thresh
// ------------------------------------------------------------------------------ //
//args: data(8-bits)
static __inline HI_VOID hi_ext_system_agc_table_snr_thresh_write(HI_U8 index,HI_U8 data){
    IOWR_8DIRECT(0x12356 + index, data);
}

static __inline HI_U8 hi_ext_system_agc_table_snr_thresh_read(HI_U8 index){
    return IORD_8DIRECT(0x12356 + index);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_agc_table_ge_strength
// ------------------------------------------------------------------------------ //

static __inline HI_VOID hi_ext_system_agc_table_ge_strength_write(HI_U8 index,HI_U8 data){
    IOWR_8DIRECT(0x12366 + index, data);
}

static __inline HI_U8 hi_ext_system_agc_table_ge_strength_read(HI_U8 index){
    return IORD_8DIRECT(0x12366 + index);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_agc_table_demosaic_np_offset
// ------------------------------------------------------------------------------ //
//args: data(8-bits)
static __inline HI_VOID hi_ext_system_agc_table_demosaic_np_offset_write(HI_U8 index,HI_U8 data){
    IOWR_8DIRECT(0x12376 + index, data);
}

static __inline HI_U8 hi_ext_system_agc_table_demosaic_np_offset_read(HI_U8 index){
    return IORD_8DIRECT(0x12376 + index);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_agc_table_demosaic_lum_thresh
// ------------------------------------------------------------------------------ //
//args: data(8-bits)
static __inline HI_VOID hi_ext_system_agc_table_demosaic_lum_thresh_write(HI_U8 index,HI_U8 data){
    IOWR_8DIRECT(0x12386 + index, data);
}

static __inline HI_U8 hi_ext_system_agc_table_demosaic_lum_thresh_read(HI_U8 index){
    return IORD_8DIRECT(0x12386 + index);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sensor_max_resolution_width
// ------------------------------------------------------------------------------ //
// width of sensor resolution 
// ------------------------------------------------------------------------------ //


#define HI_EXT_SYSTEM_SENSOR_MAX_RESOLUTION_WIDTH_DEFAULT (1280)
#define HI_EXT_SYSTEM_SENSOR_MAX_RESOLUTION_WIDTH_DATASIZE (16)

// args: data (16-bit)

static __inline HI_VOID hi_ext_system_sensor_max_resolution_width_write(HI_U16 data){
    IOWR_16DIRECT(0x12396, data);
}

static __inline HI_U16 hi_ext_system_sensor_max_resolution_width_read(HI_VOID){
    return IORD_16DIRECT(0x12396);
}



// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sensor_max_resolution_height
// ------------------------------------------------------------------------------ //
// height of sensor resolution 
// ------------------------------------------------------------------------------ //


#define HI_EXT_SYSTEM_SENSOR_MAX_RESOLUTION_HEIGHT_DEFAULT (720)
#define HI_EXT_SYSTEM_SENSOR_MAX_RESOLUTION_HEIGHT_DATASIZE (16)

// args: data (16-bit)

static __inline HI_VOID hi_ext_system_sensor_max_resolution_height_write(HI_U16 data){
    IOWR_16DIRECT(0x12398, data);
}

static __inline HI_U16 hi_ext_system_sensor_max_resolution_height_read(HI_VOID){
    return IORD_16DIRECT(0x12398);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sensor_max_resolution_fps
// ------------------------------------------------------------------------------ //
// fps of sensor output
// ------------------------------------------------------------------------------ //


#define HI_EXT_SYSTEM_SENSOR_MAX_RESOLUTION_FPS_DEFAULT (30)
#define HI_EXT_SYSTEM_SENSOR_MAX_RESOLUTION_FPS_DATASIZE (16)

// args: data (16-bit)

static __inline HI_VOID hi_ext_system_sensor_max_resolution_fps_write(HI_U16 data){
    IOWR_16DIRECT(0x1239a, data);
}

static __inline HI_U16 hi_ext_system_sensor_max_resolution_fps_read(HI_VOID){
    return IORD_16DIRECT(0x1239a);
}


// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_statistics_ctrl
// ------------------------------------------------------------------------------ //
// ISP statistics control
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_STATISTICS_DEFAULT  (0xfffffffd)
#define HI_EXT_SYSTEM_STATISTICS_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_ext_system_statistics_ctrl_read(HI_VOID)
{
    return  IORD_32DIRECT(0x1239c);
}

static __inline HI_VOID hi_ext_system_statistics_ctrl_write(HI_U32 data)
{
     IOWR_32DIRECT(0x1239c, data);
}

// ------------------------------------------------------------------------------ //
// Register: isp_ext_gamma_rgb_mem_array_data
// ------------------------------------------------------------------------------ //
// Please see other documentation for a description of the contents of this array.
// ------------------------------------------------------------------------------ //

#define HI_ISP_EXT_GAMMA_RGB_MEM_ARRAY_DATA_DEFAULT (0x0)
#define HI_ISP_EXT_GAMMA_RGB_MEM_ARRAY_DATA_DATASIZE (16)

// args: index (0-256), data (16-bit)
static __inline void hi_isp_ext_gamma_rgb_mem_array_data_write(HI_U16 index, HI_U16 data) {
    HI_U32 addr = 0x123a0 + (index << 1);
    IOWR_16DIRECT(addr, data);
}

static __inline HI_U16 hi_isp_ext_gamma_rgb_mem_array_data_read(HI_U16 index) {
    HI_U32 addr = 0x123a0 + (index << 1);
    return IORD_16DIRECT(addr);
}

//==============================================================//
//UVNR: 0x12400~0x124FF
//==============================================================//
//0x12400
static __inline HI_VOID hi_ext_system_uvnr_manu_mode_write(HI_U8 data) {
    IOWR_8DIRECT(0x12400, data&0x1);
}

static __inline HI_U8 hi_ext_system_uvnr_manu_mode_read(HI_VOID) {
    return (IORD_8DIRECT(0x12400)&0x1);
}

//0x12401
static __inline HI_VOID hi_ext_system_uvnr_enable_write(HI_U8 data) {
    IOWR_8DIRECT(0x12401, data&0x1);
}

static __inline HI_U8 hi_ext_system_uvnr_enable_read(HI_VOID) {
    return (IORD_8DIRECT(0x12401)&0x1);
}

//0x12404
#define HI_EXT_SYSTEM_MANUAL_UVNR_COLOR_CAST_WRITE_DEFAULT (0)

static __inline void hi_ext_system_manual_uvnr_color_cast_write(HI_U8 data) {
    IOWR_8DIRECT(0x12404, data);
}
static __inline HI_U8 hi_ext_system_manual_uvnr_color_cast_read(HI_VOID) {
    return (IORD_8DIRECT(0x12404));
}

//0x12408
#define HI_EXT_SYSTEM_MANUAL_UVNR_THRESHOLD_WRITE_DEFAULT (20)

static __inline void hi_ext_system_manual_uvnr_threshold_write(HI_U8 data) {
    IOWR_8DIRECT(0x12408, data);
}
static __inline HI_U8 hi_ext_system_manual_uvnr_threshold_read(HI_VOID) {
    return (IORD_8DIRECT(0x12408));
}

//0x1240c
#define HI_EXT_SYSTEM_MANUAL_UVNR_STRENGTH_WRITE_DEFAULT (34)

static __inline void hi_ext_system_manual_uvnr_strength_write(HI_U8 data) {
    IOWR_8DIRECT(0x1240c, data);
}
static __inline HI_U8 hi_ext_system_manual_uvnr_strength_read(HI_VOID) {
    return (IORD_8DIRECT(0x1240c));
}

//0x12410
static __inline void hi_ext_system_uvnr_color_cast_write(HI_U8 u8Index, HI_U8 data) {
    IOWR_8DIRECT((0x12410 + (u8Index*sizeof(HI_U8))), data);
}
static __inline HI_U8 hi_ext_system_uvnr_color_cast_read(HI_U8 u8Index) {
    return (IORD_8DIRECT(0x12410 + (u8Index*sizeof(HI_U8))));
}

//0x12420
static __inline void hi_ext_system_uvnr_threshold_write(HI_U8 u8Index, HI_U8 data) {
    IOWR_8DIRECT((0x12420 + (u8Index*sizeof(HI_U8))), data);
}
static __inline HI_U8 hi_ext_system_uvnr_threshold_read(HI_U8 u8Index) {
    return (IORD_8DIRECT(0x12420 + (u8Index*sizeof(HI_U8))));
}

//0x12430
static __inline void hi_ext_system_uvnr_strength_write(HI_U8 u8Index, HI_U8 data) {
    IOWR_8DIRECT((0x12430 + (u8Index*sizeof(HI_U8))), data);
}
static __inline HI_U8 hi_ext_system_uvnr_strength_read(HI_U8 u8Index) {
    return (IORD_8DIRECT(0x12430 + (u8Index*sizeof(HI_U8))));
}

// ------------------------------------------------------------------------------ //
// Register: isp_ext_system_gamma_change
// ------------------------------------------------------------------------------ //
// Gamma change config control
// ------------------------------------------------------------------------------ //

#define HI_ISP_EXT_SYSTEM_GAMMA_CHANGE_DEFAULT (0)
#define HI_ISP_EXT_SYSTEM_GAMMA_CHANGE_DATASIZE (1)

// args: data (1-bit)
static __inline void hi_isp_ext_system_gamma_change_write(HI_U8 data) {
    IOWR_8DIRECT(0x125a2, (data & 0x1));
}
static __inline HI_U32 hi_isp_ext_system_gamma_change_read(HI_VOID) {
    return (IORD_8DIRECT(0x125a2)& 0x1);
}



// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_black_level_00
// ------------------------------------------------------------------------------ //
// ISP black level 00
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_BLACK_LEVEL_00_DEFAULT  (0x0)
#define HI_EXT_SYSTEM_BLACK_LEVEL_00_DATASIZE (16)

// args: data (16-bit)
static __inline HI_U16 hi_ext_system_black_level_00_read(HI_VOID)
{
    return  IORD_16DIRECT(0x125a4);
}

static __inline HI_VOID hi_ext_system_black_level_00_write(HI_U16 data)
{
     IOWR_16DIRECT(0x125a4, data);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_black_level_01
// ------------------------------------------------------------------------------ //
// ISP black level 01
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_BLACK_LEVEL_01_DEFAULT  (0x0)
#define HI_EXT_SYSTEM_BLACK_LEVEL_01_DATASIZE (16)

// args: data (16-bit)
static __inline HI_U16 hi_ext_system_black_level_01_read(HI_VOID)
{
    return  IORD_16DIRECT(0x125a6);
}

static __inline HI_VOID hi_ext_system_black_level_01_write(HI_U16 data)
{
     IOWR_16DIRECT(0x125a6, data);
}


// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_black_level_10
// ------------------------------------------------------------------------------ //
// ISP black level 10
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_BLACK_LEVEL_10_DEFAULT  (0x0)
#define HI_EXT_SYSTEM_BLACK_LEVEL_10_DATASIZE (16)

// args: data (16-bit)
static __inline HI_U16 hi_ext_system_black_level_10_read(HI_VOID)
{
    return  IORD_16DIRECT(0x125a8);
}

static __inline HI_VOID hi_ext_system_black_level_10_write(HI_U16 data)
{
     IOWR_16DIRECT(0x125a8, data);
}


// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_black_level_11
// ------------------------------------------------------------------------------ //
// ISP black level 11
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_BLACK_LEVEL_11_DEFAULT  (0x0)
#define HI_EXT_SYSTEM_BLACK_LEVEL_11_DATASIZE (16)

// args: data (16-bit)
static __inline HI_U16 hi_ext_system_black_level_11_read(HI_VOID)
{
    return  IORD_16DIRECT(0x125aa);
}

static __inline HI_VOID hi_ext_system_black_level_11_write(HI_U16 data)
{
     IOWR_16DIRECT(0x125aa, data);
}


// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_black_level_change
// ------------------------------------------------------------------------------ //
// ISP black level change 
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_BLACK_LEVEL_CHANGE_DEFAULT  (0x0)
#define HI_EXT_SYSTEM_BLACK_LEVEL_CHANGE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_U8 hi_ext_system_black_level_change_read(HI_VOID)
{
    return (HI_U8)(IORD_16DIRECT(0x125ac) & 0x0001);
}

static __inline HI_VOID hi_ext_system_black_level_change_write(HI_U8 data)
{
    HI_U16 u32Current = IORD_16DIRECT(0x125ac);
    IOWR_16DIRECT(0x125ac, ((HI_U16) (data & 0x0001)) | (u32Current & 0xFFFE));
}


// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_sensor_iso
// ------------------------------------------------------------------------------ //
// Sensor ISO 
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_SENSOR_ISO_DEFAULT  (0x0)
#define HI_EXT_SYSTEM_SENSOR_ISO_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_ext_system_sensor_iso_read(HI_VOID)
{
    return  IORD_32DIRECT(0x125b0);
}

static __inline HI_VOID hi_ext_system_sensor_iso_write(HI_U32 data)
{
     IOWR_32DIRECT(0x125b0, data);
}

//==============================================================//
//DPC: 0x12600~0x127FF
//==============================================================//
// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dpc_static_cor_enable
// ------------------------------------------------------------------------------ //
// defect pixel static correction enable
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_DPC_STATIC_COR_ENABLE_DEFAULT (0x01)
#define HI_EXT_SYSTEM_DPC_STATIC_COR_ENABLE_DATASIZE (1)

// 0x12600 ~ dpc_static_cor_enable
static __inline HI_VOID hi_ext_system_dpc_static_cor_enable_write(HI_U8 data) {
    IOWR_8DIRECT(0x12600, data&0x1);
}
static __inline HI_U8 hi_ext_system_dpc_static_cor_enable_read(HI_VOID) {
    return  (IORD_8DIRECT(0x12600) & 0x1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dpc_static_calib_enable
// ------------------------------------------------------------------------------ //
// defect pixel static correction enable
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_DPC_STATIC_CALIB_ENABLE_DEFAULT (0x0)
#define HI_EXT_SYSTEM_DPC_STATIC_CALIB_ENABLE_DATASIZE (1)

// 0x12601 ~ dpc_static_calib_enable
static __inline HI_VOID hi_ext_system_dpc_static_calib_enable_write(HI_U8 data) {
    IOWR_8DIRECT(0x12601, data&0x1);
}
static __inline HI_U8 hi_ext_system_dpc_static_calib_enable_read(HI_VOID) {
    return  (IORD_8DIRECT(0x12601) & 0x1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dpc_defect_type
// ------------------------------------------------------------------------------ //
// ISP_STATIC_DP_BRIGHT:0x0,ISP_STATIC_DP_DARK:0x1
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_DPC_STATIC_DEFECT_TYPE_DEFAULT (0x0)
#define HI_EXT_SYSTEM_DPC_STATIC_DEFECT_TYPE_DATASIZE (1)

// 0x12602 ~ enStaticDPType
static __inline HI_VOID hi_ext_system_dpc_static_defect_type_write(HI_U8 data) {
    IOWR_8DIRECT(0x12602, data&0x1);
}
static __inline HI_U8 hi_ext_system_dpc_static_defect_type_read(HI_VOID) {
    return  (IORD_8DIRECT(0x12602) & 0x1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dpc_start_thresh
// ------------------------------------------------------------------------------ //
// define the start thresh of defect pixel calibration
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_DPC_START_THRESH_DEFAULT (0x3)
#define HI_EXT_SYSTEM_DPC_START_THRESH_DATASIZE (8)

// 0x12603 ~ u8StartThresh
static __inline HI_VOID hi_ext_system_dpc_start_thresh_write(HI_U8 data) {
    IOWR_8DIRECT((0x12603), data);
}
static __inline HI_U8 hi_ext_system_dpc_start_thresh_read(HI_VOID) {
    return IORD_8DIRECT(0x12603);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dpc_finish_thresh
// ------------------------------------------------------------------------------ //
//  the finish thresh of defect pixel calibration
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_DPC_FINISH_THRESH_DATASIZE (8)

// 0x12604 ~ u8FinishThresh
static __inline HI_VOID hi_ext_system_dpc_finish_thresh_write(HI_U8 data) {
    IOWR_8DIRECT(0x12604, data);
}
static __inline HI_U8 hi_ext_system_dpc_finish_thresh_read(HI_VOID) {
    return IORD_8DIRECT(0x12604);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dpc_trigger_status
// ------------------------------------------------------------------------------ //
// the static bad pixel trigger status 
// 0:Initial status
// 1:Finished
// 2:time out
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_DPC_TRIGGER_STATUS_DEFAULT (0x0)
#define HI_EXT_SYSTEM_DPC_TRIGGER_STATUS_DATASIZE (2)

// 0x12605 ~ enStatus
static __inline HI_VOID hi_ext_system_dpc_trigger_status_write(HI_U8 data) {
    IOWR_8DIRECT(0x12605, data);
}
static __inline HI_U8 hi_ext_system_dpc_trigger_status_read(HI_VOID) {
    return IORD_8DIRECT(0x12605);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dpc_static_dp_show
// ------------------------------------------------------------------------------ //
// highlight static defect pixel
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_DPC_STATIC_DP_SHOW_DEFAULT (0x0)
#define HI_EXT_SYSTEM_DPC_STATIC_DP_SHOW_DATASIZE (1)

// 0x12606 ~ bShow
static __inline HI_VOID hi_ext_system_dpc_static_dp_show_write(HI_U8 data) {
    IOWR_8DIRECT(0x12606, data&0x1);
}
static __inline HI_U8 hi_ext_system_dpc_static_dp_show_read(HI_VOID) {
    return  (IORD_8DIRECT(0x12606) & 0x1);
}


// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dpc_dynamic_cor_enable
// ------------------------------------------------------------------------------ //
// defect pixel dynamic correction enable
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_DPC_DYNAMIC_COR_ENABLE_DEFAULT (0x01)
#define HI_EXT_SYSTEM_DPC_DYNAMIC_COR_ENABLE_DATASIZE (1)

// 0x12607 ~ dynamic_cor_enable
static __inline HI_VOID hi_ext_system_dpc_dynamic_cor_enable_write(HI_U8 data) {
    IOWR_8DIRECT(0x12607, data&0x1);
}
static __inline HI_U8 hi_ext_system_dpc_dynamic_cor_enable_read(HI_VOID) {
    return  (IORD_8DIRECT(0x12607) & 0x1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dpc_count_max
// ------------------------------------------------------------------------------ //
// bad pixel calibration  the maxmum bad pixel count
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_DPC_COUNT_MAX_DEFAULT (0x400)
#define HI_EXT_SYSTEM_DPC_COUNT_MAX_DATASIZE (16)

// 0x12608 ~ 0x12609 u16CountMax
static __inline HI_VOID hi_ext_system_dpc_count_max_write(HI_U16 data) {
    IOWR_16DIRECT(0x12608, data);
}
static __inline HI_U16 hi_ext_system_dpc_count_max_read(HI_VOID) {
    return IORD_16DIRECT(0x12608);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dpc_count_min
// ------------------------------------------------------------------------------ //
// bad pixel calibration  the minmum bad pixel count
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_DPC_COUNT_MIN_DEFAULT (0x1)
#define HI_EXT_SYSTEM_DPC_COUNT_MIN_DATASIZE (16)

// 0x1260a ~0x1260b u16CountMin
static __inline HI_VOID hi_ext_system_dpc_count_min_write(HI_U16 data) {
    IOWR_16DIRECT(0x1260a, data);
}
static __inline HI_U16 hi_ext_system_dpc_count_min_read(HI_VOID) {
    return IORD_16DIRECT(0x1260a);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dpc_trigger_time
// ------------------------------------------------------------------------------ //
// the bad pixel trigger frame cnt of time out 
// ------------------------------------------------------------------------------ //

#define HI_EXT_SYSTEM_DPC_TRIGGER_TIME_DEFAULT (0x640)
#define HI_EXT_SYSTEM_DPC_TRIGGER_TIME_DATASIZE (16)

// 0x1260c ~ 0x1260d enStatus
static __inline HI_VOID hi_ext_system_dpc_trigger_time_write(HI_U16 data) {
    IOWR_16DIRECT(0x1260c, data);
}
static __inline HI_U16 hi_ext_system_dpc_trigger_time_read(HI_VOID) {
    return IORD_16DIRECT(0x1260c) ;
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dpc_dynamic_manual_enable
// ------------------------------------------------------------------------------ //
// 0:auto;
//1:manual
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_DPC_DYNAMIC_MANUAL_ENABLE_DEFAULT (0x0)
#define HI_EXT_SYSTEM_DPC_DYNAMIC_MANUAL_ENABLE_DATASIZE (1)

// 0x1260e ~ dynamic_manual_enable
static __inline HI_VOID hi_ext_system_dpc_dynamic_manual_enable_write(HI_U8 data) {
    IOWR_8DIRECT(0x1260e, data&0x1);
}
static __inline HI_U8 hi_ext_system_dpc_dynamic_manual_enable_read(HI_VOID) {
    return  (IORD_8DIRECT(0x1260e) & 0x1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dpc_dynamic_strength_table
// ------------------------------------------------------------------------------ //
// 0x12610 ~ 0x1262f dpc_dynamic_strength_table

static __inline HI_VOID hi_ext_system_dpc_dynamic_strength_table_write(HI_U8 u8Index, HI_U16 data){
	IOWR_16DIRECT(0x12610+u8Index*sizeof(HI_U16), data);
}

static __inline HI_U16 hi_ext_system_dpc_dynamic_strength_table_read(HI_U8 u8Index){
	return IORD_16DIRECT(0x12610 + u8Index*sizeof(HI_U16));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dpc_dynamic_blend_ratio_table
// ------------------------------------------------------------------------------ //
// 0x12630 ~ 0x1264f dpc_blend_ratio_table

static __inline HI_VOID hi_ext_system_dpc_dynamic_blend_ratio_table_write(HI_U8 u8Index, HI_U16 data){
	IOWR_16DIRECT(0x12630+u8Index*sizeof(HI_U16), data);
}

static __inline HI_U16 hi_ext_system_dpc_dynamic_blend_ratio_table_read(HI_U8 u8Index){
	return IORD_16DIRECT(0x12630 + u8Index*sizeof(HI_U16));
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dpc_dynamic_strength
// ------------------------------------------------------------------------------ //
// 0x12650 ~ 0x12651 dpc_dynamic_strength

#define HI_EXT_SYSTEM_DPC_DYNAMIC_STRENGTH_DEFAULT (0x0)
#define HI_EXT_SYSTEM_DPC_DYNAMIC_STRENGTH_DATASIZE (2)

static __inline HI_VOID hi_ext_system_dpc_dynamic_strength_write( HI_U16 data){
	IOWR_16DIRECT(0x12650, data);
}

static __inline HI_U16 hi_ext_system_dpc_dynamic_strength_read(HI_VOID){
	return IORD_16DIRECT(0x12650);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dpc_dynamic_blend_ratio_table
// ------------------------------------------------------------------------------ //
// 0x12652 ~ 0x12653 dpc_blend_ratio

#define HI_EXT_SYSTEM_DPC_DYNAMIC_BLEND_RATIO_DEFAULT (0x0)
#define HI_EXT_SYSTEM_DPC_DYNAMIC_BLEND_RATIO_DATASIZE (9)
static __inline HI_VOID hi_ext_system_dpc_dynamic_blend_ratio_write( HI_U16 data){
	IOWR_16DIRECT(0x12652, data);
}

static __inline HI_U16 hi_ext_system_dpc_dynamic_blend_ratio_read(HI_VOID){
	return IORD_16DIRECT(0x12652);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dpc_manual_mode
// ------------------------------------------------------------------------------ //
// 0x12654 ~dpc_manual_mode
static __inline HI_VOID hi_ext_system_dpc_manual_mode_write(HI_U8 data) {
    IOWR_8DIRECT(0x12654, data&0x1);
}
static __inline HI_U8 hi_ext_system_dpc_manual_mode_read(HI_VOID) {
    return (IORD_8DIRECT(0x12654) & 0x1);
}

//==============================================================//
//GE: 0x12700~0x127FF
//==============================================================//
// 0x12700 ~ 0x1271f ge_strength
static __inline HI_VOID hi_ext_system_ge_strength_write(HI_U8 u8Index, HI_U16 data){
    IOWR_16DIRECT(0x12700+u8Index*sizeof(HI_U16), data);
}

static __inline HI_U16 hi_ext_system_ge_strength_read(HI_U8 u8Index){
    return IORD_16DIRECT(0x12700 + u8Index*sizeof(HI_U16));
}

// 0x12720 ~ 0x12721 ge_threshold
static __inline HI_VOID hi_ext_system_ge_threshold_write( HI_U16 data){
	IOWR_16DIRECT(0x12720, data);
}

static __inline HI_U16 hi_ext_system_ge_threshold_read(HI_VOID){
	return IORD_16DIRECT(0x12720);
}

// 0x12722 ~ 0x12723 ge_slope
static __inline HI_VOID hi_ext_system_ge_slope_write( HI_U16 data){
	IOWR_16DIRECT(0x12722, data);
}

static __inline HI_U16 hi_ext_system_ge_slope_read(HI_VOID){
	return IORD_16DIRECT(0x12722);
}

// 0x12724 ~ 0x12725 ge_sensitivity
static __inline HI_VOID hi_ext_system_ge_sensitivity_write( HI_U16 data){
	IOWR_16DIRECT(0x12724, data);
}

static __inline HI_U16 hi_ext_system_ge_sensitivity_read(HI_VOID){
	return IORD_16DIRECT(0x12724);
}

// 0x12726 ~ 0x12727 ge_sensithreshold
static __inline HI_VOID hi_ext_system_ge_sensithreshold_write( HI_U16 data){
	IOWR_16DIRECT(0x12726, data);
}

static __inline HI_U16 hi_ext_system_ge_sensithreshold_read(HI_VOID){
	return IORD_16DIRECT(0x12726);
}

// 0x12728 ~ 0x12728 ge_enable

static __inline HI_VOID hi_ext_system_ge_enable_write( HI_U8 data){
	IOWR_8DIRECT(0x12728, data&0x1);
}

static __inline HI_U8 hi_ext_system_ge_enable_read(HI_VOID){
	return (IORD_8DIRECT(0x12728)& 0x1);
}



//==============================================================//
//DEMOSAIC: 0x12800~0x128FF
//==============================================================//
// 0x12800 ~ fcr_manu_mode
static __inline HI_VOID hi_ext_system_fcr_manu_mode_write(HI_U8 data) {
    IOWR_8DIRECT(0x12800, data);
}
static __inline HI_U8 hi_ext_system_fcr_manu_mode_read(HI_VOID) {
    return (IORD_8DIRECT(0x12800));
}

// 0x12801 ~ fcr_enable
static __inline HI_VOID hi_ext_system_fcr_enable_write(HI_U8 data) {
    IOWR_8DIRECT(0x12801, data&0x1);
}
static __inline HI_U8 hi_ext_system_fcr_enable_read(HI_VOID) {
    return (IORD_8DIRECT(0x12801) & 0x1);
}

// 0x12802 ~ fcr_strength
#define HI_EXT_SYSTEM_MANUAL_FCR_STRENGTH_WRITE_DEFAULT (8)
static __inline HI_VOID hi_ext_system_manual_fcr_strength_write(HI_U8 data) {
    IOWR_8DIRECT(0x12802, data);
}
static __inline HI_U8 hi_ext_system_manual_fcr_strength_read(HI_VOID) {
    return (IORD_8DIRECT(0x12802));
}

// 0x12803 ~ demosaic_vhSlope
static __inline HI_VOID hi_ext_system_demosaic_vhSlope_write(HI_U8 data) {
    IOWR_8DIRECT(0x12803, data);
}
static __inline HI_U8 hi_ext_system_demosaic_vhSlope_read(HI_VOID) {
    return (IORD_8DIRECT(0x12803));
}

// 0x12804 ~ 0x12805 demosaic_uuSlope
static __inline HI_VOID hi_ext_system_demosaic_uuSlope_write(HI_U16 data) {
    IOWR_16DIRECT(0x12804, data);
}
static __inline HI_U16 hi_ext_system_demosaic_uuSlope_read(HI_VOID) {
    return (IORD_16DIRECT(0x12804));
}

// 0x12806 ~ demosaic_vhLimit
static __inline HI_VOID hi_ext_system_demosaic_vhLimit_write(HI_U8 data) {
    IOWR_8DIRECT(0x12806, data);
}
static __inline HI_U8 hi_ext_system_demosaic_vhLimit_read(HI_VOID) {
    return (IORD_8DIRECT(0x12806));
}

// 0x12807 ~ demosaic_vhOffset
static __inline HI_VOID hi_ext_system_demosaic_vhOffset_write(HI_U8 data) {
    IOWR_8DIRECT(0x12807, data);
}
static __inline HI_U8 hi_ext_system_demosaic_vhOffset_read(HI_VOID) {
    return (IORD_8DIRECT(0x12807));
}

// 0x12808 ~ fcr_threshold
#define HI_EXT_SYSTEM_MANUAL_FCR_THRESHOLD_WRITE_DEFAULT (24)
static __inline HI_VOID hi_ext_system_manual_fcr_threshold_write(HI_U8 data) {
    IOWR_8DIRECT(0x12808, data);
}
static __inline HI_U8 hi_ext_system_manual_fcr_threshold_read(HI_VOID) {
    return (IORD_8DIRECT(0x12808));
}

// 0x12810 ~ 0x1281f fcr_strength
static __inline HI_VOID hi_ext_system_fcr_auto_strength_write(HI_U8 u8Index, HI_U8 data){
	IOWR_8DIRECT(0x12810+u8Index*sizeof(HI_U8), data);
}

static __inline HI_U8 hi_ext_system_fcr_auto_strength_read(HI_U8 u8Index){
	return IORD_8DIRECT(0x12810 + u8Index*sizeof(HI_U8));
}


// 0x12820 ~ 0x1283f demosaic_npOffset
static __inline HI_VOID hi_ext_system_demosaic_npOffset_write(HI_U8 u8Index, HI_U16 data){
    IOWR_16DIRECT(0x12820+u8Index*sizeof(HI_U16), data);
}

static __inline HI_U16 hi_ext_system_demosaic_npOffset_read(HI_U8 u8Index){
    return IORD_16DIRECT(0x12820 + u8Index*sizeof(HI_U16));
}

// 0x12840 ~ 0x1284f fcr_threshold
static __inline HI_VOID hi_ext_system_fcr_auto_threshold_write(HI_U8 u8Index, HI_U8 data){
	IOWR_8DIRECT(0x12840+u8Index*sizeof(HI_U8), data);
}

static __inline HI_U8 hi_ext_system_fcr_auto_threshold_read(HI_U8 u8Index){
	return IORD_8DIRECT(0x12840 + u8Index*sizeof(HI_U8));
}


//==============================================================//
//ISP_COMMON_EXT: 0x12900~0x129FF
//==============================================================//
//ISP_PIXEL_FORMAT
#define HI_ISP_PIXEL_FORMAT_420_DEFAULT (0)
#define HI_ISP_PIXEL_FORMAT_422_DEFAULT (1)
static __inline HI_VOID hi_ext_system_isp_pixel_format_write(HI_U8 data)
{
	 IOWR_8DIRECT(0x12900, data);
}

static __inline HI_U8 hi_ext_system_isp_pixel_format_read(HI_VOID)
{
	return	IORD_8DIRECT(0x12900);
}

static __inline HI_VOID hi_ext_system_isp_bit_bypass_write(HI_U32 data)
{
	 IOWR_32DIRECT(0x12904, data);
}

static __inline HI_U32 hi_ext_system_isp_bit_bypass_read(HI_VOID)
{
	return	IORD_32DIRECT(0x12904);
}


//args: index (0 ~ 288), data (32-bit)

//shading gain 0x13000-0x13480
static __inline HI_VOID hi_ext_system_isp_mesh_shading_r_gain_write(HI_U16 index, HI_U32 data)
{
   HI_U32 addr = 0x13000 + (index << 2);
   IOWR_32DIRECT(addr,data);
}

static __inline HI_U32 hi_ext_system_isp_mesh_shading_r_gain_read(HI_U16 index)
{
	HI_U32 addr = 0x13000 + (index << 2);
	return	IORD_32DIRECT(addr);
}

//shading gain 0x13484-0x13904
static __inline HI_VOID hi_ext_system_isp_mesh_shading_gr_gain_write(HI_U16 index, HI_U32 data)
{
   HI_U32 addr = 0x13484 + (index << 2);
   IOWR_32DIRECT(addr,data);
}

static __inline HI_U32 hi_ext_system_isp_mesh_shading_gr_gain_read(HI_U16 index)
{
	HI_U32 addr = 0x13484 + (index << 2);
	return	IORD_32DIRECT(addr);
}

//shading gain 0x13908-0x13D88
static __inline HI_VOID hi_ext_system_isp_mesh_shading_gb_gain_write(HI_U16 index, HI_U32 data)
{
   HI_U32 addr = 0x13908 + (index << 2);
   IOWR_32DIRECT(addr,data);
}

static __inline HI_U32 hi_ext_system_isp_mesh_shading_gb_gain_read(HI_U16 index)
{
	HI_U32 addr = 0x13908 + (index << 2);
	return	IORD_32DIRECT(addr);
}

//shading gain 0x13D8C-0x1420C
static __inline HI_VOID hi_ext_system_isp_mesh_shading_b_gain_write(HI_U16 index, HI_U32 data)
{
   HI_U32 addr = 0x13D8C + (index << 2);
   IOWR_32DIRECT(addr,data);
}

static __inline HI_U32 hi_ext_system_isp_mesh_shading_b_gain_read(HI_U16 index)
{
	HI_U32 addr = 0x13D8C + (index << 2);
	return	IORD_32DIRECT(addr);
}

//shading gain 0x14260 - 0x146E0 //Do we need such big space?
static __inline HI_VOID hi_ext_system_isp_mesh_shading_noise_control_write(HI_U16 index, HI_U32 data)
{
   HI_U32 addr = 0x14260 + (index << 2);  
   IOWR_32DIRECT(addr, data);
}

static __inline HI_U32 hi_ext_system_isp_mesh_shading_noise_control_read(HI_U16 index)
{
    HI_U32 addr = 0x14260 + (index << 2);
    return IORD_32DIRECT(addr);
}

//xgrid 0x146F0 - 0x1470C
static __inline HI_VOID hi_ext_system_isp_mesh_shading_xgrid_write(HI_U16 index, HI_U32 data)
{
    HI_U32 addr = 0x146f0 + (index << 2);  
    IOWR_32DIRECT(addr, data);

}

static __inline HI_U32 hi_ext_system_isp_mesh_shading_xgrid_read(HI_U16 index)
{
    HI_U32 addr = 0x146f0 + (index << 2);
    return IORD_32DIRECT(addr);
}


//ygrid 0x14710 - 0x1472C
static __inline HI_VOID hi_ext_system_isp_mesh_shading_ygrid_write(HI_U16 index, HI_U32 data)
{
    HI_U32 addr = 0x14710 + (index << 2);  
    IOWR_32DIRECT(addr, data);

}

static __inline HI_U32 hi_ext_system_isp_mesh_shading_ygrid_read(HI_U16 index)
{
    HI_U32 addr = 0x14710 + (index << 2);
    return IORD_32DIRECT(addr);
}


#define HI_EXT_SYSTEM_ISP_MESH_SHADING_UPDATA_DEFAULT (0)
#define HI_EXT_SYSTEM_ISP_MESH_SHADING_UPDATA_DATASIZE (8)

// args: data (8-bit) 0x14210
static __inline HI_VOID hi_ext_system_isp_mesh_shading_updata_write(HI_U8 data) {
    IOWR_8DIRECT(0x14210, data);
}
static __inline HI_U8 hi_ext_system_isp_mesh_shading_updata_read(HI_VOID) {
    return IORD_8DIRECT(0x14210);
}

#define HI_EXT_SYSTEM_ISP_MESH_SHADING_MANU_MODE_DEFAULT (0)
#define HI_EXT_SYSTEM_ISP_MESH_SHADING_MANU_MODE_DATASIZE (8)

// args: data (8-bit) 0x14211
static __inline HI_VOID hi_ext_system_isp_mesh_shading_manu_mode_write(HI_U8 data) {
    IOWR_8DIRECT(0x14211, data);
}
static __inline HI_U8 hi_ext_system_isp_mesh_shading_manu_mode_read(HI_VOID) {
    return IORD_8DIRECT(0x14211);
}

static __inline HI_VOID hi_ext_system_isp_rgbir_removel_en_write(HI_U8 data){
    HI_U32 u32Current = IORD_32DIRECT(0x14220);
    IOWR_32DIRECT(0x14220, (u32Current & 0xeffff) | ((data & 0x1) << 16));
}

static __inline HI_U32 hi_ext_system_isp_rgbir_removel_en_read(HI_VOID) {
    return ((IORD_32DIRECT(0x14220) & 0x10000) >> 16);
}


/*****************************************************
ISP_RGBIR_SCALE_B:        RGBIR scale b coefficient
0x205a3170:               31:30 	reserved	
                          29:20     b_b
                          19:10     b_g
                          9:0		b_r
*****************************************************/
static __inline HI_VOID hi_ext_system_isp_rgbir_scale_b_b_write(HI_U16 data){
    HI_U32 u32Current = IORD_32DIRECT(0x14230);
    IOWR_32DIRECT(0x14230, (u32Current & 0xfffff) | ((data & 0x3ff) << 20));
}
    
static __inline HI_U32 hi_ext_system_isp_rgbir_scale_b_b_read(HI_VOID) {
    HI_U32 u32Tmp;
    
    u32Tmp = ((IORD_32DIRECT(0x14230) & 0x3ff00000) >> 20);
    if (u32Tmp & 0x200)    
    {        
        u32Tmp |= 0xFFFFFC00;    
    }   

    return u32Tmp;
}

static __inline HI_VOID hi_ext_system_isp_rgbir_scale_b_g_write(HI_U16 data){
    HI_U32 u32Current = IORD_32DIRECT(0x14230);
    IOWR_32DIRECT(0x14230, (u32Current & 0x3ff003ff) | ((data & 0x3ff) << 10));
}
    
static __inline HI_U32 hi_ext_system_isp_rgbir_scale_b_g_read(HI_VOID) {
    HI_U32 u32Tmp;

    u32Tmp = (IORD_32DIRECT(0x14230) & 0xffc00) >> 10;
    if (u32Tmp & 0x200)    
    {        
        u32Tmp |= 0xFFFFFC00;    
    }   

    return u32Tmp;
}

static __inline HI_VOID hi_ext_system_isp_rgbir_scale_b_r_write(HI_U16 data){
    HI_U32 u32Current = IORD_32DIRECT(0x14230);
    IOWR_32DIRECT(0x14230, (u32Current & 0x3ffffc00) | (data & 0x3ff));
}
    
static __inline HI_U32 hi_ext_system_isp_rgbir_scale_b_r_read(HI_VOID) {
    HI_U32 u32Tmp;

    u32Tmp = (IORD_32DIRECT(0x14230) & 0x3ff);
    if (u32Tmp & 0x200)    
    {        
        u32Tmp |= 0xFFFFFC00;    
    }   

    return u32Tmp;
}

/*****************************************************
ISP_RGBIR_SCALE_G:        RGBIR scale g coefficient
0x205a3174:               31:30 	reserved	
                          29:20     g_b
                          19:10     g_g
                          9:0		g_r
*****************************************************/
static __inline HI_VOID hi_ext_system_isp_rgbir_scale_g_b_write(HI_U16 data){
    HI_U32 u32Current = IORD_32DIRECT(0x14234);
    IOWR_32DIRECT(0x14234, (u32Current & 0xfffff) | ((data & 0x3ff) << 20));
}
    
static __inline HI_U32 hi_ext_system_isp_rgbir_scale_g_b_read(HI_VOID) {
    HI_U32 u32Tmp;

    u32Tmp = ((IORD_32DIRECT(0x14234) & 0x3ff00000) >> 20);
    if (u32Tmp & 0x200)    
    {        
        u32Tmp |= 0xFFFFFC00;    
    }   

    return u32Tmp;
}

static __inline HI_VOID hi_ext_system_isp_rgbir_scale_g_g_write(HI_U16 data){
    HI_U32 u32Current = IORD_32DIRECT(0x14234);
    IOWR_32DIRECT(0x14234, (u32Current & 0x3ff003ff) | ((data & 0x3ff) << 10));
}
    
static __inline HI_U32 hi_ext_system_isp_rgbir_scale_g_g_read(HI_VOID) {
    HI_U32 u32Tmp;

    u32Tmp = ((IORD_32DIRECT(0x14234) & 0xffc00) >> 10);
    if (u32Tmp & 0x200)    
    {        
        u32Tmp |= 0xFFFFFC00;    
    }   

    return u32Tmp;
}

static __inline HI_VOID hi_ext_system_isp_rgbir_scale_g_r_write(HI_U16 data){
    HI_U32 u32Current = IORD_32DIRECT(0x14234);
    IOWR_32DIRECT(0x14234, (u32Current & 0x3ffffc00) | (data & 0x3ff));
}
    
static __inline HI_U32 hi_ext_system_isp_rgbir_scale_g_r_read(HI_VOID) {
    HI_U32 u32Tmp;

    u32Tmp = (IORD_32DIRECT(0x14234) & 0x3ff);
    if (u32Tmp & 0x200)    
    {        
        u32Tmp |= 0xFFFFFC00;    
    }   

    return u32Tmp;
}

/*****************************************************
ISP_RGBIR_SCALE_R:        RGBIR scale r coefficient
0x205a3178:               31:30 	reserved	
                          29:20     r_b
                          19:10     r_g
                          9:0		r_r
*****************************************************/
static __inline HI_VOID hi_ext_system_isp_rgbir_scale_r_b_write(HI_U16 data){
    HI_U32 u32Current = IORD_32DIRECT(0x14238);
    IOWR_32DIRECT(0x14238, (u32Current & 0xfffff) | ((data & 0x3ff) << 20));
}
    
static __inline HI_U32 hi_ext_system_isp_rgbir_scale_r_b_read(HI_VOID) {
    HI_U32 u32Tmp;

    u32Tmp = ((IORD_32DIRECT(0x14238) & 0x3ff00000) >> 20);
    if (u32Tmp & 0x200)    
    {        
        u32Tmp |= 0xFFFFFC00;    
    }   

    return u32Tmp;
}

static __inline HI_VOID hi_ext_system_isp_rgbir_scale_r_g_write(HI_U16 data){
    HI_U32 u32Current = IORD_32DIRECT(0x14238);
    IOWR_32DIRECT(0x14238, (u32Current & 0x3ff003ff) | ((data & 0x3ff) << 10));
}
    
static __inline HI_U32 hi_ext_system_isp_rgbir_scale_r_g_read(HI_VOID) {
    HI_U32 u32Tmp;

    u32Tmp = ((IORD_32DIRECT(0x14238) & 0xffc00) >> 10);
    if (u32Tmp & 0x200)    
    {        
        u32Tmp |= 0xFFFFFC00;    
    }   

    return u32Tmp;
}

static __inline HI_VOID hi_ext_system_isp_rgbir_scale_r_r_write(HI_U16 data){
    HI_U32 u32Current = IORD_32DIRECT(0x14238);
    IOWR_32DIRECT(0x14238, (u32Current & 0x3ffffc00) | (data & 0x3ff));
}
    
static __inline HI_U32 hi_ext_system_isp_rgbir_scale_r_r_read(HI_VOID) {
    HI_U32 u32Tmp;

    u32Tmp = (IORD_32DIRECT(0x14238) & 0x3ff);
    if (u32Tmp & 0x200)    
    {        
        u32Tmp |= 0xFFFFFC00;    
    }   

    return u32Tmp;
}

/*****************************************************
ISP_RGBIR_SCALE_IR:        RGBIR scale ir coefficient
0x205a317c:               31:30 	reserved	
                          29:20     ir_b
                          19:10     ir_g
                          9:0		ir_r
*****************************************************/
static __inline HI_VOID hi_ext_system_isp_rgbir_scale_ir_b_write(HI_U16 data){
    HI_U32 u32Current = IORD_32DIRECT(0x1423c);
    IOWR_32DIRECT(0x1423c, (u32Current & 0xfffff) | ((data & 0x3ff) << 20));
}
    
static __inline HI_U32 hi_ext_system_isp_rgbir_scale_ir_b_read(HI_VOID) {
    HI_U32 u32Tmp;

    u32Tmp = ((IORD_32DIRECT(0x1423c) & 0x3ff00000) >> 20);
    if (u32Tmp & 0x200)    
    {        
        u32Tmp |= 0xFFFFFC00;    
    }   

    return u32Tmp;
}

static __inline HI_VOID hi_ext_system_isp_rgbir_scale_ir_g_write(HI_U16 data){
    HI_U32 u32Current = IORD_32DIRECT(0x1423c);
    IOWR_32DIRECT(0x1423c, (u32Current & 0x3ff003ff) | ((data & 0x3ff) << 10));
}
    
static __inline HI_U32 hi_ext_system_isp_rgbir_scale_ir_g_read(HI_VOID) {
    HI_U32 u32Tmp;

    u32Tmp = ((IORD_32DIRECT(0x1423c) & 0xffc00) >> 10);
    if (u32Tmp & 0x200)    
    {        
        u32Tmp |= 0xFFFFFC00;    
    }   

    return u32Tmp;
}

static __inline HI_VOID hi_ext_system_isp_rgbir_scale_ir_r_write(HI_U16 data){
    HI_U32 u32Current = IORD_32DIRECT(0x1423c);
    IOWR_32DIRECT(0x1423c, (u32Current & 0x3ffffc00) | (data & 0x3ff));
}
    
static __inline HI_U32 hi_ext_system_isp_rgbir_scale_ir_r_read(HI_VOID) {
    HI_U32 u32Tmp;
    
    u32Tmp = (IORD_32DIRECT(0x1423c) & 0x3ff);
    if (u32Tmp & 0x200)    
    {        
        u32Tmp |= 0xFFFFFC00;    
    }   

    return u32Tmp;
}

/*****************************************************
ISP_RGBIR_SCALE_C:        RGBIR scale c coefficient
0x205a3180:               31:30 	reserved	
                          29:20     b_c
                          19:10     g_c
                          9:0		r_c
*****************************************************/
static __inline HI_VOID hi_ext_system_isp_rgbir_scale_c_b_write(HI_U16 data){
    HI_U32 u32Current = IORD_32DIRECT(0x14240);
    IOWR_32DIRECT(0x14240, (u32Current & 0xfffff) | ((data & 0x3ff) << 20));
}
    
static __inline HI_U32 hi_ext_system_isp_rgbir_scale_c_b_read(HI_VOID) {
    HI_U32 u32Tmp;

    u32Tmp = ((IORD_32DIRECT(0x14240) & 0x3ff00000) >> 20);
    if (u32Tmp & 0x200)    
    {        
        u32Tmp |= 0xFFFFFC00;    
    }   

    return u32Tmp;
}

static __inline HI_VOID hi_ext_system_isp_rgbir_scale_c_g_write(HI_U16 data){
    HI_U32 u32Current = IORD_32DIRECT(0x14240);
    IOWR_32DIRECT(0x14240, (u32Current & 0x3ff003ff) | ((data & 0x3ff) << 10));
}
    
static __inline HI_U32 hi_ext_system_isp_rgbir_scale_c_g_read(HI_VOID) {
    HI_U32 u32Tmp;

    u32Tmp = ((IORD_32DIRECT(0x14240) & 0xffc00) >> 10);
    if (u32Tmp & 0x200)    
    {        
        u32Tmp |= 0xFFFFFC00;    
    }   

    return u32Tmp;
}

static __inline HI_VOID hi_ext_system_isp_rgbir_scale_c_r_write(HI_U16 data){
    HI_U32 u32Current = IORD_32DIRECT(0x14240);
    IOWR_32DIRECT(0x14240, (u32Current & 0x3ffffc00) | (data & 0x3ff));
}
    
static __inline HI_U32 hi_ext_system_isp_rgbir_scale_c_r_read(HI_VOID) {
    HI_U32 u32Tmp;

    u32Tmp = (IORD_32DIRECT(0x14240) & 0x3ff);
    if (u32Tmp & 0x200)    
    {        
        u32Tmp |= 0xFFFFFC00;    
    }   

    return u32Tmp;
}

static __inline HI_VOID isp_ext_system_fpn_bCompressed_write(HI_U16 data){

    IOWR_16DIRECT(0x14250, data);
}
static __inline HI_U16 isp_ext_system_fpn_bCompressed_read(HI_VOID)
{
    return IORD_16DIRECT(0x14250);
}

static __inline void isp_ext_system_fpn_mode_write(HI_U16 data)
{
     IOWR_16DIRECT(0x14252, data);
}

static __inline HI_U8 isp_ext_system_fpn_mode_read(HI_VOID)
{
     return IORD_16DIRECT(0x14252);
}

// args: data (1-bit)

#define HI_EXT_SYSTEM_DEHAZE_DEBUG_ENABLE_DEFAULT (0x0)
#define HI_EXT_SYSTEM_DEHAZE_DEBUG_ENABLE_DATASIZE (1)

static __inline HI_VOID hi_ext_system_dehaze_debug_enable_write(HI_U8 id, HI_U16 data){
    IOWR_16DIRECT(0x14730, data & 0x1);
}
static __inline HI_U16 hi_ext_system_dehaze_debug_enable_read(HI_U8 id) {
    return (HI_U16) (IORD_16DIRECT(0x14730)&0x1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dehaze_debug_addr
// ------------------------------------------------------------------------------ //
// address of dehaze debug information
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_DEHAZE_DEBUG_ADDR_DEFAULT (0x0)
#define HI_EXT_SYSTEM_DEHAZE_DEBUG_ADDR_DATASIZE (32)
    
// args: data (32-bit)
static __inline HI_VOID hi_ext_system_dehaze_debug_addr_write(HI_U8 id, HI_U32 data){
    IOWR_32DIRECT(0x14734, data);
}

static __inline HI_U32 hi_ext_system_dehaze_debug_addr_read(HI_U8 id) {
    return IORD_32DIRECT(0x14734);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dehaze_debug_size
// ------------------------------------------------------------------------------ //
// size of dehaze debug struct
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_DEHAZE_DEBUG_SIZE_DEFAULT (0x0)
#define HI_EXT_SYSTEM_DEHAZE_DEBUG_SIZE_DATASIZE (32)
    
// args: data (32-bit)
static __inline HI_VOID hi_ext_system_dehaze_debug_size_write(HI_U8 id, HI_U32 data){
    IOWR_32DIRECT(0x14738, data);
}

static __inline HI_U32 hi_ext_system_dehaze_debug_size_read(HI_U8 id) {
    return IORD_32DIRECT(0x14738);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dehaze_debug_depth
// ------------------------------------------------------------------------------ //
// depth of dehaze debug struct
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_DEHAZE_DEBUG_DEPTH_DEFAULT (0x0)
#define HI_EXT_SYSTEM_DEHAZE_DEBUG_DEPTH_DATADEPTH (32)
    
// args: data (32-bit)
static __inline HI_VOID hi_ext_system_dehaze_debug_depth_write(HI_U8 id, HI_U32 data){
    IOWR_32DIRECT(0x1473c, data);
}

static __inline HI_U32 hi_ext_system_dehaze_debug_depth_read(HI_U8 id) {
    return IORD_32DIRECT(0x1473c);
}


// args: data (1-bit)
#define HI_EXT_SYSTEM_FSWDR_ENABLE_WRITE_DEFAULT (0)
#define HI_EXT_SYSTEM_FSWDR_ENABLE_WRITE_DATASIZE (1)

static __inline HI_VOID hi_ext_system_fswdr_enable_write(HI_U8 data)
{
  IOWR_8DIRECT(0x14740, data&0x1);
}

static __inline HI_U8 hi_ext_system_fswdr_enable_read()
{
  return (IORD_8DIRECT(0x14740)&0x1);
}


// args: data (1-bit)
#define HI_EXT_SYSTEM_GAMMAFE_POSITION_WRITE_DEFAULT (0)
#define HI_EXT_SYSTME_GAMMAFE_POSITION_WRITE_DATASIZE (1)
static __inline HI_VOID hi_ext_system_gammafe_position_write(HI_U8 data)
{
  IOWR_8DIRECT(0x14741, data&0x1);
}

static __inline HI_U8 hi_ext_system_gammafe_position_read()
{
  return (IORD_8DIRECT(0x14741)&0x1);
}



// args: data (2-bit)
#define HI_EXT_SYSTEM_TOP_BIT_RESERVE3_WRITE_DEFAULT (0)
#define HI_EXT_SYSTEM_TOP_BIT_RESERVE3_WRITE_DATASIZE (2)

static __inline HI_VOID hi_ext_system_top_bit_reserve3_write(HI_U8 data)
{
  IOWR_8DIRECT(0x14742, data&0x3);
}

static __inline HI_U8 hi_ext_system_top_bit_reserve3_read()
{
	return (IORD_8DIRECT(0x14742)&0x3);
}

// args: data (2-bit)

#define HI_EXT_SYSTEM_TOP_BIT_RESERVE4_WRITE_DEFAULT (0)
#define HI_EXT_SYSTEM_TOP_BIT_RESERVE4_WRITE_DATASIZE (2)

static __inline HI_VOID hi_ext_system_top_bit_reserve4_write(HI_U8 data)
{
  IOWR_8DIRECT(0x14743, data&0x3);
}

static __inline HI_U8 hi_ext_system_top_bit_reserve4_read()
{
	return (IORD_8DIRECT(0x14743)&0x3);
}

// args: data (1-bit)
#define HI_EXT_SYSTEM_ISP_CHNSWITCH_WRITE_DEFAULT (0x0)
#define HI_EXT_SYSTEM_ISP_CHNSWITCH_WRITE_DATASIZE (1)

static __inline HI_VOID hi_ext_system_isp_chnswitch_write(HI_U8 data)
{
  IOWR_8DIRECT(0x14744, data&0x1);
}	

static __inline HI_U8 hi_ext_system_isp_chnswitch_read()
{
	return (IORD_8DIRECT(0x14744)&0x1);
}

// args: data (2-bit)
#define HI_EXT_SYSTEM_ISP_BYPASSMODE_WRITE_DEFAULT (0)
#define HI_EXT_SYSTEM_ISP_BYPASSMODE_WRITE_DATASIZE (2)

static __inline HI_VOID hi_ext_system_isp_bypassmode_write(HI_U8 data)
{
	IOWR_8DIRECT(0x14745, data&0x3);

}

static __inline HI_U8 hi_ext_system_isp_bypassmode_read()
{
	return (IORD_8DIRECT(0x14745)&0x3);
}

// args: data (1-bit)
#define HI_EXT_SYSTEM_ISP_BYPASSALL_WRITE_DEFAULT (0)
#define HI_EXT_SYSTEM_ISP_BYPASSALL_WRITE_DATASIZE (1)
static __inline HI_VOID hi_ext_system_isp_bypassall_write(HI_U8 data)
{
	IOWR_8DIRECT(0x14746, data&0x1);
}

static __inline HI_U8 hi_ext_system_isp_bypassall_read()
{
	return (IORD_8DIRECT(0x14746)&0x1);

}

// args: data (3-bit)

#define HI_EXT_SYSTEM_TOP_BIT_RESERVE5_WRITE_DEFAULT (0)
#define HI_EXT_SYSTEM_TOP_BIT_RESERVE5_WRITE_DATASIZE (3)

static __inline HI_VOID hi_ext_system_top_bit_reserve5_write(HI_U8 data)
{
	IOWR_8DIRECT(0x14747, data&0x7);
}

static __inline HI_U8 hi_ext_system_top_bit_reserve5_read()
{
	return (IORD_8DIRECT(0x14747)&0x7);
}

// args: data (1-bit)

#define HI_EXT_SYSTEM_ISP_BALANCEFE_ENABLE_WRITE_DEFAULT (0)
#define HI_EXT_SYSTEM_ISP_BALANCEFE_ENABLE_WRITE_DATASIZE (1)

static __inline HI_VOID hi_ext_system_isp_balancefe_enable_write(HI_U8 data)
{
	IOWR_8DIRECT(0x14748, data&0x1);
}

static __inline HI_U8 hi_ext_system_isp_balancefe_enable_read()
{
	return (IORD_8DIRECT(0x14748)&0x1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dehaze_debug_quit
// ------------------------------------------------------------------------------ //
// depth of dehaze debug struct
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_DEHAZE_DEBUG_QUIT_DEFAULT (0x0)
#define HI_EXT_SYSTEM_DEHAZE_DEBUG_QUIT_DATADEPTH (8)
    
// args: data (32-bit)
static __inline HI_VOID hi_ext_system_dehaze_debug_quit_write(HI_U8 id, HI_U8 data){
    IOWR_8DIRECT(0x1474a, data);
}

static __inline HI_U8 hi_ext_system_dehaze_debug_quit_read(HI_U8 id) {
    return IORD_8DIRECT(0x1474a);
}

// ------------------------------------------------------------------------------ //
// Register: hi_ext_system_dehaze_debug_complete
// ------------------------------------------------------------------------------ //
// depth of dehaze debug struct
// ------------------------------------------------------------------------------ //
#define HI_EXT_SYSTEM_DEHAZE_DEBUG_COMPLETE_DEFAULT (0x0)
#define HI_EXT_SYSTEM_DEHAZE_DEBUG_COMPLETE_DATADEPTH (8)
    
// args: data (32-bit)
static __inline HI_VOID hi_ext_system_dehaze_debug_complete_write(HI_U8 id, HI_U8 data){
    IOWR_8DIRECT(0x1474b, data);
}

static __inline HI_U8 hi_ext_system_dehaze_debug_complete_read(HI_U8 id) {
    return IORD_8DIRECT(0x1474b);
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __ISP_EXT_CONFIG_H__ */

