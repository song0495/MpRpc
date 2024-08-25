#include <iostream>
#include <string>
#include <unistd.h>
#include "mprpcapplication.h"

void ShowArgsHelp()
{
    std::cout << "format: command -i <config_file>" << std::endl;
}

MprpcConfig MprpcApplication::m_config;

void MprpcApplication::Init(int argc, char** argv)
{
    if (argc < 2)
    {
        ShowArgsHelp();
        exit(EXIT_FAILURE);
    }
    
    int ret = 0;
    std::string config_flie;
    while ((ret = getopt(argc, argv, "i:")) != -1)
    {
        switch (ret)
        {
        case 'i':
            config_flie = optarg;
            break;
        case '?':
            ShowArgsHelp();
            exit(EXIT_FAILURE);
        case ':':
            ShowArgsHelp();
            exit(EXIT_FAILURE);
        default:
            break;
        }
    }

    // 开始加载配置文件 rpcserver_ip; rpcserver_port; zookeeper_ip; zookeeper_port
    m_config.LoadConfigFile(config_flie.c_str());

    //std::cout << "rpcserver_ip: " << m_config.Load("rpcserver_ip") << std::endl;
    //std::cout << "rpcserver_port: " << m_config.Load("rpcserver_port") << std::endl;
    //std::cout << "zookeeper_ip: " << m_config.Load("zookeeper_ip") << std::endl;
    //std::cout << "zookeeper_port: " << m_config.Load("zookeeper_port") << std::endl;
}

MprpcApplication& MprpcApplication::GetInstance()
{
    static MprpcApplication application;
    return application;
}

MprpcConfig& MprpcApplication::GetConfig()
{
    return m_config;
}