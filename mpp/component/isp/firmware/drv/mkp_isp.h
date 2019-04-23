/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : mkp_isp.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2012/12/19
  Description   : 
  History       :
  1.Date        : 2012/12/19
    Author      : n00168968
    Modification: Created file

******************************************************************************/

#ifndef __MKP_ISP_H__
#define __MKP_ISP_H__

#include "hi_type.h"
#include "hi_comm_3a.h"
#include "hi_common.h"
#include "hi_defines.h"

#define IOC_TYPE_ISP   'I'

#define MAX_ISP_STAT_BUF_NUM    2

#define ISP_VER_PRIX            "_ISP_V"
#define ISP_VERSION             CHIP_NAME ISP_VER_PRIX MK_VERSION(VER_X,VER_Y,VER_Z,VER_P,VER_B) VER_D

typedef enum hiIOC_NR_ISP_E
{
    IOC_NR_ISP_SET_FD = 0,
    IOC_NR_ISP_GET_FRAME_EDGE,
    IOC_NR_ISP_GET_VD_TIMEOUT,
    IOC_NR_ISP_SET_INT_ENABLE,
    IOC_NR_ISP_GET_VERSION,

    IOC_NR_ISP_STAT_BUF_INIT,
    IOC_NR_ISP_STAT_BUF_EXIT,
    IOC_NR_ISP_STAT_BUF_GET,
    IOC_NR_ISP_STAT_BUF_PUT,
    IOC_NR_ISP_STAT_SHADOW_MEMPHY_GET,
    IOC_NR_ISP_STAT_SHADOW_MEMSTATE_SET,
    IOC_NR_ISP_REG_CFG_INIT,
    IOC_NR_ISP_REG_CFG_SET,
    IOC_NR_ISP_PROC_INIT,
    IOC_NR_ISP_PROC_WRITE_ING,
    IOC_NR_ISP_PROC_WRITE_OK,
    IOC_NR_ISP_PROC_EXIT,
    IOC_NR_ISP_PROC_PARAM_GET,
    IOC_NR_ISP_DCF_INFO_SET,
    IOC_NR_ISP_DCF_INFO_GET,
    IOC_NR_ISP_SYNC_CFG_SET,
    IOC_NR_WDR_SYNC_CFG_SET,
    IOC_NR_ISP_DCFINFO_BUF_INIT,
    IOC_NR_ISP_DCFINFO_BUF_EXIT,
    IOC_NR_ACM_ATTR_GET,
    IOC_NR_ACM_ATTR_SET,
    IOC_NR_ACM_COEF_GET,
    IOC_NR_ACM_COEF_SET,
    IOC_NR_ISP_MEM_INFO_SET,
    IOC_NR_ISP_MEM_INFO_GET,
    IOC_NR_ISP_PWM_NUM_GET,
    IOC_NR_ISP_RES_SWITCH_SET,
    IOC_NR_ISP_SET_MOD_PARAM,
    IOC_NR_ISP_GET_MOD_PARAM,

    IOC_NR_ISP_BUTT,
} IOC_NR_ISP_E;

typedef struct hiISP_VD_TIMEOUT_S
{
   ISP_VD_INFO_S stVdInfo;
   HI_U32  u32MilliSec;     /*RW, Range: [0x0, 0xFFFFFFFF], the number of the */
   HI_S32  s32IntStatus;    /*RO, when there is ISP interrupt, then the s32IntStatus is 1 */
}ISP_VD_TIMEOUT_S;

typedef struct hiISP_VERSION_S
{
    HI_U8 au8MppVersion[4];
    HI_U8 au8Date[20];
    HI_U8 au8Time[20];
    HI_U32 u32Magic;
} ISP_VERSION_S;

/* the statistics of isp */
typedef struct hiISP_DP_STAT_S
{
    HI_U16  u16DefectPixelCount;
} ISP_DP_STAT_S;

typedef struct hiISP_DEHAZE_STAT_S
{
    //statics for dehaze run ,lsw
    //HI_U32 au32DehazeMinStatDout[256];
    //HI_U32 au32DehazeMaxStatDout[1024];
    //HI_U32 au32Hist[32];
    HI_U32  au32DehazeMinDout[DEFOG_ZONE_NUM/4];
} ISP_DEHAZE_STAT_S;


typedef struct hiISP_COMM_STAT_S
{
    HI_U16  au16WhiteBalanceGain[4];    
} ISP_COMM_STAT_S;

