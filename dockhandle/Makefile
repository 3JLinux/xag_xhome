#! gmake

CC = aarch64-imx8mm-linux-gnu-g++
EXE_CFLAGS = 
INCLUDE = -I../include -I../communication/interface -I../communication/message -I../communication/usr -I../WriteLog -I../WriteLog/LIST -I../WriteLog/COMMON

OBJS = dockhandle.o dockCanHandle.o

all:$(OBJS)

$(OBJS): *.cc
	$(CC) $(INCLUDE) -c $^ $(EXE_CFLAGS)

install:


clean:
	rm -f $(OBJS)
