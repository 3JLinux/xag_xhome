#! gmake

CC = aarch64-imx8mm-linux-gnu-gcc
CXX = aarch64-imx8mm-linux-gnu-g++

EXE_CFLAGS = 
INCLUDE = -I../inlcude

OBJS = list.o


all: $(OBJS)

$(OBJS): *.c
	$(CXX) -c $^ $(EXE_CFLAGS) $(INCLUDE)

clean:
	rm -f $(OBJS)

