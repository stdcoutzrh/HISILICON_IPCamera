/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_demosaic.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2015/05/11
  Description   : 
  History       :
  1.Date        : 2015/05/08
    Author      : LiShuiping l68041; Qiuleping q00214668;  Zhengzuquan z00225974;
    Modification: Created file

******************************************************************************/

#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_config.h"
#include "isp_ext_config.h"
#include "isp_proc.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#define HI_DEMOSAIC_BITDEPTH 12

/*For Demosaic*/
#define HI_ISP_DEMOSAIC_EN (1)
#define HI_ISP_DEMOSAIC_FCR_EN (1)
#define HI_ISP_DEMOSAIC_AHD_EN (1)
#define HI_ISP_DEMOSAIC_TEMPLATE_EN (0)
#define	HI_ISP_DEMOSAIC_BLEND_LIMIT1 (24)
#define	HI_ISP_DEMOSAIC_BLEND_LIMIT2 (40)
#define	HI_ISP_DEMOSAIC_BLEND_RATIO1 (0x0)
#define	HI_ISP_DEMOSAIC_BLEND_RATIO2 (0x100)
#define	HI_ISP_DEMOSAIC_HV_SEL (3)
#define	HI_ISP_DEMOSAIC_HV_CRATIO (48)
/*False Color*/
#define	HI_ISP_DEMOSAIC_FCR_THR (1)	
#define	HI_ISP_DEMOSAIC_FCR_LIMIT1 (4)
#define	HI_ISP_DEMOSAIC_FCR_LIMIT1_RANGE (HI_ISP_DEMOSAIC_FCR_LIMIT1)
#define	HI_ISP_DEMOSAIC_FCR_LIMIT2 (40)
#define	HI_ISP_DEMOSAIC_FCR_GAIN (8)
#define	HI_ISP_DEMOSAIC_FCR_GAIN_MAX (31)
#define	HI_ISP_DEMOSAIC_FCR_GAIN_RANGE (HI_ISP_DEMOSAIC_FCR_GAIN_MAX)
#define	HI_ISP_DEMOSAIC_FCR_RATIO (24)
#define	HI_ISP_DEMOSAIC_FCR_RATIO_MAX (63)
#define	HI_ISP_DEMOSAIC_FCR_RATIO_RANGE (HI_ISP_DEMOSAIC_FCR_RATIO_MAX-HI_ISP_DEMOSAIC_FCR_RATIO)
#define HI_ISP_DEMOSAIC_FCR_ISOTH1 (20000)
#define HI_ISP_DEMOSAIC_FCR_ISOTH2 (30000)
/*For Ahd*/
#define	HI_ISP_DEMOSAIC_AHD_PART2 (8)
#define	HI_ISP_DEMOSAIC_AHD_PART1 (400)
#define HI_ISP_DEMOSAIC_AHD_ISOTH1 (10000)
#define HI_ISP_DEMOSAIC_AHD_ISOTH2 (20000)

#define HI_ISP_DEMOSAIC_AHD_PAR1_MAX (1023)
#define HI_ISP_DEMOSAIC_AHD_PAR1_OFFSET_CENTER (512)

#ifndef MAX
#define MAX(a, b) (((a) < (b)) ?  (b) : (a))
#endif

#ifndef MIN
#define MIN(a, b) (((a) > (b)) ?  (b) : (a))
#endif

#define DEMOSAIC_CLIP3(low, high, x) (MAX( MIN((x), high), low))

static HI_U32 g_au32DemosaicIsoLut[ISP_AUTO_ISO_STENGTH_NUM] = {100,200,400,800,1600,3200,6400,12800,25600,51200,102400,204800,409600,819200,1638400,3276800};

static HI_U32 u32FcrShift = (HI_DEMOSAIC_BITDEPTH - 10 >= 0) ? (HI_DEMOSAIC_BITDEPTH - 10) : (10 - HI_DEMOSAIC_BITDEPTH);
	
