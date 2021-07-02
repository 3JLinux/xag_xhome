#! gmake

CC = aarch64-imx8mm-linux-gnu-g++
INCLUDE = -I./ -I./COMMON -I./LIST -I../include

WRITELOG_OBJ = WriteLog.o 

all: $(WRITELOG_OBJ)

$(WRITELOG_OBJ):*.c
	$(CC) -c $^ $(INCLUDE)

clean:
	rm -f $(WRITELOG_OBJ)
