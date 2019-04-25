/******************************************************************************

  Copyright (C), 2004-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_config.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2014/09/22
  Description   : 
  History       :
  1.Date        : 2014/09/22
    Author      : q00214668
    Modification: Created file

******************************************************************************/

#ifndef __ISP_CONFIG_CONFIG_EX_H__
#define __ISP_CONFIG_CONFIG_EX_H__

#include "hi_vreg.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

/*****************************************************
MODULE        -----COMM-----
MODULE        -----COMM-----
MODULE        -----COMM-----
*****************************************************/
/*****************************************************
ISP_MODULE_POS:       工作使能寄存器
0x205a0088:                31:04      reserved
					     03:03      af_sel,   0:在wb前面 ；1：在csc后面Y域
					     02:01      ae_sel,  00:AE在WB前面；01：AE在WB后面；10：AE在DRC后面					
                                     00:00      dcg_sel,0:DPC在BLC模块后面；1：DPC在DRC后面
*****************************************************/
static __inline HI_VOID hi_isp_module_dcg_sel_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x0088);
	IOWR_8DIRECT(0x0088 ,(data & 0x01) | (u8current & 0xfe));
}

static __inline HI_U8 hi_isp_module_dcg_sel_read(HI_VOID) {
	return (IORD_8DIRECT(0x0088) & 0x1);
}


/*****************************************************
ISP_MODULE_POS:       工作使能寄存器
0x205a0088:                31:04      reserved
					     03:03      af_sel,   0:在wb前面 ；1：在csc后面Y域
					     02:01      ae_sel,  00:AE在DG后面；01：AE在WB后面；10：AE在DRC后面					
                                     00:00      dcg_sel,0:DPC在BLC模块后面；1：DPC在DRC后面
*****************************************************/
#define HI_ISP_TOP_AE_SELECT_DEFAULT (0)
#define HI_ISP_TOP_AE_SELECT_DATASIZE (2)
    
#define HI_ISP_TOP_AE_SELECT_AFTER_DG (0)
#define HI_ISP_TOP_AE_SELECT_AFTER_STATIC_WB (1)
#define HI_ISP_TOP_AE_SELECT_AFTER_DRC (2)
static __inline HI_VOID hi_isp_module_ae_sel_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x0088);
	IOWR_8DIRECT(0x0088 ,((data & 0x03) << 1) | (u8current & 0xf9));
}

static __inline HI_U8 hi_isp_module_ae_sel_read(HI_VOID) {
	return ((IORD_8DIRECT(0x0088) & 0x6) >> 1);
}


/*****************************************************
ISP_MODULE_POS:       工作使能寄存器
0x205a0088:                31:04     reserved
					     03:03      af_sel,   0:在wb前面 ；1：在csc后面Y域
					     02:01      ae_sel,  00:AE在WB前面；01：AE在WB后面；10：AE在DRC后面					
                                     00:00      dcg_sel,0:DPC在BLC模块后面；1：DPC在DRC后面
*****************************************************/
static __inline HI_VOID hi_isp_module_af_sel_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x0088);
	IOWR_8DIRECT(0x0088 ,((data & 0x01)<< 3) | (u8current & 0xf7));
}

static __inline HI_U8 hi_isp_module_af_sel_read(HI_VOID) {
	return ((IORD_8DIRECT(0x0088) & 0x8) >> 3);
}

/*****************************************************
MODULE        -----DITHER-----
MODULE        -----DITHER-----
MODULE        -----DITHER-----
*****************************************************/
/*****************************************************
ISP_DMNR_DITHER:     ISP DMNR DITHER寄存器
0x205a0098:                31:08      reserved
					     07:04      out_bits
					     03:03      spatial_mode
                                    02:01      isp_dither_round
                                    00:00      isp_dither_en
*****************************************************/
static __inline HI_VOID hi_isp_dmnr_dither_en_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x0098);
	IOWR_8DIRECT(0x0098 ,(data & 0x01) | (u8current & 0xfe));
}

static __inline HI_U8 hi_isp_dmnr_dither_en_read(HI_VOID) {
	return (IORD_8DIRECT(0x0098) & 0x1);
}

static __inline HI_VOID hi_isp_dmnr_dither_round_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x0098);
	IOWR_8DIRECT(0x0098 ,((data & 0x03) << 1) | (u8current & 0xf9));
}

static __inline HI_U8 hi_isp_dmnr_dither_round_read(HI_VOID) {
	return ((IORD_8DIRECT(0x0098) & 0x6) >> 1);
}

static __inline HI_VOID hi_isp_dmnr_dither_spatial_mode_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x0098);
	IOWR_8DIRECT(0x0098 ,((data & 0x1) << 3) | (u8current & 0xf7));
}

static __inline HI_U8 hi_isp_dmnr_dither_spatial_mode_read(HI_VOID) {
	return ((IORD_8DIRECT(0x0098) & 0x8) >> 3);
}

static __inline HI_VOID hi_isp_dmnr_dither_out_bits_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x0098);
	IOWR_8DIRECT(0x0098 ,((data & 0xf) << 4) | (u8current & 0xf));
}

static __inline HI_U8 hi_isp_dmnr_dither_out_bits_read(HI_VOID) {
	return ((IORD_8DIRECT(0x0098) & 0xf0) >> 4);
}

/*****************************************************
ISP_DRC_DITHER:     ISP DRC DITHER寄存器
0x205a009c:                31:08      reserved
					     07:04      out_bits
					     03:03      spatial_mode
                                    02:01      isp_dither_round
                                    00:00      isp_dither_en
*****************************************************/
static __inline HI_VOID hi_isp_drc_moudle_dither_en_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x009c);
	IOWR_8DIRECT(0x009c ,(data & 0x01) | (u8current & 0xfe));
}

static __inline HI_U8 hi_isp_drc_moudle_dither_en_read(HI_VOID) {
	return (IORD_8DIRECT(0x009c) & 0x1);
}

static __inline HI_VOID hi_isp_drc_dither_round_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x009c);
	IOWR_8DIRECT(0x009c ,((data & 0x03) << 1) | (u8current & 0xf9));
}

static __inline HI_U8 hi_isp_drc_dither_round_read(HI_VOID) {
	return ((IORD_8DIRECT(0x009c) & 0x6) >> 1);
}

static __inline HI_VOID hi_isp_drc_dither_spatial_mode_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x009c);
	IOWR_8DIRECT(0x009c ,((data & 0x1) << 3) | (u8current & 0xf7));
}

static __inline HI_U8 hi_isp_drc_dither_spatial_mode_read(HI_VOID) {
	return ((IORD_8DIRECT(0x009c) & 0x8) >> 3);
}

static __inline HI_VOID hi_isp_drc_dither_out_bits_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x009c);
	IOWR_8DIRECT(0x009c ,((data & 0xf) << 4) | (u8current & 0xf));
}

static __inline HI_U8 hi_isp_drc_dither_out_bits_read(HI_VOID) {
	return ((IORD_8DIRECT(0x009c) & 0xf0) >> 4);
}

/*****************************************************
ISP_DCI_DITHER:     ISP DCI DITHER寄存器
0x205a00A0:                31:08      reserved
					     07:04      out_bits
					     03:03      spatial_mode
                                    02:01      isp_dither_round
                                    00:00      isp_dither_en
*****************************************************/
static __inline HI_VOID hi_isp_dci_dither_en_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x00a0);
	IOWR_8DIRECT(0x00a0 ,(data & 0x01) | (u8current & 0xfe));
}

static __inline HI_U8 hi_isp_dci_dither_en_read(HI_VOID) {
	return (IORD_8DIRECT(0x00a0) & 0x1);
}

static __inline HI_VOID hi_isp_dci_dither_round_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x00a0);
	IOWR_8DIRECT(0x00a0 ,((data & 0x03) << 1) | (u8current & 0xf9));
}

static __inline HI_U8 hi_isp_dci_dither_round_read(HI_VOID) {
	return ((IORD_8DIRECT(0x00a0) & 0x6) >> 1);
}

static __inline HI_VOID hi_isp_dci_dither_spatial_mode_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x00a0);
	IOWR_8DIRECT(0x00a0 ,((data & 0x1) << 3) | (u8current & 0xf7));
}

static __inline HI_U8 hi_isp_dci_dither_spatial_mode_read(HI_VOID) {
	return ((IORD_8DIRECT(0x00a0) & 0x8) >> 3);
}

static __inline HI_VOID hi_isp_dci_dither_out_bits_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x00a0);
	IOWR_8DIRECT(0x00a0 ,((data & 0xf) << 4) | (u8current & 0xf));
}

static __inline HI_U8 hi_isp_dci_dither_out_bits_read(HI_VOID) {
	return ((IORD_8DIRECT(0x00a0) & 0xf0) >> 4);
}

/*****************************************************
ISP_USER_DEFINE0:     ISP USER DEFINE寄存器
0x205a00A4:              31:08      reserved
					     07:04      reserved
					     03:03      reserved
                         02:02      reserved
                         01:00      422/420/400
*****************************************************/
static __inline HI_VOID hi_isp_pix_format_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x00a4);
	IOWR_8DIRECT(0x00a4 ,(data & 0x3) | (u8current & 0xfc));
}

static __inline HI_U8 hi_isp_pix_format_read(HI_VOID) {
	return (IORD_8DIRECT(0x00a4) & 0x3);
}


#define HI_ISP_CPI_FIX_TIMING_STAT (200)
/*****************************************************
ISP_YUV444_TIMING_CFG:       输出时序配置寄存器
0x205a01e8:                31:14      reserved
					     13:01      cpi_fix_timing_stat
                                     00:00     cpi_auto_timing_en
*****************************************************/
static __inline HI_VOID hi_isp_yuv444_timing_stat_write(HI_U16 data) {
	IOWR_16DIRECT(0x01e8 ,((data&0x1fff) << 1));
}

static __inline HI_U16 hi_isp_yuv444_timing_stat_read(HI_VOID) {
	return ((IORD_16DIRECT(0x01e8) >> 1) & 0x1fff);
}

/*****************************************************
ISP_YUV422_TIMING_CFG:       输出时序配置寄存器
0x205a02e8:                31:14      reserved
					     13:01      cpi_fix_timing_stat
                                     00:00     cpi_auto_timing_en
*****************************************************/
static __inline HI_VOID hi_isp_yuv422_timing_stat_write(HI_U16 data) {
	IOWR_16DIRECT(0x02e8 ,((data&0x1fff) << 1));
}

static __inline HI_U16 hi_isp_yuv422_timing_stat_read(HI_VOID) {
	return ((IORD_16DIRECT(0x02e8) >> 1) & 0x1fff);
}

/*****************************************************
ISP_YUV444_SIZE:       输出时序配置寄存器
0x205a01F0:                31:28      reserved
					 27:16     height
                                    15:12     reserved
                                    11:00     width
*****************************************************/
static __inline HI_VOID hi_isp_yuv444_width_write(HI_U16 data) {
	HI_U16 u16current = IORD_16DIRECT(0x01f0);
	IOWR_16DIRECT(0x01f0, (data & 0xfff) | (u16current & 0xf000));
}
static __inline HI_U16 hi_isp_yuv444_width_read(HI_VOID) {
    return (IORD_16DIRECT(0x01f0) & 0xfff);
}

static __inline HI_VOID hi_isp_yuv444_height_write(HI_U16 data) {
	HI_U16 u16current = IORD_16DIRECT(0x01f2);
	IOWR_16DIRECT(0x01f2, (data & 0xfff) | (u16current & 0xf000));
}
static __inline HI_U16 hi_isp_yuv444_height_read(HI_VOID) {
    return (IORD_16DIRECT(0x01f2) & 0xfff);
}

/*****************************************************
ISP_YUV422_SIZE:       输出时序配置寄存器
0x205a02F0:                31:28      reserved
					 27:16     height
                                    15:12     reserved
                                    11:00     width
*****************************************************/
static __inline HI_VOID hi_isp_yuv422_width_write(HI_U16 data) {
	HI_U16 u16current = IORD_16DIRECT(0x02f0);
	IOWR_16DIRECT(0x02f0, (data & 0xfff) | (u16current & 0xf000));
}
static __inline HI_U16 hi_isp_yuv422_width_read(HI_VOID) {
    return (IORD_16DIRECT(0x02f0) & 0xfff);
}

static __inline HI_VOID hi_isp_yuv422_height_write(HI_U16 data) {
	HI_U16 u16current = IORD_16DIRECT(0x02f2);
	IOWR_16DIRECT(0x02f2, (data & 0xfff) | (u16current & 0xf000));
}
static __inline HI_U16 hi_isp_yuv422_height_read(HI_VOID) {
    return (IORD_16DIRECT(0x02f2) & 0xfff);
}

/*****************************************************
ISP_YUV444_CTRL_F:       输出时序配置寄存器
0x205a01E0:                31:02     reserved
					 01:00     rggb_cfg
*****************************************************/
static __inline HI_VOID hi_isp_yuv444_rggb_start_write(HI_U8 data) {
	IOWR_8DIRECT(0x01E0, (data & 0x03));   
}
static __inline HI_U8 hi_isp_yuv444_rggb_start_read(HI_VOID) {
	return (IORD_8DIRECT(0x01E0) & 0x03); 
}

/*****************************************************
ISP_YUV422_CTRL_F:       输出时序配置寄存器
0x205a01E0:                31:02     reserved
					 01:00     rggb_cfg
*****************************************************/
static __inline HI_VOID hi_isp_yuv422_rggb_start_write(HI_U8 data) {
	IOWR_8DIRECT(0x02E0, (data & 0x03));   
}
static __inline HI_U8 hi_isp_yuv422_rggb_start_read(HI_VOID) {
	return (IORD_8DIRECT(0x02E0) & 0x03); 
}



/*****************************************************
MODULE        -----AE-----
MODULE        -----AE-----
MODULE        -----AE-----
*****************************************************/
/*****************************************************
ISP_AE_CFG:                工作使能寄存器
0x205a2000:                31:1      reserved
                              0      AE使能
*****************************************************/
static __inline HI_VOID hi_isp_ae_cfg_enable_write(HI_U8 data) {
    IOWR_8DIRECT(0x2000, (data & 0x01));
}

static __inline HI_U8 hi_isp_ae_cfg_enable_read(HI_VOID) {
    return (IORD_8DIRECT(0x2000) & 0x1);
}

/*****************************************************
ISP_AE_VERSION:      AE版本号寄存器
0x205a200c        :      31:20      1:v100, 2:v200, 3:v300 ....
					   19:16      reserved
                                  15:  0      发布日期
*****************************************************/
#define HI_ISP_AE_VERSION_V100 (1)
static __inline HI_VOID hi_isp_ae_version_write(HI_U16 data) {
	HI_U16 u16current = IORD_16DIRECT(0x200e);
    IOWR_16DIRECT(0x200e, ((data & 0xfff) << 4) | (u16current & 0xf));
}

static __inline HI_U16 hi_isp_ae_version_read(HI_VOID) {
    return ((IORD_16DIRECT(0x200e) & 0xfff0) >> 4);
}

static __inline HI_VOID hi_isp_ae_date_write(HI_U16 data) {
    IOWR_16DIRECT(0x200c, data);
}

static __inline HI_U16 hi_isp_ae_date_read(HI_VOID) {
    return (IORD_16DIRECT(0x200c));
}

/*****************************************************
ISP_AE_ZONE:                AE分窗配置
0x205a2010  :                31:13      reserved
                                       12:  8      垂直方向分窗个数
                                       7  :  5      reserved
                                       4  :  0      水平方向分窗个数
*****************************************************/
static __inline HI_VOID hi_isp_ae_h_zone_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x2010);
    IOWR_8DIRECT(0x2010, ((data & 0x1f) | (u8current & 0xe0)));
}

static __inline HI_U8 hi_isp_ae_h_zone_read(HI_VOID) {
    return (IORD_8DIRECT(0x2010) & 0x1f);
}

static __inline HI_VOID hi_isp_ae_v_zone_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x2011);
    IOWR_8DIRECT(0x2011, ((data & 0x1f) | (u8current & 0xe0)));
}

static __inline HI_U8 hi_isp_ae_v_zone_read(HI_VOID) {
    return (IORD_8DIRECT(0x2011) & 0x1f);
}


#if 0
/*****************************************************
ISP_AE_SKIP_CRG:                AE选点配置寄存器
0x205a2014          :                31:  8      reserved
                                       7  :  7      0=从第0个列开始计数；1=从第一个开始计数
                                       6  :  4      Histogram decimation in vertical direction:
									0=every pixel;      
									1=every 2nd pixel;
									2=every 3rd pixel;  
									3=every 4th pixel;
									4=every 5th pixel;  
									5=every 8th pixel ;
									6+=every 9th pixel
                                       3  :  3      0=从第0个列开始计数；1=从第一个开始计数
                                       2  :  0      Histogram decimation in horizontal direction:
									 0=every 2nd pixel;
									 1=every 3rd pixel;
									 2=every 4th pixel; 
									 3=every 5th pixel;
									 4=every 8th pixel ;
									 5+=every 9th pixel
*****************************************************/
static __inline HI_VOID hi_isp_metering_offset_y_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x2014);
    IOWR_8DIRECT(0x2014, (((data & 0x1) << 7) | (u8current & 0x7f)));
}

static __inline HI_U8 hi_isp_metering_offset_y_read(HI_VOID) {
    return ((IORD_8DIRECT(0x2014) & 0x80) >> 7);
}

static __inline HI_VOID hi_isp_metering_skip_y_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x2014);
    IOWR_8DIRECT(0x2014, (((data & 0x7) << 4) | (u8current & 0x8f)));
}

static __inline HI_U8 hi_isp_metering_skip_y_read(HI_VOID) {
    return ((IORD_8DIRECT(0x2014) & 0x70) >> 4);
}

static __inline HI_VOID hi_isp_metering_offset_x_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x2014);
    IOWR_8DIRECT(0x2014, (((data & 0x1) << 3) | (u8current & 0xf7)));
}

static __inline HI_U8 hi_isp_metering_offset_x_read(HI_VOID) {
    return ((IORD_8DIRECT(0x2014) & 0x8) >> 3);
}

static __inline HI_VOID hi_isp_metering_skip_x_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x2014);
    IOWR_8DIRECT(0x2014, ((data & 0x7) | (u8current & 0xf8)));
}

static __inline HI_U8 hi_isp_metering_skip_x_read(HI_VOID) {
    return (IORD_8DIRECT(0x2014) & 0x7);
}
#endif


/*****************************************************
ISP_AE_TOTAL_STAT:     256直方图统计时，一帧图像中选中点的个数之和
0x205a2018          :        31:  24      reserved
                                       23  :  0      256直方图统计时，一帧图像中选中点的个数之和                    
*****************************************************/
static __inline HI_VOID hi_isp_ae_pixel_selected_count_write(HI_U32 data) {
	HI_U32 u32current = IORD_32DIRECT(0x2018);
	IOWR_32DIRECT(0x2018, ((data & 0xffffff) | (u32current & 0xff000000)));
}

static __inline HI_U32 hi_isp_ae_pixel_selected_count_read(HI_VOID) {
	return (IORD_32DIRECT(0x2018) & 0xffffff);
}


/*****************************************************
ISP_AE_COUNT_STAT:     256直方图统计时，一帧图像中选中点的权重之和
0x205a201c          :         31:  28     reserved
                                       27  :  0      256直方图统计时，一帧图像中选中点的权重之和                    
*****************************************************/
static __inline HI_VOID hi_isp_ae_pixel_selected_weight_write(HI_U32 data) {
	HI_U32 u32current = IORD_32DIRECT(0x201c);
	IOWR_32DIRECT(0x201c, ((data & 0xfffffff) | (u32current & 0xf0000000)));
}

static __inline HI_U32 hi_isp_ae_pixel_selected_weight_read(HI_VOID) {
	return (IORD_32DIRECT(0x201c) & 0xfffffff);
}


/*****************************************************
ISP_AE_TOTAL_R_AVER:     均值统计时，一帧图像中r的均值
0x205a2020          :         31:0   全局r均值统计                                                          
*****************************************************/
static __inline HI_VOID hi_isp_ae_total_r_aver_write(HI_U32 data) {
	IOWR_32DIRECT(0x2020, data);
}

static __inline HI_U32 hi_isp_ae_total_r_aver_read(HI_VOID) {
	return IORD_32DIRECT(0x2020);
}

/*****************************************************
ISP_AE_TOTAL_Gr_AVER:     均值统计时，一帧图像中gr的均值
0x205a2024          :         31:0   全局gr均值统计                                                          
*****************************************************/
static __inline HI_VOID hi_isp_ae_total_gr_aver_write(HI_U32 data) {
	IOWR_32DIRECT(0x2024, data);
}

static __inline HI_U32 hi_isp_ae_total_gr_aver_read(HI_VOID) {
	return IORD_32DIRECT(0x2024);
}

/*****************************************************
ISP_AE_TOTAL_Gb_AVER:     均值统计时，一帧图像中gb的均值
0x205a2028          :         31:0   全局gb均值统计                                                          
*****************************************************/
static __inline HI_VOID hi_isp_ae_total_gb_aver_write(HI_U32 data) {
	IOWR_32DIRECT(0x2028, data);
}

static __inline HI_U32 hi_isp_ae_total_gb_aver_read(HI_VOID) {
	return IORD_32DIRECT(0x2028);
}


/*****************************************************
ISP_AE_TOTAL_b_AVER:     均值统计时，一帧图像中b的均值
0x205a202c          :         31:0   全局b均值统计                                                          
*****************************************************/
static __inline HI_VOID hi_isp_ae_total_b_aver_write(HI_U32 data) {
	IOWR_32DIRECT(0x202c, data);
}

static __inline HI_U32 hi_isp_ae_total_b_aver_read(HI_VOID) {
	return IORD_32DIRECT(0x202c);
}

/*****************************************************
AE_HIST_HIGH:     256直方图统计时,超过8bit的不参与256个bin计数，另外计数。
0x205a2030         :     31  :  0      256直方图统计时，超过个数计数                
*****************************************************/
static __inline HI_VOID hi_isp_ae_high_hist_weight_write(HI_U32 data) {
	IOWR_32DIRECT(0x2030, data);
}

static __inline HI_U32 hi_isp_ae_high_hist_weight_read(HI_VOID) {
	return IORD_32DIRECT(0x2030);
}

/*****************************************************
ISP_AE_BITMOVE:     256直方图统计时数据区间选取偏移标志
0x205a2040         : 31 :  9      reserved
                     8            sqrt data or not
                     7   :  0      256直方图统计时数据区间选取偏移标志                
*****************************************************/
static __inline HI_VOID hi_isp_ae_bit_move_gamma_write(HI_U32 data) {
HI_U32 u32current = IORD_32DIRECT(0x2040);
IOWR_32DIRECT(0x2040, ((data & 0xfff) | (u32current & 0xfffff000)));
}

static __inline HI_U32 hi_isp_ae_bit_move_gamma_read(HI_VOID) {
return (IORD_32DIRECT(0x2040) & 0xfff);
}


/*****************************************************
ISP_AE_MEM_HIST_RADDR:     AE直方图统计寄存器读地址
0x205a2088                       :     31:0   直方图统计读地址                                                          
*****************************************************/
static __inline HI_VOID hi_isp_ae_mem_hist_raddr_write(HI_U32 data) {
	IOWR_32DIRECT(0x2088, data);
}

static __inline HI_U32 hi_isp_ae_mem_hist_raddr_read(HI_VOID) {
	return IORD_32DIRECT(0x2088);
}


/*****************************************************
ISP_AE_MEM_HIST_RDATA:     AE直方图统计寄存器读数据
0x205a208c                       :     31:0   直方图统计读数据                                                          
*****************************************************/
static __inline HI_VOID hi_isp_ae_mem_hist_rdata_write(HI_U32 data) {
	IOWR_32DIRECT(0x208c, data);
}

static __inline HI_U32 hi_isp_ae_mem_hist_rdata_read(HI_VOID) {
	return IORD_32DIRECT(0x208c);
}

/*****************************************************
ISP_AE_MEM_AVER_RADDR:     AE均值统计寄存器读地址
0x205a2098                        :     31:0   均值统计读地址                                                          
*****************************************************/
static __inline HI_VOID hi_isp_ae_mem_aver_raddr_write(HI_U32 data) {
	IOWR_32DIRECT(0x2098, data);
}

static __inline HI_U32 hi_isp_ae_mem_aver_raddr_read(HI_VOID) {
	return IORD_32DIRECT(0x2098);
}

/*****************************************************
ISP_AE_MEM_AVER_RDATA:     AE均值统计寄存器读数据
0x205a209c                        :     31:0   均值统计读数据                                                          
*****************************************************/
static __inline HI_VOID hi_isp_ae_mem_aver_rdata_write(HI_U32 data) {
	IOWR_32DIRECT(0x209c, data);
}

static __inline HI_U32 hi_isp_ae_mem_aver_rdata_read(HI_VOID) {
	return IORD_32DIRECT(0x209c);
}

/*****************************************************
ISP_AE_MEM_WEIGHT_WADDR:     AE区间权重寄存器
0x205a20a0                             :     31:0   权重表写地址                                                          
*****************************************************/
static __inline HI_VOID hi_isp_ae_mem_wei_waddr_write(HI_U32 data) {
	IOWR_32DIRECT(0x20a0, data);
}

static __inline HI_U32 hi_isp_ae_mem_wei_waddr_read(HI_VOID) {
	return IORD_32DIRECT(0x20a0);
}

/*****************************************************
ISP_AE_MEM_WEIGHT_WDATA:     AE区间权重寄存器
0x205a20a4                             :     31:0   权重表写数据                                                          
*****************************************************/
static __inline HI_VOID hi_isp_ae_mem_wei_wdata_write(HI_U32 data) {
	IOWR_32DIRECT(0x20a4, data);
}

static __inline HI_U32 hi_isp_ae_mem_wei_wdata_read(HI_VOID) {
	return IORD_32DIRECT(0x20a4);
}


/*****************************************************
ISP_AE_MEM_WEIGHT_WADDR:     AE区间权重寄存器
0x205a20a8                             :     31:0   权重表读地址                                                          
*****************************************************/
static __inline HI_VOID hi_isp_ae_mem_wei_raddr_write(HI_U32 data) {
	IOWR_32DIRECT(0x20a8, data);
}

static __inline HI_U32 hi_isp_ae_mem_wei_raddr_read(HI_VOID) {
	return IORD_32DIRECT(0x20a8);
}

/*****************************************************
ISP_AE_MEM_WEIGHT_WDATA:     AE区间权重寄存器
0x205a20ac                             :     31:0   权重表写数据                                                          
*****************************************************/
static __inline HI_VOID hi_isp_ae_mem_wei_rdata_write(HI_U32 data) {
	IOWR_32DIRECT(0x20ac, data);
}

static __inline HI_U32 hi_isp_ae_mem_wei_rdata_read(HI_VOID) {
	return IORD_32DIRECT(0x20ac);
}


/*****************************************************
ISP_AE_CTRL_F                        :     AE普通控制寄存器
0x205a20e0                             :     31:2   reserved  
								       1:0  RGGBstart；0：R Gr Gb B；1：Gr R B Gb；2：Gb B R Gr；3：B Gb Gr R；  
*****************************************************/
static __inline HI_VOID hi_isp_ae_ctrl_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x20e0); 
	IOWR_8DIRECT(0x20e0, (data & 0x3) | (u8current & 0xfc));
}

static __inline HI_U8 hi_isp_ae_ctrl_read(HI_VOID) {
	return (IORD_8DIRECT(0x20e0) & 0x3);
}

/*****************************************************
ISP_AE_CTRL_I                        :     AE更新模式
0x205a20e4                             :     31:1   reserved  
								         0  isp更新模式寄存器：0：reg_newer更新；1：帧更新。  
*****************************************************/
static __inline HI_VOID hi_isp_ae_update_mode_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x20e4); 
	IOWR_8DIRECT(0x20e4, (data & 0x1) | (u8current & 0xfe));
}

static __inline HI_U8 hi_isp_ae_update_mode_read(HI_VOID) {
	return (IORD_8DIRECT(0x20e4) & 0x1);
}

/*****************************************************
ISP_AE_UPDATE                        :     AE寄存器更新寄存器
0x205a20ec                             :     31:1   reserved  
								         0   isp更新寄存器，每帧自动清0
*****************************************************/
static __inline HI_VOID hi_isp_ae_update_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x20ec); 
	IOWR_8DIRECT(0x20ec, (data & 0x1) | (u8current & 0xfe));
}

static __inline HI_U8 hi_isp_ae_update_read(HI_VOID) {
	return (IORD_8DIRECT(0x20ec) & 0x1);
}

/*****************************************************
ISP_AE_SIZE                            :     AE图像大小
0x205a20f0                             :     31:28   reserved  
								    27:16   图像的高度
								    15:12   reserved
								    11:  0   图像的宽度
*****************************************************/
static __inline HI_VOID hi_isp_ae_width_write(HI_U16 data) {
	HI_U16 u16current = IORD_16DIRECT(0x20f0); 
	IOWR_16DIRECT(0x20f0, (data & 0xfff) | (u16current & 0xf000));
}

static __inline HI_U16 hi_isp_ae_width_read(HI_VOID) {
	return (IORD_16DIRECT(0x20f0) & 0xfff);
}

static __inline HI_VOID hi_isp_ae_height_write(HI_U16 data) {
	HI_U16 u16current = IORD_16DIRECT(0x20f2); 
	IOWR_16DIRECT(0x20f2, (data & 0xfff) | (u16current & 0xf000));
}

static __inline HI_U16 hi_isp_ae_height_read(HI_VOID) {
	return (IORD_16DIRECT(0x20f2) & 0xfff);
}

/*****************************************************
MODULE        -----AWB-----
MODULE        -----AWB-----
MODULE        -----AWB-----
*****************************************************/
/*****************************************************
ISP_AWB_CFG:                工作使能寄存器
0x205a2100:                31:1      reserved
                                          0      AWB使能
*****************************************************/
static __inline HI_VOID hi_isp_awb_cfg_enable_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x2100);
    IOWR_8DIRECT(0x2100, (data & 0x01) | (u8current & 0xfe));
}

static __inline HI_U8 hi_isp_awb_cfg_enable_read(HI_VOID) {
    return (IORD_8DIRECT(0x2100) & 0x1);
}

/*****************************************************
ISP_AWB_VERSION:      AWB版本号寄存器
0x205a200c        :      31:20      1:v100, 2:v200, 3:v300 ....
					   19:16      reserved
                                  15:  0      发布日期
*****************************************************/
#define HI_ISP_AWB_VERSION_V100 (1)
static __inline HI_VOID hi_isp_awb_version_write(HI_U16 data) {
	HI_U16 u16current = IORD_16DIRECT(0x210e);
    IOWR_16DIRECT(0x210e, ((data & 0xfff) << 4) | (u16current & 0xf));
}

static __inline HI_U16 hi_isp_awb_version_read(HI_VOID) {
    return ((IORD_16DIRECT(0x210e) & 0xfff0) >> 4);
}

static __inline HI_VOID hi_isp_awb_date_write(HI_U16 data) {
    IOWR_16DIRECT(0x210c, data);
}

static __inline HI_U16 hi_isp_awb_date_read(HI_VOID) {
    return (IORD_16DIRECT(0x210c));
}

/*****************************************************
ISP_AWB_ZONE:             AWB分窗配置
0x205a2110  :                31:13      reserved
                                       12:  8      垂直方向分窗个数
                                       7  :  5      reserved
                                       4  :  0      水平方向分窗个数
*****************************************************/
static __inline HI_VOID hi_isp_awb_h_zone_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x2110);
    IOWR_8DIRECT(0x2110, ((data & 0x1f) | (u8current & 0xe0)));
}

static __inline HI_U8 hi_isp_awb_h_zone_read(HI_VOID) {
    return (IORD_8DIRECT(0x2110) & 0x1f);
}

static __inline HI_VOID hi_isp_awb_v_zone_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x2111);
    IOWR_8DIRECT(0x2111, ((data & 0x1f) | (u8current & 0xe0)));
}

static __inline HI_U8 hi_isp_awb_v_zone_read(HI_VOID) {
    return (IORD_8DIRECT(0x2111) & 0x1f);
}

/*****************************************************
ISP_AWB_THD_MIN:       AWB的RGB最小域值
0x205a2118  :                31:12      reserved
                                       11:  0     Minimal threshold for AWB sums
*****************************************************/
static __inline HI_VOID hi_isp_awb_thd_min_write(HI_U16 data) {
	HI_U16 u16current = IORD_16DIRECT(0x2118);
    IOWR_16DIRECT(0x2118, ((data & 0xfff) | (u16current & 0xf000)));
}

static __inline HI_U16 hi_isp_awb_thd_min_read(HI_VOID) {
    return (IORD_16DIRECT(0x2118) & 0xfff);
}

/*****************************************************
ISP_AWB_THD_MAX:       AWB的RGB最大域值
0x205a211c  :                31:12      reserved
                                       11:  0     Maximal threshold for AWB sums
*****************************************************/
static __inline HI_VOID hi_isp_awb_thd_max_write(HI_U16 data) {
	HI_U16 u16current = IORD_16DIRECT(0x211c);
    IOWR_16DIRECT(0x211c, ((data & 0xfff) | (u16current & 0xf000)));
}

static __inline HI_U16 hi_isp_awb_thd_max_read(HI_VOID) {
    return (IORD_16DIRECT(0x211c) & 0xfff);
}

/*****************************************************
ISP_AWB_CR_MM:       AWB的R/G的参考MAX,MIN值
0x205a2120  :                31:28     reserved
						 27:16     Minimum value of R/G for white region.unsigned 4.8-bit fixed-point
						 15:12     reserved
                                       11:  0     Maximum value of R/G for white region.unsigned 4.8-bit fixed-point
*****************************************************/
static __inline HI_VOID hi_isp_awb_cr_ref_max_write(HI_U16 data) {
	HI_U16 u16current = IORD_16DIRECT(0x2120);
    IOWR_16DIRECT(0x2120, ((data & 0xfff) | (u16current & 0xf000)));
}

static __inline HI_U16 hi_isp_awb_cr_ref_max_read(HI_VOID) {
    return (IORD_16DIRECT(0x2120) & 0xfff);
}

static __inline HI_VOID hi_isp_awb_cr_ref_min_write(HI_U16 data) {
	HI_U16 u16current = IORD_16DIRECT(0x2122);
    IOWR_16DIRECT(0x2122, ((data & 0xfff) | (u16current & 0xf000)));
}

static __inline HI_U16 hi_isp_awb_cr_ref_min_read(HI_VOID) {
    return (IORD_16DIRECT(0x2122) & 0xfff);
}

/*****************************************************
ISP_AWB_CB_MM:       AWB的B/G的参考MAX,MIN值
0x205a2124  :                31:28     reserved
						 27:16     Minimum value of B/G for white region.unsigned 4.8-bit fixed-point
						 15:12     reserved
                                       11:  0     Maximum value of B/G for white region.unsigned 4.8-bit fixed-point
*****************************************************/
static __inline HI_VOID hi_isp_awb_cb_ref_max_write(HI_U16 data) {
	HI_U16 u16current = IORD_16DIRECT(0x2124);
    IOWR_16DIRECT(0x2124, ((data & 0xfff) | (u16current & 0xf000)));
}

static __inline HI_U16 hi_isp_awb_cb_ref_max_read(HI_VOID) {
    return (IORD_16DIRECT(0x2124) & 0xfff);
}

static __inline HI_VOID hi_isp_awb_cb_ref_min_write(HI_U16 data) {
	HI_U16 u16current = IORD_16DIRECT(0x2126);
    IOWR_16DIRECT(0x2126, ((data & 0xfff) | (u16current & 0xf000)));
}

static __inline HI_U16 hi_isp_awb_cb_ref_min_read(HI_VOID) {
    return (IORD_16DIRECT(0x2126) & 0xfff);
}

/*****************************************************
ISP_AWB_TOP_K:           AWB的灰色区域的参考范围的Top斜边斜率
0x205a2130  :                31:25     reserved
						 24:0     AWB的灰色区域的参考范围的斜边斜率.signed 13.12-bit,采用补码配置
*****************************************************/
static __inline HI_VOID hi_isp_awb_top_hypotenuse_k_write(HI_U32 data) {
	HI_U32 u32current = IORD_32DIRECT(0x2130);
    IOWR_32DIRECT(0x2130, ((data & 0x1ffffff) | (u32current & 0xfe000000)));
}

static __inline HI_U32 hi_isp_awb_top_hypotenuse_k_read(HI_VOID) {
    return (IORD_32DIRECT(0x2130) & 0x1ffffff);
}

/*****************************************************
ISP_AWB_TOP_B:          AWB的灰色区域的参考范围的Top斜边截距
0x205a2134  :                31:24     reserved
						 23:0     AWB的灰色区域的参考范围的斜边截距unsigned 16.8-bit
*****************************************************/
static __inline HI_VOID hi_isp_awb_top_hypotenuse_b_write(HI_U32 data) {
	HI_U32 u32current = IORD_32DIRECT(0x2134);
    IOWR_32DIRECT(0x2134, ((data & 0xffffff) | (u32current & 0xff000000)));
}

static __inline HI_U32 hi_isp_awb_top_hypotenuse_b_read(HI_VOID) {
    return (IORD_32DIRECT(0x2134) & 0xffffff);
}

/*****************************************************
ISP_AWB_BOT_K:           AWB的灰色区域的参考范围的Bottom斜边斜率
0x205a2138  :                31:25     reserved
						 24:0     AWB的灰色区域的参考范围的斜边斜率.signed 13.12-bit,采用补码配置
*****************************************************/
static __inline HI_VOID hi_isp_awb_bot_hypotenuse_k_write(HI_U32 data) {
	HI_U32 u32current = IORD_32DIRECT(0x2138);
    IOWR_32DIRECT(0x2138, ((data & 0x1ffffff) | (u32current & 0xfe000000)));
}

static __inline HI_U32 hi_isp_awb_bot_hypotenuse_k_read(HI_VOID) {
    return (IORD_32DIRECT(0x2138) & 0x1ffffff);
}