typedef struct hiISP_DEMOSAIC_S
{
	/*For Demosaic*/
	HI_BOOL bEnable;			
	HI_U8   u8VhLimit; /* RW,Range: [0x0, 0xFF]  */
	HI_U8   u8VhOffset; /* RW,Range: [0x0, 0xFF]  */
	HI_U16  u16hvBlendLimit2;
	HI_U8   u8hvBlendLimit2;
	HI_U16  u16hvBlendRatio1; //const
	HI_U16  u16hvBlendRatio2; //const
	HI_U8   u8hvSel;
	HI_U16  u16VhSlope; /* RW,Range: [0x0, 0xFF]  */
	/*False Color*/
	HI_BOOL bFcrEnable;
	HI_U16  u16FcrThr;	
	HI_U16  u16FcrLimit1;
	HI_U16  u16FcrLimit2;
	HI_U8   u8FcrGain;
	HI_U8   u8FcrRatio;
	/*For Ahd*/
	HI_BOOL bAhdEnable;
	HI_U8   u8AndPart2;
	HI_U16  u16UuSlope;//u16UuSlope

    ISP_OP_TYPE_E enOpType;
    HI_U8 au8FcrAutoStrength[ISP_AUTO_ISO_STENGTH_NUM];
    HI_U8 au8FcrAutoThreshold[ISP_AUTO_ISO_STENGTH_NUM];
	HI_U16 au16NpOffset[ISP_AUTO_ISO_STENGTH_NUM];		
} ISP_DEMOSAIC_S;

ISP_DEMOSAIC_S g_astDemosaicCtx[ISP_MAX_DEV_NUM] = {{0}};
#define DEMOSAIC_GET_CTX(dev, pstCtx)   pstCtx = &g_astDemosaicCtx[dev]


static HI_VOID DemosaicExtRegsDefault(HI_VOID)
{
    return;
}

static HI_VOID DemosaicRegsDefault(HI_VOID) 
{
    return;
}

static HI_VOID DemosaicExtRegsInitialize(ISP_DEV IspDev)
{
    HI_U8 i;
    ISP_DEMOSAIC_S *pstDemosaic = HI_NULL;
    
    DEMOSAIC_GET_CTX(IspDev, pstDemosaic);

    hi_ext_system_fcr_manu_mode_write(pstDemosaic->enOpType);
	hi_ext_system_demosaic_vhSlope_write(pstDemosaic->u16VhSlope);
	hi_ext_system_demosaic_uuSlope_write(pstDemosaic->u16UuSlope);
	hi_ext_system_demosaic_vhLimit_write(pstDemosaic->u8VhLimit);
	hi_ext_system_demosaic_vhOffset_write(pstDemosaic->u8VhOffset);
    hi_ext_system_fcr_enable_write      (pstDemosaic->bFcrEnable);
    
    for (i = 0; i < ISP_AUTO_ISO_STENGTH_NUM; i++)
	{   
        hi_ext_system_demosaic_npOffset_write(i, pstDemosaic->au16NpOffset[i]);	
        hi_ext_system_fcr_auto_strength_write(i, pstDemosaic->au8FcrAutoStrength[i]);
        hi_ext_system_fcr_auto_threshold_write(i, pstDemosaic->au8FcrAutoThreshold[i]);
	}

    hi_ext_system_manual_fcr_strength_write(HI_EXT_SYSTEM_MANUAL_FCR_STRENGTH_WRITE_DEFAULT);
    hi_ext_system_manual_fcr_threshold_write(HI_EXT_SYSTEM_MANUAL_FCR_THRESHOLD_WRITE_DEFAULT);
    	
    return;
}

