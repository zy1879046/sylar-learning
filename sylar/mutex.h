/* ************************************************************************
> File Name:     mutex.h
> Author:        程序员XiaoJiu
> mail           zy18790460359@gmail.com
> Created Time:  一  5/20 17:00:36 2024
> Description:   
 ************************************************************************/
#pragma once
#include <semaphore.h>
#include <pthread.h>
#include "noncopyable.h"
// #include "my_semaphore.h"
#include <pthread.h>
#include <dispatch/dispatch.h>
#include <os/lock.h>


namespace sylar{
class Semaphore : Noncopyable{
public:
    Semaphore(uint32_t count = 0);
    ~Semaphore();
    void wait();
    void notify();
private:
    // sem_t m_semaphore;
    dispatch_semaphore_t m_semaphore;
};

// 局部锁的模板实现
template <typename T>
class ScopedLockImpl{
public:
    ScopedLockImpl(T& mutex):m_mutex(mutex){
        m_mutex.lock();
        m_locked = true;
    }
    ~ScopedLockImpl(){
        unlock();
    }

    void lock(){
        if(!m_locked){
            m_mutex.lock();
            m_locked = true;
        }
    }

    void unlock(){
        if(m_locked){
            m_mutex.unlock();
            m_locked = false;
        }
    }

private:
    // mutex
    T& m_mutex;
    bool m_locked;
};


template <typename T>
class ReadScopedLockImpl{
public:
    ReadScopedLockImpl(T& mutex) : m_mutex(mutex){
         m_mutex.rdlock();
         m_locked = true;
    }

    ~ReadScopedLockImpl(){
        unlock();
    }

    void lock(){
        if(!m_locked){
            m_mutex.rdlock();
            m_locked = true;
        }
    }

    void unlock(){
        if(m_locked){
            m_mutex.unlock();
            m_locked = false;
        }
    }
private:
    T& m_mutex;
    bool m_locked;
};

template <typename T>
class WriteScopedLockImpl{
public:
    WriteScopedLockImpl(T& mutex) : m_mutex(mutex){
         m_mutex.wrlock();
         m_locked = true;
    }

    ~WriteScopedLockImpl(){
        unlock();
    }

    void lock(){
        if(!m_locked){
            m_mutex.wrlock();
            m_locked = true;
        }
    }

    void unlock(){
        if(m_locked){
            m_mutex.unlock();
            m_locked = false;
        }
    }
private:
    T& m_mutex;
    bool m_locked;
};

class Mutex : Noncopyable{
public:
    using Lock = ScopedLockImpl<Mutex>;

    Mutex(){
        pthread_mutex_init(&m_mutex,nullptr);
    }
    ~Mutex(){
        pthread_mutex_destroy(&m_mutex);
    }

    void lock(){
        pthread_mutex_lock(&m_mutex);
    }

    void unlock(){
        pthread_mutex_unlock(&m_mutex);
    }
private:
    pthread_mutex_t m_mutex;
};

class NullMutex : Noncopyable{
public: 
    using Lock = ScopedLockImpl<NullMutex>;
    NullMutex(){

    }
    ~NullMutex(){

    }
    void lock(){

    }
    void unlock(){

    }
private:

};

// 读写互斥量
class RWMutex : Noncopyable{
public:
    using ReadLock = ReadScopedLockImpl<RWMutex>;
    using WriteLock = WriteScopedLockImpl<RWMutex>;
    RWMutex(){
        pthread_rwlock_init(&m_lock,nullptr);
    }
    ~RWMutex(){
        pthread_rwlock_destroy(&m_lock);
    }

    void rdlock(){
        pthread_rwlock_rdlock(&m_lock);
    }
    void wrlock(){
        pthread_rwlock_wrlock(&m_lock);
    }
    void unlock(){
        pthread_rwlock_unlock(&m_lock);
    }
private:
    pthread_rwlock_t m_lock;
};

class NullRWMutex : Noncopyable{
public:
    using ReadLock = ReadScopedLockImpl<NullRWMutex>;
    using WriteLock = WriteScopedLockImpl<NullRWMutex>;
    NullRWMutex(){
    }
    ~NullRWMutex(){
    }

    void rdlock(){
    }
    void wrlock(){
    }
    void unlock(){
    }
};

class Spinlock : Noncopyable{
public:
    using Lock = ScopedLockImpl<Spinlock>;
    Spinlock(){
        m_spinlock = OS_UNFAIR_LOCK_INIT;
    }
    ~Spinlock(){

    }
    void lock(){
        os_unfair_lock_lock(&m_spinlock);
    }
    void unlock(){
        os_unfair_lock_unlock(&m_spinlock);
    }

private:
    os_unfair_lock m_spinlock;
};

// class Spinlock : Noncopyable{
// public:
//     using Lock = ScopedLockImpl<Spinlock>;
//     Spinlock(){
//         pthread_spin_init(&m_mutex,0);
//     }
//     ~Spinlock(){
//         pthread_spin_destroy(&m_mutex);
//     }
//     void lock(){
//         pthread_spin_lock(&m_mutex);
//     }
//     void unlock(){
//         pthread_spin_unlock(&m_mutex);
//     }
// private:
//     pthread_spinlock_t m_mutex;
// };

};
