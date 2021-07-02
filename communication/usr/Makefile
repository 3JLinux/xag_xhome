#! gmake

CXX = aarch64-imx8mm-linux-gnu-g++
EXE_CFLAGS =
INCLUDE = -I../message -I../interface -I../can_driver

OBJS = superx4_SAB.o

all: $(OBJS)

$(OBJS): *.cc
	$(CXX) $(INCLUDE) -c $^ $(EXE_CFLAGS)

install:

clean:
	rm -f $(OBJS)
