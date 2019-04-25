/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_vreg.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/01/09
  Description   : 
  History       :
  1.Date        : 2013/01/09
    Author      : n00168968
    Modification: Created file

******************************************************************************/
#ifdef __KERNEL__
#else
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include "mpi_isp.h"
#include "mpi_sys.h"
#endif

#include "hi_vreg.h"
#include "hi_comm_isp.h"
#include "hi_drv_vreg.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

#ifdef __KERNEL__
extern void __iomem *reg_isp_base_va;
extern void __iomem *reg_vicap_base_va;
#else
typedef struct hiHI_VREG_ADDR_S
{
    HI_U32  u32PhyAddr;
    HI_U32  u32VirtAddr;
} HI_VREG_ADDR_S;

typedef struct hiHI_VREG_S
{
    HI_VREG_ADDR_S stIspRegAddr;
    HI_VREG_ADDR_S stIspVRegAddr;
    HI_VREG_ADDR_S astAeAddr[MAX_ALG_LIB_VREG_NUM];
    HI_VREG_ADDR_S astAwbAddr[MAX_ALG_LIB_VREG_NUM];
    HI_VREG_ADDR_S astAfAddr[MAX_ALG_LIB_VREG_NUM];    
    HI_VREG_ADDR_S stViRegAddr;
} HI_VREG_S;

static HI_VREG_S g_stHiVreg = {{0}};

HI_S32 g_s32VregFd = -1;
static inline HI_S32 VREG_CHECK_OPEN(HI_VOID)
{
    if (g_s32VregFd <= 0)
    {
        g_s32VregFd = open("/dev/isp_dev", O_RDONLY);
        if (g_s32VregFd <= 0)
        {
            perror("open isp device error!\n");
            return HI_FAILURE;
        }
    }
    return HI_SUCCESS;
}

#endif

#ifdef __KERNEL__
HI_U32 VReg_GetVirtAddr(HI_U32 u32BaseAddr)
{    
    if ((ISP_REG_BASE != (u32BaseAddr & 0xffff0000)) && ((ISP_REG_BASE+0x10000) != (u32BaseAddr & 0xffff0000)))
    {
        return 0;
    }
    
    return (HI_U32)reg_isp_base_va;
}
HI_U32 VReg_GetVirtAddr_Vi(HI_U32 u32BaseAddr)
{    
    if ((VI_REG_BASE != (u32BaseAddr & 0xffff0000)) && ((VI_REG_BASE+0x10000) != (u32BaseAddr & 0xffff0000)))
    {
        return 0;
    }
    
    return (HI_U32)reg_vicap_base_va;
}
#else
#define VREG_MUNMAP_VIRTADDR(u32VirtAddr, u32Size)\
do{\
    if (0 != (u32VirtAddr))\
    {\
        HI_MPI_SYS_Munmap((HI_VOID *)(u32VirtAddr), (u32Size));\
    }\
}while(0);

static inline HI_VREG_ADDR_S *VReg_Match(HI_U32 u32BaseAddr)
{
    switch (u32BaseAddr & 0xffff0000)
    {
        case ISP_REG_BASE :
		case ISP_REG_BASE + 0x10000:
            return &g_stHiVreg.stIspRegAddr;
        case ISP_VREG_BASE :
            return &g_stHiVreg.stIspVRegAddr;
        case 0x20000 :
            return &g_stHiVreg.astAeAddr[(u32BaseAddr >> 12) & 0xf];
        case 0x30000 :
            return &g_stHiVreg.astAwbAddr[(u32BaseAddr >> 12) & 0xf];
        case 0x40000 :
            return &g_stHiVreg.astAfAddr[(u32BaseAddr >> 12) & 0xf];
        case VI_REG_BASE :
        case VI_REG_BASE + 0x10000 :
            return &g_stHiVreg.stViRegAddr;
            
        default :
            return HI_NULL;
    }
    return HI_NULL;
}

static inline HI_U32 VReg_BaseAlign(HI_U32 u32BaseAddr)
{
    switch (u32BaseAddr & 0xffff0000)
    {
        case ISP_REG_BASE :
        case ISP_VREG_BASE :
            return (u32BaseAddr & 0xffff0000);
        case 0x20000 :
        case 0x30000 :
        case 0x40000 :
            return (u32BaseAddr & 0xfffff000);
        default :
            return (u32BaseAddr & 0xffff0000);
    }
    return (u32BaseAddr & 0xffff0000);
}

static inline HI_U32 VReg_SizeAlign(HI_U32 u32Size)
{
    return (ALG_LIB_VREG_SIZE * ((u32Size + ALG_LIB_VREG_SIZE - 1) / ALG_LIB_VREG_SIZE));
}

