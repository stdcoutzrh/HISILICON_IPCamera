/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2010/08/17
  Description   :
  History       :
  1.Date        : 2010/08/17
    Author      : n00168968
    Modification: Created file

******************************************************************************/


#include <linux/module.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <asm/errno.h>

#include <asm/uaccess.h>
#include <asm/io.h>

#include "hi_common.h"
#include "hi_comm_isp.h"
#include "hi_comm_vb.h"
#include "mkp_isp.h"
#include "isp.h"
#include "hi_drv_vreg.h"
#include "mm_ext.h"
#include "proc_ext.h"
#include "mod_ext.h"
#include "sys_ext.h"
#include "isp_ext.h"
#include "dev_ext.h"
#include "acm_ext.h"


#include "piris_ext.h"

#include "isp_metering_mem_config.h"
#include "isp_histogram_mem_config.h"
#include "isp_config.h"
#include "isp.h"

#ifdef HI_GPIO_I2C
#include "gpioi2c_ex.h"
#else
#include "hi_i2c.h"
#include "hi_spi.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */


/****************************************************************************
 * MACRO DEFINITION                                                         *
 ****************************************************************************/
#define VICAP_BASE              0x20580000

#define IO_ADDRESS_PORT(x)      (reg_vicap_base_va + x)
#define HW_REG(reg)             *((volatile unsigned int *)(reg))

#define ISP_BASE                0x205A0000
#define ISP_OFFSET              0x10000

/* 0x00F8[8]: int_isp_en; 0x00F8[16]: int_pt0_en; 0x00F8[0]: int_ch0_en; */
#define VICAP_HD_MASK           0x00F8
#define VICAP_HD_INTTERRUPT     0x00F0

#define VI_PT0_INT_MASK         0x01F8
#define VI_PT0_INT              0x01F0
#define VI_PT0_INT_FSTART       (1<<0)
#define VI_PT0_INT_ERR          (3<<1)

#define ISP_INT_MASK            0x200F8
#define ISP_INT                 0x200F0

#define CH_REG_NEWER            0x1004
#define ISP_444_REG_NEWER       0x201EC
#define ISP_422_REG_NEWER       0x202EC
/* 0x01A4: [31]:enable; [30]:mode; [29]:reset; [5:4]:vc_num; [3:2]:vc_num_max; [1:0]:vc_num_init_value; */
#define VC_NUM_ADDR             0x01A4

#define ISP_RESET               0x0078
#define CHN_SWITCH              0x10028

#define ISP_MAGIC_OFFSET        1
#define ISP_PROC_SIZE           0x10000 /* 10k */

#define IO_ISP_ADDRESS(x)      ((unsigned long)reg_isp_base_va + (x))
#define IO_RD_ISP_ADDRESS(x)   (*((unsigned long *)IO_ISP_ADDRESS(x)))

#define ISP_STAT_DRC_MEM       ((768*128)/8)
#define ISP_STAT_DRC_MEM_NUM   (2)

extern HI_BOOL VB_IsSupplementSupport(HI_U32 u32Mask);
#define CHIP_SUPPORT_JPEGEDCF()    (VB_IsSupplementSupport(VB_SUPPLEMENT_JPEG_MASK))

static HI_U32 g_u32ISP_STAT_DRC_PHY = HI_NULL;
void ISP_IntBottomHalf(unsigned long data);
/****************************************************************************
 * GLOBAL VARIABLES                                                         *
 ****************************************************************************/
ISP_DRV_CTX_S           g_astIspDrvCtx[1] = {{0}};
      
ISP_EXPORT_FUNC_S       g_stIspExpFunc = {0};
ISP_PIRIS_EXPORT_FUNC_S  g_stIspPirisExpFunc = {0};
static ISP_VERSION_S    gs_stIspLibInfo;
void __iomem            *reg_vicap_base_va = HI_NULL;
void __iomem            *reg_isp_base_va = HI_NULL;

HI_U32                  proc_param = 0;        /* 0: close proc; n: write proc info's interval int num */
HI_U32                  pwm_num = 1;
HI_U32                  update_pos = 0;         /* 0: frame start; 1: frame end */
bool                    int_bottomhalf = HI_FALSE;  /* 1 to enable interrupt processing at bottom half */

spinlock_t g_stIspLock;

//#define TEST_TIME

#ifdef TEST_TIME
HI_U32  g_test_phyaddr;
HI_U16 *g_test_pviraddr;
struct timeval time1;
struct timeval time2;
#endif
/****************************************************************************
 * INTERNAL FUNCTION DECLARATION                                            *
 ****************************************************************************/

static HI_U16 Sqrt32(HI_U32 u32Arg)
{
    HI_U32 u32Mask = (HI_U32)1 << 15;
    HI_U16 u16Res  = 0;
    HI_U32 i = 0;

    for(i=0; i<16; i++)
    {
        if((u16Res + (u32Mask >> i))*(u16Res + (u32Mask >> i)) <= u32Arg)
        {
            u16Res = u16Res + (u32Mask >> i);
        }
    }

    /* rounding */
    if (u16Res * u16Res + u16Res < u32Arg)
    {
        ++u16Res;
    }
    
    return u16Res;
}

HI_S32 ISP_SetIntEnable(ISP_DEV IspDev, HI_BOOL bEn)
{
    if (bEn)
    {
        HW_REG(IO_ADDRESS_PORT(ISP_INT_MASK)) = 0x243;
        //HW_REG(IO_ADDRESS_PORT(ISP_INT_MASK)) = 0xf;
    }
    else
    {
        HW_REG(IO_ADDRESS_PORT(ISP_INT_MASK)) = 0x0;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_GetFrameEdge(ISP_DEV IspDev, HI_U32 *pu32Status)
{
    unsigned long u32Flags;
    HI_S32 s32Ret;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;

    ISP_CHECK_DEV(IspDev);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);
    
    s32Ret = wait_event_interruptible_timeout(pstDrvCtx->stIspWait, (pstDrvCtx->bEdge == HI_TRUE), msecs_to_jiffies(200));
    if (s32Ret <= 0)
    {
        #if 0
        printk(KERN_INFO  "Wait frame edge timeout!\n");
        #endif
        return -1;
    }

    spin_lock_irqsave(&g_stIspLock, u32Flags);
    pstDrvCtx->bEdge = HI_FALSE;
    *pu32Status = pstDrvCtx->u32Status;
    spin_unlock_irqrestore(&g_stIspLock, u32Flags);

    return HI_SUCCESS;
}

static int ISP_GetVdTimeOut(ISP_DEV IspDev, ISP_VD_INFO_S  *pIspVdInfo,
    HI_U32 u32MilliSec, HI_U32 *pu32status)
{
    unsigned long u32Flags;
    HI_S32 s32Ret;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;

    ISP_CHECK_DEV(IspDev);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);
    
    if(down_interruptible(&pstDrvCtx->stIspSem))
    {
       return -ERESTARTSYS;
    }

    spin_lock_irqsave(&g_stIspLock, u32Flags);
    pstDrvCtx->bVd = HI_FALSE;
    spin_unlock_irqrestore(&g_stIspLock, u32Flags);
    
    if(0 != u32MilliSec)
    {
        s32Ret = wait_event_interruptible_timeout(pstDrvCtx->stIspWaitVd,
            (HI_TRUE == pstDrvCtx->bVd), msecs_to_jiffies(u32MilliSec));
        if (s32Ret <= 0)
        {
            #if 0
            printk(KERN_INFO  "Wait frame edge timeout!\n");
            #endif
            up(&pstDrvCtx->stIspSem);
            return -1;
        }
    }
    else
    {
        s32Ret = wait_event_interruptible(pstDrvCtx->stIspWaitVd, (HI_TRUE == pstDrvCtx->bVd));
        if(s32Ret)
        {
            up(&pstDrvCtx->stIspSem);
            return -1;
        }

    }

    *pu32status = pstDrvCtx->u32Status;

    up(&pstDrvCtx->stIspSem);

    return 0;
}

HI_S32 ISP_DRV_StatisticsRead(ISP_STAT_INFO_S *pstStatInfo)
{
    ISP_STAT_S *pstStat = HI_NULL;    
    HI_S32 i, j, k;
    HI_U32 u32Value;
	HI_U32 u32AveMem;
	HI_U32 u32HighBit = 0, u32LowBit = 0;  
	HI_U32 u32StatData;
	HI_U8 u8DataMode;
    ISP_DEV IspDev = 0;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

    pstStat = (ISP_STAT_S *)pstStatInfo->pVirtAddr;
    if (HI_NULL == pstStat)
    {
        return HI_FAILURE;
    }

#ifdef TEST_TIME
    {
        printk("%x\n", g_test_pviraddr);

        do_gettimeofday(&time1);
        for(i = 0; i < 256; i++)
        {
            pstStat->stAeStat3.au16HistogramMemArray[i] = *(g_test_pviraddr + i);
        }
        do_gettimeofday(&time2);
        printk("read memory %d\n", (time2.tv_sec -time1.tv_sec)*1000000 + time2.tv_usec - time1.tv_usec);
        do_gettimeofday(&time1);
        memcpy(pstStat->stAeStat3.au16HistogramMemArray, g_test_pviraddr, 256*2);
        do_gettimeofday(&time2);
        printk("memory copy %d\n", (time2.tv_sec -time1.tv_sec)*1000000 + time2.tv_usec - time1.tv_usec);
        do_gettimeofday(&time1);
        for(i = 0; i < 256; i++)
        {
            pstStat->stAeStat3.au16HistogramMemArray[i] =
            IO_RD_ISP_ADDRESS(HI_HISTOGRAM_MEM_BASE_ADDR + (i << 2));
        }
        do_gettimeofday(&time2);
        printk("read register %d\n", (time2.tv_sec -time1.tv_sec)*1000000 + time2.tv_usec - time1.tv_usec);
    }
#endif

#if 0
    if (pstStatInfo->unKey.bit1AeStat1)
    {
        pstStat->stAeStat1.au16MeteringHist[0] = hi_isp_metering_hist_0_read();
        pstStat->stAeStat1.au16MeteringHist[1] = hi_isp_metering_hist_1_read();
        pstStat->stAeStat1.au16MeteringHist[2] = hi_isp_metering_hist_3_read();
        pstStat->stAeStat1.au16MeteringHist[3] = hi_isp_metering_hist_4_read();
        pstStat->stAeStat1.au8MeteringHistThresh[0] = hi_isp_metering_hist_thresh_0_1_read();
        pstStat->stAeStat1.au8MeteringHistThresh[1] = hi_isp_metering_hist_thresh_1_2_read();
        pstStat->stAeStat1.au8MeteringHistThresh[2] = hi_isp_metering_hist_thresh_3_4_read();
        pstStat->stAeStat1.au8MeteringHistThresh[3] = hi_isp_metering_hist_thresh_4_5_read();
    }
#endif

    if (pstStatInfo->unKey.bit1AwbStat3)
    {
        pstStat->stAwbStat3.u16MeteringAwbAvgR = hi_isp_metering_avg_r_read();
        pstStat->stAwbStat3.u16MeteringAwbAvgG = hi_isp_metering_avg_g_read();
        pstStat->stAwbStat3.u16MeteringAwbAvgB = hi_isp_metering_avg_b_read();
        pstStat->stAwbStat3.u16MeteringAwbCountAll = hi_isp_metering_count_all_read();
        pstStat->stAwbStat3.u16MeteringAwbCountMin = hi_isp_metering_count_min_read();
        pstStat->stAwbStat3.u16MeteringAwbCountMax = hi_isp_metering_count_max_read();
    }

	if (pstStatInfo->unKey.bit1AfStat)
	{
		u8DataMode = hi_isp_af_data_mode_read();

		hi_isp_af_stat_ind_raddr_write(0x0);
		if (u8DataMode == 0)
		{
			for (i = 0; i < AF_ZONE_ROW; i++)
	    	{
		        for (j = 0; j < AF_ZONE_COLUMN; j++)
		        {
					for (k = 0; k < 3; k++)
					{
						u32StatData = hi_isp_af_stat_ind_rdata_read();
					    if (k == 0)
					    {
					        pstStat->stAfStat.stZoneMetrics[i][j].u16v1 = (HI_U16)((u32StatData & 0xFFFF0000) >> 16);
							pstStat->stAfStat.stZoneMetrics[i][j].u16h1 = (HI_U16)(u32StatData & 0xFFFF);
					    }
						else if (k == 1)
						{
						    pstStat->stAfStat.stZoneMetrics[i][j].u16v2 = (HI_U16)((u32StatData & 0xFFFF0000) >> 16);
							pstStat->stAfStat.stZoneMetrics[i][j].u16h2 = (HI_U16)(u32StatData & 0xFFFF);
						}
						else
						{
						   pstStat->stAfStat.stZoneMetrics[i][j].u16y = (HI_U16)(u32StatData & 0xFFFF);
						}
					}           
		        }
	    	}
		}
		else if (u8DataMode == 1)
		{
		    for (i = 0; i < AF_ZONE_ROW; i++)
	    	{
		        for (j = 0; j < AF_ZONE_COLUMN; j++)
		        {
					for (k = 0; k < 3; k++)
					{
						u32StatData = hi_isp_af_stat_ind_rdata_read();
					    if (k == 0)
					    {
					        pstStat->stAfStat.stZoneMetrics[i][j].u16v1 = (HI_U16)((u32StatData & 0x03FFE000) >> 13);
							pstStat->stAfStat.stZoneMetrics[i][j].u16h1 = (HI_U16)(u32StatData & 0x1FFF);
							u32LowBit = (u32StatData & 0xFC000000) >> 26;
					    }
						else if (k == 1)
						{
						    pstStat->stAfStat.stZoneMetrics[i][j].u16v2 = (HI_U16)((u32StatData & 0x03FFE000) >> 13);
							pstStat->stAfStat.stZoneMetrics[i][j].u16h2 = (HI_U16)(u32StatData & 0x1FFF);
							u32HighBit = (u32StatData & 0xFC000000) >> 26;
						}
						else
						{
							pstStat->stAfStat.stZoneMetrics[i][j].u16y  = (HI_U16)((u32HighBit << 6) | u32LowBit);
						}
					}           
		        }
	    	}
		}
	}

    if (pstStatInfo->unKey.bit1DpStat)
    {
        pstStat->stDpStat.u16DefectPixelCount = hi_isp_defect_pixel_defect_pixel_count_read();
    }

    if (pstStatInfo->unKey.bit1CommStat)
    {
        pstStat->stCommStat.au16WhiteBalanceGain[0] = hi_isp_white_balance_gain_00_read();
        pstStat->stCommStat.au16WhiteBalanceGain[1] = hi_isp_white_balance_gain_01_read();
        pstStat->stCommStat.au16WhiteBalanceGain[2] = hi_isp_white_balance_gain_10_read();
        pstStat->stCommStat.au16WhiteBalanceGain[3] = hi_isp_white_balance_gain_11_read();
    }
    
#if 0
    if (pstStatInfo->unKey.bit1AeStat2)
    {
        pstStat->stAeStat2.au8MeteringHistThresh[0] = hi_isp_metering_hist_thresh_0_1_read();
        pstStat->stAeStat2.au8MeteringHistThresh[1] = hi_isp_metering_hist_thresh_1_2_read();
        pstStat->stAeStat2.au8MeteringHistThresh[2] = hi_isp_metering_hist_thresh_3_4_read();
        pstStat->stAeStat2.au8MeteringHistThresh[3] = hi_isp_metering_hist_thresh_4_5_read();  
        
        for(i = 0;i < AE_ZONE_ROW ; i++)
        {
            for(j=0;j< AE_ZONE_COLUMN ;j++)
            {
            #if 0
                u32Hist10 = hi_metering_mem_arrary_data_read_ae((i*AE_ZONE_COLUMN+j)*2 + 0);
                u32Hist43 = hi_metering_mem_arrary_data_read_ae((i*AE_ZONE_COLUMN+j)*2 + 1);
            #else
                u32Hist10 = IO_RD_ISP_ADDRESS(HI_AE_METERING_MEM_BASE_ADDR +
                    (((i*AE_ZONE_COLUMN+j)*2 + 0) << 2));
                u32Hist43 = IO_RD_ISP_ADDRESS(HI_AE_METERING_MEM_BASE_ADDR +
                    (((i*AE_ZONE_COLUMN+j)*2 + 1) << 2));
            #endif
                
                /* todo : optimize,read first, then calculate */
                pstStat->stAeStat2.au16MeteringMemArrary[i][j][0] = (HI_U16)(u32Hist10 & 0xffff);
                pstStat->stAeStat2.au16MeteringMemArrary[i][j][1] = (HI_U16)((u32Hist10 & 0xffff0000) >> 16);
                pstStat->stAeStat2.au16MeteringMemArrary[i][j][3] = (HI_U16)(u32Hist43 & 0xffff);
                pstStat->stAeStat2.au16MeteringMemArrary[i][j][4] = (HI_U16)((u32Hist43 & 0xffff0000) >> 16);
            }       
        }
    }
#endif
	


    if (pstStatInfo->unKey.bit1AeStat3)
    {
    	hi_isp_ae_mem_hist_raddr_write(0);

        for(i = 0; i < 256; i++)
        {
            pstStat->stAeStat3.au32HistogramMemArray[i] = hi_isp_ae_mem_hist_rdata_read();
        }
        pstStat->stAeStat3.u32PixelCount  = hi_isp_ae_pixel_selected_count_read();
        pstStat->stAeStat3.u32PixelWeight = hi_isp_ae_pixel_selected_weight_read();
    }
	
	if (pstStatInfo->unKey.bit1AeStat4)
    {
        pstStat->stAeStat4.u16GlobalAvgR  = hi_isp_ae_total_r_aver_read();        
        pstStat->stAeStat4.u16GlobalAvgGr = hi_isp_ae_total_gr_aver_read();
        pstStat->stAeStat4.u16GlobalAvgGb = hi_isp_ae_total_gb_aver_read();
        pstStat->stAeStat4.u16GlobalAvgB  = hi_isp_ae_total_b_aver_read();
    }

	if (pstStatInfo->unKey.bit1AeStat5)
    {   
    	hi_isp_ae_mem_aver_raddr_write(0);
        for(i = 0;i < AE_ZONE_ROW ; i++)
        {
            for(j=0;j< AE_ZONE_COLUMN ;j++)
            {
				u32AveMem = hi_isp_ae_mem_aver_rdata_read();

                pstStat->stAeStat5.au16ZoneAvg[i][j][0] = (HI_U8)((u32AveMem & 0xff000000) >> 24);
                pstStat->stAeStat5.au16ZoneAvg[i][j][1] = (HI_U8)((u32AveMem & 0xff0000) >> 16);
                pstStat->stAeStat5.au16ZoneAvg[i][j][2] = (HI_U8)((u32AveMem & 0xff00) >> 8);
                pstStat->stAeStat5.au16ZoneAvg[i][j][3] = (HI_U8)((u32AveMem & 0xff));
            }       
        }
    }

    if (pstStatInfo->unKey.bit1AwbStat4)
    {
        hi_isp_awb_stat_raddr_write(0);
		
        for (i=0; i<255; i++)
        {
            u32Value = hi_isp_awb_stat_rdata_read();
            pstStat->stAwbStat4.au16MeteringMemArrayAvgR[i] = (u32Value & 0xffff);
            pstStat->stAwbStat4.au16MeteringMemArrayAvgG[i] = ((u32Value >> 16) & 0xffff);
            u32Value = hi_isp_awb_stat_rdata_read();
            pstStat->stAwbStat4.au16MeteringMemArrayAvgB[i] = (u32Value & 0xffff);
            pstStat->stAwbStat4.au16MeteringMemArrayCountAll[i] = ((u32Value >> 16) & 0xffff);
            u32Value = hi_isp_awb_stat_rdata_read();
            pstStat->stAwbStat4.au16MeteringMemArrayCountMin[i] = u32Value & 0xffff;
            pstStat->stAwbStat4.au16MeteringMemArrayCountMax[i] = (u32Value>>16) & 0xffff;
        }
    }

#if 0
    if (pstStatInfo->unKey.bit1AeStat2)
    {
        for(i = 0;i < AE_ZONE_ROW ; i++)
        {
            for(j=0;j< AE_ZONE_COLUMN ;j++)
            {                
                u32Sum = pstStat->stAeStat2.au16MeteringMemArrary[i][j][0]
                    + pstStat->stAeStat2.au16MeteringMemArrary[i][j][1]
                    + pstStat->stAeStat2.au16MeteringMemArrary[i][j][3]
                    + pstStat->stAeStat2.au16MeteringMemArrary[i][j][4];              
                if(u32Sum > 0xFFFF)
                {
                    u32Sum = 0xFFFF;
                }

                pstStat->stAeStat2.au16MeteringMemArrary[i][j][2] = 0xFFFF - u32Sum;
            }
        }
    }
#endif

    
    if(pstStatInfo->unKey.bit1Dehaze)
    {
        
         j = DEFOG_ZONE_NUM / 4;
		 hi_isp_dehaze_minstat_raddr_write(0);
         for(i = 0; i < j; i++)
        {
            //pstStat->stDehazeStat.au32DehazeMinDout[i] = hi_vi_dehaze_min_stat_read(i);
            pstStat->stDehazeStat.au32DehazeMinDout[i] = hi_isp_dehaze_minstat_rdata_read();
        }
        #if 0
        for(i = 0; i < j; i++)
        {
            HI_U32 u32Value = hi_vi_dehaze_min_stat_read(i);
            pstStat->stDehazeStat.au8DehazeMinDout[4*i]   = (u32Value >> 24) & 0xff;
            pstStat->stDehazeStat.au8DehazeMinDout[4*i+1] = (u32Value >> 16) & 0xff;
            pstStat->stDehazeStat.au8DehazeMinDout[4*i+2] = (u32Value >>  8) & 0xff;
            pstStat->stDehazeStat.au8DehazeMinDout[4*i+3] = (u32Value      ) & 0xff;
        }
       for(i = 0; i < 1024; i++)
        {
            pstStat->stDehazeStat.au32DehazeMaxStatDout[i] = hi_vi_dehaze_max_stat_read(i);
        }
        for(i = 0; i < 32; i++)
        {
            pstStat->stDehazeStat.au32Hist[i] = hi_vi_dehaze_hist_read(i);
        }
        #endif
    }   

    /* copy stat to shadow mem */
    // abandon this frame while user were operating
    if (HI_TRUE != pstDrvCtx->stStatShadowMem.bUsrAccess) 
    {
        memcpy(pstDrvCtx->stStatShadowMem.pVirtAddr, pstStat, sizeof(ISP_STAT_S));        
    }
    
    return HI_SUCCESS;
}

