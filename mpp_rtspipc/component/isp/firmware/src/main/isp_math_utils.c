/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isp_math_utils.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/01/16
  Description   : 
  History       :
  1.Date        : 2013/01/16
    Author      : n00168968
    Modification: Created file

******************************************************************************/

#include "isp_math_utils.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

HI_U8 sqrt16(HI_U32 arg)
{
    HI_U8 mask = 128;
    HI_U8 res  = 0;
    HI_U8 i = 0;

    for(i=0; i<8; i++)
    {
        if((res + (mask>>i))*(res + (mask>>i)) <= arg) res = res + (mask>>i);
    }

    return res;
}

HI_U8 log16(HI_U32 arg)
{
    HI_U8  k = 0;
    HI_U8  res = 0;

    for(k=0; k<16; k++)
    {
        if(arg > (1<<k))
            {
            res = (k<<4) + ((arg<<4)/(1<<(k)));
            }
    }
    return res;
}

HI_U16 Sqrt32(HI_U32 u32Arg)
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

static HI_U8 leading_one_position(const HI_U32 in)
{
    HI_U8 pos = 0;
    HI_U32 val = in;
    if (val >= 1<<16) { val >>= 16; pos += 16; }
    if (val >= 1<< 8) { val >>=  8; pos +=  8; }
    if (val >= 1<< 4) { val >>=  4; pos +=  4; }
    if (val >= 1<< 2) { val >>=  2; pos +=  2; }
    if (val >= 1<< 1) {           pos +=  1; }
    return pos;
}

//  y = log2(x)
//  input:  Integer: val
//  output: Fixed point x.y
//  y: out_precision

HI_U32 log2_int_to_fixed(const HI_U32 val, const HI_U8 out_precision, const HI_U8 shift_out)
{
	int i;
	int pos = 0;
	HI_U32 a = 0;
	HI_U32 b = 0;
	HI_U32 in = val;
	HI_U32 result = 0;
	const unsigned char precision = out_precision;

	if(0 == val)
	{
		return 0;
	}
	// integral part
	pos = leading_one_position(val);
	// fractional part
	a = (pos <= 15) ? (in << (15 - pos)) : (in >> (pos - 15));
	for(i = 0; i < precision; ++i)
	{
		b = a * a;
		if(b & (1<<31))
		{
			result = (result << 1) + 1;
			a = b >> 16;
		}
		else
		{
			result = (result << 1);
			a = b >> 15;
		}
	}
	return (((pos << precision) + result) << shift_out) | ((a & 0x7fff)>> (15-shift_out));
}


HI_U32 math_log2(
    const HI_U32 val, 
    const HI_U8 out_precision,
    const HI_U8 shift_out)
{
    int i;
    int pos = 0;
    HI_U32 a = 0;
    HI_U32 b = 0;
    HI_U32 in = val;
    HI_U32 result = 0;
    const unsigned char precision = out_precision;

    if(0 == val)
    {
        return 0;
    }

    /* integral part */
    pos = leading_one_position(val);

    /* fractional part */
    a = (pos <= 15) ? (in << (15 - pos)) : (in>>(pos - 15));
    for(i = 0; i < precision; ++i)
    {
        b = a * a;
        if(b & (1<<31))
        {
            result = (result << 1) + 1;
            a = b >> 16;
        }
        else
        {
            result = (result << 1);
            a = b >> 15;
        }
    }

    return (((pos << precision) + result) << shift_out);
}

#if 0
static int _pow2_lut[32]={
        1073741824,1097253708,1121280436,1145833280,1170923762,1196563654,1222764986,1249540052,
        1276901417,1304861917,1333434672,1362633090,1392470869,1422962010,1454120821,1485961921,
        1518500250,1551751076,1585730000,1620452965,1655936265,1692196547,1729250827,1767116489,
        1805811301,1845353420,1885761398,1927054196,1969251188,2012372174,2056437387,2101467502};

__inline HI_U32 math_exp2(
        HI_U32 val,
        const unsigned char shift_in,
        const unsigned char shift_out
        )
{
    int fract = (val & ((1<<shift_in)-1)) >> (shift_in - 5); // lut precision is 5 bits
    return _pow2_lut[fract]>>(30-shift_out-(val>>8));
}
#endif

static HI_U32 _pow2_lut[33]={
		1073741824,1097253708,1121280436,1145833280,1170923762,1196563654,1222764986,1249540052,
		1276901417,1304861917,1333434672,1362633090,1392470869,1422962010,1454120821,1485961921,
		1518500250,1551751076,1585730000,1620452965,1655936265,1692196547,1729250827,1767116489,
		1805811301,1845353420,1885761398,1927054196,1969251188,2012372174,2056437387,2101467502,
		2147483647};

