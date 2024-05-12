#include "util.h"
#include <iostream>
#include <thread>
namespace sylar{
    uint32_t GetThreadId(){
        std::thread::id threadId = std::this_thread::get_id();
        // std::cout<<threadId<<std::endl;
        return *(uint32_t*)&threadId;
        // std::ostringstream oss;
        // oss << std::this_thread::get_id();
        // std::string stid = oss.str();
        // return std::stoul(stid);
    }
    uint32_t GetFiberId(){
        return 0;
    }
};