HI_S32 ISP_DRV_StatBufInit(ISP_DEV IspDev)
{
    HI_S32 s32Ret, i;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    HI_U32 u32PhyAddr, u32Size;
    HI_U8  *pu8VirAddr;
    unsigned long u32Flags;

    ISP_CHECK_DEV(IspDev);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

    u32Size = sizeof(ISP_STAT_S) * MAX_ISP_STAT_BUF_NUM;
    s32Ret = CMPI_MmzMallocNocache(HI_NULL, "ISPStat", &u32PhyAddr, (HI_VOID**)&pu8VirAddr, u32Size);
    if (HI_SUCCESS != s32Ret)
    {
        ISP_TRACE(HI_DBG_ERR, "alloc statistics buf err\n");
        return HI_ERR_ISP_NOMEM;
    }

	/********************************************
	DRC_STAT_MEM:Temp Version
	********************************************/
	g_u32ISP_STAT_DRC_PHY = CMPI_MmzMalloc(HI_NULL, "ISPDRCStat", ISP_STAT_DRC_MEM*ISP_STAT_DRC_MEM_NUM);
	if (0 == g_u32ISP_STAT_DRC_PHY)
    {
        ISP_TRACE(HI_DBG_ERR, "alloc statistics buf err\n");
        return HI_ERR_ISP_NOMEM;
    }
	else
	{
		hi_isp_stat_frm_baddr_write((g_u32ISP_STAT_DRC_PHY&0xfff00000) >> 20);

		hi_isp_drca_frm_baddr_write(g_u32ISP_STAT_DRC_PHY&0xfffff);
		hi_isp_drca_rd_frm_baddr_write(g_u32ISP_STAT_DRC_PHY&0xfffff);
			
		hi_isp_drcb_frm_baddr_write((g_u32ISP_STAT_DRC_PHY+ISP_STAT_DRC_MEM)&0xfffff);
		hi_isp_drcb_rd_frm_baddr_write((g_u32ISP_STAT_DRC_PHY+ISP_STAT_DRC_MEM)&0xfffff);
		hi_isp_drc_rd_en_write(HI_TRUE);
	}

    memset(pu8VirAddr, 0, u32Size);

    spin_lock_irqsave(&g_stIspLock, u32Flags);
    pstDrvCtx->stStatisticsBuf.u32PhyAddr = u32PhyAddr;
    pstDrvCtx->stStatisticsBuf.pVirAddr = (HI_VOID *)pu8VirAddr;

    INIT_LIST_HEAD(&pstDrvCtx->stStatisticsBuf.stFreeList);
    INIT_LIST_HEAD(&pstDrvCtx->stStatisticsBuf.stBusyList);
    INIT_LIST_HEAD(&pstDrvCtx->stStatisticsBuf.stUserList);

    for (i = 0; i < MAX_ISP_STAT_BUF_NUM; i++)
    {
        pstDrvCtx->stStatisticsBuf.astNode[i].stStatInfo.u32PhyAddr =
            u32PhyAddr + i * sizeof(ISP_STAT_S);
        pstDrvCtx->stStatisticsBuf.astNode[i].stStatInfo.pVirtAddr =
            (HI_VOID *)(pu8VirAddr + i * sizeof(ISP_STAT_S));

        list_add_tail(&pstDrvCtx->stStatisticsBuf.astNode[i].list,
            &pstDrvCtx->stStatisticsBuf.stFreeList);
    }

    pstDrvCtx->stStatisticsBuf.u32BusyNum = 0;
    pstDrvCtx->stStatisticsBuf.u32UserNum = 0;
    pstDrvCtx->stStatisticsBuf.u32FreeNum = MAX_ISP_STAT_BUF_NUM;
    
    spin_unlock_irqrestore(&g_stIspLock, u32Flags);

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_StatBufExit(ISP_DEV IspDev)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    HI_U32 u32PhyAddr;
    HI_U8  *pu8VirAddr;
    unsigned long u32Flags;

    ISP_CHECK_DEV(IspDev);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

    u32PhyAddr = pstDrvCtx->stStatisticsBuf.u32PhyAddr;
    pu8VirAddr = (HI_U8 *)pstDrvCtx->stStatisticsBuf.pVirAddr;
    
    spin_lock_irqsave(&g_stIspLock, u32Flags);
    pstDrvCtx->stStatisticsBuf.pVirAddr = HI_NULL;
    pstDrvCtx->stStatisticsBuf.astNode[0].stStatInfo.pVirtAddr = HI_NULL;
    pstDrvCtx->stStatisticsBuf.astNode[1].stStatInfo.pVirtAddr = HI_NULL;
    pstDrvCtx->stStatisticsBuf.u32PhyAddr = 0;
    pstDrvCtx->stStatisticsBuf.astNode[0].stStatInfo.u32PhyAddr = 0;
    pstDrvCtx->stStatisticsBuf.astNode[1].stStatInfo.u32PhyAddr = 0;

    INIT_LIST_HEAD(&pstDrvCtx->stStatisticsBuf.stFreeList);
    INIT_LIST_HEAD(&pstDrvCtx->stStatisticsBuf.stBusyList);
    INIT_LIST_HEAD(&pstDrvCtx->stStatisticsBuf.stUserList);
    spin_unlock_irqrestore(&g_stIspLock, u32Flags);

    if (0 != u32PhyAddr)
    {
        CMPI_MmzFree(u32PhyAddr, pu8VirAddr);
    }

	/********************************************
	DRC_STAT_MEM:Temp Version
	********************************************/	
	if (0 != g_u32ISP_STAT_DRC_PHY)
    {
    	hi_isp_drc_rd_en_write(HI_FALSE);
        CMPI_MmzFree(g_u32ISP_STAT_DRC_PHY, HI_NULL);
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_StatBufUserGet(ISP_DEV IspDev, ISP_STAT_INFO_S **ppstStatInfo)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    struct list_head* plist;
    ISP_STAT_NODE_S *pstNode = HI_NULL;
    unsigned long u32Flags;

    ISP_CHECK_DEV(IspDev);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

    spin_lock_irqsave(&g_stIspLock, u32Flags);
    if (list_empty(&pstDrvCtx->stStatisticsBuf.stBusyList))
    {
        ISP_TRACE(HI_DBG_WARN, "busy list empty\n");
        *ppstStatInfo = HI_NULL;
        spin_unlock_irqrestore(&g_stIspLock, u32Flags);
        return HI_FAILURE;
    }

    /* get busy */
    plist = pstDrvCtx->stStatisticsBuf.stBusyList.next;
    HI_ASSERT(plist != HI_NULL);
    list_del(plist);
    pstDrvCtx->stStatisticsBuf.u32BusyNum--;

    /* return info */
    pstNode = list_entry(plist, ISP_STAT_NODE_S, list);
    *ppstStatInfo = &pstNode->stStatInfo;

    /* put user */
    list_add_tail(plist, &pstDrvCtx->stStatisticsBuf.stUserList);
    pstDrvCtx->stStatisticsBuf.u32UserNum++;
    spin_unlock_irqrestore(&g_stIspLock, u32Flags);

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_StatBufUserPut(ISP_DEV IspDev, ISP_STAT_INFO_S *pstStatInfo)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    struct list_head* plist;
    ISP_STAT_NODE_S *pstNode = HI_NULL;
    HI_BOOL bValid = HI_FALSE;
    unsigned long u32Flags;

    ISP_CHECK_DEV(IspDev);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

    spin_lock_irqsave(&g_stIspLock, u32Flags);
    list_for_each(plist, &pstDrvCtx->stStatisticsBuf.stUserList)
    {
        pstNode = list_entry(plist, ISP_STAT_NODE_S, list);
        HI_ASSERT(HI_NULL != pstNode);
        if (pstNode->stStatInfo.u32PhyAddr == pstStatInfo->u32PhyAddr)
        {
            bValid = HI_TRUE;
            pstNode->stStatInfo.unKey.u32Key = pstStatInfo->unKey.u32Key;
            break;
        }
    }
    
    if (!bValid)
    {
        ISP_TRACE(HI_DBG_ERR, "invalid stat info, phy 0x%x\n", pstStatInfo->u32PhyAddr);
        spin_unlock_irqrestore(&g_stIspLock, u32Flags);
        return HI_FAILURE;
    }

    /* get user */
    HI_ASSERT(plist != HI_NULL);
    list_del(plist);
    pstDrvCtx->stStatisticsBuf.u32UserNum--;

    /* put free */
    list_add_tail(plist, &pstDrvCtx->stStatisticsBuf.stFreeList);
    pstDrvCtx->stStatisticsBuf.u32FreeNum++;
    spin_unlock_irqrestore(&g_stIspLock, u32Flags);

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_StatBufBusyPut(ISP_DEV IspDev)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    struct list_head* plist;
    ISP_STAT_NODE_S *pstNode = HI_NULL;

    ISP_CHECK_DEV(IspDev);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

    /* There should be one frame of the newest statistics info in busy list. */
    while (!list_empty(&pstDrvCtx->stStatisticsBuf.stBusyList))
    {
        plist = pstDrvCtx->stStatisticsBuf.stBusyList.next;
        HI_ASSERT(plist != HI_NULL);
        list_del(plist);
        pstDrvCtx->stStatisticsBuf.u32BusyNum--;

        list_add_tail(plist, &pstDrvCtx->stStatisticsBuf.stFreeList);
        pstDrvCtx->stStatisticsBuf.u32FreeNum++;
    }
    
    if (list_empty(&pstDrvCtx->stStatisticsBuf.stFreeList))
    {
        ISP_TRACE(HI_DBG_WARN, "free list empty\n");
        return HI_FAILURE;
    }

    /* get free */
    plist = pstDrvCtx->stStatisticsBuf.stFreeList.next;
    HI_ASSERT(plist != HI_NULL);
    list_del(plist);
    pstDrvCtx->stStatisticsBuf.u32FreeNum--;

    /* read statistics */
    pstNode = list_entry(plist, ISP_STAT_NODE_S, list);

    //pstNode->stStatInfo.unKey.u32Key = 0xffff;
    ISP_DRV_StatisticsRead(&pstNode->stStatInfo);    

    /* put busy */
    list_add_tail(plist, &pstDrvCtx->stStatisticsBuf.stBusyList);
    pstDrvCtx->stStatisticsBuf.u32BusyNum++;

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_RegConfig(ISP_REG_CFG_S *pstRegCfgInfo)
{
    HI_S32 i, j;
	HI_U32 u32CombinWeight = 0;
	HI_U32 u32CombinWeightNum = 0;

	if (pstRegCfgInfo->stKernelCfg.unKey.bit1UvnrCfg)
	{
		hi_isp_uvnr_sel_write(pstRegCfgInfo->stKernelCfg.stUVNrRegCfg.bUvnrSel);
		hi_isp_nr_en_write(pstRegCfgInfo->stKernelCfg.stUVNrRegCfg.bNrEn);
		hi_isp_midf_en_write(pstRegCfgInfo->stKernelCfg.stUVNrRegCfg.bMidfEn);
		hi_isp_uv2c_mode_write(pstRegCfgInfo->stKernelCfg.stUVNrRegCfg.bUv2cMode);
		hi_isp_uv2c_en_write(pstRegCfgInfo->stKernelCfg.stUVNrRegCfg.bUv2cEn);
		
		hi_isp_vcds_en_write(pstRegCfgInfo->stKernelCfg.stUVNrRegCfg.bVcdsEn);
		hi_isp_hcds_en_write(pstRegCfgInfo->stKernelCfg.stUVNrRegCfg.bHcdsEn);
		hi_isp_hcds_width_out_write(pstRegCfgInfo->stKernelCfg.stUVNrRegCfg.s32WidthOut);
		hi_isp_hcds_width_in_write(pstRegCfgInfo->stKernelCfg.stUVNrRegCfg.s32WidthIn);
		hi_isp_vcus_mode_write(pstRegCfgInfo->stKernelCfg.stUVNrRegCfg.bVcusMode);
		hi_isp_vcds_mode_write(pstRegCfgInfo->stKernelCfg.stUVNrRegCfg.bVcdsMode);
		hi_isp_hcds_mode_write(pstRegCfgInfo->stKernelCfg.stUVNrRegCfg.bHcdsMode);
	
		hi_isp_hcds_coef0_write(pstRegCfgInfo->stKernelCfg.stUVNrRegCfg.s32HCoef0);
		hi_isp_hcds_coef1_write(pstRegCfgInfo->stKernelCfg.stUVNrRegCfg.s32HCoef1);
		hi_isp_hcds_coef2_write(pstRegCfgInfo->stKernelCfg.stUVNrRegCfg.s32HCoef2);
		hi_isp_hcds_coef3_write(pstRegCfgInfo->stKernelCfg.stUVNrRegCfg.s32HCoef3);
		hi_isp_hcds_coef4_write(pstRegCfgInfo->stKernelCfg.stUVNrRegCfg.s32HCoef4);
		hi_isp_hcds_coef5_write(pstRegCfgInfo->stKernelCfg.stUVNrRegCfg.s32HCoef5);
		hi_isp_hcds_coef6_write(pstRegCfgInfo->stKernelCfg.stUVNrRegCfg.s32HCoef6);
		hi_isp_hcds_coef7_write(pstRegCfgInfo->stKernelCfg.stUVNrRegCfg.s32HCoef7);
		hi_isp_vcds_coef0_write(pstRegCfgInfo->stKernelCfg.stUVNrRegCfg.s32VCoef0);
		hi_isp_vcds_coef1_write(pstRegCfgInfo->stKernelCfg.stUVNrRegCfg.s32VCoef1);
		hi_isp_vcds_coef2_write(pstRegCfgInfo->stKernelCfg.stUVNrRegCfg.s32VCoef2);
		hi_isp_vcds_coef3_write(pstRegCfgInfo->stKernelCfg.stUVNrRegCfg.s32VCoef3);
		
		hi_isp_midfilter_width_write(pstRegCfgInfo->stKernelCfg.stUVNrRegCfg.s32MfWidth);
		hi_isp_midfilter_height_write(pstRegCfgInfo->stKernelCfg.stUVNrRegCfg.s32MfHeight);
		hi_isp_uvnr_width_write(pstRegCfgInfo->stKernelCfg.stUVNrRegCfg.s32UvnrWidth);
		hi_isp_uvnr_height_write(pstRegCfgInfo->stKernelCfg.stUVNrRegCfg.s32UvnrHeight);
		hi_isp_uvnr_diffthres1_write(pstRegCfgInfo->stKernelCfg.stUVNrRegCfg.s32Diffthres1);
		hi_isp_uvnr_diffthres2_write(pstRegCfgInfo->stKernelCfg.stUVNrRegCfg.s32Diffthres2);
		hi_isp_uvnr_diffthres3_write(pstRegCfgInfo->stKernelCfg.stUVNrRegCfg.s32Diffthres3);
		hi_isp_uvnr_diffthres4_write(pstRegCfgInfo->stKernelCfg.stUVNrRegCfg.s32Diffthres4);
		hi_isp_uvnr_diffthres5_write(pstRegCfgInfo->stKernelCfg.stUVNrRegCfg.s32Diffthres5);
		hi_isp_uvnr_diffthres6_write(pstRegCfgInfo->stKernelCfg.stUVNrRegCfg.s32Diffthres6);
		hi_isp_uvnr_ratiolimit0_write(pstRegCfgInfo->stKernelCfg.stUVNrRegCfg.s32Ratiolimit0);
		hi_isp_uvnr_ratiolimit1_write(pstRegCfgInfo->stKernelCfg.stUVNrRegCfg.s32Ratiolimit1);
		hi_isp_uvnr_ratiolimit2_write(pstRegCfgInfo->stKernelCfg.stUVNrRegCfg.s32Ratiolimit2);
		hi_isp_uvnr_ratiolimit3_write(pstRegCfgInfo->stKernelCfg.stUVNrRegCfg.s32ratiolimit3);
		hi_isp_uvnr_ratiolimit4_write(pstRegCfgInfo->stKernelCfg.stUVNrRegCfg.s32Ratiolimit4);
		hi_isp_uvnr_ratiolimit5_write(pstRegCfgInfo->stKernelCfg.stUVNrRegCfg.s32Ratiolimit5);
		hi_isp_uvnr_ratiolimit6_write(pstRegCfgInfo->stKernelCfg.stUVNrRegCfg.s32Ratiolimit6);
		hi_isp_uvnr_blendratio_write(pstRegCfgInfo->stKernelCfg.stUVNrRegCfg.s32Blendratio);
		hi_isp_uvnr_diffshift_write(pstRegCfgInfo->stKernelCfg.stUVNrRegCfg.s32Diffshift);
		hi_isp_uvnr_coring_limit_write(pstRegCfgInfo->stKernelCfg.stUVNrRegCfg.u8CoringLimit);

		pstRegCfgInfo->stKernelCfg.unKey.bit1UvnrCfg = 0;

		return HI_SUCCESS;
    }
	
    if (pstRegCfgInfo->unKey.bit1AeCfg1)
    {
    	/*added by qlp*/
		hi_isp_ae_mem_wei_waddr_write(0);
		
	    for (i=0; i<15; i++)
	    {
	        for (j=0; j<17; j++)
	        {
	        	u32CombinWeight |= (pstRegCfgInfo->stAeRegCfg1.au8WeightTable[i][j] << (8*u32CombinWeightNum));
				u32CombinWeightNum++;

				/*四个权重值组合成一个32位权重值*/
				if (u32CombinWeightNum == HI_ISP_AE_WEI_COMBIN_COUNT)
				{					
					hi_isp_ae_mem_wei_wdata_write(u32CombinWeight);
					u32CombinWeightNum = 0;
					u32CombinWeight = 0;
				}			
	        }
	    }
		
		if (u32CombinWeightNum != HI_ISP_AE_WEI_COMBIN_COUNT
			&&u32CombinWeightNum != 0)
		{
			hi_isp_ae_mem_wei_wdata_write(u32CombinWeight);			
		}
		//printk("####u32WriteNum == %d####\n", u32WriteNum);
    }

    if (pstRegCfgInfo->unKey.bit1AwbCfg1)
    {
#if 1  
        hi_isp_matrix_coefft_r_r_write(CCM_CONVERT(CCM_CONVERT_PRE(pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[0])));
        hi_isp_matrix_coefft_r_g_write(CCM_CONVERT(CCM_CONVERT_PRE(pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[1])));
        hi_isp_matrix_coefft_r_b_write(CCM_CONVERT(CCM_CONVERT_PRE(pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[2])));
        hi_isp_matrix_coefft_g_r_write(CCM_CONVERT(CCM_CONVERT_PRE(pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[3])));
        hi_isp_matrix_coefft_g_g_write(CCM_CONVERT(CCM_CONVERT_PRE(pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[4])));
        hi_isp_matrix_coefft_g_b_write(CCM_CONVERT(CCM_CONVERT_PRE(pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[5])));
        hi_isp_matrix_coefft_b_r_write(CCM_CONVERT(CCM_CONVERT_PRE(pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[6])));
        hi_isp_matrix_coefft_b_g_write(CCM_CONVERT(CCM_CONVERT_PRE(pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[7])));
        hi_isp_matrix_coefft_b_b_write(CCM_CONVERT(CCM_CONVERT_PRE(pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[8])));
#endif		

#if 0    
        hi_isp_matrix_coefft_r_r_write(pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[0]);
        hi_isp_matrix_coefft_r_g_write(pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[1]);
        hi_isp_matrix_coefft_r_b_write(pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[2]);
        hi_isp_matrix_coefft_g_r_write(pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[3]);
        hi_isp_matrix_coefft_g_g_write(pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[4]);
        hi_isp_matrix_coefft_g_b_write(pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[5]);
        hi_isp_matrix_coefft_b_r_write(pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[6]);
        hi_isp_matrix_coefft_b_g_write(pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[7]);
        hi_isp_matrix_coefft_b_b_write(pstRegCfgInfo->stAwbRegCfg1.au16ColorMatrix[8]);
#endif	

#if 1
        hi_isp_white_balance_gain_00_write(pstRegCfgInfo->stAwbRegCfg1.au32WhiteBalanceGain[0]);
        hi_isp_white_balance_gain_01_write(pstRegCfgInfo->stAwbRegCfg1.au32WhiteBalanceGain[1]);
        hi_isp_white_balance_gain_10_write(pstRegCfgInfo->stAwbRegCfg1.au32WhiteBalanceGain[2]);
        hi_isp_white_balance_gain_11_write(pstRegCfgInfo->stAwbRegCfg1.au32WhiteBalanceGain[3]);
#endif
    }

    if (pstRegCfgInfo->unKey.bit1AwbCfg3)
    {
        hi_isp_metering_max_clip_write(pstRegCfgInfo->stAwbRegCfg3.bAboveWhiteLevelClip);
        hi_isp_metering_min_clip_write(pstRegCfgInfo->stAwbRegCfg3.bBelowBlackLevelClip);
        hi_isp_metering_cr_ref_max_awb_sum_write(pstRegCfgInfo->stAwbRegCfg3.u16MeteringCrRefMaxAwb);
        hi_isp_metering_cr_ref_min_awb_sum_write(pstRegCfgInfo->stAwbRegCfg3.u16MeteringCrRefMinAwb);
        hi_isp_metering_cb_ref_max_awb_sum_write(pstRegCfgInfo->stAwbRegCfg3.u16MeteringCbRefMaxAwb);
        hi_isp_metering_cb_ref_min_awb_sum_write(pstRegCfgInfo->stAwbRegCfg3.u16MeteringCbRefMinAwb);
        hi_isp_metering_max_threshold_write(pstRegCfgInfo->stAwbRegCfg3.u16MeteringWhiteLevelAwb);
        hi_isp_metering_min_threshold_write(pstRegCfgInfo->stAwbRegCfg3.u16MeteringBlackLevelAwb);
        hi_isp_awb_top_hypotenuse_k_write(-4096);
        hi_isp_awb_top_hypotenuse_b_write(0x400);
        hi_isp_awb_bot_hypotenuse_k_write(-4096);
        hi_isp_awb_bot_hypotenuse_b_write(0x40);
    }
    
    if (pstRegCfgInfo->unKey.bit1AfCfg)
    {
        hi_isp_metering_af_threshold_write_write(pstRegCfgInfo->stAfRegCfg.u16ThresholdWrite);
        hi_isp_metering_af_metrics_shift_write(pstRegCfgInfo->stAfRegCfg.u8MetricsShift); 
        hi_isp_metering_af_np_offset_write(pstRegCfgInfo->stAfRegCfg.u8NpOffset);
    }

    if (pstRegCfgInfo->unKey.bit1OffsetCfg)
    {
        /* config in isp_black_level.c */
    }

    if (pstRegCfgInfo->unKey.bit1DrcCfg)
    {
		//hi_isp_drc_exposure_write(pstRegCfgInfo->stDrcRegCfg.FrameExposure);
    }

    if (pstRegCfgInfo->unKey.bit1NrCfg)
    {
    	hi_isp_2dnr_luma_nr_enable_write(pstRegCfgInfo->stNrRegCfg.lumaNREnable);
		hi_isp_2dnr_luma_gain_write(pstRegCfgInfo->stNrRegCfg.NrLumaNrGain);
    	hi_isp_2dnr_fcr_en_write(pstRegCfgInfo->stNrRegCfg.falseColorEnable);
		hi_isp_2dnr_fcrshift_write(pstRegCfgInfo->stNrRegCfg.falseColorShift);
		hi_isp_2dnr_fcrgain_write(pstRegCfgInfo->stNrRegCfg.falseColorGain);
		hi_isp_2dnr_fcrlimit_write(pstRegCfgInfo->stNrRegCfg.falseColorLimit);

		hi_isp_2dnr_luma_low_write(pstRegCfgInfo->stNrRegCfg.rangeLumaLow);
		hi_isp_2dnr_luma_high_write(pstRegCfgInfo->stNrRegCfg.rangeLumaHig);

		hi_isp_2dnr_limit_low_write(pstRegCfgInfo->stNrRegCfg.blendLowLimit);
		hi_isp_2dnr_limit_high_write(pstRegCfgInfo->stNrRegCfg.blendHigLimit);

		for (i=0; i<17; i++)
		{
			hi_isp_2dnr_thres_low_write(i, pstRegCfgInfo->stNrRegCfg.rangeLowLUT[i]);
			hi_isp_2dnr_thres_high_write(i, pstRegCfgInfo->stNrRegCfg.rangeHigLUT[i]);
		}
    }

    if (pstRegCfgInfo->unKey.bit1SharpenCfg)
    {        
    	//modify by qlp
#if 1    	
		hi_isp_edge_amt_write(pstRegCfgInfo->stSharpenRegCfg.u16EdgeAmt);
		hi_isp_sharp_amt_write(pstRegCfgInfo->stSharpenRegCfg.u16SharpAmt1);
		hi_isp_sharpen_enpixsel_write(pstRegCfgInfo->stSharpenRegCfg.enPixSel); 
	    hi_isp_sharp_thd1_write(pstRegCfgInfo->stSharpenRegCfg.u16SharpThd1); 
		hi_isp_sharp_thd2_write(pstRegCfgInfo->stSharpenRegCfg.u16SharpThd2); 
	    hi_isp_sharp_coef_write(pstRegCfgInfo->stSharpenRegCfg.u16SharpThd1MulCoef);
	    hi_isp_edge_thd1_write(pstRegCfgInfo->stSharpenRegCfg.u16EdgeThd1); 
		hi_isp_edge_thd2_write(pstRegCfgInfo->stSharpenRegCfg.u16EdgeThd2);
	    hi_isp_edge_coef_write(pstRegCfgInfo->stSharpenRegCfg.u16EdgeThd1MulCoef); 
		hi_isp_sharpen_over_thd1_write(pstRegCfgInfo->stSharpenRegCfg.u8OvershootThd1); 
		hi_isp_sharpen_over_thd2_write(pstRegCfgInfo->stSharpenRegCfg.u8OvershootThd2); 
		hi_isp_sharpen_under_thd1_write(pstRegCfgInfo->stSharpenRegCfg.u8UndershootThd1); 
		hi_isp_sharpen_under_thd2_write(pstRegCfgInfo->stSharpenRegCfg.u8UndershootThd2); 
	   	hi_isp_over_amt_write(pstRegCfgInfo->stSharpenRegCfg.u8OvershootAmt);   
	    hi_isp_under_amt_write(pstRegCfgInfo->stSharpenRegCfg.u8UndershootAmt);
		hi_isp_sharpen_line_thd1_write(pstRegCfgInfo->stSharpenRegCfg.u8LineThd1);
		hi_isp_sharpen_line_thd2_write(pstRegCfgInfo->stSharpenRegCfg.u8LineThd2);
		hi_isp_sharpen_under_coef_write(pstRegCfgInfo->stSharpenRegCfg.u16UndershootThdMulCoef);
		hi_isp_sharpen_over_coef_write(pstRegCfgInfo->stSharpenRegCfg.u16OvershootThdMulCoef);


		hi_isp_sharpen_mid_tmp00_write(pstRegCfgInfo->stSharpenRegCfg.G3[0][0]);
		hi_isp_sharpen_mid_tmp01_write(pstRegCfgInfo->stSharpenRegCfg.G3[0][1]);
		hi_isp_sharpen_mid_tmp02_write(pstRegCfgInfo->stSharpenRegCfg.G3[0][2]);
		hi_isp_sharpen_mid_tmp10_write(pstRegCfgInfo->stSharpenRegCfg.G3[1][0]);
		hi_isp_sharpen_mid_tmp11_write(pstRegCfgInfo->stSharpenRegCfg.G3[1][1]);
		hi_isp_sharpen_mid_tmp12_write(pstRegCfgInfo->stSharpenRegCfg.G3[1][2]);
		hi_isp_sharpen_mid_tmp20_write(pstRegCfgInfo->stSharpenRegCfg.G3[2][0]);
		hi_isp_sharpen_mid_tmp21_write(pstRegCfgInfo->stSharpenRegCfg.G3[2][1]);
		hi_isp_sharpen_mid_tmp22_write(pstRegCfgInfo->stSharpenRegCfg.G3[2][2]);
#endif		
    }

    if (pstRegCfgInfo->unKey.bit1GeCfg)
    {
        //hi_isp_green_equalize_ge_strength_write(pstRegCfgInfo->stGeRegCfg.u8GeStrength);
        hi_isp_ge_enable_write(pstRegCfgInfo->stGeRegCfg.ge_enable);
        hi_isp_ge_ct_th1_write(pstRegCfgInfo->stGeRegCfg.ct_th1);
		hi_isp_ge_ct_slope1_write(pstRegCfgInfo->stGeRegCfg.detail_slop);
		hi_isp_ge_ct_th2_write(pstRegCfgInfo->stGeRegCfg.detail_th);
		hi_isp_ge_ct_th3_write(pstRegCfgInfo->stGeRegCfg.ge_th);
		hi_isp_ge_ct_slope2_write(pstRegCfgInfo->stGeRegCfg.ge_th_slop);
    }

    if (pstRegCfgInfo->unKey.bit1DpCfg)
    {
    	    hi_isp_dp_in_soft_rst_write(pstRegCfgInfo->stDpRegCfg.u32DpccInSoftRst);
		    //pstRegCfgInfo->stDpRegCfg.isp_dpcc_regs_bayer_pat
            hi_isp_dp_bpt_ctrl_write(pstRegCfgInfo->stDpRegCfg.u32DpccBptCtrl);
            //hi_isp_dpc_enable_write(pstRegCfgInfo->stDpRegCfg.isp_dpcc_cfg);
		    hi_isp_dp_mode_write(pstRegCfgInfo->stDpRegCfg.u32DpccMode);
		    hi_isp_dp_output_mode_write(pstRegCfgInfo->stDpRegCfg.u32DpccOutputMode);
		    hi_isp_dp_set_use_write(pstRegCfgInfo->stDpRegCfg.u32DpccSetUse);
		    hi_isp_dp_methods_set_1_write(pstRegCfgInfo->stDpRegCfg.u32DpccMethodsSet1);
		    hi_isp_dp_methods_set_2_write(pstRegCfgInfo->stDpRegCfg.u32DpccMethodsSet2);
		    hi_isp_dp_methods_set_3_write(pstRegCfgInfo->stDpRegCfg.u32DpccMethodsSet3);
		    hi_isp_dp_line_thresh_1_write(pstRegCfgInfo->stDpRegCfg.u32DpccLineThresh1);
		    hi_isp_dp_line_mad_fac_1_write(pstRegCfgInfo->stDpRegCfg.u32DpccLineMadFac1);
		    hi_isp_dp_pg_fac_1_write(pstRegCfgInfo->stDpRegCfg.u32DpccPgFac1);
		    hi_isp_dp_rnd_thresh_1_write(pstRegCfgInfo->stDpRegCfg.u32DpccRndThresh1);
		    hi_isp_dp_rg_fac_1_write(pstRegCfgInfo->stDpRegCfg.u32DpccRgFac1);                         
		    hi_isp_dp_line_thresh_2_write(pstRegCfgInfo->stDpRegCfg.u32DpccLineThresh2);                         
		    hi_isp_dp_line_mad_fac_2_write(pstRegCfgInfo->stDpRegCfg.u32DpccLineMadFac2);                    
		    hi_isp_dp_pg_fac_2_write(pstRegCfgInfo->stDpRegCfg.u32DpccPgFac2);                          
		    hi_isp_dp_rnd_thresh_2_write(pstRegCfgInfo->stDpRegCfg.u32DpccRndThresh2);                          
		    hi_isp_dp_rg_fac_2_write(pstRegCfgInfo->stDpRegCfg.u32DpccRgFac2);                           
		    hi_isp_dp_line_thresh_3_write(pstRegCfgInfo->stDpRegCfg.u32DpccLineThresh3);                          
		    hi_isp_dp_line_mad_fac_3_write(pstRegCfgInfo->stDpRegCfg.u32DpccLineMadFac3);                           
		    hi_isp_dp_pg_fac_3_write(pstRegCfgInfo->stDpRegCfg.u32DpccPgFac3);                          
		    hi_isp_dp_rnd_thresh_3_write(pstRegCfgInfo->stDpRegCfg.u32DpccRndThresh3);    
			hi_isp_dp_rg_fac_3_write(pstRegCfgInfo->stDpRegCfg.u32DpccRgFac3 );				
		    hi_isp_dp_ro_limits_write(pstRegCfgInfo->stDpRegCfg.u32DpccRoLimits);                           
		    hi_isp_dp_rnd_offs_write(pstRegCfgInfo->stDpRegCfg.u32DpccRndOffs);
            //pstRegCfgInfo->stDpRegCfg.isp_dpcc_bad_thresh
#if 0    
        hi_isp_defect_pixel_enable_write(pstRegCfgInfo->stDpRegCfg.u8DpEnable);
        hi_isp_green_equalize_dp_enable_write(pstRegCfgInfo->stDpRegCfg.u8GeDpEnable);
        hi_isp_defect_pixel_pointer_reset_write(pstRegCfgInfo->stDpRegCfg.u8DpPointerReset);
        hi_isp_green_equalize_dp_threshold_write(pstRegCfgInfo->stDpRegCfg.u8BadPixelThreshold);
        hi_isp_defect_pixel_detection_trigger_write(pstRegCfgInfo->stDpRegCfg.u8DpDetectionTrigger);
        hi_isp_defect_pixel_defect_pixel_count_in_write(pstRegCfgInfo->stDpRegCfg.u16DpCountIn);
        hi_isp_defect_pixel_show_static_defect_pixels_write(pstRegCfgInfo->stDpRegCfg.u8ShowStaticDefectPixels);
        hi_isp_green_equalize_dp_slope_write(pstRegCfgInfo->stDpRegCfg.u16DynamicDpSlope);
        //hi_isp_green_equalize_dp_threshold_write(pstRegCfgInfo->stDpRegCfg.u16DynamicDPThresh);
#endif        
    }

  	if (pstRegCfgInfo->unKey.bit1DehazeCfg)
    {
        
        if (pstRegCfgInfo->stDehazeRegCfg.u8DehazeEnable)
        {
            //hi_vi_dehaze_air_write(pstRegCfgInfo->stDehazeRegCfg.u32DehazeAir);
            hi_isp_dehaze_air_r_write(pstRegCfgInfo->stDehazeRegCfg.u8AirR);
			hi_isp_dehaze_air_g_write(pstRegCfgInfo->stDehazeRegCfg.u8AirG);
			hi_isp_dehaze_air_b_write(pstRegCfgInfo->stDehazeRegCfg.u8AirR);
            #if 0
            HI_U32 u32BlkNum = pstRegCfgInfo->stDehazeRegCfg.u32BlkNum;
            for(i=0;i<u32BlkNum;i++)
            {
                hi_vi_dehaze_prev_dc_write(i,pstRegCfgInfo->stDehazeRegCfg.au32DehazePrevDc[i]);          
            }
            
            if(pstRegCfgInfo->stDehazeRegCfg.u8DehazeLutChange==1)
            {
                for(i=0;i<256;i++)//256
                {
                    hi_vi_dehaze_lut_write(i,pstRegCfgInfo->stDehazeRegCfg.au32DehazeLut[i]);                                   
                }      
            }
            #endif
        }
        
         //hi_vi_dehaze_gstrth_write(pstRegCfgInfo->stDehazeRegCfg.u8Strength);
         //hi_vi_dehaze_update_write(pstRegCfgInfo->stDehazeRegCfg.u32Update);
         hi_isp_dehaze_gstrth_write(pstRegCfgInfo->stDehazeRegCfg.u8Strength);
		 hi_isp_dehaze_pre_update_write(pstRegCfgInfo->stDehazeRegCfg.u32Update);
    }
    
    if (pstRegCfgInfo->unKey.bit1WdrCfg)
    {
        //hi_vi_top_channel_switch_write(pstRegCfgInfo->stWdrRegCfg.u8TopChannelSwitch);
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_SwitchMode(ISP_DRV_CTX_S *pstDrvCtx)
{
    ISP_SYNC_CFG_S      *pstSyncCfg;
    HI_U32  u32PTIntMask = 0;
    HI_BOOL bChnSwitchEnable = HI_FALSE;
    HI_U32  au32ChnSwitch[4] = {0};

    pstSyncCfg = &pstDrvCtx->stSyncCfg;

    pstSyncCfg->u8WDRMode = pstDrvCtx->stWDRCfg.u8WDRMode;

    /* init cfg when modes change */
    //if (pstSyncCfg->u8WDRMode != pstSyncCfg->u8PreWDRMode)
    {      
        memset(&pstDrvCtx->stSyncCfg.stSyncCfgBuf, 0, sizeof(ISP_SYNC_CFG_BUF_S));
        memset(&pstDrvCtx->stSyncCfg.apstNode, 0, sizeof(pstDrvCtx->stSyncCfg.apstNode));

        pstSyncCfg->u8VCNum = 0;
        pstSyncCfg->u8VCCfgNum = 0;
        pstSyncCfg->u8Cfg2VldDlyMAX = 1;

        /* get N (N to 1 WDR) */
        switch(pstSyncCfg->u8WDRMode)
        {
            default:
            case WDR_MODE_NONE:
            case WDR_MODE_BUILT_IN:
            case WDR_MODE_2To1_LINE:
            case WDR_MODE_3To1_LINE:
            case WDR_MODE_4To1_LINE:
                pstSyncCfg->u8VCNumMax = 0;
                break;
            case WDR_MODE_2To1_FRAME :
            case WDR_MODE_2To1_FRAME_FULL_RATE:
                pstSyncCfg->u8VCNumMax = 1;
                break;
            case WDR_MODE_3To1_FRAME :
            case WDR_MODE_3To1_FRAME_FULL_RATE:
                pstSyncCfg->u8VCNumMax = 2;
                break;
            case WDR_MODE_4To1_FRAME :
            case WDR_MODE_4To1_FRAME_FULL_RATE: 
                pstSyncCfg->u8VCNumMax = 3;
                break;
        }

        /* Channel Switch config */
        if (IS_FULL_WDR_MODE(pstSyncCfg->u8WDRMode))
        {
            au32ChnSwitch[0] = HI_VI_TOP_CHN_SWITCH0_DEFAULT;
            au32ChnSwitch[1] = HI_VI_TOP_CHN_SWITCH1_DEFAULT;
            au32ChnSwitch[2] = HI_VI_TOP_CHN_SWITCH2_DEFAULT;
            au32ChnSwitch[3] = HI_VI_TOP_CHN_SWITCH3_DEFAULT;
            bChnSwitchEnable = HI_TRUE;
        }
        else if (IS_HALF_WDR_MODE(pstSyncCfg->u8WDRMode))
        {
            au32ChnSwitch[0] = 1 % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1);
            au32ChnSwitch[1] = (au32ChnSwitch[0] + 1) % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1);
            au32ChnSwitch[2] = (au32ChnSwitch[0] + 2) % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1);
            au32ChnSwitch[3] = (au32ChnSwitch[0] + 3) % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1);
            bChnSwitchEnable = HI_TRUE;
        }
        else
        {
            au32ChnSwitch[0] = HI_VI_TOP_CHN_SWITCH0_DEFAULT;
            au32ChnSwitch[1] = HI_VI_TOP_CHN_SWITCH1_DEFAULT;
            au32ChnSwitch[2] = HI_VI_TOP_CHN_SWITCH2_DEFAULT;
            au32ChnSwitch[3] = HI_VI_TOP_CHN_SWITCH3_DEFAULT;
            bChnSwitchEnable = HI_FALSE;
        }
        hi_vi_top_chn_switch0_write(au32ChnSwitch[0]);
        hi_vi_top_chn_switch1_write(au32ChnSwitch[1]);
        hi_vi_top_chn_switch2_write(au32ChnSwitch[2]);
        hi_vi_top_chn_switch3_write(au32ChnSwitch[3]);
        hi_vi_top_chn_switch_enble_write(bChnSwitchEnable);

        /* VC config */
        if (IS_HALF_WDR_MODE(pstSyncCfg->u8WDRMode))
        {
            hi_vi_top_vc_enable_write(1);
            hi_vi_top_vc_max_write(pstSyncCfg->u8VCNumMax);
            hi_vi_top_vc_reset_write(1);
        }
        else
        {
            hi_vi_top_vc_enable_write(0);
            hi_vi_top_vc_max_write(0);
            //hi_vi_top_vc_reset_write(1);
        }

        /* pt_int_mask */
        u32PTIntMask = HW_REG(IO_ADDRESS_PORT(VI_PT0_INT_MASK));
        if (IS_FULL_WDR_MODE(pstSyncCfg->u8WDRMode))
        {
            //u32PTIntMask = u32PTIntMask | (HI_U32)VI_PT0_INT_FSTART;
            u32PTIntMask = VI_PT0_INT_FSTART;
        }
        else
        {
            //u32PTIntMask = u32PTIntMask & (~(HI_U32)VI_PT0_INT_FSTART);
            u32PTIntMask = 0;
        }
        HW_REG(IO_ADDRESS_PORT(VI_PT0_INT_MASK)) = u32PTIntMask;
    }

    pstSyncCfg->u8PreWDRMode = pstSyncCfg->u8WDRMode;

    return HI_SUCCESS;

}