HI_U32 math_exp2(HI_U32 val, const unsigned char shift_in, const unsigned char shift_out)
{
	unsigned int fract_part = (val & ((1<<shift_in)-1));
	unsigned int int_part = val >> shift_in;
	if (shift_in <= 5)
	{
		unsigned int lut_index = fract_part << (5-shift_in);
		return _pow2_lut[lut_index] >> (30 - shift_out - int_part);
	}
	else
	{
		unsigned int lut_index = fract_part >> (shift_in - 5);
		unsigned int lut_fract = fract_part & ((1<<(shift_in-5))-1);
		unsigned int a = _pow2_lut[lut_index];
		unsigned int b =  _pow2_lut[lut_index+1];
		unsigned int res = ((unsigned long long)(b - a)*lut_fract) >> (shift_in-5);
		res = (res + a) >> (30-shift_out-int_part);
		return res;
	}
}


/*  Linear equation solving
 *
 *     y1 = a * x1 + b
 *     y2 = a * x2 + b
 *     y1 - (a * x1) = y2 - (a * x2)
 *     y1 - y2 = (a * x1) - (a * x2)
 *
 *     a = (y1 - y2) / (x1 - x2)
 *     b = y1 - (a * x1)
 *
 *    Result is coefficient "a" in fixed format   x.a_fraction_size
 *    y1, y2, x1, x2 - real value format
 */
HI_S32 solving_lin_equation_a(HI_S32 y1, HI_S32 y2, HI_S32 x1, HI_S32 x2, HI_S16 a_fraction_size)
{
    return ((y1 - y2) * (1 << a_fraction_size)) / (x1 - x2);
}
/*    Result is coefficient "b" in fixed format   x.a_fraction_size
 *    y1, x1 - real value format
 *    "a" in fixed format   x.a_fraction_size
 */
HI_S32 solving_lin_equation_b(HI_S32 y1, HI_S32 a, HI_S32 x1, HI_S16 a_fraction_size)
{
    return (y1 * (1 << a_fraction_size)) - (a * x1);
}

/* 	y = a / b
 *	a: fixed xxx.fraction_size
 *	b: fixed xxx.fraction_size
 */
HI_U32 div_fixed(HI_U32 a, HI_U32 b, const HI_U16 fraction_size)
{
	return (a << fraction_size) / b;
}
/*	nth root finding y = x^0.45
 *  not a precise equation - for speed issue
 *	Result is coefficient "y" in fixed format   xxx.fraction_size
 */
HI_S32 solving_nth_root_045(HI_S32 x, const HI_U16 fraction_size)
{
	HI_S32 y = (1 <<fraction_size) + ((x <<fraction_size) / 4) - ((x <<fraction_size) / 8);
	return y;
}

//	Transition matching.
//
//	lut_int  - LUT for search,					values in real format
//	lut_out  - LUT for transition matching,		values in real format
//	lut_size - size of LUT,						value in real format
//	value    - Value for search,				value in fixed format size. xxx.value_fraction_size
//
//	ret      - output value,					value in real format
//
//
//	
//	a0             ax(value)          a1
//   |--------------|-----------------|    - lut_int
//
//  b0             bx(ret)            b1
//   |--------------|-----------------|    - lut_out
//
//
//	ax = (a1 - a0) * x + a0
//	bx = (b1 - b0) * x + b0
//
//	x = (ax - a0) / (a1 - a0)
//  bx = ((b1 - b0) * (ax - a0) / (a1 - a0)) + b0
//
HI_U32 transition(HI_U32 *lut_in, HI_U32 *lut_out, HI_U32 lut_size, HI_U32 value, HI_U32 value_fraction_size)
{
	HI_U32 ret = 0;
	HI_U32 fast_search = value >> value_fraction_size;

	if (fast_search < lut_in[0])	//	fast search for minimum.
		ret = lut_out[0];
	else		//	fast search for maximum.
		if (fast_search >= lut_in[lut_size - 1])
			ret = lut_out[lut_size - 1];
		else	{	//	transition matching.
			//	searching
			HI_S32 i;
			HI_S32 a0 = lut_in[0];
			HI_S32 a1 = lut_in[0];
			HI_S32 b0 = lut_out[0];
			HI_S32 b1 = lut_out[0];
			for(i = 1; i < lut_size; i ++)	{
				if (lut_in[i] > fast_search)	{
					a0 = lut_in[i - 1];
					a1 = lut_in[i];
					b0 = lut_out[i - 1];
					b1 = lut_out[i];
					break;
				}
			}
			//	interpolation
			HI_S32 b10 = b1 - b0;
			HI_S32 ax0 = value - (a0 << value_fraction_size);
			HI_S32 a10 = a1 - a0;
			HI_S32 result;
			if (a10 == 0)
				result = 0;
			else
				result = b10 * ax0 / a10;
			result = result + (b0 << value_fraction_size);
			ret = result >> value_fraction_size;
		}
	return ret;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

