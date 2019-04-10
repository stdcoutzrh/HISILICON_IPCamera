/******************************************************************************
  A simple program of Hisilicon mpp implementation.
  Copyright (C), 2012-2020, Hisilicon Tech. Co., Ltd.
 ******************************************************************************
    Modification:  2013-7 Created
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "sample_comm_ive.h"

#include "./sample/sample_ive_main.h"


/******************************************************************************
* function : to process abnormal case                                         
******************************************************************************/
HI_VOID SAMPLE_IVE_HandleSig(HI_S32 s32Signo)
{
    if (SIGINT == s32Signo || SIGTERM == s32Signo)
    {
        SAMPLE_COMM_SYS_Exit();
        printf("\033[0;31mprogram termination abnormally!\033[0;39m\n");
    }
    exit(-1);
}

/******************************************************************************
* function : show usage
******************************************************************************/
HI_VOID SAMPLE_IVE_Usage(HI_CHAR *pchPrgName)
{
	HI_S32 i = 0;
    printf("Usage : %s <index> [complete] [vi/file]\n", pchPrgName);
    printf("index:\n");
    printf("\t %d)Canny,<complete>:0,part canny;1,complete canny.(FILE->IVE->FILE).\n",i++);	
    printf("\t %d)MemoryTest.(FILE->IVE->FILE).\n",i++);
    printf("\t %d)Sobel.(FILE->IVE->FILE).\n",i++);   
#if (CHIP_ID != CHIP_HI3518E_V201)
    printf("\t %d)Occlusion detected.(VI_BT1120->IVE->VO_BT656).\n",i++);	
    printf("\t %d)Motion detected,<vi/file>:0,vi,(VI_BT1120->IVE->VGS->VO_BT656);1,file,(FILE->VPSS->IVE->VGS->VO_BT656).\n",i++);
#endif
	
}


/******************************************************************************
* function : ive sample
******************************************************************************/
int main(int argc, char *argv[])
{
    HI_S32 s32Ret = HI_SUCCESS;

    signal(SIGINT, SAMPLE_IVE_HandleSig);
    signal(SIGTERM, SAMPLE_IVE_HandleSig);

    if (argc < 2)
    {
        SAMPLE_IVE_Usage(argv[0]);
        return HI_FAILURE;
    }
        
    switch (*argv[1])
    {
    case '0':
    	{
			if ((argc < 3) || (('0' != *argv[2]) && ('1' != *argv[2])))
			{
				SAMPLE_IVE_Usage(argv[0]);
				return HI_FAILURE;
			}
        	SAMPLE_IVE_Canny(*argv[2]);        
    	}
		break;		
	case '1':
		{
			SAMPLE_IVE_TestMemory();	   
		}
		break;
	case '2':
		{
			SAMPLE_IVE_Sobel(); 	   
		}
		break;	  
#if (CHIP_ID != CHIP_HI3518E_V201)
    case '3':
    	{
        	SAMPLE_IVE_Od();       
    	}
	 	break;
	case '4':
		{
			if ((argc < 3) || (('0' != *argv[2]) && ('1' != *argv[2])))
			{
				SAMPLE_IVE_Usage(argv[0]);
				return HI_FAILURE;
			}
			SAMPLE_IVE_Md(*argv[2]);	   
		}
		break;		
#endif
    default :
    	{
        	SAMPLE_IVE_Usage(argv[0]);        
    	}
		break;
    }
        
       

    return s32Ret;
}