#if 0
HI_S32 ISP_DRV_GetSyncCfgNode(ISP_SYNC_CFG_S *pstSyncCfg)
{
    HI_S32 i;
    ISP_SYNC_CFG_BUF_NODE_S *pstCurNode = HI_NULL;
    
    pstSyncCfg->u8Cfg2VldDlyMAX = 1;

    for (i=CFG2VLD_DLY_LIMIT; i>=1; i--)
    {
        pstSyncCfg->apstNode[i] = pstSyncCfg->apstNode[i-1];
    }
    ISPSyncBufRead(&pstSyncCfg->stSyncCfgBuf, &pstSyncCfg->apstNode[0]);

    pstCurNode = pstSyncCfg->apstNode[0];
    if (HI_NULL == pstCurNode)
    {
        return HI_SUCCESS;
    }
    if (HI_FALSE == pstCurNode->bValid)
    {
        return HI_SUCCESS;
    }

    pstSyncCfg->u8Cfg2VldDlyMAX = pstCurNode->stSnsRegsInfo.u8Cfg2ValidDelayMax;

    if ((pstSyncCfg->u8Cfg2VldDlyMAX > CFG2VLD_DLY_LIMIT) ||(pstSyncCfg->u8Cfg2VldDlyMAX < 1))
    {
        //ISP_TRACE(HI_DBG_ERR, "Delay of config to valid is:0x%x\n", pstSyncCfg->u8Cfg2VldDlyMAX);
        pstSyncCfg->u8Cfg2VldDlyMAX = 1;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_RegConfigSensor4Linear(ISP_DRV_CTX_S *pstDrvCtx)
{
    HI_S32 i;
    HI_U8 u8CurNodeIdx, u8PreNodeIdx;
    ISP_SYNC_CFG_BUF_NODE_S *pstCurNode = HI_NULL;
    ISP_SYNC_CFG_BUF_NODE_S *pstCfgNode = HI_NULL;
    ISP_I2C_DATA_S *pstI2cData = HI_NULL;
    //ISP_SSP_DATA_S *pstSspData = HI_NULL;
    
    pstCurNode = pstDrvCtx->stSyncCfg.apstNode[0];
    if (HI_NULL == pstCurNode)
    {
        return HI_SUCCESS;
    }
    if (HI_FALSE == pstCurNode->bValid)
    {
        return HI_SUCCESS;
    }

    for (i=0; i<pstCurNode->stSnsRegsInfo.u32RegNum; i++)
    {
        /* config i2c */
        if (ISP_SNS_I2C_TYPE == pstCurNode->stSnsRegsInfo.enSnsType)
        {
            u8CurNodeIdx = pstCurNode->stSnsRegsInfo.astI2cData[i].u8DelayFrmNum;
            u8PreNodeIdx = u8CurNodeIdx + 1;

            if (u8PreNodeIdx > CFG2VLD_DLY_LIMIT)
            {
                ISP_TRACE(HI_DBG_ERR, "DelayFrmNum error!\n");
                return HI_FAILURE;
            }

            pstCfgNode = pstDrvCtx->stSyncCfg.apstNode[u8CurNodeIdx];
            if (HI_NULL == pstCfgNode)
            {
                return HI_SUCCESS;
            }

            //if (HI_TRUE == pstCfgNode->bValid)
            if (HI_TRUE == pstCfgNode->bValid && (pstCfgNode != pstDrvCtx->stSyncCfg.apstNode[u8PreNodeIdx]))
            {
                pstI2cData = &pstCfgNode->stSnsRegsInfo.astI2cData[i];
                
                #ifdef HI_GPIO_I2C
                gpio_i2c_write_ex(pstI2cData->u8DevAddr, pstI2cData->u32RegAddr,
                    pstI2cData->u32AddrByteNum, pstI2cData->u32Data,
                    pstI2cData->u32DataByteNum);
                #else
                HI_I2C_Write(pstI2cData->u8DevAddr, pstI2cData->u32RegAddr,
                    pstI2cData->u32AddrByteNum, pstI2cData->u32Data,
                    pstI2cData->u32DataByteNum);
                #endif
            }
        }
        /* config ssp */
        else if (ISP_SNS_SSP_TYPE == pstCurNode->stSnsRegsInfo.enSnsType)
        {
        }
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_RegConfigIsp4Linear(ISP_DRV_CTX_S *pstDrvCtx)
{
    HI_S32 i;
    HI_U8  u8CurNodeIdx;
    HI_U32 u32IspDgain;  
    HI_U64 au64WbGain[4];
    ISP_AE_REG_CFG_2_S *pstAERegCfg = HI_NULL;
    ISP_SYNC_CFG_BUF_NODE_S *pstCfgNode = HI_NULL;
    ISP_REG_CFG_S *pstRegCfgInfo = &pstDrvCtx->stRegCfgInfo[pstDrvCtx->u32RegsInfoFlag];

    hi_vi_top_channel_switch_write(HI_VI_TOP_CHANNEL_SWITCH_DEFAULT);

    /* config isp dgain */
        /* delay of config2valid of isp reg is 1 */
    u8CurNodeIdx = (pstDrvCtx->stSyncCfg.u8Cfg2VldDlyMAX - 1);

    pstCfgNode = pstDrvCtx->stSyncCfg.apstNode[u8CurNodeIdx];
    
    if (NULL == pstCfgNode)
    {
        pstCfgNode = &pstDrvCtx->stSyncCfg.stSyncCfgBuf.stSyncCfgBufNode[0];
    }
    pstAERegCfg = &pstCfgNode->stAERegCfg;

    if (!pstRegCfgInfo->unKey.bit1AwbCfg1)
    {
        return HI_SUCCESS;
    }

    if (pstDrvCtx->stWDRCfg.u8WDRMode == WDR_MODE_NONE)
    {
        /*avoid overflow*/
        u32IspDgain = (pstAERegCfg->u32IspDgain > (4<<pstAERegCfg->u32IspDgainShift))
            ? (4<<pstAERegCfg->u32IspDgainShift) : pstAERegCfg->u32IspDgain;
     
        for (i=0; i<4; i++)
        {
            au64WbGain[i] = pstRegCfgInfo->stAwbRegCfg1.au32WhiteBalanceGain[i];

            au64WbGain[i] = (au64WbGain[i] * u32IspDgain) >> pstAERegCfg->u32IspDgainShift;            
            au64WbGain[i] = (au64WbGain[i] + 128) >> 8;
            /*avoid overflow*/
            au64WbGain[i] = (au64WbGain[i] > 0xFFF) ? 0xFFF : au64WbGain[i];
        }
    }
    else
    {
        /*avoid overflow*/
        u32IspDgain = (pstAERegCfg->u32IspDgain > (32<<pstAERegCfg->u32IspDgainShift))
            ? (32<<pstAERegCfg->u32IspDgainShift) : pstAERegCfg->u32IspDgain;
        for (i=0; i<4; i++)
        {
            au64WbGain[i] = (pstRegCfgInfo->stAwbRegCfg1.au32WhiteBalanceGain[i]
             * u32IspDgain) >> pstAERegCfg->u32IspDgainShift;
            /*avoid overflow*/
            au64WbGain[i] = (au64WbGain[i] > 0xFFE000) ? 0xFFE000 : au64WbGain[i];
            au64WbGain[i] = Sqrt32((HI_U32)au64WbGain[i]);
        }
    }

    hi_isp_white_balance_gain_00_write((HI_U16)au64WbGain[0]);
    hi_isp_white_balance_gain_01_write((HI_U16)au64WbGain[1]);
    hi_isp_white_balance_gain_10_write((HI_U16)au64WbGain[2]);
    hi_isp_white_balance_gain_11_write((HI_U16)au64WbGain[3]);

    return HI_SUCCESS;
}
#endif

HI_S32 ISP_DRV_GetSyncControlnfo(ISP_SYNC_CFG_S *pstSyncCfg)
{
    ISP_SYNC_CFG_BUF_NODE_S *pstCurNode = NULL;

    /* get VC number */
    // TODO: need VI logic to surport VC num
    //pstSyncCfg->u8VCNum = (++pstSyncCfg->u8VCNum) % DIV_0_TO_1(pstSyncCfg->u8VCNumMax + 1);
    pstSyncCfg->u8VCNum++;
    pstSyncCfg->u8VCNum = pstSyncCfg->u8VCNum % DIV_0_TO_1(pstSyncCfg->u8VCNumMax + 1);
    if (pstSyncCfg->u8VCNum > pstSyncCfg->u8VCNumMax)
    {
        printk("VC number err, can't be large than VC total!\n");
    }

    if (IS_HALF_WDR_MODE(pstSyncCfg->u8WDRMode))
    {
        if (pstSyncCfg->u8VCNum == pstSyncCfg->u8VCNumMax)
        {
            hi_vi_top_vc_reset_write(1);
        }
    }

    /* get Cfg2VldDlyMAX */
    if (!ISPSyncBufIsEmpty(&pstSyncCfg->stSyncCfgBuf))
    {
        pstCurNode = &pstSyncCfg->stSyncCfgBuf.stSyncCfgBufNode[pstSyncCfg->stSyncCfgBuf.u8BufRDFlag];
        if (pstCurNode != HI_NULL)
        {
            if (pstCurNode->bValid)
            {
                pstSyncCfg->u8Cfg2VldDlyMAX = pstCurNode->stSnsRegsInfo.u8Cfg2ValidDelayMax;
            }
        }
    }

    if ((pstSyncCfg->u8Cfg2VldDlyMAX > CFG2VLD_DLY_LIMIT) ||(pstSyncCfg->u8Cfg2VldDlyMAX < 1))
    {
        ISP_TRACE(HI_DBG_WARN, "Delay of config to valid is:0x%x\n", pstSyncCfg->u8Cfg2VldDlyMAX);
        pstSyncCfg->u8Cfg2VldDlyMAX = 1;
    }

    /* calc VCCfgNum */
    pstSyncCfg->u8VCCfgNum = (pstSyncCfg->u8VCNum + pstSyncCfg->u8Cfg2VldDlyMAX) % DIV_0_TO_1(pstSyncCfg->u8VCNumMax + 1);
    
    return HI_SUCCESS;
}

HI_S32 ISP_DRV_CalcSyncCfg(ISP_SYNC_CFG_S *pstSyncCfg)
{
    HI_U32 i,j;
    HI_U32 u32IntTime[4] = {0};
    ISP_SYNC_CFG_BUF_NODE_S *pstCurNode = HI_NULL;
    ISP_SYNC_CFG_BUF_NODE_S *pstPreNode = HI_NULL;
    HI_U64 u64CurExp = 0;
    HI_U64 u64PreExp = 0;

    /* update node when VCCfgNum is 0 */
    if (pstSyncCfg->u8VCCfgNum == 0)
    {
        for (i=CFG2VLD_DLY_LIMIT; i>=1; i--)
        {
            pstSyncCfg->apstNode[i] = pstSyncCfg->apstNode[i-1];
        }
        ISPSyncBufRead(&pstSyncCfg->stSyncCfgBuf, &pstSyncCfg->apstNode[0]);
    }

    pstCurNode = pstSyncCfg->apstNode[0];
    if (HI_NULL == pstCurNode)
    {
        return HI_SUCCESS;
    }
    if (HI_FALSE == pstCurNode->bValid)
    {
        return HI_SUCCESS;
    }

    if (HI_NULL == pstSyncCfg->apstNode[1])
    {
        pstPreNode = pstSyncCfg->apstNode[0];
    }
    else
    {
        if (HI_FALSE == pstSyncCfg->apstNode[1]->bValid)
        {
            pstPreNode = pstSyncCfg->apstNode[0];
        }
        else
        {
            pstPreNode = pstSyncCfg->apstNode[1];
        }
    }

    /* calculate exposure ratio */
    if (IS_FULL_WDR_MODE(pstSyncCfg->u8WDRMode))
    {
        switch(pstSyncCfg->u8VCCfgNum)
        {
            case 0:
                u32IntTime[0] = pstCurNode->stAERegCfg.u32IntTime[0];
                u32IntTime[1] = pstPreNode->stAERegCfg.u32IntTime[1];
                u32IntTime[2] = pstPreNode->stAERegCfg.u32IntTime[2];
                u32IntTime[3] = pstPreNode->stAERegCfg.u32IntTime[3];
                break;
            case 1:
                u32IntTime[0] = pstCurNode->stAERegCfg.u32IntTime[0];
                u32IntTime[1] = pstCurNode->stAERegCfg.u32IntTime[1];
                u32IntTime[2] = pstPreNode->stAERegCfg.u32IntTime[2];
                u32IntTime[3] = pstPreNode->stAERegCfg.u32IntTime[3];
                break;
            case 2:
                u32IntTime[0] = pstCurNode->stAERegCfg.u32IntTime[0];
                u32IntTime[1] = pstCurNode->stAERegCfg.u32IntTime[1];
                u32IntTime[2] = pstCurNode->stAERegCfg.u32IntTime[2];
                u32IntTime[3] = pstPreNode->stAERegCfg.u32IntTime[3];
                break;
            case 3:
                u32IntTime[0] = pstCurNode->stAERegCfg.u32IntTime[0];
                u32IntTime[1] = pstCurNode->stAERegCfg.u32IntTime[1];
                u32IntTime[2] = pstCurNode->stAERegCfg.u32IntTime[2];
                u32IntTime[3] = pstCurNode->stAERegCfg.u32IntTime[3];
                break;
        }
    }
    else
    {
        u32IntTime[0] = pstCurNode->stAERegCfg.u32IntTime[0];
        u32IntTime[1] = pstCurNode->stAERegCfg.u32IntTime[1];
        u32IntTime[2] = pstCurNode->stAERegCfg.u32IntTime[2];
        u32IntTime[3] = pstCurNode->stAERegCfg.u32IntTime[3];
    }

    for (j=0; j<3; j++)
    {
        for (i=CFG2VLD_DLY_LIMIT-1; i>=1; i--)
        {
            pstSyncCfg->u32ExpRatio[j][i] = pstSyncCfg->u32ExpRatio[j][i-1];
        }
        pstSyncCfg->u32ExpRatio[j][0] = (u32IntTime[j+1] << WDR_EXP_RATIO_SHIFT) / DIV_0_TO_1(u32IntTime[j]);
    }

    /* calculate DRC compensation */
    u64CurExp = pstCurNode->stAERegCfg.u64Exposure;
    u64PreExp = pstPreNode->stAERegCfg.u64Exposure;

    while (u64CurExp >= (1 << (31 - DRC_COMP_SHIFT)))
    {
        u64CurExp >>= 1;
        u64PreExp >>= 1;
    }

    for (i=CFG2VLD_DLY_LIMIT-1; i>=1; i--)
    {
        pstSyncCfg->u32DRCComp[i] = pstSyncCfg->u32DRCComp[i-1];
    }
    u64CurExp = u64CurExp << DRC_COMP_SHIFT; 
    do_div(u64CurExp, DIV_0_TO_1(u64PreExp));
    pstSyncCfg->u32DRCComp[0] = (HI_U32)u64CurExp;

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_RegConfigIsp(ISP_DRV_CTX_S *pstDrvCtx)
{
    HI_S32 i;
    HI_U8  u8CfgNodeIdx, u8CfgNodeVC;
    HI_U32 u32IspDgain;
    HI_BOOL bChnSwitchEnable = HI_FALSE;
    HI_U32  au32ChnSwitch[4] = {0};
    HI_U32  u32Ratio[3] = {0x40};
    HI_U32  u32DrcComp = 0x100;
    
    static ISP_AE_REG_CFG_2_S *pstAERegCfg = HI_NULL, *pstAERegCfg2 = HI_NULL;
    static ISP_SYNC_CFG_BUF_NODE_S *pstCfgNode = HI_NULL, *pstCfgNode2 = HI_NULL;
    //ISP_REG_CFG_S *pstRegCfgInfo = &pstDrvCtx->stRegCfgInfo[pstDrvCtx->u32RegsInfoFlag];
    
    /* config Chn Switch / Exp Ratio / ISP Dgain */
        /* delay of config2valid of isp reg is 1 */
    if (IS_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode))
    {
        if (IS_FULL_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode))
        {
            /* Channal Switch */
            au32ChnSwitch[0] = (pstDrvCtx->stSyncCfg.u8VCNumMax - pstDrvCtx->stSyncCfg.u8VCNum) % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1);
            au32ChnSwitch[1] = (au32ChnSwitch[0] + 1) % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1);
            au32ChnSwitch[2] = (au32ChnSwitch[0] + 2) % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1);
            au32ChnSwitch[3] = (au32ChnSwitch[0] + 3) % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1);
            bChnSwitchEnable = HI_TRUE;
            
            hi_vi_top_chn_switch0_write(au32ChnSwitch[0]);
            hi_vi_top_chn_switch1_write(au32ChnSwitch[1]);
            hi_vi_top_chn_switch2_write(au32ChnSwitch[2]);
            hi_vi_top_chn_switch3_write(au32ChnSwitch[3]);
            hi_vi_top_chn_switch_enble_write(bChnSwitchEnable);
        }

        /* Ratio */
        for (i=0; i<3; i++)
        {
            u32Ratio[i] = pstDrvCtx->stSyncCfg.u32ExpRatio[i][pstDrvCtx->stSyncCfg.u8Cfg2VldDlyMAX - 1];
        }

        /*  */
        u8CfgNodeIdx = ((pstDrvCtx->stSyncCfg.u8Cfg2VldDlyMAX - 1) / DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1));
        u8CfgNodeVC = ((pstDrvCtx->stSyncCfg.u8Cfg2VldDlyMAX - 1) % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1));
    }
    else
    {
        u8CfgNodeIdx = (pstDrvCtx->stSyncCfg.u8Cfg2VldDlyMAX - 1);
        u8CfgNodeVC = 0;
    }
    
    for (i=0; i<3; i++)
    {
        u32Ratio[i] = (u32Ratio[i] > EXP_RATIO_MAX) ? EXP_RATIO_MAX : u32Ratio[i];
        u32Ratio[i] = (u32Ratio[i] < EXP_RATIO_MIN) ? EXP_RATIO_MIN : u32Ratio[i];
    }

    hi_isp_wdr_exposure_ratio_write(u32Ratio[0]);

    /* config drc compensation */
    u32DrcComp = pstDrvCtx->stSyncCfg.u32DRCComp[pstDrvCtx->stSyncCfg.u8Cfg2VldDlyMAX - 1];
    if (IS_LINEAR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode))
    {
        /* do nothing */
    }
    else if (IS_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode))
    {
        u32DrcComp = Sqrt32(u32DrcComp << DRC_COMP_SHIFT);
    }
    else
    {
        /* unknow u8WDRMode */
    }
   
    u32DrcComp = MIN2(u32DrcComp, 0xFFFF);
    //hi_isp_drc_collection_correction_write(u32DrcComp);
    hi_isp_drc_exposure_write(u32DrcComp);
    

    /* config isp_dgain */
    pstCfgNode = pstDrvCtx->stSyncCfg.apstNode[u8CfgNodeIdx];
    if (HI_NULL == pstCfgNode)
    {
        return HI_SUCCESS;
    }

    if (pstDrvCtx->stSyncCfg.u8VCCfgNum == u8CfgNodeVC)
    {        
        pstAERegCfg = &pstCfgNode->stAERegCfg;
    }

    if (HI_NULL == pstAERegCfg)
    {
        return HI_SUCCESS;
    }

    u32IspDgain = pstAERegCfg->u32IspDgain;

    if (IS_LINEAR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode))
    {
        /* do nothing */
    }
    else if (IS_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode))
    {
        //u32IspDgain = Sqrt32(u32IspDgain << ISP_DIGITAL_GAIN_SHIFT);
    }
    else
    {
        /* unknow u8WDRMode */
    }   
    
    /* avoid overflow */
    u32IspDgain = CLIP3(u32IspDgain, ISP_DIGITAL_GAIN_MIN, ISP_DIGITAL_GAIN_MAX);
    //hi_isp_digital_gain_gain_write(u32IspDgain);
	hi_isp_dg_rgain_write(u32IspDgain);
	hi_isp_dg_bgain_write(u32IspDgain);
	hi_isp_dg_grgain_write(u32IspDgain);
	hi_isp_dg_gbgain_write(u32IspDgain);
    //printk("IspDgain:%x, shift:%x\n", pstAERegCfg->u32IspDgain, pstAERegCfg->u32IspDgainShift);

    
    pstCfgNode2 = pstDrvCtx->stSyncCfg.apstNode[0];
    if (HI_NULL == pstCfgNode2)
    {
        return HI_SUCCESS;
    }

    if (pstDrvCtx->stSyncCfg.u8VCCfgNum == u8CfgNodeVC)
    {        
        pstAERegCfg2 = &pstCfgNode->stAERegCfg;
    }

    if (HI_NULL == pstAERegCfg2)
    {
        return HI_SUCCESS;
    }

    if(pstAERegCfg2->bPirisValid == HI_TRUE)
    {
        if (HI_NULL != pstDrvCtx->stPirisCb.pfn_piris_gpio_update)
        {
            pstDrvCtx->stPirisCb.pfn_piris_gpio_update(&pstAERegCfg2->s32PirisPos);
        }
    }

    return HI_SUCCESS;

}