/*****************************************************
ISP_AWB_BOT_B:          AWB的灰色区域的参考范围的Bottom斜边截距
0x205a213c  :                31:24     reserved
						 23:0     AWB的灰色区域的参考范围的斜边截距unsigned 16.8-bit
*****************************************************/
static __inline HI_VOID hi_isp_awb_bot_hypotenuse_b_write(HI_U32 data) {
	HI_U32 u32current = IORD_32DIRECT(0x213c);
    IOWR_32DIRECT(0x213c, ((data & 0xffffff) | (u32current & 0xff000000)));
}

static __inline HI_U32 hi_isp_awb_bot_hypotenuse_b_read(HI_VOID) {
    return (IORD_32DIRECT(0x213c) & 0xffffff);
}

/*****************************************************
ISP_AWB_AVG_R:          整幅图像的R的均值
0x205a2140  :                31:16     reserved
						 15:0     Normalized sum of Red component
*****************************************************/
static __inline HI_VOID hi_isp_awb_avg_r_write(HI_U16 data) {
    IOWR_16DIRECT(0x2140, data);
}

static __inline HI_U16 hi_isp_awb_avg_r_read(HI_VOID) {
    return IORD_16DIRECT(0x2140);
}

/*****************************************************
ISP_AWB_AVG_G:          整幅图像的G的均值
0x205a2144  :                31:16     reserved
						 15:0     Normalized sum of Green component
*****************************************************/
static __inline HI_VOID hi_isp_awb_avg_g_write(HI_U16 data) {
    IOWR_16DIRECT(0x2144, data);
}

static __inline HI_U16 hi_isp_awb_avg_g_read(HI_VOID) {
    return IORD_16DIRECT(0x2144);
}

/*****************************************************
ISP_AWB_AVG_B:          整幅图像的B的均值
0x205a2148  :                31:16     reserved
						 15:0     Normalized sum of Blue component
*****************************************************/
static __inline HI_VOID hi_isp_awb_avg_b_write(HI_U16 data) {
    IOWR_16DIRECT(0x2148, data);
}

static __inline HI_U16 hi_isp_awb_avg_b_read(HI_VOID) {
    return IORD_16DIRECT(0x2148);
}


/*****************************************************
ISP_AWB_CNT_ALL:        整幅图像的统计点个数
0x205a214c  :                31:16     reserved
						 15:0     Normalized count of pixels which were averaged in Avg R, Avg G and Avg B
*****************************************************/
static __inline HI_VOID hi_isp_awb_count_all_write(HI_U16 data) {
    IOWR_16DIRECT(0x214c, data);
}

static __inline HI_U16 hi_isp_awb_count_all_read(HI_VOID) {
    return IORD_16DIRECT(0x214c);
}

/*****************************************************
ISP_AWB_CNT_MIN:      整幅图像的超过最小阈值的点个数
0x205a2150  :                31:16   reserved
						 15:0     Normalized count of pixels below min threshold
*****************************************************/
static __inline HI_VOID hi_isp_awb_count_min_write(HI_U16 data) {
    IOWR_16DIRECT(0x2150, data);
}

static __inline HI_U16 hi_isp_awb_count_min_read(HI_VOID) {
    return IORD_16DIRECT(0x2150);
}

/*****************************************************
ISP_AWB_CNT_MAX:      整幅图像的超过最大阈值的点个数
0x205a2154  :                31:16   reserved
						 15:0     Normalized count of pixels below max threshold
*****************************************************/
static __inline HI_VOID hi_isp_awb_count_max_write(HI_U16 data) {
    IOWR_16DIRECT(0x2154, data);
}

static __inline HI_U16 hi_isp_awb_count_max_read(HI_VOID) {
    return IORD_16DIRECT(0x2154);
}

/*****************************************************
ISP_AWB_STAT_RADDR:      AWB统计值的读取地址
0x205a2188  :                31:10   reserved
						 9:0      AWB统计信息的地址，具体排列如下:
								0:区块0的AVG G + AVG R
								1:区块0的Count ALL + AVG B
								2:区块0的Count Max + Count Min
								3:区块1的……
								以此类推，共255个区块，地址范围为0~754
*****************************************************/
static __inline HI_VOID hi_isp_awb_stat_raddr_write(HI_U16 data) {
	HI_U16 u16current = IORD_16DIRECT(0x2188);
    IOWR_16DIRECT(0x2188, (data & 0x3ff) | (u16current & 0xfc00));
}

static __inline HI_U16 hi_isp_awb_stat_raddr_read(HI_VOID) {
    return (IORD_16DIRECT(0x2188) & 0x3ff);
}

/*****************************************************
ISP_AWB_STAT_RDATA:      AWB统计值的读返回数据
0x205a218c  :                     31:0   AWB统计信息的读返回值,各个统计值分别占16位
*****************************************************/
static __inline HI_VOID hi_isp_awb_stat_rdata_write(HI_U32 data) {
    IOWR_32DIRECT(0x218c, data);
}

static __inline HI_U32 hi_isp_awb_stat_rdata_read(HI_VOID) {
    return IORD_32DIRECT(0x218c);
}

/*****************************************************
ISP_AWB_CTRL_F:      AWB普通控制寄存器
0x205a21e0  :                     31:2   reserved
							  1:0  RGGBstart；0：R Gr Gb B；1：Gr R B Gb；2：Gb B R Gr；3：B Gb Gr R；
*****************************************************/
static __inline HI_VOID hi_isp_awb_rggb_cfg_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x21e0);
    IOWR_8DIRECT(0x21e0, (data & 0x3) | (u8current & 0xfc));
}

static __inline HI_U8 hi_isp_awb_rggb_cfg_read(HI_VOID) {
    return (IORD_8DIRECT(0x21e0) & 0x3);
}

/*****************************************************
ISP_AWB_CTRL_I:      AWB普通控制寄存器
0x205a21e4  :                     31:1   reserved
							  0:0  isp更新模式寄存器：0：reg_newer更新；1：帧更新。
*****************************************************/
static __inline HI_VOID hi_isp_awb_update_mode_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x21e4);
    IOWR_8DIRECT(0x21e4, (data & 0x1) | (u8current & 0xfe));
}

static __inline HI_U8 hi_isp_awb_update_mode_read(HI_VOID) {
    return (IORD_8DIRECT(0x21e4) & 0x1);
}

/*****************************************************
ISP_AWB_UPDATE:      AWB寄存器更新寄存器
0x205a21ec  :                     31:1   reserved
							  0:0  isp更新寄存器，每帧自动清0
*****************************************************/
static __inline HI_VOID hi_isp_awb_update_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x21ec);
    IOWR_8DIRECT(0x21ec, (data & 0x1) | (u8current & 0xfe));
}

static __inline HI_U8 hi_isp_awb_update_read(HI_VOID) {
    return (IORD_8DIRECT(0x21ec) & 0x1);
}

/*****************************************************
ISP_AWB_SIZE:      AWB图像大小
0x205a21f0  :         31:28   reserved
					27:16  图像的高度
					15:12   reserved
					11:0    图像的宽度
*****************************************************/
static __inline HI_VOID hi_isp_awb_hsize_write(HI_U16 data) {
	HI_U16 u16current = IORD_16DIRECT(0x21f0);
    IOWR_16DIRECT(0x21f0, (data & 0xfff) | (u16current & 0xf000));
}

static __inline HI_U16 hi_isp_awb_hsize_read(HI_VOID) {
    return (IORD_16DIRECT(0x21f0) & 0xfff);
}

static __inline HI_VOID hi_isp_awb_vsize_write(HI_U16 data) {
	HI_U16 u16current = IORD_16DIRECT(0x21f2);
    IOWR_16DIRECT(0x21f2, (data & 0xfff) | (u16current & 0xf000));
}

static __inline HI_U16 hi_isp_awb_vsize_read(HI_VOID) {
    return (IORD_16DIRECT(0x21f2) & 0xfff);
}

/*****************************************************
ISP_WB_CFG:                工作使能寄存器
0x205a1100:                31:1      reserved
                                          0      WB使能
*****************************************************/
static __inline HI_VOID hi_isp_wb_enable_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x1100);
    IOWR_8DIRECT(0x1100, (data & 0x01) | (u8current & 0xfe));
}

static __inline HI_U8 hi_isp_wb_enable_read(HI_VOID) {
    return (IORD_8DIRECT(0x1100) & 0x1);
}

/*****************************************************
MODULE        -----CC-----
MODULE        -----CC-----
MODULE        -----CC-----
*****************************************************/
/*****************************************************
ISP_CC_CFG:                工作使能寄存器
0x205a1200:                31:1      reserved
                                          0      cc使能
*****************************************************/
static __inline HI_VOID hi_isp_cc_enable_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x1200);
    IOWR_8DIRECT(0x1200, (data & 0x01) | (u8current & 0xfe));
}

static __inline HI_U8 hi_isp_cc_enable_read(HI_VOID) {
    return (IORD_8DIRECT(0x1200) & 0x1);
}

#define HI_ISP_CC_IN_DC0_DEFAULT (0)
#define HI_ISP_CC_IN_DC1_DEFAULT (0)
#define HI_ISP_CC_IN_DC2_DEFAULT (0)
/*****************************************************
ISP_CC_IN_DC0:       CSC输入直流分量寄存器0。
0x205a1230:            31:21     in_dc1
                                20:16      reserved
                                15:05      in_dc0
                                04:00      reserved
*****************************************************/

static __inline HI_VOID hi_isp_cc_in_dc1_write(HI_U16 data) {
    IOWR_16DIRECT(0x1232, ((data & 0x7ff) << 5));
}

static __inline HI_U16 hi_isp_cc_in_dc1_read(HI_VOID) {
    return ((IORD_16DIRECT(0x1232) & 0xffe0) >> 5);
}

static __inline HI_VOID hi_isp_cc_in_dc0_write(HI_U16 data) {
    IOWR_16DIRECT(0x1230, ((data & 0x7ff) << 5));
}

static __inline HI_U16 hi_isp_cc_in_dc0_read(HI_VOID) {
    return ((IORD_16DIRECT(0x1230) & 0xffe0) >> 5);
}

/*****************************************************
ISP_CC_IN_DC1:         CSC输入直流分量寄存器1。
0x205a1234:              31:16      reserved
                                  15:05      in_dc2
                                  04:00      reserved
*****************************************************/
static __inline HI_VOID hi_isp_cc_in_dc2_write(HI_U16 data) {
    IOWR_16DIRECT(0x1234, ((data & 0x7ff) << 5));
}

static __inline HI_U16 hi_isp_cc_in_dc2_read(HI_VOID) {
    return ((IORD_16DIRECT(0x1234) & 0xffe0) >> 5);
}

#define HI_ISP_CC_OUT_DC0_DEFAULT (0)
#define HI_ISP_CC_OUT_DC1_DEFAULT (0)
#define HI_ISP_CC_OUT_DC2_DEFAULT (0)
/*****************************************************
ISP_CC_OUT_DC0:         CSC输出直流分量寄存器0。
0x205a1240:                 31:21      out_dc1
                                     20:16      reserved
                                     15:05      out_dc0
                                     04:00      reserved
*****************************************************/
static __inline HI_VOID hi_isp_cc_out_dc1_write(HI_U16 data) {
    IOWR_16DIRECT(0x1242, ((data & 0x7ff) << 5));
}

static __inline HI_U16 hi_isp_cc_out_dc1_read(HI_VOID) {
    return ((IORD_16DIRECT(0x1242) & 0xffe0) >> 5);
}

static __inline HI_VOID hi_isp_cc_out_dc0_write(HI_U16 data) {
    IOWR_16DIRECT(0x1240, ((data & 0x7ff) << 5));
}

static __inline HI_U16 hi_isp_cc_out_dc0_read(HI_VOID) {
    return ((IORD_16DIRECT(0x1240) & 0xffe0) >> 5);
}

/*****************************************************
ISP_CC_OUT_DC1:         CSC输出直流分量寄存器1。
0x205a1244:                 31:16      reserved 
                                     15:05      out_dc2
                                     04:00      reserved
*****************************************************/
static __inline HI_VOID hi_isp_cc_out_dc2_write(HI_U16 data) {
    IOWR_16DIRECT(0x1244, ((data & 0x7ff) << 5));
}

static __inline HI_U16 hi_isp_cc_out_dc2_read(HI_VOID) {
    return ((IORD_16DIRECT(0x1244) & 0xffe0) >> 5);
}

/*****************************************************
MODULE        -----CAC-----
MODULE        -----CAC-----
MODULE        -----CAC-----
*****************************************************/
/*****************************************************
ISP_CAC_CFG:         工作使能寄存器
0x205a5900:                     31:01      reserved
                                          00:00      cac使能
*****************************************************/
static __inline HI_VOID hi_isp_cac_enable_write(HI_U8 data) {
    IOWR_8DIRECT(0x5900, (data & 0x1));
}

static __inline HI_U8 hi_isp_cac_enable_read(HI_VOID) {
    return (IORD_8DIRECT(0x5900) & 0x1);
}

/*****************************************************
ISP_CAC_CFG1:         工作使能寄存器
0x205a5904:                     31:01      reserved
                                          00:00      inter_en使能
*****************************************************/
static __inline HI_VOID hi_isp_cac_inter_enable_write(HI_U8 data) {
    IOWR_8DIRECT(0x5904, (data & 0x1));
}

static __inline HI_U8 hi_isp_cac_inter_enable_read(HI_VOID) {
    return (IORD_8DIRECT(0x5904) & 0x1);
}


/*****************************************************
ISP_CAC_VERSION:         ISP版本,1:V100；2：V200；3：V300依此类推
0x205a590c:                     31:20      version
                                          19:16      reserved
                                          15: 0       date
*****************************************************/
static __inline HI_VOID hi_isp_cac_version_write(HI_U16 data) {
	HI_U16 u16current = IORD_16DIRECT(0x590e);
    IOWR_16DIRECT(0x590e, (data << 4) | (u16current&0xf));
}

static __inline HI_U16 hi_isp_cac_version_read(HI_VOID) {
    return (IORD_16DIRECT(0x590e) >> 4);
}

static __inline HI_VOID hi_isp_cac_date_write(HI_U16 data) {
    IOWR_16DIRECT(0x590c, data);
}

static __inline HI_U16 hi_isp_cac_date_read(HI_VOID) {
    return (IORD_16DIRECT(0x590c));
}

/*****************************************************
ISP_CAC_LUT_SAMPLE_RATIO:   CAC LUT采样率寄存器。
0x205a5914:                     31:20      reserved
                                          19:16      sample_ratio_v
                                          15:04      reserved
                                          03:00      sample_ratio_h
*****************************************************/
static __inline HI_VOID hi_isp_cac_sample_ratio_v_write(HI_U8 data) {
    IOWR_8DIRECT(0x5916, (data & 0xf));
}

static __inline HI_U8 hi_isp_cac_sample_ratio_v_read(HI_VOID) {
    return (IORD_8DIRECT(0x5916) & 0xf);
}

static __inline HI_VOID hi_isp_cac_sample_ratio_h_write(HI_U8 data) {
    IOWR_8DIRECT(0x5914, (data & 0xf));
}

static __inline HI_U8 hi_isp_cac_sample_ratio_h_read(HI_VOID) {
    return (IORD_8DIRECT(0x5914) & 0xf);
}

/*****************************************************
ISP_CAC_H_WADDR:       ISP_CAC_H_WADDR为CAC R、B通道水平插值写地址寄存器。
0x205a5980:                     31:00     cac_h_waddr
*****************************************************/
static __inline HI_VOID hi_isp_cac_h_waddr_write(HI_U32 data) { 
    IOWR_32DIRECT(0x5980, data);
}

static __inline HI_U32 hi_isp_cac_h_waddr_read(HI_VOID) {
    return (IORD_32DIRECT(0x5980));
}

/*****************************************************
ISP_CAC_H_WDATA     ISP_CAC_H_WDATAC R,B通道水平插值写地址寄存器。
0x205a5984:                     31:25     reserved
						    24:16     cac_bh_wdata
						    15:09     reserved
						    08:00     cac_rh_wdata
*****************************************************/
static __inline HI_VOID hi_isp_cac_h_wdata_write(HI_U32 data) { 
    IOWR_32DIRECT(0x5984, data);
}

static __inline HI_U32 hi_isp_cac_h_wdata_read(HI_VOID) {
    return (IORD_32DIRECT(0x5984));
}

/*****************************************************
ISP_CAC_H_RADDR     ISP_CAC_H_RADDR为CAC R,B通道水平插值读地址寄存器。
0x205a5988:                 31:00     cac_h_raddr
*****************************************************/
static __inline HI_VOID hi_isp_cac_h_raddr_write(HI_U32 data) { 
    IOWR_32DIRECT(0x5988, data);
}

static __inline HI_U32 hi_isp_cac_h_raddr_read(HI_VOID) {
    return (IORD_32DIRECT(0x5988));
}

/*****************************************************
ISP_CAC_H_RDATA     ISP_CAC_H_RDATA为CAC R通道水平插值读数据寄存器。
0x205a598c:               31:25     reserved
					    24:16     cac_bh_rdata
					    15:09     reserved
					    08:00     cac_rh_rdata
*****************************************************/
static __inline HI_VOID hi_isp_cac_h_rdata_write(HI_U32 data) { 
    IOWR_32DIRECT(0x598c, data);
}

static __inline HI_U32 hi_isp_cac_h_rdata_read(HI_VOID) {
    return (IORD_32DIRECT(0x598c));
}

/*****************************************************
ISP_CAC_V_WADDR    ISP_CAC_V_WADDR为CAC R,B通道水平插值写地址寄存器。
0x205a5990:                 31:00     cac_v_waddr
*****************************************************/
static __inline HI_VOID hi_isp_cac_v_waddr_write(HI_U32 data) { 
    IOWR_32DIRECT(0x5990, data);
}

static __inline HI_U32 hi_isp_cac_v_waddr_read(HI_VOID) {
    return (IORD_32DIRECT(0x5990));
}

/*****************************************************
ISP_CAC_V_WDATA    ISP_CAC_V_WDATA为CAC R, B通道水平插值写数据寄存器。
0x205a5994:               31:25     reserved
					    24:16     cac_bv_wdata
					    15:09     reserved
					    08:00     cac_rv_wdata
*****************************************************/
static __inline HI_VOID hi_isp_cac_v_wdata_write(HI_U32 data) { 
    IOWR_32DIRECT(0x5994, data);
}

static __inline HI_U32 hi_isp_cac_v_wdata_read(HI_VOID) {
    return (IORD_32DIRECT(0x5994));
}

/*****************************************************
ISP_CAC_V_RADDR    ISP_CAC_V_RADDR为CAC R, B通道水平插值读地址寄存器。
0x205a5998:                 31:00     cac_v_raddr
*****************************************************/
static __inline HI_VOID hi_isp_cac_v_raddr_write(HI_U32 data) { 
    IOWR_32DIRECT(0x5998, data);
}

static __inline HI_U32 hi_isp_cac_v_raddr_read(HI_VOID) {
    return (IORD_32DIRECT(0x5998));
}

/*****************************************************
ISP_CAC_V_RDATA     ISP_CAC_V_RDATA为CAC R, B通道水平插值读数据寄存器。
0x205a599c:               31:25     reserved
					    24:16     cac_bv_rdata
					    15:09     reserved
					    08:00     cac_rv_rdata
*****************************************************/
static __inline HI_VOID hi_isp_cac_v_rdata_write(HI_U32 data) { 
    IOWR_32DIRECT(0x599c, data);
}

static __inline HI_U32 hi_isp_cac_v_rdata_read(HI_VOID) {
    return (IORD_32DIRECT(0x599c));
}

/*****************************************************
MODULE        -----COMPANDER-----
MODULE        -----COMPANDER-----
MODULE        -----COMPANDER-----
*****************************************************/
/*****************************************************
ISP_COMPANDER_CFG:         工作使能寄存器
0x205a7500:                     31:1      reserved
                                          0           COMPANDER使能
*****************************************************/
static __inline HI_VOID hi_isp_compander_enable_write(HI_U8 data) {
    IOWR_8DIRECT(0x7500, data);
}

static __inline HI_U8 hi_isp_compander_enable_read(HI_VOID) {
    return (IORD_8DIRECT(0x7500));
}

/*****************************************************
ISP_GAMMAFE_VERSION:  COMPANDER版本号寄存器
0x205a750c:                     31:20      version
                                          19:16      reserved
                                          15: 0       date
*****************************************************/
static __inline HI_VOID hi_isp_compander_version_write(HI_U16 data) {
	HI_U16 u16current = IORD_16DIRECT(0x400e);
    IOWR_16DIRECT(0x750e, (data << 4) | (u16current&0xf));
}

static __inline HI_U16 hi_isp_compander_version_read(HI_VOID) {
    return (IORD_16DIRECT(0x750e) >> 4);
}

static __inline HI_VOID hi_isp_compander_date_write(HI_U16 data) {
    IOWR_16DIRECT(0x750c, data);
}

static __inline HI_U16 hi_isp_compander_date_read(HI_VOID) {
    return (IORD_16DIRECT(0x750c));
}

/*****************************************************
ISP_COMPANDER_BITW:   COMPANDER数据位宽控制寄存器
0x205a7510:                     31:21      reserved
                                          20:16      bitw_in
                                          15:05      reserved
                                          04:00      bitw_out
*****************************************************/
static __inline HI_VOID hi_isp_compander_bitw_in_write(HI_U8 data) {
    IOWR_8DIRECT(0x7512, (data & 0x1f));
}

static __inline HI_U8 hi_isp_compander_bitw_in_read(HI_VOID) {
    return (IORD_8DIRECT(0x7512) & 0x1f);
}

static __inline HI_VOID hi_isp_compander_bitw_out_write(HI_U8 data) {
    IOWR_8DIRECT(0x7510, (data & 0x1f));
}

static __inline HI_U8 hi_isp_compander_bitw_out_read(HI_VOID) {
    return (IORD_8DIRECT(0x7510) & 0x1f);
}

/*****************************************************
ISP_COMPANDER_OFFSETR:   COMPANDER为R通道数据偏移值配置寄存器
0x205a7514:                     31:20      reserved
                                          19:00      R通道偏移值配置寄存器。
*****************************************************/
static __inline HI_VOID hi_isp_compander_OFFSETR_in_write(HI_U32 data) {
    IOWR_32DIRECT(0x7514, (data & 0xfffff));
}

static __inline HI_U32 hi_isp_compander_OFFSETR_in_read(HI_VOID) {
    return (IORD_32DIRECT(0x7514) & 0xfffff);
}

/*****************************************************
ISP_COMPANDER_OFFSETGR:   COMPANDER为GR通道数据偏移值配置寄存器
0x205a7518:                     31:20      reserved
                                          19:00      GR通道偏移值配置寄存器。
*****************************************************/
static __inline HI_VOID hi_isp_compander_OFFSETGR_in_write(HI_U32 data) {
    IOWR_32DIRECT(0x7518, (data & 0xfffff));
}

static __inline HI_U32 hi_isp_compander_OFFSETGR_in_read(HI_VOID) {
    return (IORD_32DIRECT(0x7518) & 0xfffff);
}

/*****************************************************
ISP_COMPANDER_OFFSETGB:   COMPANDER为GB通道数据偏移值配置寄存器
0x205a751c:                     31:20      reserved
                                          19:00      GB通道偏移值配置寄存器。
*****************************************************/
static __inline HI_VOID hi_isp_compander_OFFSETGB_in_write(HI_U32 data) {
    IOWR_32DIRECT(0x751c, (data & 0xfffff));
}

static __inline HI_U32 hi_isp_compander_OFFSETGB_in_read(HI_VOID) {
    return (IORD_32DIRECT(0x751c) & 0xfffff);
}

/*****************************************************
ISP_COMPANDER_OFFSETB:   COMPANDER为B通道数据偏移值配置寄存器
0x205a7520:                     31:20      reserved
                                          19:00      B通道偏移值配置寄存器。
*****************************************************/
static __inline HI_VOID hi_isp_compander_OFFSETB_in_write(HI_U32 data) {
    IOWR_32DIRECT(0x7520, (data & 0xfffff));
}

static __inline HI_U32 hi_isp_compander_OFFSETB_in_read(HI_VOID) {
    return (IORD_32DIRECT(0x7520) & 0xfffff);
}

/*****************************************************
ISP_COMPANDER_WADDR:         COMPANDER的129长度LUT的写地址。
0x205a7580:                     31:00      compander_waddr
*****************************************************/
static __inline HI_VOID hi_isp_compander_waddr_write(HI_U32 data) {
    IOWR_32DIRECT(0x7580, data);
}

static __inline HI_U32 hi_isp_compander_waddr_read(HI_VOID) {
    return (IORD_32DIRECT(0x7580));
}

/*****************************************************
ISP_COMPANDER_WDATA:  COMPANDER的129长度LUT的写数据。
0x205a7584:                     31:00      compander_wdata
*****************************************************/
static __inline HI_VOID hi_isp_compander_wdata_write(HI_U32 data) {
    IOWR_32DIRECT(0x7584, data);
}

static __inline HI_U32 hi_isp_compander_wdata_read(HI_VOID) {
    return (IORD_32DIRECT(0x7584));
}

/*****************************************************
ISP_COMPANDER_RADDR:         COMPANDER的129长度LUT的读地址。
0x205a7588:                     31:00      compander_raddr
*****************************************************/
static __inline HI_VOID hi_isp_compander_raddr_write(HI_U32 data) {
    IOWR_32DIRECT(0x7588, data);
}

static __inline HI_U32 hi_isp_compander_raddr_read(HI_VOID) {
    return (IORD_32DIRECT(0x7588));
}

/*****************************************************
ISP_COMPANDER_RDATA:  COMPANDER的129长度LUT的读数据。
0x205a758c:                     31:00      compander_rdata
*****************************************************/
static __inline HI_VOID hi_isp_compander_rdata_write(HI_U32 data) {
    IOWR_32DIRECT(0x758c, data);
}

static __inline HI_U32 hi_isp_compander_rdata_read(HI_VOID) {
    return (IORD_32DIRECT(0x758c));
}

/*****************************************************
MODULE        -----CSC-----
MODULE        -----CSC-----
MODULE        -----CSC-----
*****************************************************/
/*****************************************************
ISP_CSC_CFG:         工作使能寄存器
0x205a1300:                     31:1      reserved
                                          0          CSC使能。0：禁止；1：使能；
*****************************************************/
static __inline HI_VOID hi_isp_csc_enable_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x1300);
    IOWR_8DIRECT(0x1300, (data & 0x01) | (u8current & 0xfe));
}

static __inline HI_U8 hi_isp_csc_enable_read(HI_VOID) {
    return (IORD_8DIRECT(0x1300) & 0x1);
}

/*****************************************************
ISP_CSC_COEF0:         工作使能寄存器
0x205a1310:                     31:17      coef01
                                          16:16      reserved
                                          15:01      coef00
                                            0:  0      reserved
*****************************************************/
static __inline HI_VOID hi_isp_csc_coef01_write(HI_U16 data) {
    IOWR_16DIRECT(0x1312, ((data & 0x7fff) << 1));
}

static __inline HI_U16 hi_isp_csc_coef01_read(HI_VOID) {
    return ((IORD_16DIRECT(0x1312) & 0xfffe) >> 1);
}

static __inline HI_VOID hi_isp_csc_coef00_write(HI_U16 data) {
    IOWR_16DIRECT(0x1310, ((data & 0x7fff) << 1));
}

static __inline HI_U16 hi_isp_csc_coef00_read(HI_VOID) {
    return ((IORD_16DIRECT(0x1310) & 0xfffe) >> 1);
}

/*****************************************************
ISP_CSC_COEF1:         工作使能寄存器
0x205a1314:                     31:17      coef10
                                          16:16      reserved
                                          15:01      coef02
                                            0:  0      reserved
*****************************************************/
static __inline HI_VOID hi_isp_csc_coef10_write(HI_U16 data) {
    IOWR_16DIRECT(0x1316, ((data & 0x7fff) << 1));
}

static __inline HI_U16 hi_isp_csc_coef10_read(HI_VOID) {
    return ((IORD_16DIRECT(0x1316) & 0xfffe) >> 1);
}

static __inline HI_VOID hi_isp_csc_coef02_write(HI_U16 data) {
    IOWR_16DIRECT(0x1314, ((data & 0x7fff) << 1));
}

static __inline HI_U16 hi_isp_csc_coef02_read(HI_VOID) {
    return ((IORD_16DIRECT(0x1314) & 0xfffe) >> 1);
}

/*****************************************************
ISP_CSC_COEF2:         工作使能寄存器
0x205a1318:                     31:17      coef12
                                          16:16      reserved
                                          15:01      coef11
                                          00:00      reserved
*****************************************************/
static __inline HI_VOID hi_isp_csc_coef12_write(HI_U16 data) {
    IOWR_16DIRECT(0x131a, ((data & 0x7fff) << 1));
}

static __inline HI_U16 hi_isp_csc_coef12_read(HI_VOID) {
    return ((IORD_16DIRECT(0x131a) & 0xfffe) >> 1);
}

static __inline HI_VOID hi_isp_csc_coef11_write(HI_U16 data) {
    IOWR_16DIRECT(0x1318, ((data & 0x7fff) << 1));
}

static __inline HI_U16 hi_isp_csc_coef11_read(HI_VOID) {
    return ((IORD_16DIRECT(0x1318) & 0xfffe) >> 1);
}

/*****************************************************
ISP_CSC_COEF3:         工作使能寄存器
0x205a131c:                     31:17      coef21
                                          16:16      reserved
                                          15:01      coef20
                                          00:00      reserved
*****************************************************/
static __inline HI_VOID hi_isp_csc_coef21_write(HI_U16 data) {
    IOWR_16DIRECT(0x131e, ((data & 0x7fff) << 1));
}

static __inline HI_U16 hi_isp_csc_coef21_read(HI_VOID) {
    return ((IORD_16DIRECT(0x131e) & 0xfffe) >> 1);
}

static __inline HI_VOID hi_isp_csc_coef20_write(HI_U16 data) {
    IOWR_16DIRECT(0x131c, ((data & 0x7fff) << 1));
}

static __inline HI_U16 hi_isp_csc_coef20_read(HI_VOID) {
    return ((IORD_16DIRECT(0x131c) & 0xfffe) >> 1);
}

/*****************************************************
ISP_CSC_COEF4:         工作使能寄存器
0x205a1320:                     31:17      reserved
                                          16:16      reserved
                                          15:01      coef22
                                          00:00      reserved
*****************************************************/
static __inline HI_VOID hi_isp_csc_coef22_write(HI_U16 data) {
    IOWR_16DIRECT(0x1320, ((data & 0x7fff) << 1));
}

static __inline HI_U16 hi_isp_csc_coef22_read(HI_VOID) {
    return ((IORD_16DIRECT(0x1320) & 0xfffe) >> 1);
}

/*****************************************************
ISP_CSC_IN_DC0:         工作使能寄存器
0x205a1330:                     31:21      in_dc1
                                          20:16      reserved
                                          15:05      in_dc0
                                            4:  0      reserved
*****************************************************/
static __inline HI_VOID hi_isp_csc_in_dc1_write(HI_U16 data) {
    IOWR_16DIRECT(0x1332, ((data & 0x7ff) << 5));
}

static __inline HI_U16 hi_isp_csc_in_dc1_read(HI_VOID) {
    return ((IORD_16DIRECT(0x1332) & 0xfffe) >> 5);
}

static __inline HI_VOID hi_isp_csc_in_dc0_write(HI_U16 data) {
    IOWR_16DIRECT(0x1330, ((data & 0x7ff) << 5));
}

static __inline HI_U16 hi_isp_csc_in_dc0_read(HI_VOID) {
    return ((IORD_16DIRECT(0x1330) & 0xfffe) >> 5);
}

/*****************************************************
ISP_CSC_IN_DC1:         工作使能寄存器
0x205a1334:                     31:21      reserved
                                          20:16      reserved
                                          15:05      in_dc2
                                            4:  0      reserved
*****************************************************/
static __inline HI_VOID hi_isp_csc_in_dc2_write(HI_U16 data) {
    IOWR_16DIRECT(0x1334, ((data & 0x7ff) << 5));
}

static __inline HI_U16 hi_isp_csc_in_dc2_read(HI_VOID) {
    return ((IORD_16DIRECT(0x1334) & 0xfffe) >> 5);
}

/*****************************************************
ISP_CSC_OUT_DC0:         工作使能寄存器
0x205a1340:                     31:21      out_dc1
                                          20:16      reserved
                                          15:05      out_dc0
                                            4:  0      reserved
*****************************************************/
static __inline HI_VOID hi_isp_csc_out_dc1_write(HI_U16 data) {
    IOWR_16DIRECT(0x1342, ((data & 0x7ff) << 5));
}

static __inline HI_U16 hi_isp_csc_out_dc1_read(HI_VOID) {
    return ((IORD_16DIRECT(0x1342) & 0xfffe) >> 5);
}

static __inline HI_VOID hi_isp_csc_out_dc0_write(HI_U16 data) {
    IOWR_16DIRECT(0x1340, ((data & 0x7ff) << 5));
}

static __inline HI_U16 hi_isp_csc_out_dc0_read(HI_VOID) {
    return ((IORD_16DIRECT(0x1340) & 0xfffe) >> 5);
}

/*****************************************************
ISP_CSC_OUT_DC0:         工作使能寄存器
0x205a1344:                     31:21      reserved
                                          20:16      reserved
                                          15:05      out_dc2
                                            4:  0      reserved
*****************************************************/
static __inline HI_VOID hi_isp_csc_out_dc2_write(HI_U16 data) {
    IOWR_16DIRECT(0x1344, ((data & 0x7ff) << 5));
}

static __inline HI_U16 hi_isp_csc_out_dc2_read(HI_VOID) {
    return ((IORD_16DIRECT(0x1344) & 0xfffe) >> 5);
}

/*****************************************************
MODULE        -----DEMOSAIC-----
MODULE        -----DEMOSAIC-----
MODULE        -----DEMOSAIC-----
*****************************************************/
/*****************************************************
ISP_DEMOSAIC_CFG:         工作使能寄存器
0x205a3000:                     31:1      reserved
                                          0      DEMOSAIC使能
*****************************************************/
static __inline HI_VOID hi_isp_demosaic_cfg_enable_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x3000);
    IOWR_8DIRECT(0x3000, (data & 0x01) | (u8current & 0xfe));
}

static __inline HI_U8 hi_isp_demosaic_cfg_enable_read(HI_VOID) {
    return (IORD_8DIRECT(0x3000) & 0x1);
}

/*****************************************************
ISP_DEMOSAIC_CFG1:         工作使能寄存器
0x205a3004:                     31:17      reserved
                                          16:16      fcr使能
                                          15: 1       reserved
                                          0  : 0       ahd使能
*****************************************************/
static __inline HI_VOID hi_isp_demosaic_fcr_enable_write(HI_U8 data) {
    IOWR_8DIRECT(0x3006, data);
}

static __inline HI_U8 hi_isp_demosaic_fcr_enable_read(HI_VOID) {
    return (IORD_8DIRECT(0x3006));
}

static __inline HI_VOID hi_isp_demosaic_ahd_enable_write(HI_U8 data) {
    IOWR_8DIRECT(0x3004, data);
}

static __inline HI_U8 hi_isp_demosaic_ahd_enable_read(HI_VOID) {
    return (IORD_8DIRECT(0x3004));
}

/*****************************************************
ISP_DEMOSAIC_VERSION:         工作使能寄存器
0x205a300c:                     31:20      ISP版本,1:V100；2：V200；3：V300依此类推
                                          19:16      保留
                                          15:  0      发布日期。
*****************************************************/

static __inline HI_VOID hi_isp_demosaic_version_write(HI_U16 data) {
	HI_U16 u16current = IORD_16DIRECT(0x300e);
    IOWR_16DIRECT(0x300e, (data & 0xfff) << 4 | (u16current & 0xf));
}

static __inline HI_U16 hi_isp_demosaic_version_read(HI_VOID) {
    return ((IORD_16DIRECT(0x300e) & 0xfff) >> 4);
}

static __inline HI_VOID hi_isp_demosaic_date_write(HI_U16 data) {
    IOWR_16DIRECT(0x300c, data);
}

static __inline HI_U16 hi_isp_demosaic_date_read(HI_VOID) {
    return (IORD_16DIRECT(0x300c));
}

/*****************************************************
ISP_DEMOSAIC_COEF0:         工作使能寄存器
0x205a3010:                     31:24      保留
                                          23:16      bld_limit1
                                          15:  8      保留
                                            7:  0       bld_limit2
*****************************************************/

static __inline HI_VOID hi_isp_demosaic_bld_limit1_write(HI_U8 data) {
    IOWR_8DIRECT(0x3012, data);
}

static __inline HI_U8 hi_isp_demosaic_bld_limit1_read(HI_VOID) {
    return (IORD_8DIRECT(0x3012));
}

static __inline HI_VOID hi_isp_demosaic_bld_limit2_write(HI_U8 data) {
    IOWR_8DIRECT(0x3010, data);
}

static __inline HI_U8 hi_isp_demosaic_bld_limit2_read(HI_VOID) {
    return (IORD_8DIRECT(0x3010));
}

/*****************************************************
ISP_DEMOSAIC_COEF1:         工作使能寄存器
0x205a3014:                     31:25      保留
						    24:16       bld_ratio1
						    15:9         保留
						      8:0         bld_ratio2
*****************************************************/

static __inline HI_VOID hi_isp_demosaic_bld_ratio1_write(HI_U16 data) {
    IOWR_16DIRECT(0x3016, (data & 0x1ff));
}

static __inline HI_U16 hi_isp_demosaic_bld_ratio1_read(HI_VOID) {
    return (IORD_16DIRECT(0x3016)& 0x1ff);
}

static __inline HI_VOID hi_isp_demosaic_bld_ratio2_write(HI_U16 data) {
    IOWR_16DIRECT(0x3014, (data & 0x1ff));
}

static __inline HI_U16 hi_isp_demosaic_bld_ratio2_read(HI_VOID) {
    return (IORD_16DIRECT(0x3014)& 0x1ff);
}


