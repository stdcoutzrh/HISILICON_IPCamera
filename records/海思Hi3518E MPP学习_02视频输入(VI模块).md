@[toc](海思Hi3518E MPP学习_02视频输入 )

### 1.视频输入介绍
视频输入(VI模块)可以通过**DC**(Digital Camera)接口、 MIPI Rx(含 MIPI 接口、 LVDS 接口和 HISPI 接口)等**接收视频数据**。当工作在离线模式时，视频输入将接收到的数据存入到指定的内存区域；当工作在在线模式时，视频输入会将数据直接送给 VPSS模块。在此过程中， VI 可以对接收到的原始视频图像数据进行裁剪等处理，并实现一路原始视频图像输入，输出一路视频图像功能。

### 2.重要概念
* **视频输入设备**
视频输入设备支持若干种时序输入，负责对时序进行解析。
* **在线模式与离线模式**
**离线模式**：是指 VI 写出数据到 DDR，然后与之绑定的模块从 DDR 读取数
据。
**在线模式**：在线是指 VI 与 VPSS 之间的在线数据流传输，在此模式下 VI 不会
写出到 DDR，而是直接把数据流送给 VPSS。
* **视频物理通道**
视频物理通道负责将输入设备解析后得到的视频数据输出到 DDR。在真正将数据
输出到 DDR 之前，它可以实现裁剪等功能，具体功能见各芯片的详细描述。
* **掩码**
掩码用于指示 VI 设备的视频数据来源。
* **镜头畸变校正**（LDC）
镜头畸变校正，一些低端镜头容易产生图像畸变，需要根据畸变程度对其图像进
行校正。
* **DCI**
Dynamic Contrast Improvement，动态对比度调节。对图像进行动态的对比度调
节，即在增强图像暗区亮度时而不使亮区过亮，或降低亮区亮度时而不使暗区过
暗
* **DIS**
DIS 模块通过比较当前图像与前两帧图像采用不同自由度的防抖算法计算出当前
图像在各个轴方向上的抖动偏移向量，然后根据抖动偏移向量对当前图像进行校
正，从而起到防抖的效果。

### 3.从官方venc Sample看VI模块
**step 3: start vi dev & chn to capture**的步骤：
 * step 1: mipi configure：配置mipi。
*  step 2: configure sensor and ISP ：配置ISP。
 * step 3: run isp thread:开启ISP线程 。
 * step 4 : config & start vicap dev：配置和开启设备。
 *  step 5: config & start vicap chn ：配置和开启视频通道 。
```c
SAMPLE_COMM_VI_StartVi（step3）
			IsSensorInput
			SAMPLE_COMM_VI_StartIspAndVi
				SAMPLE_COMM_VI_StartMIPI（1）
					SAMPLE_COMM_VI_SetMipiAttr
						fd = open("/dev/hi_mipi", O_RDWR);
						ioctl(fd, HI_MIPI_SET_DEV_ATTR, pstcomboDevAttr)
				SAMPLE_COMM_ISP_Init（2）
					sensor_register_callback
					HI_MPI_AE_Register
					HI_MPI_AWB_Register
					HI_MPI_AF_Register
					HI_MPI_ISP_MemInit
					HI_MPI_ISP_SetWDRMode
					HI_MPI_ISP_SetPubAttr
					HI_MPI_ISP_Init
				SAMPLE_COMM_ISP_Run（3）
					pthread_create(&gs_IspPid, &attr, (void* (*)(void*))Test_ISP_Run, NULL)
						Test_ISP_Run
							HI_MPI_ISP_Run
				SAMPLE_COMM_VI_StartDev（4）
					HI_MPI_VI_SetDevAttr
					HI_MPI_ISP_GetWDRMode
					HI_MPI_VI_SetWDRAttr
					HI_MPI_VI_EnableDev
				SAMPLE_COMM_VI_StartChn（5）
					HI_MPI_VI_SetChnAttr
					HI_MPI_VI_SetRotate
					HI_MPI_VI_EnableChn	
```		
HI3518E内部的ISP单元是隶属于VI模块的。
VI模块就包含3大部分：
* 第一部分：和Sensor对接的部分。
* 第二部分：ISP配置和启动。
* 第三部分：VI dev和channel配置和启动。

