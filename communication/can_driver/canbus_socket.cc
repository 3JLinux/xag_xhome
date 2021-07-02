#include "canbus_socket.h"
using namespace xag_can;
using namespace xag_can::canbus;
ErrorCode SocketCan::start()
{
	if (ifcanstart_)
	{
		return ErrorCode::OK;
	}
	struct sockaddr_can addr;
	struct ifreq ifr;
	// 创建套接子
	if ((can_socket_fd_ = socket(PF_CAN,SOCK_RAW,CAN_RAW)) < 0)
	{
		std::cout<< "creating socketcan failed" << std::endl;
		return ErrorCode::SOCKCAN_ERROR_CREATE_ERROR;
	}

	//设置关联硬件
	strcpy(ifr.ifr_name,"can0");
	if(ioctl(can_socket_fd_,SIOCGIFINDEX,&ifr)< 0)
	{
		std::cout<< "ioctl error" << std::endl;
		return ErrorCode::SOCKCAN_ERROR_IOCTL_ERROR;
	}

	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;
	if (bind(can_socket_fd_,(struct sockaddr*)&addr,sizeof(addr)) < 0)
	{
		std::cout<< "bind failed" << std::endl;
		return ErrorCode::SOCKCAN_ERROR_BIND_ERROR;
	}

	// 设置过滤规则
	int ret;
	// struct can_filter filter;
	// filter.can_id = static_cast<uint32_t>(0x0000);
	// filter.can_mask = CAN_SFF_MASK;
	// ret = setsockopt(can_socket_fd_,SOL_CAN_RAW,CAN_RAW_FILTER,&filter,sizeof(filter));
	// if (ret < 0)
	// {
	// 	std::cout<< "enable the filter failed!" << std::endl;
	// 	return ErrorCode::SOCKCAN_ERROR_FILTER_ERROR;
	// }

	// 使能接受功能
	int enable =1;
	ret = setsockopt(can_socket_fd_,SOL_CAN_RAW,CAN_RAW_FD_FRAMES,&enable,sizeof(enable));
	if (ret < 0)
	{
		std::cout<< "enable RECV_ENABLE failed!" << std::endl;
		return ErrorCode::SOCKCAN_ERROR_RECV_ERROR;
	}

	// 设置非阻塞IO
	fcntl(can_socket_fd_,F_SETFL,fcntl(can_socket_fd_,F_GETFL,0) | O_NONBLOCK);
	std::cout<< "CAN start() is successful" << std::endl;
	ifcanstart_ = true;
	return ErrorCode::OK;

}

ErrorCode SocketCan::stop()
{
	if (ifcanstart_)
	{
		ifcanstart_ = false;
	}

	int ret = ::close(can_socket_fd_); 
	if(ret < 0)
	{
		std::cout<< "close failed" << std::endl;
		return ErrorCode::SOCKCAN_ERROR_CLOSE_ERROR;
	}
	std::cout<< "CAN stop() is successful" << std::endl;
	return ErrorCode::OK;
}

int SocketCan::read(uint8_t *buffer)
{
	int ret;
	int read_bytes_num = 0;
	Can_un *ptr=NULL;
	can_frame _frame;

	ptr = reinterpret_cast<Can_un *> (buffer);
	
	memset(&_frame,0,sizeof(_frame));
	ret = ::read(can_socket_fd_,&_frame,sizeof(_frame));
	if(ret < 0){
		return 0;
	}
	read_bytes_num++;
	memcpy(ptr->messages.data,_frame.data,_frame.can_dlc);
	ptr->messages.id = _frame.can_id;
	// std::cout << "can read:" << ptr->messages.data << std::endl;
	// for(int i; i<8; i++)
	// {
	// 	printf("0x%02x", _frame.data[i]);
	// }
	// printf("\n");
	return read_bytes_num;
}

int SocketCan::write(uint8_t *buffer,uint32_t id)
{
	int ret;
	int write_bytes_num=0;

//	std::cout<< "in write"<<std::endl;
	can_frame _frame;
	_frame.can_id = id;
	_frame.can_dlc = 8;
	memcpy(_frame.data,buffer,_frame.can_dlc);
	ret = ::write(can_socket_fd_,&_frame,sizeof(_frame));
	usleep(200);
	if(ret < 0)
	{
		return 1;
	}
	return write_bytes_num;
}