HI_S32 ISP_DRV_RegConfigSensor(ISP_DRV_CTX_S *pstDrvCtx)
{
    HI_S32 i = 0;
    HI_U8 u8CfgNodeIdx = 0, u8CfgNodeVC = 0;
    HI_U8 u8WDRMode = WDR_MODE_NONE;
    ISP_SYNC_CFG_BUF_NODE_S *pstCurNode = HI_NULL;
    ISP_SYNC_CFG_BUF_NODE_S *pstCfgNode = HI_NULL;
    ISP_SYNC_CFG_BUF_NODE_S *pstPreCfgNode = HI_NULL;
    ISP_I2C_DATA_S *pstI2cData = HI_NULL;
    ISP_SSP_DATA_S *pstSspData = HI_NULL;

    u8WDRMode = pstDrvCtx->stWDRCfg.u8WDRMode;
    pstCurNode = pstDrvCtx->stSyncCfg.apstNode[0];
    if (HI_NULL == pstCurNode)
    {
        ISP_TRACE(HI_DBG_WARN, "NULL point!\n");
        return HI_SUCCESS;
    }
    if (HI_FALSE == pstCurNode->bValid)
    {
        ISP_TRACE(HI_DBG_WARN, "Invalid Node!\n");
        return HI_SUCCESS;
    }

    if (0 == pstCurNode->stSnsRegsInfo.u32RegNum)
    {
        ISP_TRACE(HI_DBG_WARN, "Err u32RegNum!\n");
        return HI_SUCCESS;
    }

    for (i=0; i<pstCurNode->stSnsRegsInfo.u32RegNum; i++)
    {
        /* config i2c */
        if (ISP_SNS_I2C_TYPE == pstCurNode->stSnsRegsInfo.enSnsType)
        {
            u8CfgNodeIdx = (pstCurNode->stSnsRegsInfo.astI2cData[i].u8DelayFrmNum / DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1));
            u8CfgNodeVC = (pstCurNode->stSnsRegsInfo.astI2cData[i].u8DelayFrmNum % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1));
            if (pstDrvCtx->stSyncCfg.u8VCCfgNum == u8CfgNodeVC)
            {
                if (u8CfgNodeIdx > CFG2VLD_DLY_LIMIT-1)
                {
                    ISP_TRACE(HI_DBG_WARN, "DelayFrmNum error!\n");
                    return HI_FAILURE;
                }
                
                pstCfgNode    = pstDrvCtx->stSyncCfg.apstNode[u8CfgNodeIdx];
                pstPreCfgNode = pstDrvCtx->stSyncCfg.apstNode[u8CfgNodeIdx + 1];

                if (HI_NULL == pstCfgNode)
                {
                    continue;
                }

                if (HI_NULL == pstPreCfgNode)
                {
                }
                else
                {
                    if ((IS_LINEAR_MODE(u8WDRMode)) || (IS_BUILT_IN_WDR_MODE(u8WDRMode)))
                    {
                        /* not config sensor when cur == pre */
                        if (pstCfgNode == pstPreCfgNode)
                        {
                            continue;
                        }
                    }
                }
                
                if (HI_TRUE == pstCfgNode->bValid)
                {
                    pstI2cData = &pstCfgNode->stSnsRegsInfo.astI2cData[i];
                    if (pstI2cData->bUpdate == HI_TRUE)
                    {
                        if (HI_NULL != pstDrvCtx->stBusCb.pfnISPWriteI2CData)
                        {
                            pstDrvCtx->stBusCb.pfnISPWriteI2CData(pstI2cData->u8DevAddr,
                                pstI2cData->u32RegAddr, pstI2cData->u32AddrByteNum,
                                pstI2cData->u32Data, pstI2cData->u32DataByteNum);
                        }
                    }
                }
                else
                {
                    ;
                }
            }
        }
        /* config ssp */
        else if (ISP_SNS_SSP_TYPE == pstCurNode->stSnsRegsInfo.enSnsType)
        {
            u8CfgNodeIdx = (pstCurNode->stSnsRegsInfo.astSspData[i].u8DelayFrmNum / DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1));
            u8CfgNodeVC = (pstCurNode->stSnsRegsInfo.astSspData[i].u8DelayFrmNum % DIV_0_TO_1(pstDrvCtx->stSyncCfg.u8VCNumMax + 1));
            if (pstDrvCtx->stSyncCfg.u8VCCfgNum == u8CfgNodeVC)
            {
                if (u8CfgNodeIdx > CFG2VLD_DLY_LIMIT-1)
                {
                    ISP_TRACE(HI_DBG_WARN, "DelayFrmNum error!\n");
                    return HI_FAILURE;
                }
                
                pstCfgNode    = pstDrvCtx->stSyncCfg.apstNode[u8CfgNodeIdx];
                pstPreCfgNode = pstDrvCtx->stSyncCfg.apstNode[u8CfgNodeIdx + 1];

                if (HI_NULL == pstCfgNode)
                {
                    continue;
                }
                
                if (HI_NULL == pstPreCfgNode)
                {
                    
                }
                else
                {
                    if ((IS_LINEAR_MODE(u8WDRMode)) || (IS_BUILT_IN_WDR_MODE(u8WDRMode)))
                    {
                        /* not config sensor when cur == pre */
                        if (pstCfgNode == pstPreCfgNode)
                        {
                            continue;
                        }
                    }
                }

                if (HI_TRUE == pstCfgNode->bValid)
                {
                    pstSspData = &pstCfgNode->stSnsRegsInfo.astSspData[i];
                    if (pstSspData->bUpdate == HI_TRUE)
                    {
                        if (HI_NULL != pstDrvCtx->stBusCb.pfnISPWriteSSPData)
                        {
                            pstDrvCtx->stBusCb.pfnISPWriteSSPData(pstSspData->u32DevAddr,
                                pstSspData->u32DevAddrByteNum, pstSspData->u32RegAddr,
                                pstSspData->u32RegAddrByteNum, pstSspData->u32Data,
                                pstSspData->u32DataByteNum);
                        }
                    }
                }
                else
                {
                    ;
                }
            }            
        }
        else
        {

        }
    }

    return HI_SUCCESS;
}

