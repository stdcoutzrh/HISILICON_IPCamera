#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hi_common.h"
#include "hi_comm_video.h"
#include "hi_comm_sys.h"
#include "hi_comm_vo.h"
#include "hi_comm_rc.h"
#include "hi_comm_venc.h"

#include "mpi_vb.h"
#include "mpi_sys.h"
#include "mpi_vi.h"
#include "mpi_vo.h"
#include "mpi_venc.h"


#define USAGE_HELP(void)\
    {\
        printf("\n\tusage : %s chn para value，如./rc_attr 1 stattime 2\n", argv[0]);    \
        printf("\n\t para: \n");    \
        printf("\t?   查询当前rc状态，如:./rc_attr 1 ? \n");   \
        printf("\tstattime   统计时间,单位:秒 \n");   \
        printf("\tbitrate    码率，单位:kbps\n");    \
        printf("\tgop        两个I帧的间隔，单位:帧\n");   \
        printf("\tqpdelta    I帧与P帧间的QP差异，差异越大整体效果好但呼吸效应严重\n");    \
        printf("\tmqpdelta	 每一行宏块的起始Qp 相对于帧起始Qp 的波动幅度值，高码率推荐值:0,中码率:0或1，低码率:2~5\n");   \
        printf("\tmaxqp      最大QP约束\n");   \
        printf("\tthresh     I / P 帧的宏块复杂度的一组阈值,范围：[0, 255]，注意此时输入参数为15个\n");   \
        printf("\tpthresh    P 帧的宏块复杂度的一组阈值,范围：[0, 255]，注意此时输入参数为15个\n");   \
        printf("\tframerate  目标帧率,取值范围：(0, u32SrcFrmRate]\n");   \
        printf("\tflut       最大码率相对平均码率的波动等级,范围：[0, 5]\n");   \
        printf("\tlevel      质量等级,范围：[1, 5]\n");   \
        printf("\tdeblock    块滤波功能,三个语法元素分别是:disable_deblocking_filter_idc,lice_alpha_c0_offset_div2,slice_beta_offset_div2的值\n");   \
    }

#define CHECK_RET(express,name)\
    do{\
        if (HI_SUCCESS != express)\
        {\
            printf("%s failed at %s: LINE: %d ! errno:%d \n",\
                   name, __FUNCTION__, __LINE__, express);\
            return HI_FAILURE;\
        }\
    }while(0)