static HI_VOID DemosaicRegsInitialize(ISP_DEV IspDev)
{
	HI_U32 i;
	ISP_DEMOSAIC_S *pstDemosaic = HI_NULL;
	ISP_CMOS_DEFAULT_S *pstSnsDft = HI_NULL;
    HI_S16 s16FcrStrengthTemp = HI_ISP_DEMOSAIC_FCR_GAIN;
    HI_S16 s16FcrThresholdTemp= HI_ISP_DEMOSAIC_FCR_RATIO;
        	
    ISP_SensorGetDefault(IspDev, &pstSnsDft);
    
    DEMOSAIC_GET_CTX(IspDev, pstDemosaic);
	
    for(i=0; i < ISP_AUTO_ISO_STENGTH_NUM; i++)
    {	
	    pstDemosaic->au8FcrAutoStrength[i]   = pstSnsDft->stDemosaic.au8FcrStrength[i];	
	    pstDemosaic->au8FcrAutoThreshold[i]  = pstSnsDft->stDemosaic.au8FcrThreshold[i];
    		pstDemosaic->au16NpOffset[i]   = pstSnsDft->stDemosaic.au16NpOffset[i];
	} 	
    
    pstDemosaic->enOpType   = OP_TYPE_AUTO;
	pstDemosaic->bEnable    = pstSnsDft->stDemosaic.bEnable;
	pstDemosaic->bFcrEnable = pstSnsDft->stDemosaic.bFcrEnable;
	pstDemosaic->bAhdEnable = 1;//pstSnsDft->stDemosaic.bAhdEnable;
	
	pstDemosaic->u8VhLimit   = pstSnsDft->stDemosaic.u8VhLimit;
	pstDemosaic->u8VhOffset   = pstSnsDft->stDemosaic.u8VhOffset;	
	
	pstDemosaic->u16hvBlendLimit2 = (HI_U16)(pstDemosaic->u8VhLimit) + (HI_U16)(pstDemosaic->u8VhOffset) ;
	pstDemosaic->u8hvBlendLimit2 = (HI_U8)(MIN(255, pstDemosaic->u16hvBlendLimit2));
	pstDemosaic->u16hvBlendRatio1 = HI_ISP_DEMOSAIC_BLEND_RATIO1;//pstSnsDft->stDemosaic.u16hvBlendRatio1;
	pstDemosaic->u16hvBlendRatio2 = HI_ISP_DEMOSAIC_BLEND_RATIO2;//pstSnsDft->stDemosaic.u16hvBlendRatio2;
	
	pstDemosaic->u8hvSel           = HI_ISP_DEMOSAIC_HV_SEL;//pstSnsDft->stDemosaic.u8hvSel;
	pstDemosaic->u16VhSlope   = pstSnsDft->stDemosaic.u16VhSlope;
	
	/*False Color*/

      //pstDemosaic->u16FcrThr    = 1 ;//pstSnsDft->stDemosaic.u16FcrThr;	
      //pstDemosaic->u16FcrLimit1 = 4<<u32Shift ;//(pstSnsDft->stDemosaic.u16FcrLimit1) << u32Shift;
      //pstDemosaic->u16FcrLimit2 = 40<<u32Shift;//(pstSnsDft->stDemosaic.u16FcrLimit2) << u32Shift;
      //pstDemosaic->u8FcrStrength= pstSnsDft->stDemosaic.u8FcrStrength;
      //pstDemosaic->u8FcrRatio   = 24;//pstSnsDft->stDemosaic.u8FcrRatio;
	        
	        s16FcrStrengthTemp   = (HI_S16)(pstSnsDft->stDemosaic.au8FcrStrength[0]);
	        s16FcrThresholdTemp  = (HI_S16)(pstSnsDft->stDemosaic.au8FcrThreshold[0]);
	        
	        if( s16FcrStrengthTemp > HI_ISP_DEMOSAIC_FCR_GAIN_RANGE ) { pstDemosaic->u8FcrGain    = HI_ISP_DEMOSAIC_FCR_GAIN_MAX                          ;}
	        else                                                      { pstDemosaic->u8FcrGain    = (HI_U8)s16FcrStrengthTemp                             ;}
				                	           	        
	        s16FcrStrengthTemp-=HI_ISP_DEMOSAIC_FCR_GAIN_RANGE;         pstDemosaic->u16FcrLimit1 = HI_ISP_DEMOSAIC_FCR_LIMIT1 ;	              
	        if( s16FcrStrengthTemp > HI_ISP_DEMOSAIC_FCR_LIMIT1_RANGE){ pstDemosaic->u16FcrLimit1 = 0                                                     ;}
	        else if(s16FcrStrengthTemp > 0)                           { pstDemosaic->u16FcrLimit1 = (HI_U16)(pstDemosaic->u16FcrLimit1 - s16FcrStrengthTemp) ;}	        

	        s16FcrStrengthTemp-=HI_ISP_DEMOSAIC_FCR_LIMIT1_RANGE;       pstDemosaic->u16FcrLimit2 = HI_ISP_DEMOSAIC_FCR_LIMIT2;      
	        if(s16FcrStrengthTemp > 0)                                { pstDemosaic->u16FcrLimit2 = (HI_U16)(pstDemosaic->u16FcrLimit2 + s16FcrStrengthTemp) ;}
	        	        
	        pstDemosaic->u16FcrLimit1 <<= u32FcrShift ;
	        pstDemosaic->u16FcrLimit2 <<= u32FcrShift ;                   

	        if( s16FcrThresholdTemp > HI_ISP_DEMOSAIC_FCR_RATIO_RANGE ){ pstDemosaic->u8FcrRatio  = HI_ISP_DEMOSAIC_FCR_RATIO_MAX                         ;}
	        else                                                       { pstDemosaic->u8FcrRatio  = (HI_U8)s16FcrThresholdTemp                            ;}
	        
        if(pstSnsDft->stDemosaic.bFcrEnable)
        {	        	        
	        s16FcrThresholdTemp-=HI_ISP_DEMOSAIC_FCR_RATIO_RANGE;        pstDemosaic->u16FcrThr   = HI_ISP_DEMOSAIC_FCR_THR ; 	        
	        if(s16FcrThresholdTemp > 0)                                { pstDemosaic->u16FcrThr   = (HI_U16)(pstDemosaic->u16FcrThr    + s16FcrThresholdTemp);}   
        }
        else
        {
        	pstDemosaic->u16FcrThr   = 0 ;
        }       	
	/*For Ahd*/
	pstDemosaic->u8AndPart2  = HI_ISP_DEMOSAIC_AHD_PART2;//pstSnsDft->stDemosaic.u8AndPart2;
	pstDemosaic->u16UuSlope = pstSnsDft->stDemosaic.u16UuSlope;

	hi_isp_demosaic_cfg_enable_write(pstDemosaic->bEnable);
	hi_isp_demosaic_fcr_enable_write(pstDemosaic->bFcrEnable);
	hi_isp_demosaic_ahd_enable_write(pstDemosaic->bAhdEnable);
	hi_isp_demosaic_bld_limit1_write(pstDemosaic->u8VhLimit);
	hi_isp_demosaic_bld_limit2_write(pstDemosaic->u8hvBlendLimit2);
	hi_isp_demosaic_bld_ratio1_write(pstDemosaic->u16hvBlendRatio1);
	hi_isp_demosaic_bld_ratio2_write(pstDemosaic->u16hvBlendRatio2);
	hi_isp_demosaic_fcr_limit1_write(pstDemosaic->u16FcrLimit1);
	hi_isp_demosaic_fcr_limit2_write(pstDemosaic->u16FcrLimit2);
	hi_isp_demosaic_ahd_par1_write(pstDemosaic->u16UuSlope);
	hi_isp_demosaic_ahd_par2_write(pstDemosaic->u8AndPart2);
	hi_isp_demosaic_fcr_thr_write(pstDemosaic->u16FcrThr);
	hi_isp_demosaic_fcr_gain_write(pstDemosaic->u8FcrGain);
	hi_isp_demosaic_fcr_ratio_write(pstDemosaic->u8FcrRatio);
	hi_isp_demosaic_hv_ratio_write(pstDemosaic->u16VhSlope);
	hi_isp_demosaic_hv_sel_write(pstDemosaic->u8hvSel);

    return;
}