HI_S32 ISP_RegisterBusCallBack (ISP_DEV IspDev,
    ISP_BUS_TYPE_E enType, ISP_BUS_CALLBACK_S *pstBusCb)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    unsigned long u32Flags;
    
    ISP_CHECK_POINTER(pstBusCb);
    ISP_CHECK_DEV(IspDev);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

    spin_lock_irqsave(&g_stIspLock, u32Flags);
    if (ISP_BUS_TYPE_I2C == enType)
    {
        pstDrvCtx->stBusCb.pfnISPWriteI2CData = pstBusCb->pfnISPWriteI2CData;
    }
    else if (ISP_BUS_TYPE_SSP == enType)
    {
        pstDrvCtx->stBusCb.pfnISPWriteSSPData = pstBusCb->pfnISPWriteSSPData;
    }
    else
    {
        spin_unlock_irqrestore(&g_stIspLock, u32Flags);
        printk("The bus type %d registerd to isp is err!", enType);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    spin_unlock_irqrestore(&g_stIspLock, u32Flags);
    return HI_SUCCESS;
}

HI_S32 ISP_RegisterPirisCallBack (ISP_DEV IspDev, ISP_PIRIS_CALLBACK_S *pstPirisCb)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    unsigned long u32Flags;
    
    ISP_CHECK_POINTER(pstPirisCb);
    ISP_CHECK_DEV(IspDev);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

    spin_lock_irqsave(&g_stIspLock, u32Flags);
    pstDrvCtx->stPirisCb.pfn_piris_gpio_update = pstPirisCb->pfn_piris_gpio_update;
    spin_unlock_irqrestore(&g_stIspLock, u32Flags);

    return HI_SUCCESS;
}


