/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : mkp_sys.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2007/1/31
  Description   : hi3511 system private Head File
  History       :
  1.Date        : 2007/1/31
    Author      : c42025
    Modification: Created file

******************************************************************************/
#ifndef  __MKP_SYS_H__
#define  __MKP_SYS_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#include "mkp_ioctl.h"
#include "hi_common.h"
#include "hi_comm_sys.h"

#define SYS_CHECK_NULL_PTR(ptr)\
do{\
    if (NULL == ptr)\
    {\
        HI_TRACE_SYS(HI_DBG_ERR, "Null point \n");\
        return HI_ERR_SYS_NULL_PTR;\
    }\
} while(0)


typedef enum hiMPP_DATA_TYPE_E
{
    MPP_DATA_VIU_FRAME,     /* VIDEO_FRAME_INFO_S   */
    MPP_DATA_VOU_FRAME,     /* VIDEO_FRAME_INFO_S */
    MPP_DATA_VDEC_FRAME,    /* VIDEO_FRAME_INFO_S   */
    MPP_DATA_VIDEO_FRAME,   /* VIDEO_FRAME_INFO_S   */
    MPP_DATA_VOU_WBC_FRAME,
    MPP_DATA_AUDIO_FRAME,   /* */

    MPP_DATA_BUTT
} MPP_DATA_TYPE_E;

#define BIND_SRC_MAXNUM 64

typedef struct hiMPP_BIND_SRC_S
{
    HI_U32    u32Num;
    MPP_CHN_S astMppChn[BIND_SRC_MAXNUM];
}MPP_BIND_SRC_S;

typedef struct hiBIND_SENDER_INFO_S
{
    MOD_ID_E    enModId;
    HI_U32      u32MaxDevCnt;   /* If no dev id, should set it 1 */
    HI_U32      u32MaxChnCnt;
    HI_S32      (*pGiveBindCallBack)(HI_S32 s32DevId, HI_S32 s32ChnId, MPP_BIND_SRC_S *pstBindSend);
} BIND_SENDER_INFO_S;

typedef struct hiBIND_RECEIVER_INFO_S
{
    MOD_ID_E    enModId;
    HI_U32      u32MaxDevCnt;   /* If no dev id, should set it 1 */
    HI_U32      u32MaxChnCnt;
    HI_S32      (*pCallBack)(HI_S32 s32DevId, HI_S32 s32ChnId, HI_BOOL bBlock,
        MPP_DATA_TYPE_E enDataType, HI_VOID *pvData);
    HI_S32      (*pResetCallBack)(HI_S32 s32DevId, HI_S32 s32ChnId, HI_VOID *pvData);
} BIND_RECEIVER_INFO_S;

typedef struct hiSYS_BIND_ARGS_S
{
    MPP_CHN_S stSrcChn;
    MPP_CHN_S stDestChn;
} SYS_BIND_ARGS_S;

#define SYS_DDR_MAXNUM 16
typedef struct hiMPP_SYS_DDR_NAME_S
{
    HI_U32    u32Num;
    HI_VOID *apMmzName[SYS_DDR_MAXNUM];
}MPP_SYS_DDR_NAME_S;

typedef struct hiSYS_MEM_ARGS_S
{
    MPP_CHN_S stMppChn;
    HI_CHAR acMmzName[MAX_MMZ_NAME_LEN];
}SYS_MEM_ARGS_S;

typedef struct hiKERNEL_CONFIG_S
{
    HI_S32 s32HRTimer;
    HI_S32 s32RRMode;
}KERNEL_CONFIG_S;

typedef struct hiSYS_MEM_CACHE_INFO_S
{
    HI_U32  u32PhyAddr;
    HI_VOID *pVirAddr;
    HI_U32  u32Size;
}SYS_MEM_CACHE_INFO_S;

