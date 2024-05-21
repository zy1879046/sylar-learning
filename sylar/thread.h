/* ************************************************************************
> File Name:     sylar/thread.h
> Author:        程序员XiaoJiu
> mail           zy18790460359@gmail.com
> Created Time:  四  5/16 20:46:58 2024
> Description:   
 ************************************************************************/
#pragma once
#include <string>
#include <thread>
#include <functional>
#include <memory>
#include <pthread.h>

#include "mutex.h"

namespace sylar{
class Thread{
public:
    using ptr = std::shared_ptr<Thread>;
    Thread(std::function<void()> cb,const std::string& name);
    ~Thread();

    //获取线程id
    const pid_t getId()const{
        return m_id;
    }
    //获取线程名称
    const std::string& getName()const{
        return m_name;
    }
    //等待线程完成
    void join();
    //获取当前线程指针
    static Thread* GetThis();
    //获取当前线程名称
    static const std::string& GetName();
    //设置线程名称
    static void SetName(const std::string& name);
private:
    Thread(const Thread&) = delete;
    Thread(const Thread&&) = delete;
    Thread& operator=(const Thread&) = delete;
    // 线程执行函数
    static void* run(void *arg);

private:
    pid_t m_id = -1; //线程id
    pthread_t m_thread = 0; //线程结构
    std::function<void()> m_cb; //线程执行函数
    std::string m_name; //线程名称
    Semaphore m_semaphore;
};
};