static HI_S32 DemosaicReadExtregs(ISP_DEV IspDev)
{   
	HI_U8 i;
    ISP_DEMOSAIC_S *pstDemosaic = HI_NULL;
	static ISP_DEMOSAIC_S PrestDemosaic = {0};
	HI_U8 update = 0;
    
    DEMOSAIC_GET_CTX(IspDev, pstDemosaic);

	pstDemosaic->u16VhSlope   = hi_ext_system_demosaic_vhSlope_read();
	pstDemosaic->u16UuSlope      = hi_ext_system_demosaic_uuSlope_read();

	pstDemosaic->u8VhLimit   = hi_ext_system_demosaic_vhLimit_read();
	pstDemosaic->u8VhOffset      = hi_ext_system_demosaic_vhOffset_read();
	pstDemosaic->bFcrEnable = hi_ext_system_fcr_enable_read();

    for (i = 0; i < ISP_AUTO_ISO_STENGTH_NUM; i++)
	{
        pstDemosaic->au16NpOffset[i]       = hi_ext_system_demosaic_npOffset_read(i);
	    pstDemosaic->au8FcrAutoStrength[i] = hi_ext_system_fcr_auto_strength_read(i);	
	    pstDemosaic->au8FcrAutoThreshold[i]= hi_ext_system_fcr_auto_threshold_read(i);
	}		
	if (OP_TYPE_MANUAL == hi_ext_system_fcr_manu_mode_read())
	{
	    update = 1;
	}
	if (PrestDemosaic.bFcrEnable != pstDemosaic->bFcrEnable)
    {
        update = 1;
	}
    if (PrestDemosaic.u16VhSlope != pstDemosaic->u16VhSlope)
    {
        update = 1;
	}
    if (PrestDemosaic.u16UuSlope != pstDemosaic->u16UuSlope)
    {
       update = 1;
	}
	if (PrestDemosaic.u8VhLimit != pstDemosaic->u8VhLimit)
    {
		update = 1;
	}
	if (PrestDemosaic.u8VhOffset !=pstDemosaic->u8VhOffset)
    {
		update = 1;
	}

	for (i = 0; i < ISP_AUTO_ISO_STENGTH_NUM; i++)
	{
		if(PrestDemosaic.au16NpOffset[i] != pstDemosaic->au16NpOffset[i])
		{
			update = 1;
			break;
		}
	   else	if(PrestDemosaic.au8FcrAutoStrength[i] != pstDemosaic->au8FcrAutoStrength[i])
		{
			update = 1;
			break;
		}
	   else	if(PrestDemosaic.au8FcrAutoThreshold[i] != pstDemosaic->au8FcrAutoThreshold[i])
		{
			update = 1;
			break;
		}
	   else 
	   	{
          
	    }
	}	

	PrestDemosaic.bFcrEnable = pstDemosaic->bFcrEnable;
	PrestDemosaic.u16VhSlope = pstDemosaic->u16VhSlope;
	PrestDemosaic.u16UuSlope = pstDemosaic->u16UuSlope;
	PrestDemosaic.u8VhLimit = pstDemosaic->u8VhLimit;
	PrestDemosaic.u8VhOffset =pstDemosaic->u8VhOffset;
	
	for (i = 0; i < ISP_AUTO_ISO_STENGTH_NUM; i++)
	{
        PrestDemosaic.au16NpOffset[i]       = pstDemosaic->au16NpOffset[i];
	    PrestDemosaic.au8FcrAutoStrength[i] = pstDemosaic->au8FcrAutoStrength[i];	
	    PrestDemosaic.au8FcrAutoThreshold[i]= pstDemosaic->au8FcrAutoThreshold[i];
	}	

    return update;
}

