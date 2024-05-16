/* ************************************************************************
> File Name:     log.h
> Author:        程序员XiaoJiu
> mail           zy18790460359@gmail.com
> Created Time:  一  5/ 6 18:34:01 2024
> Description:   
 ************************************************************************/
#pragma once

#include <cstdlib>
#include <string>
#include <stdint.h>
#include <memory>
#include <list>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <stdarg.h>
#include <iostream>
#include "singleton.h"
#include "util.h"
//简而言之创建一个LogEventWrap类，其中包含一个LogEvent指针，LogEvent中包含Logger指针，然后通过LogEventWrap的析构调用Logger的log
#define SYLAR_LOG_LEVEL(logger,level) \
    if(logger->getLevel() <= level) \
        sylar::LogEventWrap(sylar::LogEvent::ptr(new sylar::LogEvent(logger,level, \
            __FILE__,__LINE__,0,sylar::GetThreadId(),sylar::GetFiberId(),time(0)))).getSS()

#define SYLAR_LOG_DEBUG(logger) SYLAR_LOG_LEVEL(logger,sylar::LogLevel::DEBUG)
#define SYLAR_LOG_INFO(logger) SYLAR_LOG_LEVEL(logger,sylar::LogLevel::INFO)
#define SYLAR_LOG_WARN(logger) SYLAR_LOG_LEVEL(logger,sylar::LogLevel::WARN)
#define SYLAR_LOG_ERROR(logger) SYLAR_LOG_LEVEL(logger,sylar::LogLevel::ERROR)
#define SYLAR_LOG_FATAL(logger) SYLAR_LOG_LEVEL(logger,sylar::LogLevel::FATAL)

// 格式化输出
#define SYLAR_LOG_FMT_LEVEL(logger, level, fmt, ...) \
    if(logger->getLevel() <= level) \
        sylar::LogEventWrap(sylar::LogEvent::ptr(new sylar::LogEvent(logger, level, \
                        __FILE__, __LINE__, 0, sylar::GetThreadId(),\
                sylar::GetFiberId(), time(0)))).getEvent()->format(fmt, __VA_ARGS__)

#define SYLAR_LOG_FMT_DEBUG(logger, fmt, ...) SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::DEBUG, fmt, __VA_ARGS__)
#define SYLAR_LOG_FMT_INFO(logger, fmt, ...)  SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::INFO, fmt, __VA_ARGS__)
#define SYLAR_LOG_FMT_WARN(logger, fmt, ...)  SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::WARN, fmt, __VA_ARGS__)
#define SYLAR_LOG_FMT_ERROR(logger, fmt, ...) SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::ERROR, fmt, __VA_ARGS__)
#define SYLAR_LOG_FMT_FATAL(logger, fmt, ...) SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::FATAL, fmt, __VA_ARGS__)

#define SYLAR_LOG_ROOT() \
    sylar::LoggerMgr::GetInstance()->getRoot()

#define SYLAR_LOG_NAME(name) sylar::LoggerMgr::GetInstance()->getLogger(name)


namespace sylar{
    //提前声明。有的类会用到该类
    class Logger;
    class LogEvent;
    class LogLevel;
    class LoggerManager;
    //自定义的日志级别
    class LogLevel{
    public:
        enum Level{
            UNKNOW = 0,     //未知级别
            DEBUG = 1,      //DEBUG级别
            INFO = 2,       //INFO 级别
            WARN = 3,       //WARN 级别
            ERROR = 4,      //ERROR 级别
            FATAL = 5       //FATAL 级别
        };

        static const char* ToString(LogLevel::Level level);
        static LogLevel::Level FromString(const std::string& level);
    };

    class LogEvent{
    public:
        using ptr = std::shared_ptr<LogEvent>;//智能指针，让其他函数声明该类的指针更加方便
        LogEvent(std::shared_ptr<Logger> logger,LogLevel::Level level,const char* file,int32_t line,uint32_t eplase,uint32_t threadId,uint32_t fiberId,uint64_t time);
        
        const char* getFile()const{
            return m_file;
        }
        int32_t getLine()const {
            return m_line;
        }
        uint32_t getEplase()const{
            return m_eplase;
        }
        uint32_t getThreadId()const{
            return m_threadId;
        }
        uint32_t getFiberId()const{
            return m_fiberId;
        }
        uint64_t getTime()const{
            return m_time;
        }
        const std::string getcContent()const{
            return m_ss.str();
        }
        std::stringstream& getSS(){
            return m_ss;
        }
        std::shared_ptr<Logger> getLogger()const{return m_logger;}
        LogLevel::Level getLevel()const {return m_level;}
        void setLevel(LogLevel::Level val){m_level = val;}
        void format(const char* fmt,...);
        void format(const char* fmt,va_list al);
    private:
        const char* m_file = nullptr;//文件名
        int32_t m_line = 0;         //行号
        uint32_t m_eplase = 0;      //程序启动到现在的毫秒
        uint32_t m_threadId = 0;    //线程ID
        uint32_t m_fiberId = 0;     //协程ID
        uint64_t m_time;            //时间戳
        std::shared_ptr<Logger> m_logger;
        LogLevel::Level m_level;
        std::stringstream m_ss;     //内容
    };