typedef struct hiISP_STAT_S
{
    ISP_AE_STAT_1_S     stAeStat1;   /* not support */  
    ISP_AE_STAT_2_S     stAeStat2;   /* not support */ 
    ISP_AE_STAT_3_S     stAeStat3;
    ISP_AE_STAT_4_S     stAeStat4;
    ISP_AE_STAT_5_S     stAeStat5;
    ISP_AWB_STAT_1_S    stAwbStat1;    
    ISP_AWB_STAT_2_S    stAwbStat2;
    ISP_AWB_STAT_3_S    stAwbStat3;    
    ISP_AWB_STAT_4_S    stAwbStat4;
    ISP_AF_STAT_S       stAfStat;
    ISP_DP_STAT_S       stDpStat;
    ISP_COMM_STAT_S     stCommStat;
    ISP_DEHAZE_STAT_S   stDehazeStat;
} ISP_STAT_S;

typedef union hiISP_STAT_KEY_U
{
    HI_U32  u32Key;
    struct
    {
        HI_U32  bit1AeStat1     : 1 ;   /* [0] not support */
        HI_U32  bit1AeStat2     : 1 ;   /* [1] not support */
        HI_U32  bit1AeStat3     : 1 ;   /* [2] */
        HI_U32  bit1AeStat4     : 1 ;   /* [3] */
        HI_U32  bit1AeStat5     : 1 ;   /* [4] */
        HI_U32  bit1AwbStat1    : 1 ;   /* [5] */
        HI_U32  bit1AwbStat2    : 1 ;   /* [6] */
        HI_U32  bit1AwbStat3    : 1 ;   /* [7] */
        HI_U32  bit1AwbStat4    : 1 ;   /* [8] */
        HI_U32  bit1AfStat      : 1 ;   /* [9] */
        HI_U32  bit1DpStat      : 1 ;   /* [10] */
        HI_U32  bit1CommStat    : 1 ;   /* [11] */
        HI_U32  bit1Dehaze      : 1 ;   /* [12] */
        HI_U32  bit19Rsv        : 19;   /* [13:31] */
    };
}ISP_STAT_KEY_U;

typedef struct hiISP_STAT_INFO_S
{
    ISP_STAT_KEY_U  unKey;    
    HI_U32  u32PhyAddr;
    HI_VOID *pVirtAddr;                 /* (ISP_STAT_S *) */
} ISP_STAT_INFO_S;


typedef struct hiISP_STAT_SHADOW_MEM_S
{
    HI_U32  u32PhyAddr;
    HI_VOID *pVirtAddr; 
    HI_U32 u32Size;
    HI_BOOL bUsrAccess;
} ISP_STAT_SHADOW_MEM_S;


/* the register config of isp */
typedef struct hiISP_AE_REG_CFG_1_S
{
    HI_U8   au8MeteringHistThresh[4];
    HI_U8   au8WeightTable[15][17];
} ISP_AE_REG_CFG_1_S;

typedef struct hiISP_AE_REG_CFG_2_S
{
    HI_U64  u64Exposure;
    HI_U32  u32IntTime[4];  /* 0: Shortest shutter(SS), 1: Short shutter(S), 2: long shutter(L), 3: longest shutter(LL) */
    HI_U32  u32IspDgain;

    HI_BOOL bPirisValid;
    HI_S32  s32PirisPos;
} ISP_AE_REG_CFG_2_S;

typedef struct hiISP_AWB_REG_CFG_1_S
{
    HI_U32  au32WhiteBalanceGain[4];
    HI_U16  au16ColorMatrix[9];
} ISP_AWB_REG_CFG_1_S;

typedef struct hiISP_AWB_REG_CFG_2_S
{
    HI_U16  u16MeteringWhiteLevelAwb;
    HI_U16  u16MeteringBlackLevelAwb;
    HI_U16  u16MeteringCrRefMaxAwb;
    HI_U16  u16MeteringCbRefMaxAwb;
    HI_U16  u16MeteringCrRefMinAwb;
    HI_U16  u16MeteringCbRefMinAwb;
    HI_U16  u16MeteringCrRefHighAwb;
    HI_U16  u16MeteringCrRefLowAwb;
    HI_U16  u16MeteringCbRefHighAwb;
    HI_U16  u16MeteringCbRefLowAwb;
} ISP_AWB_REG_CFG_2_S;

typedef struct hiISP_AWB_REG_CFG_3_S
{
    HI_BOOL bAboveWhiteLevelClip;
    HI_BOOL bBelowBlackLevelClip;
    HI_U16  u16MeteringWhiteLevelAwb;
    HI_U16  u16MeteringBlackLevelAwb;
    HI_U16  u16MeteringCrRefMaxAwb;
    HI_U16  u16MeteringCbRefMaxAwb;
    HI_U16  u16MeteringCrRefMinAwb;
    HI_U16  u16MeteringCbRefMinAwb;
    HI_U16  u16MeteringCrRefHighAwb;
    HI_U16  u16MeteringCrRefLowAwb;
    HI_U16  u16MeteringCbRefHighAwb;
    HI_U16  u16MeteringCbRefLowAwb;
} ISP_AWB_REG_CFG_3_S;

