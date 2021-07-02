#! gmake

CC = aarch64-imx8mm-linux-gnu-g++
EXE_CFLAGS = 
INCLUDE = -I../include -I../platforms/imx8mm/include -I../platforms/imx8mm/lib -lpthread
PROTOBUFSET = `pkg-config --cflags --libs protobuf`

OBJS = xhome.o xhome.pb.o

all:$(OBJS)

$(OBJS): *.cc
	$(CC) $(INCLUDE) -c $^ $(EXE_CFLAGS)

install:


clean:
	rm -f $(OBJS)