HI_S32 main(int argc, char* argv[])
{
    HI_S32 s32Ret;
    HI_S32 s32EncChn;
    HI_CHAR para[16];
    HI_U32 value = 0;
    HI_U32 i;
    VENC_RC_MODE_E eMode;

    VENC_RC_PARAM_S stVencRcParam;
    VENC_CHN_ATTR_S stVencChnAttr;
	VENC_PARAM_H264_DBLK_S stH264Dblk;

	if(argc != 4 && argc != 15 && argc != 3 && argc != 6)
	{
		
		printf("\n\t(1)the parameter count is not right,please check\n");
		USAGE_HELP();
	 	return 0;
	}
	
    s32EncChn = 0;
    s32EncChn = atoi(argv[1]);
    strcpy(para, argv[2]);

	if(0 ==  strcmp(para, "?")  && argc != 3)
	{
		printf("\n\tyou are query the current state,its count must be 3,but %d, please check\n",argc);
		USAGE_HELP();	
	 	return -1;
	}

	if((0 ==  strcmp(para, "thresh") || 0 ==  strcmp(para, "pthresh")) && argc != 15)
	{
		printf("\n\tthe parameter is %s,its count must be 15,but %d, please check\n",para,argc);
		USAGE_HELP();	
	 	return -1;
	}

	if(0 ==  strcmp(para, "deblock") && argc != 6)
	{
		printf("\n\tthe parameter is %s,its count must be 6,but %d, please check\n",para,argc);
		USAGE_HELP();	
	 	return -1;
	}
	
	if (0 != strcmp(para, "mqpdelta")&&0 != strcmp(para, "level")&&0 != strcmp(para, "maxqp")\
		&&0 != strcmp(para, "qpdelta")&&0 != strcmp(para, "flut")&&0 != strcmp(para, "gop")&&0 != strcmp(para, "framerate")\
		&&0 != strcmp(para, "bitrate")&&0 != strcmp(para, "stattime") && 0 != strcmp(para, "?")&& 0 != strcmp(para, "thresh")\
		&& 0 != strcmp(para, "pthresh")&& 0 != strcmp(para, "deblock"))
	{
		printf("\n\t(2)the parameter is not right,please check\n");
		USAGE_HELP();	
		return -1;
	}

	if ((0 == strcmp(para, "mqpdelta")||0 == strcmp(para, "level")||0 == strcmp(para, "maxqp")\
		||0 == strcmp(para, "qpdelta")||0 == strcmp(para, "flut")||0 == strcmp(para, "gop")||0 == strcmp(para, "framerate")\
		||0 == strcmp(para, "bitrate")||0 == strcmp(para, "stattime"))&& argc != 4)
	{
		printf("\n\t(3)the parameter cnt is not right,please check\n");
		USAGE_HELP();	
		return -1;
	}
	

    s32Ret = HI_MPI_VENC_GetRcParam(s32EncChn, &stVencRcParam);
    CHECK_RET(s32Ret, "get Rc param");
    s32Ret = HI_MPI_VENC_GetChnAttr(s32EncChn, &stVencChnAttr);
    CHECK_RET(s32Ret, "get Rc Attr");

    eMode = stVencChnAttr.stRcAttr.enRcMode;
    if (0 == strcmp(para, "?"))
    {	
        if (eMode == VENC_RC_MODE_H264CBR)
        {
        	printf("\tenRcMode is VENC_RC_MODE_H264CBR\n");
            printf("\tstattime %d\n", stVencChnAttr.stRcAttr.stAttrH264Cbr.u32StatTime);
            printf("\tbitrate %d\n", stVencChnAttr.stRcAttr.stAttrH264Cbr.u32BitRate);
            printf("\tgop %d\n", stVencChnAttr.stRcAttr.stAttrH264Cbr.u32Gop);
            printf("\tqpdelta %d\n", stVencRcParam.stParamH264Cbr.s32IPQPDelta);
            printf("\tmaxqp %d\n", stVencRcParam.stParamH264Cbr.u32MaxQp);			
			printf("\tframerate %d\n", stVencChnAttr.stRcAttr.stAttrH264Cbr.fr32DstFrmRate);
			printf("\tflut %d\n", stVencChnAttr.stRcAttr.stAttrH264Cbr.u32FluctuateLevel);
			printf("\tlevel %d\n", stVencRcParam.stParamH264Cbr.s32QualityLevel);
        }			
        else if(eMode == VENC_RC_MODE_H265CBR)
        {
        	printf("\tenRcMode is VENC_RC_MODE_H265CBR\n");
            printf("\tstattime %d\n", stVencChnAttr.stRcAttr.stAttrH265Cbr.u32StatTime);
            printf("\tbitrate %d\n", stVencChnAttr.stRcAttr.stAttrH265Cbr.u32BitRate);
            printf("\tgop %d\n", stVencChnAttr.stRcAttr.stAttrH265Cbr.u32Gop);
            printf("\tqpdelta %d\n", stVencRcParam.stParamH265Cbr.s32IPQPDelta);
            printf("\tmaxqp %d\n", stVencRcParam.stParamH265Cbr.u32MaxQp);
			printf("\tframerate %d\n", stVencChnAttr.stRcAttr.stAttrH265Cbr.fr32DstFrmRate);
			printf("\tflut %d\n", stVencChnAttr.stRcAttr.stAttrH265Cbr.u32FluctuateLevel);
			printf("\tlevel %d\n", stVencRcParam.stParamH265Cbr.s32QualityLevel);
			
        }

		printf("\tthresh");
		for(i=0;i<12;i++)
		{
			printf(" %d",stVencRcParam.u32ThrdI[i]);
		}
		printf("\n");

		printf("\tpthresh");
		for(i=0;i<12;i++)
		{
			printf(" %d",stVencRcParam.u32ThrdP[i]);
		}
		printf("\n");
		
        printf("\tmqpdelta %d\n", stVencRcParam.u32RowQpDelta);

		s32Ret = HI_MPI_VENC_GetH264Dblk(s32EncChn, &stH264Dblk);
        CHECK_RET(s32Ret, "get deblock");
        printf("\tdeblock %d %d %d\n",stH264Dblk.disable_deblocking_filter_idc,stH264Dblk.slice_alpha_c0_offset_div2,stH264Dblk.slice_beta_offset_div2);

        return 0;
    }
    
	if(argc == 15)
	{
		printf("chn %d, para %s value",s32EncChn, para);
		for(i=0;i<12;i++)
		{
			printf(" %d",atoi(argv[3+i]));
		}
		printf("\n");
	}
	else if(argc == 6)
	{
		printf("chn %d, para %s value",s32EncChn, para);
		for(i=0;i<3;i++)
		{
			printf(" %d",atoi(argv[3+i]));
		}
		printf("\n");
	}
    else
	{
		value = atoi(argv[3]);
		printf("chn %d, para %s, value %d\n", s32EncChn, para, value);
    }

    if (0 == strcmp(para, "stattime")&& argc == 4)
    {
        if (eMode == VENC_RC_MODE_H264CBR)
        {
            stVencChnAttr.stRcAttr.stAttrH264Cbr.u32StatTime = value;
        }
        else if(eMode == VENC_RC_MODE_H265CBR)
        {
            stVencChnAttr.stRcAttr.stAttrH265Cbr.u32StatTime = value;
        }
    }
    else if (0 ==  strcmp(para, "thresh") && argc == 15)
    {
        for (i = 0; i < 12; i++)
        {
            stVencRcParam.u32ThrdI[i] = atoi(argv[i + 3]);
            stVencRcParam.u32ThrdP[i] = atoi(argv[i + 3]);
        }
    }  
    else if (0 ==  strcmp(para, "pthresh") && argc == 15)
    {
        for (i = 0; i < 12; i++)
        {
            stVencRcParam.u32ThrdP[i] = atoi(argv[i + 3]);
        }
    }
    else if (0 == strcmp(para, "bitrate")&& argc == 4)
    {
        if (eMode == VENC_RC_MODE_H264CBR)
        {
            stVencChnAttr.stRcAttr.stAttrH264Cbr.u32BitRate = value;
        }
        else if(eMode == VENC_RC_MODE_H265CBR)
        {
            stVencChnAttr.stRcAttr.stAttrH265Cbr.u32BitRate = value;
        }
    }
    else if ( 0 == strcmp(para, "framerate")&& argc == 4)
    {
        if (eMode == VENC_RC_MODE_H264CBR)
        {
            stVencChnAttr.stRcAttr.stAttrH264Cbr.fr32DstFrmRate = value;
        }
        else if(eMode == VENC_RC_MODE_H265CBR)
        {
            stVencChnAttr.stRcAttr.stAttrH265Cbr.fr32DstFrmRate = value;
        }
    }
    else if (0 == strcmp(para, "gop") && argc == 4)
    {
        if (eMode == VENC_RC_MODE_H264CBR)
        {
            stVencChnAttr.stRcAttr.stAttrH264Cbr.u32Gop = value;
        }
        else if(eMode == VENC_RC_MODE_H265CBR)
        {

            stVencChnAttr.stRcAttr.stAttrH265Cbr.u32Gop = value;
        }
    }
    else if (0 == strcmp(para, "flut")&& argc == 4)
    {
        if (eMode == VENC_RC_MODE_H264CBR)
        {
            stVencChnAttr.stRcAttr.stAttrH264Cbr.u32FluctuateLevel = value;
        }
        else if(eMode == VENC_RC_MODE_H265CBR)
        {
            stVencChnAttr.stRcAttr.stAttrH265Cbr.u32FluctuateLevel = value;
        }
    }

    else if (0 == strcmp(para, "qpdelta")&& argc == 4)
    {
        if (eMode == VENC_RC_MODE_H264CBR)
        {
            stVencRcParam.stParamH264Cbr.s32IPQPDelta = value;
        }
        else if(eMode == VENC_RC_MODE_H265CBR)
        {
            stVencRcParam.stParamH265Cbr.s32IPQPDelta = value;
        }
    }
    else if (0 == strcmp(para, "maxqp")&& argc == 4)
    {
        if (eMode == VENC_RC_MODE_H264CBR)
        {
            stVencRcParam.stParamH264Cbr.u32MaxQp = value;

        }
        else if(eMode == VENC_RC_MODE_H265CBR)
        {
            stVencRcParam.stParamH265Cbr.u32MaxQp = value;
        }
    }
    else if (0 == strcmp(para, "level")&& argc == 4)
    {
        if (eMode == VENC_RC_MODE_H264CBR)
        {
            stVencRcParam.stParamH264Cbr.s32QualityLevel = value;
        }
        else if(eMode == VENC_RC_MODE_H265CBR)
        { 
            stVencRcParam.stParamH265Cbr.s32QualityLevel = value;
        }
    }
    else if (0 == strcmp(para, "mqpdelta")&& argc == 4)
    {
        stVencRcParam.u32RowQpDelta = value;
    }
    else if ( 0 == strcmp(para, "deblock")&& argc == 6)
    {
        s32Ret = HI_MPI_VENC_GetH264Dblk(s32EncChn, &stH264Dblk);
        CHECK_RET(s32Ret, "get deblock");
        stH264Dblk.disable_deblocking_filter_idc = atoi(argv[3]);
        stH264Dblk.slice_alpha_c0_offset_div2 = atoi(argv[4]);
        stH264Dblk.slice_beta_offset_div2 = atoi(argv[5]);
        s32Ret = HI_MPI_VENC_SetH264Dblk(s32EncChn, &stH264Dblk);
        CHECK_RET(s32Ret, "get deblock");
    }
	else 
	{
		printf("\n\tthe parameter is unexist,please check\n");
		USAGE_HELP();	
	 	return -1;
	}
		

    s32Ret = HI_MPI_VENC_SetChnAttr(s32EncChn, &stVencChnAttr);
    CHECK_RET(s32Ret, "set Chn Attr");

    s32Ret = HI_MPI_VENC_SetRcParam(s32EncChn, &stVencRcParam);
    CHECK_RET(s32Ret, "set Rc param");

    return 0;
}

