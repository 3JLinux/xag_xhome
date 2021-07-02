#ifndef _UTIL_UTILS_H__
#define _UTIL_UTILS_H__

#include <string>
#include <cxxabi.h>
#include <unistd.h> 
#include <functional>
#include <cstdlib>
#include <typeinfo>
namespace xag_nav{
namespace os{
namespace util{
template<class MessageT>
const std::string TypeToName() {
    static const std::string s_name = abi::__cxa_demangle(typeid(MessageT).name(), nullptr, nullptr, nullptr);
    return std::string(s_name);
}

// 用于类型转换
class TypeUtil {
public:
    static int8_t ToChar(const std::string& str);
    static int64_t Atoi(const std::string& str);
    static double Atof(const std::string& str);
    static int8_t ToChar(const char* str);
    static int64_t Atoi(const char* str);
    static double Atof(const char* str);
};



}
}
}

#endif //_UTIL_UTILS_H__