typedef struct hiISP_AF_REG_CFG_S
{
    HI_U16  u16ThresholdWrite;
    HI_U8   u8MetricsShift;
    HI_U8   u8NpOffset;
} ISP_AF_REG_CFG_S;

typedef struct hiISP_OFFSET_REG_CFG_S
{
    HI_U16  au16BlackLevelOffset[4];
} ISP_OFFSET_REG_CFG_S;

typedef struct hiISP_DRC_REG_CFG_S
{
	HI_U32 FrameExposure;
} ISP_DRC_REG_CFG_S;

typedef struct hiISP_DEHAZE_REG_CFG_S
{
    HI_U8   u8Strength;
    HI_U8   u8DehazeEnable;
    HI_U8  	u8AirR;
	HI_U8  	u8AirG;
	HI_U8  	u8AirB;
    HI_U32  u32Update;   
} ISP_DEHAZE_REG_CFG_S;

typedef struct hiISP_DP_REG_CFG_S
{
	HI_U32 u32DpccInSoftRst;		//U1.0
	HI_U32 u32DpccregsBayerPat;   //U2.0
	HI_U32 u32DpccCfg;              //U1.0  
	HI_U32 u32DpccAlpha;		      //U32.0
	HI_U32 u32DpccMode;             //U3.0    
	HI_U32 u32DpccOutputMode;      //U4.0    
	HI_U32 u32DpccSetUse;          //U4.0    
	HI_U32 u32DpccMethodsSet1;    //U13.0    
	HI_U32 u32DpccMethodsSet2;    //U13.0    
	HI_U32 u32DpccMethodsSet3;    //U13.0    
	HI_U32 u32DpccLineThresh1;    //U16.0    
	HI_U32 u32DpccLineMadFac1;   //U16.0    
	HI_U32 u32DpccPgFac1;         //U16.0    
	HI_U32 u32DpccRndThresh1;     //U16.0    
	HI_U32 u32DpccRgFac1;         //U16.0    
	HI_U32 u32DpccLineThresh2;    //U16.0    
	HI_U32 u32DpccLineMadFac2;   //U16.0    
	HI_U32 u32DpccPgFac2;         //U16.0    
	HI_U32 u32DpccRndThresh2;     //U16.0   
	HI_U32 u32DpccRgFac2;         //U16.0    
	HI_U32 u32DpccLineThresh3;    //U16.0    
	HI_U32 u32DpccLineMadFac3;   //U16.0    
	HI_U32 u32DpccPgFac3;         //U16.0    
	HI_U32 u32DpccRndThresh3;     //U16.0   
	HI_U32 u32DpccRgFac3;         //U16.0    
	HI_U32 u32DpccRoLimits;        //U12.0    
	HI_U32 u32DpccRndOffs;         //U12.0    
	HI_U32 u32DpccBptCtrl;         //U12.0    
	HI_U32 u32DpccBadThresh;       //U32.0    
	HI_U32 u32DpccBptNumber;       //U12.0
	HI_U32 au32DpccBpTable[2048];
} ISP_DP_REG_CFG_S;

typedef struct hiISP_NR_REG_CFG_S
{
	HI_U32	 rangeLowThres;	  // low  threshold for range -> blend			//10bit;u,10.0
	HI_U32	 rangeHigThres;	  // high threshold for range -> blend			//10bit;u,10.0
	
	HI_S32	 rangeGain;		  // luma adaptive								//13bit;s,12.0	[0, 4096]
	HI_U16	 rangeLumaLow; 	  // luma adaptive								//10bit;u,10.0
	HI_U16	 rangeLumaHig; 	  // luma adaptive								//10bit;u,10.0
	
	HI_S32	 rangeLowLUT[17];    // 10bit
	HI_S32	 rangeHigLUT[17];    // 10bit
	HI_U16	 blendLowLimit;	  // lower blend ratio	  0x000 => NR 100%		//9bit;u,9.0  [0, 0x100]
	HI_U16	 blendHigLimit;	  // higher blend ratio   0x100 => DM 100%		//9bit;u,9.0 
 
	HI_U8	 falseColorEnable;   // 1bit
	HI_U8	 falseColorShift;    // 8bit
	HI_U8	 falseColorGain;	   // 8bit
	HI_U8	 falseColorLimit;    // 8bit
	
	HI_U8    NrLutEnable;	   // 1bit Luma Noise Reduction Enable
	HI_U8    lumaNREnable;
	HI_U8    NrLumaNrGain;      // 

} ISP_NR_REG_CFG_S;

