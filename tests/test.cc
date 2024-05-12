#include <iostream>
#include "../sylar/log.h"
#include "../sylar/util.h"
int main(int argc,char** argv){
    sylar::Logger::ptr logger(new sylar::Logger);
    logger->addAppender(sylar::LogAppender::ptr(new sylar::StdoutAppender));
    sylar::LogEvent::ptr event(new sylar::LogEvent(logger,sylar::LogLevel::FATAL,__FILE__,__LINE__,0,sylar::GetThreadId(),sylar::GetFiberId(),time((0))));
    // logger->log(sylar::LogLevel::DEBUG,event);
    sylar::FileLogAppender::ptr file_appender(new sylar::FileLogAppender("../log_out/log.txt"));
    // sylar::LogFormatter::ptr fmt(new sylar::LogFormatter("%d%T%m%n"));
    // file_appender->setFormatter(fmt);
    file_appender->setLevel(sylar::LogLevel::DEBUG);
    logger->addAppender(file_appender);
    logger->log(sylar::LogLevel::FATAL,event);
    SYLAR_LOG_DEBUG(logger)<< "this is a logger output";
    SYLAR_LOG_FMT_FATAL(logger,"fmt error %s","hy");

    auto l = sylar::LoggerMgr::GetInstance()->getLogger("xx");
    sylar::LoggerMgr::GetInstance()->pushLogger("file_fatal",logger);
    auto file = sylar::LoggerMgr::GetInstance()->getLogger("file_fatal");
    SYLAR_LOG_FATAL(file) << "XX";
    return 0;
}