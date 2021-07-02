#! gmake

CC = aarch64-imx8mm-linux-gnu-gcc
CXX = aarch64-imx8mm-linux-gnu-g++
LD = aarch64-imx8mm-linux-gnu-g++
EXE_CFLAGS = 
INCLUDE = -I../include

OBJS = memb.o

all: $(OBJS)

$(OBJS): *.c
	$(CXX) $(INCLUDE) -c $^ $(EXE_CFLAGS)

clean:
	rm -f $(OBJS)

