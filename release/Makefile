export HOMEDIR := /home/prilag/pr_modbus/kernel/openwrt
export TC_DIR := $(HOMEDIR)/staging_dir/toolchain-mipsel_gcc-linaro_uClibc-0.9.32
export STAGINGDIR := $(HOMEDIR)/staging_dir/target-mipsel_uClibc-0.9.32

export CC := $(TC_DIR)/bin/mipsel-openwrt-linux-gcc

# this is for embedded compilation #
export LIB := -L$(STAGINGDIR)/usr/lib -luClibc++ -L$(STAGINGDIR)/usr/lib/mysql -lmysqlclient -lz
export INC := -I$(STAGINGDIR)/usr/include/mysql -DBIG_JOINS=1 -fPIC -I$(TC_DIR)/include

# this is for desktop compilation #
#export LIB = /usr/lib/mysql -lmysqlclient
#export INC = /usr/include/mysql  -DBIG_JOINS=1 -fPIC

CFLAGS  = -Wall
LDFLAGS =

SRCS = test.c modbus.c sgpio.c
OBJS = ${SRCS:%.c=%.o}

.c.o:
	@ echo "----- object file is ready -----"
	$(CC) -c $(CFLAGS) $(INC) $^

all: ${OBJS}
	@ echo "[1] compiling"
	$(CC) ${OBJS} $(LDFLAGS) $(LIB) -o modbus_test

clean:
	rm $(BUILD)*.o
