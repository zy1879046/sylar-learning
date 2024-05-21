#include "mutex.h"
#include <functional>
#include <iostream>
#include <stdexcept>

namespace sylar{
    Semaphore::Semaphore(uint32_t count){
        // if(sem_init(&m_semaphore,0,count))
        //     throw std::logic_error("my_sem_init error");
        m_semaphore = dispatch_semaphore_create(count);
        if (m_semaphore == NULL) {
            throw std::logic_error("dispatch_semaphore_create error");
        }
    }
    Semaphore::~Semaphore(){
        // sem_destroy(&m_semaphore);
        dispatch_release(m_semaphore);
    }
    void Semaphore::wait(){
        // if(sem_wait(&m_semaphore)){
        //     throw std::logic_error("sem_wait error");
        // }
        dispatch_semaphore_wait(m_semaphore, DISPATCH_TIME_FOREVER);
    }
    void Semaphore::notify(){
        // if(sem_post(&m_semaphore)){
        //     throw std::logic_error("sem_post error");
        // }
        dispatch_semaphore_signal(m_semaphore);
    }
};
