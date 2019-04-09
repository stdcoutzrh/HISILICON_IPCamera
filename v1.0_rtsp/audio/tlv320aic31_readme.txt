1. Update the file: mpp/ko/pinmux_hi3518e.sh,
   It need to use I2S2 interface, so open the pinmux setting as below. 
	i2s_pin_mux()
	{
		# pin_mux with GPIO1 
		#himm 0x200f007c 0x3;		    # i2s_bclk_tx
		#himm 0x200f0080 0x3;		    # i2s_sd_tx
		#himm 0x200f0084 0x3;		    # i2s_mclk
		#himm 0x200f0088 0x3;		    # i2s_ws_tx
		#himm 0x200f008c 0x3;		    # i2s_ws_rx
		#himm 0x200f0090 0x3;		    # i2s_bclk_rx
		#himm 0x200f0094 0x3;		    # i2s_sd_rx

		# pin_mux with UART1
		#himm 0x200f00bc 0x2;		    # i2s_sd_tx
		#himm 0x200f00c0 0x2;		    # i2s_ws_tx
		#himm 0x200f00c4 0x2;		    # i2s_mclk
		#himm 0x200f00c8 0x2;		    # i2s_sd_rx
		#himm 0x200f00d0 0x2;		    # i2s_bclk_tx

		# pin_mux with JTAG
		himm 0x200f00d4 0x3;		    # i2s_mclk
		himm 0x200f00d8 0x3;		    # i2s_ws_tx
		himm 0x200f00dc 0x3;		    # i2s_sd_tx
		himm 0x200f00e0 0x3;		    # i2s_sd_rx
		himm 0x200f00e4 0x3;		    # i2s_bclk_tx
	}


	#i2c0_pin_mux;
	i2c2_pin_mux
	vicap_pin_mux;
	i2s_pin_mux;
	#vo_bt656_mode;

2. update the file: mpp/ko/sysctl_hi3518e.sh£¬
   It need to use the external audio codec, please modify the script as below. 
	# msic config
	#himm 0x201200E0 0xd				# internal codec-AIO MCLK out, CODEC AIO TX MCLK 
	himm 0x201200E0 0xe				# external codec: AIC31/AK7756EN AIO MCLK out, CODEC AIO TX MCLK 

3. update the file: mpp\ko\load3518e£¬
   It need to insmod tlv_320aic31's driver. 
	insmod extdrv/tlv_320aic31.ko

4. modify the makefile parameter: mpp/sample/Makefile.param£¬from ACODEC_TYPE to  ACODEC_TYPE_TLV320AIC31. 
   It means use the external codec tlv_320aic31 sample code.
	################ select audio codec type for your sample ################
	ACODEC_TYPE ?= ACODEC_TYPE_INNER
	#external acodec
	#ACODEC_TYPE ?= ACODEC_TYPE_AK7756
	#ACODEC_TYPE ?= ACODEC_TYPE_TLV320AIC31

5. Rebuild the sample and get the sample_audio.