typedef enum hiIOC_NR_SYS_E
{
	IOC_NR_SYS_INIT = 0,
	IOC_NR_SYS_EXIT,
	IOC_NR_SYS_SETCONFIG,
	IOC_NR_SYS_GETCONFIG,
	IOC_NR_SYS_INITPTSBASE,
    IOC_NR_SYS_SYNCPTS,
	IOC_NR_SYS_GETCURPTS,

    IOC_NR_SYS_BIND,
    IOC_NR_SYS_UNBIND,
    IOC_NR_SYS_GETBINDBYDEST,

    IOC_NR_SYS_MEM_SET,
    IOC_NR_SYS_MEM_GET,

    IOC_NR_SYS_GET_CUST_CODE,

    IOC_NR_SYS_SET_WIZARD,

    IOC_NR_SYS_GET_KERNELCONFIG,

    IOC_NR_SYS_GET_CHIPID,    

    IOC_NR_SYS_GET_VIVPSS_MODE,

    IOC_NR_SYS_SET_PROFILE,
    IOC_NR_SYS_GET_PROFILE,
    
    IOC_NR_SYS_SET_RESULT,
    IOC_NR_SYS_GET_RESULT,

    IOC_NR_SYS_MFLUSH_CACHE,
	
    IOC_NR_SYS_SET_SCALE_COEFF,
    IOC_NR_SYS_GET_SCALE_COEFF,
} IOC_NR_SYS_E;


#define SYS_INIT_CTRL          _IO(IOC_TYPE_SYS, IOC_NR_SYS_INIT)
#define SYS_EXIT_CTRL          _IO(IOC_TYPE_SYS, IOC_NR_SYS_EXIT)
#define SYS_SET_CONFIG_CTRL    _IOW(IOC_TYPE_SYS, IOC_NR_SYS_SETCONFIG, MPP_SYS_CONF_S)
#define SYS_GET_CONFIG_CTRL    _IOR(IOC_TYPE_SYS, IOC_NR_SYS_GETCONFIG, MPP_SYS_CONF_S)
#define SYS_INIT_PTSBASE       _IOW(IOC_TYPE_SYS, IOC_NR_SYS_INITPTSBASE, HI_U64)
#define SYS_SYNC_PTS           _IOW(IOC_TYPE_SYS, IOC_NR_SYS_SYNCPTS, HI_U64)
#define SYS_GET_CURPTS         _IOR(IOC_TYPE_SYS, IOC_NR_SYS_GETCURPTS, HI_U64)

#define SYS_BIND_CTRL          _IOW(IOC_TYPE_SYS, IOC_NR_SYS_BIND, SYS_BIND_ARGS_S)
#define SYS_UNBIND_CTRL        _IOW(IOC_TYPE_SYS, IOC_NR_SYS_UNBIND, SYS_BIND_ARGS_S)
#define SYS_GETBINDBYDEST      _IOWR(IOC_TYPE_SYS, IOC_NR_SYS_GETBINDBYDEST, SYS_BIND_ARGS_S)

#define SYS_MEM_SET_CTRL       _IOW(IOC_TYPE_SYS, IOC_NR_SYS_MEM_SET, SYS_MEM_ARGS_S)
#define SYS_MEM_GET_CTRL       _IOWR(IOC_TYPE_SYS, IOC_NR_SYS_MEM_GET, SYS_MEM_ARGS_S)

#define SYS_GET_CUST_CODE      _IOR(IOC_TYPE_SYS, IOC_NR_SYS_GET_CUST_CODE, HI_U32)

#define SYS_SET_WIZARD         _IOW(IOC_TYPE_SYS, IOC_NR_SYS_SET_WIZARD, HI_U32)

#define SYS_GET_KERNELCONFIG   _IOR(IOC_TYPE_SYS, IOC_NR_SYS_GET_KERNELCONFIG, KERNEL_CONFIG_S)

#define SYS_GET_CHIPID         _IOR(IOC_TYPE_SYS, IOC_NR_SYS_GET_CHIPID, HI_U32)

