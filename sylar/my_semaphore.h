/* ************************************************************************
> File Name:     my_semaphore.h
> Author:        程序员XiaoJiu
> mail           zy18790460359@gmail.com
> Created Time:  一  5/20 19:22:10 2024
> Description:   
 ************************************************************************/
#pragma once

#include <dispatch/dispatch.h>
namespace sylar{
// 定义信号量结构
    typedef struct {
        dispatch_semaphore_t semaphore;
    } my_sem_t;

    // 初始化信号量
    int my_sem_init(my_sem_t *sem, int pshared, unsigned int value) {
        sem->semaphore = dispatch_semaphore_create(value);
        if (sem->semaphore == NULL) {
            return -1;
        }
        return 0;
    }

    // 销毁信号量
    int my_sem_destroy(my_sem_t *sem) {
        // 在 GCD 中，信号量的释放由系统自动管理，所以这里不需要额外的操作
        return 0;
    }

    // 等待信号量
    int my_sem_wait(my_sem_t *sem) {
        dispatch_semaphore_wait(sem->semaphore, DISPATCH_TIME_FOREVER);
        return 0;
    }

    // 发布信号量
    int my_sem_post(my_sem_t *sem) {
        dispatch_semaphore_signal(sem->semaphore);
        return 0;
    }
};