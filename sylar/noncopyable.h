/* ************************************************************************
> File Name:     noncopyable.h
> Author:        程序员XiaoJiu
> mail           zy18790460359@gmail.com
> Created Time:  一  5/20 16:57:33 2024
> Description:   
 ************************************************************************/
#pragma once

namespace sylar{
class Noncopyable{
public:
    Noncopyable() = default;
    ~Noncopyable() = default;
    Noncopyable(const Noncopyable& ) = delete;
    Noncopyable(const Noncopyable&&) = delete;
    Noncopyable& operator=(const Noncopyable&) = delete;
};
};
