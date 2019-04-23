/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_fpn.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/07/24
  Description   : 
  History       :
  1.Date        : 2013/07/24
    Author      : z00186359
    Modification: Created file

******************************************************************************/

#include "isp_config.h"
#include "isp_alg.h"
#include "isp_sensor.h"
#include "isp_ext_config.h"


#include "hi_comm_vi.h"
#include "mpi_vi.h"
#include "mkp_vi.h"
#include "isp_proc.h"
#include <sys/ioctl.h>

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

/****************************************************************************
 * MACRO DEFINITION                                                         *
 ****************************************************************************/
#define FPN_OVERFLOWTHR			0x7C0
#define FPN_OVERFLOWTHR_OFF		0x3FFF

HI_S32 g_s32ViFd = -1;
HI_S32 g_s32Iso = 0;
HI_S32 u32IspIntCnt = 3;

#define FPN_CHN_NUM     1
        
#define CHECK_VI_DEV_OPEN(ViDev)\
do{\
    if (g_s32ViFd <= 0)\
    {\
        g_s32ViFd = HI_MPI_VI_GetFd(0);\
        if (g_s32ViFd < 0)\
        {\
            printf("open chn %d err, ret:%d \n", 0, g_s32ViFd);\
            return HI_ERR_VI_SYS_NOTREADY;\
        }\
    }\
}while(0)


HI_S32  VI_SetCalibrateAttr(VI_DEV ViDev, VI_FPN_ATTR_S *pstFPNAttr)
{
    CHECK_VI_DEV_OPEN(ViDev);

    return ioctl(g_s32ViFd, VIU_FPN_ATTR_SET_CTRL, pstFPNAttr);
}

HI_S32  VI_GetCorrectionAttr(VI_DEV ViDev, VI_FPN_ATTR_S *pstFPNAttr)
{
    CHECK_VI_DEV_OPEN(ViDev);

    return ioctl(g_s32ViFd, VIU_FPN_ATTR_GET_CTRL, pstFPNAttr);
}

