//  Copyright 2020  wenweijie Sensing & Controling Dept.
#pragma once

#include <mutex>
#include <list>

#include "xnet_sdk.h"   // XnetSDK

namespace nav_os {
namespace sdk {

class Singleton {
  public:
   static Singleton *Instance(bool create_if_needed = true) {
       static Singleton *instance = nullptr;
       if (!instance && create_if_needed) {
           static std::once_flag flag;
           std::call_once(flag, [&]{
               instance = new (std::nothrow) Singleton();
           });
       }
       return instance;
   }

   bool addXnode(XnetSdk *);
   bool delXnode(const std::string &);
   XnetSdk *getXnode(const std::string &);

  private:
   Singleton();
   ~Singleton();
   Singleton(const Singleton &) = delete;
   Singleton &operator=(const Singleton &) = delete;

   private:
    std::list<XnetSdk *> xnodes_;
};
}  // namespace sdk
}  // namespace nav_os
