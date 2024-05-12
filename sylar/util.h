#pragma once
#include <thread>
#include <sstream>
namespace sylar{
    //获取线程id
    uint32_t GetThreadId();

    //获取协程id
    uint32_t GetFiberId();
};