/*****************************************************
ISP_DEMOSAIC_COEF2:         工作使能寄存器
0x205a3018:                     31:28      保留
						    27:16      fcr_limit1
						    15:12      保留
						    11:  0      fcr_limit2
*****************************************************/

static __inline HI_VOID hi_isp_demosaic_fcr_limit1_write(HI_U16 data) {
    IOWR_16DIRECT(0x301a, (data & 0xfff));
}

static __inline HI_U16 hi_isp_demosaic_fcr_limit1_read(HI_VOID) {
    return (IORD_16DIRECT(0x301a)& 0xfff);
}

static __inline HI_VOID hi_isp_demosaic_fcr_limit2_write(HI_U16 data) {
    IOWR_16DIRECT(0x3018, (data & 0xfff));
}

static __inline HI_U16 hi_isp_demosaic_fcr_limit2_read(HI_VOID) {
    return (IORD_16DIRECT(0x3018)& 0xfff);
}

/*****************************************************
ISP_DEMOSAIC_COEF3:         工作使能寄存器
0x205a301c:                     31:26      保留
						    25:16      ahd_par1
						    15:  8      保留
						      7:  0      ahd_par2
*****************************************************/

static __inline HI_VOID hi_isp_demosaic_ahd_par1_write(HI_U16 data) {
    IOWR_16DIRECT(0x301e, data);
}

static __inline HI_U16 hi_isp_demosaic_ahd_par1_read(HI_VOID) {
    return (IORD_16DIRECT(0x301e));
}

static __inline HI_VOID hi_isp_demosaic_ahd_par2_write(HI_U8 data) {
    IOWR_8DIRECT(0x301c, data);
}

static __inline HI_U8 hi_isp_demosaic_ahd_par2_read(HI_VOID) {
    return (IORD_8DIRECT(0x301c));
}


/*****************************************************
ISP_DEMOSAIC_COEF4:         工作使能寄存器
0x205a3020:                     31:28      保留
						    27:16      fcr_thr
						    15:  5      保留
						      4:  0      fcr_gain
*****************************************************/

static __inline HI_VOID hi_isp_demosaic_fcr_thr_write(HI_U16 data) {
    IOWR_16DIRECT(0x3022, (data & 0xfff));
}

static __inline HI_U16 hi_isp_demosaic_fcr_thr_read(HI_VOID) {
    return (IORD_16DIRECT(0x3022) & 0xfff);
}

static __inline HI_VOID hi_isp_demosaic_fcr_gain_write(HI_U8 data) {
    IOWR_8DIRECT(0x3020, (data & 0x1f));
}

static __inline HI_U8 hi_isp_demosaic_fcr_gain_read(HI_VOID) {
    return (IORD_8DIRECT(0x3020) & 0x1f);
}


/*****************************************************
ISP_DEMOSAIC_COEF5:         工作使能寄存器
0x205a3024:                     31:8      保留
						    7:  0        hv_ratio
*****************************************************/

static __inline HI_VOID hi_isp_demosaic_hv_ratio_write(HI_U8 data) {
    IOWR_8DIRECT(0x3024, data);
}

static __inline HI_U8 hi_isp_demosaic_hv_ratio_read(HI_VOID) {
    return (IORD_8DIRECT(0x3024));
}

/*****************************************************
ISP_DEMOSAIC_COEF6:         工作使能寄存器
0x205a3028:                     31:06      保留
						    05:00      fcr_ratio
*****************************************************/

static __inline HI_VOID hi_isp_demosaic_fcr_ratio_write(HI_U8 data) {
    IOWR_8DIRECT(0x3028, (data & 0x3f));
}

static __inline HI_U8 hi_isp_demosaic_fcr_ratio_read(HI_VOID) {
    return (IORD_8DIRECT(0x3028) & 0x3f);
}

/*****************************************************
ISP_DEMOSAIC_COEF6:         工作使能寄存器
0x205a3030:                     31:24      保留
						      1:  0      hv_sel
*****************************************************/

static __inline HI_VOID hi_isp_demosaic_hv_sel_write(HI_U8 data) {
    IOWR_8DIRECT(0x3030, (data & 0x3));
}

static __inline HI_U8 hi_isp_demosaic_hv_sel_read(HI_VOID) {
    return (IORD_8DIRECT(0x3030) & 0x3);
}

/*****************************************************
ISP_DEMOSAIC_COEF7:         工作使能寄存器
0x205a3034:                     31:1      reserved
                                          0           tmplate
*****************************************************/
static __inline HI_VOID hi_isp_demosaic_tmplate_write(HI_U8 data) {
    IOWR_8DIRECT(0x3034, (data & 0x1));
}

static __inline HI_U8 hi_isp_demosaic_tmplate_read(HI_VOID) {
    return (IORD_8DIRECT(0x3034) & 0x1);
}

/*****************************************************
ISP_DEMOSAIC_CTRL_F:         DEMOSAIC普通控制寄存器
0x205a30e0:                     31:2      reserved
                                          1  :0      rggb_cfg   RGGBstart；0：R Gr Gb B；1：Gr R B Gb；2：Gb B R Gr；3：B Gb Gr R；
*****************************************************/
static __inline HI_VOID hi_isp_demosaic_rggb_cfg_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x30e0);
    IOWR_8DIRECT(0x30e0, (data & 0x3) | (u8current & 0xfc));
}

static __inline HI_U8 hi_isp_demosaic_rggb_cfg_read(HI_VOID) {
    return (IORD_8DIRECT(0x30e0) & 0x3);
}

/*****************************************************
ISP_DEMOSAIC_CTRL_I:   DEMOSAIC普通控制寄存器
0x205a30E4:                     31:1      reserved
                                          0  :0      isp更新模式寄存器：0：update更新；1：帧更新。
*****************************************************/
static __inline HI_VOID hi_isp_demosaic_update_mode_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x30e4);
    IOWR_8DIRECT(0x30e4, (data & 0x1) | (u8current & 0xfe));
}

static __inline HI_U8 hi_isp_demosaic_update_mode_read(HI_VOID) {
    return (IORD_8DIRECT(0x30e4) & 0x1);
}

/*****************************************************
ISP_DEMOSAIC_TIMING:   DEMOSAIC普通控制寄存器
0x205a30E8:                     31:14      reserved
                                          13  :1      cpi_fix_timing_stat
                                                 0      cpi_auto_timing_en
*****************************************************/
static __inline HI_VOID hi_isp_demosaic_cpi_fix_timing_stat_write(HI_U16 data) {
	HI_U16 u16current = IORD_16DIRECT(0x30e8);
    IOWR_16DIRECT(0x30e8, ((data & 0x1fff) << 1) | (u16current & 0xc001));
}

static __inline HI_U16 hi_isp_demosaic_cpi_fix_timing_stat_read(HI_VOID) {
    return ((IORD_16DIRECT(0x30e8) & 0x1fff) >> 1);
}

static __inline HI_VOID hi_isp_demosaic_cpi_auto_timing_en_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x30e8);
    IOWR_8DIRECT(0x30e8, (data & 0x1) | (u8current & 0xfe));
}

static __inline HI_U8 hi_isp_demosaic_cpi_auto_timing_en_read(HI_VOID) {
    return (IORD_8DIRECT(0x30e8) & 0x1);
}

/*****************************************************
ISP_DEMOSAIC_UPDATE:   DEMOSAIC普通控制寄存器
0x205a30Ec:                     31:1      reserved
                                          0  :0      isp更新寄存器，每帧自动清0
*****************************************************/
static __inline HI_VOID hi_isp_demosaic_update_write(HI_U8 data) {
	HI_U8 u8current = IORD_8DIRECT(0x30ec);
    IOWR_8DIRECT(0x30ec, (data & 0x1) | (u8current & 0xfe));
}

static __inline HI_U8 hi_isp_demosaic_update_read(HI_VOID) {
    return (IORD_8DIRECT(0x30ec) & 0x1);
}

/*****************************************************
ISP_DEMOSAIC_SIZE:   DEMOSAIC普通控制寄存器
0x205a30f0:                     31:29      reserved
                                         28:16      height
                                         15:13      reserved
                                         12:00      width
*****************************************************/
static __inline HI_VOID hi_isp_demosaic_height_write(HI_U16 data) {
	HI_U16 u16current = IORD_16DIRECT(0x30f2);
    IOWR_16DIRECT(0x30f2, (data & 0x1fff) | (u16current & 0xe000));
}

static __inline HI_U16 hi_isp_demosaic_height_read(HI_VOID) {
    return (IORD_16DIRECT(0x30f2) & 0x1fff);
}

static __inline HI_VOID hi_isp_demosaic_width_write(HI_U16 data) {
	HI_U16 u16current = IORD_16DIRECT(0x30f0);
    IOWR_16DIRECT(0x30f0, (data & 0x1fff) | (u16current & 0xe000));
}

static __inline HI_U16 hi_isp_demosaic_width_read(HI_VOID) {
    return (IORD_16DIRECT(0x30f0) & 0x1fff);
}

/*****************************************************
MODULE        -----DG-----
MODULE        -----DG-----
MODULE        -----DG-----
*****************************************************/
/*****************************************************
ISP_DG_CFG:         DG控制寄存器
0x205a1400:         31:1      reserved
                                   0      en
*****************************************************/
static __inline HI_VOID hi_isp_dg_enable_write(HI_U8 data) {
    IOWR_8DIRECT(0x1400, data);
}

static __inline HI_U8 hi_isp_dg_enable_read(HI_VOID) {
    return (IORD_8DIRECT(0x1400));
}

/*****************************************************
ISP_DG_VERSION:         工作使能寄存器
0x205a140c:                     31:20      ISP版本,1:V100；2：V200；3：V300依此类推
                                          19:16      保留
                                          15:  0      发布日期。
*****************************************************/

static __inline HI_VOID hi_isp_dg_version_write(HI_U16 data) {
	HI_U16 u16current = IORD_16DIRECT(0x140e);
    IOWR_16DIRECT(0x140e, (data & 0xfff) << 4 | (u16current & 0xf));
}

static __inline HI_U16 hi_isp_dg_version_read(HI_VOID) {
    return ((IORD_16DIRECT(0x140e) & 0xfff) >> 4);
}

static __inline HI_VOID hi_isp_dg_date_write(HI_U16 data) {
    IOWR_16DIRECT(0x140c, data);
}

static __inline HI_U16 hi_isp_dg_date_read(HI_VOID) {
    return (IORD_16DIRECT(0x140c));
}

/*****************************************************
ISP_DG_GAIN1:         工作使能寄存器
0x205a1410:                    
                                          31:16      rgain
                                          15:00      grgain
*****************************************************/

static __inline HI_VOID hi_isp_dg_rgain_write(HI_U16 data) {
    IOWR_16DIRECT(0x1412, (data));
}

static __inline HI_U16 hi_isp_dg_rgain_read(HI_VOID) {
    return ((IORD_16DIRECT(0x1412)));
}

static __inline HI_VOID hi_isp_dg_grgain_write(HI_U16 data) {
    IOWR_16DIRECT(0x1410, (data));
}

static __inline HI_U16 hi_isp_dg_grgain_read(HI_VOID) {
    return ((IORD_16DIRECT(0x1410)));
}

/*****************************************************
ISP_DG_GAIN2:         工作使能寄存器
0x205a1414:                     31:28      保留
                                          31:16      bgain
                                          15:00      gbgain
*****************************************************/

static __inline HI_VOID hi_isp_dg_bgain_write(HI_U16 data) {
    IOWR_16DIRECT(0x1416, (data));
}

static __inline HI_U16 hi_isp_dg_bgain_read(HI_VOID) {
    return ((IORD_16DIRECT(0x1416)));
}

static __inline HI_VOID hi_isp_dg_gbgain_write(HI_U16 data) {
    IOWR_16DIRECT(0x1414, (data));
}

static __inline HI_U16 hi_isp_dg_gbgain_read(HI_VOID) {
    return ((IORD_16DIRECT(0x1414)));
}

/*****************************************************
MODULE        -----DPC-----
MODULE        -----DPC-----
MODULE        -----DPC-----
*****************************************************/
/*****************************************************
ISP_DPC_CFG:         DPC控制寄存器
0x205a5000:           31:1    reserved
                                0         DPC使能；0：禁止；1：使能；
*****************************************************/
static __inline HI_VOID hi_isp_dpc_enable_write(HI_U8 data) {
    IOWR_8DIRECT(0x5000, (data & 0x01));
}

static __inline HI_U8 hi_isp_dpc_enable_read(HI_VOID) {
    return (IORD_8DIRECT(0x5000) & 0x1);
}

/*****************************************************
ISP_DPC_MEM_CFG:    DPC MEM复用方式寄存器。
0x205a5004:               31:1    reserved
                                    0         mem_sel  memory复用；0：复用外部memory；1：复用CAC/GEmemory。
*****************************************************/
static __inline HI_VOID hi_isp_dpc_mem_sel_write(HI_U8 data) {
    IOWR_8DIRECT(0x5004, (data & 0x01));
}

static __inline HI_U8 hi_isp_dpc_mem_sel_read(HI_VOID) {
    return (IORD_8DIRECT(0x5004) & 0x1);
}

/*****************************************************
ISP_DPC_BLEND:    DPC融合比率寄存器
0x205a5008:               31:0    reserved
*****************************************************/
static __inline HI_VOID hi_isp_dpc_blend_write(HI_U32 data) {
    IOWR_32DIRECT(0x5008, data);
}

static __inline HI_U32 hi_isp_dpc_blend_read(HI_VOID) {
    return (IORD_32DIRECT(0x5008));
}


/*****************************************************
ISP_DPC_VERSION:    DPC版本号寄存器
0x205a500c:               31:20    version
					    19:16    reserved
                                   15:  0     data       发布日期。
*****************************************************/
static __inline HI_VOID hi_isp_dpc_version_write(HI_U16 data) {
	HI_U16 u16Current = IORD_16DIRECT(0x500e);
    IOWR_16DIRECT(0x500e, ((data & 0xfff) << 4) | (u16Current & 0xf));
}

static __inline HI_U16 hi_isp_dpc_version_read(HI_VOID) {
    return ((IORD_16DIRECT(0x500e) & 0xfff) >> 4);
}

static __inline HI_VOID hi_isp_dpc_data_write(HI_U16 data) {
    IOWR_16DIRECT(0x500c, data);
}

static __inline HI_U8 hi_isp_dpc_data_read(HI_VOID) {
    return (IORD_16DIRECT(0x500c));
}

/*****************************************************
ISP_DPC_MODE:          DPC控制寄存器
0x205a5010:               31:10      reserved
					9      IR_channel            	是否是RGBIR sensor，1：是，0：否
					8      IR_position		IR在sensor上是在Gr的位置还是在Gb的位置，1：Gr；0：Gb	
					7      dp_hightlight_en     	坏点高亮显示使能，1：有效，1：无效。
                                        6      dpt_det_sel              坏点表检测选择，1：检测暗点，0：检测亮点。
                                        5      bpt_cor_en               使用坏点表校正使能，1：有效，0：无效。
                                        4      grayscale_mode        灰度图模式，1：灰度图，0：RAW数据。
                                        3      bpt_update               坏点表标定模式，1：有效，0：无效。
                                        2      six_det_en                最大最小值检测，只在坏点表标定使用，1：有效，0：无效。
                                        1      cor_en                       动态检测校正使能，1：有效，0：无效。
                                        0      det_en                       动态检测使能，1：有效，0：无效。                 
*****************************************************/
static __inline HI_VOID hi_isp_dp_mode_write(HI_U16 data) {
    IOWR_16DIRECT(0x5010, data);
}

static __inline HI_U16 hi_isp_dp_mode_read(HI_VOID) {
    return (IORD_16DIRECT(0x5010));
}

/*****************************************************
ISP_DPC_MODE:          DPC控制寄存器
0x205a5010:               31:8      reserved
					9      IR_channel            	是否是RGBIR sensor，1：是，0：否
					8      IR_position		IR在sensor上是在Gr的位置还是在Gb的位置，1：Gr；0：Gb	
					7      dp_hightlight_en     坏点高亮显示使能，1：有效，1：无效。
                                        6      dpt_det_sel              坏点表检测选择，1：检测暗点，0：检测亮点。
                                        5      bpt_cor_en               使用坏点表校正使能，1：有效，0：无效。
                                        4      grayscale_mode        灰度图模式，1：灰度图，0：RAW数据。
                                        3      bpt_update               坏点表标定模式，1：有效，0：无效。
                                        2      six_det_en                最大最小值检测，只在坏点表标定使用，1：有效，0：无效。
                                        1      cor_en                       动态检测校正使能，1：有效，0：无效。
                                        0      det_en                       动态检测使能，1：有效，0：无效。                 
*****************************************************/
static __inline HI_VOID hi_isp_dp_IR_channel_write(HI_U8 data) {
	HI_U8 u8Current = IORD_8DIRECT(0x5010);
    IOWR_8DIRECT(0x5010, ((data & 0x1) << 9) | (u8Current & 0x1ff));
}

static __inline HI_U8 hi_isp_dp_IR_channel_read(HI_VOID) {
    return ((IORD_8DIRECT(0x5010) & 0x200) >> 9);
}

static __inline HI_VOID hi_isp_dp_IR_position_write(HI_U8 data) {
	HI_U8 u8Current = IORD_8DIRECT(0x5010);
    IOWR_8DIRECT(0x5010, ((data & 0x1) << 8) | (u8Current & 0x2ff));
}

static __inline HI_U8 hi_isp_dp_IR_position_read(HI_VOID) {
    return ((IORD_8DIRECT(0x5010) & 0x100) >>8);
}



static __inline HI_VOID hi_isp_dp_hightlight_en_write(HI_U8 data) {
	HI_U8 u8Current = IORD_8DIRECT(0x5010);
    IOWR_8DIRECT(0x5010, ((data & 0x1) << 7) | (u8Current & 0x7f));
}

static __inline HI_U8 hi_isp_dp_hightlight_en_read(HI_VOID) {
    return ((IORD_8DIRECT(0x5010) & 0x80) >> 7);
}

static __inline HI_VOID hi_isp_dp_dpt_det_sel_write(HI_U8 data) {
	HI_U8 u8Current = IORD_8DIRECT(0x5010);
    IOWR_8DIRECT(0x5010, ((data & 0x1) << 6) | (u8Current & 0xbf));
}

static __inline HI_U8 hi_isp_dp_dpt_det_sel_read(HI_VOID) {
    return ((IORD_8DIRECT(0x5010) & 0x40) >> 6);
}

static __inline HI_VOID hi_isp_dp_bpt_cor_en_write(HI_U8 data) {
	HI_U8 u8Current = IORD_8DIRECT(0x5010);
    IOWR_8DIRECT(0x5010, ((data & 0x1) << 5) | (u8Current & 0xdf));
}

static __inline HI_U8 hi_isp_dp_bpt_cor_en_read(HI_VOID) {
    return ((IORD_8DIRECT(0x5010) & 0x20) >> 5);
}

static __inline HI_VOID hi_isp_dp_grayscale_mode_write(HI_U8 data) {
	HI_U8 u8Current = IORD_8DIRECT(0x5010);
    IOWR_8DIRECT(0x5010, ((data & 0x1) << 4) | (u8Current & 0xef));
}

static __inline HI_U8 hi_isp_dp_grayscale_mode_read(HI_VOID) {
    return ((IORD_8DIRECT(0x5010) & 0x10) >> 4);
}

static __inline HI_VOID hi_isp_dp_bpt_update_write(HI_U8 data) {
	HI_U8 u8Current = IORD_8DIRECT(0x5010);
    IOWR_8DIRECT(0x5010, ((data & 0x1) << 3) | (u8Current & 0xf7));
}

static __inline HI_U8 hi_isp_dp_bpt_update_read(HI_VOID) {
    return ((IORD_8DIRECT(0x5010) & 0x08) >> 3);
}

static __inline HI_VOID hi_isp_dp_six_det_en_write(HI_U8 data) {
	HI_U8 u8Current = IORD_8DIRECT(0x5010);
    IOWR_8DIRECT(0x5010, ((data & 0x1) << 2) | (u8Current & 0xfb));
}

static __inline HI_U8 hi_isp_six_det_en_read(HI_VOID) {
    return ((IORD_8DIRECT(0x5010) & 0x04) >> 2);
}

static __inline HI_VOID hi_isp_dp_cor_en_write(HI_U8 data) {
	HI_U8 u8Current = IORD_8DIRECT(0x5010);
    IOWR_8DIRECT(0x5010, ((data & 0x1) << 1) | (u8Current & 0xfd));
}

static __inline HI_U8 hi_isp_cor_en_read(HI_VOID) {
    return ((IORD_8DIRECT(0x5010) & 0x02) >> 1);
}

static __inline HI_VOID hi_isp_dp_det_en_write(HI_U8 data) {
	HI_U8 u8Current = IORD_8DIRECT(0x5010);
    IOWR_8DIRECT(0x5010, ((data & 0x1)) | (u8Current & 0xfe));
}

static __inline HI_U8 hi_isp_det_en_read(HI_VOID) {
    return ((IORD_8DIRECT(0x5010) & 0x01));
}

/*****************************************************
ISP_DPC_MODE:          DPC控制寄存器
0x205a5014:               31:4      reserved
					      3:0      output_mode
*****************************************************/
static __inline HI_VOID hi_isp_dp_output_mode_write(HI_U8 data) {
    IOWR_8DIRECT(0x5014, data);
}

static __inline HI_U8 hi_isp_dp_output_mode_read(HI_VOID) {
    return ((IORD_8DIRECT(0x5014) & 0xf));
}

/*****************************************************
ISP_DPC_SET_USE:          DPC控制寄存器
0x205a5018:               31:4      reserved
					      3:0      set_use
*****************************************************/
static __inline HI_VOID hi_isp_dp_set_use_write(HI_U8 data) {
    IOWR_8DIRECT(0x5018, data);
}

static __inline HI_U8 hi_isp_dp_set_use_read(HI_VOID) {
    return ((IORD_8DIRECT(0x5018) & 0xf));
}

/*****************************************************
ISP_DPC_METHODS_SET_1:          DPC控制寄存器
0x205a501c:               31:13      reserved
					      12:0      methods_set_1
*****************************************************/
static __inline HI_VOID hi_isp_dp_methods_set_1_write(HI_U16 data) {
    IOWR_16DIRECT(0x501c, (data & 0x1fff));
}

static __inline HI_U16 hi_isp_dp_methods_set_1_read(HI_VOID) {
    return ((IORD_16DIRECT(0x501c) & 0x1fff));
}

/*****************************************************
ISP_DPC_METHODS_SET_2:          DPC控制寄存器
0x205a5020:               31:13      reserved
					      12:0      methods_set_2
*****************************************************/
static __inline HI_VOID hi_isp_dp_methods_set_2_write(HI_U16 data) {
    IOWR_16DIRECT(0x5020, (data & 0x1fff));
}

static __inline HI_U16 hi_isp_dp_methods_set_2_read(HI_VOID) {
    return ((IORD_16DIRECT(0x5020) & 0x1fff));
}

/*****************************************************
ISP_DPC_METHODS_SET_3:          DPC控制寄存器
0x205a5024:               31:13      reserved
					      12:0      methods_set_3
*****************************************************/
static __inline HI_VOID hi_isp_dp_methods_set_3_write(HI_U16 data) {
    IOWR_16DIRECT(0x5024, (data & 0x1fff));
}

static __inline HI_U16 hi_isp_dp_methods_set_3_read(HI_VOID) {
    return ((IORD_16DIRECT(0x5024) & 0x1fff));
}

/*****************************************************
ISP_DPC_LINE_THRESH_1:          DPC控制寄存器
0x205a5028:               31:16      reserved
					    15: 00      line_thresh_1
*****************************************************/
static __inline HI_VOID hi_isp_dp_line_thresh_1_write(HI_U16 data) {
    IOWR_16DIRECT(0x5028, data);
}

static __inline HI_U16 hi_isp_dp_line_thresh_1_read(HI_VOID) {
    return (IORD_16DIRECT(0x5028));
}

/*****************************************************
ISP_DPC_LINE_MAD_FAC_1:          DPC控制寄存器
0x205a502c:               31:16      reserved
					    15: 00      line_mad_fac_1
*****************************************************/
static __inline HI_VOID hi_isp_dp_line_mad_fac_1_write(HI_U16 data) {
    IOWR_16DIRECT(0x502c, data);
}

static __inline HI_U16 hi_isp_dp_line_mad_fac_1_read(HI_VOID) {
    return (IORD_16DIRECT(0x502c));
}

/*****************************************************
ISP_DPC_PG_FAC_1:          DPC控制寄存器
0x205a5030:               31:16      reserved
					    15: 00      pg_fac_1
*****************************************************/
static __inline HI_VOID hi_isp_dp_pg_fac_1_write(HI_U16 data) {
    IOWR_16DIRECT(0x5030, data);
}

static __inline HI_U16 hi_isp_dp_pg_fac_1_read(HI_VOID) {
    return (IORD_16DIRECT(0x5030));
}

/*****************************************************
ISP_DPC_RND_THRESH_1:          DPC控制寄存器
0x205a5034:               31:16      reserved
					    15: 00     rnd_thresh_1
*****************************************************/
static __inline HI_VOID hi_isp_dp_rnd_thresh_1_write(HI_U16 data) {
    IOWR_16DIRECT(0x5034, data);
}

static __inline HI_U16 hi_isp_dp_rnd_thresh_1_read(HI_VOID) {
    return (IORD_16DIRECT(0x5034));
}

/*****************************************************
ISP_DPC_RG_FAC_1:          DPC控制寄存器
0x205a5038:               31:16      reserved
					    15: 00     rg_fac_1
*****************************************************/
static __inline HI_VOID hi_isp_dp_rg_fac_1_write(HI_U16 data) {
    IOWR_16DIRECT(0x5038, data);
}

static __inline HI_U16 hi_isp_dp_rg_fac_1_read(HI_VOID) {
    return (IORD_16DIRECT(0x5038));
}

/*****************************************************
ISP_DPC_LINE_THRESH_2:          DPC控制寄存器
0x205a503c:               31:16      reserved
					    15: 00     line_thresh_2
*****************************************************/
static __inline HI_VOID hi_isp_dp_line_thresh_2_write(HI_U16 data) {
    IOWR_16DIRECT(0x503c, data);
}

static __inline HI_U16 hi_isp_dp_line_thresh_2_read(HI_VOID) {
    return (IORD_16DIRECT(0x503c));
}


/*****************************************************
ISP_DPC_LINE_MAD_FAC_2:          DPC控制寄存器
0x205a5040:               31:16      reserved
					    15: 00     line_mad_fac_2
*****************************************************/
static __inline HI_VOID hi_isp_dp_line_mad_fac_2_write(HI_U16 data) {
    IOWR_16DIRECT(0x5040, data);
}

static __inline HI_U16 hi_isp_dp_line_mad_fac_2_read(HI_VOID) {
    return (IORD_16DIRECT(0x5040));
}

/*****************************************************
ISP_DPC_PG_FAC_2:          DPC控制寄存器
0x205a5044:               31:16      reserved
					    15: 00     pg_fac_2
*****************************************************/
static __inline HI_VOID hi_isp_dp_pg_fac_2_write(HI_U16 data) {
    IOWR_16DIRECT(0x5044, data);
}

static __inline HI_U16 hi_isp_dp_pg_fac_2_read(HI_VOID) {
    return (IORD_16DIRECT(0x5044));
}

/*****************************************************
ISP_DPC_RND_THRESH_2:          DPC控制寄存器
0x205a5048:               31:16      reserved
					    15: 00     rnd_thresh_2
*****************************************************/
static __inline HI_VOID hi_isp_dp_rnd_thresh_2_write(HI_U16 data) {
    IOWR_16DIRECT(0x5048, data);
}

static __inline HI_U16 hi_isp_dp_rnd_thresh_2_read(HI_VOID) {
    return (IORD_16DIRECT(0x5048));
}

/*****************************************************
ISP_DPC_RG_FAC_2:          DPC控制寄存器
0x205a504c:               31:16      reserved
					    15: 00     rg_fac_2
*****************************************************/
static __inline HI_VOID hi_isp_dp_rg_fac_2_write(HI_U16 data) {
    IOWR_16DIRECT(0x504c, data);
}

static __inline HI_U16 hi_isp_dp_rg_fac_2_read(HI_VOID) {
    return (IORD_16DIRECT(0x504c));
}

/*****************************************************
ISP_DPC_LINE_THRESH_3:          DPC控制寄存器
0x205a5050:               31:16      reserved
					    15: 00     line_thresh_3
*****************************************************/
static __inline HI_VOID hi_isp_dp_line_thresh_3_write(HI_U16 data) {
    IOWR_16DIRECT(0x5050, data);
}

static __inline HI_U16 hi_isp_dp_line_thresh_3_read(HI_VOID) {
    return (IORD_16DIRECT(0x5050));
}

/*****************************************************
ISP_DPC_LINE_MAD_FAC_3:          DPC控制寄存器
0x205a5054:               31:16      reserved
					    15: 00     line_mad_fac_3
*****************************************************/
static __inline HI_VOID hi_isp_dp_line_mad_fac_3_write(HI_U16 data) {
    IOWR_16DIRECT(0x5054, data);
}

static __inline HI_U16 hi_isp_dp_line_mad_fac_3_read(HI_VOID) {
    return (IORD_16DIRECT(0x5054));
}

/*****************************************************
ISP_DPC_PG_FAC_3:          DPC控制寄存器
0x205a5058:               31:16      reserved
					    15: 00     pg_fac_3
*****************************************************/
static __inline HI_VOID hi_isp_dp_pg_fac_3_write(HI_U16 data) {
    IOWR_16DIRECT(0x5058, data);
}

static __inline HI_U16 hi_isp_dp_pg_fac_3_read(HI_VOID) {
    return (IORD_16DIRECT(0x5058));
}

/*****************************************************
ISP_DPC_RND_THRESH_3:          DPC控制寄存器
0x205a505c:               31:16      reserved
					    15: 00     rnd_thresh_3
*****************************************************/
static __inline HI_VOID hi_isp_dp_rnd_thresh_3_write(HI_U16 data) {
    IOWR_16DIRECT(0x505c, data);
}

static __inline HI_U16 hi_isp_dp_rnd_thresh_3_read(HI_VOID) {
    return (IORD_16DIRECT(0x505c));
}


/*****************************************************
ISP_DPC_RG_FAC_3:          DPC控制寄存器
0x205a5060:               31:16      reserved
					    15: 00     rg_fac_3
*****************************************************/
static __inline HI_VOID hi_isp_dp_rg_fac_3_write(HI_U16 data) {
    IOWR_16DIRECT(0x5060, data);
}

static __inline HI_U16 hi_isp_dp_rg_fac_3_read(HI_VOID) {
    return (IORD_16DIRECT(0x5060));
}

/*****************************************************
ISP_DPC_RO_LIMITS:          DPC控制寄存器
0x205a5064:               31:16      reserved
					    15: 00     ro_limits
*****************************************************/
static __inline HI_VOID hi_isp_dp_ro_limits_write(HI_U16 data) {
    IOWR_16DIRECT(0x5064, data);
}

static __inline HI_U16 hi_isp_dp_ro_limits_read(HI_VOID) {
    return (IORD_16DIRECT(0x5064));
}

/*****************************************************
ISP_DPC_RND_OFFS:          DPC控制寄存器
0x205a5068:               31:12      reserved
					    11: 00     rnd_offs
*****************************************************/
static __inline HI_VOID hi_isp_dp_rnd_offs_write(HI_U16 data) {
    IOWR_16DIRECT(0x5068, (data & 0xfff));
}

static __inline HI_U16 hi_isp_dp_rnd_offs_read(HI_VOID) {
    return (IORD_16DIRECT(0x5068) & 0xfff);
}

/*****************************************************
ISP_DPC_BPT_CTRL:   DPC控制寄存器
0x205a506c:               31:16      bp_data    坏点高亮显示值。
					    15:12      reserved
					    11: 00     bpt_ctrl
*****************************************************/
static __inline HI_VOID hi_isp_dp_bpt_ctrl_write(HI_U32 data) {
    IOWR_32DIRECT(0x506c, data );
}




static __inline HI_U32 hi_isp_dp_bpt_ctrl_read(HI_VOID) {
    return (IORD_32DIRECT(0x506c) );
}

/*****************************************************
ISP_DPC_BPT_NUMBER:   DPC控制寄存器
0x205a5070:               31:12      reserved
					    11: 00     bpt_number
*****************************************************/
static __inline HI_VOID hi_isp_dp_bpt_number_write(HI_U16 data) {
    IOWR_16DIRECT(0x5070, (data & 0xfff));
}

static __inline HI_U16 hi_isp_dp_bpt_number_read(HI_VOID) {
    return (IORD_16DIRECT(0x5070) & 0xfff);
}

/*****************************************************
ISP_DPC_BPT_CALIB_NUMBER:   DPC控制寄存器
0x205a5074:               31:12      reserved
					    11: 00     bpt_calib_number
*****************************************************/
static __inline HI_VOID hi_isp_dp_bpt_calib_number_write(HI_U16 data) {
    IOWR_16DIRECT(0x5074, (data & 0xfff));
}

static __inline HI_U16 hi_isp_dp_bpt_calib_number_read(HI_VOID) {
    return (IORD_16DIRECT(0x5074) & 0xfff);
}

/*****************************************************
ISP_DPC_BPT_CALIB_NUMBER:   DPC控制寄存器
0x205a5078:               31: 01      reserved
					            0      in_soft_rst
*****************************************************/
static __inline HI_VOID hi_isp_dp_in_soft_rst_write(HI_U8 data) {
    IOWR_8DIRECT(0x5078, data);
}

static __inline HI_U8 hi_isp_dp_in_soft_rst_read(HI_VOID) {
    return (IORD_8DIRECT(0x5078) & 0x1);
}

/*****************************************************
ISP_DPC_BPT_THRD:   DPC控制寄存器
0x205a507c:               31: 24      abs_hot_thresh
					    23: 16      dev_hot_thresh
					    15: 08      abs_dead_thresh
					    07: 00      dev_dead_thresh
*****************************************************/
static __inline HI_VOID hi_isp_dp_bpt_thresh_write(HI_U32 data) {
	IOWR_32DIRECT(0x507c, data);
}

static __inline HI_U32 hi_isp_dp_bpt_thresh_read(HI_VOID) {

	return (IORD_32DIRECT(0x507c));
}

/*****************************************************
ISP_DPC_BPT_WADDR:   DPC控制寄存器
0x205a5080:               31: 00      bpt_waddr
*****************************************************/
static __inline HI_VOID hi_isp_dp_bpt_waddr_write(HI_U32 data) {
	IOWR_32DIRECT(0x5080, data);
}

static __inline HI_U32 hi_isp_dp_bpt_waddr_read(HI_VOID) {
	return (IORD_32DIRECT(0x5080));
}

/*****************************************************
ISP_DPC_BPT_WDATA:   DPC控制寄存器
0x205a5084:               31: 00      bpt_wdata
*****************************************************/
static __inline HI_VOID hi_isp_dp_bpt_wdata_write(HI_U32 data) {
	IOWR_32DIRECT(0x5084, data);
}

static __inline HI_U32 hi_isp_dp_bpt_wdata_read(HI_VOID) {
	return (IORD_32DIRECT(0x5084));
}

/*****************************************************
ISP_DPC_BPT_RADDR:   DPC控制寄存器
0x205a5088:                  31: 00      bpt_raddr
*****************************************************/
static __inline HI_VOID hi_isp_dp_bpt_raddr_write(HI_U32 data) {
	IOWR_32DIRECT(0x5088, data);
}

static __inline HI_U32 hi_isp_dp_bpt_raddr_read(HI_VOID) {
	return (IORD_32DIRECT(0x5088));
}

/*****************************************************
ISP_DPC_BPT_RDATA:   DPC控制寄存器
0x205a508c:                  31: 00      bpt_rdata
*****************************************************/
static __inline HI_VOID hi_isp_dp_bpt_rdata_write(HI_U32 data) {
	IOWR_32DIRECT(0x508c, data);
}

static __inline HI_U32 hi_isp_dp_bpt_rdata_read(HI_VOID) {
	return (IORD_32DIRECT(0x508c));
}

/*****************************************************
MODULE        -----FPN-----
MODULE        -----FPN-----
MODULE        -----FPN-----
*****************************************************/
/*****************************************************
ISP_FPN_CFG:         			FPN配置寄存器
0x205a5200:                    	31		gt_en
								30:10	reserved
								9		line_frame
								8		calib_corr
								7:1		reserved
                      			0		enable
*****************************************************/
static __inline HI_VOID hi_isp_fpn_clk_gating_write(HI_U8 data){
	HI_U32 u32current = IORD_32DIRECT(0x5200);
    IOWR_32DIRECT(0x5200, (u32current & (~(1 << 31))) | ((data&0x1) << 31));
}

static __inline HI_U8 hi_isp_fpn_clk_gating_read(HI_VOID) {
    return ((IORD_32DIRECT(0x5200) >> 31) & 0x1);
}

static __inline HI_VOID hi_isp_fpn_mode_write(HI_U8 data){
	HI_U32 u32current = IORD_32DIRECT(0x5200);
    IOWR_32DIRECT(0x5200, (u32current & (~(1 << 9))) | ((data&0x1) << 9));
}

static __inline HI_U8 hi_isp_fpn_mode_read(HI_VOID) {
    return ((IORD_32DIRECT(0x5200) >> 9) & 0x1);
}

static __inline HI_VOID hi_isp_fpn_work_mode_write(HI_U8 data){
	HI_U32 u32current = IORD_32DIRECT(0x5200);
    IOWR_32DIRECT(0x5200, (u32current & (~(1 << 8))) | ((data&0x1) << 8));
}

static __inline HI_U8 hi_isp_fpn_work_mode_read(HI_VOID) {
    return ((IORD_32DIRECT(0x5200) >> 8) & 0x1);
}

static __inline HI_VOID hi_isp_fpn_enable_write(HI_U8 data){
	HI_U32 u32current = IORD_32DIRECT(0x5200);
    IOWR_32DIRECT(0x5200, (u32current & (~(1 << 0))) | (data&0x1));
}