HI_S32 ISP_CheckFpnConfig(ISP_FPN_TYPE_E enFpnType, 
                          const ISP_FPN_FRAME_INFO_S *pstFPNFrmInfo)
{
    COMPRESS_MODE_E	enCompressMode;
    PIXEL_FORMAT_E  enPixelFormat;
    
    enCompressMode = pstFPNFrmInfo->stFpnFrame.stVFrame.enCompressMode;
    enPixelFormat  = pstFPNFrmInfo->stFpnFrame.stVFrame.enPixelFormat;

    if ((enFpnType != ISP_FPN_TYPE_FRAME) && (enFpnType != ISP_FPN_TYPE_LINE))
    {
       ISP_TRACE(HI_DBG_ERR, "Invalid fpn Mode, should be in {line, frame} mode!\n");
       return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if ((enCompressMode != COMPRESS_MODE_NONE) && (enCompressMode != COMPRESS_MODE_LINE))
    {
        ISP_TRACE(HI_DBG_ERR, 
                  "fpn compress can only be {none(%d), frame(%d)} mode!\n",
                  COMPRESS_MODE_NONE, COMPRESS_MODE_LINE);
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (enFpnType == ISP_FPN_TYPE_LINE)
    {
        /* 只支持16bit */
        if (PIXEL_FORMAT_RGB_BAYER != enPixelFormat)
        {
            ISP_TRACE(HI_DBG_ERR, "only support 16bit fpn in line mode!\n");
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }

        if (COMPRESS_MODE_NONE != enCompressMode)
        {
            ISP_TRACE(HI_DBG_ERR, "fpn compress is not supported in line mode!\n");
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }
    else
    {
        if ((PIXEL_FORMAT_RGB_BAYER != enPixelFormat)
            && (PIXEL_FORMAT_RGB_BAYER_8BPP != enPixelFormat)
            && (PIXEL_FORMAT_RGB_BAYER_10BPP != enPixelFormat)
            && (PIXEL_FORMAT_RGB_BAYER_12BPP != enPixelFormat))
        {
            ISP_TRACE(HI_DBG_ERR, 
                "enPixelFormat invalid(%d), only support Raw8,Raw10,Raw12,Raw16.\n",
                enPixelFormat);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
        
        if ((COMPRESS_MODE_NONE != enCompressMode)
            && (PIXEL_FORMAT_RGB_BAYER == enPixelFormat))
        {
            ISP_TRACE(HI_DBG_ERR, "fpn compress is not supported in 16bit frame mode!\n");
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }

    return HI_SUCCESS;
}

HI_S32 ISP_CheckCalibrateAttr(VI_DEV ViDev, ISP_FPN_CALIBRATE_ATTR_S *pstCalibrate)
{
    HI_S32 s32Ret = HI_SUCCESS;
    ISP_FPN_TYPE_E enFpnType;
    const ISP_FPN_FRAME_INFO_S *pstFPNFrmInfo;

    enFpnType = pstCalibrate->enFpnType;
    pstFPNFrmInfo = &pstCalibrate->stFpnCaliFrame;
    s32Ret = ISP_CheckFpnConfig(enFpnType, pstFPNFrmInfo);
    return s32Ret;
}

HI_S32 ISP_CheckCorrectionAttr(VI_DEV ViDev, const ISP_FPN_ATTR_S *pstCorrection)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32Strength;
    ISP_FPN_TYPE_E enFpnType; 
    HI_U32 u32CalIso;
    const ISP_FPN_FRAME_INFO_S *pstFPNFrmInfo;
    
    if (pstCorrection->enOpType >= OP_TYPE_BUTT)
    {
        ISP_TRACE(HI_DBG_ERR,"Invalid fpn op type!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }
    if ((pstCorrection->bEnable != HI_TRUE)
        &&(pstCorrection->bEnable != HI_FALSE))
    {
       ISP_TRACE(HI_DBG_ERR,"Invalid fpn enable, must be {true, false}!\n");
       return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (OP_TYPE_MANUAL == pstCorrection->enOpType)
    {
        u32Strength   = pstCorrection->stManual.u32Strength;
        enFpnType     = pstCorrection->enFpnType;
        pstFPNFrmInfo = &pstCorrection->stFpnFrmInfo;

        if (u32Strength > 1023)
        {
            ISP_TRACE(HI_DBG_ERR, "Invalid fpn strength, over 1023!\n");
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
    }
    else
    {
        //u32Strength   = pstCorrection->stAuto.u32Strength;
        u32CalIso = pstCorrection->stFpnFrmInfo.u32Iso;
        if(0 == u32CalIso)
        {
            
            ISP_TRACE(HI_DBG_ERR, "Invalid fpn ISO(%d)!\n", u32CalIso);
            return HI_ERR_ISP_ILLEGAL_PARAM;
        }
        enFpnType     = pstCorrection->enFpnType;
        pstFPNFrmInfo = &pstCorrection->stFpnFrmInfo;
    }
    
    if (pstFPNFrmInfo->u32Offset > 0xFFF)
    {
       ISP_TRACE(HI_DBG_ERR, "Invalid fpn offset, can't be larger than 0xFFF!\n");
       return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    s32Ret = ISP_CheckFpnConfig(enFpnType, pstFPNFrmInfo);
    
    /* to do param check */
    return s32Ret;
}


HI_S32  ISP_SetCalibrateAttr(VI_DEV ViDev, ISP_FPN_CALIBRATE_ATTR_S *pstCalibrate)
{
    VI_FPN_ATTR_S stFpnAttr;
    HI_S32 s32Ret = HI_SUCCESS;    
    //ISP_CTX_S *pstIspCtx = HI_NULL;

    //ISP_GET_CTX(0, pstIspCtx);   
    //pstCalibrate->stFpnCaliFrame.u32Iso = pstIspCtx->stLinkage.u32SensorIso;
    pstCalibrate->stFpnCaliFrame.u32Iso = hi_ext_system_sensor_iso_read();
    
    s32Ret = ISP_CheckCalibrateAttr(ViDev, pstCalibrate);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    stFpnAttr.enFpnWorkMode = FPN_MODE_CALIBRATE;
    stFpnAttr.enFpnType     = pstCalibrate->enFpnType;    
   
    stFpnAttr.stCalibrate.u32Threshold = pstCalibrate->u32Threshold;
    stFpnAttr.stCalibrate.u32FrameNum  = pstCalibrate->u32FrameNum;

    memcpy(&stFpnAttr.stCalibrate.stFpnCaliFrame,
           &pstCalibrate->stFpnCaliFrame, 
           sizeof(ISP_FPN_FRAME_INFO_S));

    /* get calibrate result */
    s32Ret = VI_SetCalibrateAttr(ViDev, &stFpnAttr);

    /* get value from temp variable */
    memcpy(&pstCalibrate->stFpnCaliFrame, 
           &stFpnAttr.stCalibrate.stFpnCaliFrame,
           sizeof(ISP_FPN_FRAME_INFO_S));
    
    return s32Ret;
}

HI_U32 ISP_FPN_CLIP(HI_U32 u32Gain,HI_U32 u32Min, HI_U32 u32Max)
{
    if(u32Gain < u32Min)
    {
        return u32Min;
    }
    else if(u32Gain >u32Max)
    {
        return u32Max;
    }
    else
    {
        return u32Gain;
    }
}

inline HI_U32 ISP_Fpn_GetStrength(HI_U32 u32Iso, HI_S32 s32K1, HI_S32 s32K2, HI_S32 s32K3, HI_U32 u32CalibrateIso)
{
    HI_U32 u32Strength = 256;
    HI_U32 u32In;

    u32In = 256 * u32Iso /DIV_0_TO_1(u32CalibrateIso);
    //s32In = 256 * s32Iso /6400;
    //u32Strength = (s32K1 + s32K2*u32In + s32K3*u32In*u32In) >> 8;
    u32Strength = u32In;    
    return u32Strength;
}

HI_S32 ISP_SetCorrectionAttr(VI_DEV ViDev, const ISP_FPN_ATTR_S *pstCorrection)
{
    HI_S32 s32Ret = HI_SUCCESS;
    VI_FPN_ATTR_S stFpnAttr;
    ISP_CTX_S *pstIspCtx = HI_NULL;
    const ISP_FPN_FRAME_INFO_S *pstFPNFrmInfo = HI_NULL;
    HI_U32 u32Iso;
    VI_FPN_TYPE_E  enFpnType;
    HI_U32 u32Gain =0;
   
    s32Ret = ISP_CheckCorrectionAttr(ViDev, pstCorrection);    
    if (HI_SUCCESS != s32Ret)
    {        
        return s32Ret;
    }
    
    isp_ext_system_manual_fpn_opmode_write(pstCorrection->enOpType);

    /* 是否可以不用，待确认 */
 
	isp_ext_system_manual_fpn_strength_write(pstCorrection->stManual.u32Strength);

    if(OP_TYPE_AUTO == pstCorrection->enOpType)
    {
        ISP_GET_CTX(0, pstIspCtx);   
        //u32Iso = pstIspCtx->stLinkage.u32SensorIso;
        u32Iso = hi_ext_system_sensor_iso_read();
        isp_ext_system_manual_fpn_update_write(0x1);
        isp_ext_system_manual_FPN_ISO_write(pstCorrection->stFpnFrmInfo.u32Iso);
        //u32Gain = ISP_Fpn_GetStrength(s32Iso, 0, pstCorrection->stAuto.u32Strength, 0);

		/* Config FPN WDR mode correction threshold */
		if (IS_WDR_MODE(pstIspCtx->u8PreSnsWDRMode))
		{
		    hi_isp_fpn_coverflowthr_write(FPN_OVERFLOWTHR);
		}
		else
		{
			hi_isp_fpn_coverflowthr_write(FPN_OVERFLOWTHR_OFF);
		}
		
        /* Auto模式下k2配置为256 */
        u32Gain = ISP_Fpn_GetStrength(u32Iso, 0, 256, 0, pstCorrection->stFpnFrmInfo.u32Iso);
        enFpnType = pstCorrection->enFpnType;
        pstFPNFrmInfo = &pstCorrection->stFpnFrmInfo;
    }
    else
    {
        u32Gain       = pstCorrection->stManual.u32Strength;
        enFpnType     = pstCorrection->enFpnType;
        pstFPNFrmInfo = &pstCorrection->stFpnFrmInfo;
    }
	
    isp_ext_system_fpn_mode_write((HI_U16)enFpnType);
	isp_ext_system_fpn_bCompressed_write((HI_U16)pstFPNFrmInfo->stFpnFrame.stVFrame.enCompressMode);
	
    u32Gain = ISP_FPN_CLIP(u32Gain, 0, 1023);
    stFpnAttr.enFpnWorkMode            = FPN_MODE_CORRECTION;
    stFpnAttr.enFpnType                = enFpnType;   
    stFpnAttr.stCorrection.bEnable     = pstCorrection->bEnable;
    stFpnAttr.stCorrection.u32Gain     = u32Gain;
    stFpnAttr.stCorrection.enFpnOpType = pstCorrection->enOpType;  
   
    memcpy(&stFpnAttr.stCorrection.stFpnFrmInfo, pstFPNFrmInfo, 
           sizeof(ISP_FPN_FRAME_INFO_S));    
    
    s32Ret = VI_SetCalibrateAttr(ViDev, &stFpnAttr);
    if(HI_SUCCESS == s32Ret)
    {
        /*如果设置校正成功,则设置为已配置*/
        isp_ext_system_manual_fpn_CorrCfg_write(HI_TRUE);
    }
    else
    {
        /*如果设置校正不成功,则设置为未配置*/
        isp_ext_system_manual_fpn_CorrCfg_write(HI_FALSE);
    }
    
    return s32Ret;
}

HI_S32 ISP_GetCorrectionAttr(VI_DEV ViDev, ISP_FPN_ATTR_S *pstCorrection)
{
    HI_S32 s32Ret = HI_SUCCESS;
    VI_FPN_ATTR_S stTempViFpnAttr;
    ISP_FPN_FRAME_INFO_S *pstFPNFrmInfo;

    if(0 == isp_ext_system_manual_fpn_CorrCfg_read())
    {
       return HI_ERR_ISP_ATTR_NOT_CFG;
    }
    
    s32Ret =  VI_GetCorrectionAttr(ViDev, &stTempViFpnAttr);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    pstCorrection->bEnable  = stTempViFpnAttr.stCorrection.bEnable;
    pstCorrection->enOpType = stTempViFpnAttr.stCorrection.enFpnOpType;  
	
    /* auto.strength is equal to fpn gain, high 16 bits: gain */
    pstCorrection->stAuto.u32Strength = (hi_isp_fpn_corr_gainoffset_read(0) >> 16);
    if (OP_TYPE_MANUAL == pstCorrection->enOpType)
    {
        pstCorrection->stManual.u32Strength = isp_ext_system_manual_fpn_strength_read();
        pstCorrection->enFpnType   = stTempViFpnAttr.enFpnType;
        pstFPNFrmInfo = &pstCorrection->stFpnFrmInfo;
    }
    else
    {
        pstCorrection->enFpnType = stTempViFpnAttr.enFpnType;
        pstFPNFrmInfo = &pstCorrection->stFpnFrmInfo;
    }

    /* fpn frame info copy */
    memcpy(pstFPNFrmInfo, &stTempViFpnAttr.stCorrection.stFpnFrmInfo, sizeof(ISP_FPN_FRAME_INFO_S));

    return HI_SUCCESS;
}

HI_S32 ISP_CheckBLCAttr(const ISP_BLC_ATTR_S *pstBLCAttr)
{
    if (HI_FALSE != pstBLCAttr->bEnable && HI_TRUE != pstBLCAttr->bEnable)
    {
        ISP_TRACE(HI_DBG_ERR, "BLC bEnable should be {0, 1}!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    if (pstBLCAttr->u32BOffset >= 0xFF)
    {
        ISP_TRACE(HI_DBG_ERR, "BLC offset can't larger than 0xFF!\n");
        return HI_ERR_ISP_ILLEGAL_PARAM;
    }

    return HI_SUCCESS;
}

static HI_VOID FPNExtRegsDefault(HI_VOID)
{
    hi_ext_system_sensor_iso_write(HI_EXT_SYSTEM_SENSOR_ISO_DEFAULT);
	isp_ext_system_manual_FPN_ISO_write(6375);
    return;
}

static HI_VOID FPNRegsDefault(HI_VOID) 
{
    g_s32Iso = 0;  
    isp_ext_system_manual_fpn_CorrCfg_write(HI_FALSE);
    isp_ext_system_manual_fpn_strength_write(0);
    isp_ext_system_manual_fpn_opmode_write(0xf);
    isp_ext_system_manual_fpn_update_write(0);
    return;
}

static HI_VOID FPNExtRegsInitialize(ISP_DEV IspDev)
{
    return;
}

static HI_VOID FPNRegsInitialize(ISP_DEV IspDev)
{
    return;
}

static HI_S32 FPNReadExtregs(ISP_DEV IspDev)
{
    return 0;
}


HI_S32 FPNUpdateExtRegs(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_GET_CTX(IspDev, pstIspCtx);

    hi_ext_system_sensor_iso_write(pstIspCtx->stLinkage.u32SensorIso);
    
    return 0;
}

HI_VOID IspSetStrength(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;    
    HI_U32 u32Iso, u32Gain, Offfset, i;
    HI_U32 u32CalibrateIso;
    HI_U8  u8FpnOpMode, u8FpnEn;    
    HI_U32 u32GainOffset[FPN_CHN_NUM];
    
    ISP_GET_CTX(IspDev, pstIspCtx);   
  
    /* calculate every 120 interrupts */
    #if 0
    /* 连续校正时没有中断*/
    if (0 == pstIspCtx->u32FrameCnt % 120)
    {
        printf("\nisp auto set fpn's strength and offset!\n");
    }
    #endif
    
    if (0 != pstIspCtx->u32FrameCnt % u32IspIntCnt)
    {
        return ;
    }
    
    u8FpnOpMode = isp_ext_system_manual_fpn_opmode_read();    
    if(OP_TYPE_MANUAL == u8FpnOpMode)        
    {
        return ;
    }   
    
    //u32Iso = pstIspCtx->stLinkage.u32SensorIso;
    u32Iso = hi_ext_system_sensor_iso_read();
	u8FpnEn = hi_isp_fpn_enable_read();

	/* 配置FPN WDR模式的校正阀值 */
	if (u8FpnEn)
	{
    	/* 配置FPN WDR模式的校正阀值 */
		if (IS_WDR_MODE(pstIspCtx->u8PreSnsWDRMode))
		{
		    hi_isp_fpn_coverflowthr_write(FPN_OVERFLOWTHR);
		}
		else
		{
			hi_isp_fpn_coverflowthr_write(FPN_OVERFLOWTHR_OFF);
		}
	}
	
    if(u8FpnEn && ((g_s32Iso != u32Iso) || isp_ext_system_manual_fpn_update_read()))
    {     	
        if(isp_ext_system_manual_fpn_update_read())
        {
            isp_ext_system_manual_fpn_update_write(0);
        }

        /* 跟yanzhiwei讨论，auto时，会根据标定时ISO读出来，然后进行联动配置 */
        u32CalibrateIso = isp_ext_system_manual_FPN_ISO_read();        
        u32Gain = ISP_Fpn_GetStrength(u32Iso, 0 , 255, 0, u32CalibrateIso);        
        u32Gain = ISP_FPN_CLIP(u32Gain,0 ,1023);        
        for (i = 0; i < FPN_CHN_NUM; i++)
        {
            u32GainOffset[i] = hi_isp_fpn_corr_gainoffset_read(i);   
            Offfset  = u32GainOffset[i] & 0xFFF;
            u32GainOffset[i] = (u32Gain << 16) + Offfset;             
            hi_isp_fpn_corr_gainoffset_write(i, u32GainOffset[i]); 
        }  

        //printf("\ns32Isos =%d,32Strength= %d\n",s32Iso,u32Gain);           
        g_s32Iso = u32Iso;                  
    } 
    else
    {
        return ;

    }   
}

HI_S32 ISP_FPNInit(ISP_DEV IspDev)
{
    FPNRegsDefault();
    FPNExtRegsDefault();
	FPNReadExtregs(IspDev);
    FPNRegsInitialize(IspDev);
    FPNExtRegsInitialize(IspDev);
    
    return HI_SUCCESS;
}

HI_S32 ISP_FPNRun(ISP_DEV IspDev, const HI_VOID *pStatInfo,
    HI_VOID *pRegCfg, HI_S32 s32Rsv)
{
    FPNReadExtregs(IspDev);
    FPNUpdateExtRegs(IspDev);

	IspSetStrength(IspDev); 

    return HI_SUCCESS;
}

HI_S32 FpnProcWrite(ISP_DEV IspDev, ISP_CTRL_PROC_WRITE_S *pstProc)
{
    ISP_CTRL_PROC_WRITE_S stProcTmp;
    HI_U32 u32GainOffset;
 
    if ((HI_NULL == pstProc->pcProcBuff)
        || (0 == pstProc->u32BuffLen))
    {
        return HI_FAILURE;
    }

    stProcTmp.pcProcBuff = pstProc->pcProcBuff;
    stProcTmp.u32BuffLen = pstProc->u32BuffLen;
    
    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
        "-----FPN INFO------------------------------------------------------------------\n");
           
    ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
        "%8s" "%7s"     "%9s"       "%8s"      "%8s"      "%8s",
        "En", "OpType", "Strength", "Offset",  "Mode",    "CompressMode\n" );

    u32GainOffset = hi_isp_fpn_corr_gainoffset_read(0);
    
    if(hi_isp_fpn_enable_read())
    {
        ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
        "%8d" "%4d"  "%9d"  "%8d"   "%8d"  "%8d",
        hi_isp_fpn_enable_read(),
        isp_ext_system_manual_fpn_opmode_read(),
        u32GainOffset >> 16,
        u32GainOffset & 0xffff,
        isp_ext_system_fpn_mode_read(),
        isp_ext_system_fpn_bCompressed_read());
		
    }
    else
    {
        ISP_PROC_PRINTF(&stProcTmp, pstProc->u32WriteLen,
        "%8d" "%4s"  "%9s"  "%8s\n",
        hi_isp_fpn_enable_read(),
        "--",
        "--",
        "--");
    }

    pstProc->u32WriteLen += 1;
    
    return HI_SUCCESS;
}

HI_S32 ISP_FPNCtrl(ISP_DEV IspDev, HI_U32 u32Cmd, HI_VOID *pValue)
{
    switch (u32Cmd)
    {
        case ISP_PROC_WRITE:
            FpnProcWrite(IspDev, (ISP_CTRL_PROC_WRITE_S *)pValue);
            break;
            
        default :
            break;
    }
    return HI_SUCCESS;
}

HI_S32 ISP_FPNExit(ISP_DEV IspDev)
{
    return HI_SUCCESS;
}

HI_S32 ISP_AlgRegisterFPN(ISP_DEV IspDev)
{
    ISP_CTX_S *pstIspCtx = HI_NULL;
    ISP_ALG_NODE_S *pstAlgs = HI_NULL;
    
    ISP_GET_CTX(IspDev, pstIspCtx);

    pstAlgs = ISP_SearchAlg(pstIspCtx->astAlgs);
    ISP_CHECK_POINTER(pstAlgs);

    pstAlgs->enAlgType = ISP_ALG_FPN;
    pstAlgs->stAlgFunc.pfn_alg_init = ISP_FPNInit;
    pstAlgs->stAlgFunc.pfn_alg_run  = ISP_FPNRun;
    pstAlgs->stAlgFunc.pfn_alg_ctrl = ISP_FPNCtrl;
    pstAlgs->stAlgFunc.pfn_alg_exit = ISP_FPNExit;
    pstAlgs->bUsed = HI_TRUE;

    return HI_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

