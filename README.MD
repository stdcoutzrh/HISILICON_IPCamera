
![logo](https://github.com/StdCoutZRH/HISILICON_IPCamera/blob/master/pictures/hisilicon.jpg)
---------------------------------------------------

# HISILICON_IPCamera

This repository records my personal **IP_CAMERA** development by HISILICON 3518EV200.🌼

### Key Words:
**HISILICON3518EV200**,**AR0130**,**H.264**,**RTSP**,**MP4**,**OSD**,**IPCamera**.       
 
---------------------------------------------------

## Version Introduction
* [RtspIPC](https://github.com/StdCoutZRH/HISILICON_IPCamera/tree/RtspIPCamera) : Realized the function of transmitting video stream through RTSP protocol.
* [master](https://github.com/StdCoutZRH/HISILICON_IPCamera/tree/master) :Currently under development.Realized the function of transmitting video stream through RTSP protocol,save stream to MP4 and OSD.

## Introduction to the System Architecture of the Scheme
### Hardware：
> * HI3518EV200：CPU+DSP+64MB DDR.
> * External 16M SPI Flash is used to store programs(uboot、kernel、rfs、app).
> * SD Card stores recorded MP4 videos.
> * USB Wifi provides networking capabilities.
> * Sensor interface (parallel data channel+I2C control channel)provides motherboard and sensor connection.
> * Serial port as debugging port and Linux system console.

### Software：
> * SPI Flash partition burns uboot,kernel and rootfs,and sets reasonable environment variables to start the system.
> * App completes the core tasks of video capture, coding and compression, network transmission, etc.

## You can download HISI_SDK for this project here:[SDK_DOWNLOAD](https://pan.baidu.com/s/1jpmh3GCv9kPewSqKCXY99g)