static __inline HI_U8 hi_isp_fpn_enable_read(HI_VOID) {
    return (IORD_32DIRECT(0x5200) & 0x1);
}

/*****************************************************
ISP_FPN_CORR:         			FPN校正参数配置(0~3)
0x205a5230 ~ 0x205a523C:                    	
								31:16		strength
								15:12		reserved
								11:0		offset
*****************************************************/
static __inline HI_VOID hi_isp_fpn_corr_gainoffset_write(HI_U8 index, HI_U32 data) {
	HI_U32 u32Current = IORD_32DIRECT(0x5230 + (index<<2));
	IOWR_32DIRECT((0x5230 + (index<<2)), ((u32Current & 0x0000f000) | data));
}

static __inline HI_U32 hi_isp_fpn_corr_gainoffset_read(HI_U8 index){
	return (IORD_32DIRECT(0x5230 + (index<<2)));
}


/*****************************************************
ISP_FPN_OVERFLOWTHR:         			FPN校正阈值
0x205a5254:                    	
								31:14		reserved
								13:0		overflowthr
*****************************************************/
static __inline HI_VOID hi_isp_fpn_coverflowthr_write(HI_U16 data) {
	HI_U16 u16Current = IORD_16DIRECT(0x5254);
	IOWR_16DIRECT(0x5254, (u16Current & 0xc000) | (data & 0x3fff));
}

static __inline HI_U16 hi_isp_fpn_overflowthr_read(HI_VOID){
	return (IORD_16DIRECT(0x5254) & 0x3fff);
}

/*****************************************************
MODULE        -----DRC-----
MODULE        -----DRC-----
MODULE        -----DRC-----
*****************************************************/
/*****************************************************
ISP_DRC_CFG:         DRC控制寄存器
0x205a6200:           31:1    reserved
                                0         DRC使能；0：禁止；1：使能；
*****************************************************/
static __inline HI_VOID hi_isp_drc_enable_write(HI_U8 data) {
	HI_U8 u8Current = IORD_8DIRECT(0x6200);
    IOWR_8DIRECT(0x6200, (data & 0x01) | (u8Current & 0xfe));
}

static __inline HI_U8 hi_isp_drc_enable_read(HI_VOID) {
    return (IORD_8DIRECT(0x6200) & 0x1);
}

/*****************************************************
ISP_DRC_ZONE:       DRC分区配置寄存器。
0x205a6210:             31:30  reserved
                               29:24  chk_y
					23:16  vnum
					15:14  reserved
                                13:8   chk_x
                                7:0     hnum
*****************************************************/
static __inline HI_VOID hi_isp_drc_hnum_write(HI_U8 data) {
    IOWR_8DIRECT(0x6210, (data & 0x7f));
}

static __inline HI_U8 hi_isp_drc_hnum_read(HI_VOID) {
    return ((IORD_8DIRECT(0x6210) & 0x7f));
}

static __inline HI_VOID hi_isp_drc_vnum_write(HI_U8 data) {
    IOWR_8DIRECT(0x6211, (data & 0x7f));
}

static __inline HI_U8 hi_isp_drc_vnum_read(HI_VOID) {
    return ((IORD_8DIRECT(0x6211) & 0x7f));
}

/*****************************************************
ISP_DRC_ZONE_SIZE:       DRC区块大小寄存器。
0x205a6214:             31:30  reserved
                               29:24  chk_y
					23:16  vnum
					15:14  reserved
                                13:8   chk_x
                                7:0     hnum
*****************************************************/
static __inline HI_VOID hi_isp_drc_hsize_write(HI_U8 data) {
    IOWR_8DIRECT(0x6214, (data));
}

static __inline HI_U8 hi_isp_drc_hsize_read(HI_VOID) {
	return ((IORD_8DIRECT(0x6214)));
}

static __inline HI_VOID hi_isp_drc_chk_x_write(HI_U8 data) {
    IOWR_8DIRECT(0x6215, (data)&0x3f);
}

static __inline HI_U8 hi_isp_drc_chk_x_read(HI_VOID) {
	return ((IORD_8DIRECT(0x6215))&0x3f);
}

static __inline HI_VOID hi_isp_drc_vsize_write(HI_U8 data) {
    IOWR_8DIRECT(0x6216, (data));
}

static __inline HI_U8 hi_isp_drc_vsize_read(HI_VOID) {
	return ((IORD_8DIRECT(0x6216)));
}

static __inline HI_VOID hi_isp_drc_chk_y_write(HI_U8 data) {
    IOWR_8DIRECT(0x6217, (data)&0x3f);
}

static __inline HI_U8 hi_isp_drc_chk_y_read(HI_VOID) {
	return ((IORD_8DIRECT(0x6217))&0x3f);
}


/*****************************************************
ISP_DRC_ZONE_DIV0:       DRC区块大小寄存器。
0x205a6218:              31       reserved
					30:16   div_y0
                                15       reserved
                                14:0    div_x0
*****************************************************/
static __inline HI_VOID hi_isp_drc_div_x0_write(HI_U16 data) {
    IOWR_16DIRECT(0x6218, (data & 0x7fff));
}

static __inline HI_U16 hi_isp_drc_div_x0_read(HI_VOID) {
	return ((IORD_16DIRECT(0x6218) & 0x7fff));
}

static __inline HI_VOID hi_isp_drc_div_y0_write(HI_U16 data) {
    IOWR_16DIRECT(0x621a, (data & 0x7fff));
}

static __inline HI_U16 hi_isp_drc_div_y0_read(HI_VOID) {
	return ((IORD_16DIRECT(0x621a) & 0x7fff));
}

/*****************************************************
ISP_DRC_ZONE_DIV1:       DRC区块大小寄存器。
0x205a621C:             31       reserved
					30:16   div_y1
                                15       reserved
                                14:0    div_x1
*****************************************************/
static __inline HI_VOID hi_isp_drc_div_x1_write(HI_U16 data) {
    IOWR_16DIRECT(0x621c, (data & 0x7fff));
}

static __inline HI_U16 hi_isp_drc_div_x1_read(HI_VOID) {
	return ((IORD_16DIRECT(0x621c) & 0x7fff));
}

static __inline HI_VOID hi_isp_drc_div_y1_write(HI_U16 data) {
    IOWR_16DIRECT(0x621e, (data & 0x7fff));
}

static __inline HI_U16 hi_isp_drc_div_y1_read(HI_VOID) {
	return ((IORD_16DIRECT(0x621e) & 0x7fff));
}



/*****************************************************
ISP_DRC_BIN:         DRC区块直方图段数寄存器。
0x205a6220:           31:05    reserved
					04:00    bin
*****************************************************/
static __inline HI_VOID hi_isp_drc_bin_write(HI_U8 data) {
    IOWR_8DIRECT(0x6220, (data & 0x1f));
}

static __inline HI_U8 hi_isp_drc_bin_read(HI_VOID) {
	return ((IORD_8DIRECT(0x6220) & 0x1f));
}

/*****************************************************
ISP_DRC_BIN_DIV:         DRC区块直方图段数寄存器。
0x205a6224:           31:15    reserved
					14:00    div_z DRC每个区块的宽度的倒数。
*****************************************************/
static __inline HI_VOID hi_isp_drc_div_z_write(HI_U16 data) {
    IOWR_16DIRECT(0x6224, (data & 0x7fff));
}

static __inline HI_U16 hi_isp_drc_div_z_read(HI_VOID) {
	return ((IORD_16DIRECT(0x6224) & 0x7fff));
}

/*****************************************************
ISP_DRC_BIN_SCALE:         DRC直方图缩放比例寄存器。
0x205a6228:           31:03    reserved
					02:00    bin_scale DRC每个区块的宽度的倒数。
*****************************************************/
static __inline HI_VOID hi_isp_drc_bin_scale_write(HI_U8 data) {
    IOWR_8DIRECT(0x6228, (data & 0x7));
}

static __inline HI_U8 hi_isp_drc_bin_scale_read(HI_VOID) {
	return ((IORD_8DIRECT(0x6228) & 0x7));
}

/*****************************************************
ISP_DRC_RANGE_FLT_COEF:         DRC值域滤波系数
0x205a622c:             31:03    reserved
					03:00   range flt coef
*****************************************************/
static __inline HI_VOID hi_isp_drc_range_flt_coef_write(HI_U8 data) {
    IOWR_8DIRECT(0x622c, (data & 0xf));
}

static __inline HI_U8 hi_isp_drc_range_flt_coef_read(HI_VOID) {
	return ((IORD_8DIRECT(0x622c) & 0xf));
}

/*****************************************************
ISP_DRC_EXPOSURE:         DRC当前帧曝光值寄存器。
0x205a6230:           31:16    reserved
					15:00    exposure DRC当前帧曝光值配置，用于对亮暗切换补偿。
*****************************************************/
static __inline HI_VOID hi_isp_drc_exposure_write(HI_U16 data) {
    IOWR_16DIRECT(0x6230, (data));
}

static __inline HI_U16 hi_isp_drc_exposure_read(HI_VOID) {
	return ((IORD_16DIRECT(0x6230)));
}

/*****************************************************
ISP_DRC_LOCAL_EDGE_LMT:         DRC局部限制寄存器。
0x205a6234:           31:08    reserved
					07:00    local_edge_lmt DRC当前帧曝光值配置，用于对亮暗切换补偿。
*****************************************************/
static __inline HI_VOID hi_isp_drc_local_edge_lmt_write(HI_U8 data) {
    IOWR_8DIRECT(0x6234, (data));
}

static __inline HI_U8 hi_isp_drc_local_edge_lmt_read(HI_VOID) {
	return ((IORD_8DIRECT(0x6234)));
}

/*****************************************************
ISP_DRC_STRENGTH:         DRC强度调节寄存器。
0x205a6238:           31:08    reserved
					07:00    strength DRC当前帧曝光值配置，用于对亮暗切换补偿。
*****************************************************/
static __inline HI_VOID hi_isp_drc_strength_write(HI_U8 data) {
    IOWR_8DIRECT(0x6238, (data));
}

static __inline HI_U8 hi_isp_drc_strength_read(HI_VOID) {
	return ((IORD_8DIRECT(0x6238)));
}

/*****************************************************
ISP_DRC_DETAIL_GAIN: DRC细节增益调节寄存器
0x205a623C:             31:24    reserved
                               23:16    detail_gain_thres
                               15:8      detail_gain_dark
					07:00    detail_gain_bright
*****************************************************/
static __inline HI_VOID hi_isp_drc_detail_mixing_thres_write(HI_U8 data) {
    IOWR_8DIRECT(0x623e, data);
}

static __inline HI_U8 hi_isp_drc_detail_mixing_thres_read(HI_VOID) {
	return ((IORD_8DIRECT(0x623e)));
}

static __inline HI_VOID hi_isp_drc_detail_mixing_dark_write(HI_U8 data) {
    IOWR_8DIRECT(0x623d, (data));
}

static __inline HI_U8 hi_isp_drc_detail_mixing_dark_read(HI_VOID) {
	return ((IORD_8DIRECT(0x623d)));
}

static __inline HI_VOID hi_isp_drc_detail_mixing_bright_write(HI_U8 data) {
    IOWR_8DIRECT(0x623C, (data));
}

static __inline HI_U8 hi_isp_drc_detail_mixing_bright_read(HI_VOID) {
	return ((IORD_8DIRECT(0x623C)));
}





/*****************************************************
ISP_DRC_DARK_GAIN_LMT_Y:         DRC暗区亮度限制寄存器。
0x205a6240:           31:16    reserved
					15:14    SFT2  DRC每个区块的高度（配置为实际值减1）。
					13:10    VAL2 DRC每个区块的高度（配置为实际值减1）。
					09:07    SFT1
					06:00    VAL1
*****************************************************/
static __inline HI_VOID hi_isp_drc_y_val1_write(HI_U16 data) {
	HI_U16 u16Current = IORD_16DIRECT(0x6240);
    IOWR_16DIRECT(0x6240, (data & 0x7f) | (u16Current & 0xff80));
}

static __inline HI_U16 hi_isp_drc_y_val1_read(HI_VOID) {
	return ((IORD_16DIRECT(0x6240) & 0x7f));
}

static __inline HI_VOID hi_isp_drc_y_sft1_write(HI_U16 data) {
	HI_U16 u16Current = IORD_16DIRECT(0x6240);
    IOWR_16DIRECT(0x6240, ((data & 0x7) << 7) | (u16Current & 0xfc7f));
}

static __inline HI_U16 hi_isp_drc_y_sft1_read(HI_VOID) {
	return ((IORD_16DIRECT(0x6240) & 0x380) >> 7);
}

static __inline HI_VOID hi_isp_drc_y_val2_write(HI_U16 data) {
	HI_U16 u16Current = IORD_16DIRECT(0x6240);
    IOWR_16DIRECT(0x6240, ((data & 0xf) << 10) | (u16Current & 0xc3ff));
}

static __inline HI_U16 hi_isp_drc_y_val2_read(HI_VOID) {
	return ((IORD_16DIRECT(0x6240) & 0x3c00) >> 10);
}

static __inline HI_VOID hi_isp_drc_y_sft2_write(HI_U16 data) {
	HI_U16 u16Current = IORD_16DIRECT(0x6240);
    IOWR_16DIRECT(0x6240, ((data & 0x3) << 14) | (u16Current & 0x3fff));
}

static __inline HI_U16 hi_isp_drc_y_sft2_read(HI_VOID) {
	return ((IORD_16DIRECT(0x6240) & 0xc000) >> 14);
}

/*****************************************************
ISP_DRC_DARK_GAIN_LMT_C:         DRC暗区亮度限制寄存器。
0x205a6244:           31:16    reserved
					15:14    SFT2  DRC每个区块的高度（配置为实际值减1）。
					13:10    VAL2 DRC每个区块的高度（配置为实际值减1）。
					09:07    SFT1
					06:00    VAL1
*****************************************************/
static __inline HI_VOID hi_isp_drc_c_val1_write(HI_U16 data) {
	HI_U16 u16Current = IORD_16DIRECT(0x6244);
    IOWR_16DIRECT(0x6244, (data & 0x7f) | (u16Current & 0xff80));
}

static __inline HI_U16 hi_isp_drc_c_val1_read(HI_VOID) {
	return ((IORD_16DIRECT(0x6244) & 0x7f));
}

static __inline HI_VOID hi_isp_drc_c_sft1_write(HI_U16 data) {
	HI_U16 u16Current = IORD_16DIRECT(0x6244);
    IOWR_16DIRECT(0x6244, ((data & 0x7) << 7) | (u16Current & 0xfc7f));
}

static __inline HI_U16 hi_isp_drc_c_sft1_read(HI_VOID) {
	return ((IORD_16DIRECT(0x6244) & 0x380) >> 7);
}

static __inline HI_VOID hi_isp_drc_c_val2_write(HI_U16 data) {
	HI_U16 u16Current = IORD_16DIRECT(0x6244);
    IOWR_16DIRECT(0x6244, ((data & 0xf) << 10) | (u16Current & 0xc3ff));
}

static __inline HI_U16 hi_isp_drc_c_val2_read(HI_VOID) {
	return ((IORD_16DIRECT(0x6244) & 0x3c00) >> 10);
}

static __inline HI_VOID hi_isp_drc_c_sft2_write(HI_U16 data) {
	HI_U16 u16Current = IORD_16DIRECT(0x6244);
    IOWR_16DIRECT(0x6244, ((data & 0x3) << 14) | (u16Current & 0x3fff));
}

static __inline HI_U16 hi_isp_drc_c_sft2_read(HI_VOID) {
	return ((IORD_16DIRECT(0x6244) & 0xc000) >> 14);
}

/*****************************************************
ISP_DRC_BRIGHT_GAIN_LMT:         DRC暗区亮度限制寄存器。
0x205a6248:           31:10    reserved
					09:07    SFT
					06:00    VAL
*****************************************************/
static __inline HI_VOID hi_isp_drc_bright_gain_val_write(HI_U16 data) {
	HI_U16 u16Current = IORD_16DIRECT(0x6248);
    IOWR_16DIRECT(0x6248, (data & 0x7f) | (u16Current & 0xff80));
}

static __inline HI_U16 hi_isp_drc_bright_gain_val_read(HI_VOID) {
	return ((IORD_16DIRECT(0x6248) & 0x7f));
}

static __inline HI_VOID hi_isp_drc_bright_gain_sft_write(HI_U16 data) {
	HI_U16 u16Current = IORD_16DIRECT(0x6248);
    IOWR_16DIRECT(0x6248, ((data & 0x7) << 7) | (u16Current & 0xfc7f));
}

static __inline HI_U16 hi_isp_drc_bright_gain_sft_read(HI_VOID) {
	return ((IORD_16DIRECT(0x6248) & 0x380) >> 7);
}

/*****************************************************
ISP_DRC_VAR_SPATIAL:  DRC局部滤波混合寄存器。
0x205a6250:           07:04    var_range
				    03:00    var_spatial
*****************************************************/
static __inline HI_VOID hi_isp_drc_var_spatial_write(HI_U8 data) {
	HI_U8 u8Current = IORD_8DIRECT(0x6250);
	IOWR_8DIRECT(0x6250, (data & 0xf) | (u8Current & 0xf0));
}

static __inline HI_U8 hi_isp_drc_var_spatial_read(HI_VOID) {
	return ((IORD_8DIRECT(0x6250) & 0xf));
}

static __inline HI_VOID hi_isp_drc_var_range_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x6250);
    IOWR_8DIRECT(0x6250, ((data&0xf) << 4) | (u8Current & 0xf));
}

static __inline HI_U8 hi_isp_drc_var_range_read(HI_VOID) {
	return ((IORD_8DIRECT(0x6250) & 0xf0)>> 4);
}



/*****************************************************
ISP_DRC_LUT_MIX_CTRL:  DRC TM LUT 混合滤波器
0x205a6254:           31:08    reserved
					07:00    outbits
*****************************************************/
static __inline HI_VOID hi_isp_drc_lut_mix_ctrl_write(HI_U8 data) {
    IOWR_8DIRECT(0x6254, (data & 0xff));
}

static __inline HI_U8 hi_isp_drc_lut_mix_ctrl_read(HI_VOID) {
	return ((IORD_8DIRECT(0x6254) & 0xff));
}

/*****************************************************
ISP_DRC_GAIN_CLIP_KNEE:  DRC限制增益寄存器
0x205a6258:             31:08    reserved
					07:04    gain_clip_knee
					03:00    gain_clip_step
*****************************************************/
static __inline HI_VOID hi_isp_drc_gain_clip_step_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x6258);
	IOWR_8DIRECT(0x6258, (data & 0xf) | (u8Current & 0x70));  
}

static __inline HI_U8 hi_isp_drc_gain_clip_step_read(HI_VOID) {
	return ((IORD_8DIRECT(0x6258) & 0xf));
}

/*****************************************************
ISP_DRC_GAIN_CLIP_STEP:  DRC限制增益寄存器
0x205a6258:             31:08    reserved
					07:04    gain_clip_knee
					03:00    gain_clip_step
*****************************************************/
static __inline HI_VOID hi_isp_drc_gain_clip_knee_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x6258);
	IOWR_8DIRECT(0x6258, (((data & 0xf)<<4) | (u8Current & 0xf)));  
}

static __inline HI_U8 hi_isp_drc_gain_clip_knee_read(HI_VOID) {
	return IORD_8DIRECT(((0x6258& 0xf0)>> 4));
}


/*****************************************************
ISP_DRC_STAT_IND_WADDR   DRC ToneMapping表间接写地址寄存器。
0x205a6280:                 31:00     drc_stat_ind_waddr
*****************************************************/
static __inline HI_VOID hi_isp_drc_stat_ind_waddr0_write(HI_U32 data) { 
    IOWR_32DIRECT(0x6280, data);
}

static __inline HI_U32 hi_isp_drc_stat_ind_waddr0_read(HI_VOID) {
    return (IORD_32DIRECT(0x6280));
}

/*****************************************************
ISP_DRC_STAT_IND_WDATA   DRC ToneMapping表间接写数据寄存器。
0x205a6284:                 31:00     drc_stat_ind_wdata
*****************************************************/
static __inline HI_VOID hi_isp_drc_stat_ind_wdata0_write(HI_U32 data) { 
    IOWR_32DIRECT(0x6284, data);
}

static __inline HI_U32 hi_isp_drc_stat_ind_wdata0_read(HI_VOID) {
    return (IORD_32DIRECT(0x6284));
}

/*****************************************************
ISP_DRC_STAT_IND_RADDR  DRC ToneMapping表间接读地址寄存器。
0x205a6288:                 31:00     drc_stat_ind_raddr
*****************************************************/
static __inline HI_VOID hi_isp_drc_stat_ind_raddr0_write(HI_U32 data) { 
    IOWR_32DIRECT(0x6288, data);
}

static __inline HI_U32 hi_isp_drc_stat_ind_raddr0_read(HI_VOID) {
    return (IORD_32DIRECT(0x6288));
}

/*****************************************************
ISP_DRC_STAT_IND_RDATA  DRC ToneMapping表间接读数据寄存器。
0x205a628c:                 31:00     drc_stat_ind_rdata
*****************************************************/
static __inline HI_VOID hi_isp_drc_stat_ind_rdata0_write(HI_U32 data) { 
    IOWR_32DIRECT(0x628c, data);
}

static __inline HI_U32 hi_isp_drc_stat_ind_rdata0_read(HI_VOID) {
    return (IORD_32DIRECT(0x628c));
}

/*****************************************************
ISP_DRC_STAT_IND_WADDR   DRC ToneMapping表间接写地址寄存器。
0x205a6280:                 31:00     drc_stat_ind_waddr
*****************************************************/
static __inline HI_VOID hi_isp_drc_stat_ind_waddr1_write(HI_U32 data) { 
    IOWR_32DIRECT(0x6290, data);
}

static __inline HI_U32 hi_isp_drc_stat_ind_waddr1_read(HI_VOID) {
    return (IORD_32DIRECT(0x6290));
}

/*****************************************************
ISP_DRC_STAT_IND_WDATA   DRC ToneMapping表间接写数据寄存器。
0x205a6294:                 31:00     drc_stat_ind_wdata
*****************************************************/
static __inline HI_VOID hi_isp_drc_stat_ind_wdata1_write(HI_U32 data) { 
    IOWR_32DIRECT(0x6294, data);
}

static __inline HI_U32 hi_isp_drc_stat_ind_wdata1_read(HI_VOID) {
    return (IORD_32DIRECT(0x6294));
}

/*****************************************************
ISP_DRC_STAT_IND_RADDR  DRC ToneMapping表间接读地址寄存器。
0x205a6298:                 31:00     drc_stat_ind_raddr
*****************************************************/
static __inline HI_VOID hi_isp_drc_stat_ind_raddr1_write(HI_U32 data) { 
    IOWR_32DIRECT(0x6298, data);
}

static __inline HI_U32 hi_isp_drc_stat_ind_raddr1_read(HI_VOID) {
    return (IORD_32DIRECT(0x6298));
}

/*****************************************************
ISP_DRC_STAT_IND_RDATA  DRC ToneMapping表间接读数据寄存器。
0x205a629c:                 31:00     drc_stat_ind_rdata
*****************************************************/
static __inline HI_VOID hi_isp_drc_stat_ind_rdata1_write(HI_U32 data) { 
    IOWR_32DIRECT(0x629c, data);
}

static __inline HI_U32 hi_isp_drc_stat_ind_rdata1_read(HI_VOID) {
    return (IORD_32DIRECT(0x629c));
}

/*****************************************************
ISP_DRC_STAT_IND_RDATA  DRC ColorCorrectionLut表间接写地址寄存器
0x205a62a0:                 31:00     drc_stat_ind_rdata
*****************************************************/
static __inline HI_VOID hi_isp_drc_stat_ind_waddr2_write(HI_U32 data) { 
    IOWR_32DIRECT(0x62a0, data);
}

static __inline HI_U32 hi_isp_drc_stat_ind_waddr2_read(HI_VOID) {
    return (IORD_32DIRECT(0x62a0));
}

/*****************************************************
ISP_DRC_STAT_IND_WDATA   DRC ColorCorrectionLut表间接写数据寄存器。
0x205a62a4:                 31:00     drc_stat_ind_wdata
*****************************************************/
static __inline HI_VOID hi_isp_drc_stat_ind_wdata2_write(HI_U32 data) { 
    IOWR_32DIRECT(0x62a4, data);
}

static __inline HI_U32 hi_isp_drc_stat_ind_wdata2_read(HI_VOID) {
    return (IORD_32DIRECT(0x62a4));
}

/*****************************************************
ISP_DRC_STAT_IND_RADDR  DRC ColorCorrectionLut表间接读地址寄存器。
0x205a62a8:                 31:00     drc_stat_ind_raddr
*****************************************************/
static __inline HI_VOID hi_isp_drc_stat_ind_raddr2_write(HI_U32 data) { 
    IOWR_32DIRECT(0x62a8, data);
}

static __inline HI_U32 hi_isp_drc_stat_ind_raddr2_read(HI_VOID) {
    return (IORD_32DIRECT(0x62a8));
}

/*****************************************************
ISP_DRC_STAT_IND_RDATA  DRC ColorCorrectionLut表间接读数据寄存器。
0x205a62ac:                 31:00     drc_stat_ind_rdata
*****************************************************/
static __inline HI_VOID hi_isp_drc_stat_ind_rdata2_write(HI_U32 data) { 
    IOWR_32DIRECT(0x62ac, data);
}

static __inline HI_U32 hi_isp_drc_stat_ind_rdata2_read(HI_VOID) {
    return (IORD_32DIRECT(0x62ac));
}


/*****************************************************
MODULE        -----GAMMA-----
MODULE        -----GAMMA-----
MODULE        -----GAMMA-----
*****************************************************/
// args: data (1-bit)
static __inline HI_VOID hi_isp_gamma_enable_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x7300);
    IOWR_8DIRECT(0x7300, (data & 0x01) | (u8Current & 0xFE));
}
static __inline HI_U8 hi_isp_gamma_enable_read(HI_VOID) {
    return (IORD_8DIRECT(0x7300) & 0x01);
}

static __inline HI_VOID hi_isp_gamma_lut_update_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x7310);
    IOWR_8DIRECT(0x7310, (data & 0x01) | (u8Current & 0xFE));
}
static __inline HI_U8 hi_isp_gamma_lut_update_read(HI_VOID) {
    return (IORD_8DIRECT(0x7310) & 0x01);
}

static __inline HI_VOID hi_isp_gamma_waddr_write(HI_U32 data) {
	IOWR_32DIRECT(0x7380, data);
}
static __inline HI_U32 hi_isp_gamma_waddr_read(HI_VOID) {
    return IORD_32DIRECT(0x7380);
}

static __inline HI_VOID hi_isp_gamma_wdata_write(HI_U32 data) {
	IOWR_32DIRECT(0x7384, data);
}
static __inline HI_U32 hi_isp_gamma_wdata_read(HI_VOID) {
    return IORD_32DIRECT(0x7384);
}

static __inline HI_VOID hi_isp_gamma_raddr_write(HI_U32 data) {
	IOWR_32DIRECT(0x7388, data);
}
static __inline HI_U32 hi_isp_gamma_raddr_read(HI_VOID) {
    return IORD_32DIRECT(0x7388);
}

static __inline HI_VOID hi_isp_gamma_rdata_write(HI_U32 data) {
	IOWR_32DIRECT(0x738c, data);
}
static __inline HI_U32 hi_isp_gamma_rdata_read(HI_VOID) {
    return IORD_32DIRECT(0x738c);
}

/*****************************************************
MODULE        -----GAMMAFE-----
MODULE        -----GAMMAFE-----
MODULE        -----GAMMAFE-----
*****************************************************/
/*****************************************************
ISP_GAMMAFE_CFG:         工作使能寄存器
0x205a7400:                     31:1      reserved
                                          0           GAMMA使能
*****************************************************/
static __inline HI_VOID hi_isp_gamma_fe_enable_write(HI_U8 data) {
    IOWR_8DIRECT(0x7400, data);
}

static __inline HI_U8 hi_isp_gamma_fe_enable_read(HI_VOID) {
    return (IORD_8DIRECT(0x7400));
}

/*****************************************************
ISP_GAMMAFE_VERSION:         工作使能寄存器
0x205a740c:                     31:20      version
                                          19:16      reserved
                                          15: 0       date
*****************************************************/
static __inline HI_VOID hi_isp_gamma_fe_version_write(HI_U16 data) {
	HI_U16 u16current = IORD_16DIRECT(0x400e);
    IOWR_16DIRECT(0x740e, (data << 4) | (u16current&0xf));
}

static __inline HI_U16 hi_isp_gamma_fe_version_read(HI_VOID) {
    return (IORD_16DIRECT(0x740e) >> 4);
}

static __inline HI_VOID hi_isp_gamma_fe_date_write(HI_U16 data) {
    IOWR_16DIRECT(0x740c, data);
}

static __inline HI_U16 hi_isp_gamma_fe_date_read(HI_VOID) {
    return (IORD_16DIRECT(0x740c));
}

/*****************************************************
ISP_GAMMAFE_CTRL_BITW:         工作使能寄存器
0x205a7410:                     31:21      reserved
                                          20:16      bitw_in
                                          15:05      reserved
                                          04:00      bitw_out
*****************************************************/
static __inline HI_VOID hi_isp_gamma_fe_bitw_in_write(HI_U8 data) {
    IOWR_8DIRECT(0x7412, (data & 0x1f));
}

static __inline HI_U8 hi_isp_gamma_fe_bitw_in_read(HI_VOID) {
    return (IORD_8DIRECT(0x7412) & 0x1f);
}

static __inline HI_VOID hi_isp_gamma_fe_bitw_out_write(HI_U8 data) {
    IOWR_8DIRECT(0x7410, (data & 0x1f));
}

static __inline HI_U8 hi_isp_gamma_fe_bitw_out_read(HI_VOID) {
    return (IORD_8DIRECT(0x7410) & 0x1f);
}

/*****************************************************
ISP_GAMMAFE_MIX_CTRL:         工作使能寄存器
0x205a7414:                     31:08     reserved
                                          07:00     mix_ctrl 控制输出数据混合比例。
                                          Mix_ctrl=0  相当于直接截位输出。
						    Mix_ctrl=255插值结果输出。
*****************************************************/
static __inline HI_VOID hi_isp_gamma_fe_mix_ctrl_write(HI_U8 data) {
    IOWR_8DIRECT(0x7414, data);
}

static __inline HI_U8 hi_isp_gamma_fe_mix_ctrl_read(HI_VOID) {
    return (IORD_8DIRECT(0x7414));
}

/*****************************************************
ISP_GAMMAFE_WADDR0:         GAMAM_FE的257长度LUT的写地址。
0x205a7480:                     31:00      gamma_waddr
*****************************************************/
static __inline HI_VOID hi_isp_gamma_fe_waddr0_write(HI_U32 data) {
    IOWR_32DIRECT(0x7480, data);
}

static __inline HI_U32 hi_isp_gamma_fe_waddr0_read(HI_VOID) {
    return (IORD_32DIRECT(0x7480));
}

/*****************************************************
ISP_GAMMAFE_WDATA0:  GAMAM_FE的257长度LUT的写数据。
0x205a7484:                     31:00      gamma_wdata
*****************************************************/
static __inline HI_VOID hi_isp_gamma_fe_wdata0_write(HI_U32 data) {
    IOWR_32DIRECT(0x7484, data);
}

static __inline HI_U32 hi_isp_gamma_fe_wdata0_read(HI_VOID) {
    return (IORD_32DIRECT(0x7484));
}

/*****************************************************
ISP_GAMMAFE_RADDR0:         GAMAM_FE的257长度LUT的读地址。
0x205a7488:                     31:00      gamma_raddr
*****************************************************/
static __inline HI_VOID hi_isp_gamma_fe_raddr0_write(HI_U32 data) {
    IOWR_32DIRECT(0x7488, data);
}

static __inline HI_U32 hi_isp_gamma_fe_raddr0_read(HI_VOID) {
    return (IORD_32DIRECT(0x7488));
}

/*****************************************************
ISP_GAMMAFE_RDATA0:  GAMAM_FE的257长度LUT的读数据。
0x205a748c:                     31:00      gamma_rdata
*****************************************************/
static __inline HI_VOID hi_isp_gamma_fe_rdata0_write(HI_U32 data) {
    IOWR_32DIRECT(0x748c, data);
}

static __inline HI_U32 hi_isp_gamma_fe_rdata0_read(HI_VOID) {
    return (IORD_32DIRECT(0x748c));
}

/*****************************************************
ISP_GAMMAFE_WADDR1:         GAMAM_FE的257长度LUT的写地址。
0x205a7490:                     31:00      gamma_waddr
*****************************************************/
static __inline HI_VOID hi_isp_gamma_fe_waddr1_write(HI_U32 data) {
    IOWR_32DIRECT(0x7490, data);
}

static __inline HI_U32 hi_isp_gamma_fe_waddr1_read(HI_VOID) {
    return (IORD_32DIRECT(0x7490));
}

/*****************************************************
ISP_GAMMAFE_WDATA0:  GAMAM_FE的257长度LUT的写数据。
0x205a7494:                     31:00      gamma_wdata
*****************************************************/
static __inline HI_VOID hi_isp_gamma_fe_wdata1_write(HI_U32 data) {
    IOWR_32DIRECT(0x7494, data);
}

static __inline HI_U32 hi_isp_gamma_fe_wdata1_read(HI_VOID) {
    return (IORD_32DIRECT(0x7494));
}

/*****************************************************
ISP_GAMMAFE_RADDR0:         GAMAM_FE的257长度LUT的读地址。
0x205a7498:                     31:00      gamma_raddr
*****************************************************/
static __inline HI_VOID hi_isp_gamma_fe_raddr1_write(HI_U32 data) {
    IOWR_32DIRECT(0x7498, data);
}

static __inline HI_U32 hi_isp_gamma_fe_raddr1_read(HI_VOID) {
    return (IORD_32DIRECT(0x7498));
}

/*****************************************************
ISP_GAMMAFE_RDATA0:  GAMAM_FE的257长度LUT的读数据。
0x205a749c:                     31:00      gamma_rdata
*****************************************************/
static __inline HI_VOID hi_isp_gamma_fe_rdata1_write(HI_U32 data) {
    IOWR_32DIRECT(0x749c, data);
}

static __inline HI_U32 hi_isp_gamma_fe_rdata1_read(HI_VOID) {
    return (IORD_32DIRECT(0x749c));
}

/*****************************************************
MODULE        -----GE-----
MODULE        -----GE-----
MODULE        -----GE-----
*****************************************************/
/*****************************************************
ISP_GE_CFG:         GE控制寄存器。
0x205a5100:         31:09      reserved
				     08:08      isp_ge           GE校正使能；0：禁止；1：使能。	
                              07:05      reserved 
                              04:04      isp_ge_aver   GE_AVER使能；0：禁止；1：使能。
                              03:01      reserved
                              00:00      en                  GE使能；0：禁止；1：使能。
*****************************************************/
static __inline HI_VOID hi_isp_ge_enable_write(HI_U8 data) {
	HI_U8 u8Current = IORD_8DIRECT(0x5100);
    IOWR_8DIRECT(0x5100, (data & 0x1) | (u8Current & 0xfe));
}

static __inline HI_U8 hi_isp_ge_enable_read(HI_VOID) {
    return (IORD_8DIRECT(0x5100) & 0x1);
}

static __inline HI_VOID hi_isp_ge_aver_enable_write(HI_U8 data) {
	HI_U8 u8Current = IORD_8DIRECT(0x5100);
    IOWR_8DIRECT(0x5100, ((data & 0x1) << 4) | (u8Current & 0xef));
}

static __inline HI_U8 hi_isp_ge_aver_enable_read(HI_VOID) {
    return ((IORD_8DIRECT(0x5100) & 0x10) >> 4);
}

static __inline HI_VOID hi_isp_ge_correct_enable_write(HI_U8 data) {
    IOWR_8DIRECT(0x5101, (data & 0x1));
}

static __inline HI_U8 hi_isp_ge_correct_enable_read(HI_VOID) {
    return ((IORD_8DIRECT(0x5101) & 0x1));
}

/*****************************************************
ISP_GE_VERSION:         工作使能寄存器
0x205a510c:                     31:20      ISP版本,1:V100；2：V200；3：V300依此类推
                                          19:16      保留
                                          15:  0      发布日期。
*****************************************************/

static __inline HI_VOID hi_isp_ge_version_write(HI_U16 data) {
	HI_U16 u16current = IORD_16DIRECT(0x510e);
    IOWR_16DIRECT(0x510e, (data & 0xfff) << 4 | (u16current & 0xf));
}

static __inline HI_U16 hi_isp_ge_version_read(HI_VOID) {
    return ((IORD_16DIRECT(0x510e) & 0xfff) >> 4);
}

static __inline HI_VOID hi_isp_ge_date_write(HI_U16 data) {
    IOWR_16DIRECT(0x510c, data);
}

static __inline HI_U16 hi_isp_ge_date_read(HI_VOID) {
    return (IORD_16DIRECT(0x510c));
}

/*****************************************************
ISP_GE_CT_TH1:         GE配置寄存器。
0x205a5110:         31:14      reserved
				     13:00      ct_th1   门限值配置。
*****************************************************/
static __inline HI_VOID hi_isp_ge_ct_th1_write(HI_U16 data) {
    IOWR_16DIRECT(0x5110, (data & 0x3fff));
}

static __inline HI_U16 hi_isp_ge_ct_th1_read(HI_VOID) {
    return (IORD_16DIRECT(0x5110) & 0x3fff);
}

/*****************************************************
ISP_GE_CT_TH2:         GE配置寄存器。
0x205a5114:         31:14      reserved
				     13:00      ct_th2   门限值配置。
*****************************************************/
static __inline HI_VOID hi_isp_ge_ct_th2_write(HI_U16 data) {
    IOWR_16DIRECT(0x5114, (data & 0x3fff));
}