HI_S32 DemosaicProcWrite(ISP_DEV IspDev, ISP_CTRL_PROC_WRITE_S *pstProc)
{
    ISP_CTRL_PROC_WRITE_S stProcTmp;
    ISP_DEMOSAIC_S *pstDemosaic = HI_NULL;
    
    DEMOSAIC_GET_CTX(IspDev, pstDemosaic);
 
    if ((HI_NULL == pstProc->pcProcBuff)
        || (0 == pstProc->u32BuffLen))
    {
        return HI_FAILURE;
    }

    stProcTmp.pcProcBuff = pstProc->pcProcBuff;
    stProcTmp.u32BuffLen = pstProc->u32BuffLen;


    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
        "-----DEMOSAIC INFO-------------------------------------------------------------\n");

    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
            "%10s"    "%10s"    "%10s"    "%10s"  ,
            "VhSlope" ,  "UuSlope" , "VhLimit" ,  "VhOffset\n");

    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
            "%10d"    "%10d"     "%10d"    "%10d     \n" ,
            pstDemosaic->u16VhSlope,
            pstDemosaic->u16UuSlope,
            pstDemosaic->u8VhLimit,
            pstDemosaic->u8VhOffset
            );

    pstProc->u32WriteLen += 1;
    
    return HI_SUCCESS;
}


HI_S32 ISP_DemosaicInit(ISP_DEV IspDev)
{
    DemosaicRegsDefault();
    DemosaicExtRegsDefault();
    DemosaicRegsInitialize(IspDev);
    DemosaicExtRegsInitialize(IspDev);
    
    return HI_SUCCESS;
}

