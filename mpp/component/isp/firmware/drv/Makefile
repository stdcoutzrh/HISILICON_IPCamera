#
# isp ko Makefile
#

ifeq ($(PARAM_FILE), )
	PARAM_FILE:=../../../../Makefile.param
	include $(PARAM_FILE)
endif

ifeq ($(MPP_BUILD), y)
EXT_PATH := $(SDK_PATH)/mpp/$(EXTDRV)
else
EXT_PATH := $(SDK_PATH)/mpp/extdrv
endif

CONFIG_JPEGEDCF=y

ISP_KO := ../../ko
EXTRA_CFLAGS += $(MPP_CFLAGS)
EXTRA_CFLAGS += -I$(REL_INC) -I$(SDK_PATH)/mpp/code/mkp/include
EXTRA_CFLAGS += -I$(REL_INC) -I$(SDK_PATH)/mpp/code/arch/$(HIARCH)/include
EXTRA_CFLAGS += -I$(SDK_PATH)/mpp/code/mkp_inc
EXTRA_CFLAGS += -I$(SDK_PATH)/mpp/component/isp/firmware/vreg
EXTRA_CFLAGS += -I$(SDK_PATH)/mpp/component/isp/firmware/vreg/arch/$(HIARCH)
EXTRA_CFLAGS += -I$(SDK_PATH)/mpp/component/isp/include
EXTRA_CFLAGS += -I$(EXT_PATH)/i2c
EXTRA_CFLAGS += -I$(EXT_PATH)/hi_i2c
EXTRA_CFLAGS += -I$(EXT_PATH)/ssp-sony

ifeq ($(CONFIG_GPIO_I2C), y)
    INC += -I$(EXT_PATH)/gpio-i2c-ex
    INC += -I$(EXT_PATH)/gpio-i2c
else
    INC += -I$(EXT_PATH)/i2c
    INC += -I$(EXT_PATH)/hi_i2c
endif

EXTRA_CFLAGS += $(INC)
ifeq ($(HIGDB),HI_GDB)
EXTRA_CFLAGS += -g
endif
ifeq ($(CONFIG_JPEGEDCF), y)
     EXTRA_CFLAGS += -D ENABLE_JPEGEDCF 
endif
ifeq ($(CONFIG_GPIO_I2C),y)
HI_GPIO_I2C:=HI_GPIO_I2C
else
HI_GPIO_I2C:=HI_GPIO_XXX
endif

EXTRA_CFLAGS += -D$(HI_FPGA) -D$(HI_GPIO_I2C) -D$(HIDBG)

ISP_SRC := isp.c mkp_acm.c isp_acm_lut.c  ../vreg/hi_drv_vreg.c  ../vreg/hi_vreg.c
ISP_SRC += isp_st.c
$(HIARCH)_isp-objs = $(ISP_SRC:%.c=%.o)

ifneq ($(KERNELRELEASE),)
	obj-m := $(HIARCH)_isp.o
endif

.PHONY: default clean
default:
	@$(MAKE) -C $(LINUX_ROOT) M=$(PWD) modules 
	@mkdir -p $(ISP_KO) && cp -rf *.ko $(ISP_KO)
clean: 
	@$(MAKE) -C $(LINUX_ROOT) M=$(PWD)/.. clean
	@if [ -e $(ISP_KO) ];then rm -rf "$(ISP_KO)"; fi