static __inline HI_U16 hi_isp_ge_ct_th2_read(HI_VOID) {
    return (IORD_16DIRECT(0x5114) & 0x3fff);
}

/*****************************************************
ISP_GE_CT_TH3:         GE配置寄存器。
0x205a5118:         31:14      reserved
				     13:00      ct_th3   门限值配置。
*****************************************************/
static __inline HI_VOID hi_isp_ge_ct_th3_write(HI_U16 data) {
    IOWR_16DIRECT(0x5118, (data & 0x3fff));
}

static __inline HI_U16 hi_isp_ge_ct_th3_read(HI_VOID) {
    return (IORD_16DIRECT(0x5118) & 0x3fff);
}

/*****************************************************
ISP_GE_CT_SLOPE1:         GE配置寄存器。
0x205a511c:         31:04      reserved
				    03:00      ct_slope1   0:1,1:2,2:4,3:8。
*****************************************************/
static __inline HI_VOID hi_isp_ge_ct_slope1_write(HI_U8 data) {
    IOWR_8DIRECT(0x511c, (data & 0xf));
}

static __inline HI_U8 hi_isp_ge_ct_slope1_read(HI_VOID) {
    return (IORD_8DIRECT(0x511c) & 0xf);
}

/*****************************************************
ISP_GE_CT_SLOPE2:         GE配置寄存器。
0x205a5120:         31:04      reserved
				     03:00      ct_slope2   0:1,1:2,2:4,3:8。
*****************************************************/
static __inline HI_VOID hi_isp_ge_ct_slope2_write(HI_U8 data) {
    IOWR_8DIRECT(0x5120, (data & 0xf));
}

static __inline HI_U8 hi_isp_ge_ct_slope2_read(HI_VOID) {
    return (IORD_8DIRECT(0x5120) & 0xf);
}

/*****************************************************
ISP_GE_ZONE:         GE配置寄存器。
0x205a5130:         31:13      reserved
				     12:08      vnum     垂直方向分窗个数。
				     07:05      reserved
				     04:00      hnum     水平方向分窗个数。
*****************************************************/
static __inline HI_VOID hi_isp_ge_hnum_write(HI_U8 data) {
    IOWR_8DIRECT(0x5130, (data & 0x1f));
}

static __inline HI_U8 hi_isp_ge_hnum_read(HI_VOID) {
    return (IORD_8DIRECT(0x5130) & 0x1f);
}

static __inline HI_VOID hi_isp_ge_vnum_write(HI_U8 data) {
    IOWR_8DIRECT(0x5131, (data & 0x1f));
}

static __inline HI_U8 hi_isp_ge_vnum_read(HI_VOID) {
    return (IORD_8DIRECT(0x5131) & 0x1f);
}

/*****************************************************
ISP_GE_MEM_AVER_RADDR:         GE均值统计寄存器读地址寄存器。
0x205a5188:         31:00      aver_raddr
*****************************************************/
static __inline HI_VOID hi_isp_ge_aver_raddr_write(HI_U32 data) {
    IOWR_32DIRECT(0x5188, data);
}

static __inline HI_U32 hi_isp_ge_aver_raddr_read(HI_VOID) {
    return (IORD_32DIRECT(0x5188));
}

/*****************************************************
ISP_GE_MEM_AVER_RDATA:         GE均值统计寄存器读地址寄存器。
0x205a518c:         31:00      aver_rdata
*****************************************************/
static __inline HI_VOID hi_isp_ge_aver_rdata_write(HI_U32 data) {
    IOWR_32DIRECT(0x518c, data);
}

static __inline HI_U32 hi_isp_ge_aver_rdata_read(HI_VOID) {
    return (IORD_32DIRECT(0x518c));
}

/*****************************************************
MODULE        -----NR-----
MODULE        -----NR-----
MODULE        -----NR-----
*****************************************************/
/*****************************************************
ISP_2DNR_CFG:         工作使能寄存器
0x205a4000:                     31:2      reserved
						     1      luma_nr_en
                                          0      2DNR使能
*****************************************************/
static __inline HI_VOID hi_isp_2dnr_cfg_enable_write(HI_U8 data) {
	HI_U8 u8Current = IORD_8DIRECT(0x4000);
    IOWR_8DIRECT(0x4000, (data & 0x1) | (u8Current & 0xfe));
}

static __inline HI_U8 hi_isp_2dnr_cfg_enable_read(HI_VOID) {
    return (IORD_8DIRECT(0x4000) & 0x1);
}

static __inline HI_VOID hi_isp_2dnr_luma_nr_enable_write(HI_U8 data) {
	HI_U8 u8Current = IORD_8DIRECT(0x4000);
    IOWR_8DIRECT(0x4000, ((data & 0x1) << 1) | (u8Current & 0xfd));
}

static __inline HI_U8 hi_isp_2dnr_luma_nr_enable_read(HI_VOID) {
    return ((IORD_8DIRECT(0x4000) & 0x2) >> 1);
}

/*****************************************************
ISP_2DNR_VERSION:         工作使能寄存器
0x205a400c:                     31:20      version
                                          19:16      reserved
                                          15: 0       date
*****************************************************/
static __inline HI_VOID hi_isp_2dnr_version_write(HI_U16 data) {
	HI_U16 u16current = IORD_16DIRECT(0x400e);
    IOWR_16DIRECT(0x400e, (data << 4) | (u16current&0xf));
}

static __inline HI_U16 hi_isp_2dnr_version_read(HI_VOID) {
    return (IORD_16DIRECT(0x400e) >> 4);
}

static __inline HI_VOID hi_isp_2dnr_date_write(HI_U16 data) {
    IOWR_16DIRECT(0x400c, data);
}

static __inline HI_U16 hi_isp_2dnr_date_read(HI_VOID) {
    return (IORD_16DIRECT(0x400c));
}

/*****************************************************
ISP_2DNR_FCR:        2DNR去伪色配置寄存器
0x205a4010:                     31:25      reserved
                                          24           falsecolorenable
                                          23: 16     falsecolorshift
                                          15:8        falsecolorgain
                                          7:0          falsecolorlimit
*****************************************************/
static __inline HI_VOID hi_isp_2dnr_fcr_en_write(HI_U8 data) {
    IOWR_8DIRECT(0x4013, (data&0x1));
}

static __inline HI_U8 hi_isp_2dnr_fcr_en_read(HI_VOID) {
    return (IORD_8DIRECT(0x4013)&0x1);
}

static __inline HI_VOID hi_isp_2dnr_fcrshift_write(HI_U8 data) {
    IOWR_8DIRECT(0x4012, (data));
}

static __inline HI_U8 hi_isp_2dnr_fcrshift_read(HI_VOID) {
    return (IORD_8DIRECT(0x4012));
}

static __inline HI_VOID hi_isp_2dnr_fcrgain_write(HI_U8 data) {
    IOWR_8DIRECT(0x4011, (data));
}

static __inline HI_U8 hi_isp_2dnr_fcrgain_read(HI_VOID) {
    return (IORD_8DIRECT(0x4011));
}

static __inline HI_VOID hi_isp_2dnr_fcrlimit_write(HI_U8 data) {
    IOWR_8DIRECT(0x4010, (data));
}

static __inline HI_U8 hi_isp_2dnr_fcrlimit_read(HI_VOID) {
    return (IORD_8DIRECT(0x4010));
} 

/*****************************************************
ISP_2DNR_LUMANR:          亮度降噪增益
0x205a4014:                     31:05      reserved
                                          04:00      luma_nr_gain 亮度降噪增益，[0,16]
*****************************************************/
static __inline HI_VOID hi_isp_2dnr_luma_gain_write(HI_U8 data) {
    IOWR_8DIRECT(0x4014, (data&0x1f));
}

static __inline HI_U8 hi_isp_2dnr_luma_gain_read(HI_VOID) {
    return (IORD_8DIRECT(0x4014)&0x1f);
}


/*****************************************************
ISP_2DNR_LUMA:        当前点的亮度限制
0x205a4018:                     31:30      reserved
                                          29:16      luma_low
                                          15: 14     reserved
                                          13:0        luma_high
*****************************************************/
static __inline HI_VOID hi_isp_2dnr_luma_low_write(HI_U16 data) {
    IOWR_16DIRECT(0x401a, (data&0x3fff));
}

static __inline HI_U16 hi_isp_2dnr_luma_low_read(HI_VOID) {
    return (IORD_16DIRECT(0x401a)&0x3fff);
}

static __inline HI_VOID hi_isp_2dnr_luma_high_write(HI_U16 data) {
    IOWR_16DIRECT(0x4018, (data&0x3fff));
}

static __inline HI_U16 hi_isp_2dnr_luma_high_read(HI_VOID) {
    return (IORD_16DIRECT(0x4018)&0x3fff);
}

/*****************************************************
ISP_2DNR_LIMIT:        2DNR与DEMOSAIC的融合比率范围限制
0x205a401c:                     31:25       reserved
                                          24:16      limit_low
                                          15: 9       reserved
                                          8:0          limit_high
*****************************************************/
static __inline HI_VOID hi_isp_2dnr_limit_low_write(HI_U16 data) {
    IOWR_16DIRECT(0x401e, (data&0x1ff));
}

static __inline HI_U16 hi_isp_2dnr_limit_low_read(HI_VOID) {
    return (IORD_16DIRECT(0x401e)&0x1ff);
}

static __inline HI_VOID hi_isp_2dnr_limit_high_write(HI_U16 data) {
    IOWR_16DIRECT(0x401c, (data&0x1ff));
}

static __inline HI_U16 hi_isp_2dnr_limit_high_read(HI_VOID) {
    return (IORD_16DIRECT(0x401c)&0x1ff);
}

/*****************************************************
ISP_2DNR_THRES :        2DNR与DEMOSAIC融合比率线性范围门限
0x205a4020                     31:28      reserved
                                          27:16      thres_low
                                          15:11      reserved
                                          10:0          thres_high
*****************************************************/
static __inline HI_VOID hi_isp_2dnr_thres_low_write(HI_U8 index, HI_U16 data) {
    IOWR_16DIRECT((0x4022 + index*0x4),(data&0xfff));
}

static __inline HI_U16 hi_isp_2dnr_thres_low_read(HI_U8 index) {
    return (IORD_16DIRECT(0x4022 + index*0x4)&0xfff);
}

static __inline HI_VOID hi_isp_2dnr_thres_high_write(HI_U8 index, HI_U16 data) {
    IOWR_16DIRECT((0x4020 + index*0x4), (data&0xfff));
}

static __inline HI_U16 hi_isp_2dnr_thres_high_read(HI_U8 index) {
    return (IORD_16DIRECT(0x4020 + index*0x4)&0xfff);
}

/*****************************************************
MODULE        -----SHARPEN-----
MODULE        -----SHARPEN-----
MODULE        -----SHARPEN-----
*****************************************************/
/*****************************************************
ISP_SHARPEN_CFG:         工作使能寄存器
0x205a4200:                     31:1      reserved
                                          0      sharpen使能
*****************************************************/
static __inline HI_VOID hi_isp_sharpen_cfg_enable_write(HI_U8 data) {
    IOWR_8DIRECT(0x4200, data);
}

static __inline HI_U8 hi_isp_sharpen_cfg_enable_read(HI_VOID) {
    return (IORD_8DIRECT(0x4200)&0x1);
}

/*****************************************************
ISP_2DNR_VERSION:         SHARPEN版本号寄存器
0x205a420c:                     31:20      version
                                          19:16      reserved
                                          15: 0       date
*****************************************************/
static __inline HI_VOID hi_isp_sharpen_version_write(HI_U16 data) {
	HI_U16 u16current = IORD_16DIRECT(0x420e);
    IOWR_16DIRECT(0x420e, (data << 4) | (u16current&0xf));
}

static __inline HI_U16 hi_isp_sharpen_version_read(HI_VOID) {
    return (IORD_16DIRECT(0x420e) >> 4);
}

static __inline HI_VOID hi_isp_sharpen_date_write(HI_U16 data) {
    IOWR_16DIRECT(0x420c, data);
}

static __inline HI_U16 hi_isp_sharpen_date_read(HI_VOID) {
    return (IORD_16DIRECT(0x420c));
}

/*****************************************************
ISP_SHARPEN_AMT:         SHARPEN强度
0x205a4210:                     31:28      reserved
                                          27:16      edge_amt
                                          15:12      reserved
                                          11:  0      sharp_amt
*****************************************************/
static __inline HI_VOID hi_isp_edge_amt_write(HI_U16 data) {
    IOWR_16DIRECT(0x4212, (data&0xfff));
}

static __inline HI_U16 hi_isp_edge_amt_read(HI_VOID) {
    return (IORD_16DIRECT(0x4212)&0xfff);
}

static __inline HI_VOID hi_isp_sharp_amt_write(HI_U16 data) {
    IOWR_16DIRECT(0x4210, (data&0xfff));
}

static __inline HI_U16 hi_isp_sharp_amt_read(HI_VOID) {
    return (IORD_16DIRECT(0x4210)&0xfff);
}

/*****************************************************
ISP_SHARPEN_THD1:         SHARPEN阈值
0x205a4214:                     
                                          31:16     edge_thd1
                                          15:  0     sharp_thd1
*****************************************************/
static __inline HI_VOID hi_isp_edge_thd1_write(HI_U16 data) {
    IOWR_16DIRECT(0x4216, data);
}

static __inline HI_U16 hi_isp_edge_thd1_read(HI_VOID) {
    return (IORD_16DIRECT(0x4216));
}

static __inline HI_VOID hi_isp_sharp_thd1_write(HI_U16 data) {
    IOWR_16DIRECT(0x4214, data);
}

static __inline HI_U16 hi_isp_sharp_thd1_read(HI_VOID) {
    return (IORD_16DIRECT(0x4214));
}

/*****************************************************
ISP_SHARPEN_COEF:         SHARPEN强度系数
0x205a4218:                     31:28     reserved
                                          27:16     edge_coef
                                          15:12     reserved
                                          11:  0     sharp_coef
*****************************************************/
static __inline HI_VOID hi_isp_edge_coef_write(HI_U16 data) {
	IOWR_16DIRECT(0x421a, (data&0xfff));
}

static __inline HI_U16 hi_isp_edge_coef_read(HI_VOID) {
	return (IORD_16DIRECT(0x421a)&0xfff);
}

static __inline HI_VOID hi_isp_sharp_coef_write(HI_U16 data) {
	IOWR_16DIRECT(0x4218, (data&0xfff));
}

static __inline HI_U16 hi_isp_sharp_coef_read(HI_VOID) {
	return (IORD_16DIRECT(0x4218)&0xfff);
}

/*****************************************************
ISP_SHARPEN_SHOOTAMT:      SHARPENshoot控制
0x205a421c:                     31:24   reserved
                                          23:16   over_amt
                                          15:8     reserved
                                          7:  0     under_amt
*****************************************************/
static __inline HI_VOID hi_isp_over_amt_write(HI_U8 data) {
	IOWR_8DIRECT(0x421e, data);
}

static __inline HI_U8 hi_isp_over_amt_read(HI_VOID) {
	return (IORD_8DIRECT(0x421e));
}

static __inline HI_VOID hi_isp_under_amt_write(HI_U8 data) {
	IOWR_8DIRECT(0x421c, data);
}

static __inline HI_U8 hi_isp_under_amt_read(HI_VOID) {
	return (IORD_8DIRECT(0x421c));
}

/*****************************************************
ISP_SHARPEN_MID0:      SHARPEN中频滤波系数
0x205a4230:                     31:24   reserved
                                          23:16   mid_tmp02
                                          15:8     mid_tmp01
                                          7:  0     mid_tmp00
*****************************************************/
static __inline HI_VOID hi_isp_sharpen_mid_tmp02_write(HI_U8 data) {
	IOWR_8DIRECT(0x4232, data);
}

static __inline HI_U8 hi_isp_sharpen_mid_tmp02_read(HI_VOID) {
	return (IORD_8DIRECT(0x4232));
}

static __inline HI_VOID hi_isp_sharpen_mid_tmp01_write(HI_U8 data) {
	IOWR_8DIRECT(0x4231, data);
}

static __inline HI_U8 hi_isp_sharpen_mid_tmp01_read(HI_VOID) {
	return (IORD_8DIRECT(0x4231));
}

static __inline HI_VOID hi_isp_sharpen_mid_tmp00_write(HI_U8 data) {
	IOWR_8DIRECT(0x4230, data);
}

static __inline HI_U8 hi_isp_sharpen_mid_tmp00_read(HI_VOID) {
	return (IORD_8DIRECT(0x4230));
}

/*****************************************************
ISP_SHARPEN_MID1:      SHARPEN中频滤波系数
0x205a4234:                     31:24   reserved
                                          23:16   mid_tmp12
                                          15:8     mid_tmp11
                                          7:  0     mid_tmp10
*****************************************************/
static __inline HI_VOID hi_isp_sharpen_mid_tmp12_write(HI_U8 data) {
	IOWR_8DIRECT(0x4236, data);
}

static __inline HI_U8 hi_isp_sharpen_mid_tmp12_read(HI_VOID) {
	return (IORD_8DIRECT(0x4236));
}

static __inline HI_VOID hi_isp_sharpen_mid_tmp11_write(HI_U8 data) {
	IOWR_8DIRECT(0x4235, data);
}

static __inline HI_U8 hi_isp_sharpen_mid_tmp11_read(HI_VOID) {
	return (IORD_8DIRECT(0x4235));
}

static __inline HI_VOID hi_isp_sharpen_mid_tmp10_write(HI_U8 data) {
	IOWR_8DIRECT(0x4234, data);
}

static __inline HI_U8 hi_isp_sharpen_mid_tmp10_read(HI_VOID) {
	return (IORD_8DIRECT(0x4234));
}

/*****************************************************
ISP_SHARPEN_MID2:      SHARPEN中频滤波系数
0x205a4238:                     31:24   reserved
                                          23:16   mid_tmp22
                                          15:8     mid_tmp21
                                          7:  0     mid_tmp20
*****************************************************/
static __inline HI_VOID hi_isp_sharpen_mid_tmp22_write(HI_U8 data) {
	IOWR_8DIRECT(0x423a, data);
}

static __inline HI_U8 hi_isp_sharpen_mid_tmp22_read(HI_VOID) {
	return (IORD_8DIRECT(0x423a));
}

static __inline HI_VOID hi_isp_sharpen_mid_tmp21_write(HI_U8 data) {
	IOWR_8DIRECT(0x4239, data);
}

static __inline HI_U8 hi_isp_sharpen_mid_tmp21_read(HI_VOID) {
	return (IORD_8DIRECT(0x4239));
}

static __inline HI_VOID hi_isp_sharpen_mid_tmp20_write(HI_U8 data) {
	IOWR_8DIRECT(0x4238, data);
}

static __inline HI_U8 hi_isp_sharpen_mid_tmp20_read(HI_VOID) {
	return (IORD_8DIRECT(0x4238));
}

/*****************************************************
ISP_SHARPEN_LINETHD:    SHARPEN线条检测阈值
0x205a423c:                      31:24   reserved
                                          23:16   line_thd2
                                          15:8     reserved
                                          7:  0     line_thd1
*****************************************************/
static __inline HI_VOID hi_isp_sharpen_line_thd2_write(HI_U8 data) {
	IOWR_8DIRECT(0x423e, data);
}

static __inline HI_U8 hi_isp_sharpen_line_thd2_read(HI_VOID) {
	return (IORD_8DIRECT(0x423e));
}

static __inline HI_VOID hi_isp_sharpen_line_thd1_write(HI_U8 data) {
	IOWR_8DIRECT(0x423c, data);
}

static __inline HI_U8 hi_isp_sharpen_line_thd1_read(HI_VOID) {
	return (IORD_8DIRECT(0x423c));
}

/*****************************************************
ISP_SHARPEN_THD2:         SHARPEN阈值
0x205a4240:                     
                                          31:16     edge_thd2
                                          15:  0     sharp_thd2
*****************************************************/
static __inline HI_VOID hi_isp_edge_thd2_write(HI_U16 data) {
    IOWR_16DIRECT(0x4242, data);
}

static __inline HI_U16 hi_isp_edge_thd2_read(HI_VOID) {
    return (IORD_16DIRECT(0x4242));
}

static __inline HI_VOID hi_isp_sharp_thd2_write(HI_U16 data) {
    IOWR_16DIRECT(0x4240, data);
}

static __inline HI_U16 hi_isp_sharp_thd2_read(HI_VOID) {
    return (IORD_16DIRECT(0x4240));
}


/*****************************************************
ISP_SHARPEN_SHOOTTHD:      SHARPENshootTHD控制
0x205a4244:                     31:24   over_thd2
                                          23:16   over_thd1
                                          15:8     under_thd2
                                          7:  0     under_thd1
*****************************************************/
static __inline HI_VOID hi_isp_sharpen_over_thd2_write(HI_U8 data) {
	IOWR_8DIRECT(0x4247, data);
}

static __inline HI_U8 hi_isp_sharpen_over_thd2_read(HI_VOID) {
	return (IORD_8DIRECT(0x4247));
}

static __inline HI_VOID hi_isp_sharpen_over_thd1_write(HI_U8 data) {
	IOWR_8DIRECT(0x4246, data);
}

static __inline HI_U8 hi_isp_sharpen_over_thd1_read(HI_VOID) {
	return (IORD_8DIRECT(0x4246));
}

static __inline HI_VOID hi_isp_sharpen_under_thd2_write(HI_U8 data) {
	IOWR_8DIRECT(0x4245, data);
}

static __inline HI_U8 hi_isp_sharpen_under_thd2_read(HI_VOID) {
	return (IORD_8DIRECT(0x4245));
}

static __inline HI_VOID hi_isp_sharpen_under_thd1_write(HI_U8 data) {
	IOWR_8DIRECT(0x4244, data);
}

static __inline HI_U8 hi_isp_sharpen_under_thd1_read(HI_VOID) {
	return (IORD_8DIRECT(0x4244));
}


/*****************************************************
ISP_SHARPEN_SHOOTCOEF:      SHARPENshootCOEF控制
0x205a4248:                     31:25   reserverd
                                          24:16   over_coef
                                          15:9     reserverd
                                          8:  0     under_coef
*****************************************************/
static __inline HI_VOID hi_isp_sharpen_over_coef_write(HI_U16 data) {
	IOWR_16DIRECT(0x424a, data&0x1ff);
}

static __inline HI_U16 hi_isp_sharpen_over_coef_read(HI_VOID) {
	return (IORD_16DIRECT(0x424a));
}

static __inline HI_VOID hi_isp_sharpen_under_coef_write(HI_U16 data) {
	IOWR_16DIRECT(0x4248, data&0x1ff);
}

static __inline HI_U16 hi_isp_sharpen_under_coef_read(HI_VOID) {
	return (IORD_16DIRECT(0x4248));
}

/*****************************************************
ISP_SHARPEN_ENPIXSEL:      SHARPENENPIXSEL控制
0x205a424c:                          31:01   reserverd
                                              0          enpixsel
*****************************************************/
static __inline HI_VOID hi_isp_sharpen_enpixsel_write(HI_U8 data) {
	IOWR_8DIRECT(0x424c, (data&0x1));
}

static __inline HI_U8 hi_isp_sharpen_enpixsel_read(HI_VOID) {
	return (IORD_8DIRECT(0x424c) & 0x1);
}

/*****************************************************
ISP_SHARPEN_SIZE:      SHARPEN中频滤波系数
0x205a42f0:                     31:29    reserved
                                          28:16   height
                                          15:13   reserved
                                          12:  0   width
*****************************************************/
static __inline HI_VOID hi_isp_sharpen_height_write(HI_U16 data) {
	IOWR_16DIRECT(0x42f2, (data&0x1fff));
}

static __inline HI_U16 hi_isp_sharpen_height_read(HI_VOID) {
	return (IORD_16DIRECT(0x42f2)&0x1fff);
}

static __inline HI_VOID hi_isp_sharpen_width_write(HI_U16 data) {
	IOWR_16DIRECT(0x42f0, (data&0x1fff));
}

static __inline HI_U8 hi_isp_sharpen_width_read(HI_VOID) {
	return (IORD_16DIRECT(0x42f0)&0x1fff);
}

/*****************************************************
MODULE        -----UVNR-----
MODULE        -----UVNR-----
MODULE        -----UVNR-----
*****************************************************/
/*****************************************************
ISP_UVNR_CFG:          ISP_UVNR_CFG为         UVNR控制寄存器。
0x205a1600: 		   31:13	      reserved
					   12:12      uvnr_sel           UVNR降噪通道选择；0：midf通道；1：uvnr通道；
					   11:09      reserved
					   08:08      nr_en               UVNR降噪使能；0：关闭；1：使能；
					   07:05      reserved
					   04:04      midf_en           MIDFILTER使能；0：关闭；1：使能；
					   03:03      uv2c_mode      UV2C bypass模式。0：输出U分量数据；1：输出V分量数据。
					   02:02      uv2c_en           UV2C使能，将内部两分量UV数据调整为单分量C数据。0：关闭；1：使能。
					   01:01      vcds_en           色度垂直下采样使能。0：关闭；1：使能。
					   00:00      hcds_en           色度水平下采样使能。0：关闭；1：使能。
*****************************************************/
static __inline HI_VOID hi_isp_uvnr_sel_write(HI_U8 data) {
	HI_U8 u8Current = IORD_8DIRECT(0x1601);
	IOWR_8DIRECT(0x1601, (data & 0x1) << 4 | (u8Current & 0xef));
}

static __inline HI_U8 hi_isp_uvnr_sel_read(HI_VOID) {
	return ((IORD_8DIRECT(0x1601)&0x10) >> 4);
}

static __inline HI_VOID hi_isp_nr_en_write(HI_U8 data) {
	HI_U8 u8Current = IORD_8DIRECT(0x1601);
	IOWR_8DIRECT(0x1601, (data & 0x1) | (u8Current & 0xfe));
}

static __inline HI_U8 hi_isp_nr_en_read(HI_VOID) {
	return ((IORD_8DIRECT(0x1601)&0x1));
}

static __inline HI_VOID hi_isp_midf_en_write(HI_U8 data) {
	HI_U8 u8Current = IORD_8DIRECT(0x1600);
	IOWR_8DIRECT(0x1600, (data & 0x1) << 4 | (u8Current & 0xef));
}

static __inline HI_U8 hi_isp_midf_en_read(HI_VOID) {
	return ((IORD_8DIRECT(0x1600)&0x10) >> 4);
}

static __inline HI_VOID hi_isp_uv2c_mode_write(HI_U8 data) {
	HI_U8 u8Current = IORD_8DIRECT(0x1600);
	IOWR_8DIRECT(0x1600, (data & 0x1) << 3 | (u8Current & 0xf7));
}

static __inline HI_U8 hi_isp_uv2c_mode_read(HI_VOID) {
	return ((IORD_8DIRECT(0x1600)&0x8) >> 3);
}

static __inline HI_VOID hi_isp_uv2c_en_write(HI_U8 data) {
	HI_U8 u8Current = IORD_8DIRECT(0x1600);
	IOWR_8DIRECT(0x1600, (data & 0x1) << 2 | (u8Current & 0xfb));
}

static __inline HI_U8 hi_isp_uv2c_en_read(HI_VOID) {
	return ((IORD_8DIRECT(0x1600)&0x4) >> 2);
}

static __inline HI_VOID hi_isp_vcds_en_write(HI_U8 data) {
	HI_U8 u8Current = IORD_8DIRECT(0x1600);
	IOWR_8DIRECT(0x1600, (data & 0x1) << 1 | (u8Current & 0xfd));
}

static __inline HI_U8 hi_isp_vcds_en_read(HI_VOID) {
	return ((IORD_8DIRECT(0x1600)&0x2) >> 1);
}

static __inline HI_VOID hi_isp_hcds_en_write(HI_U8 data) {
	HI_U8 u8Current = IORD_8DIRECT(0x1600);
	IOWR_8DIRECT(0x1600, (data & 0x1)| (u8Current & 0xfe));
}

static __inline HI_U8 hi_isp_hcds_en_read(HI_VOID) {
	return ((IORD_8DIRECT(0x1600)&0x1));
}

/*****************************************************
ISP_HCDS_SIZE:       ISP_HCD_SSIZE为色度水平下采样输入大小置寄存器。
0x205a1614:          31:29   reserved
                                 28:16   width_out  色度水平下采样输出图像U或V一行的宽度（以像素为单位），配置值为实际值减1。
                                 15:13   reserved
                                 12:00   width_in     输入图像U或V一行的宽度（以像素为单位），配置值为实际值减1。
*****************************************************/
static __inline HI_VOID hi_isp_hcds_width_out_write(HI_U16 data) {
	IOWR_16DIRECT(0x1616, (data&0x1fff));
}

static __inline HI_U16 hi_isp_hcds_width_out_read(HI_VOID) {
	return (IORD_16DIRECT(0x1616)&0x1fff);
}

static __inline HI_VOID hi_isp_hcds_width_in_write(HI_U16 data) {
	IOWR_16DIRECT(0x1614, (data&0x1fff));
}

static __inline HI_U16 hi_isp_hcds_width_in_read(HI_VOID) {
	return (IORD_16DIRECT(0x1614)&0x1fff);
}

/*****************************************************
ISP_DSUS_MODE:     ISP_DSUS_MODE为色度水平下采样输出大小置寄存器。
0x205a1618:          31:03   reserved
                                 02:02   vcus_mode  UV垂直上采样倍数。0：上采样2倍。1：上采样4倍。
                                 01:01   vcds_mode  UV垂直下采样倍数。0：下采样2倍。1：下采样4倍。
                                 00:00   hcds_mode  UV水平下采样倍数。0：下采样2倍。1：下采样4倍。
*****************************************************/
static __inline HI_VOID hi_isp_vcus_mode_write(HI_U8 data) {
	HI_U8 u8Current = IORD_8DIRECT(0x1618);
	IOWR_8DIRECT(0x1618, ((data & 0x1) << 2)| (u8Current & 0xfb));
}

static __inline HI_U8 hi_isp_vcus_mode_read(HI_VOID) {
	return ((IORD_8DIRECT(0x1618)&0x4) >> 2);
}

static __inline HI_VOID hi_isp_vcds_mode_write(HI_U8 data) {
	HI_U8 u8Current = IORD_8DIRECT(0x1618);
	IOWR_8DIRECT(0x1618, ((data & 0x1) << 1)| (u8Current & 0xfd));
}

static __inline HI_U8 hi_isp_vcds_mode_read(HI_VOID) {
	return ((IORD_8DIRECT(0x1618)&0x2) >> 1);
}

static __inline HI_VOID hi_isp_hcds_mode_write(HI_U8 data) {
	HI_U8 u8Current = IORD_8DIRECT(0x1618);
	IOWR_8DIRECT(0x1618, (data & 0x1)| (u8Current & 0xfe));
}

static __inline HI_U8 hi_isp_hcds_mode_read(HI_VOID) {
	return (IORD_8DIRECT(0x1618)&0x1);
}

/*****************************************************
ISP_HCDS_COEF0:      ISP_HCDS_COEF0为色度水平下采样系数寄存器0。
0x205a161c:              31:30   reserved
                                  29:20   coef2  色度水平缩放滤波系数2。
                                  19:10   coef1  色度水平缩放滤波系数1。
                                  09:00   coef0  色度水平缩放滤波系数0。
*****************************************************/
static __inline HI_VOID hi_isp_hcds_coef2_write(HI_U32 data) {
	HI_U32 u32Current = IORD_32DIRECT(0x161c);
	IOWR_32DIRECT(0x161c, ((data & 0x3ff) << 20) | (u32Current & 0xc00fffff));
}

static __inline HI_U32 hi_isp_hcds_coef2_read(HI_VOID) {
	return ((IORD_32DIRECT(0x161c)&0x3ff00000) >> 20);
}

static __inline HI_VOID hi_isp_hcds_coef1_write(HI_U32 data) {
	HI_U32 u32Current = IORD_32DIRECT(0x161c);
	IOWR_32DIRECT(0x161c, ((data & 0x3ff) << 10) | (u32Current & 0xfff003ff));
}

static __inline HI_U32 hi_isp_hcds_coef1_read(HI_VOID) {
	return ((IORD_32DIRECT(0x161c)&0xffc00) >> 10);
}

static __inline HI_VOID hi_isp_hcds_coef0_write(HI_U32 data) {
	HI_U32 u32Current = IORD_32DIRECT(0x161c);
	IOWR_32DIRECT(0x161c, (data & 0x3ff) | (u32Current & 0xfffffc00));
}

static __inline HI_U32 hi_isp_hcds_coef0_read(HI_VOID) {
	return ((IORD_32DIRECT(0x161c)&0x3ff));
}

/*****************************************************
ISP_HCDS_COEF1:     ISP_HCDS_COEF0为色度水平下采样系数寄存器0。
0x205a1620:             31:30   reserved
                                  29:20   coef5  色度水平缩放滤波系数5。
                                  19:10   coef4  色度水平缩放滤波系数4。
                                  09:00   coef3  色度水平缩放滤波系数3。
*****************************************************/
static __inline HI_VOID hi_isp_hcds_coef5_write(HI_U32 data) {
	HI_U32 u32Current = IORD_32DIRECT(0x1620);
	IOWR_32DIRECT(0x1620, ((data & 0x3ff) << 20) | (u32Current & 0xc00fffff));
}

static __inline HI_U32 hi_isp_hcds_coef5_read(HI_VOID) {
	return ((IORD_32DIRECT(0x1620)&0x3ff00000) >> 20);
}

static __inline HI_VOID hi_isp_hcds_coef4_write(HI_U32 data) {
	HI_U32 u32Current = IORD_32DIRECT(0x1620);
	IOWR_32DIRECT(0x1620, ((data & 0x3ff) << 10) | (u32Current & 0xfff003ff));
}

static __inline HI_U32 hi_isp_hcds_coef4_read(HI_VOID) {
	return ((IORD_32DIRECT(0x1620)&0xffc00) >> 10);
}

static __inline HI_VOID hi_isp_hcds_coef3_write(HI_U32 data) {
	HI_U32 u32Current = IORD_32DIRECT(0x1620);
	IOWR_32DIRECT(0x1620, (data & 0x3ff) | (u32Current & 0xfffffc00));
}

static __inline HI_U32 hi_isp_hcds_coef3_read(HI_VOID) {
	return ((IORD_32DIRECT(0x1620)&0x3ff));
}

/*****************************************************
ISP_HCDS_COEF1:     ISP_HCDS_COEF0为色度水平下采样系数寄存器0。
0x205a1624:             31:20   reserved
                                  19:10   coef7  色度水平缩放滤波系数7。
                                  09:00   coef6  色度水平缩放滤波系数6。
*****************************************************/
static __inline HI_VOID hi_isp_hcds_coef7_write(HI_U32 data) {
	HI_U32 u32Current = IORD_32DIRECT(0x1624);
	IOWR_32DIRECT(0x1624, ((data & 0x3ff) << 10) | (u32Current & 0xfff003ff));
}

static __inline HI_U32 hi_isp_hcds_coef7_read(HI_VOID) {
	return ((IORD_32DIRECT(0x1624)&0xffc00) >> 10);
}

static __inline HI_VOID hi_isp_hcds_coef6_write(HI_U32 data) {
	HI_U32 u32Current = IORD_32DIRECT(0x1624);
	IOWR_32DIRECT(0x1624, (data & 0x3ff) | (u32Current & 0xfffffc00));
}

static __inline HI_U32 hi_isp_hcds_coef6_read(HI_VOID) {
	return ((IORD_32DIRECT(0x1624)&0x3ff));
}

/*****************************************************
ISP_VCDS_COEF:      色度垂直下采样系数寄存器。
0x205a1628:            31:27   reserved
                                 26:24   下采样系数3。与系数0一致。
                                 23:19   reserved
                                 18:16   下采样系数2。与系数0一致。
                                 15:11   reserved
                                 10:08   coef1
                                 07:03   reserved
                                 02:00   coef0
*****************************************************/
static __inline HI_VOID hi_isp_vcds_coef3_write(HI_U8 data) {
	IOWR_8DIRECT(0x162b, (data & 0x7));
}

static __inline HI_U8 hi_isp_vcds_coef3_read(HI_VOID) {
	return (IORD_8DIRECT(0x162b)&0x7);
}

static __inline HI_VOID hi_isp_vcds_coef2_write(HI_U8 data) {
	IOWR_8DIRECT(0x162a, (data & 0x7));
}

static __inline HI_U8 hi_isp_vcds_coef2_read(HI_VOID) {
	return (IORD_8DIRECT(0x162a)&0x7);
}

static __inline HI_VOID hi_isp_vcds_coef1_write(HI_U8 data) {
	IOWR_8DIRECT(0x1629, (data & 0x7));
}

static __inline HI_U8 hi_isp_vcds_coef1_read(HI_VOID) {
	return (IORD_8DIRECT(0x1629)&0x7);
}

static __inline HI_VOID hi_isp_vcds_coef0_write(HI_U8 data) {
	IOWR_8DIRECT(0x1628, (data & 0x7));
}

static __inline HI_U8 hi_isp_vcds_coef0_read(HI_VOID) {
	return (IORD_8DIRECT(0x1628)&0x7);
}

/*****************************************************
ISP_MIDFILTER_SIZE:       中值滤波输入图像色度宽高
0x205a162c:                      31:29   reserved
						    28:16   height
                                          15:13   reserved
                                          12:00   width
*****************************************************/
static __inline HI_VOID hi_isp_midfilter_width_write(HI_U32 data) {
	HI_U32 u32Current = IORD_32DIRECT(0x162c);
	IOWR_32DIRECT(0x162c, (u32Current & 0xffffe000) | (data & 0x1fff));
}

static __inline HI_U16 hi_isp_midfilter_width_read(HI_VOID) {
	return (IORD_32DIRECT(0x162c) & 0x1fff);
}

static __inline HI_VOID hi_isp_midfilter_height_write(HI_U32 data) {
	HI_U32 u32Current = IORD_32DIRECT(0x162c);
	IOWR_32DIRECT(0x162c, (u32Current & 0xe000ffff) | ((data & 0x1fff) << 16));
}

