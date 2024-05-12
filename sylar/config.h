#pragma once
#include <iterator>
#include <memory>
#include <sstream>
#include <string>
#include <boost/lexical_cast.hpp>
#include <map>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#include <yaml-cpp/yaml.h>
#include "log.h"

namespace sylar{
    //配置参数基类
    //将一个node节点所有的参数存入output中，即一个
    static void ListAllMember(const std::string& prefix,const YAML::Node& node,std::list<std::pair<std::string,const YAML::Node>>& output){
        if(prefix.find_first_not_of(("abcdefghijklmnopqrstuvwxyz._0123456789")) != std::string::npos){
            SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) << "Prefix name invalid " << prefix <<" : "<<node;
            return;
        }
        output.push_back(std::make_pair(prefix,node));
        // 如果node是一个对象
        if(node.IsMap()){
            for(auto it = node.begin(); it != node.end();++it){
                ListAllMember(prefix.empty() ? it->first.Scalar() : prefix + "." + it->first.Scalar(),it->second,output);
            }
        }
    }

    //基础类型转换 把F 转换为 T
    template <typename F, typename T>
    class LexicalCast{
    public:
        T operator()(const F&v){
            return boost::lexical_cast<T>(v);
        }
    };

    //string -> vector 偏特化
    template <typename T>
    class LexicalCast<std::string,std::vector<T>>{
    public:
        std::vector<T> operator()(const std::string& v){
            //调用 YAML 中的 load函数 ，接受一个string类型输入，将其转换为node结构
            YAML::Node node = YAML::Load(v);
            std::vector<T> vec;
            std::stringstream ss;// 使用字符串流来获取node中的每个string值
            for(size_t i = 0; i < node.size(); ++i){
                ss.str("");
                ss << node[i];
                vec.push_back(LexicalCast<std::string,T>()(ss.str()));//// 这里相当于调用基础类型转换，如果传入的vec里面元素是int类型，那么就是string -> int
            }
            return vec;
        }
    };
    
    //vector -> string
    template <typename T>
    class LexicalCast<std::vector<T>,std::string>{
    public:
        std::string operator()(const std::vector<T>& v){
            YAML::Node node;
            for(auto& i : v){
                node.push_back(YAML::Load(LexicalCast<T,std::string>()(i)));
            }
            std::stringstream ss;
            ss <<node;
            return ss.str();
        }
    };

    //string -> list偏特化
    template <typename T>
    class LexicalCast<std::string,std::list<T>>{
    public:
        std::list<T> operator()(const std::string& v){
            //调用 YAML 中的 load函数 ，接受一个string类型输入，将其转换为node结构
            YAML::Node node = YAML::Load(v);
            std::list<T> vec;
            std::stringstream ss;// 使用字符串流来获取node中的每个string值
            for(size_t i = 0; i < node.size(); ++i){
                ss.str("");
                ss << node[i];
                vec.push_back(LexicalCast<std::string,T>()(ss.str()));//// 这里相当于调用基础类型转换，如果传入的vec里面元素是int类型，那么就是string -> int
            }
            return vec;
        }
    };
    
    //list -> string
    template <typename T>
    class LexicalCast<std::list<T>,std::string>{
    public:
        std::string operator()(const std::list<T>& v){
            YAML::Node node;
            for(auto& i : v){
                node.push_back(YAML::Load(LexicalCast<T,std::string>()(i)));
            }
            std::stringstream ss;
            ss <<node;
            return ss.str();
        }
    };

    //string -> set 偏特化
    template <typename T>
    class LexicalCast<std::string,std::set<T>>{
    public:
        std::set<T> operator()(const std::string& v){
            //调用 YAML 中的 load函数 ，接受一个string类型输入，将其转换为node结构
            YAML::Node node = YAML::Load(v);
            std::set<T> vec;
            std::stringstream ss;// 使用字符串流来获取node中的每个string值
            for(size_t i = 0; i < node.size(); ++i){
                ss.str("");
                ss << node[i];
                vec.insert(LexicalCast<std::string,T>()(ss.str()));//// 这里相当于调用基础类型转换，如果传入的vec里面元素是int类型，那么就是string -> int
            }
            return vec;
        }
    };
    
    //set -> string
    template <typename T>
    class LexicalCast<std::set<T>,std::string>{
    public:
        std::string operator()(const std::set<T>& v){
            YAML::Node node;
            for(auto& i : v){
                node.push_back(YAML::Load(LexicalCast<T,std::string>()(i)));
            }
            std::stringstream ss;
            ss <<node;
            return ss.str();
        }
    };

    //string -> unordered_set 偏特化
    template <typename T>
    class LexicalCast<std::string,std::unordered_set<T>>{
    public:
        std::unordered_set<T> operator()(const std::string& v){
            //调用 YAML 中的 load函数 ，接受一个string类型输入，将其转换为node结构
            YAML::Node node = YAML::Load(v);
            std::vector<T> vec;
            std::stringstream ss;// 使用字符串流来获取node中的每个string值
            for(size_t i = 0; i < node.size(); ++i){
                ss.str("");
                ss << node[i];
                vec.insert(LexicalCast<std::string,T>()(ss.str()));//// 这里相当于调用基础类型转换，如果传入的vec里面元素是int类型，那么就是string -> int
            }
            return vec;
        }
    };
    
    //unordered_set -> string
    template <typename T>
    class LexicalCast<std::unordered_set<T>,std::string>{
    public:
        std::string operator()(const std::unordered_set<T>& v){
            YAML::Node node;
            for(auto& i : v){
                node.push_back(YAML::Load(LexicalCast<T,std::string>()(i)));
            }
            std::stringstream ss;
            ss <<node;
            return ss.str();
        }
    };

    //string -> map
    template <typename T>
    class LexicalCast<std::string,std::map<std::string,T>>{
    public:
        std::map<std::string,T> operator()(const std::string& v){
            YAML::Node node = YAML::Load(v);
            std::map<std::string,T> vec;
            std::stringstream ss;
            for(auto it = node.begin(); it != node.end();++it){
                ss.str("");
                ss << it->second;
                vec.insert(std::make_pair(it->first.Scalar(),LexicalCast<std::string,T>()(ss.str())));
            }
            return vec;
        }
    };

    //map -> string
    template <typename T>
    class LexicalCast<std::map<std::string,T>,std::string>{
   public:
       std::string operator()(const std::map<std::string,T>& v){
            YAML::Node node(YAML::NodeType::Map);
            for(auto& i : v){
                node[i.first] = YAML::Load(LexicalCast<T,std::string>()(i.second));
            }
            std::stringstream ss;
            ss << node;
            return ss.str();
       }
    };
    
    // string -> unordered_map
    template <typename T>
    class LexicalCast<std::string,std::unordered_map<std::string,T>>{
   public:
       std::unordered_map<std::string, T> operator()(const std::string& v){
           YAML::Node node = YAML::Load(v);
           typename std::unordered_map<std::string,T> vec;
           std::stringstream ss;
           for(auto it = node.begin(); it != node.end(); ++it){
               ss.str("");
               ss << it->second;
               vec.insert(std::make_pair(it->first.Scalar(),LexicalCast<std::string,T>()(ss.str())));
           }
           return vec;
       }
    };

    // unordered_map -> string
    template <typename T>
    class LexicalCast<std::unordered_map<std::string,T>,std::string>{
   public:
       std::string operator()(const std::unordered_map<std::string,T>& v){
           YAML::Node node(YAML::NodeType::Map);
           for(auto& i : v){
               node[i.first] = YAML::Load(LexicalCast<T,std::string>()(i.second));
           }
           std::stringstream ss;
           ss << node;
           return ss.str();
       }
    };

    class ConfigVarBase{
    public:
        using ptr = std::shared_ptr<ConfigVarBase>;
        ConfigVarBase(const std::string& name,const std::string& description = "" ):
            m_name(name),m_description(description){
                std::transform(m_name.begin(),m_name.end(),m_name.begin(),::tolower);
                }

        const std::string& getName()const{return m_name;}//返回配置参数名称
        const std::string& getDescription()const{return m_description;}//返回配置参数描述
        virtual std::string toString() = 0;
        virtual bool fromString(const std::string& val) = 0;
        virtual ~ConfigVarBase(){}
    private:
        std::string m_name;
        std::string m_description;
    };

    //配置参数类
    template< typename T ,typename FromStr = LexicalCast<std::string,T> ,typename ToStr = LexicalCast<T,std::string> >
    class ConfigVar : public ConfigVarBase{
    public:
        using ptr = std::shared_ptr<ConfigVar>;
        ConfigVar(const std::string& name,const T& default_value,const std::string& description):
            ConfigVarBase(name,description),m_val(default_value){}

        std::string toString()override{
            try{
                //return boost::lexical_cast<std::string>(m_val);
                return ToStr()(m_val);
            }catch(std::exception& e){
                SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) << "ConfigVar::toString exception" <<e.what() <<"convert: "<< typeid(m_val).name() <<"to string";
            }
            return "";
        }
        bool fromString(const std::string& val)override{
            try {
                //m_val = boost::lexical_cast<T>(val);
                setValue(FromStr()(val));
                return true;
            }catch(std::exception& e){
                SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) << "ConfigVar::fromString exception" <<e.what() <<"convert: string to "<<typeid(m_val).name();
            }
            return false;
        }
        const T& getValue()const{return m_val;}
        void setValue(const T& val){m_val = val;}
    private:
        T m_val;
    };
    //配置管理类
    class Config {
    public:
        // using ptr = std::shared_ptr<Config>;
        using ConfigVarMap = std::map<std::string,ConfigVarBase::ptr>;

        static ConfigVarBase::ptr LookupBase(const std::string& name){
            auto it = s_datas.find(name);
            return it == s_datas.end() ? nullptr : it->second;
        // return nullptr;
        };
        static void LoadFromYaml(const YAML::Node& root){
            //定义一个list，装载的元素是<string,node>类型
            std::list<std::pair<std::string,const YAML::Node>> all_nodes;
            //root中获取日志里面的配置内容，ListAllMember则把里面的内容分解成一个个节点
            ListAllMember("",root,all_nodes);
            for(auto& i : all_nodes){
                std::string key = i.first;
                if(key.empty()) continue;
                std::transform(key.begin(),key.end(),key.begin(),::tolower);
                ConfigVarBase::ptr var = LookupBase(key);
                if(var){
                    if(i.second.IsScalar()){
                        var->fromString(i.second.Scalar());
                    }else{
                        std::stringstream ss;
                        ss << i.second;
                        var->fromString(ss.str());
                    }
                }
            }
            
        };
        
        template<typename T>
        static typename ConfigVar<T>::ptr Lookup(const std::string& name,const T& default_value,const std::string& description = ""){
            auto it = s_datas.find(name);
            if(it != s_datas.end()){
                auto tmp = std::dynamic_pointer_cast<ConfigVar<T>>(it->second);
                if(tmp){
                    SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << "Lookup name = " <<name << " exists";
                }else{
                    SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << "Lookup name = " << name << " exists but type not " << typeid(T).name();
                    return nullptr;
                }
            }
            //发现异常，即不符合参数类型
            if(name.find_first_not_of("abcdefghijklmnopqrstuvwxyz0123456789._") != std::string::npos){
                SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) <<"Lookup name invalid: " << name;
                throw std::invalid_argument(name);
            }
            //无异常即定义
            typename ConfigVar<T>::ptr v(new ConfigVar<T>(name,default_value,description));
            s_datas[name] = v;
            return v;
        };

        template<typename T>
            static typename ConfigVar<T>::ptr Lookup(const std::string& name){
                auto it = s_datas.find(name);
                if(it == s_datas.end()){
                    return nullptr;
                }
                return std::dynamic_pointer_cast<ConfigVar<T>>(it->second);//找到就转换为具体类的只能指针
            };
    private:
        static ConfigVarMap s_datas;
    };
};