HI_S32 ISP_DRV_ProcInit(ISP_DEV IspDev)
{
    HI_S32 s32Ret;
    HI_U32 u32PhyAddr;
    HI_U8  *pu8VirAddr;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    
    if (0 == proc_param)
    {
        return HI_SUCCESS;
    }
    
    ISP_CHECK_DEV(IspDev);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

    s32Ret = CMPI_MmzMallocCached(HI_NULL, "ISPProc", &u32PhyAddr, (HI_VOID**)&pu8VirAddr, ISP_PROC_SIZE);
    if (HI_SUCCESS != s32Ret)
    {
        ISP_TRACE(HI_DBG_ERR, "alloc proc buf err\n");
        return HI_ERR_ISP_NOMEM;
    }

    ((HI_CHAR *)pu8VirAddr)[0] = '\0';
    ((HI_CHAR *)pu8VirAddr)[ISP_PROC_SIZE-1] = '\0';
    
    if (down_interruptible(&pstDrvCtx->stProcSem))
    {
        return -ERESTARTSYS;
    }
    pstDrvCtx->stPorcMem.u32ProcPhyAddr = u32PhyAddr;
    pstDrvCtx->stPorcMem.u32ProcSize = ISP_PROC_SIZE;
    pstDrvCtx->stPorcMem.pProcVirtAddr = pu8VirAddr;
    up(&pstDrvCtx->stProcSem);

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_ProcExit(ISP_DEV IspDev)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    HI_U32 u32PhyAddr;
    HI_U8  *pu8VirAddr;
    
    if (0 == proc_param)
    {
        return HI_SUCCESS;
    }

    ISP_CHECK_DEV(IspDev);
    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

    u32PhyAddr = pstDrvCtx->stPorcMem.u32ProcPhyAddr;
    pu8VirAddr = (HI_U8 *)pstDrvCtx->stPorcMem.pProcVirtAddr;
    if (down_interruptible(&pstDrvCtx->stProcSem))
    {
        return -ERESTARTSYS;
    }
    pstDrvCtx->stPorcMem.u32ProcPhyAddr = 0;
    pstDrvCtx->stPorcMem.u32ProcSize = 0;
    pstDrvCtx->stPorcMem.pProcVirtAddr = HI_NULL;
    up(&pstDrvCtx->stProcSem);


    if (0 != u32PhyAddr)
    {
        CMPI_MmzFree(u32PhyAddr, pu8VirAddr);
    }

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_ProcPrintf(ISP_DEV IspDev, struct seq_file *s)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    
    if (0 == proc_param)
    {
        return HI_SUCCESS;
    }
    
    //ISP_CHECK_DEV(IspDev);    /* don't defensive check */
    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);
    
    if (down_interruptible(&pstDrvCtx->stProcSem))
    {
        return -ERESTARTSYS;
    }
    if (HI_NULL != pstDrvCtx->stPorcMem.pProcVirtAddr)
    {
        seq_printf(s, "%s", (HI_CHAR *)pstDrvCtx->stPorcMem.pProcVirtAddr);
    }
    up(&pstDrvCtx->stProcSem);

    return HI_SUCCESS;
}

#ifdef ENABLE_JPEGEDCF
HI_S32 ISP_SetDCFInfo(ISP_DEV IspDev,ISP_DCF_INFO_S *pstIspDCF)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    ISP_CHECK_DEV(IspDev);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);
    memcpy(pstDrvCtx->pDCFInfoVirAddr,pstIspDCF,sizeof(ISP_DCF_INFO_S));
    return HI_SUCCESS;
}

HI_S32 ISP_GetDCFInfo(ISP_DEV IspDev,ISP_DCF_INFO_S *pstIspDCF)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    ISP_CHECK_DEV(IspDev);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);
	if (pstIspDCF && pstDrvCtx->pDCFInfoVirAddr)
	{
		memcpy(pstIspDCF,pstDrvCtx->pDCFInfoVirAddr,sizeof(ISP_DCF_INFO_S));
	}
    
    return HI_SUCCESS;
}

HI_S32 ISP_DRV_DCFInfoBufInit(ISP_DEV IspDev)
{
    HI_S32 s32Ret;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    HI_U32 u32PhyAddr, u32Size;
    HI_U8  *pu8VirAddr;

    ISP_CHECK_DEV(IspDev);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

    u32Size = sizeof(ISP_DCF_INFO_S);
    s32Ret = CMPI_MmzMallocNocache(HI_NULL, "ISPDCFInfo", &u32PhyAddr, (HI_VOID**)&pu8VirAddr, u32Size);
    if (HI_SUCCESS != s32Ret)
    {
        ISP_TRACE(HI_DBG_ERR, "alloc DCFInfo buf err\n");
        return HI_ERR_ISP_NOMEM;
    }

    memset(pu8VirAddr, 0, u32Size);

    if (down_interruptible(&pstDrvCtx->stProcSem))
    {
        return -ERESTARTSYS;
    }
    
    pstDrvCtx->u32DCFInfoPhyAddr = u32PhyAddr;
    pstDrvCtx->pDCFInfoVirAddr = (ISP_DCF_INFO_S *)pu8VirAddr;

    up(&pstDrvCtx->stProcSem);

    return HI_SUCCESS;
}

HI_S32 ISP_DRV_DCFInfoBufExit(ISP_DEV IspDev)
{
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    HI_U32 u32PhyAddr;
    HI_U8  *pu8VirAddr;

    ISP_CHECK_DEV(IspDev);

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

    u32PhyAddr = pstDrvCtx->u32DCFInfoPhyAddr;
    pu8VirAddr = (HI_U8 *)pstDrvCtx->pDCFInfoVirAddr;
    if (down_interruptible(&pstDrvCtx->stProcSem))
    {
        return -ERESTARTSYS;
    }
    pstDrvCtx->pDCFInfoVirAddr = HI_NULL;
    pstDrvCtx->u32DCFInfoPhyAddr = 0;
    up(&pstDrvCtx->stProcSem);

    if (0 != u32PhyAddr)
    {
        CMPI_MmzFree(u32PhyAddr, pu8VirAddr);
    }

    return HI_SUCCESS;
}


#endif

HI_S32 ISP_DRV_FuncPreFilter(unsigned int cmd)
{
    /* not support jpegedcf */
    if (!CHIP_SUPPORT_JPEGEDCF() && ((ISP_DCF_INFO_SET == cmd) || (ISP_DCF_INFO_GET == cmd)
        || (ISP_DCFINFO_BUF_INIT == cmd) || (ISP_DCFINFO_BUF_EXIT == cmd)))
    {
        return HI_ERR_ISP_NOT_SUPPORT;
    }

    return HI_SUCCESS;
}

HI_S32 ISP_SetModParam(ISP_MOD_PARAM_S *pstIspModParam)
{  
    if ( ((0 == proc_param) && (pstIspModParam->proc_param != 0)) ||
		 ((0 != proc_param) && (pstIspModParam->proc_param == 0)) )
    {		
        ISP_TRACE(HI_DBG_ERR, "proc_param do not support to change! %d to %d.\n",
			proc_param, pstIspModParam->proc_param);
        return HI_ERR_ISP_NOT_SUPPORT;
    }
    
	proc_param = pstIspModParam->proc_param;

    return HI_SUCCESS;
}

HI_S32 ISP_GetModParam(ISP_MOD_PARAM_S *pstIspModParam)
{
	pstIspModParam->proc_param = proc_param;

    return HI_SUCCESS;
}



static long ISP_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    unsigned int __user *argp = (unsigned int __user *)arg;
    ISP_SYNC_CFG_BUF_S  *pstSyncCfgBuf = HI_NULL;
    ISP_SYNC_CFG_BUF_NODE_S  *pstCurNode = HI_NULL;
    HI_S32  s32Ret;
    ISP_DEV IspDev = ISP_GET_DEV(file);
    unsigned long u32Flags;

    s32Ret = ISP_DRV_FuncPreFilter(cmd);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    switch (cmd)
    {
        case ISP_DEV_SET_FD :
        {
            file->private_data = (HI_VOID *)(*((HI_U32 *)arg));
            return 0;
        }
        case ISP_GET_VERSION :
        {
            if (copy_from_user(&gs_stIspLibInfo, argp, sizeof(ISP_VERSION_S)))
            {
                printk("copy isp lib info from user failed!\n");
                return -EFAULT;
            }
            gs_stIspLibInfo.u32Magic = VERSION_MAGIC + ISP_MAGIC_OFFSET;
            if (copy_to_user(argp, &gs_stIspLibInfo, sizeof(ISP_VERSION_S)))
            {
                printk(KERN_INFO "copy lib infor to user failed!\n");
                return -EFAULT;
            }
            return 0;
        }
        case ISP_GET_FRAME_EDGE :
        {
            HI_U32 u32Status = 0x0;
            s32Ret = ISP_GetFrameEdge(IspDev, &u32Status);
            if (s32Ret)
            {
                //printk(KERN_INFO "Get Interrupt failed!\n");
                ISP_TRACE(HI_DBG_WARN, "Get Interrupt failed!\n");
                return -EFAULT;
            }
            u32Status &= HW_REG(IO_ADDRESS_PORT(ISP_INT_MASK));
            if (copy_to_user(argp, &u32Status, sizeof(HI_U32)))
            {
                printk(KERN_INFO "copy status to user failed!\n");
                return -EFAULT;
            }
            return 0;
        }
        case ISP_GET_VD_TIMEOUT:
        {
            ISP_VD_TIMEOUT_S stVdTimeOut;

            if (copy_from_user(&stVdTimeOut, argp, sizeof(ISP_VD_TIMEOUT_S)))
            {
                printk(KERN_INFO "copy from user failed!\n");
                return -EFAULT;
            }

            ISP_GetVdTimeOut(IspDev, &stVdTimeOut.stVdInfo,
                stVdTimeOut.u32MilliSec, &stVdTimeOut.s32IntStatus);
            
            stVdTimeOut.s32IntStatus &= HW_REG(IO_ADDRESS_PORT(ISP_INT_MASK));
            
            if (copy_to_user(argp, &stVdTimeOut, sizeof(ISP_VD_TIMEOUT_S)))
            {
                printk(KERN_INFO "copy status to user failed!\n");
                return -EFAULT;
            }
            return 0;
        }
#ifdef ENABLE_JPEGEDCF
        case ISP_DCF_INFO_SET:
        {
            ISP_DCF_INFO_S stIspDCF;

            if (copy_from_user(&stIspDCF, argp, sizeof(ISP_DCF_INFO_S)))
            {
                printk(KERN_INFO "copy from user failed!\n");
                return -EFAULT;
            }

            ISP_SetDCFInfo(IspDev, &stIspDCF);
            
            return HI_SUCCESS;
        }
        case ISP_DCF_INFO_GET:
        {
            ISP_DCF_INFO_S stIspDCF;

            ISP_GetDCFInfo(IspDev, &stIspDCF);
            
            if (copy_to_user(argp, &stIspDCF, sizeof(ISP_DCF_INFO_S)))
            {
                printk(KERN_INFO "copy status to user failed!\n");
                return -EFAULT;
            }   
            return HI_SUCCESS;
        }
        case ISP_DCFINFO_BUF_INIT :
        {
            s32Ret = ISP_DRV_DCFInfoBufInit(IspDev);
            if (copy_to_user(argp, &g_astIspDrvCtx[IspDev].u32DCFInfoPhyAddr, sizeof(HI_U32)))
            {
                printk(KERN_INFO "copy base phyaddr to user failed!\n");
                return -EFAULT;
            }
            return s32Ret;
        }
        case ISP_DCFINFO_BUF_EXIT :
        {
            return ISP_DRV_DCFInfoBufExit(IspDev);
        }
#endif
        case ISP_SET_INT_ENABLE :
        {
            HI_BOOL bEn;
            if (copy_from_user(&bEn, argp, sizeof(bEn)))
            {
                printk(KERN_INFO  "copy arg from user failed!\n");
                return -EFAULT;
            }
            return ISP_SetIntEnable(IspDev, bEn);
        }
        /* There should be two ioctl:init & get base phyaddr. */
        case ISP_STAT_BUF_INIT :
        {
            s32Ret = ISP_DRV_StatBufInit(IspDev);
            if (copy_to_user(argp, &g_astIspDrvCtx[IspDev].stStatisticsBuf.u32PhyAddr, sizeof(HI_U32)))
            {
                printk(KERN_INFO "copy base phyaddr to user failed!\n");
                return -EFAULT;
            }
            return s32Ret;
        }
        case ISP_STAT_BUF_EXIT :
        {
            return ISP_DRV_StatBufExit(IspDev);
        }
        case ISP_STAT_BUF_GET :
        {
            ISP_STAT_INFO_S *pstStatInfo = HI_NULL;            
            s32Ret = ISP_DRV_StatBufUserGet(IspDev, &pstStatInfo);
            if (HI_NULL == pstStatInfo)
            {
                return s32Ret;
            }
            
            if (copy_to_user(argp, pstStatInfo, sizeof(ISP_STAT_INFO_S)))
            {
                printk(KERN_INFO "copy statistics to user failed!\n");
                return -EFAULT;
            }
            return 0;
        }
        case ISP_STAT_BUF_PUT :
        {
            ISP_STAT_INFO_S stStatInfo;
            if (copy_from_user(&stStatInfo, argp, sizeof(ISP_STAT_INFO_S)))
            {
                printk(KERN_INFO "copy statistics from user failed!\n");
                return -EFAULT;
            }
            
            return ISP_DRV_StatBufUserPut(IspDev, &stStatInfo);
        }

        case ISP_STAT_SHADOW_MEMSTATE_SET:
        {
            if (copy_from_user(&g_astIspDrvCtx[IspDev].stStatShadowMem.bUsrAccess, argp, sizeof(HI_BOOL)))
            {
                printk(KERN_INFO "set isp stat shadow mem state from user failed!\n");
                return -EFAULT;
            }
            
            return 0;
        }

        case ISP_STAT_SHADOW_MEMPHY_GET:
        {
            if (copy_to_user(argp, &g_astIspDrvCtx[IspDev].stStatShadowMem.u32PhyAddr, sizeof(HI_U32)))
            {
                printk(KERN_INFO "get isp stat shadpw mem phyaddr failed!\n");
                return -EFAULT;
            }
            return 0;
        }
        
        case ISP_REG_CFG_SET:
        {
            HI_U32 u32Flag = 1 - g_astIspDrvCtx[IspDev].u32RegCfgInfoFlag;
            
            if (copy_from_user(&g_astIspDrvCtx[IspDev].stRegCfgInfo[u32Flag].stKernelCfg, argp, sizeof(ISP_REG_KERNEL_CFG_S)))
            {
                printk(KERN_INFO "copy register config from user failed!\n");
                return -EFAULT;
            }

            spin_lock_irqsave(&g_stIspLock, u32Flags);
            g_astIspDrvCtx[IspDev].u32RegCfgInfoFlag = u32Flag;
            spin_unlock_irqrestore(&g_stIspLock, u32Flags);
            return 0;
        }
        case ISP_SYNC_CFG_SET:
        {
            spin_lock_irqsave(&g_stIspLock, u32Flags);
            pstSyncCfgBuf = &g_astIspDrvCtx[IspDev].stSyncCfg.stSyncCfgBuf;
            if (ISPSyncBufIsFull(pstSyncCfgBuf))
            {
                //printk("isp sync buffer is full!\n");
                spin_unlock_irqrestore(&g_stIspLock, u32Flags);
                return -EFAULT;
            }
            
            pstCurNode = &pstSyncCfgBuf->stSyncCfgBufNode[pstSyncCfgBuf->u8BufWRFlag];            
            spin_unlock_irqrestore(&g_stIspLock, u32Flags);
            
            if (copy_from_user(pstCurNode, argp, sizeof(ISP_SYNC_CFG_BUF_NODE_S)))
            {
                printk("copy isp sync config from user failed!\n");                
                return -EFAULT;
            }

            spin_lock_irqsave(&g_stIspLock, u32Flags);
            pstSyncCfgBuf->u8BufWRFlag = (pstSyncCfgBuf->u8BufWRFlag + 1) % ISP_SYNC_BUF_NODE_NUM;
            spin_unlock_irqrestore(&g_stIspLock, u32Flags);
            
            return 0;
        }
        case ISP_WDR_CFG_SET:
        {
            ISP_WDR_CFG_S       stTempWDRCfg;
            
            if (copy_from_user(&stTempWDRCfg, argp, sizeof(ISP_WDR_CFG_S)))
            {
                printk(KERN_INFO "copy WDR config from user failed!\n");
                return -EFAULT;
            }

            spin_lock_irqsave(&g_stIspLock, u32Flags);
            memcpy(&g_astIspDrvCtx[IspDev].stWDRCfg, &stTempWDRCfg, sizeof(ISP_WDR_CFG_S));
            ISP_DRV_SwitchMode(&g_astIspDrvCtx[IspDev]);
            spin_unlock_irqrestore(&g_stIspLock, u32Flags);
            
            return 0;
        }

        case ISP_RES_SWITCH_SET:
        {
            ISP_RES_SWITCH_MODE_S   stSnsImageMode;
            
            if (copy_from_user(&stSnsImageMode, argp, sizeof(ISP_RES_SWITCH_MODE_S)))
            {
                printk(KERN_INFO "copy SnsImageMode config from user failed!\n");
                return -EFAULT;
            }

            spin_lock_irqsave(&g_stIspLock, u32Flags);
            ISP_DRV_SwitchMode(&g_astIspDrvCtx[IspDev]);            
            spin_unlock_irqrestore(&g_stIspLock, u32Flags);
            
            return 0;
        }
        
        case ISP_ACM_ATTR_GET:
        case ISP_ACM_ATTR_SET:
        case ISP_ACM_COEF_GET:
        case ISP_ACM_COEF_SET:
        {
            return ISP_ACM_DRV_Ioctrl(file,cmd,arg);
        }
        case ISP_PROC_INIT:
        {
            s32Ret = ISP_DRV_ProcInit(IspDev);
            if (HI_SUCCESS != s32Ret)
            {
                return s32Ret;
            }
            if (copy_to_user(argp, &g_astIspDrvCtx[IspDev].stPorcMem, sizeof(ISP_PROC_MEM_S)))
            {
                printk(KERN_INFO "copy proc mem to user failed!\n");
                return -EFAULT;
            }
            return 0;
        }
        case ISP_PROC_WRITE_ING:
        {
            if (down_interruptible(&g_astIspDrvCtx[IspDev].stProcSem))
            {
                return -ERESTARTSYS;
            }
            return 0;
        }
        case ISP_PROC_WRITE_OK:
        {
            up(&g_astIspDrvCtx[IspDev].stProcSem);
            return 0;
        }
        case ISP_PROC_EXIT:
        {           
            return ISP_DRV_ProcExit(IspDev);
        }
        case ISP_PROC_PARAM_GET:
        {
            HI_U32 __user *argp = (HI_U32 __user *)arg;
            
            if (copy_to_user(argp, &proc_param, sizeof(HI_U32)))
            {
                printk(KERN_INFO "copy proc_param to user failed!\n");
                return -EFAULT;
            }
            return 0;
        }
        case ISP_MEM_INFO_SET:
        {
            if (copy_from_user(&g_astIspDrvCtx[IspDev].bMemInit, argp, sizeof(HI_BOOL)))
            {
                printk(KERN_INFO "copy arg from user failed!\n");
                return -EFAULT;
            }
            return 0;
        }
        case ISP_MEM_INFO_GET:
        {
            if (copy_to_user(argp, &g_astIspDrvCtx[IspDev].bMemInit, sizeof(HI_BOOL)))
            {
                printk(KERN_INFO "copy arg to user failed!\n");
                return -EFAULT;
            }
            return 0;
        }
        case ISP_PWM_NUM_GET:
        {
            HI_U32 __user *argp = (HI_U32 __user *)arg;
            
            if (copy_to_user(argp, &pwm_num, sizeof(HI_U32)))
            {
                printk(KERN_INFO "copy pwm number to user failed!\n");
                return -EFAULT;
            }
            return 0;
        }
        case ISP_SET_MOD_PARAM:
        {
            ISP_MOD_PARAM_S stIspModParam;

            if (copy_from_user(&stIspModParam, argp, sizeof(ISP_MOD_PARAM_S)))
            {
                printk(KERN_INFO "copy from user failed!\n");
                return -EFAULT;
            }
            
            return ISP_SetModParam(&stIspModParam);
        }
        case ISP_GET_MOD_PARAM:
        {
            ISP_MOD_PARAM_S stIspModParam;

            ISP_GetModParam(&stIspModParam);
            
            if (copy_to_user(argp, &stIspModParam, sizeof(ISP_MOD_PARAM_S)))
            {
                printk(KERN_INFO "copy status to user failed!\n");
                return -EFAULT;
            }   
            return HI_SUCCESS;
        }
		
        default:
        {
            return VREG_DRV_ioctl(file, cmd, arg);
        }
    }

    return 0;
}