HI_S32 VReg_Init(HI_U32 u32BaseAddr, HI_U32 u32Size)
{
    VREG_ARGS_S stVreg;
    
    if (u32BaseAddr != VReg_BaseAlign(u32BaseAddr))
    {
        return HI_FAILURE;
    }

    if (VREG_CHECK_OPEN())
    {
        return HI_FAILURE;
    }

    /* malloc vreg's phyaddr in kernel */
    stVreg.u32BaseAddr = VReg_BaseAlign(u32BaseAddr);
    stVreg.u32Size = VReg_SizeAlign(u32Size);
    if (ioctl(g_s32VregFd, VREG_DRV_INIT, &stVreg))
    {
        return HI_FAILURE;
    }
    
    return HI_SUCCESS;
}

HI_S32 VReg_Exit(HI_U32 u32BaseAddr, HI_U32 u32Size)
{
    HI_VREG_ADDR_S *pstVReg = HI_NULL;
    VREG_ARGS_S stVreg;

    if (u32BaseAddr != VReg_BaseAlign(u32BaseAddr))
    {
        return HI_FAILURE;
    }
    
    /* check base */
    pstVReg = VReg_Match(VReg_BaseAlign(u32BaseAddr));
    if (HI_NULL == pstVReg)
    {
        return HI_FAILURE;
    }

    if (0 != pstVReg->u32VirtAddr)
    {
        /* munmap virtaddr */
        VREG_MUNMAP_VIRTADDR(pstVReg->u32VirtAddr, VReg_SizeAlign(u32Size));
        pstVReg->u32VirtAddr = 0;
        pstVReg->u32PhyAddr = 0;
    }

    if (VREG_CHECK_OPEN())
    {
        return HI_FAILURE;
    }
    /* release the buf in the kernel */
    stVreg.u32BaseAddr = VReg_BaseAlign(u32BaseAddr);
    if (ioctl(g_s32VregFd, VREG_DRV_EXIT, &stVreg))
    {
        return HI_FAILURE;
    }
    
    return HI_SUCCESS;
}

HI_U32 VReg_GetVirtAddr(HI_U32 u32BaseAddr)
{
    HI_U32 u32Base, u32Size;
    HI_VREG_ADDR_S *pstVReg = HI_NULL;
    VREG_ARGS_S stVreg;
    
    if (VREG_CHECK_OPEN())
    {
        return 0;
    }    

    /* check base */
    pstVReg = VReg_Match(u32BaseAddr);
    if (HI_NULL == pstVReg)
    {
        return 0;
    }

    if (0 != pstVReg->u32VirtAddr)
    {
        return pstVReg->u32VirtAddr;
    }

    /* get phyaddr first */
    if ((ISP_REG_BASE == (u32BaseAddr & 0xffff0000)) || ((ISP_REG_BASE+0x10000) == (u32BaseAddr & 0xffff0000)))
    {
        pstVReg->u32PhyAddr = ISP_REG_BASE;
    }
    else
    {
        u32Base = (ISP_VREG_BASE == (u32BaseAddr & 0xffff0000)) ? ISP_VREG_BASE :
                u32BaseAddr & 0xfffff000;
        stVreg.u32BaseAddr = u32Base;
        if (ioctl(g_s32VregFd, VREG_DRV_GETADDR, &stVreg))
        {
            return 0;
        }
        pstVReg->u32PhyAddr = stVreg.u32PhyAddr;
    }
    
    /* map virtaddr */
    u32Size = (((ISP_REG_BASE == (u32BaseAddr & 0xffff0000)) || ((ISP_REG_BASE+0x10000) == (u32BaseAddr & 0xffff0000))) ? ISP_REG_SIZE :
            ((ISP_VREG_BASE == (u32BaseAddr & 0xffff0000)) ? ISP_VREG_SIZE : ALG_LIB_VREG_SIZE));
    pstVReg->u32VirtAddr = (HI_U32)HI_MPI_SYS_Mmap(pstVReg->u32PhyAddr, u32Size);

    return pstVReg->u32VirtAddr;
}

HI_U32 VReg_GetVirtAddr_Vi(HI_U32 u32BaseAddr)
{
    HI_U32 u32Size;
    HI_VREG_ADDR_S *pstVReg = HI_NULL;

    /* check base */
    pstVReg = VReg_Match(u32BaseAddr);
    if (HI_NULL == pstVReg)
    {
        return 0;
    }

    if (0 != pstVReg->u32VirtAddr)
    {
        return pstVReg->u32VirtAddr;
    }

    pstVReg->u32PhyAddr = VI_REG_BASE;
    u32Size = VI_REG_SIZE;
    
    pstVReg->u32VirtAddr = (HI_U32)HI_MPI_SYS_Mmap(pstVReg->u32PhyAddr, u32Size);

    return pstVReg->u32VirtAddr;
}


