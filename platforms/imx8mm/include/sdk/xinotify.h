#ifndef __XINOTIFY_H__
#define __XINOTIFY_H__
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <iostream>
#include <string.h>
#include <stdio.h>


namespace nav_os{
namespace sdk {

class XnetSdk;

class Xinotify{
 public:
    Xinotify(std::string path, XnetSdk *xnet_sdk);
    ~Xinotify();
    int init();
    int get_filestatus_handle();
 private:
    std::string path_;
    XnetSdk *xnet_sdk_;
    int fd_;
    int wd_;
    int len_;
    int nread_;
    FILE * fp_;
    int status_;
    char buf[1024];
    struct inotify_event *event_;
};

}//namespace sdk
}//namespace nav_os

#endif
