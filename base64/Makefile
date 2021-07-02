#! gmake

CC = aarch64-imx8mm-linux-gnu-g++
EXE_CFLAGS = 
INCLUDE = -I../include

OBJS = base64.o

all: $(OBJS)

$(OBJS): *.cc
	$(CC) $(INCLUDE) -c $^ $(EXE_CFLAGS)

install:


clean:
	rm -f $(OBJS)
