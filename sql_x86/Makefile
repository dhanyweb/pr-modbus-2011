
export CC := gcc
export LIB := /usr/lib/mysql -lmysqlclient
export INC := /usr/include/mysql  -DBIG_JOINS=1 -fPIC

ACTUAL_CFLAGS  = -Wall
SRCS = test_2.c 
OBJS = ${SRCS:%.c=%.o}

.c.o:
	@ echo "----- object file is ready -----"
	$(CC) -c $(ACTUAL_CFLAGS) -I$(INC) $^

all: ${OBJS}
	@ echo "[1] compiling"
	$(CC) ${OBJS} -L$(LIB) -o test

clean:
	rm $(BUILD)*.o
	
#gcc -o output-file $(mysql_config --cflags) sql_sapp.c $(mysql_config --libs)
