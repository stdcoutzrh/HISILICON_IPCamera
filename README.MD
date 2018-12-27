# HISILICON_IPC
------
此仓库为个人用**海思HI3518EV200**开发**IP_CAMERA**的记录。

## 1.视频设备开发的技术流
### 视频从产生到被消费的整个流程
①基本认知：视频是由单帧图像以每秒x帧的速率连续组成的。             
②原始视频产生：镜头和sensor       
③图像处理：ISP（image signal processing）           
④视频编码压缩：h.264压缩算法，运算，内置DSP进行压缩运算。           
⑤视频流传输：网络传输、http/rtsp等。             
⑥视频存储：打包成MP4等格式存储，等待调阅。          
⑦视频回放：解码+播放。 

## 2.本方案系统整体架构介绍
### 硬件上
> * HI3518E单芯片提供：CPU+DSP+内置64MB DDR + ETHERNET MAC
> * 外置16M SPIFlash用来存放程序（uboot、kernel、rfs、app）
> * SDcard扩展提供用户数据区
> * 板载ethernet PHY和USB HOST扩展WIFI提供联网能力
> * sensor接口（并行数据通道+I2C控制通道）提供主板和sensor连接
> * 串口作为调试口和linux系统控制台

### 软件上
> * SPIFlash分区烧录uboot.bin、zImage、rootfs，并设置合理的环境变量使系统启动。
> * sensor、ethernet等硬件均需要驱动支持
> * app实现视频采集、编码压缩、网络传输等核心工作
> * 各种专业工作（譬如利用内置DSP实现h.264编码压缩）都由海思开发好并以ko的形式提供，有API文档参考，app编写者在sample的帮助下逐步实现自定义的功能。

## 可在此处下载本项目使用的HISI_SDK:[SDK_DOWNLOAD](https://pan.baidu.com/s/1jpmh3GCv9kPewSqKCXY99g)

