#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hi_common.h"
#include "hi_comm_video.h"
#include "hi_comm_sys.h"
#include "hi_comm_vo.h"
#include "hi_comm_vi.h"
#include "hi_comm_vpss.h"
#include "hi_type.h"

#include "mpi_vb.h"
#include "mpi_sys.h"
#include "mpi_vi.h"
#include "mpi_vo.h"
#include "mpi_vpss.h"

#define USAGE_HELP(void)\
    {\
        printf("\n\tusage : %s  group para value \n", argv[0]);    \
        printf("\n\t para: \n");    \
        printf("\t\tenNR    [0, disable; 1,enable]\n");    \
        printf("\t\typk     [the strength of the texture to improve, value:0~63,   default:0]\n");   \
        printf("\t\tysf     [the strength of the main spatial filtering, value:0~200, default:100]\n");   \
        printf("\t\tytf     [the strength of the main temporal filtering, value:0~128,  default:64]\n");   \
        printf("\t\tytfmax  [the strength limit of the main temporal filtering, value:0~15, default:12]\n");   \
        printf("\t\tyss     [the absolute strength of the smooth filtering, value:0~200, default:0]\n");   \
        printf("\t\tysr     [the relative strength of the smooth filtering, value:0~32, default:16]\n");   \
        printf("\t\tysfdlt  [the strength of the auxiliary spatial filtering, value:-128~127, default:0]\n");   \
        printf("\t\tytfdlt  [the strength of the auxiliary temporal filtering(1), value:-64~63, default:0]\n");   \
        printf("\t\tytfdl   [the strength of the auxiliary temporal filtering(2), value:0~31, default:0]\n");   \
        printf("\t\tysfbr   [the relative strength of the spatial filtering in the light area, value:0~64, default:24]\n");   \
        printf("\t\tcsf     [the strength of spatial filtering for the color difference, value:0~80, default:32]\n");   \
        printf("\t\tctf     [the strength of temporal filtering for the color difference, value:0~32, default:0]\n");   \
    }

#define CHECK_RET(express,name)\
    do{\
        if (HI_SUCCESS != express)\
        {\
            printf("%s failed at %s: LINE: %d ! errno:%#x \n", \
                   name, __FUNCTION__, __LINE__, express);\
            return HI_FAILURE;\
        }\
    }while(0)


HI_S32 main(int argc, char* argv[])
{
    HI_S32 s32Ret;
    VPSS_GRP_ATTR_S stVpssGrpAttr = {0};
    VPSS_NR_PARAM_U unNrParam = {{0}};

    char paraTemp[16];
    HI_U32 value = 0;
    VPSS_GRP VpssGrp = 0;
    const char* para = paraTemp;

    if (argc < 4)
    {
        USAGE_HELP();
        return -1;
    }

    strcpy(paraTemp, argv[2]);
    value = atoi(argv[3]);
    VpssGrp = atoi(argv[1]);

    s32Ret = HI_MPI_VPSS_GetGrpAttr(VpssGrp, &stVpssGrpAttr);
    CHECK_RET(s32Ret, "HI_MPI_VPSS_GetGrpAttr");


    s32Ret = HI_MPI_VPSS_GetNRParam(VpssGrp, &unNrParam);
    CHECK_RET(s32Ret, "HI_MPI_VPSS_GetGrpParam");

    if (0 == strcmp(para, "enNR"))
    {
        stVpssGrpAttr.bNrEn = value;
    }
    else if (0 == strcmp(para, "ypk"))
    {
        unNrParam.stNRParam_V1.s32YPKStr = value;
    }
    else if (0 == strcmp(para, "ysf"))
    {
        unNrParam.stNRParam_V1.s32YSFStr = value;
    }
    else if (0 == strcmp(para, "ytf"))
    {
        unNrParam.stNRParam_V1.s32YTFStr = value;
    }
    else if (0 == strcmp(para, "ytfmax"))
    {
        unNrParam.stNRParam_V1.s32TFStrMax = value;
    }
    else if (0 == strcmp(para, "yss"))
    {
        unNrParam.stNRParam_V1.s32YSmthStr = value;
    }
    else if (0 == strcmp(para, "ysr"))
    {
        unNrParam.stNRParam_V1.s32YSmthRat = value;
    }
    else if (0 == strcmp(para, "ysfdlt"))
    {
        unNrParam.stNRParam_V1.s32YSFStrDlt = value;
    }
    else if (0 == strcmp(para, "ytfdlt"))
    {
        unNrParam.stNRParam_V1.s32YTFStrDlt = value;
    }
    else if (0 == strcmp(para, "ytfdl"))
    {
        unNrParam.stNRParam_V1.s32YTFStrDl = value;
    }
    else if (0 == strcmp(para, "ysfbr"))
    {
        unNrParam.stNRParam_V1.s32YSFBriRat = value;
    }
    else if (0 == strcmp(para, "csf"))
    {
        unNrParam.stNRParam_V1.s32CSFStr = value;
    }
    else if (0 == strcmp(para, "ctf"))
    {
        unNrParam.stNRParam_V1.s32CTFstr = value;
    }
    else
    {
        printf("err para\n");
        USAGE_HELP();
    }

    s32Ret = HI_MPI_VPSS_SetGrpAttr(VpssGrp, &stVpssGrpAttr);
    CHECK_RET(s32Ret, "HI_MPI_VPSS_SetGrpAttr");


    s32Ret = HI_MPI_VPSS_SetNRParam(VpssGrp, &unNrParam);
    CHECK_RET(s32Ret, "HI_MPI_VPSS_SetGrpParam");


    printf("\t\tenNR     %d\n",  stVpssGrpAttr.bNrEn);
    printf("\t\typk      %d\n",  unNrParam.stNRParam_V1.s32YPKStr);
    printf("\t\tysf       %d\n", unNrParam.stNRParam_V1.s32YSFStr);
    printf("\t\tytf       %d\n", unNrParam.stNRParam_V1.s32YTFStr);
    printf("\t\tytfmax    %d\n", unNrParam.stNRParam_V1.s32TFStrMax);
    printf("\t\tyss       %d\n", unNrParam.stNRParam_V1.s32YSmthStr);
    printf("\t\tysr       %d\n", unNrParam.stNRParam_V1.s32YSmthRat);
    printf("\t\tysfdlt    %d\n", unNrParam.stNRParam_V1.s32YSFStrDlt);
    printf("\t\tytfdlt    %d\n", unNrParam.stNRParam_V1.s32YTFStrDlt);
    printf("\t\tytfdl     %d\n", unNrParam.stNRParam_V1.s32YTFStrDl);
    printf("\t\tysfbr     %d\n", unNrParam.stNRParam_V1.s32YSFBriRat);
    printf("\t\tcsf       %d\n", unNrParam.stNRParam_V1.s32CSFStr);
    printf("\t\tctf       %d\n", unNrParam.stNRParam_V1.s32CTFstr);

    return 0;
}

