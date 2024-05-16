#include "./config.h"
#include <algorithm>

namespace sylar{
    //Config::ConfigVarMap Config::s_datas;
    
    // ConfigVarBase::ptr Config::LookupBase(const std::string& name){
    //     auto it = s_datas.find(name);
    //     return it == s_datas.end() ? nullptr : it->second;
    //     // return nullptr;
    // }

    // static void ListAllMember(const std::string& prefix,const YAML::Node& node,std::list<std::pair<std::string,const YAML::Node>>& output){
    //     if(prefix.find_first_not_of(("abcdefghijklmnopqrstuvwxyz._0123456789")) != std::string::npos){
    //         SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) << "Prefix name invalid " << prefix <<" : "<<node;
    //         return;
    //     }
    //     output.push_back(std::make_pair(prefix,node));
    //     // 如果node是一个对象
    //     if(node.IsMap()){
    //         for(auto it = node.begin(); it != node.end();++it){
    //             ListAllMember(prefix.empty() ? it->first.Scalar() : prefix + "." + it->first.Scalar(),it->second,output);
    //         }
    //     }
    // }

    // void Config::LoadFromYaml(const YAML::Node& root){
    //     //定义一个list，装载的元素是<string,node>类型
    //     std::list<std::pair<std::string,const YAML::Node>> all_nodes;
    //     //root中获取日志里面的配置内容，ListAllMember则把里面的内容分解成一个个节点
    //     ListAllMember("",root,all_nodes);
    //     for(auto& i : all_nodes){
    //         std::string key = i.first;
    //         if(key.empty()) continue;
    //         std::transform(key.begin(),key.end(),key.begin(),::tolower);
    //         ConfigVarBase::ptr var = LookupBase(key);
    //         if(var){
    //             if(i.second.IsScalar()){
    //                 // var->fromString(i.second.Scalar());
    //             }else{
    //                 std::stringstream ss;
    //                 ss << i.second;
    //                 var->fromString(ss.str());
    //             }
    //         }
    //     }
        
    // }

};
