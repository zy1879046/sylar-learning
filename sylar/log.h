/* ************************************************************************
> File Name:     log.h
> Author:        程序员XiaoJiu
> mail           zy18790460359@gmail.com
> Created Time:  一  5/ 6 18:34:01 2024
> Description:   
 ************************************************************************/
#pragma once

#include <string>
#include <stdint.h>
#include <memory>
#include <list>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <iostream>

namespace sylar{
    //提前声明。有的类会用到该类
    class Logger;
    class LogEvent{
    public:
        using ptr = std::shared_ptr<LogEvent>;//智能指针，让其他函数声明该类的指针更加方便
        LogEvent(const char* file,int32_t line,uint32_t eplase,uint32_t threadId,uint32_t fiberId,uint64_t time);
        
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
    private:
        const char* m_file = nullptr;//文件名
        int32_t m_line = 0;         //行号
        uint32_t m_eplase = 0;      //程序启动到现在的毫秒
        uint32_t m_threadId = 0;    //线程ID
        uint32_t m_fiberId = 0;     //协程ID
        uint64_t m_time;            //时间戳
        std::stringstream m_ss;     //内容
    };
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

        void init();
    private:
        std::string m_pattern;                  //解析格式
        std::vector<FormatItem::ptr> m_items;   //解析内容
    };

    //日志输出地
    class LogAppender{
    public:
        using ptr = std::shared_ptr<LogAppender>;
        virtual ~LogAppender(){}

        virtual void log(std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event) = 0;
        void setFormatter(LogFormatter::ptr formatter){m_formatter = formatter;}
        LogFormatter::ptr getFormatter()const{return m_formatter;}
    protected:
        LogLevel::Level m_level = LogLevel::DEBUG;
        LogFormatter::ptr m_formatter;
    };

    //日志输出器
    class Logger : public std::enable_shared_from_this<Logger>{
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
    private:
        std::string m_name;                         //日志名称
        LogLevel::Level m_level;                    //级别
        std::list<LogAppender::ptr> m_appender;     //Appender集合
        LogFormatter::ptr m_formatter;              //格式
    };

    //输出方法分类
    
    //输出到控制台
    class StdoutAppender : public LogAppender{
    public:
        using ptr = std::shared_ptr<StdoutAppender>;
        void log(Logger::ptr logger,LogLevel::Level level,LogEvent::ptr event);
    };

    //输出到文件
    class FileLogAppender :public LogAppender{
    public:
        using ptr = std::shared_ptr<FileLogAppender>;
        FileLogAppender(const std::string& filename);
        void log(Logger::ptr logger,LogLevel::Level level,LogEvent::ptr event);
        
        bool reopen();
    private:
        std::string m_filename;
        std::ofstream m_filestream;
    };
};