static HI_U8 DemosaicGetIsoIndex(HI_U32 u32Iso)
{   
	HI_U8 u8Index;
	
    for (u8Index = 0; u8Index < ISP_AUTO_ISO_STENGTH_NUM-1; u8Index++)
    {
        if (u32Iso <= g_au32DemosaicIsoLut[u8Index])
        {
            break;
        }
    }
	
    return u8Index;
}

static __inline HI_U16 NrOffsetCalculate(
    const HI_U16 u16Y2,
    const HI_U16 u16Y1,
    const HI_U32 u32X2,
    const HI_U32 u32X1,
    const HI_U32 u32Iso)    
{
    HI_U32 u32Offset;
	        if(u32X1==u32X2){u32Offset = u16Y2;}
	   else if(u16Y1 <= u16Y2){u32Offset = u16Y1+(ABS(u16Y2 - u16Y1) * ABS(u32Iso - u32X1)+ ABS((u32X2 - u32X1)/2))/ABS((u32X2 - u32X1));}
	   else if(u16Y1> u16Y2){u32Offset = u16Y1-(ABS(u16Y2 - u16Y1) * ABS(u32Iso - u32X1)+ ABS((u32X2 - u32X1)/2))/ABS((u32X2 - u32X1));}
    return (HI_U16)u32Offset;
}

HI_U16 GetAHDAdptiveGain(ISP_DEV IspDev, HI_U32 eIsoLevel, HI_U32 u32ISO2, HI_U32 u32ISO1, HI_S32 ISO)
{
    HI_U16 u16Y2;
    HI_U16 u16Y1;    
	HI_S32 gain_ret;
	
    ISP_DEMOSAIC_S *pstDemosaic = HI_NULL;
    
    DEMOSAIC_GET_CTX(IspDev, pstDemosaic);

    u16Y1=(eIsoLevel)?(HI_U16)pstDemosaic->au16NpOffset[eIsoLevel-1] : (HI_U16)pstDemosaic->au16NpOffset[eIsoLevel];
	u16Y2=            (HI_U16)pstDemosaic->au16NpOffset[eIsoLevel];

    gain_ret = pstDemosaic->u16UuSlope + NrOffsetCalculate(u16Y2, u16Y1, u32ISO2, u32ISO1, ISO) - HI_ISP_DEMOSAIC_AHD_PAR1_OFFSET_CENTER;   
	
    if(gain_ret > HI_ISP_DEMOSAIC_AHD_PAR1_MAX)
    {
        gain_ret = HI_ISP_DEMOSAIC_AHD_PAR1_MAX; 
    }
    
    if(gain_ret < 0)
    {
        gain_ret = 0;
    }
	
	return (HI_U16)gain_ret;
}

