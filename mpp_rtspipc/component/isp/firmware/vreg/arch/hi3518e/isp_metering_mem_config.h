/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_metering_mem_config.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/01/16
  Description   : 
  History       :
  1.Date        : 2013/01/16
    Author      : n00168968
    Modification: Created file

******************************************************************************/
#ifndef __ISP_METERING_MEM_CONFIG_H__
#define __ISP_METERING_MEM_CONFIG_H__

#include "hi_vreg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

// ------------------------------------------------------------------------------ //
// Instance 'metering_mem' of module 'metering_mem'
// ------------------------------------------------------------------------------ //


#define HI_METERING_MEM_BASE_ADDR (0x8000)
#define HI_METERING_MEM_SIZE      (0x4000)

#define HI_AE_METERING_MEM_BASE_ADDR  (0x8000)
#define HI_AE_METERING_MEM_SIZE  (0x7F8)  //0x800

#define HI_AE_SUM_METERING_MEM_BASE_ADDR  (0x9800)
#define HI_AE_SUM_METERING_MEM_SIZE  (0x7F8)  //0x800

#define HI_AWB_METERING_MEM_BASE_ADDR (0x8800)
#define HI_AWB_METERING_MEM_SIZE  (0x3FC) //0x800

#define HI_AWB_SUM_METERING_MEM_BASE_ADDR (0xA800)
#define HI_AWB_SUM_METERING_MEM_SIZE  (0x5FA) //0x600

#define HI_AF_METERING_MEM_BASE_ADDR  (0x9200)
#define HI_AF_METERING_MEM_SIZE  (0x7FC)  //0x800

#define HISI_AF_METERING_MEM_BASE_ADDR  (0x2100)
#define HISI_AF_METERING_MEM_SIZE  (0xBF0)  

// ------------------------------------------------------------------------------ //
// Group: Array
// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Register: hi_metering_mem_array_data
// ------------------------------------------------------------------------------ //
// Please see other documentation for a description of the contents of this array.
// ------------------------------------------------------------------------------ //

#define HI_METERING_MEM_ARRAY_DATA_DEFAULT (0x0)
#define HI_METERING_MEM_ARRAY_DATA_DATASIZE (32)
static __inline HI_U32 hi_metering_mem_arrary_data_read_ae( HI_U16 index){
    HI_U32 addr = HI_AE_METERING_MEM_BASE_ADDR + (index << 2);
    return IORD_32DIRECT(addr);
}

//Add AWB statistic information read function
static __inline HI_U16 hi_metering_mem_array_data_read_rg(HI_U16 index) {
    HI_U32 addr = HI_AWB_METERING_MEM_BASE_ADDR + (index << 3);
    return (HI_U16) (IORD_32DIRECT(addr) & 0xfff);
}

static __inline HI_U16 hi_metering_mem_array_data_read_bg(HI_U16 index) {
    HI_U32 addr = HI_AWB_METERING_MEM_BASE_ADDR + (index << 3);
    return (HI_U16) ((IORD_32DIRECT(addr) >> 16) & 0xfff);
}

static __inline HI_U32 hi_metering_mem_array_data_read_bg_rg(HI_U16 index) {
    HI_U32 addr = HI_AWB_METERING_MEM_BASE_ADDR + (index << 3);
    return IORD_32DIRECT(addr);
}

static __inline HI_U32 hi_metering_mem_array_data_read_sum(HI_U16 index) {
    HI_U32 addr = HI_AWB_METERING_MEM_BASE_ADDR + (index << 3) + 4;
    return IORD_32DIRECT(addr);
}


//Each AF zone data spans 32 bits. 16 bits are allocated for contrast metrics and 16 bits for noise threshold. 
static __inline HI_U16 hi_metering_mem_array_data_read_af(HI_U16 index) {
    HI_U32 addr = HI_AF_METERING_MEM_BASE_ADDR + (index << 2);
    return (HI_U16) (IORD_32DIRECT(addr) & 0xffff);
}

//Hisi AF areazone data stat 32 bits.  
static __inline HI_U32 hi_area_stat_mem_array_data_read_af_v1h1(HI_U16 index) {
    HI_U32 addr = HISI_AF_METERING_MEM_BASE_ADDR + (index * 12);
    return IORD_32DIRECT_VI(addr);
}

static __inline HI_U32 hi_area_stat_mem_array_data_read_af_v2h2(HI_U16 index) {
    HI_U32 addr = HISI_AF_METERING_MEM_BASE_ADDR + (index * 12) + 4;
    return IORD_32DIRECT_VI(addr);
}

