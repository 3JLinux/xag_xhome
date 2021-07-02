#! gmake

CC = aarch64-imx8mm-linux-gnu-g++
EXE_CFLAGS = 
INCLUDE = -I../include

HDLS_OBJS = hdls.o unicverlog.o

all:$(HDLS_OBJS)

$(HDLS_OBJS): *.cc
	$(CC) $(INCLUDE) -c $^ $(EXE_CFLAGS)

install:


clean:
	rm -f $(HDLS_OBJS)