static int ISP_open(struct inode * inode, struct file * file)
{
    return 0;
}

static int ISP_close(struct inode * inode, struct file * file)
{
    return 0;
}

#ifdef CONFIG_HISI_SNAPSHOT_BOOT
static HI_S32 ISP_Freeze(UMAP_DEVICE_S *pdev)
{
    printk(KERN_DEBUG "%s %d\n", __func__, __LINE__);
    return HI_SUCCESS;
}

static HI_S32 ISP_Restore(UMAP_DEVICE_S *pdev)
{
    HI_U32 u32VicapIntMask;
    
#ifndef HI_FPGA  
    MPP_CHN_S stMppChn;
    HI_BOOL bReset = HI_FALSE;
    HI_BOOL bClkEn = HI_TRUE;
    
    /* ?????,????????ISP??,??ISP??? */
    stMppChn.enModId = HI_ID_ISP;
    stMppChn.s32DevId = 0;
    CALL_SYS_DrvIoCtrl(&stMppChn, SYS_VIU_ISP_CLK_EN, &bClkEn);
    CALL_SYS_DrvIoCtrl(&stMppChn, SYS_VIU_ISP_CFG_RESET_SEL, &bReset);
    CALL_SYS_DrvIoCtrl(&stMppChn, SYS_VIU_ISP_CORE_RESET_SEL, &bReset);
#endif

    /* enable port0 int & isp int */
    u32VicapIntMask = HW_REG(IO_ADDRESS_PORT(VICAP_HD_MASK));
    HW_REG(IO_ADDRESS_PORT(VICAP_HD_MASK)) = (u32VicapIntMask | 0x10100);
    HW_REG(IO_ADDRESS_PORT(VI_PT0_INT_MASK)) = (VI_PT0_INT_FSTART);
    HW_REG(IO_ADDRESS_PORT(ISP_INT_MASK)) = (0x0);

    printk(KERN_DEBUG "%s %d\n", __func__, __LINE__);
    return HI_SUCCESS;
}
#else
static HI_S32 ISP_Freeze(UMAP_DEVICE_S *pdev)
{
    return HI_SUCCESS;
}

static HI_S32 ISP_Restore(UMAP_DEVICE_S *pdev)
{
    return HI_SUCCESS;
}
#endif

static struct file_operations stIspFileOp =
{
    .owner      = THIS_MODULE,
    .unlocked_ioctl      = ISP_ioctl,
    .open       = ISP_open,
    .release    = ISP_close
};

struct himedia_ops stHiISPDrvOps = {
    .pm_freeze = ISP_Freeze,
    .pm_restore  = ISP_Restore
};

static UMAP_DEVICE_S s_stHiISPDevice = {
    .devfs_name = "isp_dev",
    .minor      = UMAP_ISP_MINOR_BASE,
    .fops       = &stIspFileOp,
    .drvops     = &stHiISPDrvOps,
    .owner      = THIS_MODULE,
};

static inline irqreturn_t ISP_ISR(int irq, void *id)
{
    ISP_DEV IspDev = 0;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    HI_U32 u32PortIntStatus = 0, u32IspIntStatus = 0, u32PortTempIntStatus = 0;

    ISP_CHECK_DEV(IspDev);

    u32PortIntStatus = HW_REG(IO_ADDRESS_PORT(VI_PT0_INT));
    u32PortTempIntStatus = u32PortIntStatus & VI_PT0_INT_ERR;
    u32PortIntStatus &= VI_PT0_INT_FSTART;
    
    u32IspIntStatus = HW_REG(IO_ADDRESS_PORT(ISP_INT));
    u32IspIntStatus &= HW_REG(IO_ADDRESS_PORT(ISP_INT_MASK));
    
    if ((!u32PortIntStatus) && (!u32IspIntStatus) && (!u32PortTempIntStatus))
    {
        return IRQ_NONE;
    }

    if (u32IspIntStatus)
    {
        HW_REG(IO_ADDRESS_PORT(ISP_INT)) = u32IspIntStatus;
    }
    if (u32PortIntStatus)
    {
        HW_REG(IO_ADDRESS_PORT(VI_PT0_INT)) = VI_PT0_INT_FSTART;       
    }
    /*When detect vi port's width&height changed,then reset isp*/
    if(u32PortTempIntStatus)
    {
        //printk("\nVI WIDTH&HEIGTH= 0x%x\n",HW_REG(IO_ADDRESS_PORT(0x01ec)));
        pstDrvCtx = ISP_DRV_GET_CTX(IspDev);
        pstDrvCtx->stDrvDbgInfo.u32IspResetCnt++;
        HW_REG(IO_ADDRESS_PORT(VI_PT0_INT)) = u32PortTempIntStatus;            
        HW_REG(IO_ISP_ADDRESS(ISP_RESET)) = 0x1;           
        HW_REG(IO_ISP_ADDRESS(ISP_RESET)) = 0;
        return IRQ_HANDLED;
    }    

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);
    pstDrvCtx->stIntSch.u32IspIntStatus = u32IspIntStatus;
    pstDrvCtx->stIntSch.u32PortIntStatus= u32PortIntStatus;

    if ( !int_bottomhalf )
    {
        ISP_IntBottomHalf((unsigned long)pstDrvCtx);
    }
    else
    {
        tasklet_schedule(&pstDrvCtx->stIntSch.tsklet);
    }

	return IRQ_HANDLED;
}
void ISP_IntBottomHalf(unsigned long data)
{
    ISP_DEV IspDev = 0;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    ISP_SYNC_CFG_S *pstSyncCfg = HI_NULL;
	
    HI_U64 u64PtTime1 = 0, u64PtTime2 = 0;
    HI_U64 u64IspTime1 = 0, u64IspTime2 = 0;
    HI_U64 u64SensorCfgTime1 = 0, u64SensorCfgTime2 = 0;
    HI_U32 u32SensorCfgInt = 0; 
	HI_U32 u32PortIntFStart;
    HI_U32 u32IspIntStatus;

    pstDrvCtx = (ISP_DRV_CTX_S *)data;
    pstSyncCfg = &pstDrvCtx->stSyncCfg;

    u32PortIntFStart = pstDrvCtx->stIntSch.u32PortIntStatus;
    u32IspIntStatus = pstDrvCtx->stIntSch.u32IspIntStatus;

    /* * * * * * * * vi_pt_int_process begin * * * * * * * */
    if (u32PortIntFStart)
    {
        /* port int proc */
        {
            pstDrvCtx->stDrvDbgInfo.u32PtIntCnt++;
            u64PtTime1 = CALL_SYS_GetTimeStamp();
            pstDrvCtx->stDrvDbgInfo.u32PtIntGapTime = u64PtTime1 - pstDrvCtx->stDrvDbgInfo.u64PtLastIntTime;
            if (pstDrvCtx->stDrvDbgInfo.u32PtIntGapTime > pstDrvCtx->stDrvDbgInfo.u32PtIntGapTimeMax)
            {
                pstDrvCtx->stDrvDbgInfo.u32PtIntGapTimeMax = pstDrvCtx->stDrvDbgInfo.u32PtIntGapTime;
            }
            pstDrvCtx->stDrvDbgInfo.u64PtLastIntTime = u64PtTime1;
        }

        /* N to 1 WDR mode */
        if ((IS_FULL_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode)) || (IS_HALF_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode)))
        {
            ISP_DRV_GetSyncControlnfo(pstSyncCfg);
            ISP_DRV_CalcSyncCfg(pstSyncCfg);
            ISP_DRV_RegConfigIsp(pstDrvCtx);
            
            u64SensorCfgTime1 = CALL_SYS_GetTimeStamp();
            ISP_DRV_RegConfigSensor(pstDrvCtx);
            u64SensorCfgTime2 = CALL_SYS_GetTimeStamp();
            pstDrvCtx->stDrvDbgInfo.u32SensorCfgTime = u64SensorCfgTime2 - u64SensorCfgTime1;
        }

        /* port int proc */
        {
            u64PtTime2 = CALL_SYS_GetTimeStamp();
            pstDrvCtx->stDrvDbgInfo.u32PtIntTime = u64PtTime2 - u64PtTime1;

            if (pstDrvCtx->stDrvDbgInfo.u32PtIntTime > pstDrvCtx->stDrvDbgInfo.u32PtIntTimeMax)
            {
                pstDrvCtx->stDrvDbgInfo.u32PtIntTimeMax = pstDrvCtx->stDrvDbgInfo.u32PtIntTime;
            }

            if ((u64PtTime2 - pstDrvCtx->stDrvDbgInfo.u64PtLastRateTime) >= 1000000ul)
            {
                pstDrvCtx->stDrvDbgInfo.u64PtLastRateTime = u64PtTime2;
                pstDrvCtx->stDrvDbgInfo.u32PtRate = pstDrvCtx->stDrvDbgInfo.u32PtRateIntCnt;
                pstDrvCtx->stDrvDbgInfo.u32PtRateIntCnt = 0;
            }

            pstDrvCtx->stDrvDbgInfo.u32PtRateIntCnt++;
        }

        //return IRQ_HANDLED;
    }

    /* * * * * * * * isp_int_process begin * * * * * * * */
    if (u32IspIntStatus & 0x2) /* ISP Input formatter int (Frame Start int) */
    {
        /* use ISP AF int in linear/Built-in WDR mode; use ISP Frame start int in line WDR mode */
        if (IS_LINE_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode))
        {
            ISP_DRV_GetSyncControlnfo(pstSyncCfg);
            ISP_DRV_CalcSyncCfg(pstSyncCfg);
            ISP_DRV_RegConfigIsp(pstDrvCtx);
            
            u64SensorCfgTime1 = CALL_SYS_GetTimeStamp();
            ISP_DRV_RegConfigSensor(pstDrvCtx);
            u64SensorCfgTime2 = CALL_SYS_GetTimeStamp();
            pstDrvCtx->stDrvDbgInfo.u32SensorCfgTime = u64SensorCfgTime2 - u64SensorCfgTime1;
        }
    }

    if (u32IspIntStatus & 0x1) /* ISP AF int */
    //if (u32IspIntStatus & 0x8) /* ISP AF int */
    {
        /* isp int proc */
        {
            pstDrvCtx->stDrvDbgInfo.u32IspIntCnt++;            
            u64IspTime1 = CALL_SYS_GetTimeStamp();
            pstDrvCtx->stDrvDbgInfo.u32IspIntGapTime = u64IspTime1 - pstDrvCtx->stDrvDbgInfo.u64IspLastIntTime;
            if (pstDrvCtx->stDrvDbgInfo.u32IspIntGapTime > pstDrvCtx->stDrvDbgInfo.u32IspIntGapTimeMax)
            {
                pstDrvCtx->stDrvDbgInfo.u32IspIntGapTimeMax = pstDrvCtx->stDrvDbgInfo.u32IspIntGapTime;
            }
            pstDrvCtx->stDrvDbgInfo.u64IspLastIntTime = u64IspTime1;
        }

        /* N to 1 WDR mode, there is isp int only in the last frame(N-1) */
        if (IS_FULL_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode))
        {
            if (pstDrvCtx->stSyncCfg.u8VCNum != pstDrvCtx->stSyncCfg.u8VCNumMax)
            {
                /* isp int proc */
                {
                    u64IspTime2 = CALL_SYS_GetTimeStamp();
                    pstDrvCtx->stDrvDbgInfo.u32IspIntTime = u64IspTime2 - u64IspTime1;

                    if (pstDrvCtx->stDrvDbgInfo.u32IspIntTime > pstDrvCtx->stDrvDbgInfo.u32IspIntTimeMax)
                    {
                        pstDrvCtx->stDrvDbgInfo.u32IspIntTimeMax = pstDrvCtx->stDrvDbgInfo.u32IspIntTime;
                    }

                    if ((u64IspTime2 - pstDrvCtx->stDrvDbgInfo.u64IspLastRateTime) >= 1000000ul)
                    {
                        pstDrvCtx->stDrvDbgInfo.u64IspLastRateTime = u64IspTime2;
                        pstDrvCtx->stDrvDbgInfo.u32IspRate = pstDrvCtx->stDrvDbgInfo.u32IspRateIntCnt;
                        pstDrvCtx->stDrvDbgInfo.u32IspRateIntCnt = 0;
                    }

                    pstDrvCtx->stDrvDbgInfo.u32IspRateIntCnt++;
                }
                
                return ;
            }
        }
        else    /* linear or Built-in WDR mode */
        {
            #if 1
            /* use ISP AF int in linear/Built-in WDR mode; use ISP Frame start int in line WDR mode */
            if (!IS_LINE_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode))
            {
                #if 0
                ISP_DRV_GetSyncControlnfo(pstSyncCfg);
                ISP_DRV_CalcSyncCfg(pstSyncCfg);
                ISP_DRV_RegConfigIsp(pstDrvCtx);

                u64SensorCfgTime1 = CALL_SYS_GetTimeStamp();
                ISP_DRV_RegConfigSensor(pstDrvCtx);
                u64SensorCfgTime2 = CALL_SYS_GetTimeStamp();
                pstDrvCtx->stDrvDbgInfo.u32SensorCfgTime = u64SensorCfgTime2 - u64SensorCfgTime1;
                #endif
            }
            #else
            ISP_DRV_GetSyncCfgNode(pstSyncCfg);
            ISP_DRV_RegConfigSensor4Linear(pstDrvCtx);
            //ISP_DRV_RegConfigIsp4Linear(pstDrvCtx);
            
            ISP_DRV_RegConfigIsp(pstDrvCtx);
            #endif
        }

        pstDrvCtx->u32Status = (u32IspIntStatus & 0x1);

        /* first config register. */
        //ISP_DRV_RegConfig(&pstDrvCtx->stRegCfgInfo[pstDrvCtx->u32RegCfgInfoFlag]);

        ISP_DRV_StatBufBusyPut(IspDev);

        pstDrvCtx->bEdge = HI_TRUE;
        pstDrvCtx->bVd   = HI_TRUE;

		HW_REG(IO_ADDRESS_PORT(ISP_444_REG_NEWER)) = 1;
		HW_REG(IO_ADDRESS_PORT(CH_REG_NEWER)) = 1;		
		//HW_REG(IO_ADDRESS_PORT(ISP_422_REG_NEWER)) = 1;

        wake_up_interruptible(&pstDrvCtx->stIspWait);
        wake_up_interruptible(&pstDrvCtx->stIspWaitVd);
       IspSyncTaskProcess(IspDev);

        /* isp int proc */
        {
            u64IspTime2 = CALL_SYS_GetTimeStamp();
            pstDrvCtx->stDrvDbgInfo.u32IspIntTime = u64IspTime2 - u64IspTime1;

            if (pstDrvCtx->stDrvDbgInfo.u32IspIntTime > pstDrvCtx->stDrvDbgInfo.u32IspIntTimeMax)
            {
                pstDrvCtx->stDrvDbgInfo.u32IspIntTimeMax = pstDrvCtx->stDrvDbgInfo.u32IspIntTime;
            }

            if ((u64IspTime2 - pstDrvCtx->stDrvDbgInfo.u64IspLastRateTime) >= 1000000ul)
            {
                pstDrvCtx->stDrvDbgInfo.u64IspLastRateTime = u64IspTime2;
                pstDrvCtx->stDrvDbgInfo.u32IspRate = pstDrvCtx->stDrvDbgInfo.u32IspRateIntCnt;
                pstDrvCtx->stDrvDbgInfo.u32IspRateIntCnt = 0;
            }

            pstDrvCtx->stDrvDbgInfo.u32IspRateIntCnt++;
        }        
    }
