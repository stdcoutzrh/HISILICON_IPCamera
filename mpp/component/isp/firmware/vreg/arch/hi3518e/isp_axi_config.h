/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_axi_config.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/05/14
  Description   : 
  History       :
  1.Date        : 2013/05/14
    Author      : n00168968
    Modification: Created file

******************************************************************************/

#ifndef __ISP_AXI_CONFIG_H__
#define __ISP_AXI_CONFIG_H__

#include "hi_vreg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

// ------------------------------------------------------------------------------ //
// Instance 'axi' of module 'ip_config_axi'
// ------------------------------------------------------------------------------ //

#define HI_AXI_BASE_ADDR (0x8000)
#define HI_AXI_SIZE      (0x100)

// ------------------------------------------------------------------------------ //
// Group: FPGA Top
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_axi_fpga_top_active_width
// ------------------------------------------------------------------------------ //
// Active video width in pixels
// ------------------------------------------------------------------------------ //

#define HI_AXI_FPGA_TOP_ACTIVE_WIDTH_DEFAULT (0x780)
#define HI_AXI_FPGA_TOP_ACTIVE_WIDTH_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_axi_fpga_top_active_width_write(HI_U16 data) {
    IOWR_16DIRECT(0x8000, data);
}
static __inline HI_U16 hi_axi_fpga_top_active_width_read(HI_VOID) {
    return IORD_16DIRECT(0x8000);
}

// ------------------------------------------------------------------------------ //
// Register: hi_axi_fpga_top_active_height
// ------------------------------------------------------------------------------ //
// Active video height in lines
// ------------------------------------------------------------------------------ //

#define HI_AXI_FPGA_TOP_ACTIVE_HEIGHT_DEFAULT (0x438)
#define HI_AXI_FPGA_TOP_ACTIVE_HEIGHT_DATASIZE (16)

// args: data (16-bit)
static __inline HI_VOID hi_axi_fpga_top_active_height_write(HI_U16 data) {
    IOWR_16DIRECT(0x8004, data);
}
static __inline HI_U16 hi_axi_fpga_top_active_height_read(HI_VOID) {
    return IORD_16DIRECT(0x8004);
}

// ------------------------------------------------------------------------------ //
// Register: hi_axi_fpga_top_global_fsm_reset
// ------------------------------------------------------------------------------ //
// Global reset of FSMs
// ------------------------------------------------------------------------------ //

#define HI_AXI_FPGA_TOP_GLOBAL_FSM_RESET_DEFAULT (0x0)
#define HI_AXI_FPGA_TOP_GLOBAL_FSM_RESET_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_axi_fpga_top_global_fsm_reset_write(HI_U8 data) {
    IOWR_8DIRECT(0x8008, (data & 0x01));
}
static __inline HI_U8 hi_axi_fpga_top_global_fsm_reset_read(HI_VOID) {
    return (IORD_8DIRECT(0x8008) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Group: WDR Frame Buffer
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_axi_wdr_frame_buffer_axi_port_enable
// ------------------------------------------------------------------------------ //

#define HI_AXI_WDR_FRAME_BUFFER_AXI_PORT_ENABLE_DEFAULT (0x0)
#define HI_AXI_WDR_FRAME_BUFFER_AXI_PORT_ENABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_axi_wdr_frame_buffer_axi_port_enable_write(HI_U8 data) {
    HI_U8 current = IORD_8DIRECT(0x8010);
    IOWR_8DIRECT(0x8010, (data & 0x01) | (current & 0xFE));
}
static __inline HI_U8 hi_axi_wdr_frame_buffer_axi_port_enable_read(HI_VOID) {
    return (IORD_8DIRECT(0x8010) & 0x01);
}

// ------------------------------------------------------------------------------ //
// Register: hi_axi_wdr_frame_buffer_write_enable
// ------------------------------------------------------------------------------ //

#define HI_AXI_WDR_FRAME_BUFFER_WRITE_ENABLE_DEFAULT (0x1)
#define HI_AXI_WDR_FRAME_BUFFER_WRITE_ENABLE_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_axi_wdr_frame_buffer_write_enable_write(HI_U8 data) {
    HI_U8 current = IORD_8DIRECT(0x8010);
    IOWR_8DIRECT(0x8010, ((data & 0x01) << 1) | (current & 0xFD));
}
static __inline HI_U8 hi_axi_wdr_frame_buffer_write_enable_read(HI_VOID) {
    return ((IORD_8DIRECT(0x8010) & 0x02) >> 1);
}

// ------------------------------------------------------------------------------ //
// Register: hi_axi_wdr_frame_buffer_frame_cancel
// ------------------------------------------------------------------------------ //

