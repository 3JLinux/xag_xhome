#! gmake

CANFILE = communication

all: xag_dock

xag_dock:
	$(MAKE) -C hdls all
	$(MAKE) -C base64 all
	$(MAKE) -C xhome all
	$(MAKE) -C tps all
	$(MAKE) -C dockhandle all
	$(MAKE) -C WriteLog all
	$(MAKE) -C WriteLog/COMMON all
	$(MAKE) -C WriteLog/LIST all
	$(MAKE) -C $(CANFILE)/can_driver all 
	$(MAKE) -C $(CANFILE)/interface all
	$(MAKE) -C $(CANFILE)/message all
	$(MAKE) -C $(CANFILE)/usr all
	$(MAKE) -C usr all

install: xag_dock
	$(MAKE) -C usr $@

clean:
	$(MAKE) -C usr $@
	$(MAKE) -C dockhandle $@
	$(MAKE) -C base64 $@
	$(MAKE) -C xhome $@
	$(MAKE) -C tps $@
	$(MAKE) -C hdls $@
	$(MAKE) -C WriteLog $@
	$(MAKE) -C WriteLog/COMMON $@
	$(MAKE) -C WriteLog/LIST $@
	$(MAKE) -C $(CANFILE)/can_driver all $@
	$(MAKE) -C $(CANFILE)/interface all $@
	$(MAKE) -C $(CANFILE)/message all $@
	$(MAKE) -C $(CANFILE)/usr all $@

mrproper: clean
	$(MAKE) -C usr $@
	$(MAKE) -C dockhandle $@
	$(MAKE) -C base64 $@
	$(MAKE) -C xhome $@
	$(MAKE) -C tps $@
	$(MAKE) -C hdls $@
	$(MAKE) -C WriteLog $@
	$(MAKE) -C WriteLog/COMMON $@
	$(MAKE) -C WriteLog/LIST $@
	$(MAKE) -C $(CANFILE)/can_driver all $@
	$(MAKE) -C $(CANFILE)/interface all $@
	$(MAKE) -C $(CANFILE)/message all $@
	$(MAKE) -C $(CANFILE)/usr all $@