typedef struct hiISP_SHARPEN_REG_CFG_S
{
    HI_U8   G3[3][3];               // mid-band template                                                              // 8-bit  signed integer   // s8.0
    HI_U8  enPixSel;               // switch for selecting local max and min value in overshoot/undershoot control   // 1-bit unsigned integer  // U1.0
    HI_U16 u16SharpAmt1;              // mid-band sharpen strength                                                      // 12-bit unsigned integer // U12.0
    HI_U16 u16EdgeAmt;                // high-band sharpen strength                                                     // 12-bit unsigned integer // U12.0
    HI_U16 u16SharpThd1;              // threshold for sharp1                                                           // 16-bit unsigned integer // u16.0
    HI_U16 u16SharpThd2;              // threshold for sharp1                                                           // 16-bit unsigned integer // u16.0
    HI_U16 u16SharpThd1MulCoef;       // convert 1/(u16SharpThd1-u16SharpThd2) to multiplication                              // 12-bit unsigned integer // u12.0
    HI_U16 u16EdgeThd1;               // threshold for sharp1                                                           // 16-bit unsigned integer // u16.0
    HI_U16 u16EdgeThd2;               // threshold for sharp1                                                           // 16-bit unsigned integer // u16.0
    HI_U16 u16EdgeThd1MulCoef;        // convert 1/(u16EdgeThd1-u16EdgeThd2) to multiplication.                               // 12-bit unsigned integer // u12.0
    HI_U8  u8LineThd1;               // threshold for line detection                                                   // 8-bit unsigned integer    //U8.0
    HI_U8  u8LineThd2;               // threshold for line detection                                                   // 8-bit unsigned integer    //U8.0
    HI_U8  u8OvershootThd1;          // threshold for overshoot control                                                // 8-bit unsigned integer  // u8.0
    HI_U8  u8OvershootThd2;          // threshold for overshoot control                                                // 8-bit unsigned integer  // u8.0
    HI_U8  u8UndershootThd1;         // threshold for undershoot control                                               // 8-bit unsigned integer  // u8.0
    HI_U8  u8UndershootThd2;         // threshold for undershoot control                                               // 8-bit unsigned integer  // u8.0
    HI_U8  u8OvershootAmt;           // overshoot amplitude                                                            // 8-bit unsigned integer  // u8.0
    HI_U8  u8UndershootAmt;          // undershoot amplitude                                                           // 8-bit unsigned integer  // u8.0
    HI_U16 u16OvershootThdMulCoef;    // (256 - u8OvershootAmt)/(u8OvershootThd1-u8OvershootThd2)                             // 9-bit unsigned integer  // U1.8
    HI_U16 u16UndershootThdMulCoef;   // (256 - u8UndershootAmt)/(u8UndershootThd1-u8UndershootThd2)                          // 9-bit unsigned integer  // U1.8
} ISP_SHARPEN_REG_CFG_S;

typedef struct hiISP_GE_REG_CFG_S
{    
    HI_BOOL ge_enable;
    HI_U16  ct_th1;         //u14.0  
    HI_U16  ge_th;          //u14.0
    HI_U8   ge_th_slop;    //u4.0
    HI_U16  detail_th;      //u14.0
    HI_U8   detail_slop;   //u4.0
    HI_U8 u8GeStrength;
} ISP_GE_REG_CFG_S;

typedef struct hiISP_UVNR_REG_CFG_S
{
	HI_BOOL bUvnrSel;		//0: med; 1: uvnr
	HI_BOOL bNrEn;			//0: uvnr close; 1: uvnr open
	HI_BOOL bMidfEn;		//0: Med Filter close; 1: Med Filter open
	HI_BOOL bUv2cMode;		//UV2C bypass Mode, 0: U, 1: V
	HI_BOOL bUv2cEn;		//0: uv2c close; 1: uv2c open
	HI_BOOL bVcdsEn;		//UV vertical DS enable; 0: close; 1: open
	HI_BOOL bHcdsEn;		//UV Horizontal DS enable; 0: close; 1: open
	HI_S32 s32WidthOut;		
	HI_S32 s32WidthIn;	
	HI_BOOL bVcusMode;
	HI_BOOL bVcdsMode;
	HI_BOOL bHcdsMode;
	HI_S32 s32HCoef0;
	HI_S32 s32HCoef1;
	HI_S32 s32HCoef2;
	HI_S32 s32HCoef3;
	HI_S32 s32HCoef4;
	HI_S32 s32HCoef5;
	HI_S32 s32HCoef6;
	HI_S32 s32HCoef7;
	HI_S32 s32VCoef0;
	HI_S32 s32VCoef1;
	HI_S32 s32VCoef2;
	HI_S32 s32VCoef3;
	HI_S32 s32MfHeight;
	HI_S32 s32MfWidth;
	HI_S32 s32UvnrHeight;
	HI_S32 s32UvnrWidth;
	HI_S32 s32Diffthres6;         // u8
	HI_S32 s32Diffthres5;         // u8
	HI_S32 s32Diffthres4;         // u8
	HI_S32 s32Diffthres3;         // u8
	HI_S32 s32Diffthres2;         // u8
	HI_S32 s32Diffthres1;         // u8
	HI_S32 s32Ratiolimit6;         // u7
	HI_S32 s32Ratiolimit5;         // u7
	HI_S32 s32Ratiolimit4;         // u7
	HI_S32 s32ratiolimit3;         // u7
	HI_S32 s32Ratiolimit2;         // u7
	HI_S32 s32Ratiolimit1;         // u7
	HI_S32 s32Ratiolimit0;         // u7
	HI_S32 s32Blendratio;             // u6
	HI_S32 s32Diffshift;              // u2
	HI_U8  u8CoringLimit;               //u6
	HI_S32 s32SysWidthIn;
	HI_S32 s32SysHeightIn;
	HI_S32 s32CpiFixTimingStat;
	HI_BOOL bCpiAutoTimingEn;
} ISP_UVNR_REG_CFG_S;