#define HI_AXI_WDR_FRAME_BUFFER_FRAME_CANCEL_DEFAULT (0x1)
#define HI_AXI_WDR_FRAME_BUFFER_FRAME_CANCEL_DATASIZE (1)

// args: data (1-bit)
static __inline HI_VOID hi_axi_wdr_frame_buffer_frame_cancel_write(HI_U8 data) {
    HI_U8 current = IORD_8DIRECT(0x8010);
    IOWR_8DIRECT(0x8010, ((data & 0x01) << 2) | (current & 0xFB));
}
static __inline HI_U8 hi_axi_wdr_frame_buffer_frame_cancel_read(HI_VOID) {
    return ((IORD_8DIRECT(0x8010) & 0x04) >> 2);
}

// ------------------------------------------------------------------------------ //
// Register: hi_axi_wdr_frame_buffer_config
// ------------------------------------------------------------------------------ //

#define HI_AXI_WDR_FRAME_BUFFER_CONFIG_DEFAULT (0x0000)
#define HI_AXI_WDR_FRAME_BUFFER_CONFIG_DATASIZE (32)

// args: data (32-bit)
static __inline HI_VOID hi_axi_wdr_frame_buffer_config_write(HI_U32 data) {
    IOWR_32DIRECT(0x8014, data);
}
static __inline HI_U32 hi_axi_wdr_frame_buffer_config_read(HI_VOID) {
    return IORD_32DIRECT(0x8014);
}

// ------------------------------------------------------------------------------ //
// Register: hi_axi_wdr_frame_buffer_status
// ------------------------------------------------------------------------------ //

#define HI_AXI_WDR_FRAME_BUFFER_STATUS_DEFAULT (0x0000)
#define HI_AXI_WDR_FRAME_BUFFER_STATUS_DATASIZE (32)

// args: data (32-bit)
static __inline HI_U32 hi_axi_wdr_frame_buffer_status_read(HI_VOID) {
    return IORD_32DIRECT(0x8018);
}

// ------------------------------------------------------------------------------ //
// Register: hi_axi_wdr_frame_buffer_bank0_base
// ------------------------------------------------------------------------------ //
// bank 0 base address for frame buffer, should be word-aligned
// ------------------------------------------------------------------------------ //

#define HI_AXI_WDR_FRAME_BUFFER_BANK0_BASE_DEFAULT (0x0)
#define HI_AXI_WDR_FRAME_BUFFER_BANK0_BASE_DATASIZE (32)

// args: data (32-bit)
static __inline HI_VOID hi_axi_wdr_frame_buffer_bank0_base_write(HI_U32 data) {
    IOWR_32DIRECT(0x8020, data);
}
static __inline HI_U32 hi_axi_wdr_frame_buffer_bank0_base_read(HI_VOID) {
    return IORD_32DIRECT(0x8020);
}

// ------------------------------------------------------------------------------ //
// Register: hi_axi_wdr_frame_buffer_bank1_base
// ------------------------------------------------------------------------------ //
// bank 1 base address for frame buffer, should be word-aligned
// ------------------------------------------------------------------------------ //

#define HI_AXI_WDR_FRAME_BUFFER_BANK1_BASE_DEFAULT (0x0)
#define HI_AXI_WDR_FRAME_BUFFER_BANK1_BASE_DATASIZE (32)

// args: data (32-bit)
static __inline HI_VOID hi_axi_wdr_frame_buffer_bank1_base_write(HI_U32 data) {
    IOWR_32DIRECT(0x8024, data);
}
static __inline HI_U32 hi_axi_wdr_frame_buffer_bank1_base_read(HI_VOID) {
    return IORD_32DIRECT(0x8024);
}

// ------------------------------------------------------------------------------ //
// Register: hi_axi_wdr_frame_buffer_line_offset
// ------------------------------------------------------------------------------ //
// distance between lines, should be word-aligned
// ------------------------------------------------------------------------------ //

#define HI_AXI_WDR_FRAME_BUFFER_LINE_OFFSET_DEFAULT (0x1000)
#define HI_AXI_WDR_FRAME_BUFFER_LINE_OFFSET_DATASIZE (32)

// args: data (32-bit)
static __inline HI_VOID hi_axi_wdr_frame_buffer_line_offset_write(HI_U32 data) {
    IOWR_32DIRECT(0x802C, data);
}
static __inline HI_U32 hi_axi_wdr_frame_buffer_line_offset_read(HI_VOID) {
    return IORD_32DIRECT(0x802C);
}

// ------------------------------------------------------------------------------ //

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __ISP_AXI_CONFIG_H__ */

