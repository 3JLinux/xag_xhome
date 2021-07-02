#! gmake 

CXX = aarch64-imx8mm-linux-gnu-g++
EXE_CFLAGS = 
INCLUDE = -I../message -I../can_driver -I../usr

INTERFACE_OBJS = intearface.o

all: $(INTERFACE_OBJS)

$(INTERFACE_OBJS): *.cc
	$(CXX) $(INCLUDE) -c $^ $(EXE_CFLAGS)

install:

clean:
	rm -f $(INTERFACE_OBJS)