HI_S32 ISP_DemosaicFw(HI_U32 u32Iso, ISP_DEV IspDev, ISP_DEMOSAIC_REG_CFG_S *pstDmCfg)
{
    HI_U8 u8IsoLevel;
	ISP_DEMOSAIC_S *pstDemosaic = HI_NULL;

    HI_U16 u16Y2;
    HI_U16 u16Y1;    
	HI_U32 u32ISO1 = 0;
    HI_U32 u32ISO2 = 0;
    HI_S16 s16FcrStrengthTemp = HI_ISP_DEMOSAIC_FCR_GAIN;
    HI_S16 s16FcrThresholdTemp= HI_ISP_DEMOSAIC_FCR_RATIO;
        
    if (u32Iso > g_au32DemosaicIsoLut[15])
    {
        u8IsoLevel = 15;
        u32ISO1 = g_au32DemosaicIsoLut[15];
        u32ISO2 = g_au32DemosaicIsoLut[15]; 	
    }
    else if (u32Iso <= g_au32DemosaicIsoLut[0])
    {
        u8IsoLevel = 0;
        u32ISO1 = 0;
        u32ISO2 = g_au32DemosaicIsoLut[0]; 		
    }	
    else
    {
        u8IsoLevel = DemosaicGetIsoIndex(u32Iso);
        u32ISO1 = g_au32DemosaicIsoLut[u8IsoLevel-1];
        u32ISO2 = g_au32DemosaicIsoLut[u8IsoLevel]; 
    }
    
    DEMOSAIC_GET_CTX(IspDev, pstDemosaic);

	pstDmCfg->u8hvColorRatio = pstDemosaic->u16VhSlope;
	pstDmCfg->u16AndPart1    = GetAHDAdptiveGain(IspDev, u8IsoLevel, u32ISO2, u32ISO1, u32Iso);
	 
	pstDmCfg->u8hvBlendLimit1 = pstDemosaic->u8VhLimit;
	pstDemosaic->u16hvBlendLimit2 = (HI_U16)(pstDemosaic->u8VhLimit) + (HI_U16)(pstDemosaic->u8VhOffset) ;
	pstDemosaic->u8hvBlendLimit2 = (HI_U8)(MIN(255, pstDemosaic->u16hvBlendLimit2));	
	pstDmCfg->u8hvBlendLimit2 = pstDemosaic->u8hvBlendLimit2;

	if(hi_ext_system_fcr_enable_read()) 
      {  
		//hi_isp_demosaic_fcr_thr_write(1);
		
                if (OP_TYPE_AUTO == hi_ext_system_fcr_manu_mode_read())
                {
                    u16Y1 = (u8IsoLevel)?(HI_U16)pstDemosaic->au8FcrAutoStrength[u8IsoLevel-1]:pstDemosaic->au8FcrAutoStrength[0];
                    u16Y2 =            (HI_U16)pstDemosaic->au8FcrAutoStrength[u8IsoLevel];                
                    s16FcrStrengthTemp   = (HI_S16)NrOffsetCalculate(u16Y2, u16Y1, u32ISO2, u32ISO1, u32Iso);
                 		        
                    u16Y1 = (u8IsoLevel)?(HI_U16)pstDemosaic->au8FcrAutoThreshold[u8IsoLevel-1]:pstDemosaic->au8FcrAutoThreshold[0];
                    u16Y2 =             (HI_U16)pstDemosaic->au8FcrAutoThreshold[u8IsoLevel];                
                    s16FcrThresholdTemp   = (HI_S16)NrOffsetCalculate(u16Y2, u16Y1, u32ISO2, u32ISO1, u32Iso);
                }
		
                if (OP_TYPE_MANUAL == hi_ext_system_fcr_manu_mode_read())
                {
                    s16FcrStrengthTemp   =  (HI_S16)hi_ext_system_manual_fcr_strength_read();            	    
                    s16FcrThresholdTemp  =  (HI_S16)hi_ext_system_manual_fcr_threshold_read();  
                }
                
	        if( s16FcrStrengthTemp > HI_ISP_DEMOSAIC_FCR_GAIN_RANGE ) { pstDmCfg->u8FcrGain    = HI_ISP_DEMOSAIC_FCR_GAIN_MAX                          ;}
	        else                                                      { pstDmCfg->u8FcrGain    = (HI_U8)s16FcrStrengthTemp                             ;}
				                	           	        
	        s16FcrStrengthTemp-=HI_ISP_DEMOSAIC_FCR_GAIN_RANGE;         pstDmCfg->u16FcrLimit1 = HI_ISP_DEMOSAIC_FCR_LIMIT1 ;	              
	        if( s16FcrStrengthTemp > HI_ISP_DEMOSAIC_FCR_LIMIT1_RANGE){ pstDmCfg->u16FcrLimit1 = 0                                                     ;}
	        else if(s16FcrStrengthTemp > 0)                           { pstDmCfg->u16FcrLimit1 = (HI_U16)(pstDmCfg->u16FcrLimit1 - s16FcrStrengthTemp) ;}	        

	        s16FcrStrengthTemp-=HI_ISP_DEMOSAIC_FCR_LIMIT1_RANGE;       pstDmCfg->u16FcrLimit2 = HI_ISP_DEMOSAIC_FCR_LIMIT2;      
	        if(s16FcrStrengthTemp > 0)                                { pstDmCfg->u16FcrLimit2 = (HI_U16)(pstDmCfg->u16FcrLimit2 + s16FcrStrengthTemp) ;}
	        	        
	        pstDmCfg->u16FcrLimit1 <<= u32FcrShift ;
	        pstDmCfg->u16FcrLimit2 <<= u32FcrShift ;                   

	        if( s16FcrThresholdTemp > HI_ISP_DEMOSAIC_FCR_RATIO_RANGE ){ pstDmCfg->u8FcrRatio  = HI_ISP_DEMOSAIC_FCR_RATIO_MAX                         ;}
	        else                                                       { pstDmCfg->u8FcrRatio  = (HI_U8)s16FcrThresholdTemp                            ;}
	        	        
	        s16FcrThresholdTemp-=HI_ISP_DEMOSAIC_FCR_RATIO_RANGE;        pstDmCfg->u16FcrThr   = HI_ISP_DEMOSAIC_FCR_THR ; 	        
	        if(s16FcrThresholdTemp > 0)                                { pstDmCfg->u16FcrThr   = (HI_U16)(pstDmCfg->u16FcrThr    + s16FcrThresholdTemp);}  	             	        
	}
	else
	{
            pstDmCfg->u16FcrThr=0;
	}
	
  
    	
    return	HI_SUCCESS;
}