    class LogEventWrap{
    public:
        LogEventWrap(LogEvent::ptr);
        ~LogEventWrap();
        std::stringstream& getSS();
        LogEvent::ptr getEvent()const{return m_event;}
    private:
        LogEvent::ptr m_event;
    };

    

    //日志格式
    class LogFormatter{
    public:
        using ptr = std::shared_ptr<LogFormatter>;
        LogFormatter(const std::string& pattern);
        std::string format(std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event);
    public:
        class FormatItem{
            public:
                using ptr = std::shared_ptr<FormatItem>;
                virtual ~FormatItem(){}
                virtual void format(std::ostream& os,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) = 0;
        };
        const bool isError(){
            return m_error;
        }
        const std::string getPattern()const{
            return m_pattern;
        }
        void init();
    private:
        std::string m_pattern;                  //解析格式
        std::vector<FormatItem::ptr> m_items;   //解析内容
        bool m_error = false;
    };

    //日志输出地
    class LogAppender{
    public:
        using ptr = std::shared_ptr<LogAppender>;
        virtual ~LogAppender(){}

        virtual void log(std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) = 0;
        void setFormatter(LogFormatter::ptr formatter){m_formatter = formatter;}
        LogFormatter::ptr getFormatter()const{return m_formatter;}
        void setLevel(LogLevel::Level level){
            m_level = level;
            m_hasLevel = true;
        }
        const LogLevel::Level getLevel()const{
            return m_level;
        }
        virtual std::string toYamlString() = 0;
        void setHasFormatter(const bool& v){
            m_hasFormatter = v;
        }
        const bool getHasFormatter()const{
            return m_hasFormatter;
        }
        void setHasLevel(const bool&v){
            m_hasLevel = v;
        }
        const bool getHasLevel()const{
            return m_hasLevel;
        }
    protected:
        LogLevel::Level m_level = LogLevel::DEBUG;
        LogFormatter::ptr m_formatter;
        bool m_hasFormatter = false;
        bool m_hasLevel = false;
    };

    //日志输出器
    class Logger : public std::enable_shared_from_this<Logger>{
        friend class LoggerManager;
    public:
        using ptr = std::shared_ptr<Logger>;
        Logger(const std::string& name = "root");
        void log(LogLevel::Level level ,LogEvent::ptr event);
        //不同级别的日志输出函数
        void debug(LogEvent::ptr event);
        void info(LogEvent::ptr event);
        void warn(LogEvent::ptr event);
        void fatal(LogEvent::ptr event);
        void error(LogEvent::ptr event);

        void addAppender(LogAppender::ptr appender);
        void delAppender(LogAppender::ptr appender);
        LogLevel::Level getLevel()const {return m_level;}
        void setLevel(LogLevel::Level level){m_level = level;}
        const std::string& getName()const{return m_name;}
        void setFormatter(LogFormatter::ptr formatter){
            m_formatter = formatter;
            for(auto& i : m_appender){
                if(!i->getHasFormatter()){
                    i->setFormatter(formatter);
                    i->setHasFormatter(false);
                }
            }
        }
        void setFormatter(const std::string& str){
            LogFormatter::ptr fmt(new LogFormatter(str));
            if(!fmt->isError())
                setFormatter(fmt);
        }
        const LogFormatter::ptr getFormatter()const{
            return m_formatter;
        }
        void clearAppender(){
            m_appender.clear();
        }
        std::string toYamlString();
    private:
        std::string m_name;                         //日志名称
        LogLevel::Level m_level;                    //级别
        std::list<LogAppender::ptr> m_appender;     //Appender集合
        LogFormatter::ptr m_formatter;              //格式
        Logger::ptr m_root;
    };

    //输出方法分类
    
    //输出到控制台
    class StdoutAppender : public LogAppender{
    public:
        using ptr = std::shared_ptr<StdoutAppender>;
        void log(Logger::ptr logger,LogLevel::Level level,LogEvent::ptr event)override;
        std::string toYamlString()override;
    };

    //输出到文件
    class FileLogAppender :public LogAppender{
    public:
        using ptr = std::shared_ptr<FileLogAppender>;
        FileLogAppender(const std::string& filename);
        void log(Logger::ptr logger,LogLevel::Level level,LogEvent::ptr event)override;
        bool reopen();
        std::string toYamlString()override;
    private:
        std::string m_filename;
        std::ofstream m_filestream;
    };

    class LoggerManager{
    public:
        LoggerManager();
        Logger::ptr getLogger(const std::string& name);
        void pushLogger(const std::string& str,Logger::ptr);
        void eraseLogger(const std::string&);
        Logger::ptr getRoot(){return m_root;}
        std::string toYamlString();
    private:
        void init();
        std::unordered_map<std::string,Logger::ptr> m_loggers;
        Logger::ptr m_root;
    };
    using LoggerMgr = sylar::Singleton<LoggerManager>;
};