HI_VOID VReg_Munmap(HI_VOID)
{
    HI_S32 i;
    
    VREG_MUNMAP_VIRTADDR(g_stHiVreg.stIspRegAddr.u32VirtAddr, ISP_REG_SIZE);
    VREG_MUNMAP_VIRTADDR(g_stHiVreg.stIspVRegAddr.u32VirtAddr, ISP_VREG_SIZE);
    VREG_MUNMAP_VIRTADDR(g_stHiVreg.stViRegAddr.u32VirtAddr, VI_REG_SIZE);

    for (i=0; i<MAX_ALG_LIB_VREG_NUM; i++)
    {
        VREG_MUNMAP_VIRTADDR(g_stHiVreg.astAeAddr[i].u32VirtAddr, ALG_LIB_VREG_SIZE);
        VREG_MUNMAP_VIRTADDR(g_stHiVreg.astAwbAddr[i].u32VirtAddr, ALG_LIB_VREG_SIZE);
        VREG_MUNMAP_VIRTADDR(g_stHiVreg.astAfAddr[i].u32VirtAddr, ALG_LIB_VREG_SIZE);
    }

    memset(&g_stHiVreg, 0, sizeof(HI_VREG_S));
    return;
}
#endif

/*--------------------------------------------------------------------------------------*/
/* write or read vi reg */
HI_U32 IO_READ32_VI(HI_U32 u32Addr)
{
    HI_U32 *pu32Addr, u32VirtAddrBase, u32Value;

    u32VirtAddrBase = VReg_GetVirtAddr_Vi(u32Addr);
    if (0 == u32VirtAddrBase)
    {
        return 0;
    }

    pu32Addr = (HI_U32 *)((HI_U32)u32VirtAddrBase + (u32Addr & 0x1ffff));
    u32Value = *pu32Addr;
    
    return u32Value;
}

HI_S32 IO_WRITE32_VI(HI_U32 u32Addr, HI_U32 u32Value)
{
    HI_U32 *pu32Addr, u32VirtAddrBase;

    u32VirtAddrBase = VReg_GetVirtAddr_Vi(u32Addr);
    if (0 == u32VirtAddrBase)
    {
        return 0;
    }
    pu32Addr = (HI_U32 *)((HI_U32)u32VirtAddrBase + (u32Addr & 0x1ffff));
    *pu32Addr = u32Value;

    return HI_SUCCESS;
}
/*--------------------------------------------------------------------------------------*/

HI_U32 IO_READ32(HI_U32 u32Addr)
{
    HI_U32 *pu32Addr, u32VirtAddrBase, u32Value;

    u32VirtAddrBase = VReg_GetVirtAddr(u32Addr);
    if (0 == u32VirtAddrBase)
    {
        return 0;
    }
    
    if ((ISP_REG_BASE == (u32Addr & 0xFFFF0000))
        || (ISP_VREG_BASE == (u32Addr & 0xFFFF0000)))
    {
        pu32Addr = (HI_U32 *)((HI_U32)u32VirtAddrBase + (u32Addr & 0xffff));
    }
	else if ((ISP_REG_BASE+0x10000) == (u32Addr & 0xFFFF0000))
	{
	    pu32Addr = (HI_U32 *)((HI_U32)u32VirtAddrBase + ((u32Addr - ISP_REG_BASE) & 0x1ffff));
	}
    else
    {
        pu32Addr = (HI_U32 *)((HI_U32)u32VirtAddrBase + (u32Addr & 0xfff));
    }
    u32Value = *pu32Addr;
    
    return u32Value;
}

HI_S32 IO_WRITE32(HI_U32 u32Addr, HI_U32 u32Value)
{
    HI_U32 *pu32Addr, u32VirtAddrBase;

    u32VirtAddrBase = VReg_GetVirtAddr(u32Addr);
    if (0 == u32VirtAddrBase)
    {
        return 0;
    }
    
    if ((ISP_REG_BASE == (u32Addr & 0xFFFF0000))
        || (ISP_VREG_BASE == (u32Addr & 0xFFFF0000)))
    {
        pu32Addr = (HI_U32 *)((HI_U32)u32VirtAddrBase + (u32Addr & 0xffff));
    }
	else if ((ISP_REG_BASE+0x10000) == (u32Addr & 0xFFFF0000))
	{
	    pu32Addr = (HI_U32 *)((HI_U32)u32VirtAddrBase + ((u32Addr - ISP_REG_BASE) & 0x1ffff));
	}
    else
    {
        pu32Addr = (HI_U32 *)((HI_U32)u32VirtAddrBase + (u32Addr & 0xfff));
    }
    *pu32Addr = u32Value;

    return HI_SUCCESS;
}


