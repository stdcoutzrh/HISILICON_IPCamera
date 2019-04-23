/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : strfunc.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2005/7/27
  Last Modified :
  Description   : String functions
  Function List :
  History       :
  1.Date        : 2005/7/27
    Author      : T41030
    Modification: Created file

******************************************************************************/

#include <stdio.h>
#include <ctype.h>
#include "strfunc.h"

static int atoul(char* str, unsigned int* pulValue);
static int atoulx(char* str, unsigned int* pulValue);


/*****************************************************************************
 Prototype    : StrToNumber
 Description  : 10/16 进制字符串转换为无符号数字。
 Input  args  : IN CHAR *str
                   10进制字符串, 不接受符号
                   16进制字符串, 不包括前缀0x. 如ABCDE

 Output args  : U32* pulValue, 转换后的数字
 Return value : HI_RET  HI_SUCCESS 转换成功
                        HI_FAILURE 转换失败
 Calls        : isdigit

 Called  By   :

 History        :
 1.Date         : 2005年7月10日
   Author       : t41030
   Modification : Created function
*****************************************************************************/

int StrToNumber(char* str , unsigned int* pulValue)
{
    /*判断是否16进制的字符串*/
    if ( *str == '0' && (*(str + 1) == 'x' || *(str + 1) == 'X') )
    {
        if (*(str + 2) == '\0')
        {
            return -1;
        }
        else
        {
            return atoulx(str + 2, pulValue);
        }
    }
    else
    {
        return atoul(str, pulValue);
    }
}

/*****************************************************************************
 Prototype    : atoul
 Description  : 10进制字符串转换为无符号数字。
 Input  args  : IN CHAR *str 10进制字符串
                不接受符号
 Output args  : U32* pulValue, 转换后的数字
 Return value : HI_RET  HI_SUCCESS 转换成功
                        HI_FAILURE 转换失败
 Calls        : isdigit

 Called  By   :

 History        :
 1.Date         : 2005年7月10日
   Author       : t41030
   Modification : Created function
*****************************************************************************/
static int atoul(char* str, unsigned int* pulValue)
{
    unsigned int ulResult = 0;

    while (*str)
    {
        if (isdigit((int)*str))
        {
            /*最大支持到0xFFFFFFFF(4294967295),
               X * 10 + (*str)-48 <= 4294967295
               所以， X = 429496729 */
            if ((ulResult < 429496729) || ((ulResult == 429496729) && (*str < '6')))
            {
                ulResult = ulResult * 10 + (*str) - 48;
            }
            else
            {
                *pulValue = ulResult;
                return -1;
            }
        }
        else
        {
            *pulValue = ulResult;
            return -1;
        }
        str++;
    }
    *pulValue = ulResult;
    return 0;
}



/*****************************************************************************
 Prototype    : atoulx
 Description  : 16进制字符串转换为无符号数字。输入的16进制字符串不包括前缀0x
 Input  args  : IN CHAR *str 16进制字符串, 不包括前缀0x. 如ABCDE
 Output args  : U32* pulValue, 转换后的数字
 Return value : HI_RET  HI_SUCCESS 转换成功
                        HI_FAILURE 转换失败
 Calls        : toupper
                isdigit

 Called  By   :

 History        :
 1.Date         : 2005年7月10日
   Author       : t41030
   Modification : Created function
*****************************************************************************/
#define ASC2NUM(ch) (ch - '0')
#define HEXASC2NUM(ch) (ch - 'A' + 10)

int  atoulx(char* str, unsigned int* pulValue)
{
    unsigned int   ulResult = 0;
    unsigned char ch;

    while (*str)
    {
        ch = toupper(*str);
        if (isdigit(ch) || ((ch >= 'A') && (ch <= 'F' )))
        {
            if (ulResult < 0x10000000)
            {
                ulResult = (ulResult << 4) + ((ch <= '9') ? (ASC2NUM(ch)) : (HEXASC2NUM(ch)));
            }
            else
            {
                *pulValue = ulResult;
                return -1;
            }
        }
        else
        {
            *pulValue = ulResult;
            return -1;
        }
        str++;
    }

    *pulValue = ulResult;
    return 0;
}



