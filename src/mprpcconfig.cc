#include <iostream>
#include "mprpcconfig.h"


void MprpcConfig::LoadConfigFile(const char* config_file)
{
    FILE* pf = fopen(config_file, "r");
    if (pf == nullptr)
    {
        std::cout << config_file << " is not exist!" << std::endl;
        exit(EXIT_FAILURE);
    }

    // 1. 注释  2. 正确的配置项 =   3. 去掉开头多余的空格
    while (!feof(pf))
    {
        char buf[BUFSIZ] = {0};
        fgets(buf, BUFSIZ, pf);

        std::string src_buf(buf);
        Trim(src_buf);

        // 判断#的注释
        if (src_buf[0] == '#' || src_buf.empty())
            continue;
        
        // 解析配置项
        int idx = src_buf.find('=');
        if (idx == -1)
            // 配置项不合法
            continue;

        std::string key;
        std::string value;
        key = src_buf.substr(0, idx);
        Trim(key);
        // 找出value的最后一个换行符'\n'
        int endidx = src_buf.find('\n', idx);
        // 删除'\n'
        value = src_buf.substr(idx+1, endidx-idx-1);
        m_configmap.insert({key, value});
    }   
}

std::string MprpcConfig::Load(const std::string& key)
{
    auto it = m_configmap.find(key);
    if (it == m_configmap.end())
    {
        return "";
    }
    return it->second;
}

void MprpcConfig::Trim(std::string &src_buf)
{
    // 去掉字符串前面多余的空格
    int idx = src_buf.find_first_not_of(' ');
    if (idx != -1)
        src_buf = src_buf.substr(idx, src_buf.size()-idx);

    // 去掉字符串后面多余的空格
    idx = src_buf.find_last_not_of(' ');
    if (idx != -1)
        src_buf = src_buf.substr(0, idx+1);
}