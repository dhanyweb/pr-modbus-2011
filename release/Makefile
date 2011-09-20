export CC = gcc

# this is for desktop compilation #
export LIB = -L/usr/lib/mysql -lmysqlclient -lpthread
export INC = -I/usr/include/mysql  -DBIG_JOINS=1 -fPIC

CFLAGS  = -Wall
LDFLAGS =

SRCS = main.c modbus.c sgpio.c parser.c scheduler.c dataacq.c

OBJS = ${SRCS:%.c=%.o}

.c.o:
	@ echo "----- object file is ready -----"
	$(CC) -c $(CFLAGS) $(INC) $^

all: ${OBJS}
	@ echo "[1] compiling"
	$(CC) ${OBJS} $(LDFLAGS) $(LIB) -o modbus_test

clean:
	rm $(BUILD)*.o
