#include "./log.h"
#include <time.h>
#include <functional>
namespace sylar{
    const char* LogLevel::ToString(LogLevel::Level level){
        switch(level){
    #define XX(name) \
        case LogLevel::name: \
            return #name;\
            break;
        XX(DEBUG);
        XX(INFO);
        XX(WARN);
        XX(ERROR);
        XX(FATAL);
    #undef XX
        default:
        return "UNKNOW";    
            
        }
        return "UNKNOW";
    }
    void LogEvent::format(const char* fmt,...){
        va_list al;
        va_start(al, fmt);  //引入stdarg.h
        format(fmt, al);
        va_end(al);
    }
    void LogEvent::format(const char* fmt,va_list al){
        char* buf = nullptr;
        int len = vasprintf(&buf, fmt, al);
        if(len != -1) {
            m_ss << std::string(buf, len);
            free(buf);
        }
    }

    LogEventWrap::LogEventWrap(LogEvent::ptr e) : m_event(e){

    }
    LogEventWrap::~LogEventWrap(){
        m_event->getLogger()->log(m_event->getLevel(),m_event);
    }
    std::stringstream& LogEventWrap::getSS(){
        return m_event->getSS();
    }
    class MessageFormatItem : public LogFormatter::FormatItem{
    public:
        MessageFormatItem(const std::string& fmt = ""){}
        void format(std::ostream& os,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event)override{
            os << event->getcContent();
        }    
    };

    class LevelFormatItem : public LogFormatter::FormatItem{
    public:
        LevelFormatItem(const std::string& fmt = ""){}
        void format(std::ostream& os,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event)override{
            os << LogLevel::ToString(level);
        } 
    };
    class CLineFormatItem : public LogFormatter::FormatItem{
    public:
        CLineFormatItem(const std::string& fmt = ""){}
        void format(std::ostream& os,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event)override{
            os << '\n';
        } 
    };
    class LineFormatItem : public LogFormatter::FormatItem{
    public:
        LineFormatItem(const std::string& fmt = ""){}
        void format(std::ostream& os,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event)override{
            os << event->getLine();
        } 
    };

    class StringFormatItem : public LogFormatter::FormatItem{
    public:
        StringFormatItem(const std::string& str) : m_string(str){}
        void format(std::ostream& os,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event)override{
            os << m_string;
        } 
    private:
        std::string m_string;
    };

    class TabFormatItem : public LogFormatter::FormatItem{
    public:
        TabFormatItem(const std::string& str = ""){}
        void format(std::ostream& os,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event)override{
            os << "\t";
        } 
    };

    class FileNameFormatItem : public LogFormatter::FormatItem{
    public:
        FileNameFormatItem(const std::string& str = ""){}
        void format(std::ostream& os,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event)override{
            os << event->getFile();
        } 
    };

    class EplaseFormatItem : public LogFormatter::FormatItem{
    public:
        EplaseFormatItem(const std::string& fmt = ""){}
        void format(std::ostream& os,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event)override{
            os << event->getEplase();
        } 
    };

    class LoggerNameFormatItem : public LogFormatter::FormatItem{
    public:
        LoggerNameFormatItem(const std::string& fmt = ""){}
        void format(std::ostream& os,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event)override{
            os << logger->getName();
        } 
    };

    class ThreadIdFormatItem : public LogFormatter::FormatItem{
    public:
        ThreadIdFormatItem(const std::string& fmt = ""){}
        void format(std::ostream& os,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event)override{
            os << event->getThreadId();
        } 
    };

    class FiberIdFormatItem : public LogFormatter::FormatItem{
    public:
        FiberIdFormatItem(const std::string& fmt = ""){}
        void format(std::ostream& os,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event)override{
            os << event->getFiberId();
        } 
    };

    class DateTimeFormatItem : public LogFormatter::FormatItem{
    public:
        DateTimeFormatItem(const std::string& fmt = "%Y-%m-%d %H:%M:%S") : m_format(fmt){
            if(fmt.empty()){
                m_format = "%Y-%m-%d %H:%M:%S";
            }
        }
        void format(std::ostream& os,std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event)override{
            struct tm tm;
            time_t time = event->getTime();//当前时间戳
            localtime_r(&time,&tm);//当时间戳转换为本地时间，并存到tm中
            char buf[64];
            strftime(buf,sizeof(buf),m_format.c_str(),&tm);
            os << buf;
        } 
    private:
        std::string m_format;
    };
    LogEvent::LogEvent(std::shared_ptr<Logger> logger,LogLevel::Level level,const char* file,int32_t line,uint32_t eplase,uint32_t threadId,uint32_t fiberId,uint64_t time):
        m_file(file),m_line(line),m_eplase(eplase),m_threadId(threadId),m_fiberId(fiberId),m_time(time),m_logger(logger),m_level(level)
    {

    }
    Logger::Logger(const std::string& name):m_name(name),m_level(LogLevel::DEBUG){
        m_formatter.reset(new LogFormatter("%d{%Y-%m-%d %H:%M:%S}%T%t%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"));
    }

