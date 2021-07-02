#include <string>
#include <unistd.h>
#include "spdlog/spdlog.h"
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/un.h>


namespace xag_nav{
namespace os{
namespace transport{

class UdsUdpCom
{
  public:
    UdsUdpCom(const std::string& _socket_path) : udp_fd_(0),
                                                 is_init_(false)
    {
        if (_socket_path == "")
        {
            is_init_ = false;
            SPDLOG_WARN("UdsUdpCom : socket path error!");
            return;
        }

        unlink(_socket_path.c_str());

        sockudp_un_.sun_family = AF_UNIX;
        strcpy(sockudp_un_.sun_path, _socket_path.c_str());

        udp_fd_ = socket(AF_UNIX, SOCK_DGRAM, 0);
        if(udp_fd_ < 0)
        {
            is_init_ = false;
            SPDLOG_WARN("UdsUdpCom : request socket failed!");
            return;
        }

        int val = fcntl(udp_fd_, F_GETFL, 0);
        fcntl(udp_fd_, F_SETFL, val | O_NONBLOCK);

        if(bind(udp_fd_,(struct sockaddr *)&sockudp_un_,sizeof(sockudp_un_)) <0 )
        {
            is_init_ = false;
            SPDLOG_WARN("UdsUdpCom : bind failed!");
            return ;
        }
        is_init_ = true;
    }

    void setClientPath(const std::string& _client_path)
    {
        sockudp_client_un_.sun_family = AF_UNIX;
        strcpy(sockudp_client_un_.sun_path, _client_path.c_str());
    }

    virtual ~UdsUdpCom()
    {
        if(is_init_)
        {
            if (sockudp_un_.sun_path[0] != 0)
            {
                unlink(sockudp_un_.sun_path);
            }
        }
    }

    ssize_t recv(void *buff, size_t nbytes)
    {
        if(!is_init_)
        {
            return 0;
        }
        struct sockaddr *_preply_addr;
        return recvfrom(udp_fd_, buff, nbytes, 0, nullptr, nullptr);
    }

    ssize_t send(const void *buff, size_t nbytes)
    {
        if(!is_init_)
        {
            return 0;
        }
        return sendto(udp_fd_, buff, nbytes, 0, (struct sockaddr *)&sockudp_client_un_,sizeof(sockudp_client_un_));
    }

  private:
    struct sockaddr_un sockudp_un_;
    struct sockaddr_un sockudp_client_un_;
    int udp_fd_;

    bool is_init_;
};

    
} // namespace xag_nav
}// namespace os
}// namespace transport