static __inline HI_U32 hi_area_stat_mem_array_data_read_af_y(HI_U16 index) {
    HI_U32 addr = HISI_AF_METERING_MEM_BASE_ADDR + (index * 12) + 8;
    return IORD_32DIRECT_VI(addr);
}

static __inline HI_U32 hi_area_block_distribute_data_read_af(HI_VOID) {
    return IORD_32DIRECT_VI(0x2010);
}


// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Instance 'AE_sum_mem' of module 'AE_sum_mem'
// ------------------------------------------------------------------------------ //
#define HI_AE_SUM_MEM_BASE_ADDR (0x9800)
#define HI_AE_SUM_MEM_SIZE      (0x800)

static __inline HI_U16 hi_ae_sum_array_data_read_r(HI_U16 index) {
    HI_U32 addr = HI_AE_SUM_MEM_BASE_ADDR + (index << 3);
    return IORD_16DIRECT(addr);
}

static __inline HI_U16 hi_ae_sum_array_data_read_gr(HI_U16 index) {
    HI_U32 addr = HI_AE_SUM_MEM_BASE_ADDR + (index << 3) + 2;
    return IORD_16DIRECT(addr);
}

static __inline HI_U16 hi_ae_sum_array_data_read_gb(HI_U16 index) {
    HI_U32 addr = HI_AE_SUM_MEM_BASE_ADDR + (index << 3) + 4;
    return IORD_16DIRECT(addr);
}

static __inline HI_U16 hi_ae_sum_array_data_read_b(HI_U16 index) {
    HI_U32 addr = HI_AE_SUM_MEM_BASE_ADDR + (index << 3) + 6;
    return IORD_16DIRECT(addr);
}

// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Instance 'AWB_sum_mem' of module 'AWB_sum_mem'
// ------------------------------------------------------------------------------ //
#define HI_AWB_SUM_MEM_BASE_ADDR (0xA800)
#define HI_AWB_SUM_MEM_SIZE      (0x2000)

static __inline HI_U16 hi_awb_sum_array_data_read_avg_r(HI_U16 index) {
    HI_U32 addr = HI_AWB_SUM_MEM_BASE_ADDR + (index * 0xC);
    return IORD_16DIRECT(addr);
}

static __inline HI_U16 hi_awb_sum_array_data_read_avg_g(HI_U16 index) {
    HI_U32 addr = HI_AWB_SUM_MEM_BASE_ADDR + (index  * 0xC) + 2;
    return IORD_16DIRECT(addr);
}

static __inline HI_U16 hi_awb_sum_array_data_read_avg_b(HI_U16 index) {
    HI_U32 addr = HI_AWB_SUM_MEM_BASE_ADDR + (index * 0xC) + 4;
    return IORD_16DIRECT(addr);
}

static __inline HI_U16 hi_awb_sum_array_data_read_avg_y(HI_U16 index) {
    HI_U32 addr = HI_AWB_SUM_MEM_BASE_ADDR + (index  * 0xC) + 6;
    return IORD_16DIRECT(addr);
}

static __inline HI_U32 hi_awb_sum_array_data_read_avg_g_r(HI_U16 index) {
    HI_U32 addr = HI_AWB_SUM_MEM_BASE_ADDR + (index * 0xC);
    return IORD_32DIRECT(addr);
}

static __inline HI_U32 hi_awb_sum_array_data_read_all_avgb(HI_U16 index) {
    HI_U32 addr = HI_AWB_SUM_MEM_BASE_ADDR + (index * 0xC) + 4;
    return IORD_32DIRECT(addr);
}

static __inline HI_U32 hi_awb_sum_array_data_read_min_max(HI_U16 index) {
    HI_U32 addr = HI_AWB_SUM_MEM_BASE_ADDR + (index * 0xC) + 8;
    return IORD_32DIRECT(addr);
}

// ------------------------------------------------------------------------------ //

// ------------------------------------------------------------------------------ //
// Instance 'DIS_mem' of module 'DIS_mem'
// ------------------------------------------------------------------------------ //
#define HI_DIS_MEM_BASE_ADDR (0xA000)
#define HI_DIS_MEM_SIZE      (0x1000)

static __inline HI_U16 hi_dis_array_data_read_corr_data(HI_U16 index) {
    HI_U32 addr = HI_DIS_MEM_BASE_ADDR + (index << 1);
    return IORD_16DIRECT(addr);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __ISP_METERING_MEM_CONFIG_H__ */

