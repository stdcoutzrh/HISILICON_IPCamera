# Hisilicon Hi3516 sample Makefile

include ../Makefile.param
#ifeq ($(SAMPLE_PARAM_FILE), )
#     SAMPLE_PARAM_FILE:=../Makefile.param
#     include $(SAMPLE_PARAM_FILE)
#endif


# target source
INC	 := ./sample/
SRC  := $(wildcard *.c) 
IVE_SAMPLE_SRC := $(wildcard sample/*.c) 
OBJ  := $(SRC:%.c=%.o)
IVE_SAMPLE_OBJ  := $(IVE_SAMPLE_SRC:%.c=%.o)
TARGET := $(OBJ:%.o=%)
.PHONY : clean all

all: $(TARGET)


#MPI_LIBS += $(REL_LIB)/libmpi.a
#MPI_LIBS += $(REL_LIB)/libhdmi.a


$(TARGET):%:%.o $(COMM_OBJ) $(IVE_SAMPLE_OBJ)
	$(CC) $(CFLAGS) -lpthread -lm -o $@ $^ -I$(INC) $(MPI_LIBS) $(AUDIO_LIBA) $(SENSOR_LIBS) $(HVD_LIBS) $(MPI_LIBS)

clean:
	@echo "$(CFLAGS)"
	@echo "$(IVE_SAMPLE_OBJ)"
	@rm -f $(TARGET)
	@echo $(OBJ)
	@rm -f $(OBJ)
	@rm -f $(COMM_OBJ)
	@rm -f $(IVE_SAMPLE_OBJ)
cleanstream:
	@rm -f *.h264
	@rm -f *.jpg
	@rm -f *.mjp
	@rm -f *.mp4