typedef struct hiISP_WDR_REG_CFG_S
{
    HI_U8   u8TopChannelSwitch;
} ISP_WDR_REG_CFG_S;

typedef struct hiISP_LSC_REG_CFG
{	
    //HI_U32 noise_control[289];
    HI_U32 grr_gain[289];
	HI_U32 gbb_gain[289];
    HI_U32 xgrid[8];
    HI_U32 ygrid[8];
    HI_U32 xinvgrid[8];
    HI_U32 yinvgrid[8];
}ISP_LSC_REG_CFG_S;

typedef struct hiISP_DEMOSAIC_REG_CFG
{	
    HI_U8   u8hvColorRatio;
    HI_U8   u8FcrGain;
	HI_U16  u16AndPart1;            
    HI_U8   u8hvBlendLimit1;
    HI_U8   u8hvBlendLimit2;    
    HI_U8   u8FcrRatio     ;
    HI_U16  u16FcrLimit1   ;
    HI_U16  u16FcrLimit2   ;
    HI_U16  u16FcrThr      ;
}ISP_DEMOSAIC_REG_CFG_S;


typedef union hiISP_REG_KERNEL_CFG_KEY_U
{
    HI_U32  u32Key;
    struct
    {
        HI_U32  bit1UvnrCfg     : 1 ;   /* [0] */
        HI_U32  bit20Rsv        : 31;   /* [1:31] */
    };
} ISP_REG_KERNEL_CFG_KEY_U;

typedef struct hiISP_REG_KERNEL_CFG_S
{
    ISP_REG_KERNEL_CFG_KEY_U    unKey;
    ISP_UVNR_REG_CFG_S      stUVNrRegCfg;
} ISP_REG_KERNEL_CFG_S;

typedef union hiISP_REG_CFG_KEY_U
{
    HI_U32  u32Key;
    struct
    {
        HI_U32  bit1AeCfg1      : 1 ;   /* [0] */
        HI_U32  bit1AeCfg2      : 1 ;   /* [1] */
        HI_U32  bit1AwbCfg1     : 1 ;   /* [2] */
        HI_U32  bit1AwbCfg2     : 1 ;   /* [3] */
        HI_U32  bit1AfCfg       : 1 ;   /* [4] */
        HI_U32  bit1OffsetCfg   : 1 ;   /* [5] */
        HI_U32  bit1DrcCfg      : 1 ;   /* [6] */
        HI_U32  bit1NrCfg       : 1 ;   /* [7] */
        HI_U32  bit1DpCfg       : 1 ;   /* [8] */
        HI_U32  bit1SharpenCfg  : 1 ;   /* [9] */
        HI_U32  bit1GeCfg       : 1 ;   /* [10] */
        HI_U32  bit1WdrCfg      : 1 ;   /* [11] */
        HI_U32  bit1DehazeCfg   : 1 ;   /* [12] */
        HI_U32  bit1AwbCfg3     : 1 ;   /* [13] */
        HI_U32  bit1UvnrCfg     : 1 ;   /* [14] */
        HI_U32  bit1LscCfg		: 1 ;	/* [15] */
        HI_U32  bit1DemCfg		: 1 ;	/* [16] */
        HI_U32  bit16Rsv        : 15;   /* [17:31] */
    };
}ISP_REG_CFG_KEY_U;