    void Logger::addAppender(LogAppender::ptr appender){
        if(!appender->getFormatter()){
            appender->setFormatter(m_formatter);//保证每个日志都有默认格式
        }
        m_appender.push_back(appender);
    }

    void Logger::delAppender(LogAppender::ptr appender){
        auto it = std::find(m_appender.begin(),m_appender.end(),appender);
        if(it != m_appender.end()) m_appender.erase(it);
    }

    void Logger::log(LogLevel::Level level ,LogEvent::ptr event){
        if(level >= m_level){
            auto self = shared_from_this();
            for(auto& i : m_appender){
                i->log(self,level,event);
            }
        }
    }

    void Logger::debug(LogEvent::ptr event){
        log(LogLevel::DEBUG,event);
    }
    void Logger::info(LogEvent::ptr event){
        log(LogLevel::INFO,event);
    }
    void Logger::warn(LogEvent::ptr event){
        log(LogLevel::WARN,event);
    }
    void Logger::fatal(LogEvent::ptr event){
        log(LogLevel::FATAL,event);
    }
    void Logger::error(LogEvent::ptr event){
        log(LogLevel::ERROR,event);
    }

    FileLogAppender::FileLogAppender(const std::string& filename) : m_filename(filename) ,m_level(LogLevel::DEBUG){
        
    }

    void FileLogAppender::log(Logger::ptr logger,LogLevel::Level level,LogEvent::ptr event){
        if(level >= m_level){
            reopen();
            m_filestream << m_formatter->format(logger,level,event);
            m_filestream.close();
        }
    }

    bool FileLogAppender::reopen(){
        if(m_filestream){
            m_filestream.close();
        }

        m_filestream.open(m_filename,std::ios::app);
        return !!m_filestream;
    }

    void StdoutAppender::log(Logger::ptr logger,LogLevel::Level level,LogEvent::ptr event){
        if(level >= m_level){
            std::cout << m_formatter->format(logger,level,event);
        }
    }

    LogFormatter::LogFormatter(const std::string& pattern) : m_pattern(pattern){
        init();
    }

    std::string LogFormatter::format(std::shared_ptr<Logger> logger,LogLevel::Level level,LogEvent::ptr event){
        std::stringstream ss;
        for(auto& i: m_items){
            i->format(ss,logger,level,event);
        }
        return ss.str();
    }

