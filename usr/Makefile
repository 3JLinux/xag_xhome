#! gmake


CC = aarch64-imx8mm-linux-gnu-gcc
CXX = aarch64-imx8mm-linux-gnu-g++
LD = aarch64-imx8mm-linux-gnu-g++
EXE_CFLAGS = -lpthread -lrt -ldl
INCLUDE = -I../include -lpthread -I../platforms/imx8mm/include -I../platforms/imx8mm/lib -I../communication/interface -I../communication/message -I../communication/usr -I../communication/can_driver -I../communication/usr \
-I../WriteLog -I../WriteLog/COMMON -I../WriteLog/LIST
OS_NAME = unix
ARCH_NAME = arm
PREFIX = /home/jjj/XAG/xag_xhome/obj
TARGET = unix
PROTOBUFSET = `pkg-config --cflags --libs protobuf`

OBJS = xag_xhome
BASE64FILE = base64/base64.o
DOCKHANDLEFILE = dockhandle/dockhandle.o
DOCKCANHANDLEFILE = dockhandle/dockCanHandle.o
HDLSFILE = hdls/hdls.o
TPSFILE = tps/tps.o
XHOMEFILE = xhome/xhome.o
XHOMEPBFILE = xhome/xhome.pb.o
UNICVERLOGFILE = hdls/unicverlog.o
WRITELOGFILE = WriteLog/WriteLog.o
MEMBFILE = WriteLog/COMMON/memb.o
LISTFILE = WriteLog/LIST/list.o
SUPERX4SABFILE = communication/usr/superx4_SAB.o
INTERFACEFILE = communication/interface/interface.o
CANDRIVERFILE = communication/can_driver/canbus_socket.o
MESSAGEFILE = communication/message/message.o

MASTER_OBJS = dock_main.o \
			  ../$(BASE64FILE) \
			  ../$(DOCKHANDLEFILE) \
			  ../$(DOCKCANHANDLEFILE) \
			  ../$(HDLSFILE) \
			  ../$(TPSFILE) \
			  ../$(XHOMEFILE) \
			  ../$(XHOMEPBFILE) \
			  ../$(UNICVERLOGFILE) \
			  ../$(WRITELOGFILE) \
			  ../$(MEMBFILE) \
			  ../$(LISTFILE) \
			  ../$(SUPERX4SABFILE) \
			  ../$(INTERFACEFILE) \
			  ../$(CANDRIVERFILE) \
			  ../$(MESSAGEFILE)

all:$(OBJS)

$(OBJS): $(MASTER_OBJS)
	$(LD) $(CFLAGS) $(PROTOBUFSET) $(INCLUDE) -o $@ $(MASTER_OBJS) $(EXE_CFLAGS)

%.o: %.cc
	$(CXX) $(CFLAGS) $(PROTOBUFSET) $(INCLUDE) -o $@ -c $<

clean: 
	rm -f $(MASTER_OBJS)
	rm -f $(OBJS)

install: xag_xhome
	cp $< ../obj

uninstall:
	rm -f ../obj/$(OBJS)
