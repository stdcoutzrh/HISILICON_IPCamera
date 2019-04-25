/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_math_utils.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/01/16
  Description   : 
  History       :
  1.Date        : 2013/01/16
    Author      : n00168968
    Modification: Created file

******************************************************************************/

#ifndef __ISP_MATH_UTILS_H__
#define __ISP_MATH_UTILS_H__

#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

HI_U8 sqrt16(HI_U32 arg);
HI_U8 log16(HI_U32 arg);
HI_U16 Sqrt32(HI_U32 u32Arg);
HI_U32 math_log2(const HI_U32 val, const HI_U8 out_precision, const HI_U8 shift_out);
HI_U32 log2_int_to_fixed(const HI_U32 val, const HI_U8 out_precision, const HI_U8 shift_out);
HI_U32 math_exp2(HI_U32 val, const unsigned char shift_in, const unsigned char shift_out);
HI_S32 solving_lin_equation_a(HI_S32 y1, HI_S32 y2, HI_S32 x1, HI_S32 x2, HI_S16 a_fraction_size);
HI_S32 solving_lin_equation_b(HI_S32 y1, HI_S32 a, HI_S32 x1, HI_S16 a_fraction_size);
HI_U32 div_fixed(HI_U32 a, HI_U32 b, const HI_U16 fraction_size);
HI_S32 solving_nth_root_045(HI_S32 x, const HI_U16 fraction_size);
HI_U32 transition(HI_U32 *lut_in, HI_U32 *lut_out, HI_U32 lut_size, HI_U32 value, HI_U32 value_fraction_size);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif // __ISP_MATH_UTILS_H__

