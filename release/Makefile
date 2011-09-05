export HOMEDIR := ~/pr_modbus/kernel/openwrt
export TC_DIR := $(HOMEDIR)/staging_dir/toolchain-mipsel_gcc-linaro_uClibc-0.9.32

export CC := $(TC_DIR)/bin/mipsel-openwrt-linux-gcc
export LIB := $(TC_DIR)/lib /usr/lib/mysql -lmysqlclient
export INC := $(TC_DIR)/include /usr/include/mysql -DBIG_JOINS=1 -fPIC

#export LIB += /usr/lib/mysql -lmysqlclient
#export INC += /usr/include/mysql  -DBIG_JOINS=1 -fPIC

ACTUAL_CFLAGS  = -Wall
ACTUAL_LDFLAGS = 
SRCS = test.c modbus.c sgpio.c
#SRCS = modbus_test.c
OBJS = ${SRCS:%.c=%.o}

.c.o:
	@ echo "----- object file is ready -----"
	$(CC) -c $(ACTUAL_CFLAGS) -I$(INC) $^

all: ${OBJS}
	@ echo "[1] compiling"
	$(CC) ${OBJS} $(ACTUAL_LDFLAGS) -L$(LIB) -o modbus_test

clean:
	rm $(BUILD)*.o