static __inline HI_U16 hi_isp_midfilter_height_read(HI_VOID) {
	return ((IORD_32DIRECT(0x162c) & 0x1fff0000) >> 16);
}

/*****************************************************
ISP_NR_SIZE:       NR输入图像色度宽高
0x205a1630:                      31:29   reserved
						    28:16   height
                                          15:13   reserved
                                          12:00   width
*****************************************************/
static __inline HI_VOID hi_isp_uvnr_width_write(HI_U32 data) {
	HI_U32 u32Current = IORD_32DIRECT(0x1630);
	IOWR_32DIRECT(0x1630, (u32Current & 0xffffe000) | (data & 0x1fff));
}

static __inline HI_U16 hi_isp_uvnr_width_read(HI_VOID) {
	return (IORD_32DIRECT(0x1630) & 0x1fff);
}

static __inline HI_VOID hi_isp_uvnr_height_write(HI_U32 data) {
	HI_U32 u32Current = IORD_32DIRECT(0x1630);
	IOWR_32DIRECT(0x1630, (u32Current & 0xe000ffff) | ((data & 0x1fff) << 16));
}

static __inline HI_U16 hi_isp_uvnr_height_read(HI_VOID) {
	return ((IORD_32DIRECT(0x1630) & 0x1fff0000) >> 16);
}

/*****************************************************
ISP_UVNR_DIFFTHRES0:      diff的门限值
0x205a1634:                        31:24   diffthres4
						       23:16   diffthres3
                                            15:08   diffthres2
                                            07:00   diffthres1
*****************************************************/
static __inline HI_VOID hi_isp_uvnr_diffthres4_write(HI_U8 data) {
	IOWR_8DIRECT(0x1637, data);
}

static __inline HI_U8 hi_isp_uvnr_diffthres4_read(HI_VOID) {
	return IORD_8DIRECT(0x1637);
}

static __inline HI_VOID hi_isp_uvnr_diffthres3_write(HI_U8 data) {
	IOWR_8DIRECT(0x1636, data);
}

static __inline HI_U8 hi_isp_uvnr_diffthres3_read(HI_VOID) {
	return IORD_8DIRECT(0x1636);
}

static __inline HI_VOID hi_isp_uvnr_diffthres2_write(HI_U8 data) {
	IOWR_8DIRECT(0x1635, data);
}

static __inline HI_U8 hi_isp_uvnr_diffthres2_read(HI_VOID) {
	return IORD_8DIRECT(0x1635);
}

static __inline HI_VOID hi_isp_uvnr_diffthres1_write(HI_U8 data) {
	IOWR_8DIRECT(0x1634, data);
}

static __inline HI_U8 hi_isp_uvnr_diffthres1_read(HI_VOID) {
	return IORD_8DIRECT(0x1634);
}

/*****************************************************
ISP_UVNR_DIFFTHRES1:      diff的门限值
0x205a1638:                        31:24   reserved
                                             15:08   diffthres6
                                             07:00   diffthres5
*****************************************************/
static __inline HI_VOID hi_isp_uvnr_diffthres6_write(HI_U8 data) {
	IOWR_8DIRECT(0x1639, data);
}

static __inline HI_U8 hi_isp_uvnr_diffthres6_read(HI_VOID) {
	return IORD_8DIRECT(0x1639);
}

static __inline HI_VOID hi_isp_uvnr_diffthres5_write(HI_U8 data) {
	IOWR_8DIRECT(0x1638, data);
}

static __inline HI_U8 hi_isp_uvnr_diffthres5_read(HI_VOID) {
	return IORD_8DIRECT(0x1638);
}

/*****************************************************
ISP_UVNR_RATIO0:      ratiolimit权重限制
0x205a163c:                        31:31    reserved
                                            30:24    ratiolimit3 距离3的权重上限，7bit无符号数。
                                            23:23    reserved
                                            22:16    ratiolimit2 距离2的权重上限，7bit无符号数。
                                            15:15    reserved
                                            14:08    ratiolimit1 距离1的权重上限，7bit无符号数。
                                            07:07    reserved
                                            06:00    ratiolimit0 距离0的权重上限，7bit无符号数。
*****************************************************/
static __inline HI_VOID hi_isp_uvnr_ratiolimit3_write(HI_U8 data) {
	IOWR_8DIRECT(0x163f, data&0x7f);
}

static __inline HI_U8 hi_isp_uvnr_ratiolimit3_read(HI_VOID) {
	return IORD_8DIRECT(0x163f);
}

static __inline HI_VOID hi_isp_uvnr_ratiolimit2_write(HI_U8 data) {
	IOWR_8DIRECT(0x163e, data&0x7f);
}

static __inline HI_U8 hi_isp_uvnr_ratiolimit2_read(HI_VOID) {
	return IORD_8DIRECT(0x163e);
}

static __inline HI_VOID hi_isp_uvnr_ratiolimit1_write(HI_U8 data) {
	IOWR_8DIRECT(0x163d, data&0x7f);
}

static __inline HI_U8 hi_isp_uvnr_ratiolimit1_read(HI_VOID) {
	return IORD_8DIRECT(0x163d);
}

static __inline HI_VOID hi_isp_uvnr_ratiolimit0_write(HI_U8 data) {
	IOWR_8DIRECT(0x163c, data&0x7f);
}

static __inline HI_U8 hi_isp_uvnr_ratiolimit0_read(HI_VOID) {
	return IORD_8DIRECT(0x163c);
}

/*****************************************************
ISP_UVNR_RATIO1:      ratiolimit权重限制
0x205a1640:                        31:23    reserved
                                            22:16    ratiolimit6 距离2的权重上限，7bit无符号数。
                                            15:15    reserved
                                            14:08    ratiolimit5 距离1的权重上限，7bit无符号数。
                                            07:07    reserved
                                            06:00    ratiolimit4 距离0的权重上限，7bit无符号数。
*****************************************************/
static __inline HI_VOID hi_isp_uvnr_ratiolimit6_write(HI_U8 data) {
	IOWR_8DIRECT(0x1642, data&0x7f);
}

static __inline HI_U8 hi_isp_uvnr_ratiolimit6_read(HI_VOID) {
	return IORD_8DIRECT(0x1642);
}

static __inline HI_VOID hi_isp_uvnr_ratiolimit5_write(HI_U8 data) {
	IOWR_8DIRECT(0x1641, data&0x7f);
}

static __inline HI_U8 hi_isp_uvnr_ratiolimit5_read(HI_VOID) {
	return IORD_8DIRECT(0x1641);
}

static __inline HI_VOID hi_isp_uvnr_ratiolimit4_write(HI_U8 data) {
	IOWR_8DIRECT(0x1640, data&0x7f);
}

static __inline HI_U8 hi_isp_uvnr_ratiolimit4_read(HI_VOID) {
	return IORD_8DIRECT(0x1640);
}

/*****************************************************
ISP_UVNR_PARA:       NR的融合比例和移位
0x205a1644:              31:22   reserved
					   21:16   coring_limit
					   15:10   reserved      coring功能，limit设置，配置范围0-63。
                                   09:08   diffshift       无符号的2bit，取值0,1,2,3。
                                   07:06   reserved
                                   05:00   blendratio   blendRatio无符号的6bit数据，取值0-32。
*****************************************************/
static __inline HI_VOID hi_isp_uvnr_coring_limit_write(HI_U8 data) {
	IOWR_8DIRECT(0x1646, data&0x3f);
}

static __inline HI_U8 hi_isp_uvnr_coring_limit_read(HI_VOID) {
	return (IORD_8DIRECT(0x1646));
}

static __inline HI_VOID hi_isp_uvnr_blendratio_write(HI_U8 data) {
	IOWR_8DIRECT(0x1644, data&0x3f);
}

static __inline HI_U8 hi_isp_uvnr_blendratio_read(HI_VOID) {
	return (IORD_8DIRECT(0x1644));
}

static __inline HI_VOID hi_isp_uvnr_diffshift_write(HI_U8 data) {
	IOWR_8DIRECT(0x1645, data&0x3);
}

static __inline HI_U8 hi_isp_uvnr_diffshift_read(HI_VOID) {
	return (IORD_8DIRECT(0x1645));
}

#define HI_ISP_MIDFILTER_TIMING_DEFAULT (0x20)
/*****************************************************
ISP_MIDFILTER_TIMING:       MIDFILTER输出时序配置寄存器
0x205a16e8:                     31:14   reserved
                                          13:01   cpi_fix_timing_stat 手动时序参数设置，设置生成的行消隐区长度
                                          00:00   cpi_auto_timing_en 自动时序输出使能，使统计出来的输入行消隐区长度等于输出行消隐区长度。只用于MIDF。
*****************************************************/
static __inline HI_VOID hi_isp_midfilter_timing_write(HI_U16 data) {
	HI_U16 u16Current = IORD_16DIRECT(0x16e8);
	IOWR_16DIRECT(0x16e8, ((data&0x1fff) << 1) | (u16Current & 0x1));
}

static __inline HI_U16 hi_isp_midfilter_timing_read(HI_VOID) {
	return ((IORD_16DIRECT(0x16e8)&0x3ffe) >> 1);
}

static __inline HI_VOID hi_isp_midfilter_auto_timing_en_write(HI_U8 data) {
	HI_U16 u16Current = IORD_16DIRECT(0x16e8);
	IOWR_16DIRECT(0x16e8, (data&0x1) | (u16Current & 0x3ffe));
}

static __inline HI_U8 hi_isp_midfilter_auto_timing_en_read(HI_VOID) {
	return ((IORD_16DIRECT(0x16e8)&0x1));
}

/*****************************************************
MODULE        -----STAT-----
MODULE        -----STAT-----
MODULE        -----STAT-----
*****************************************************/
/*****************************************************
ISP_STAT_EN:      stat工作使能寄存器
0x20584000: 	   31:02	 reserved
				   01:01 drc_rd_en DRC读通道使能。1：使能，0：关闭	
				   00:00 stat_en	STAT使能。1：使能，0：关闭
*****************************************************/
static __inline HI_VOID hi_isp_stat_en_write(HI_U32 data) {
	HI_U32 u32Current = IORD_32DIRECT_VI(0x4000);
	IOWR_32DIRECT_VI(0x4000, (data & 0x1) | (u32Current & 0x2));
}

static __inline HI_U8 hi_isp_stat_en_read(HI_VOID) {
	return (IORD_32DIRECT_VI(0x4000) & 0x1);
}

static __inline HI_VOID hi_isp_drc_rd_en_write(HI_U32 data) {
	HI_U32 u32Current = IORD_32DIRECT_VI(0x4000);
	IOWR_32DIRECT_VI(0x4000, ((data & 0x1) << 1) | (u32Current & 0x1));
}

static __inline HI_U8 hi_isp_drc_rd_en_read(HI_VOID) {
	return ((IORD_32DIRECT_VI(0x4000) & 0x2) >> 1);
}


/*****************************************************
ISP_STAT_FRM_BADDR:	   stat帧数据基地址
0x20584008: 		               31:12	 reserved
					               11:00  stat_frm_baddr    stat帧数据基地址
*****************************************************/
static __inline HI_VOID hi_isp_stat_frm_baddr_write(HI_U32 data) {
	IOWR_32DIRECT_VI(0x4008, (data & 0xfff));
}

static __inline HI_U32 hi_isp_stat_frm_baddr_read(HI_VOID) {
	return (IORD_32DIRECT_VI(0x4008) & 0xfff);
}

/*****************************************************
ISP_STAT_DRCA_FRM_BADDR:	   drca帧数据基地址
0x20584020: 		               31:20	 reserved
					               19:00  drca_frm_baddr    drca帧数据基地址
*****************************************************/
static __inline HI_VOID hi_isp_drca_frm_baddr_write(HI_U32 data) {
	IOWR_32DIRECT_VI(0x4020, (data & 0xfffff));
}

static __inline HI_U32 hi_isp_drca_frm_baddr_read(HI_VOID) {
	return (IORD_32DIRECT_VI(0x4020) & 0xfffff);
}

/*****************************************************
ISP_STAT_DRCB_FRM_BADDR:	   drcb帧数据基地址
0x20584024: 		               31:20	 reserved
					               19:00  drcb_frm_baddr    drcb帧数据基地址
*****************************************************/
static __inline HI_VOID hi_isp_drcb_frm_baddr_write(HI_U32 data) {
	IOWR_32DIRECT_VI(0x4024, (data & 0xfffff));
}

static __inline HI_U32 hi_isp_drcb_frm_baddr_read(HI_VOID) {
	return (IORD_32DIRECT_VI(0x4024) & 0xfffff);
}

/*****************************************************
ISP_STAT_DRCA_RD_FRM_BADDR:	   drca read帧数据基地址
0x20584028: 		               31:20	 reserved
					               19:00  drca_rd_frm_baddr    drca read 帧数据基地址
*****************************************************/
static __inline HI_VOID hi_isp_drca_rd_frm_baddr_write(HI_U32 data) {
	IOWR_32DIRECT_VI(0x4028, (data & 0xfffff));
}

static __inline HI_U32 hi_isp_drca_rd_frm_baddr_read(HI_VOID) {
	return (IORD_32DIRECT_VI(0x4028) & 0xfffff);
}

/*****************************************************
ISP_STAT_DRCB_RD_FRM_BADDR:	   drcb read帧数据基地址
0x2058402c: 		               31:20	 reserved
					               19:00  drcb_rd_frm_baddr    drcb帧数据基地址
*****************************************************/
static __inline HI_VOID hi_isp_drcb_rd_frm_baddr_write(HI_U32 data) {
	IOWR_32DIRECT_VI(0x402c, (data & 0xfffff));
}

static __inline HI_U32 hi_isp_drcb_rd_frm_baddr_read(HI_VOID) {
	return (IORD_32DIRECT_VI(0x402c) & 0xfffff);
}

/*****************************************************
MODULE        -----LSC-----
MODULE        -----LSC-----
MODULE        -----LSC-----
*****************************************************/
/*****************************************************
ISP_LSC_CFG:         工作使能寄存器
0x205a5800:                     31:1      reserved
                                          0      lsc使能
*****************************************************/
static __inline HI_VOID hi_isp_lsc_cfg_enable_write(HI_U8 data) {
    HI_U8 u8Current = IORD_8DIRECT(0x5800);
    IOWR_8DIRECT(0x5800, (data & 0x1) | (u8Current & 0xfe));
}

static __inline HI_U8 hi_isp_lsc_cfg_enable_read(HI_VOID) {
    return (IORD_8DIRECT(0x5800) & 0x1);
}

/*****************************************************
ISP_LSC_VERSION:         LSC版本号寄存器
0x205a580c:                     31:0      version                                         
                                       
*****************************************************/
static __inline HI_U32 hi_isp_lsc_version_read(HI_VOID) {
    return (IORD_32DIRECT(0x580c));
}

/*****************************************************
ISP_LSC_WINNUM:         图像分割的窗口个数配置寄存器
0x205a5810:                    	31:21		reserved                                        
                                20:16		numv
                                15:5		reserved
                                4:0			numh			
*****************************************************/
static __inline HI_VOID hi_isp_lsc_win_num_write(HI_U8 numh, HI_U8 numv){
	HI_U32 u32Current = IORD_32DIRECT(0x5810);
	IOWR_32DIRECT(0x5810, (u32Current & 0xff00ff00) | ((numv & 0xff) << 16) | (numh & 0xff));
}

static __inline HI_U8 hi_isp_lsc_win_numh_read(HI_VOID) {
    return (IORD_32DIRECT(0x5810) & 0xfffffff0);
}

static __inline HI_U8 hi_isp_lsc_win_numv_read(HI_VOID) {
    return ((IORD_32DIRECT(0x5810) & 0xfff0ffff) >> 16);
}

/*****************************************************
ISP_LSC_WINX:         LSC插值水平方向8个窗口宽度信息寄存器
0x205a5814~0x205a5830                   31:28		reserved                                         
                                		27:16		invx1
			                      		15:10		reserved
                                		9:0			deltax1			
*****************************************************/
static __inline HI_VOID hi_isp_lsc_hwin_info_write(HI_U8 index, HI_U16 delta, HI_U16 inv){
	HI_U32 u32Current = IORD_32DIRECT(0x5814 + (index * 0x4));
	IOWR_32DIRECT((0x5814 + (index * 0x4)), ((u32Current & 0xf000fc00) | (delta&0x3ff) | ((inv&0xfff) << 16)));
}

static __inline HI_U16 hi_isp_lsc_hwin_deltax_read(HI_U8 index){
	return (IORD_32DIRECT((0x5814 + (index * 0x4))) & 0x3ff);
}

static __inline HI_U16 hi_isp_lsc_hwin_invx_read(HI_U8 index){
	return ((IORD_32DIRECT((0x5814 + (index * 0x4))) & 0xfff0000) >> 16);
}


/*****************************************************
ISP_LSC_WINX:         LSC插值垂直方向8个窗口宽度信息寄存器
0x205a5834~0x205a5850                   31:28		reserved                                         
                                		27:16		invy1
			                      		15:10		reserved
                                		9:0			deltay1		
*****************************************************/
static __inline HI_VOID hi_isp_lsc_vwin_info_write(HI_U8 index, HI_U16 delta, HI_U16 inv){
	HI_U32 u32Current = IORD_32DIRECT(0x5834 + (index * 0x4));
	IOWR_32DIRECT((0x5834 + (index * 0x4)), ((u32Current & 0xf000fc00) | (delta&0x3ff) | ((inv&0xfff) << 16)));
}

static __inline HI_U16 hi_isp_lsc_vwin_deltay_read(HI_U8 index){
	return (IORD_32DIRECT((0x5834 + (index * 0x4))) & 0x3ff);
}

static __inline HI_U16 hi_isp_lsc_vwin_invy_read(HI_U8 index){
	return ((IORD_32DIRECT((0x5834 + (index * 0x4))) & 0xfff0000) >> 16);
}


/*****************************************************
ISP_LSC_GRRGAIN_WADDR:         LSC插值R通道和GR通道增益写地址寄存器
0x205a5880:                    	31:0		rgain_waddr                                         
                      	
*****************************************************/
static __inline HI_VOID hi_isp_lsc_grr_gain_waddr_write(HI_U32 data){
	IOWR_32DIRECT(0x5880, data);
}

static __inline HI_U32 hi_isp_lsc_grr_gain_waddr_read(HI_VOID) {
    return (IORD_32DIRECT(0x5880));
}

/*****************************************************
ISP_LSC_GRRGAIN_WDATA:         LSC插值R通道和GR通道增益信息写数据寄存器
0x205a5884:                    	31:0		rgain_wdata                                        
                      	
*****************************************************/
static __inline HI_VOID hi_isp_lsc_grr_gain_wdata_write(HI_U32 data){
    IOWR_32DIRECT(0x5884, data);
}

static __inline HI_U32 hi_isp_lsc_grr_gain_wdata_read(HI_VOID) {
    return (IORD_32DIRECT(0x5884));
}

/*****************************************************
ISP_LSC_GRRGAIN_RADDR:         LSC插值R通道和GR通道增益信息读地址寄存器
0x205a5888:                    	31:0		rgain_raddr                                       
                      	
*****************************************************/
static __inline HI_VOID hi_isp_lsc_grr_gain_raddr_write(HI_U32 data){
    IOWR_32DIRECT(0x5888, data);
}

static __inline HI_U32 hi_isp_lsc_grr_gain_raddr_read(HI_VOID) {
    return (IORD_32DIRECT(0x5888));
}

/*****************************************************
ISP_LSC_GRRGAIN_RDATA:         LSC插值R通道和GR通道增益信息读数据寄存器
0x205a588c:                    	31:0		grgain_rdata                                       
                      	
*****************************************************/
static __inline HI_U32 hi_isp_lsc_grr_gain_rdata_read(HI_VOID) {
    return (IORD_32DIRECT(0x588c));
}

/*****************************************************
ISP_LSC_GBBGAIN_WADDR:         LSC插值GB通道和B通道增益信息写地址寄存器
0x205a5890:                    	31:0		grgain_waddr                                       
                      	
*****************************************************/
static __inline HI_VOID hi_isp_lsc_gbb_gain_waddr_write(HI_U32 data){
	IOWR_32DIRECT(0x5890, data);
}

static __inline HI_U32 hi_isp_lsc_gbb_gain_waddr_read(HI_VOID) {
    return (IORD_32DIRECT(0x5890));
}

/*****************************************************
ISP_LSC_GBBGAIN_WDATA:         LSC插值GB通道和B通道增益信息写数据寄存器
0x205a5894:                    	31:0		grgain_wdata                                      
                      	
*****************************************************/
static __inline HI_VOID hi_isp_lsc_gbb_gain_wdata_write(HI_U32 data){
    IOWR_32DIRECT(0x5894, data);
}

static __inline HI_U32 hi_isp_lsc_gbb_gain_wdata_read(HI_VOID) {
    return (IORD_32DIRECT(0x5894));
}

/*****************************************************
ISP_LSC_GBBGAIN_RADDR:         LSC插值GB通道和B通道增益信息读地址寄存器
0x205a5898:                    	31:0		grgain_raddr                                     
                      	
*****************************************************/
static __inline HI_VOID hi_isp_lsc_gbb_gain_raddr_write(HI_U32 data){
    IOWR_32DIRECT(0x5898, data);
}

static __inline HI_U32 hi_isp_lsc_gbb_gain_raddr_read(HI_VOID) {
    return (IORD_32DIRECT(0x5898));
}

/*****************************************************
ISP_LSC_GBBGAIN_RDATA:         LSC插值GB通道和B通道增益信息读数据寄存器
0x205a589c:                    	31:0		grgain_rdata                                       
                      	
*****************************************************/
static __inline HI_U32 hi_isp_lsc_gbb_gain_rdata_read(HI_VOID) {
    return (IORD_32DIRECT(0x589c));
}


/*****************************************************
ISP_LSC_CTRL_F:         RGGB顺序配置寄存器
0x205a58e0:                    	31:2		greserved                                    
                      			1:0			rggb_cfg
*****************************************************/
static __inline HI_VOID hi_isp_lsc_rggb_cfg_write(HI_U8 data){
    HI_U8 u8Current = IORD_8DIRECT(0x58e0);
	IOWR_8DIRECT(0x58e0, (u8Current & 0xfc) | (data & 0x3));
}

static __inline HI_U8 hi_isp_lsc_rggb_cfg_read(HI_VOID) {
    return ((IORD_8DIRECT(0x58e0) & 0x3));
}

/*****************************************************
ISP_LSC_CTRL_I:         寄存器更新控制寄存器
0x205a58e4:                    	31:1		greserved                                    
                      			0			update_mode
*****************************************************/
static __inline HI_VOID hi_isp_lsc_update_mode_write(HI_U8 data){
	IOWR_8DIRECT(0x58e4, data);
}

static __inline HI_U8 hi_isp_lsc_update_mode_read(HI_VOID) {
    return (IORD_8DIRECT(0x58e4));
}

/*****************************************************
ISP_LSC_UPDATE:         寄存器更新控制寄存器
0x205a58ec:                    	31:1		greserved                                    
                      			0			newer
*****************************************************/
static __inline HI_VOID hi_isp_lsc_update_newer_write(HI_U8 data){
	IOWR_8DIRECT(0x58ec, data);
}

static __inline HI_U8 hi_isp_lsc_update_newer_read(HI_VOID) {
    return (IORD_8DIRECT(0x58ec));
}



/*****************************************************
ISP_COLORBAR_CFG:         寄存器控制寄存器
0x205a8100:                    31:1		greserved                                    
                      			0			en
*****************************************************/
static __inline HI_VOID hi_isp_colorbar_cfg_write(HI_U8 data){
	    HI_U8 u8Current = IORD_8DIRECT(0x8100);
    IOWR_8DIRECT(0x8100, (data & 0x1) | (u8Current & 0xfe));
}
static __inline HI_U8 hi_isp_colorbar_cfg_read(HI_VOID) {
	return (IORD_8DIRECT(0x8100) & 0x1);
}


/*****************************************************
MODULE        -----ACM-----
MODULE        -----ACM-----
MODULE        -----ACM-----
*****************************************************/
/*****************************************************
ISP_ACM_CTRL:         			ACM控制寄存器
0x205b1400:                    	31		acm_en
								30		acm_dbg_en
								29		acm_stretch
								28		acm_cliprange
								27		acm_cliporwrap
								26:8	reserved
                      			7:0		acm_cbcrthr
*****************************************************/
static __inline HI_VOID hi_isp_acm_enable_write(HI_U8 data){
	HI_U32 u32current = IORD_32DIRECT_ISP_REG(0x11400);
    IOWR_32DIRECT_ISP_REG(0x11400, (u32current & (~(1 << 31))) | ((data&0x1) << 31));
}

static __inline HI_U8 hi_isp_acm_enable_read(HI_VOID) {
    return ((IORD_32DIRECT_ISP_REG(0x11400) >> 31) & 0x1);
}

static __inline HI_VOID hi_isp_acm_dbg_enable_write(HI_U8 data){
	HI_U32 u32current = IORD_32DIRECT_ISP_REG(0x11400);
    IOWR_32DIRECT_ISP_REG(0x11400, (u32current & (~(1 << 30))) | ((data&0x1) << 30));
}

static __inline HI_U8 hi_isp_acm_dbg_enable_read(HI_VOID) {
    return ((IORD_32DIRECT_ISP_REG(0x11400) >> 30) & 0x1);
}

static __inline HI_VOID hi_isp_acm_stretch_write(HI_U8 data){
	HI_U32 u32current = IORD_32DIRECT_ISP_REG(0x11400);
    IOWR_32DIRECT_ISP_REG(0x11400, (u32current & (~(1 << 29))) | ((data&0x1) << 29));
}

static __inline HI_U8 hi_isp_acm_stretch_read(HI_VOID) {
    return ((IORD_32DIRECT_ISP_REG(0x11400) >> 29) & 0x1);
}

static __inline HI_VOID hi_isp_acm_cliprange_write(HI_U8 data){
	HI_U32 u32current = IORD_32DIRECT_ISP_REG(0x11400);
    IOWR_32DIRECT_ISP_REG(0x11400, (u32current & (~(1 << 28))) | ((data&0x1) << 28));
}

static __inline HI_U8 hi_isp_acm_cliprange_read(HI_VOID) {
    return ((IORD_32DIRECT_ISP_REG(0x11400) >> 28) & 0x1);
}

static __inline HI_VOID hi_isp_acm_cliporwrap_write(HI_U8 data){
	HI_U32 u32current = IORD_32DIRECT_ISP_REG(0x11400);
    IOWR_32DIRECT_ISP_REG(0x11400, (u32current & (~(1 << 27))) | ((data&0x1) << 27));
}

static __inline HI_U8 hi_isp_acm_cliporwrap_read(HI_VOID) {
    return ((IORD_32DIRECT_ISP_REG(0x11400) >> 27) & 0x1);
}

static __inline HI_VOID hi_isp_acm_cbcrthr_write(HI_U8 data){
	HI_U32 u32current = IORD_32DIRECT_ISP_REG(0x11400);
    IOWR_32DIRECT_ISP_REG(0x11400, (u32current & (~(0xff))) | (data & 0xff));
}

static __inline HI_U8 hi_isp_acm_cbcrthr_read(HI_VOID) {
    return (IORD_32DIRECT_ISP_REG(0x11400) & 0xff);
}

/*****************************************************
ISP_ACM_CTRL:         			ACM处理后像素的改变量寄存器
0x205b1404:                    	31:30		reserved
								29:20		acm_gain0 (luma)
								19:10		acm_gain1 (hue)
								9:0			acm_gain2 (sat)
								
*****************************************************/
static __inline HI_VOID hi_isp_acm_luma_gain_write(HI_U16 data){
	HI_U32 u32current = IORD_32DIRECT_ISP_REG(0x11404);
    IOWR_32DIRECT_ISP_REG(0x11404, (u32current & 0xc00fffff) | ((data & 0x3ff) << 20));
}

static __inline HI_U16 hi_isp_acm_luma_gain_read(HI_VOID) {
    return ((IORD_32DIRECT_ISP_REG(0x11404) & 0x3ff00000) >> 20);
}

static __inline HI_VOID hi_isp_acm_hue_gain_write(HI_U16 data){
	HI_U32 u32current = IORD_32DIRECT_ISP_REG(0x11404);
    IOWR_32DIRECT_ISP_REG(0x11404, (u32current & 0xfff003ff) | ((data & 0x3ff) << 10));
}

static __inline HI_U16 hi_isp_acm_hue_gain_read(HI_VOID) {
    return ((IORD_32DIRECT_ISP_REG(0x11404) & 0x000ffc00) >> 10);
}

static __inline HI_VOID hi_isp_acm_sat_gain_write(HI_U16 data){
	HI_U32 u32current = IORD_32DIRECT_ISP_REG(0x11404);
    IOWR_32DIRECT_ISP_REG(0x11404, (u32current & 0xfffffc00) | (data & 0x3ff));
}

static __inline HI_U16 hi_isp_acm_sat_gain_read(HI_VOID) {
    return ((IORD_32DIRECT_ISP_REG(0x11404) & 0x3ff));
}

/*****************************************************
ISP_ACM_SIZE:         			ACM处理图像大小寄存器
0x205b1410:                    	31:29		reserved
								28:16		height
								15:13		reserved
								12:0		width
								
*****************************************************/
static __inline HI_VOID hi_isp_acm_height_write(HI_U16 data){
	HI_U32 u32current = IORD_32DIRECT_ISP_REG(0x11410);
    IOWR_32DIRECT_ISP_REG(0x11410, (u32current & 0xe000ffff) | ((data & 0x1fff) << 16));
}

static __inline HI_U16 hi_isp_acm_height_read(HI_VOID) {
    return ((IORD_32DIRECT_ISP_REG(0x11410) & 0x1fff0000) >> 16);
}

static __inline HI_VOID hi_isp_acm_width_write(HI_U16 data){
	HI_U32 u32current = IORD_32DIRECT_ISP_REG(0x11410);
    IOWR_32DIRECT_ISP_REG(0x11410, (u32current & 0xffffe000) | (data & 0x1fff));
}

static __inline HI_U16 hi_isp_acm_width_read(HI_VOID) {
    return (IORD_32DIRECT_ISP_REG(0x11410) & 0x1fff);
}

/*****************************************************
ISP_ACM_PARA_ADDR:         		ACM系数配置载入地址寄存器
0x205b1414:                    	31:0		acm_para_addr
								
*****************************************************/
static __inline HI_VOID hi_isp_acm_coeff_addr_write(HI_U32 data){
    IOWR_32DIRECT_ISP_REG(0x11414, data);
}

static __inline HI_U32 hi_isp_acm_coeff_addr_read(HI_VOID) {
    return (IORD_32DIRECT_ISP_REG(0x11414));
}

/*****************************************************
ISP_ACM_PARA_UP:         		ACM系数配置更新寄存器
0x205b1420:                    	31:1		reserved
								0			para_up
								
*****************************************************/
static __inline HI_VOID hi_isp_acm_coeff_up_write(HI_U32 data){
    IOWR_32DIRECT_ISP_REG(0x11420, data);
}

static __inline HI_U32 hi_isp_acm_coeff_up_read(HI_VOID) {
    return (IORD_32DIRECT_ISP_REG(0x11420));
}

#if 0
/*****************************************************
MODULE        -----H_LDC-----
MODULE        -----H_LDC-----
MODULE        -----H_LDC-----
*****************************************************/
/*****************************************************
ISP_HLDC_CFG:				CH_HLDC工作使能寄存器。
0x205b2300: 				31:31		hldc_l_en                 HLDC亮度使能。
															0：关闭亮度HLDC；
															1：使能亮度HLDC。
							30:30          hldc_c_en
															HLDC色度使能。
															0：关闭色度HLDC；
															1：使能色度HLDC。
                                            29:01          reserved
                                            00:00          hldc_in_b422          HLDC处理数据格式。
															0：420；
															1：422。
*****************************************************/
static __inline HI_VOID hi_isp_hldc_l_en_write(HI_U8 data){
	HI_U32 u32current = IORD_32DIRECT_ISP_REG(0x12300);
	IOWR_32DIRECT_ISP_REG(0x12300, (u32current & 0x7fffffff) | ((data & 0x1) << 31));
}

static __inline HI_U8 hi_isp_hldc_l_en_read(HI_VOID) {
	return ((IORD_32DIRECT_ISP_REG(0x12300) & 0x80000000) >> 31);
}

static __inline HI_VOID hi_isp_hldc_c_en_write(HI_U8 data){
	HI_U32 u32current = IORD_32DIRECT_ISP_REG(0x12300);
	IOWR_32DIRECT_ISP_REG(0x12300, (u32current & 0xbfffffff) | ((data & 0x1) << 30));
}

static __inline HI_U8 hi_isp_hldc_c_en_read(HI_VOID) {
	return ((IORD_32DIRECT_ISP_REG(0x12300) & 0x40000000) >> 30);
}

static __inline HI_VOID hi_isp_hldc_in_b422_write(HI_U8 data){
	HI_U32 u32current = IORD_32DIRECT_ISP_REG(0x12300);
	IOWR_32DIRECT_ISP_REG(0x12300, (u32current & 0xfffffffe) | (data & 0x1));
}

static __inline HI_U8 hi_isp_hldc_in_b422_read(HI_VOID) {
	return ((IORD_32DIRECT_ISP_REG(0x12300) & 0x1));
}

/*****************************************************
ISP_HLDC_SIZE:			CH_HLDC_SIZE为HLDC处理亮度SIZE配置寄存器。
0x205b2310: 				31:29		reserved
							28:16          height
                                            15:13          reserved
                                            12:00          width
*****************************************************/
static __inline HI_VOID hi_isp_hldc_height_write(HI_U16 data){
	HI_U32 u32current = IORD_32DIRECT_ISP_REG(0x12310);
	IOWR_32DIRECT_ISP_REG(0x12310, (u32current & 0xe000ffff) | ((data & 0x1fff) << 16));
}

static __inline HI_U16 hi_isp_hldc_height_read(HI_VOID) {
	return ((IORD_32DIRECT_ISP_REG(0x12310) & 0x1fff0000) >> 16);
}

static __inline HI_VOID hi_isp_hldc_width_write(HI_U16 data){
	HI_U32 u32current = IORD_32DIRECT_ISP_REG(0x12310);
	IOWR_32DIRECT_ISP_REG(0x12310, (u32current & 0xffffe000) | ((data & 0x1fff)));
}

static __inline HI_U16 hi_isp_hldc_width_read(HI_VOID) {
	return ((IORD_32DIRECT_ISP_REG(0x12310) & 0x1fff ) );
}

/*****************************************************
ISP_HLDC_CENTER:			CH_HLDC中心坐标参数。
0x205b2320: 				31:24			ldc_norcoef
							23:12			ldc_y_center 
                            11:00			ldc_x_center
*****************************************************/
static __inline HI_VOID hi_isp_hldc_norcoef_write(HI_U8 data){
	HI_U32 u32current = IORD_32DIRECT_ISP_REG(0x12320);
	IOWR_32DIRECT_ISP_REG(0x12320, (u32current & 0x00ffffff) | ((data & 0xff) << 24));
}

static __inline HI_U8 hi_isp_hldc_norcoef_read(HI_VOID) {
	return ((IORD_32DIRECT_ISP_REG(0x12320) & 0xff000000) >> 24);
}

static __inline HI_VOID hi_isp_hldc_y_center_write(HI_U16 data){
	HI_U32 u32current = IORD_32DIRECT_ISP_REG(0x12320);
	IOWR_32DIRECT_ISP_REG(0x12320, (u32current & 0xff000fff) | ((data & 0xfff) << 12));
}

static __inline HI_U16 hi_isp_hldc_y_center_read(HI_VOID) {
	return ((IORD_32DIRECT_ISP_REG(0x12320) & 0xfff000) >> 12);
}

static __inline HI_VOID hi_isp_hldc_x_center_write(HI_U16 data){
	HI_U32 u32current = IORD_32DIRECT_ISP_REG(0x12320);
	IOWR_32DIRECT_ISP_REG(0x12320, (u32current & 0xfffff000) | ((data & 0xfff)));
}

static __inline HI_U16 hi_isp_hldc_x_center_read(HI_VOID) {
	return ((IORD_32DIRECT_ISP_REG(0x12320) & 0xfff));
}

/*****************************************************
ISP_HLDC_PARA:			CH_HLDC配置参数。
0x205b2324: 				31:22		reserved 
							21             ldc_black_mode
                            20:16          shift_right
                            15:07          ratio
                            06:00          scale
*****************************************************/
static __inline HI_VOID hi_isp_hldc_black_mode_write(HI_U8 data){
	HI_U32 u32current = IORD_32DIRECT_ISP_REG(0x12324);
	IOWR_32DIRECT_ISP_REG(0x12324, (u32current & (~(1 << 21))) | ((data & 0x1) << 21));
}

static __inline HI_U8 hi_isp_hldc_black_mode_read(HI_VOID) {
	return ((IORD_32DIRECT_ISP_REG(0x12324) & (1 << 21)) >> 21);
}

static __inline HI_VOID hi_isp_hldc_shift_right_write(HI_U8 data){
	HI_U32 u32current = IORD_32DIRECT_ISP_REG(0x12324);
	IOWR_32DIRECT_ISP_REG(0x12324, (u32current & 0xffe0ffff) | ((data & 0x1f) << 16));
}

static __inline HI_U8 hi_isp_hldc_shift_right_read(HI_VOID) {
	return ((IORD_32DIRECT_ISP_REG(0x12324) & 0x1f0000) >> 16);
}

static __inline HI_VOID hi_isp_hldc_ratio_write(HI_U16 data){
	HI_U32 u32current = IORD_32DIRECT_ISP_REG(0x12324);
	IOWR_32DIRECT_ISP_REG(0x12324, (u32current & 0xffff007f) | ((data & 0x1ff) << 7));
}

static __inline HI_U16 hi_isp_hldc_ratio_read(HI_VOID) {
	return ((IORD_32DIRECT_ISP_REG(0x12324) & 0xff80) >> 7);
}

