#
# isp firmware Makefile
#

ifeq ($(PARAM_FILE), )
	PARAM_FILE:=../../../Makefile.param
	include $(PARAM_FILE)
endif

ISP_PATH := $(SDK_PATH)/mpp/component/isp
LIBPATH = $(ISP_PATH)/lib
OBJPATH = ./obj

INI_INC  := $(ISP_PATH)/iniparser
3A_PATH := $(ISP_PATH)/include

#******* List all the release file ***********
REL_FILE := $(shell find $(ISP_PATH)/iniparser -name "*.h")

ifeq ($(MPP_BUILD), y)
EXT_PATH := $(SDK_PATH)/mpp/$(EXTDRV)
else
EXT_PATH := $(SDK_PATH)/mpp/extdrv
endif

BUS_DIR := $(EXT_PATH)/pwm

ARFLAGS = rcv
CFLAGS  = -Wall -fPIC

ifeq ($(HIGDB),HI_GDB)
CFLAGS += -g
endif

ifeq ($(CONFIG_JPEGEDCF), y)
     CFLAGS += -D ENABLE_JPEGEDCF 
endif

CFLAGS  += -O2
CFLAGS  += $(LIBS_CFLAGS)
DFLAGS  := -DEXT_REG

ISP_INC := $(ISP_PATH)/include
3A_INC := $(3A_PATH)/include

INC := -I$(REL_INC) -I$(ISP_INC) -I$(BUS_DIR) -I$(INI_INC) -I$(3A_INC)
#-I$(SRC_INC) -I$(VREG_INC) -I$(VREG_INC)/arch/$(HIARCH)

COMPILE = $(CC) $(CFLAGS) $(DFLAGS) -lm

$(OBJPATH)/%.o: ./%.c
	@(echo "compiling $< ...")
	@[ -e $(LIBPATH) ] || mkdir $(LIBPATH)
	@[ -e $(OBJPATH) ] || mkdir $(OBJPATH)
	@($(COMPILE) -o $@ -c $< $(INC))

SRCS = $(wildcard ./*.c)
OBJS = $(SRCS:%.c=%.o)
OBJS := $(OBJS:./%=obj/%)

LIB_A := $(LIBPATH)/lib_iniparser.a
LIB_S := $(LIBPATH)/lib_iniparser.so

define maco_cp
    $(foreach file,$(1), $(shell mkdir -p "$(2)";cp -f $(file) "$(2)/$(notdir $(file))"))
endef

all:$(OBJS)
	@($(AR) $(ARFLAGS) $(LIB_A) $(OBJS))
	@($(CC) $(LIBS_LD_CFLAGS) -shared -fPIC -o $(LIB_S) $(OBJS))
	@echo "---- Copy the head file to the release include directory"
	@$(call maco_cp,$(REL_FILE),$(REL_INC))	

clean:
	@$(RM) -rf $(LIB_A) $(LIB_S) $(OBJS)
	@$(RM) -rf $(LIBPATH) $(OBJPATH)

show:
	@echo "$(REL_FILE)"