typedef struct hiISP_REG_CFG_S
{
    ISP_REG_CFG_KEY_U       unKey;

    ISP_AE_REG_CFG_1_S      stAeRegCfg1;
    ISP_AE_REG_CFG_2_S      stAeRegCfg2;
    ISP_AWB_REG_CFG_1_S     stAwbRegCfg1;
    ISP_AWB_REG_CFG_2_S     stAwbRegCfg2;      /*Hi3518EV200 does't support RGB Configs*/
    ISP_AWB_REG_CFG_3_S     stAwbRegCfg3;
    ISP_AF_REG_CFG_S        stAfRegCfg;
    ISP_DP_REG_CFG_S        stDpRegCfg;
    ISP_DRC_REG_CFG_S       stDrcRegCfg;
    ISP_NR_REG_CFG_S        stNrRegCfg;
    ISP_GE_REG_CFG_S        stGeRegCfg;
    ISP_SHARPEN_REG_CFG_S   stSharpenRegCfg;
    ISP_OFFSET_REG_CFG_S    stOffsetRegCfg;
    ISP_WDR_REG_CFG_S       stWdrRegCfg;
	ISP_DEHAZE_REG_CFG_S    stDehazeRegCfg;
	ISP_UVNR_REG_CFG_S      stUVNrRegCfg;
	ISP_LSC_REG_CFG_S		stLscRegCfg;
	ISP_DEMOSAIC_REG_CFG_S  stDemRegCfg;
    ISP_REG_KERNEL_CFG_S    stKernelCfg;
} ISP_REG_CFG_S;

#define ISP_SYNC_BUF_NODE_NUM 5
#define CFG2VLD_DLY_LIMIT 4
#define WDR_EXP_RATIO_SHIFT 6
#define DRC_COMP_SHIFT 12

#define IS_LINEAR_MODE(mode)      (WDR_MODE_NONE == (mode))
#define IS_WDR_MODE(mode)         ((!IS_LINEAR_MODE(mode)) && ((mode) < WDR_MODE_BUTT))
#define IS_BUILT_IN_WDR_MODE(mode) (WDR_MODE_BUILT_IN == (mode))
#define IS_FS_WDR_MODE(mode)      ((!IS_LINEAR_MODE(mode))&& (!IS_BUILT_IN_WDR_MODE(mode)) && ((mode) < WDR_MODE_BUTT))
#define IS_2to1_WDR_MODE(mode)    ((WDR_MODE_2To1_FRAME == (mode)) || (WDR_MODE_2To1_FRAME_FULL_RATE == (mode)) || (WDR_MODE_2To1_LINE == (mode)))
#define IS_3to1_WDR_MODE(mode)    ((WDR_MODE_3To1_FRAME == (mode)) || (WDR_MODE_3To1_FRAME_FULL_RATE == (mode)) || (WDR_MODE_3To1_LINE == (mode)))
#define IS_4to1_WDR_MODE(mode)    ((WDR_MODE_4To1_FRAME == (mode)) || (WDR_MODE_4To1_FRAME_FULL_RATE == (mode)) || (WDR_MODE_4To1_LINE == (mode)))
#define IS_FULL_WDR_MODE(mode)    ((WDR_MODE_2To1_FRAME_FULL_RATE == (mode)) || (WDR_MODE_3To1_FRAME_FULL_RATE == (mode)) || (WDR_MODE_4To1_FRAME_FULL_RATE == (mode)))
#define IS_HALF_WDR_MODE(mode)    ((WDR_MODE_2To1_FRAME == (mode)) || (WDR_MODE_3To1_FRAME == (mode)) || (WDR_MODE_4To1_FRAME == (mode)))
#define IS_LINE_WDR_MODE(mode)    ((WDR_MODE_2To1_LINE == (mode)) || (WDR_MODE_3To1_LINE == (mode)) || (WDR_MODE_4To1_LINE == (mode)))

#define ISP_SET_MOD_PARAM      _IOW(IOC_TYPE_ISP, IOC_NR_ISP_SET_MOD_PARAM, ISP_MOD_PARAM_S)
#define ISP_GET_MOD_PARAM      _IOR(IOC_TYPE_ISP, IOC_NR_ISP_GET_MOD_PARAM, ISP_MOD_PARAM_S)

typedef struct hiISP_SYNC_CFG_BUF_NODE_S
{
    HI_BOOL bValid;
    
    ISP_SNS_REGS_INFO_S stSnsRegsInfo;
    ISP_AE_REG_CFG_2_S  stAERegCfg;
} ISP_SYNC_CFG_BUF_NODE_S;

typedef struct hiISP_REGCFG_S
{
    HI_BOOL bInit;
    ISP_REG_CFG_S stRegCfg;
    ISP_SYNC_CFG_BUF_NODE_S stSyncCfgNode;
} ISP_REGCFG_S;

typedef struct hiISP_WDR_CFG_S
{
    HI_U8 u8WDRMode;
} ISP_WDR_CFG_S;

typedef struct hiISP_RES_SWITCH_MODE_S
{
    HI_U16   u16Width;
    HI_U16   u16Height;
    HI_FLOAT f32Fps;
}ISP_RES_SWITCH_MODE_S;


typedef struct hiISP_PROC_MEM_S
{
    HI_U32 u32ProcSize;
    HI_U32 u32ProcPhyAddr;
    HI_VOID *pProcVirtAddr;
} ISP_PROC_MEM_S;

#define ISP_1ST_INT             0x1
#define ISP_2ND_INT             0x2
#define ISP_UNDEF_INT           0xF4