#if 1
    if (0 == update_pos) /* frame start */
    {
        u32SensorCfgInt = (u32IspIntStatus & 0x1);
    }
    else                 /* frame end */
    {
        u32SensorCfgInt = (u32IspIntStatus & 0x40);
    }

    /* * * * * * * * isp_int2_process * * * * * * * */
    if (u32SensorCfgInt)
    {
        /* In linear mode or built-in WDR mode, config sensor and vi(isp) register with isp_int(frame start interrupt) */
        if (!IS_LINE_WDR_MODE(pstDrvCtx->stSyncCfg.u8WDRMode))
        {
            ISP_DRV_GetSyncControlnfo(pstSyncCfg);
            ISP_DRV_CalcSyncCfg(pstSyncCfg);
            ISP_DRV_RegConfigIsp(pstDrvCtx);
        
            u64SensorCfgTime1 = CALL_SYS_GetTimeStamp();
            ISP_DRV_RegConfigSensor(pstDrvCtx);
            u64SensorCfgTime2 = CALL_SYS_GetTimeStamp();
            pstDrvCtx->stDrvDbgInfo.u32SensorCfgTime = u64SensorCfgTime2 - u64SensorCfgTime1;
        }
    }
#endif

	if (u32IspIntStatus & 0x200)
	{
		ISP_DRV_RegConfig(&pstDrvCtx->stRegCfgInfo[pstDrvCtx->u32RegCfgInfoFlag]);
	}
    
    return ;
}

static int ISP_DRV_Init(void)
{
    HI_U32 u32VicapIntMask;
    HI_S32 s32Ret;

    memset(g_astIspDrvCtx, 0, sizeof(ISP_DRV_CTX_S) * 1);

    reg_vicap_base_va = (void __iomem*)IO_ADDRESS(VICAP_BASE);
    if (reg_vicap_base_va == HI_NULL)
    {
        printk(KERN_ERR "Remap VI base failed!\n");
        return -ENOMEM;
    }

    reg_isp_base_va = (void __iomem*)IO_ADDRESS(ISP_BASE);
    if (reg_isp_base_va == HI_NULL)
    {
        printk(KERN_ERR "Remap isp base failed!\n");
        return -ENOMEM;
    }

    /* enable port0 int & isp int */
    u32VicapIntMask = HW_REG(IO_ADDRESS_PORT(VICAP_HD_MASK));
    HW_REG(IO_ADDRESS_PORT(VICAP_HD_MASK)) = (u32VicapIntMask | 0x10100);

    // TODO: Frame WDR使用PT中断，其它模式?
    HW_REG(IO_ADDRESS_PORT(VI_PT0_INT_MASK)) = (VI_PT0_INT_FSTART);

    HW_REG(IO_ADDRESS_PORT(ISP_INT_MASK)) = (0x0);

    if (request_irq(ISP_IRQ_NR, ISP_ISR, IRQF_SHARED, "ISP", (void*)&g_astIspDrvCtx))
    {
        printk(KERN_ERR  "ISP Register Interrupt Failed!\n");
        return -EAGAIN;
    }

    init_waitqueue_head(&g_astIspDrvCtx[0].stIspWait);
    init_waitqueue_head(&g_astIspDrvCtx[0].stIspWaitVd);
    g_astIspDrvCtx[0].bEdge = HI_FALSE;
    g_astIspDrvCtx[0].bVd = HI_FALSE;
    g_astIspDrvCtx[0].bMemInit = HI_FALSE;
    sema_init(&g_astIspDrvCtx[0].stIspSem,1);
    sema_init(&g_astIspDrvCtx[0].stProcSem,1);

    /* init vc_num register */
        /* work bad in line WDR mode */
    //HW_REG(IO_ADDRESS_PORT(VC_NUM_ADDR)) = 0x2000000;
    ISP_ACM_DRV_Init();
    if (int_bottomhalf)
    {
        tasklet_init(&g_astIspDrvCtx[0].stIntSch.tsklet, ISP_IntBottomHalf, (unsigned long)&g_astIspDrvCtx[0]);
    }    

    /* alloc isp stat shandow mem for application use */
    g_astIspDrvCtx[0].stStatShadowMem.u32Size = sizeof(ISP_STAT_S);
    s32Ret = CMPI_MmzMallocNocache(HI_NULL, "ISP shadow mem", &g_astIspDrvCtx[0].stStatShadowMem.u32PhyAddr, 
        (HI_VOID**)&g_astIspDrvCtx[0].stStatShadowMem.pVirtAddr, g_astIspDrvCtx[0].stStatShadowMem.u32Size);
    
    if (HI_SUCCESS != s32Ret)
    {
        ISP_TRACE(HI_DBG_ERR, "alloc ISP shadow mem err\n");
        return HI_ERR_ISP_NOMEM;
    }
	
    SyncTaskInit(0);
    
    return 0;
}

static int ISP_DRV_Exit(void)
{
    free_irq(ISP_IRQ_NR, (void*)&g_astIspDrvCtx);
    //iounmap((void*)reg_vicap_base_va);
    //iounmap((void*)reg_isp_base_va);
    ISP_ACM_DRV_Exit();
    reg_vicap_base_va = HI_NULL;
    reg_isp_base_va = HI_NULL;

    if (0 != g_astIspDrvCtx[0].stStatShadowMem.u32PhyAddr)
    {
        CMPI_MmzFree(g_astIspDrvCtx[0].stStatShadowMem.u32PhyAddr, g_astIspDrvCtx[0].stStatShadowMem.pVirtAddr);
    }

    return 0;
}

#ifdef TEST_TIME
static int ISP_Test_Init(void)
{
    CMPI_MmzMallocNocache(HI_NULL, "ISPStatTest", &g_test_phyaddr, (HI_VOID**)&g_test_pviraddr, 256*2);
}

static int ISP_Test_Exit(void)
{
    CMPI_MmzFree(g_test_phyaddr, g_test_pviraddr);
}
#endif

static int ISP_ProcShow(struct seq_file *s, void *pArg)
{
    ISP_DEV IspDev = 0;
    ISP_DRV_CTX_S *pstDrvCtx = HI_NULL;
    HI_U8 u8Bayer;

    pstDrvCtx = ISP_DRV_GET_CTX(IspDev);

    seq_printf(s, "\n[ISP] Version: [%s], Build Time[%s, %s]\n",
        ISP_VERSION, 
        gs_stIspLibInfo.au8Date, gs_stIspLibInfo.au8Time);
    seq_printf(s, "\n");

    if (0 == proc_param)
    {
        return HI_SUCCESS;
    }
    
    seq_printf(s, "-----DRV INFO---------------------------------------------------------------------------------\n");

    seq_printf(s, "%12s" "%12s" "%12s" "%12s" "%12s" "%12s" "%12s" "%12s\n" 
            ,"IspDev","IntCnt","IntT","MaxIntT","IntGapT","MaxGapT","IntRat","IspResetCnt");

    seq_printf(s, "%12d" "%12d" "%12d" "%12d" "%12d" "%12d" "%12d" "%12d\n",
            0,
            pstDrvCtx->stDrvDbgInfo.u32IspIntCnt,
            pstDrvCtx->stDrvDbgInfo.u32IspIntTime,
            pstDrvCtx->stDrvDbgInfo.u32IspIntTimeMax,
            pstDrvCtx->stDrvDbgInfo.u32IspIntGapTime,
            pstDrvCtx->stDrvDbgInfo.u32IspIntGapTimeMax,
            pstDrvCtx->stDrvDbgInfo.u32IspRate,
            pstDrvCtx->stDrvDbgInfo.u32IspResetCnt);

    seq_printf(s, "\n");

    seq_printf(s, "%12s" "%12s" "%12s" "%12s" "%12s" "%12s" "%12s" "%12s\n"
            ,"","PtIntCnt","PtIntT","PtMaxIntT","PtIntGapT","PtMaxGapT","PtIntRat","SensorCfgT");

    seq_printf(s, "%12s" "%12d" "%12d" "%12d" "%12d" "%12d" "%12d" "%12d\n",
            "",
            pstDrvCtx->stDrvDbgInfo.u32PtIntCnt,
            pstDrvCtx->stDrvDbgInfo.u32PtIntTime,
            pstDrvCtx->stDrvDbgInfo.u32PtIntTimeMax,
            pstDrvCtx->stDrvDbgInfo.u32PtIntGapTime,
            pstDrvCtx->stDrvDbgInfo.u32PtIntGapTimeMax,
            pstDrvCtx->stDrvDbgInfo.u32PtRate,
            pstDrvCtx->stDrvDbgInfo.u32SensorCfgTime);

    seq_printf(s, "\n");

    /* TODO: show isp attribute here. width/height/bayer_format, etc..
      * Read parameter from memory directly.
      */
    u8Bayer = hi_isp_top_rggb_start_read();
    seq_printf(s, "-----PubAttr INFO---------------------------------------------------------------------------------\n");
    
    seq_printf(s, "%12s" "%12s" "%12s" "%12s" "%12s\n" 
            ,"WndX","WndY","WndW","WndH","Bayer");
    
    seq_printf(s, "%12d" "%12d" "%12d" "%12d" "%12s\n\n",
            hi_isp_crop_x_start_read(),
            hi_isp_crop_y_start_read(),
           (hi_isp_yuv444_width_read()+1),
           (hi_isp_yuv444_height_read()+1),
            (0 == u8Bayer) ? "RGGB" :
                (1 == u8Bayer) ? "GRBG" :
                (2 == u8Bayer) ? "GBRG" :
                (3 == u8Bayer) ? "BGGR" : "BUTT");
    
	ISP_DRV_ProcPrintf(0, s);

    return 0;
}

HI_S32 ISP_Init(void *p)
{
    memset(&g_astIspDrvCtx[0].stDrvDbgInfo, 0, sizeof(ISP_DRV_DBG_INFO_S));    
    return HI_SUCCESS;
}

HI_VOID ISP_Exit(void)
{
    VREG_DRV_ReleaseAll();

    ISP_DRV_StatBufExit(0);
    ISP_DRV_ProcExit(0);
    
    return ;
}

static HI_U32 ISP_GetVerMagic(HI_VOID)
{
    return VERSION_MAGIC;
}

static UMAP_MODULE_S s_stModule =
{
    .pstOwner      = THIS_MODULE,
    .enModId       = HI_ID_ISP,
    .aModName      = "isp",

    .pfnInit       = ISP_Init,
    .pfnExit       = ISP_Exit,
    .pfnVerChecker = ISP_GetVerMagic,
    .pData         = HI_NULL,
};

static int __init ISP_ModInit(void)
{
    HI_S32  s32Ret;
#ifndef HI_FPGA  
    MPP_CHN_S stMppChn;
    HI_BOOL bReset = HI_FALSE;
    HI_BOOL bClkEn = HI_TRUE;
    
    /* 低功耗策略，模块初始化时打开ISP时钟，撤销ISP软复位 */
    stMppChn.enModId = HI_ID_ISP;
    stMppChn.s32DevId = 0;
    CALL_SYS_DrvIoCtrl(&stMppChn, SYS_VIU_ISP_CLK_EN, &bClkEn);
    CALL_SYS_DrvIoCtrl(&stMppChn, SYS_VIU_ISP_CFG_RESET_SEL, &bReset);
    CALL_SYS_DrvIoCtrl(&stMppChn, SYS_VIU_ISP_CORE_RESET_SEL, &bReset);
#endif

    s32Ret = CMPI_RegisterDevice(&s_stHiISPDevice);
    if (s32Ret)
    {
        printk("Kernel: Could not register isp devices\n");
        return s32Ret;
    }

    if(HI_NULL == CMPI_CreateProc(PROC_ENTRY_ISP, ISP_ProcShow, 0))
    {
        printk("Kernel: Register isp proc failed!\n");
        goto OUT3;
    }

    if (CMPI_RegisterModule(&s_stModule))
    {
        goto OUT2;
    }

    spin_lock_init(&g_stIspLock);

    if(0 != ISP_DRV_Init())
    {
        printk("isp init failed\n");
        goto OUT1;
    }

    g_stIspExpFunc.pfnISPRegisterBusCallBack = ISP_RegisterBusCallBack;
    g_stIspPirisExpFunc.pfnISPRegisterPirisCallBack = ISP_RegisterPirisCallBack;

#ifdef TEST_TIME
    ISP_Test_Init();
#endif
    printk(KERN_INFO  "ISP Mod init!\n");
    return HI_SUCCESS;
OUT1:
    CMPI_UnRegisterModule(HI_ID_ISP);

OUT2:
    CMPI_RemoveProc(PROC_ENTRY_ISP);

OUT3:
    CMPI_UnRegisterDevice(&s_stHiISPDevice);

    printk(KERN_INFO  "ISP Mod init failed!\n");
    return HI_FAILURE;
}

static void __exit ISP_ModExit(void)
{
#ifndef HI_FPGA  
    MPP_CHN_S stMppChn;
    HI_BOOL bReset = HI_TRUE;
    HI_BOOL bClkEn = HI_FALSE;
#endif

    g_stIspExpFunc.pfnISPRegisterBusCallBack = HI_NULL;
    g_stIspPirisExpFunc.pfnISPRegisterPirisCallBack = HI_NULL;
    ISP_DRV_Exit();
    
    CMPI_UnRegisterModule(HI_ID_ISP);
    CMPI_RemoveProc(PROC_ENTRY_ISP);

    CMPI_UnRegisterDevice(&s_stHiISPDevice);

#ifdef TEST_TIME
    ISP_Test_Exit();
#endif
#ifndef HI_FPGA
    /* 低功耗策略，模块卸载时先复位ISP，关闭ISP时钟 */
    stMppChn.enModId = HI_ID_ISP;
    stMppChn.s32DevId = 0;
    CALL_SYS_DrvIoCtrl(&stMppChn, SYS_VIU_ISP_CLK_EN, &bClkEn);
    CALL_SYS_DrvIoCtrl(&stMppChn, SYS_VIU_ISP_CFG_RESET_SEL, &bReset);
    CALL_SYS_DrvIoCtrl(&stMppChn, SYS_VIU_ISP_CORE_RESET_SEL, &bReset);
#endif
    printk(KERN_INFO  "ISP Mod Exit!\n");
}

module_init(ISP_ModInit);
module_exit(ISP_ModExit);
module_param(proc_param, uint, S_IRUGO);
module_param(pwm_num, uint, S_IRUGO);
module_param(update_pos, uint, S_IRUGO);
module_param(int_bottomhalf, bool, S_IRUGO);


EXPORT_SYMBOL(g_stIspExpFunc);
EXPORT_SYMBOL(g_stIspPirisExpFunc);

#ifdef ENABLE_JPEGEDCF
EXPORT_SYMBOL(ISP_GetDCFInfo);
#endif
MODULE_AUTHOR(MPP_AUTHOR);
MODULE_LICENSE(MPP_LICENSE);
MODULE_VERSION(MPP_VERSION);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

