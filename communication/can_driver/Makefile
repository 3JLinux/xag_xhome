#! gmake

CXX	= aarch64-imx8mm-linux-gnu-g++
EXE_CFLAGS = 
INCLUDE = -I../message

CAN_OBJS = canbus_socket.o

all: $(CAN_OBJS)

$(CAN_OBJS): *.cc
	$(CXX) $(INCLUDE) -c $^ $(EXE_CFLAGS)

install:

clean:
	rm -f $(CAN_OBJS)
