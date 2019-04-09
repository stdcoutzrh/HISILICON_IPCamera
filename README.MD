![logo](https://github.com/StdCoutZRH/HISILICON_IPC/blob/v1.0_rtsp/pictures/hisilicon.jpg)

---------------------------------------------------

<div align=center>![logo](https://github.com/StdCoutZRH/HISILICON_IPC/blob/v1.0_rtsp/pictures/hisilicon.jpg)

---------------------------------------------------
# HISILICON_IPC
---------------------------------------------------

此仓库为个人用**海思HI3518EV200**开发**IP_CAMERA**(1.0版本)的记录。

## 1.v1.0_rtsp
>* 实现了rtsp传输视频码流的功能。

## 2.本方案系统整体架构介绍
### 硬件上
> * HI3518E单芯片：CPU+DSP+内置64MB DDR + ETHERNET MAC
> * 外置16M SPIFlash用来存放程序（uboot、kernel、rfs、app）
> * SDcard扩展提供用户数据区
> * 板载ethernet PHY和USB HOST扩展WIFI提供联网能力
> * sensor接口（并行数据通道+I2C控制通道）提供主板和sensor连接
> * 串口作为调试口和linux系统控制台

### 软件上
> * SPIFlash分区烧录uboot.bin、zImage、rootfs，并设置合理的环境变量使系统启动。
> * sensor、ethernet等硬件均需要驱动支持
> * app实现视频采集、编码压缩、网络传输等核心工作
> * 各种专业工作由海思开发好并以ko的形式提供。

## 可在此处下载本项目使用的HISI_SDK:[SDK_DOWNLOAD](https://pan.baidu.com/s/1jpmh3GCv9kPewSqKCXY99g)

