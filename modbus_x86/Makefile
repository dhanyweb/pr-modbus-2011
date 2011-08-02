export HOMEDIR := /home/prilag/Desktop/pr_modbus/kernel
export TC_DIR := $(HOMEDIR)/staging_dir/toolchain-mipsel_gcc-linaro_uClibc-0.9.32

export CC := $(TC_DIR)/bin/mipsel-openwrt-linux-gcc
export LIB := $(TC_DIR)/usr/lib
export INC := $(TC_DIR)/usr/include

ACTUAL_CFLAGS  = -Wall
ACTUAL_LDFLAGS = -static -lpthread
SRCS = modbus.c test.c 
OBJS = ${SRCS:%.c=%.o}

.c.o:
	@ echo "----- object file is ready -----"
	gcc -c $(ACTUAL_CFLAGS) -I$(INC) $^

all: ${OBJS}
	@ echo "[1] compiling"
	gcc ${OBJS} $(ACTUAL_LDFLAGS) -L$(LIB) -o test

clean:
	rm $(BUILD)*.o
