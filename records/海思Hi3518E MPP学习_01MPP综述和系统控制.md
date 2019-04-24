@[toc](海思Hi3518E MPP学习_01MPP综述和系统控制)

## 1.海思Hi3518E MPP综述
### 1.1海思Hi3518E MPP介绍
**海思系列芯片网络摄像头开发主要就是使用海思提供的MPP**(媒体处理软件平台Media Process Platform,简称 MPP),说简单点MPP就是一套封装好的系统，这套系统包含了**已经编译好的硬件驱动及函数依赖库**和**封装屏蔽芯片底层寄存器操作而可以直接调用相应功能的函数接口MPI(MPP Programe Interface)操作芯片**，利用这套系统我们可以快速开发应用软件。

MPP目录结构：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190422122737479.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1BlY29IZQ==,size_16,color_FFFFFF,t_70)

### 1.2海思Hi3518E典型系统架构
Hi35xx 典型的系统层次图：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2019042212321611.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1BlY29IZQ==,size_16,color_FFFFFF,t_70)

### 1.3海思Hi3518E MPP处理流程
海思媒体处理平台的主要媒体业务分为**视频输入(VI)**、**视频处理(VPSS)**、**视频编码(VENC)**、**视频解码(VDEC)**、**视频输出(VO)**、**音频输入(AI)**、**音频输出(AO)**、**音频编码(AENC)**、**音频解码(ADEC)**、**区域管理(REGION)** 等模块。

典型的视频编码示意图：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190422124318670.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1BlY29IZQ==,size_16,color_FFFFFF,t_70)
其中：
* **VI模块**捕获视频图像,可对其做剪切、缩放等处理，并输出多路不同分辨率的图像数据。
* **VDEC模块**对编码后的视频码流进行解码,并将解析后的图像数据送VPSS进行图像处理或直接送 VO 显示。(可对 H264/MPEG4/MPEG2 格式的视频码流进行解码。)
* **VPSS模块**接收VI和VDEC发送过来的图像，可对图像进行去噪、图像增强、锐化等处理，并实现同源输出多路不同分辨率的图像数据用于编码、预览或抓拍。
* **VENC模块**接收 VI 捕获并经 VPSS 处理后输出的图像数据，可叠加用户通过 **Region模块**设置的 OSD 图像，然后按不同协议进行编码并输出相应码流。
* **VDA模块**接收 VI 的输出图像，并进行移动侦测和遮挡侦测，最后输出侦测分析结果。
* **VO模块**接收 VPSS 处理后的输出图像，可进行播放控制等处理，最后按用户配置的输出协议输出给外围视频设备。


## 2.海思Hi3518E系统控制
### 2.1海思Hi3518E系统控制概述
系统控制根据 Hi35xx 芯片特性，完成硬件各个部件的复位、基本初始化工作，同时负责完成 MPP系统各个业务模块的初始化、去初始化以及管理 MPP 系统各个业务模块的工作状态、提供当前 MPP系统的版本信息、提供大块物理内存管理等功能。
**应用程序启动 MPP 业务前，必须完成 MPP 系统初始化工作。应用程序退出MPP 业务后，也要完成 MPP 系统去初始化工作，释放资源**。

### 2.2海思Hi3518E典型的视频编解码处理流程
 * step  1: init sys variable：定义和赋值MPP中的相关变量。
 * step 2: mpp system init.:利用上面的值初始化设置MPP中的相关值。
  * step 3: start vi dev & chn to capture：启动VI设备采集图像。
  * step 4: start vpss and vi bind vpss：绑定VI和VPSS。
  * step 5: start stream venc：开始视频流编码。
  * step 6: stream venc process -- get stream, then save it to file:处理码流。
  * step 7: exit process：退出程序。


### 2.3视频缓存池的概念
视频缓存池向MPP提供**大块物理内存管理功能**，负责内存的分配和回收，由**一组大小相同、物理地址连续的缓存块组成**。
**所有的VI模块都可以从公共视频缓存池中获取视频缓存块用于保存采集的图像**。由于VI不提供创建和销毁公共视频缓存池功能，因此，在系统初始化之前，必须为视频输入通道配置公共视频缓存池。缓存池需要几个，每个中包含几个缓存块，每个缓存块多大，都是可以由用户程序设置好参数，然后调用MPP的相应API来向MPP申请分配的。

视频缓存池相关数据结构:
VB_CONF_S:
```c
typedef struct hiVB_CONF_S
{
    HI_U32 u32MaxPoolCnt;     
    struct hiVB_CPOOL_S	//单个缓存池描述结构体
    {
        HI_U32 u32BlkSize;	//block size
        HI_U32 u32BlkCnt;	//block number
        HI_CHAR acMmzName[MAX_MMZ_NAME_LEN];
    	}astCommPool[VB_MAX_COMM_POOLS];
} VB_CONF_S;
```
VB_CONF_S是整个系统中视频缓存池的属性描述结构体，其中u32MaxPoolCnt是整个系统中的最大缓存池数量，而hiVB_CPOOL_S是单个缓存池描述结构体，u32BlkSize是单个缓存池中缓存块大小，u32Blkcnt是单个缓存池中缓存块数量。VB_MAX_COMM_POOLS默认最大为16。

注：本文参考了《HiMPP IPC V2.0 媒体处理软件开发参考》。