#define SYS_GET_VIVPSS_MODE    _IOR(IOC_TYPE_SYS, IOC_NR_SYS_GET_VIVPSS_MODE, HI_U32)

#define SYS_SET_PROFILE        _IOW(IOC_TYPE_SYS, IOC_NR_SYS_SET_PROFILE, PROFILE_TYPE_E)
#define SYS_GET_PROFILE        _IOR(IOC_TYPE_SYS, IOC_NR_SYS_GET_PROFILE, HI_U32)

#define SYS_SET_RESULT         _IOW(IOC_TYPE_SYS, IOC_NR_SYS_SET_RESULT, HI_S32)
#define SYS_GET_RESULT         _IOWR(IOC_TYPE_SYS, IOC_NR_SYS_GET_RESULT, HI_U32)

#define SYS_MFLUSH_CACHE       _IOW(IOC_TYPE_SYS, IOC_NR_SYS_MFLUSH_CACHE, SYS_MEM_CACHE_INFO_S)

#define SYS_SET_SCALE_COEFF    _IOW(IOC_TYPE_SYS, IOC_NR_SYS_SET_SCALE_COEFF, SCALE_COEFF_INFO_S)
#define SYS_GET_SCALE_COEFF    _IOWR(IOC_TYPE_SYS, IOC_NR_SYS_GET_SCALE_COEFF, SCALE_COEFF_INFO_S)


/*cur sender:VIU,VOU,VDEC,VPSS,AI
 *cur receive:VOU,VPSS,GRP,AO
 */
#define BIND_ADJUST_SRC_DEVID(enModId ,s32DevId)\
do{\
    if ((HI_ID_VIU == enModId) || \
        (HI_ID_VDEC== enModId) )\
    {\
        s32DevId = 0;\
    }\
}while(0)


#define BIND_ADJUST_SRC_CHNID(enModId ,s32ChnId)\
do{\
    if ((HI_ID_VOU == enModId))\
    {\
        s32ChnId = 0;\
    }\
}while(0)

#define BIND_ADJUST_DEST_DEVID(enModId ,s32DevId)\
do{\
}while(0)


#define BIND_ADJUST_DEST_CHNID(enModId ,s32ChnId)\
do{\
    if ((HI_ID_VPSS == enModId) ||\
        (HI_ID_GROUP == enModId))\
    {\
        s32ChnId = 0;\
    }\
}while(0)


HI_S32 SYS_Bind(MPP_CHN_S *pstBindSrc, MPP_CHN_S *pstBindDest);
HI_S32 SYS_UnBind(MPP_CHN_S *pstBindSrc, MPP_CHN_S *pstBindDest);
HI_S32 SYS_GetBindbyDest(MPP_CHN_S *pstBindDest, MPP_CHN_S *pstBindSrc);
HI_S32 SYS_GetBindbySrc(MPP_CHN_S *pstSrcChn,MPP_BIND_SRC_S *pstBindSrc);

HI_S32 SYS_BIND_RegisterSender(BIND_SENDER_INFO_S *pstInfo);
HI_S32 SYS_BIND_UnRegisterSender(MOD_ID_E enModId);
HI_S32 SYS_BIND_SendData(MOD_ID_E enModId, HI_S32 s32DevId, HI_S32 s32ChnId, HI_BOOL bBlock,
        MPP_DATA_TYPE_E enDataType, HI_VOID *pvData);

HI_S32 SYS_BIND_ResetData(MOD_ID_E enModId, HI_S32 s32DevId, 
                                            HI_S32 s32ChnId, HI_VOID *pvData);

HI_S32 SYS_BIND_RegisterReceiver(BIND_RECEIVER_INFO_S *pstInfo);
HI_S32 SYS_BIND_UnRegisterReceiver(MOD_ID_E enModId);

HI_S32  SYS_BIND_Init(HI_VOID);
HI_VOID SYS_BIND_Exit(HI_VOID);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __MKP_SYS_H__ */

