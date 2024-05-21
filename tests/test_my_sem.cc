#include <stdio.h>
#include "../sylar/my_semaphore.h"
int main() {
    sylar::my_sem_t sem;
    my_sem_init(&sem, 0, 1); // 初始化信号量，初始值为 1

    // 向信号量发送信号
    my_sem_post(&sem);

    // 等待信号量
    my_sem_wait(&sem);

    // 销毁信号量
    my_sem_destroy(&sem);

    return 0;
}