HI_U8 IO_READ8(HI_U32 u32Addr)
{
    HI_U32 u32Value;    

    if(((u32Addr>>REG_ACCESS_WIDTH_1) & 0x3) == 0)    
    {
        u32Value = IO_READ32(u32Addr);       
        u32Value = (u32Value & 0x000000FF);   
    }      
    else if(((u32Addr>>REG_ACCESS_WIDTH_1) & 0x3) == 1)
    {
        u32Value = IO_READ32(((u32Addr>>REG_ACCESS_WIDTH_1) - 1)<<REG_ACCESS_WIDTH_1);    
        u32Value = (u32Value & 0x0000FF00) >> 8; 
    }
    else if(((u32Addr>>REG_ACCESS_WIDTH_1) & 0x3) == 2)
    {
        u32Value = IO_READ32(((u32Addr>>REG_ACCESS_WIDTH_1) - 2)<<REG_ACCESS_WIDTH_1);    
        u32Value = (u32Value & 0x00FF0000) >> 16; 
    }
    else 
    {
        u32Value = IO_READ32(((u32Addr>>REG_ACCESS_WIDTH_1) - 3)<<REG_ACCESS_WIDTH_1);    
        u32Value = (u32Value & 0xFF000000) >> 24; 
    }

    return u32Value;
}

HI_S32 IO_WRITE8(HI_U32 u32Addr, HI_U32 u32Value)
{
    HI_U32  u32TmpAddr, u32Current;

    if(((u32Addr>>REG_ACCESS_WIDTH_1) & 0x3) == 0)    
    {
        u32Current = IO_READ32(u32Addr);      
        IO_WRITE32(u32Addr, ( u32Value & 0x000000FF )|(u32Current & 0xFFFFFF00 )); 
    }  
    else if(((u32Addr>>REG_ACCESS_WIDTH_1) & 0x3) == 1) 
    {
        u32TmpAddr = ((u32Addr >> REG_ACCESS_WIDTH_1)-1)<<REG_ACCESS_WIDTH_1;
        u32Current = IO_READ32(u32TmpAddr);    
        IO_WRITE32(u32TmpAddr, ((u32Value & 0x000000FF ) << 8)|(u32Current & 0xFFFF00FF ));
    }
    else if(((u32Addr>>REG_ACCESS_WIDTH_1) & 0x3) == 2)
    {
        u32TmpAddr = ((u32Addr >> REG_ACCESS_WIDTH_1)-2)<<REG_ACCESS_WIDTH_1;
        u32Current = IO_READ32(u32TmpAddr);    
        IO_WRITE32(u32TmpAddr, ((u32Value & 0x000000FF) << 16)|(u32Current & 0xFF00FFFF ));
    }
    else
    {
        u32TmpAddr = ((u32Addr >> REG_ACCESS_WIDTH_1)-3)<<REG_ACCESS_WIDTH_1;
        u32Current = IO_READ32(u32TmpAddr);    
        IO_WRITE32(u32TmpAddr, ((u32Value & 0x000000FF) << 24)|(u32Current & 0x00FFFFFF ));
    }

    return HI_SUCCESS;
}

HI_U16 IO_READ16(HI_U32 u32Addr)
{
    HI_U32  u32Value;

    if(((u32Addr>>REG_ACCESS_WIDTH_1) & 2) == 0)    
    {    
        u32Value = IO_READ32( u32Addr );       
        u32Value = (u32Value & 0xFFFF);    
    }      
    else   
    {
        u32Value = IO_READ32(((u32Addr>>REG_ACCESS_WIDTH_1) - 2)<<REG_ACCESS_WIDTH_1);    
        u32Value = ((u32Value & 0xFFFF0000) >> 16); 
    }
    
    return u32Value;
}

HI_S32 IO_WRITE16(HI_U32 u32Addr, HI_U32 u32Value)
{
    HI_U32  u32TmpAddr, u32Current;        
    if(((u32Addr>>REG_ACCESS_WIDTH_1) & 2 )== 0)    
    {
        u32Current = IO_READ32(u32Addr);      
        IO_WRITE32(u32Addr, (u32Value & 0x0000FFFF )|(u32Current & 0xFFFF0000 )); 
    }  
    else  
    {
        u32TmpAddr = ((u32Addr >>REG_ACCESS_WIDTH_1)-2)<<REG_ACCESS_WIDTH_1;
        u32Current = IO_READ32(u32TmpAddr);    
        IO_WRITE32(u32TmpAddr, ((u32Value & 0x0000FFFF ) << 16)|(u32Current & 0x0000FFFF ));
    }
    return HI_SUCCESS;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

