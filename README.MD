
![logo](https://github.com/StdCoutZRH/HISILICON_IPCamera/blob/master/pictures/hisilicon.jpg)
---------------------------------------------------

# HISILICON_IPCamera

---------------------------------------------------


This repository records my personal **IP_CAMERA** development by HISILICON 3518EV200.

### Key Words:
**AR0130**,**ISP(image signal processing)**,**H.264**,**RTSP**,**MP4**.       
 

## 1.Version Introduction
* rtspipc : Realize the function of transmitting video stream through RTSP protocol.

## 2.Introduction to the System Architecture of the Scheme
### Hardware：
> * HI3518EV200：CPU+DSP+64MB DDR.
> * External 16M SPI Flash is used to store programs(uboot、kernel、rfs、app).
> * SD Card stores recorded MP4 videos.
> * USB Wifi provides networking capabilities.
> * Sensor interface (parallel data channel+I2C control channel)provides motherboard and sensor connection.
> * Serial port as debugging port and Linux system console.

### Software：
> * SPI Flash partition burns uboot.bin,zImage and rootfs,and sets reasonable environment variables to start the system.
> * App completes the core tasks of video capture, coding and compression, network transmission, etc.

## 3.You can download HISI_SDK for this project here.:[SDK_DOWNLOAD](https://pan.baidu.com/s/1jpmh3GCv9kPewSqKCXY99g)