#define MAX_AE_LIB_NUM              2
#define MAX_AWB_LIB_NUM             2
#define MAX_AF_LIB_NUM              2

#define HI_AE_LIB_NAME "hisi_ae_lib"
#define HI_AWB_LIB_NAME "hisi_awb_lib"
#define HI_AF_LIB_NAME "hisi_af_lib"

#define ISP_DEV_SET_FD          _IOW(IOC_TYPE_ISP, IOC_NR_ISP_SET_FD, HI_S32)
#define ISP_GET_FRAME_EDGE      _IOR(IOC_TYPE_ISP, IOC_NR_ISP_GET_FRAME_EDGE, HI_S32)
#define ISP_GET_VD_TIMEOUT      _IOWR(IOC_TYPE_ISP, IOC_NR_ISP_GET_VD_TIMEOUT, ISP_VD_TIMEOUT_S)
#define ISP_SET_INT_ENABLE      _IOW(IOC_TYPE_ISP, IOC_NR_ISP_SET_INT_ENABLE, HI_BOOL)
#define ISP_GET_VERSION         _IOWR(IOC_TYPE_ISP, IOC_NR_ISP_GET_VERSION, ISP_VERSION_S)
#define ISP_STAT_BUF_INIT       _IOR(IOC_TYPE_ISP, IOC_NR_ISP_STAT_BUF_INIT, HI_U32)
#define ISP_STAT_BUF_EXIT       _IO(IOC_TYPE_ISP, IOC_NR_ISP_STAT_BUF_EXIT)
#define ISP_STAT_BUF_GET        _IOR(IOC_TYPE_ISP, IOC_NR_ISP_STAT_BUF_GET, ISP_STAT_INFO_S)
#define ISP_STAT_BUF_PUT        _IOW(IOC_TYPE_ISP, IOC_NR_ISP_STAT_BUF_PUT, ISP_STAT_INFO_S)

#define ISP_STAT_SHADOW_MEMPHY_GET     _IOR(IOC_TYPE_ISP, IOC_NR_ISP_STAT_SHADOW_MEMPHY_GET, HI_U32)
#define ISP_STAT_SHADOW_MEMSTATE_SET   _IOW(IOC_TYPE_ISP, IOC_NR_ISP_STAT_SHADOW_MEMSTATE_SET, HI_BOOL)

#define ISP_REG_CFG_INIT        _IOWR(IOC_TYPE_ISP, IOC_NR_ISP_REG_CFG_INIT, ISP_REG_CFG_S)
#define ISP_REG_CFG_SET         _IOW(IOC_TYPE_ISP, IOC_NR_ISP_REG_CFG_SET, ISP_REG_KERNEL_CFG_S)
#define ISP_MEM_INFO_SET        _IOW(IOC_TYPE_ISP, IOC_NR_ISP_MEM_INFO_SET, HI_BOOL)
#define ISP_MEM_INFO_GET        _IOR(IOC_TYPE_ISP, IOC_NR_ISP_MEM_INFO_GET, HI_BOOL)

#define ISP_PROC_INIT           _IOR(IOC_TYPE_ISP, IOC_NR_ISP_PROC_INIT, ISP_PROC_MEM_S)
#define ISP_PROC_WRITE_ING      _IO(IOC_TYPE_ISP, IOC_NR_ISP_PROC_WRITE_ING)
#define ISP_PROC_WRITE_OK       _IO(IOC_TYPE_ISP, IOC_NR_ISP_PROC_WRITE_OK)
#define ISP_PROC_EXIT           _IO(IOC_TYPE_ISP, IOC_NR_ISP_PROC_EXIT)
#define ISP_PROC_PARAM_GET      _IOR(IOC_TYPE_ISP, IOC_NR_ISP_PROC_PARAM_GET, HI_U32)
#define ISP_DCF_INFO_SET        _IOW(IOC_TYPE_ISP, IOC_NR_ISP_DCF_INFO_SET, ISP_SNS_REGS_INFO_S)
#define ISP_DCF_INFO_GET        _IOR(IOC_TYPE_ISP, IOC_NR_ISP_DCF_INFO_GET, ISP_SNS_REGS_INFO_S)
#define ISP_DCFINFO_BUF_INIT    _IOR(IOC_TYPE_ISP, IOC_NR_ISP_DCFINFO_BUF_INIT, HI_U32)
#define ISP_DCFINFO_BUF_EXIT    _IO(IOC_TYPE_ISP, IOC_NR_ISP_DCFINFO_BUF_EXIT)