HI_S32 ISP_DemosaicRun(ISP_DEV IspDev, const HI_VOID *pStatInfo,
    HI_VOID *pRegCfg, HI_S32 s32Rsv)
{    
	ISP_REG_CFG_S *pstReg;
    ISP_CTX_S *pstIspCtx = HI_NULL;
    static HI_U32 u32PrevIso = 0,u32PreDemosaicEn = 0; 
	HI_U32 u32Ret;
    ISP_GET_CTX(IspDev, pstIspCtx);

    /* calculate every two interrupts */
	
    if (0 != pstIspCtx->u32FrameCnt % 2)
    {
        return HI_SUCCESS;
    }
	
   
	u32Ret = DemosaicReadExtregs(IspDev);

	if((0 == hi_isp_demosaic_cfg_enable_read())&&(u32Ret == 0))
	{
   
	   ((ISP_REG_CFG_S *)pRegCfg)->unKey.bit1DemCfg = 0;	
       return HI_SUCCESS;
	}
	
    if((u32PrevIso == pstIspCtx->stLinkage.u32Iso)&&(u32PreDemosaicEn == hi_isp_demosaic_cfg_enable_read()))
    {
      if(u32Ret == 0)
      	{
	      ((ISP_REG_CFG_S *)pRegCfg)->unKey.bit1DemCfg = 0;	
	      return HI_SUCCESS;    
      	}
	}
	
	pstReg = (ISP_REG_CFG_S *)pRegCfg;
	ISP_DemosaicFw(pstIspCtx->stLinkage.u32Iso, IspDev, &pstReg->stDemRegCfg);

	((ISP_REG_CFG_S *)pRegCfg)->unKey.bit1DemCfg = 1;	
	
    return HI_SUCCESS;
}

HI_S32 ISP_DemosaicCtrl(ISP_DEV IspDev, HI_U32 u32Cmd, HI_VOID *pValue)
{
    switch (u32Cmd)
    {
        case ISP_WDR_MODE_SET :
            ISP_DemosaicInit(IspDev);
            break;
        case ISP_PROC_WRITE:
            DemosaicProcWrite(IspDev, (ISP_CTRL_PROC_WRITE_S *)pValue);
            break;
        default :
            break;
    }
    return HI_SUCCESS;
}

HI_S32 ISP_DemosaicExit(ISP_DEV IspDev)
{
	//hi_isp_demosaic_cfg_enable_write(HI_FALSE);
	//hi_isp_demosaic_fcr_enable_write(HI_FALSE);
	//hi_isp_demosaic_ahd_enable_write(HI_FALSE);
    return HI_SUCCESS;
}

HI_S32 ISP_AlgRegisterDemosaic(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_ALG_NODE_S *pstAlgs = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->enAlgType = ISP_ALG_DEMOSAIC;
    pstAlgs->stAlgFunc.pfn_alg_init = ISP_DemosaicInit;
    pstAlgs->stAlgFunc.pfn_alg_run  = ISP_DemosaicRun;
    pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_DemosaicCtrl;
    pstAlgs->stAlgFunc.pfn_alg_exit = ISP_DemosaicExit;
    pstAlgs->bUsed = HI_TRUE;

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


