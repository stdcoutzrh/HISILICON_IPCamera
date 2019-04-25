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

#ifndef __ISP_ACM_CONFIG_H__
#define __ISP_ACM_CONFIG_H__

#include "hi_vreg.h"


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

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

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* __ISP_ACM_CONFIG_H__ */