#define ISP_SYNC_CFG_SET        _IOW(IOC_TYPE_ISP, IOC_NR_ISP_SYNC_CFG_SET, ISP_SYNC_CFG_BUF_NODE_S)
#define ISP_WDR_CFG_SET         _IOW(IOC_TYPE_ISP, IOC_NR_WDR_SYNC_CFG_SET, ISP_WDR_CFG_S)
#define ISP_RES_SWITCH_SET      _IOW(IOC_TYPE_ISP, IOC_NR_ISP_RES_SWITCH_SET, ISP_RES_SWITCH_MODE_S)
#define ISP_ACM_ATTR_GET        _IOR(IOC_TYPE_ISP, IOC_NR_ACM_ATTR_GET, ISP_ACM_ATTR_S)
#define ISP_ACM_ATTR_SET        _IOW(IOC_TYPE_ISP, IOC_NR_ACM_ATTR_SET, ISP_ACM_ATTR_S)
#define ISP_ACM_COEF_GET        _IOWR(IOC_TYPE_ISP, IOC_NR_ACM_COEF_GET, ISP_ACM_COEF_S)
#define ISP_ACM_COEF_SET        _IOW(IOC_TYPE_ISP, IOC_NR_ACM_COEF_SET, ISP_ACM_COEF_S)

#define ISP_PWM_NUM_GET        _IOR(IOC_TYPE_ISP, IOC_NR_ISP_PWM_NUM_GET, HI_U32)

#define ISP_GET_DEV(f)          ((HI_U32)((f)->private_data))

#define ISP_CHECK_DEV(dev)\
do {\
    if (0 != dev)\
    {\
        ISP_TRACE(HI_DBG_ERR, "Err isp dev %d in %s!\n", dev, __FUNCTION__);\
        return HI_ERR_ISP_ILLEGAL_PARAM;\
    }\
}while(0)    

#define ISP_CHECK_POINTER(ptr)\
do {\
    if (HI_NULL == ptr)\
    {\
        ISP_TRACE(HI_DBG_ERR, "Null Pointer in %s!\n", __FUNCTION__);\
        return HI_ERR_ISP_NULL_PTR;\
    }\
}while(0)

#define ISP_CHECK_BOOL(bool)\
do {\
    if (( HI_TRUE != bool)&&( HI_FALSE != bool))\
    {\
        ISP_TRACE(HI_DBG_ERR, "Invalid Isp Bool Type %d in %s!\n", bool, __FUNCTION__);\
        return HI_ERR_ISP_ILLEGAL_PARAM;\
    }\
}while(0)

#define ISP_DBG(bEn, fmt...)\
do{\
    if (bEn)\
        printf(fmt);\
}while(0)

#define ISP_AE_CHECK_HANDLE_ID(s32Handle)\
do {\
	if (((s32Handle) < 0) || ((s32Handle) >= MAX_AE_LIB_NUM))\
	{\
		ISP_TRACE(HI_DBG_ERR, "Illegal handle id %d in %s!\n", (s32Handle), __FUNCTION__);\
		return HI_FAILURE;\
	}\
}while(0)
	
#define ISP_AE_CHECK_LIB_NAME(acName)\
do {\
	if (0 != strcmp((acName), HI_AE_LIB_NAME))\
	{\
		ISP_TRACE(HI_DBG_ERR, "Illegal lib name %s in %s!\n", (acName), __FUNCTION__);\
		return HI_ERR_ISP_ILLEGAL_PARAM;\
	}\
}while(0)

#define ISP_AWB_CHECK_HANDLE_ID(s32Handle)\
do {\
	if (((s32Handle) < 0) || ((s32Handle) >= MAX_AWB_LIB_NUM))\
	{\
		ISP_TRACE(HI_DBG_ERR, "Illegal handle id %d in %s!\n", (s32Handle), __FUNCTION__);\
		return HI_FAILURE;\
	}\
}while(0)
	
#define ISP_AWB_CHECK_LIB_NAME(acName)\
do {\
	if (0 != strcmp((acName), HI_AWB_LIB_NAME))\
	{\
		ISP_TRACE(HI_DBG_ERR, "Illegal lib name %s in %s!\n", (acName), __FUNCTION__);\
		return HI_ERR_ISP_ILLEGAL_PARAM;\
	}\
}while(0)

#define ISP_AF_CHECK_HANDLE_ID(s32Handle)\
do {\
	if (((s32Handle) < 0) || ((s32Handle) >= MAX_AF_LIB_NUM))\
	{\
		ISP_TRACE(HI_DBG_ERR, "Illegal handle id %d in %s!\n", (s32Handle), __FUNCTION__);\
		return HI_FAILURE;\
	}\
}while(0)
	
#define ISP_AF_CHECK_LIB_NAME(acName)\
do {\
	if (0 != strcmp((acName), HI_AF_LIB_NAME))\
	{\
		ISP_TRACE(HI_DBG_ERR, "Illegal lib name %s in %s!\n", (acName), __FUNCTION__);\
		return HI_ERR_ISP_ILLEGAL_PARAM;\
	}\
}while(0)


#endif /* __HI3518_ISP_DEF_H__ */

