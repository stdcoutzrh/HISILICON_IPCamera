# Hisilicon Hi3531 sample Makefile

include ../Makefile.param


# target source
SRC  := $(wildcard *.c) 
OBJ  := $(SRC:%.c=%.o)

MPI_LIBS += $(REL_LIB)/libtde.a $(SENSOR_LIBS) $(AUDIO_LIBA)

TARGET := $(OBJ:%.o=%)
.PHONY : clean all

all: $(TARGET)

$(TARGET):%:%.o $(COMM_OBJ)
	$(CC) $(CFLAGS) -lpthread -lm -o $@ $^ $(MPI_LIBS) 

clean:
	@-rm -f $(TARGET)
	@-rm -f $(OBJ)