static __inline HI_VOID hi_isp_hldc_scale_write(HI_U8 data){
	HI_U32 u32current = IORD_32DIRECT_ISP_REG(0x12324);
	IOWR_32DIRECT_ISP_REG(0x12324, (u32current & 0xffffff80) | ((data & 0x7f)));
}

static __inline HI_U8 hi_isp_hldc_scale_read(HI_VOID) {
	return ((IORD_32DIRECT_ISP_REG(0x12324) & 0x7f));
}
#endif

/*****************************************************
MODULE        -----ISP_CH-----
MODULE        -----ISP_CH-----
MODULE        -----ISP_CH-----
*****************************************************/
/*****************************************************
ISP_SWITCH422_CTRL:			ISP_CTRL为通道控制寄存器。
0x205b2000: 			31:31		enable
                                      30:00          reserved
*****************************************************/
static __inline HI_VOID hi_isp_ch_ctrl_en_write(HI_U8 data){
	IOWR_32DIRECT_ISP_REG(0x12000, (data&0x1) << 31);
}

static __inline HI_U8 hi_isp_ch_ctrl_en_read(HI_VOID) {
	return ((IORD_32DIRECT_ISP_REG(0x12000) & 0x80000000) >> 31);
}

/*****************************************************
MODULE        -----DEHAZE-----
MODULE        -----DEHAZE-----
MODULE        -----DEHAZE-----
*****************************************************/
/*****************************************************
ISP_DEHAZE_CFG: 		ISP_DEHAZE_CFG为DEHAZE使能寄存器。
0x205a7000: 			31:1		reserved									 
						0			en
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_en_write(HI_U8 data){
	IOWR_8DIRECT(0x7000, data);
}

static __inline HI_U8 hi_isp_dehaze_en_read(HI_VOID) {
	return (IORD_8DIRECT(0x7000));
}

/*****************************************************
ISP_DEHAZE_PRE_UPDATE: 		ISP_DEHAZE_PRE_UPDATE为DEHAZE统计结果更新寄存器
0x205a7010: 			31:1		reserved									 
						0			pre_update
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_pre_update_write(HI_U8 data){
	IOWR_8DIRECT(0x7010, data);
}

static __inline HI_U8 hi_isp_dehaze_pre_update_read(HI_VOID) {
	return (IORD_8DIRECT(0x7010));
}

/*****************************************************
ISP_DEHAZE_BLK_SIZE: 		ISP_DEHAZE_BLK_SIZE为DEHAZE分块大小配置寄存器。
0x205a7014: 			            31:01		reserved	
									24:16		block_sizeh   水平分窗大小，减1配入。
									15:06         reserved
						            08:00		block_sizev    垂直分窗大小，减1配入。
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_block_sizeh_write(HI_U16 data){
	HI_U32 u32Current = IORD_32DIRECT(0x7014);
	IOWR_32DIRECT(0x7014, (u32Current & 0xfe00ffff) | ((data & 0x1ff) << 16));
}

static __inline HI_U16 hi_isp_dehaze_block_sizeh_read(HI_VOID) {
	return ((IORD_32DIRECT(0x7014)&0x1ff0000) >> 16);
}

static __inline HI_VOID hi_isp_dehaze_block_sizev_write(HI_U16 data){
	HI_U32 u32Current = IORD_32DIRECT(0x7014);
	IOWR_32DIRECT(0x7014, (u32Current & 0xfffffe00) | (data & 0x1ff));
}

static __inline HI_U16 hi_isp_dehaze_block_sizev_read(HI_VOID) {
	return (IORD_32DIRECT(0x7014)&0x1ff);
}

/*****************************************************
ISP_DEHAZE_BLK_NUM: 		ISP_DEHAZE_BLK_NUM为DEHAZE分块个数配置寄存器。
0x205a7018: 			            31:21		reserved	
									20:16		block_numh   水平分窗个数，减1配入。
									15:05         reserved
						            04:00		block_numv    垂直分窗个数，减1配入。
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_block_numh_write(HI_U8 data){
	HI_U32 u32Current = IORD_32DIRECT(0x7018);
	IOWR_32DIRECT(0x7018, (u32Current & 0xffe0ffff) | ((data & 0x1f) << 16));
}

static __inline HI_U8 hi_isp_dehaze_block_numh_read(HI_VOID) {
	return ((IORD_32DIRECT(0x7018)&0x1f0000) >> 16);
}

static __inline HI_VOID hi_isp_dehaze_block_numv_write(HI_U8 data){
	HI_U32 u32Current = IORD_32DIRECT(0x7018);
	IOWR_32DIRECT(0x7018, (u32Current & 0xffffffe0) | (data & 0x1f));
}

static __inline HI_U8 hi_isp_dehaze_block_numv_read(HI_VOID) {
	return (IORD_32DIRECT(0x7018)&0x1f);
}

/*****************************************************
ISP_DEHAZE_DC_SIZE: 		ISP_DEHAZE_BLK_NUM为DEHAZE分块个数配置寄存器。
0x205a701c: 			            31:10		reserved	
									09:05		dc_numh   双线性插值水平方向点的个数，减1配入。
						            04:00		dc_numv   双线性插值垂直方向点的个数，减1配入。
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_dc_numh_write(HI_U16 data){
	HI_U16 u16Current = IORD_16DIRECT(0x701c);
	IOWR_16DIRECT(0x701c, (u16Current & 0xfc1f) | ((data & 0x1f) << 5));
}

static __inline HI_U16 hi_isp_dehaze_dc_numh_read(HI_VOID) {
	return ((IORD_16DIRECT(0x701c)&0x3e0) >> 5);
}

static __inline HI_VOID hi_isp_dehaze_dc_numv_write(HI_U16 data){
	HI_U16 u16Current = IORD_16DIRECT(0x701c);
	IOWR_16DIRECT(0x701c, (u16Current & 0xffe0) | (data & 0x1f));
}

static __inline HI_U16 hi_isp_dehaze_dc_numv_read(HI_VOID) {
	return (IORD_16DIRECT(0x701c)&0x1f);
}

/*****************************************************
ISP_DEHAZE_X: 		ISP_DEHAZE_X为DEHAZE放大图像像素点之间的水平相位差配置寄存器。
0x205a7020: 			31:22		reserved	
						21:00		phase_x   放大图像像素点之间的水平相位差。无符号整数。
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_phase_x_write(HI_U32 data){
	HI_U32 u32Current = IORD_32DIRECT(0x7020);
	IOWR_32DIRECT(0x7020, (u32Current & 0xffc00000) | (data & 0x3fffff));
}

static __inline HI_U32 hi_isp_dehaze_phase_x_read(HI_VOID) {
	return ((IORD_32DIRECT(0x7020)&0x3fffff));
}

/*****************************************************
ISP_DEHAZE_Y: 		ISP_DEHAZE_Y为DEHAZE放大图像像素点之间的水平相位差配置寄存器。
0x205a7024: 			31:22		reserved	
						21:00		phase_y   放大图像像素点之间的水平相位差。无符号整数。
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_phase_y_write(HI_U32 data){
	HI_U32 u32Current = IORD_32DIRECT(0x7024);
	IOWR_32DIRECT(0x7024, (u32Current & 0xffc00000) | (data & 0x3fffff));
}

static __inline HI_U32 hi_isp_dehaze_phase_y_read(HI_VOID) {
	return ((IORD_32DIRECT(0x7024)&0x3fffff));
}

/*****************************************************
ISP_DEHAZE_STAT_MODE: 		ISP_DEHAZE_STATMODE为DEHAZE统计模块控制寄存器。
0x205a7028: 			31:3		reserved									 
						2			max_mode
						1:0			min_mode
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_max_mode_write(HI_U8 data){
	HI_U8 u8Current = IORD_8DIRECT(0x7028);
	IOWR_8DIRECT(0x7028, (u8Current & 0xfb) | (data & 0x1) << 2);
}

static __inline HI_U8 hi_isp_dehaze_max_mode_read(HI_VOID) {
	return ((IORD_8DIRECT(0x7028) & 0x1) >> 2);
}

static __inline HI_VOID hi_isp_dehaze_min_mode_write(HI_U8 data){
	HI_U8 u8Current = IORD_8DIRECT(0x7028);
	IOWR_8DIRECT(0x7028, (u8Current & 0xfc) | (data & 0x3));
}

static __inline HI_U8 hi_isp_dehaze_min_mode_read(HI_VOID) {
	return (IORD_8DIRECT(0x7028) & 0x3);
}

/*****************************************************
ISP_DEHAZE_NEG_MODE: 		ISP_DEHAZE_NEG_MODE为DEHAZE取反处理控制寄存器。
0x205a702c: 			31:1		reserved									 
						0			neg_mode取反处理模式控制。
									0：正常处理模式；
									1：取反处理模式。
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_neg_mode_write(HI_U8 data){
	IOWR_8DIRECT(0x702c, data);
}

static __inline HI_U8 hi_isp_dehaze_neg_mode_read(HI_VOID) {
	return (IORD_8DIRECT(0x702c));
}

/*****************************************************
ISP_DEHAZE_AIR: 		ISP_DEHAZE_NEG_MODE为DEHAZE取反处理控制寄存器。
0x205a7030: 			31:24		reserved	
					    23:16       air_r r通道对应的大气光A。 
						15:08		air_g g通道对应的大气光A。 					      
						07:00		air_b b通道对应的大气光A。 
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_air_r_write(HI_U8 data){
	HI_U32 u32Current = IORD_32DIRECT(0x7030);
	IOWR_32DIRECT(0x7030, (u32Current & 0xff00ffff) | ((data & 0xff) << 16));
}

static __inline HI_U8 hi_isp_dehaze_air_r_read(HI_VOID) {
	return ((IORD_32DIRECT(0x7030)&0x00ff0000) >> 16);
}

static __inline HI_VOID hi_isp_dehaze_air_g_write(HI_U8 data){
	HI_U32 u32Current = IORD_32DIRECT(0x7030);
	IOWR_32DIRECT(0x7030, (u32Current & 0xffff00ff) | ((data & 0xff) << 8));
}

static __inline HI_U8 hi_isp_dehaze_air_g_read(HI_VOID) {
	return ((IORD_32DIRECT(0x7030)&0x0000ff00) >> 8);
}

static __inline HI_VOID hi_isp_dehaze_air_b_write(HI_U8 data){
	HI_U32 u32Current = IORD_32DIRECT(0x7030);
	IOWR_32DIRECT(0x7030, (u32Current & 0xffffff00) | (data & 0xff));
}

static __inline HI_U8 hi_isp_dehaze_air_b_read(HI_VOID) {
	return (IORD_32DIRECT(0x7030)&0x000000ff);
}

/*****************************************************
ISP_DEHAZE_THLD: 	ISP_DEHAZE_THLD为T的阈值系数配置寄存器。
0x205a7034: 			31:08		reserved						      
						07:00		thld t的阈值系数。无符号整数。
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_thld_write(HI_U8 data){
	IOWR_8DIRECT(0x7034, data);
}

static __inline HI_U8 hi_isp_dehaze_thld_read(HI_VOID) {
	return (IORD_8DIRECT(0x7034));
}

/*****************************************************
ISP_DEHAZE_GSTRTH: 	ISP_DEHAZE_GSTRTH为T的计算系数配置寄存器。
0x205a7038: 			31:08		reserved						      
						07:00		gstrth t的阈值系数。无符号整数。
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_gstrth_write(HI_U8 data){
	IOWR_8DIRECT(0x7038, data);
}

static __inline HI_U8 hi_isp_dehaze_gstrth_read(HI_VOID) {
	return (IORD_8DIRECT(0x7038));
}

/*****************************************************
ISP_DEHAZE_BLTHLD: 	ISP_DEHAZE_BLTHLD为最小值滤波阈值系数配置寄存器。
0x205a703c: 			31:08		reserved						      
						07:00		blthld 最小值滤波阈值系数。无符号整数。
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_blthld_write(HI_U8 data){
	IOWR_8DIRECT(0x703c, data);
}

static __inline HI_U8 hi_isp_dehaze_blthld_read(HI_VOID) {
	return (IORD_8DIRECT(0x703c));
}

/*****************************************************
ISP_DEHAZE_MINSTAT_WADDR: 	dehaze最小值统计信息写地址
0x205a7080: 			31:00		minstat_waddr						      
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_minstat_waddr_write(HI_U32 data){
	IOWR_32DIRECT(0x7080, data);
}

static __inline HI_U32 hi_isp_dehaze_minstat_waddr_read(HI_VOID) {
	return (IORD_32DIRECT(0x7080));
}

/*****************************************************
ISP_DEHAZE_MINSTAT_WDATA: 	dehaze最小值统计信息写数据
0x205a7084: 			31:00		minstat_wdata						      
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_minstat_wdata_write(HI_U32 data){
	IOWR_32DIRECT(0x7084, data);
}

static __inline HI_U32 hi_isp_dehaze_minstat_wdata_read(HI_VOID) {
	return (IORD_32DIRECT(0x7084));
}

/*****************************************************
ISP_DEHAZE_MINSTAT_RADDR: 	dehaze最小值统计信息读地址寄存器
0x205a7088: 			31:00		minstat_raddr						      
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_minstat_raddr_write(HI_U32 data){
	IOWR_32DIRECT(0x7088, data);
}

static __inline HI_U32 hi_isp_dehaze_minstat_raddr_read(HI_VOID) {
	return (IORD_32DIRECT(0x7088));
}

/*****************************************************
ISP_DEHAZE_MINSTAT_RDATA: 	dehaze最小值统计信息读数据寄存器
0x205a708c: 			31:00		minstat_rdata						      
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_minstat_rdata_write(HI_U32 data){
	IOWR_32DIRECT(0x708c, data);
}

static __inline HI_U32 hi_isp_dehaze_minstat_rdata_read(HI_VOID) {
	return (IORD_32DIRECT(0x708c));
}

/*****************************************************
ISP_DEHAZE_MAXSTAT_WADDR: 	ISP_DEHAZE_MAXSTAT_WADDR为DEHAZE最大值统计信息写地址寄存器
0x205a7090: 			31:00		maxstat_waddr						      
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_maxstat_waddr_write(HI_U32 data){
	IOWR_32DIRECT(0x7090, data);
}

static __inline HI_U32 hi_isp_dehaze_maxstat_waddr_read(HI_VOID) {
	return (IORD_32DIRECT(0x7090));
}

/*****************************************************
ISP_DEHAZE_MAXSTAT_WDATA: 	dehaze最大值统计信息写数据寄存器
0x205a7094: 			31:00		maxstat_wdata						      
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_maxstat_wdata_write(HI_U32 data){
	IOWR_32DIRECT(0x7094, data);
}

static __inline HI_U32 hi_isp_dehaze_maxstat_wdata_read(HI_VOID) {
	return (IORD_32DIRECT(0x7094));
}

/*****************************************************
ISP_DEHAZE_MAXSTAT_RADDR: 	dehaze最大值统计信息读地址寄存器
0x205a7098: 			31:00		maxstat_raddr						      
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_maxstat_raddr_write(HI_U32 data){
	IOWR_32DIRECT(0x7098, data);
}

static __inline HI_U32 hi_isp_dehaze_maxstat_raddr_read(HI_VOID) {
	return (IORD_32DIRECT(0x7098));
}

/*****************************************************
ISP_DEHAZE_MAXSTAT_RDATA: 	dehaze最大值统计信息读地址寄存器
0x205a709c: 			31:00		maxstat_rdata						      
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_maxstat_rdata_write(HI_U32 data){
	IOWR_32DIRECT(0x709c, data);
}

static __inline HI_U32 hi_isp_dehaze_maxstat_rdata_read(HI_VOID) {
	return (IORD_32DIRECT(0x709c));
}

/*****************************************************
ISP_DEHAZE_PRESTAT_WADDR: 	dehaze前一帧最小值统计信息写地址寄存器
0x205a70a0: 			31:00		prestat_waddr						      
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_prestat_waddr_write(HI_U32 data){
	IOWR_32DIRECT(0x70a0, data);
}

static __inline HI_U32 hi_isp_dehaze_prestat_waddr_read(HI_VOID) {
	return (IORD_32DIRECT(0x70a0));
}

/*****************************************************
ISP_DEHAZE_PRESTAT_WDATA: 	dehaze前一帧最小值统计信息写数据寄存器
0x205a70a4: 			31:00		prestat_wdata						      
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_prestat_wdata_write(HI_U32 data){
	IOWR_32DIRECT(0x70a4, data);
}

static __inline HI_U32 hi_isp_dehaze_prestat_wdata_read(HI_VOID) {
	return (IORD_32DIRECT(0x70a4));
}

/*****************************************************
ISP_DEHAZE_PRESTAT_RADDR: 	dehaze前一帧最小值统计信息读地址寄存器
0x205a70a8: 			31:00		prestat_raddr						      
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_prestat_raddr_write(HI_U32 data){
	IOWR_32DIRECT(0x70a8, data);
}

static __inline HI_U32 hi_isp_dehaze_prestat_raddr_read(HI_VOID) {
	return (IORD_32DIRECT(0x70a8));
}

/*****************************************************
ISP_DEHAZE_PRESTAT_RADDR: 	dehaze前一帧最小值统计信息读数据寄存器
0x205a70ac: 			31:00		prestat_rdata						      
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_prestat_rdata_write(HI_U32 data){
	IOWR_32DIRECT(0x70ac, data);
}

static __inline HI_U32 hi_isp_dehaze_prestat_rdata_read(HI_VOID) {
	return (IORD_32DIRECT(0x70ac));
}

/*****************************************************
ISP_DEHAZE_LUT_WADDR: 	dehaze强度LUT信息写地址寄存器
0x205a70b0: 			31:00		lut_waddr						      
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_lut_waddr_write(HI_U32 data){
	IOWR_32DIRECT(0x70b0, data);
}

static __inline HI_U32 hi_isp_dehaze_lut_waddr_read(HI_VOID) {
	return (IORD_32DIRECT(0x70b0));
}

/*****************************************************
ISP_DEHAZE_LUT_WDATA: 	dehaze强度LUT信息写数据寄存器
0x205a70b4: 			31:00		lut_wdata						      
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_lut_wdata_write(HI_U32 data){
	IOWR_32DIRECT(0x70b4, data);
}

static __inline HI_U32 hi_isp_dehaze_lut_wdata_read(HI_VOID) {
	return (IORD_32DIRECT(0x70b4));
}

/*****************************************************
ISP_DEHAZE_LUT_RADDR: 	dehaze强度LUT信息读地址寄存器
0x205a70b8: 			31:00		lut_raddr						      
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_lut_raddr_write(HI_U32 data){
	IOWR_32DIRECT(0x70b8, data);
}

static __inline HI_U32 hi_isp_dehaze_lut_raddr_read(HI_VOID) {
	return (IORD_32DIRECT(0x70b8));
}

/*****************************************************
ISP_DEHAZE_LUT_RDATA: 	dehaze强度LUT信息读数据寄存器
0x205a70bc: 			31:00		lut_rdata						      
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_lut_rdata_write(HI_U32 data){
	IOWR_32DIRECT(0x70bc, data);
}

static __inline HI_U32 hi_isp_dehaze_lut_rdata_read(HI_VOID) {
	return (IORD_32DIRECT(0x70bc));
}

/*****************************************************
ISP_DEHAZE_CTRL_I: 		ISP_DEHAZE_NEG_MODE为DEHAZE取反处理控制寄存器。
0x205a70e4: 			31:1		reserved									 
						0			update_mode
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_update_mode_write(HI_U8 data){
	IOWR_8DIRECT(0x70e4, data);
}

static __inline HI_U8 hi_isp_dehaze_update_mode_read(HI_VOID) {
	return (IORD_8DIRECT(0x70e4));
}

/*****************************************************
ISP_DEHAZE_UPDATE: 		ISP_DEHAZE_UPDATE为寄存器更新寄存器
0x205a70ec: 			31:1		reserved									 
						0			newer
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_newer_write(HI_U8 data){
	IOWR_8DIRECT(0x70ec, data);
}

static __inline HI_U8 hi_isp_dehaze_newer_read(HI_VOID) {
	return (IORD_8DIRECT(0x70ec));
}

/*****************************************************
ISP_DEHAZE_SIZE: 		ISP_DEHAZE_SIZE为DEHAZE图像宽高。
0x205a70f0: 			31:29		reserved									 
						28:16		height
						15:13		reserved
						12:00		width
*****************************************************/
static __inline HI_VOID hi_isp_dehaze_height_write(HI_U16 data){
	HI_U32 u32Current = IORD_32DIRECT(0x70f0);
	IOWR_32DIRECT(0x70f0, (u32Current & 0xe000ffff) | ((data & 0x1fff) << 16));
}

static __inline HI_U16 hi_isp_dehaze_height_read(HI_VOID) {
	return ((IORD_32DIRECT(0x70f0)&0x1fff0000) >> 16);
}

static __inline HI_VOID hi_isp_dehaze_width_write(HI_U16 data){
	IOWR_16DIRECT(0x70ec, data&0x1fff);
}

static __inline HI_U16 hi_isp_dehaze_width_read(HI_VOID) {
	return (IORD_16DIRECT(0x70ec)&0x1fff);
}

/*****************************************************
MODULE        -----AF-----
MODULE        -----AF-----
MODULE        -----AF-----
*****************************************************/
/*****************************************************
ISP_AF_CFG:      AF控制寄存器
0x205a2200:      31		   	ck_gt_en		|	9			data_mode
	                 30:16   	reserved			|	8			squ_mode
	                 15			bayer_mode	|	7			peak_mode
	                 14			raw_mode	|	6			iir1_en2
	                 13			gamma_en	|	5			iir1_en1
	                 12			median_en	|	4			iir1_en0
	                 11			lpf_en		|	3			iir0_en2
	                 10			crop_en		|	2			iir0_en1
                                					|	1			iir0_en0
                                					|	0			af_en                                                          
*****************************************************/
//------------------------------------------------------------------------------ 
//  History        :
//  1.Date         : 2015/12/8
//    Author       : c00298856
//    Modification : add reg [ISP_MODULE_POS 0x205a0088] to select AF position 
//------------------------------------------------------------------------------

static __inline HI_VOID hi_vi_af_pos_write(HI_U8 data) {
	HI_U32 u32current = IORD_32DIRECT_VI(0x20088);
    IOWR_32DIRECT_VI(0x20088, (u32current & (~(0x1 << 3))) | ((data&0x1) << 3));
}

static __inline HI_U8 hi_vi_af_pos_read(HI_VOID) {
	return ((IORD_32DIRECT_VI(0x20088) & (0x1 << 3)) >> 3);
}

static __inline HI_VOID hi_isp_af_enable_write(HI_U8 data) {
	HI_U32 u32current = IORD_32DIRECT(0x2200);
    IOWR_32DIRECT(0x2200, (u32current & (~(1))) | (data&0x1));
}

static __inline HI_U8 hi_isp_af_enable_read(HI_VOID) {
	return (IORD_32DIRECT(0x2200) & 0x1);
}

static __inline HI_VOID hi_isp_af_iir0_en0_write(HI_U8 data) {
	HI_U32 u32current = IORD_32DIRECT(0x2200);
    IOWR_32DIRECT(0x2200, (u32current & (~(1 << 1))) | ((data&0x1) << 1));
}

static __inline HI_U8 hi_isp_af_iir0_en0_read(HI_VOID) {
	return ((IORD_32DIRECT(0x2200) & (0x1 << 1)) >> 1);
}

static __inline HI_VOID hi_isp_af_iir0_en1_write(HI_U8 data) {
	HI_U32 u32current = IORD_32DIRECT(0x2200);
    IOWR_32DIRECT(0x2200, (u32current & (~(1 << 2))) | ((data&0x1) << 2));
}

static __inline HI_U8 hi_isp_af_iir0_en1_read(HI_VOID) {
	return ((IORD_32DIRECT(0x2200) & (0x1 << 2)) >> 2);
}

static __inline HI_VOID hi_isp_af_iir0_en2_write(HI_U8 data) {
	HI_U32 u32current = IORD_32DIRECT(0x2200);
    IOWR_32DIRECT(0x2200, (u32current & (~(1 << 3))) | ((data&0x1) << 3));
}

static __inline HI_U8 hi_isp_af_iir0_en2_read(HI_VOID) {
	return ((IORD_32DIRECT(0x2200) & (0x1 << 3)) >> 3);
}

static __inline HI_VOID hi_isp_af_iir1_en0_write(HI_U8 data) {
	HI_U32 u32current = IORD_32DIRECT(0x2200);
    IOWR_32DIRECT(0x2200, (u32current & (~(1 << 4))) | ((data&0x1) << 4));
}

static __inline HI_U8 hi_isp_af_iir1_en0_read(HI_VOID) {
	return ((IORD_32DIRECT(0x2200) & (0x1 << 4)) >> 4);
}

static __inline HI_VOID hi_isp_af_iir1_en1_write(HI_U8 data) {
	HI_U32 u32current = IORD_32DIRECT(0x2200);
    IOWR_32DIRECT(0x2200, (u32current & (~(1 << 5))) | ((data&0x1) << 5));
}

static __inline HI_U8 hi_isp_af_iir1_en1_read(HI_VOID) {
	return ((IORD_32DIRECT(0x2200) & (0x1 << 5)) >> 5);
}

static __inline HI_VOID hi_isp_af_iir1_en2_write(HI_U8 data) {
	HI_U32 u32current = IORD_32DIRECT(0x2200);
    IOWR_32DIRECT(0x2200, (u32current & (~(1 << 6))) | ((data&0x1) << 6));
}

static __inline HI_U8 hi_isp_af_iir1_en2_read(HI_VOID) {
	return ((IORD_32DIRECT(0x2200) & (0x1 << 6)) >> 6);
}

static __inline HI_VOID hi_isp_af_peak_mode_write(HI_U8 data) {
	HI_U32 u32current = IORD_32DIRECT(0x2200);
    IOWR_32DIRECT(0x2200, (u32current & (~(1 << 7))) | ((data&0x1) << 7));
}

static __inline HI_U8 hi_isp_af_peak_mode_read(HI_VOID) {
	return ((IORD_32DIRECT(0x2200) & (0x1 << 7)) >> 7);
}

static __inline HI_VOID hi_isp_af_squ_mode_write(HI_U8 data) {
	HI_U32 u32current = IORD_32DIRECT(0x2200);
    IOWR_32DIRECT(0x2200, (u32current & (~(1 << 8))) | ((data&0x1) << 8));
}

static __inline HI_U8 hi_isp_af_squ_mode_read(HI_VOID) {
	return ((IORD_32DIRECT(0x2200) & (0x1 << 8)) >> 8);
}

static __inline HI_VOID hi_isp_af_data_mode_write(HI_U8 data) {
	HI_U32 u32current = IORD_32DIRECT(0x2200);
    IOWR_32DIRECT(0x2200, (u32current & (~(1 << 9))) | ((data&0x1) << 9));
}

static __inline HI_U8 hi_isp_af_data_mode_read(HI_VOID) {
	return ((IORD_32DIRECT(0x2200) & (0x1 << 9)) >> 9);
}

static __inline HI_VOID hi_isp_af_crop_en_write(HI_U8 data) {
	HI_U32 u32current = IORD_32DIRECT(0x2200);
    IOWR_32DIRECT(0x2200, (u32current & (~(1 << 10))) | ((data&0x1) << 10));
}

static __inline HI_U8 hi_isp_af_crop_en_read(HI_VOID) {
	return ((IORD_32DIRECT(0x2200) & (0x1 << 10)) >> 10);
}

static __inline HI_VOID hi_isp_af_lpf_en_write(HI_U8 data) {
	HI_U32 u32current = IORD_32DIRECT(0x2200);
    IOWR_32DIRECT(0x2200, (u32current & (~(1 << 11))) | ((data&0x1) << 11));
}

static __inline HI_U8 hi_isp_af_lpf_en_read(HI_VOID) {
	return ((IORD_32DIRECT(0x2200) & (0x1 << 11)) >> 11);
}

static __inline HI_VOID hi_isp_af_median_en_write(HI_U8 data) {
	HI_U32 u32current = IORD_32DIRECT(0x2200);
    IOWR_32DIRECT(0x2200, (u32current & (~(1 << 12))) | ((data&0x1) << 12));
}

static __inline HI_U8 hi_isp_af_median_en_read(HI_VOID) {
	return ((IORD_32DIRECT(0x2200) & (0x1 << 12)) >> 12);
}

static __inline HI_VOID hi_isp_af_gamma_en_write(HI_U8 data) {
	HI_U32 u32current = IORD_32DIRECT(0x2200);
    IOWR_32DIRECT(0x2200, (u32current & (~(1 << 13))) | ((data&0x1) << 13));
}

static __inline HI_U8 hi_isp_af_gamma_en_read(HI_VOID) {
	return ((IORD_32DIRECT(0x2200) & (0x1 << 13)) >> 13);
}

static __inline HI_VOID hi_isp_af_raw_mode_write(HI_U8 data) {
	HI_U32 u32current = IORD_32DIRECT(0x2200);
    IOWR_32DIRECT(0x2200, (u32current & (~(1 << 14))) | ((data&0x1) << 14));
}

static __inline HI_U8 hi_isp_af_raw_mode_read(HI_VOID) {
	return ((IORD_32DIRECT(0x2200) & (0x1 << 14)) >> 14);
}

static __inline HI_VOID hi_isp_af_bayer_mode_write(HI_U8 data) {
	HI_U32 u32current = IORD_32DIRECT(0x2200);
    IOWR_32DIRECT(0x2200, (u32current & (~(1 << 15))) | ((data&0x1) << 15));
}

static __inline HI_U8 hi_isp_af_bayer_mode_read(HI_VOID) {
	return ((IORD_32DIRECT(0x2200) & (0x1 << 15)) >> 15);
}

static __inline HI_VOID hi_isp_af_ck_gt_en_write(HI_U8 data) {
	HI_U32 u32current = IORD_32DIRECT(0x2200);
    IOWR_32DIRECT(0x2200, (u32current & (~(1 << 31))) | ((data&0x1) << 31));
}

static __inline HI_U8 hi_isp_af_ck_gt_en_read(HI_VOID) {
	return ((IORD_32DIRECT(0x2200) & (0x1 << 31)) >> 31);
}

/*****************************************************
ISP_AF_ZONE: 			AF区块分配寄存器。
0x205a2210: 				31:13	reserved									 
						12:8			vnum
						7:5		reserved
						4:0			hnum
*****************************************************/
static __inline HI_VOID hi_isp_af_vnum_write(HI_U8 data){
	HI_U16 u16Current = IORD_16DIRECT(0x2210);
	IOWR_16DIRECT(0x2210, (u16Current & 0xe0ff) | ((data & 0x1f) << 8));
}

static __inline HI_U8 hi_isp_af_vnum_read(HI_VOID) {
	return ((IORD_16DIRECT(0x2210)&0x1f00) >> 8);
}

static __inline HI_VOID hi_isp_af_hnum_write(HI_U8 data){
	HI_U16 u16Current = IORD_16DIRECT(0x2210);
	IOWR_16DIRECT(0x2210, (u16Current & 0xffe0) | (data & 0x1f));
}

static __inline HI_U8 hi_isp_af_hnum_read(HI_VOID) {
	return (IORD_16DIRECT(0x2210)&0x1f);
}

/*****************************************************
ISP_AF_CROP_START: 			AF图像CROP起始坐标寄存器
0x205a2214: 				31:28	reserved									 
						27:16		pos_y
						15:12	reserved
						11:0			pos_x
*****************************************************/
static __inline HI_VOID hi_isp_af_pos_y_write(HI_U16 data){
	HI_U32 u32Current = IORD_32DIRECT(0x2214);
	IOWR_32DIRECT(0x2214, (u32Current & 0xf000ffff) | ((data & 0xfff) << 16));
}

static __inline HI_U16 hi_isp_af_pos_y_read(HI_VOID) {
	return ((IORD_32DIRECT(0x2214)&0xfff0000) >> 16);
}

static __inline HI_VOID hi_isp_af_pos_x_write(HI_U16 data){
	HI_U32 u32Current = IORD_32DIRECT(0x2214);
	IOWR_32DIRECT(0x2214, (u32Current & 0xfffff000) | (data & 0xfff));
}

static __inline HI_U16 hi_isp_af_pos_x_read(HI_VOID) {
	return (IORD_32DIRECT(0x2214)&0x0fff);
}

/*****************************************************
ISP_AF_CROP_SIZE: 			AF图像CROP SIZE寄存器
0x205a2218: 				31:28	reserved									 
						27:16		vsize
						15:12	reserved
						11:0			hsize
*****************************************************/
static __inline HI_VOID hi_isp_af_crop_vsize_write(HI_U16 data){
	HI_U32 u32Current = IORD_32DIRECT(0x2218);
	IOWR_32DIRECT(0x2218, (u32Current & 0xf000ffff) | ((data & 0xfff) << 16));
}

static __inline HI_U16 hi_isp_af_crop_vsize_read(HI_VOID) {
	return ((IORD_32DIRECT(0x2218)&0xfff0000) >> 16);
}

static __inline HI_VOID hi_isp_af_crop_hsize_write(HI_U16 data){
	HI_U32 u32Current = IORD_32DIRECT(0x2218);
	IOWR_32DIRECT(0x2218, (u32Current & 0xfffff000) | (data & 0xfff));
}

static __inline HI_U16 hi_isp_af_crop_hsize_read(HI_VOID) {
	return (IORD_32DIRECT(0x2218)&0x0fff);
}	

/*****************************************************
ISP_AF_MEAN_THRES: 			AF 中值滤波阈值寄存器
0x205a221c: 				31:16	reserved									 
						15:0			mean_thres
*****************************************************/
static __inline HI_VOID hi_isp_af_median_thres_write(HI_U16 data){
	HI_U16 u16Current = IORD_16DIRECT(0x221c);
	IOWR_16DIRECT(0x221c, (u16Current & 0x0) | (data & 0xffff));
}

static __inline HI_U16 hi_isp_af_median_thres_read(HI_VOID) {
	return (IORD_16DIRECT(0x221c)&0xffff);
}	

/*****************************************************
ISP_AF_IIRG: 			AF IIR滤波参数(0)寄存器
0x205a2220: 				31:24	reserved									 
						23:16		iirgx_1
						15:8		reserved
						7:0			iirgx_0
					AF IIR滤波参数(1-6)寄存器
0x205a2224: 				31:26	reserved									 
0x205a2238:				25:16		iirgx_1
						15:8		reserved
						9:0			iirgx_0
*****************************************************/
static __inline HI_VOID hi_isp_af_iir1_g0_write(HI_U8 data){
	HI_U32 u32Current = IORD_32DIRECT(0x2220);
	IOWR_32DIRECT(0x2220, (u32Current & 0xff00ffff) | ((data & 0xff) << 16));
}

static __inline HI_U8 hi_isp_af_iir1_g0_read(HI_VOID) {
	return ((IORD_32DIRECT(0x2220) & 0xff0000) >> 16);
}

static __inline HI_VOID hi_isp_af_iir0_g0_write(HI_U8 data){
	HI_U32 u32Current = IORD_32DIRECT(0x2220);
	IOWR_32DIRECT(0x2220, (u32Current & 0xffffff00) | (data & 0xff));
}

static __inline HI_U8 hi_isp_af_iir0_g0_read(HI_VOID) {
	return (IORD_32DIRECT(0x2220) & 0xff);
}

static __inline HI_VOID hi_isp_af_iir1_g_write(HI_U8 index, HI_U16 data){
	HI_U32 u32Current = IORD_32DIRECT(0x2220 + (index << 2));
	IOWR_32DIRECT(0x2220 + (index << 2), (u32Current & 0xfc00ffff) | ((data & 0x3ff) << 16));
}

static __inline HI_U16 hi_isp_af_iir1_g_read(HI_U8 index) {
	return ((IORD_32DIRECT(0x2220 + (index << 2)) & 0x3ff0000) >> 16);
}

static __inline HI_VOID hi_isp_af_iir0_g_write(HI_U8 index, HI_U16 data){
	HI_U32 u32Current = IORD_32DIRECT(0x2220 + (index << 2));
	IOWR_32DIRECT(0x2220 + (index << 2), (u32Current & 0xfffffc00) | (data & 0x3ff));
}

static __inline HI_U16 hi_isp_af_iir0_g_read(HI_U8 index) {
	return (IORD_32DIRECT(0x2220 + (index << 2)) & 0x3ff);
}

/*****************************************************
ISP_AF_SHIFT: 			AF IIR滤波移位参数寄存器
0x205a2240: 				31:28		iirshift1_3		|	15:12		iirshift0_3								 
						27:24		iirshift1_2		|	11:8			iirshift0_2
						23:20		iirshift1_1		|	7:4			iirshift0_1
						19:16		iirshift1_0		|	3:0			iirshift0_0
*****************************************************/
static __inline HI_VOID hi_isp_af_iir1_shift_write(HI_U8 index, HI_U8 data){
	HI_U32 u32Current = IORD_32DIRECT(0x2240);
	IOWR_32DIRECT(0x2240, (u32Current & (~(0xf << (16 + (index<<2))))) | ((data & 0xf) << (16 + (index<<2))));
}

static __inline HI_U8 hi_isp_af_iir1_shift_read(HI_U8 index) {
	return ((IORD_32DIRECT(0x2240) & (0xf << (16 + (index<<2)))) >> (16 + (index<<2)));
}

static __inline HI_VOID hi_isp_af_iir0_shift_write(HI_U8 index, HI_U8 data){
	HI_U32 u32Current = IORD_32DIRECT(0x2240);
	IOWR_32DIRECT(0x2240, (u32Current & (~(0xf << (index<<2)))) | ((data & 0xf) << (index<<2)));
}

static __inline HI_U8 hi_isp_af_iir0_shift_read(HI_U8 index) {
	return ((IORD_32DIRECT(0x2240) & (0xf << (index<<2))) >> (index<<2));
}

/*****************************************************
ISP_AF_FIRH: 			AF FIR滤波参数(0-4)寄存器
0x205a2250: 				31:22	reserved									 
0x205a2260:				21:16		firh_1
						15:6		reserved
						5:0			firh_0
*****************************************************/
static __inline HI_VOID hi_isp_af_fir1_h_write(HI_U8 index, HI_U8 data){
	HI_U32 u32Current = IORD_32DIRECT(0x2250 + (index << 2));
	IOWR_32DIRECT(0x2250 + (index << 2), (u32Current & 0xffc0ffff) | ((data & 0x3f) << 16));
}

