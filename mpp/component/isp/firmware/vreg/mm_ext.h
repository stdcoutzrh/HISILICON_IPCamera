/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : mm_ext.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2010/11/16
  Description   : 
  History       :
  1.Date        : 2010/11/16
    Author      : Z44949
    Modification: Created file

******************************************************************************/
#ifndef __MM_EXT_H__
#define __MM_EXT_H__

#include <linux/kernel.h>
#include <linux/hardirq.h>
#include <asm/io.h>
#include <linux/slab.h>
#include <linux/version.h>

#include "hi_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

HI_U32  CMPI_MmzMalloc(HI_CHAR *pMmzName, HI_CHAR *pBufName, HI_U32 u32Size);
HI_VOID CMPI_MmzFree(HI_U32 u32PhyAddr, HI_VOID* pVirtAddr);
HI_S32	CMPI_MmzMallocNocache(HI_CHAR* cpMmzName, HI_CHAR* pBufName,
                             HI_U32* pu32PhyAddr, HI_VOID** ppVitAddr, HI_S32 s32Len);
HI_S32  CMPI_MmzMallocCached(HI_CHAR* cpMmzName, HI_CHAR* pBufName,
                            HI_U32* pu32PhyAddr, HI_VOID** ppVitAddr, HI_S32 s32Len);


/******************************************************************************
** The following functions can be used for physical memory which is outside OS.
** CMPI_Remap_Cached 
** CMPI_Remap_Nocache
** CMPI_Unmap
******************************************************************************/


__inline static HI_VOID * CMPI_Remap_Cached(HI_U32 u32PhyAddr, HI_U32 u32Size)
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,18,10)
    return ioremap_cached(u32PhyAddr, HIALIGN(u32Size, 4));
#else
	return ioremap_cache(u32PhyAddr, HIALIGN(u32Size, 4));
#endif
}




__inline static HI_VOID * CMPI_Remap_Nocache(HI_U32 u32PhyAddr, HI_U32 u32Size)
{
    return ioremap_nocache(u32PhyAddr, HIALIGN(u32Size, 4));
}

__inline static HI_VOID   CMPI_Unmap(HI_VOID *pVirtAddr)
{
    iounmap(pVirtAddr);
}

/******************************************************************************
** The following functions only can be used for memory which is inside the OS.
** CMPI_Kmalloc 
** CMPI_Kfree
** CMPI_Virt2Phys
** CMPI_Phys2Virt
******************************************************************************/
__inline static HI_VOID * CMPI_Kmalloc(HI_U32 u32Size)
{
    HI_VOID *pMalloc = NULL;
    
    if(in_interrupt())
    {
        pMalloc = kmalloc(u32Size, GFP_ATOMIC);
    }
    else
    {
        pMalloc = kmalloc(u32Size, GFP_KERNEL);
    }

    return pMalloc;
}

__inline static HI_VOID CMPI_Kfree(HI_VOID *pVirAddr)
{
    kfree(pVirAddr);
}

/******************************************************************************
** Attention: Only the memory is malloced by CMPI_Kmalloc can use it!!
** CMPI_Virt2Phys
** CMPI_Phys2Virt
******************************************************************************/
__inline static HI_U32 CMPI_Virt2Phys(HI_VOID *pVirAddr)
{
    return virt_to_phys(pVirAddr);
}

__inline static HI_VOID * CMPI_Phys2Virt(HI_U32 u32PhyAddr)
{
    return phys_to_virt(u32PhyAddr);
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* End of #ifndef  __MM_EXT_H__ */
