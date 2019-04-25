# $(ROOT)/mpp/Mafile

ifeq ($(PARAM_FILE), ) 
    PARAM_FILE:=../../Makefile.param
    include $(PARAM_FILE)
endif

ISP_KO := ./ko
ISP_LIB := ./lib
ISP_INC := ./include
3A_INC := ./3a/include
DEFOG_INC := ./defog

#HIARCH = hi3516A

.PHONY:clean all rel
all:
	@echo -e "\e[0;32;1m--Compiling 'isp'... Configs as follow:\e[0;36;1m"
	@echo ---- CROSS=$(CROSS)
	@echo ---- HIARCH=$(HIARCH), HICHIP=$(HICHIP), CVER=$(CVER), HIDBG=$(HIDBG)
	@echo ---- SDK_PATH=$(SDK_PATH) , PARAM_FILE=$(PARAM_FILE)
	@echo ---- LINUX_ROOT=$(LINUX_ROOT) 
	@echo -e "\e[0m"
	@mkdir -p $(REL_INC); cp $(ISP_INC)/*.h $(3A_INC)/*.h $(DEFOG_INC)/isp_dehaze.h $(REL_INC) -rf ;
	@for x in `find ./ -maxdepth 2 -mindepth 2 -name "Makefile" `; \
	   do cd `dirname $$x`; if [ $$? ]; then make || exit 1; cd ../; fi; done
	@mkdir -p $(REL_KO); cp $(ISP_KO)/*.ko $(REL_KO) -rf
	@mkdir -p $(REL_LIB); cp $(ISP_LIB)/*.a $(REL_LIB) -rf; cp $(ISP_LIB)/*.so $(REL_LIB) -rf

clean:
	@for x in `find ./ -maxdepth 2 -mindepth 2 -name "Makefile" `; \
	   do cd `dirname $$x`; if [ $$? ]; then make clean; cd ../; fi; done
	@rm $(REL_LIB)/libisp.a $(REL_LIB)/libsns* -rf
	@rm $(REL_KO)/$(HIARCH)_isp.ko -rf