static __inline HI_U8 hi_isp_af_fir1_h_read(HI_U8 index) {
	return ((IORD_32DIRECT(0x2250 + (index << 2)) & 0x3f0000) >> 16);
}

static __inline HI_VOID hi_isp_af_fir0_h_write(HI_U8 index, HI_U8 data){
	HI_U32 u32Current = IORD_32DIRECT(0x2250 + (index << 2));
	IOWR_32DIRECT(0x2250 + (index << 2), (u32Current & 0xffffffc0) | (data & 0x3f));
}

static __inline HI_U8 hi_isp_af_fir0_h_read(HI_U8 index) {
	return (IORD_32DIRECT(0x2250 + (index << 2)) & 0x3f);
}

/*****************************************************
ISP_AF_IIRTHRE: 			AF IIR滤波阈值寄存器
0x205a2270: 				31:27	reserved									 
						26:16		thre2_x
						15:11	reserved
						10:0			thre1_x
*****************************************************/
static __inline HI_VOID hi_isp_af_iir1_thre_x_write(HI_U16 data){
	HI_U32 u32Current = IORD_32DIRECT(0x2270);
	IOWR_32DIRECT(0x2270, (u32Current & 0xf800ffff) | ((data & 0x7ff) << 16));
}

static __inline HI_U16 hi_isp_af_iir1_thre_x_read(HI_VOID) {
	return ((IORD_32DIRECT(0x2270)&0x7ff0000) >> 16);
}

static __inline HI_VOID hi_isp_af_iir0_thre_x_write(HI_U16 data){
	HI_U32 u32Current = IORD_32DIRECT(0x2270);
	IOWR_32DIRECT(0x2270, (u32Current & 0xfffff800) | (data & 0x7ff));
}

static __inline HI_U16 hi_isp_af_iir0_thre_x_read(HI_VOID) {
	return (IORD_32DIRECT(0x2270)&0x7ff);
}

/*****************************************************
ISP_AF_FIRTHRE: 			AF FIR滤波阈值寄存器
0x205a2274: 				31:27	reserved									 
						26:16		thre2_y
						15:11	reserved
						10:0			thre1_y
*****************************************************/
static __inline HI_VOID hi_isp_af_fir1_thre_y_write(HI_U16 data){
	HI_U32 u32Current = IORD_32DIRECT(0x2274);
	IOWR_32DIRECT(0x2274, (u32Current & 0xf800ffff) | ((data & 0x7ff) << 16));
}

static __inline HI_U16 hi_isp_af_fir1_thre_y_read(HI_VOID) {
	return ((IORD_32DIRECT(0x2274)&0x7ff0000) >> 16);
}

static __inline HI_VOID hi_isp_af_fir0_thre_y_write(HI_U16 data){
	HI_U32 u32Current = IORD_32DIRECT(0x2274);
	IOWR_32DIRECT(0x2274, (u32Current & 0xfffff800) | (data & 0x7ff));
}

static __inline HI_U16 hi_isp_af_fir0_thre_y_read(HI_VOID) {
	return (IORD_32DIRECT(0x2274)&0x7ff);
}

/*****************************************************
ISP_AF_ACC_SHIFT: 			AF累加统计值移位寄存器
0x205a2278: 				31:20	reserved									 
						19:16		acc_shift_y
						15:12		acc_shift2_v
						11:8			acc_shift1_v
						7:4			acc_shift2_h
						3:0			acc_shift1_h
*****************************************************/
static __inline HI_VOID hi_isp_af_acc_shift_y_write(HI_U8 data){
	HI_U32 u32Current = IORD_32DIRECT(0x2278);
	IOWR_32DIRECT(0x2278, (u32Current & 0xfff0ffff) | ((data & 0xf) << 16));
}

static __inline HI_U8 hi_isp_af_acc_shift_y_read(HI_VOID) {
	return ((IORD_32DIRECT(0x2278)&0xf0000) >> 16);
}

static __inline HI_VOID hi_isp_af_acc_shift_v1_write(HI_U8 data){
	HI_U32 u32Current = IORD_32DIRECT(0x2278);
	IOWR_32DIRECT(0x2278, (u32Current & 0xffff0fff) | ((data & 0xf) << 12));
}

static __inline HI_U8 hi_isp_af_acc_shift_v1_read(HI_VOID) {
	return ((IORD_32DIRECT(0x2278)&0xf000) >> 12);
}

static __inline HI_VOID hi_isp_af_acc_shift_v0_write(HI_U8 data){
	HI_U32 u32Current = IORD_32DIRECT(0x2278);
	IOWR_32DIRECT(0x2278, (u32Current & 0xfffff0ff) | ((data & 0xf) << 8));
}

static __inline HI_U8 hi_isp_af_acc_shift_v0_read(HI_VOID) {
	return ((IORD_32DIRECT(0x2278)&0xf00) >> 8);
}

static __inline HI_VOID hi_isp_af_acc_shift_h1_write(HI_U8 data){
	HI_U32 u32Current = IORD_32DIRECT(0x2278);
	IOWR_32DIRECT(0x2278, (u32Current & 0xffffff0f) | ((data & 0xf) << 4));
}

static __inline HI_U8 hi_isp_af_acc_shift_h1_read(HI_VOID) {
	return ((IORD_32DIRECT(0x2278)&0xf0) >> 4);
}

static __inline HI_VOID hi_isp_af_acc_shift_h0_write(HI_U8 data){
	HI_U32 u32Current = IORD_32DIRECT(0x2278);
	IOWR_32DIRECT(0x2278, (u32Current & 0xfffffff0) | (data & 0xf));
}

static __inline HI_U8 hi_isp_af_acc_shift_h0_read(HI_VOID) {
	return (IORD_32DIRECT(0x2278)&0xf);
}

/*****************************************************
ISP_AF_CNT_SHIFT: 			AF计数统计值移位寄存器
0x205a227c: 				31:16	reserved									 
						15:12		cnt_shift2_v
						11:8			cnt_shift1_v
						7:4			cnt_shift2_h
						3:0			cnt_shift1_h
*****************************************************/
static __inline HI_VOID hi_isp_af_cnt_shift_v1_write(HI_U8 data){
	HI_U32 u32Current = IORD_32DIRECT(0x227c);
	IOWR_32DIRECT(0x227c, (u32Current & 0xffff0fff) | ((data & 0xf) << 12));
}

static __inline HI_U8 hi_isp_af_cnt_shift_v1_read(HI_VOID) {
	return ((IORD_32DIRECT(0x227c)&0xf000) >> 12);
}

static __inline HI_VOID hi_isp_af_cnt_shift_v0_write(HI_U8 data){
	HI_U32 u32Current = IORD_32DIRECT(0x227c);
	IOWR_32DIRECT(0x227c, (u32Current & 0xfffff0ff) | ((data & 0xf) << 8));
}

static __inline HI_U8 hi_isp_af_cnt_shift_v0_read(HI_VOID) {
	return ((IORD_32DIRECT(0x227c)&0xf00) >> 8);
}

static __inline HI_VOID hi_isp_af_cnt_shift_h1_write(HI_U8 data){
	HI_U32 u32Current = IORD_32DIRECT(0x227c);
	IOWR_32DIRECT(0x227c, (u32Current & 0xffffff0f) | ((data & 0xf) << 4));
}

static __inline HI_U8 hi_isp_af_cnt_shift_h1_read(HI_VOID) {
	return ((IORD_32DIRECT(0x227c)&0xf0) >> 4);
}

static __inline HI_VOID hi_isp_af_cnt_shift_h0_write(HI_U8 data){
	HI_U32 u32Current = IORD_32DIRECT(0x227c);
	IOWR_32DIRECT(0x227c, (u32Current & 0xfffffff0) | (data & 0xf));
}

static __inline HI_U8 hi_isp_af_cnt_shift_h0_read(HI_VOID) {
	return (IORD_32DIRECT(0x227c)&0xf0);
}


/*****************************************************
ISP_AF_STAT_IND_RADDR: 	AF各区块统计信息间接读地址寄存器
0x205a2288: 			31:00		af_stat_ind_raddr					      
*****************************************************/
static __inline HI_VOID hi_isp_af_stat_ind_raddr_write(HI_U32 data){
	IOWR_32DIRECT(0x2288, data);
}

static __inline HI_U32 hi_isp_af_stat_ind_raddr_read(HI_VOID) {
	return (IORD_32DIRECT(0x2288));
}

/*****************************************************
ISP_AF_STAT_IND_RDATA: 	AF各区块统计信息间接读数据寄存器
0x205a228c: 			31:00		af_stat_ind_raddr					      
*****************************************************/
static __inline HI_U32 hi_isp_af_stat_ind_rdata_read(HI_VOID) {
	return (IORD_32DIRECT(0x228c));
}

/*****************************************************
ISP_AF_SIZE: 			AF图像大小寄存器
0x205a22f0: 				31:28	reserved									 
						27:16		vsize
						15:12	reserved
						11:00		hsize
*****************************************************/
static __inline HI_VOID hi_isp_af_vsize_write(HI_U16 data){
	HI_U32 u32Current = IORD_32DIRECT(0x22f0);
	IOWR_32DIRECT(0x22f0, (u32Current & 0xf000ffff) | ((data & 0xfff) << 16));
}

static __inline HI_U16 hi_isp_af_vsize_read(HI_VOID) {
	return ((IORD_32DIRECT(0x22f0)&0xfff0000) >> 16);
}

static __inline HI_VOID hi_isp_af_hsize_write(HI_U16 data){
	IOWR_16DIRECT(0x22f0, data&0xfff);
}

static __inline HI_U16 hi_isp_af_hsize_read(HI_VOID) {
	return (IORD_16DIRECT(0x22f0)&0xfff);
}

/*****************************************************
MODULE        -----ISP_CROP_CFG-----
MODULE        -----ISP_CROP_CFG-----
MODULE        -----ISP_CROP_CFG-----
*****************************************************/
/*****************************************************
ISP_CROP_CFG:			ISP_CROP_CFG为CROP使能寄存器。
0x205b1100: 				31:01          reserved
                                          00:00          en
*****************************************************/
static __inline HI_VOID hi_isp_crop_en_write(HI_U8 data){
	IOWR_32DIRECT_ISP_REG(0x11100, data & 0x1);
}

static __inline HI_U8 hi_isp_crop_en_read(HI_VOID) {
	return (IORD_32DIRECT_ISP_REG(0x11100)&0x1);
}

/*****************************************************
ISP_CROP0_START:			ISP_CROP_CFG为CROP使能寄存器。
0x205b1110: 				31:29          reserved
						28:16          y_start
						15:13          reserved
                                          12:00          x_start
*****************************************************/
static __inline HI_VOID hi_isp_crop_x_start_write(HI_U16 data){
	HI_U32 u32current = IORD_32DIRECT_ISP_REG(0x11110);
	IOWR_32DIRECT_ISP_REG(0x11110, (data & 0x1fff) | (u32current & 0xffffe000));
}

static __inline HI_U16 hi_isp_crop_x_start_read(HI_VOID) {
	return (IORD_32DIRECT_ISP_REG(0x11110)&0x1fff);
}

static __inline HI_VOID hi_isp_crop_y_start_write(HI_U16 data){
	HI_U32 u32current = IORD_32DIRECT_ISP_REG(0x11110);
	IOWR_32DIRECT_ISP_REG(0x11110, ((data & 0x1fff) << 16) | (u32current & 0x1fff));
}

static __inline HI_U16 hi_isp_crop_y_start_read(HI_VOID) {
	return ((IORD_32DIRECT_ISP_REG(0x11110)&0x1fff0000) >> 16);
}

/*****************************************************
ISP_CROP0_SIZE:			ISP_CROP_CFG为CROP使能寄存器。
0x205b1114: 				31:29          reserved
						28:16          height
						15:13          reserved
                                          12:00          width
*****************************************************/
static __inline HI_VOID hi_isp_crop_width_write(HI_U16 data){
	HI_U32 u32current = IORD_32DIRECT_ISP_REG(0x11114);
	IOWR_32DIRECT_ISP_REG(0x11114, (data & 0x1fff) | (u32current & 0xffffe000));
}

static __inline HI_U16 hi_isp_crop_width_read(HI_VOID) {
	return (IORD_32DIRECT_ISP_REG(0x11114)&0x1fff);
}

static __inline HI_VOID hi_isp_crop_height_write(HI_U16 data){
	HI_U32 u32current = IORD_32DIRECT_ISP_REG(0x11114);
	IOWR_32DIRECT_ISP_REG(0x11114, ((data & 0x1fff) << 16) | (u32current & 0x1fff));
}

static __inline HI_U16 hi_isp_crop_height_read(HI_VOID) {
	return ((IORD_32DIRECT_ISP_REG(0x11114)&0x1fff0000) >> 16);
}

/*****************************************************
MODULE        -----WCH_Y_SIZE-----
MODULE        -----WCH_Y_SIZE-----
MODULE        -----WCH_Y_SIZE-----
*****************************************************/
/*****************************************************
CH_WCH_Y_SIZE:			CH_WCH_Y_SIZE为WCH模块Y分量存储大小寄存器。
0x20581084: 				31:29          reserved
						28:16          height
						15:13          reserved
                                          12:00          width
*****************************************************/
static __inline HI_VOID hi_isp_wch_y_width_write(HI_U16 data){
	HI_U32 u32current = IO_READ32_VI(0x1084);
	IO_WRITE32_VI(0x1084, (data & 0x1fff) | (u32current & 0xffffe000));
}

static __inline HI_U16 hi_isp_wch_y_width_read(HI_VOID) {
	return (IO_READ32_VI(0x1084)&0x1fff);
}

static __inline HI_VOID hi_isp_wch_y_height_write(HI_U16 data){
	HI_U32 u32current = IO_READ32_VI(0x1084);
	IO_WRITE32_VI(0x1084, ((data & 0x1fff) << 16) | (u32current & 0x1fff));
}

static __inline HI_U16 hi_isp_wch_y_height_read(HI_VOID) {
	return ((IO_READ32_VI(0x1084)&0x1fff0000) >> 16);
}

/*****************************************************
CH_WCH_C_SIZE:			CH_WCH_C_SIZE为WCH模块Y分量存储大小寄存器。
0x205810A4: 				31:29          reserved
						28:16          height
						15:13          reserved
                                          12:00          width
*****************************************************/
static __inline HI_VOID hi_isp_wch_c_width_write(HI_U16 data){
	HI_U32 u32current = IO_READ32_VI(0x10a4);
	IO_WRITE32_VI(0x10a4, (data & 0x1fff) | (u32current & 0xffffe000));
}

static __inline HI_U16 hi_isp_wch_c_width_read(HI_VOID) {
	return (IO_READ32_VI(0x10a4)&0x1fff);
}

static __inline HI_VOID hi_isp_wch_c_height_write(HI_U16 data){
	HI_U32 u32current = IO_READ32_VI(0x10a4);
	IO_WRITE32_VI(0x10a4, ((data & 0x1fff) << 16) | (u32current & 0x1fff));
}

static __inline HI_U16 hi_isp_wch_c_height_read(HI_VOID) {
	return ((IO_READ32_VI(0x10a4)&0x1fff0000) >> 16);
}

/*****************************************************
ISP_RGBIR_EN: 			RGBIR module enable
0x205a3100: 				31:1 	reserved		
						0		enable
*****************************************************/
static __inline HI_VOID hi_isp_rgbir_en_write(HI_U8 data){
    IOWR_8DIRECT(0x3100, (data & 0x1));
}

static __inline HI_U32 hi_isp_rgbir_en_read(HI_VOID) {
    return (IORD_32DIRECT(0x3100) & 0x1);
}

/*****************************************************
ISP_RGBIR_EN: 			RGBIR control enable
0x205a3104: 				31:20 	reserved	
                           19       ir_out_en
                           18       ir_filter_en
                           17       blc_off_en
                           16       removel ir enable
                           15:2     reserved
                           1        auto gain enable
						0		manul gain enable
*****************************************************/
static __inline HI_VOID hi_isp_rgbir_ir_out_en_write(HI_U8 data){
    HI_U32 u32Current = IORD_32DIRECT(0x3104);
    IOWR_32DIRECT(0x3104, (u32Current & 0x7ffff) | ((data & 0x1) << 19));
}

static __inline HI_U32 hi_isp_rgbir_ir_out_en_read(HI_VOID) {
    return ((IORD_32DIRECT(0x3104) & 0x80000) >> 19);
}

static __inline HI_VOID hi_isp_rgbir_ir_filter_en_write(HI_U8 data){
    HI_U32 u32Current = IORD_32DIRECT(0x3104);
    IOWR_32DIRECT(0x3104, (u32Current & 0xbffff) | ((data & 0x1) << 18));
}

static __inline HI_U32 hi_isp_rgbir_ir_filter_en_read(HI_VOID) {
    return ((IORD_32DIRECT(0x3104) & 0x40000) >> 18);
}

static __inline HI_VOID hi_isp_rgbir_blc_off_en_write(HI_U8 data){
    HI_U32 u32Current = IORD_32DIRECT(0x3104);
    IOWR_32DIRECT(0x3104, (u32Current & 0xdffff) | ((data & 0x1) << 17));
}

static __inline HI_U32 hi_isp_rgbir_blc_off_en_read(HI_VOID) {
    return ((IORD_32DIRECT(0x3104) & 0x20000) >> 17);
}

static __inline HI_VOID hi_isp_rgbir_removel_en_write(HI_U8 data){
    HI_U32 u32Current = IORD_32DIRECT(0x3104);
    IOWR_32DIRECT(0x3104, (u32Current & 0xeffff) | ((data & 0x1) << 16));
}

static __inline HI_U32 hi_isp_rgbir_removel_en_read(HI_VOID) {
    return ((IORD_32DIRECT(0x3104) & 0x10000) >> 16);
}

static __inline HI_VOID hi_isp_rgbir_auto_gain_en_write(HI_U8 data){
    HI_U32 u32Current = IORD_8DIRECT(0x3104);
    IOWR_8DIRECT(0x3104, (u32Current & 0xffffd) | ((data & 0x1) << 1));
}

static __inline HI_U32 hi_isp_rgbir_auto_gain_en_read(HI_VOID) {
    return ((IORD_8DIRECT(0x3104) & 0x2) >> 1);
}

static __inline HI_VOID hi_isp_rgbir_manul_gain_en_write(HI_U8 data){
    HI_U32 u32Current = IORD_8DIRECT(0x3104);
    IOWR_8DIRECT(0x3104, (u32Current & 0xffffe) | (data & 0x1));
}

static __inline HI_U32 hi_isp_rgbir_manul_gain_en_read(HI_VOID) {
    return (IORD_8DIRECT(0x3104) & 0x1);
}

/*****************************************************
ISP_RGBIR_CTRL: 			RGBIR control 
0x205a3110: 				31:1 	reserved	
						0		IR position
*****************************************************/
static __inline HI_VOID hi_isp_rgbir_ir_position_write(HI_U8 data){
    IOWR_8DIRECT(0x3110, (data & 0x1));
}
    
static __inline HI_U32 hi_isp_rgbir_ir_position_read(HI_VOID) {
    return (IORD_8DIRECT(0x3110) & 0x1);
}

/*****************************************************
ISP_RGBIR_MANU_GAIN: 		RGBIR compensation manul gain 
0x205a3120: 				31:11 	reserved	
						9:0		manul gain
*****************************************************/
static __inline HI_VOID hi_isp_rgbir_manul_gain_write(HI_U16 data){
    IOWR_16DIRECT(0x3120, (data & 0x3ff));
}
    
static __inline HI_U32 hi_isp_rgbir_manul_gain_read(HI_VOID) {
    return (IORD_16DIRECT(0x3120) & 0x3ff);
}

/*****************************************************
ISP_RGBIR_G_MIN:          RGBIR green channel min
0x205a3130:               31:12 	reserved	
                          11:0		min_g
*****************************************************/
static __inline HI_VOID hi_isp_rgbir_g_min_write(HI_U16 data){
    IOWR_32DIRECT(0x3130, (data & 0xfff));
}
    
static __inline HI_U32 hi_isp_rgbir_g_min_read(HI_VOID) {
    return (IORD_32DIRECT(0x3130) & 0xfff);
}

/*****************************************************
ISP_RGBIR_RB_MAX:          RGBIR red and blue channel max
0x205a3134:               31:12 	reserved	
                          11:0		max_rb
*****************************************************/
static __inline HI_VOID hi_isp_rgbir_rb_max_write(HI_U16 data){
    IOWR_32DIRECT(0x3134, (data & 0xfff));
}
    
static __inline HI_U32 hi_isp_rgbir_rb_max_read(HI_VOID) {
    return (IORD_32DIRECT(0x3134) & 0xfff);
}

/*****************************************************
ISP_RGBIR_BLACK_LEVEL:    RGBIR black level
0x205a3138:               31:12 	reserved	
                          11:0		black_level
*****************************************************/
static __inline HI_VOID hi_isp_rgbir_black_level_write(HI_U16 data){
    IOWR_32DIRECT(0x3138, (data & 0xfff));
}
    
static __inline HI_U32 hi_isp_rgbir_black_level_read(HI_VOID) {
    return (IORD_32DIRECT(0x3138) & 0xfff);
}

/*****************************************************
ISP_RGBIR_F0:             RGBIR filter coefficient f0
0x205a3140:               31:12 	reserved	
                          11:8      coef02
                          7:4       coef01
                          3:0		coef00
*****************************************************/
static __inline HI_VOID hi_isp_rgbir_filter_coef02_write(HI_U8 data){
    HI_U32 u32Current = IORD_32DIRECT(0x3140);
    IOWR_16DIRECT(0x3140, (u32Current & 0xff) | ((data & 0xf) << 8));
}
    
static __inline HI_U32 hi_isp_rgbir_filter_coef02_read(HI_VOID) {
    return ((IORD_16DIRECT(0x3140) & 0xf00) >> 8);
}

static __inline HI_VOID hi_isp_rgbir_filter_coef01_write(HI_U8 data){
    HI_U32 u32Current = IORD_32DIRECT(0x3140);
    IOWR_16DIRECT(0x3140, (u32Current & 0xf0f) | ((data & 0xf) << 4));
}
    
static __inline HI_U32 hi_isp_rgbir_filter_coef01_read(HI_VOID) {
    return ((IORD_16DIRECT(0x3140) & 0xf0) >> 4);
}

static __inline HI_VOID hi_isp_rgbir_filter_coef00_write(HI_U8 data){
    HI_U32 u32Current = IORD_32DIRECT(0x3140);
    IOWR_16DIRECT(0x3140, (u32Current & 0xff0) | (data & 0xf));
}
    
static __inline HI_U32 hi_isp_rgbir_filter_coef00_read(HI_VOID) {
    return (IORD_16DIRECT(0x3140) & 0xf);
}

/*****************************************************
ISP_RGBIR_F1:             RGBIR filter coefficient f1
0x205a3144:               31:12 	reserved	
                          11:8      coef12
                          7:4       coef11
                          3:0		coef10
*****************************************************/
static __inline HI_VOID hi_isp_rgbir_filter_coef12_write(HI_U8 data){
    HI_U32 u32Current = IORD_32DIRECT(0x3144);
    IOWR_16DIRECT(0x3144, (u32Current & 0xff) | ((data & 0xf) << 8));
}
    
static __inline HI_U32 hi_isp_rgbir_filter_coef12_read(HI_VOID) {
    return ((IORD_16DIRECT(0x3144) & 0xf00) >> 8);
}

static __inline HI_VOID hi_isp_rgbir_filter_coef11_write(HI_U8 data){
    HI_U32 u32Current = IORD_32DIRECT(0x3144);
    IOWR_16DIRECT(0x3144, (u32Current & 0xf0f) | ((data & 0xf) << 4));
}
    
static __inline HI_U32 hi_isp_rgbir_filter_coef11_read(HI_VOID) {
    return ((IORD_16DIRECT(0x3144) & 0xf0) >> 4);
}

static __inline HI_VOID hi_isp_rgbir_filter_coef10_write(HI_U8 data){
    HI_U32 u32Current = IORD_32DIRECT(0x3144);
    IOWR_16DIRECT(0x3144, (u32Current & 0xff0) | (data & 0xf));
}
    
static __inline HI_U32 hi_isp_rgbir_filter_coef10_read(HI_VOID) {
    return (IORD_16DIRECT(0x3144) & 0xf);
}

/*****************************************************
ISP_RGBIR_F2:             RGBIR filter coefficient f2
0x205a3148:               31:12 	reserved	
                          11:8      coef22
                          7:4       coef21
                          3:0		coef20
*****************************************************/
static __inline HI_VOID hi_isp_rgbir_filter_coef22_write(HI_U8 data){
    HI_U32 u32Current = IORD_32DIRECT(0x3148);
    IOWR_16DIRECT(0x3148, (u32Current & 0xff) | ((data & 0xf) << 8));
}
    
static __inline HI_U32 hi_isp_rgbir_filter_coef22_read(HI_VOID) {
    return ((IORD_16DIRECT(0x3148) & 0xf00) >> 8);
}

static __inline HI_VOID hi_isp_rgbir_filter_coef21_write(HI_U8 data){
    HI_U32 u32Current = IORD_32DIRECT(0x3148);
    IOWR_16DIRECT(0x3148, (u32Current & 0xf0f) | ((data & 0xf) << 4));
}
    
static __inline HI_U32 hi_isp_rgbir_filter_coef21_read(HI_VOID) {
    return ((IORD_16DIRECT(0x3148) & 0xf0) >> 4);
}

static __inline HI_VOID hi_isp_rgbir_filter_coef20_write(HI_U8 data){
    HI_U32 u32Current = IORD_32DIRECT(0x3148);
    IOWR_16DIRECT(0x3148, (u32Current & 0xff0) | (data & 0xf));
}
    
static __inline HI_U32 hi_isp_rgbir_filter_coef20_read(HI_VOID) {
    return (IORD_16DIRECT(0x3148) & 0xf);
}

/*****************************************************
ISP_RGBIR_THRE:           RGBIR threshold
0x205a3150:               31:24 	reserved	
                          23:16     thre2
                          15:8      reserved
                          7:0		thre1
*****************************************************/
static __inline HI_VOID hi_isp_rgbir_thre2_write(HI_U8 data){
    HI_U32 u32Current = IORD_32DIRECT(0x3150);
    IOWR_32DIRECT(0x3150, (u32Current & 0xffff) | ((data & 0xff) << 16));
}
    
static __inline HI_U32 hi_isp_rgbir_thre2_read(HI_VOID) {
    return ((IORD_32DIRECT(0x3150) & 0xff0000) >> 16);
}

static __inline HI_VOID hi_isp_rgbir_thre1_write(HI_U8 data){
    HI_U32 u32Current = IORD_32DIRECT(0x3150);
    IOWR_16DIRECT(0x3150, (u32Current & 0xffff00) | (data & 0xff));
}
    
static __inline HI_U32 hi_isp_rgbir_thre1_read(HI_VOID) {
    return (IORD_16DIRECT(0x3150) & 0xff);
}

/*****************************************************
ISP_RGBIR_EXP_VALU:       RGBIR exposure max value
0x205a3160:               31:12 	reserved	
                          11:0		exp_value
*****************************************************/
static __inline HI_VOID hi_isp_rgbir_exp_value_write(HI_U16 data){
    IOWR_16DIRECT(0x3160, (data & 0xfff));
}
    
static __inline HI_U32 hi_isp_rgbir_exp_value_read(HI_VOID) {
    return (IORD_16DIRECT(0x3160) & 0xfff);
}

/*****************************************************
ISP_RGBIR_GAIN_USE:       RGBIR compensation gain use(read only)
0x205a3164:               31:12 	reserved	
                          11:0		gain_use
*****************************************************/

static __inline HI_U32 hi_isp_rgbir_gain_use_read(HI_VOID) {
    return (IORD_16DIRECT(0x3164) & 0xfff);
}

/*****************************************************
ISP_RGBIR_EXP_CNT:       RGBIR over exposure count
0x205a3168:               31:20 	reserved	
                          19:0		exp_cnt
*****************************************************/

static __inline HI_U32 hi_isp_rgbir_exp_cnt_read(HI_VOID) {
    return (IORD_32DIRECT(0x3168) & 0xfffff);
}

/*****************************************************
ISP_RGBIR_SCALE_B:        RGBIR scale b coefficient
0x205a3170:               31:30 	reserved	
                          29:20     b_b
                          19:10     b_g
                          9:0		b_r
*****************************************************/
static __inline HI_VOID hi_isp_rgbir_scale_b_b_write(HI_U16 data){
    HI_U32 u32Current = IORD_32DIRECT(0x3170);
    IOWR_32DIRECT(0x3170, (u32Current & 0xfffff) | ((data & 0x3ff) << 20));
}
    
static __inline HI_U32 hi_isp_rgbir_scale_b_b_read(HI_VOID) {
    HI_U32 u32Tmp;
    
    u32Tmp = ((IORD_32DIRECT(0x3170) & 0x3ff00000) >> 20);
    if (u32Tmp & 0x200)    
    {        
        u32Tmp |= 0xFFFFFC00;    
    }   

    return u32Tmp;
}

static __inline HI_VOID hi_isp_rgbir_scale_b_g_write(HI_U16 data){
    HI_U32 u32Current = IORD_32DIRECT(0x3170);
    IOWR_32DIRECT(0x3170, (u32Current & 0x3ff003ff) | ((data & 0x3ff) << 10));
}
    
static __inline HI_U32 hi_isp_rgbir_scale_b_g_read(HI_VOID) {
    HI_U32 u32Tmp;

    u32Tmp = (IORD_32DIRECT(0x3170) & 0xffc00) >> 10;
    if (u32Tmp & 0x200)    
    {        
        u32Tmp |= 0xFFFFFC00;    
    }   

    return u32Tmp;
}

static __inline HI_VOID hi_isp_rgbir_scale_b_r_write(HI_U16 data){
    HI_U32 u32Current = IORD_32DIRECT(0x3170);
    IOWR_32DIRECT(0x3170, (u32Current & 0x3ffffc00) | (data & 0x3ff));
}
    
static __inline HI_U32 hi_isp_rgbir_scale_b_r_read(HI_VOID) {
    HI_U32 u32Tmp;

    u32Tmp = (IORD_32DIRECT(0x3170) & 0x3ff);
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
static __inline HI_VOID hi_isp_rgbir_scale_g_b_write(HI_U16 data){
    HI_U32 u32Current = IORD_32DIRECT(0x3174);
    IOWR_32DIRECT(0x3174, (u32Current & 0xfffff) | ((data & 0x3ff) << 20));
}
    
static __inline HI_U32 hi_isp_rgbir_scale_g_b_read(HI_VOID) {
    HI_U32 u32Tmp;

    u32Tmp = ((IORD_32DIRECT(0x3174) & 0x3ff00000) >> 20);
    if (u32Tmp & 0x200)    
    {        
        u32Tmp |= 0xFFFFFC00;    
    }   

    return u32Tmp;
}

static __inline HI_VOID hi_isp_rgbir_scale_g_g_write(HI_U16 data){
    HI_U32 u32Current = IORD_32DIRECT(0x3174);
    IOWR_32DIRECT(0x3174, (u32Current & 0x3ff003ff) | ((data & 0x3ff) << 10));
}
    
static __inline HI_U32 hi_isp_rgbir_scale_g_g_read(HI_VOID) {
    HI_U32 u32Tmp;

    u32Tmp = ((IORD_32DIRECT(0x3174) & 0xffc00) >> 10);
    if (u32Tmp & 0x200)    
    {        
        u32Tmp |= 0xFFFFFC00;    
    }   

    return u32Tmp;
}

static __inline HI_VOID hi_isp_rgbir_scale_g_r_write(HI_U16 data){
    HI_U32 u32Current = IORD_32DIRECT(0x3174);
    IOWR_32DIRECT(0x3174, (u32Current & 0x3ffffc00) | (data & 0x3ff));
}
    
static __inline HI_U32 hi_isp_rgbir_scale_g_r_read(HI_VOID) {
    HI_U32 u32Tmp;

    u32Tmp = (IORD_32DIRECT(0x3174) & 0x3ff);
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
static __inline HI_VOID hi_isp_rgbir_scale_r_b_write(HI_U16 data){
    HI_U32 u32Current = IORD_32DIRECT(0x3178);
    IOWR_32DIRECT(0x3178, (u32Current & 0xfffff) | ((data & 0x3ff) << 20));
}
    
static __inline HI_U32 hi_isp_rgbir_scale_r_b_read(HI_VOID) {
    HI_U32 u32Tmp;

    u32Tmp = ((IORD_32DIRECT(0x3178) & 0x3ff00000) >> 20);
    if (u32Tmp & 0x200)    
    {        
        u32Tmp |= 0xFFFFFC00;    
    }   

    return u32Tmp;
}

static __inline HI_VOID hi_isp_rgbir_scale_r_g_write(HI_U16 data){
    HI_U32 u32Current = IORD_32DIRECT(0x3178);
    IOWR_32DIRECT(0x3178, (u32Current & 0x3ff003ff) | ((data & 0x3ff) << 10));
}
    
static __inline HI_U32 hi_isp_rgbir_scale_r_g_read(HI_VOID) {
    HI_U32 u32Tmp;

    u32Tmp = ((IORD_32DIRECT(0x3178) & 0xffc00) >> 10);
    if (u32Tmp & 0x200)    
    {        
        u32Tmp |= 0xFFFFFC00;    
    }   

    return u32Tmp;
}

static __inline HI_VOID hi_isp_rgbir_scale_r_r_write(HI_U16 data){
    HI_U32 u32Current = IORD_32DIRECT(0x3178);
    IOWR_32DIRECT(0x3178, (u32Current & 0x3ffffc00) | (data & 0x3ff));
}
    
static __inline HI_U32 hi_isp_rgbir_scale_r_r_read(HI_VOID) {
    HI_U32 u32Tmp;

    u32Tmp = (IORD_32DIRECT(0x3178) & 0x3ff);
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
static __inline HI_VOID hi_isp_rgbir_scale_ir_b_write(HI_U16 data){
    HI_U32 u32Current = IORD_32DIRECT(0x317c);
    IOWR_32DIRECT(0x317c, (u32Current & 0xfffff) | ((data & 0x3ff) << 20));
}
    
static __inline HI_U32 hi_isp_rgbir_scale_ir_b_read(HI_VOID) {
    HI_U32 u32Tmp;

    u32Tmp = ((IORD_32DIRECT(0x317c) & 0x3ff00000) >> 20);
    if (u32Tmp & 0x200)    
    {        
        u32Tmp |= 0xFFFFFC00;    
    }   

    return u32Tmp;
}

static __inline HI_VOID hi_isp_rgbir_scale_ir_g_write(HI_U16 data){
    HI_U32 u32Current = IORD_32DIRECT(0x317c);
    IOWR_32DIRECT(0x317c, (u32Current & 0x3ff003ff) | ((data & 0x3ff) << 10));
}
    
static __inline HI_U32 hi_isp_rgbir_scale_ir_g_read(HI_VOID) {
    HI_U32 u32Tmp;

    u32Tmp = ((IORD_32DIRECT(0x317c) & 0xffc00) >> 10);
    if (u32Tmp & 0x200)    
    {        
        u32Tmp |= 0xFFFFFC00;    
    }   

    return u32Tmp;
}

static __inline HI_VOID hi_isp_rgbir_scale_ir_r_write(HI_U16 data){
    HI_U32 u32Current = IORD_32DIRECT(0x317c);
    IOWR_32DIRECT(0x317c, (u32Current & 0x3ffffc00) | (data & 0x3ff));
}
    
static __inline HI_U32 hi_isp_rgbir_scale_ir_r_read(HI_VOID) {
    HI_U32 u32Tmp;
    
    u32Tmp = (IORD_32DIRECT(0x317c) & 0x3ff);
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
static __inline HI_VOID hi_isp_rgbir_scale_c_b_write(HI_U16 data){
    HI_U32 u32Current = IORD_32DIRECT(0x3180);
    IOWR_32DIRECT(0x3180, (u32Current & 0xfffff) | ((data & 0x3ff) << 20));
}
    
static __inline HI_U32 hi_isp_rgbir_scale_c_b_read(HI_VOID) {
    HI_U32 u32Tmp;

    u32Tmp = ((IORD_32DIRECT(0x3180) & 0x3ff00000) >> 20);
    if (u32Tmp & 0x200)    
    {        
        u32Tmp |= 0xFFFFFC00;    
    }   

    return u32Tmp;
}

static __inline HI_VOID hi_isp_rgbir_scale_c_g_write(HI_U16 data){
    HI_U32 u32Current = IORD_32DIRECT(0x3180);
    IOWR_32DIRECT(0x3180, (u32Current & 0x3ff003ff) | ((data & 0x3ff) << 10));
}
    
static __inline HI_U32 hi_isp_rgbir_scale_c_g_read(HI_VOID) {
    HI_U32 u32Tmp;

    u32Tmp = ((IORD_32DIRECT(0x3180) & 0xffc00) >> 10);
    if (u32Tmp & 0x200)    
    {        
        u32Tmp |= 0xFFFFFC00;    
    }   

    return u32Tmp;
}

static __inline HI_VOID hi_isp_rgbir_scale_c_r_write(HI_U16 data){
    HI_U32 u32Current = IORD_32DIRECT(0x3180);
    IOWR_32DIRECT(0x3180, (u32Current & 0x3ffffc00) | (data & 0x3ff));
}
    
static __inline HI_U32 hi_isp_rgbir_scale_c_r_read(HI_VOID) {
    HI_U32 u32Tmp;

    u32Tmp = (IORD_32DIRECT(0x3180) & 0x3ff);
    if (u32Tmp & 0x200)    
    {        
        u32Tmp |= 0xFFFFFC00;    
    }   

    return u32Tmp;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __ISP_CONFIG_CONFIG_EX_H__ */

