#include "thread.h"
#include "log.h"
#include "util.h"
#include <functional>
#include <stdexcept>

namespace sylar{
    //定义线程局部变量
    static thread_local Thread* t_thread = nullptr;
    //定义线程局部变量的名称
    static thread_local std::string t_thread_name = "UNKNOW";
    //定义日志系统
    static sylar::Logger::ptr g_logger = SYLAR_LOG_NAME("system");
    Thread::Thread(std::function<void()> cb,const std::string& name):m_cb(cb),m_name(name){
        if(name.empty()){
            m_name = "UNKNOW";
        }
        int rt = pthread_create(&m_thread,nullptr,&Thread::run,this);
        if(rt){
            SYLAR_LOG_ERROR(g_logger) << "pthread_create thread fail, rt = " << rt << " nama = " <<name;
            throw std::logic_error("pthread_create error");
        }
        m_semaphore.wait();
    }
    Thread::~Thread(){
        if(m_thread){
            pthread_detach(m_thread);
        }
    }

    //等待线程完成
    void Thread::join(){
        pthread_join(m_thread,nullptr);
    }
    //获取当前线程指针
    Thread* Thread:: GetThis(){
        return t_thread;
    }
    //获取当前线程名称
    const std::string& Thread::GetName(){
        return t_thread_name;
    }
    //设置线程名称
    void Thread::SetName(const std::string& name){
        if(t_thread){
            t_thread->m_name = name; 
        }
        t_thread_name = name;
    }
    
    void* Thread::run(void *arg){
    Thread* thread = (Thread*)arg;
    t_thread = thread;
    t_thread_name = thread->m_name;
    thread->m_id = sylar::GetThreadId();
    pthread_setname_np(thread->m_name.substr(0,15).c_str());
    std::function<void()> cb;
    cb.swap(thread->m_cb);
    thread->m_semaphore.notify();
    cb();
    return 0;
    }
};