    void LogFormatter::init(){
        std::vector<std::tuple<std::string,std::string,int>> vec;   // tuple   [str,format,type]
        std::string nstr;   //当前str
        for(size_t i = 0; i < m_pattern.size(); ++i){//对每一个进行处理
            if(m_pattern[i] != '%'){//如果没有遇到 % 表示正常的字符
                nstr.append(1,m_pattern[i]);
                continue;
            }
            if((i+1) < m_pattern.size()){//如果遇到两个 % 表示就是 %
                if(m_pattern[i+1] == '%'){
                    nstr.append(1,'%');
                    continue;
                }
            }

            size_t n = i + 1;//从当前位置的下一个位置开始
            int fmt_status = 0;//表示当前状态，0 表示单独的格式，1表示正在处理 fmt ，2 表示已经处理完fmt
            std::string str; //当前的格式 例如 m ，c ， f 等
            std::string fmt;//当前格式化的格式 fmt
            size_t fmt_begin = 0;// 当前fmt的起始地址，就是{ 字符的下一个位置
            while(n < m_pattern.size()){//每次只处理一个
                if(!isalpha(m_pattern[n]) && m_pattern[n] != '{'){//如果当前 字符 既不是 字母也不是{ ,则代表格式错误
                    str = m_pattern.substr(i+1,n - i - 1);
                    break;
                }

                if(fmt_status == 0){//如果当前还处于格式化信息的部分，判断是否有fmt,遇到 { 代表fmt
                    if(m_pattern[n] == '{'){
                        str = m_pattern.substr(i+1,n - i - 1);//当前的格式化的信息存在 str 中
                        //解析格式
                        fmt_status = 1;//格式化 fmt 开始
                        fmt_begin = n;//格式化开始的位置
                        ++n;
                        // continue;
                    }
                }
                if(fmt_status == 1){//若处于查询 fmt 的方式，则判断是否结束 遇到 } 结束
                    while(n < m_pattern.size() && m_pattern[n] != '}') ++n;
                    if(m_pattern[n] == '}'){
                        fmt = m_pattern.substr(fmt_begin + 1,n-fmt_begin - 1);//若结束，则将f格式化的方式存在fmt
                        fmt_status = 0;//表示有格式化的方式的信息
                        ++n;
                        break;
                    }
                }
                ++n;
                if (n == m_pattern.size())
                {
                    if (str.empty())
                    {
                        str = m_pattern.substr(i + 1);
                    }
                }
            }
            if(fmt_status == 0){//若没有fmt信息，先将前面的纯字符修饰信息存起，再存当前的格式化信息
                if(!nstr.empty()){//这个是字符修饰信息，即像 printf("this is a %d",i)中的this is a 
                    vec.push_back(std::make_tuple(nstr,std::string(),0));//这个 0 代表string类型修饰
                    nstr.clear();
                }
                // str = m_pattern.substr(i+1,n-i-1);//当前的格式化信息
                vec.push_back(std::make_tuple(str,fmt,1));//将当前的格式化信息和格式存入vec中
                i = n -1 ;//因为每次while循环多一次 ++n，所以下一次循环需要 -1
            }else if(fmt_status == 1){//如果当前的格式化格式错误，则存储错误的格式化信息
                std::cout<< "pattern parse error: "<<m_pattern << " - " << m_pattern.substr(i) <<std::endl;
                vec.push_back(std::make_tuple("<parse_error>",fmt,0));
            }
            //最后面的字符创修饰
            if(!nstr.empty()){
                vec.push_back(std::make_tuple(nstr,std::string(),0));
            }
        }
        static std::map<std::string,std::function<FormatItem::ptr(const std::string& str)>>s_format_items = {//根据格式化信息创建一个map映射，key 为格式化信息，Value 为一个函数指针，返回一个formatItem
            // {"m",[](const std::string& fmt) { return FormatItem::ptr(new MessageFormatItem(fmt)); } }
#define XX(str,C) \
        {#str,[](const std::string& fmt){return FormatItem::ptr(new C(fmt));}}
        XX(m,MessageFormatItem),    // m : 消息
        XX(p,LevelFormatItem),      //p : 日志级别
        XX(r,EplaseFormatItem),     //r : 累计毫秒数
        XX(c,LoggerNameFormatItem), //c : 日志名称
        XX(t,ThreadIdFormatItem),   //t : 线程id
        XX(n,CLineFormatItem),       //n : 换行
        XX(d,DateTimeFormatItem),   //d : 时间
        XX(l,LineFormatItem),       //l : 行号
        XX(F,FiberIdFormatItem),    // f : 协程 id
        XX(T,TabFormatItem),        //t : Tab
        XX(f,FileNameFormatItem),
#undef XX
        };
        for(auto& i : vec){//对每一个存入的三元组
            if(std::get<2>(i) == 0){//如果代表string修饰，将修饰放入m_items中
                m_items.push_back(FormatItem::ptr(new StringFormatItem(std::get<0>(i))));
            }else{//否则代表格式话信息，将格式化的信息存入 m_items
                auto it = s_format_items.find(std::get<0>(i));
                if(it == s_format_items.end()){
                    m_items.push_back(FormatItem::ptr(new StringFormatItem("<<error_format %" + std::get<0>(i) + ">>")));
                }else{
                    m_items.push_back(it->second(std::get<1>(i)));
                }
            }
            // std::cout<<'(' << std::get<0>(i) <<") - (" << std::get<1>(i) << ") - (" <<std::get<2>(i) << ")"<< std::endl;
        }
    }

    LoggerManager::LoggerManager(){
        m_root.reset(new Logger);
        m_root->addAppender(LogAppender::ptr(new StdoutAppender));
    }
    Logger::ptr LoggerManager::getLogger(const std::string& name){
        auto it = m_loggers.find(name);
        return it == m_loggers.end() ? m_root : it->second;
    }
    void LoggerManager::pushLogger(const std::string& str,Logger::ptr logger){
        auto it = m_loggers.find(str);
        if(it == m_loggers.end())
        {
            m_loggers.insert({str,logger});
        }else{
            std::cout<< "logger " <<str <<" exited"<<std::endl;
        }
    }
    void LoggerManager::eraseLogger(const std::string& str){
        auto it = m_loggers.find(str);
        if(it != m_loggers.end()) m_loggers.erase(it);
    }
}