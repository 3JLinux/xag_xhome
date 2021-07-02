#include <unistd.h>
#include "hdls.h"
#include "dockhandle.h"
#include "unicverlog.h"
#include "../communication/usr/superx4_SAB.h"
#include "loguser.h"



int main()
{
	logFileInit();
	hdls_socket_init();
	XHOME_subscription();
	get_sensor_thread_create();
	hdls_rcv_thread_create();
	dockhandle_thread_create();
	unicverlog_thread_create();
	can_locationStateCheck_thread_create();

	get_sensor_thread_wait();
	hdls_rcv_thread_wait();
	dockhandle_thread_wait();
	unicverlog_thread_wait();
	can_locationStateCheck_thread_wait();
    // while(1)
    // {
    //     docking_mission_load();
    //     sleep(10);
    // }
}
