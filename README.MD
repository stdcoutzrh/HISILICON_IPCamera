
![hisi_logo](https://github.com/StdCoutZRH/HISILICON_IPCamera/blob/master/pictures/hisilicon.jpg)

--------------------------------------------------

# RTSP_IPcamera

---------------------------------------------------
* usage:
```shell
	cd mpp_rtspipc/RtspIPC/rtspipc
	make
```

* This branch is the first version of this project.You can compile it in any directory as long as you configure the development environment(you can refer to this bolg:[[configure de]](https://blog.csdn.net/PecoHe/article/details/88843312) ).
* App of this branch completes the function of coding video into H.264 stream and pushing it out with RTSP protocol.You can transmit video over wired networks or WiFi as long as you have the right IP address for the development board.
* You can select your ChipID and SensorType in "mpp_rtspipc/RtspIPC/Makefile.para".
* You can use video players such as VLC to enter the correct URL (such as rtsp://192.168.1.10:554/stream_chn0.h264) to open the network stream to watch the video.

