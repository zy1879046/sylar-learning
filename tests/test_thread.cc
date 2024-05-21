/* ************************************************************************
> File Name:     test_thread.cpp
> Author:        程序员XiaoJiu
> mail           zy18790460359@gmail.com
> Created Time:  四  5/16 21:23:09 2024
> Description:   
 ************************************************************************/

#include "../sylar/sylar.h"
#include <vector>

sylar::Logger::ptr g_logger = SYLAR_LOG_ROOT();

int count = 0;
sylar::RWMutex s_mutex;

void func1(){

    for(int i = 0; i < 100000; ++i){
        sylar::RWMutex::WriteLock lock(s_mutex);
        ++count;
    }

    SYLAR_LOG_INFO(g_logger) << "name: " <<sylar::Thread::GetName()
                             << " this.name: " <<sylar::Thread::GetThis()->getName()
                             << " id: " << sylar::GetThreadId()
                             <<" this.id: " << sylar::Thread::GetThis()->getId();
}
void func2(){
    auto i = 100;
    while(--i){
        SYLAR_LOG_INFO(g_logger) << "####################################";
    }
}

void func3(){
    auto i = 100;
    while(--i){
        SYLAR_LOG_INFO(g_logger) << "=====================================";
    }
}
int main(int argc , char* argv[]){
    SYLAR_LOG_INFO(g_logger) << "thread test begin";
    YAML::Node root = YAML::LoadFile("../bin/conf/log2.yml");
    sylar::Config::LoadFromYaml(root);
    std::vector<sylar::Thread::ptr> thrs;
    for(int i = 0; i < 2 ;++i){
        sylar::Thread::ptr thr(new sylar::Thread(func2,"name_" + std::to_string(i)));
        sylar::Thread::ptr thr2(new sylar::Thread(func3,"name_" + std::to_string(i)));

        thrs.push_back(thr2);
        thrs.push_back(thr);
    }
    for(int i = 0; i < thrs.size(); ++i){
        thrs[i]->join();
    }
    // func2();
    SYLAR_LOG_INFO(g_logger) << "thread test end";
    SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << "count = " << count;
    return 0;

}
