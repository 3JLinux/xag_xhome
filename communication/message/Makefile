#! gmake

CXX = aarch64-imx8mm-linux-gnu-g++
EXE_CFLAGS = 
INCLUDE = 

MESSAGE_OBJS = message.o

all: $(MESSAGE_OBJS)

$(MESSAGE_OBJS): *.cc
	$(CXX) $(INCLUDE) -c $^ $(EXE_CFLAGS)

install:

clean:
	rm -f $(MESSAGE_OBJS)
