#include <iostream>
#include "zookeeperutil.h"
#include "mprpcapplication.h"

// 全局的watcher观察器 zkserver给zkclient的通知
void global_watcher(zhandle_t* zh, int type, int state, const char* path, void* watcherCtx)
{
    if (type == ZOO_SESSION_EVENT)  // 回调消息类型是和会话相关的消息类型
    {
        if (state == ZOO_CONNECTED_STATE) // zkclient和zkserver连接成功
        {
            sem_t* sem = (sem_t*)zoo_get_context(zh);
            sem_post(sem);
        }   
    }
}

ZkClient::ZkClient(): m_zhandle(nullptr){}

ZkClient::~ZkClient()
{
    if (m_zhandle != nullptr)
        zookeeper_close(m_zhandle); // 关闭句柄, 释放资源
}

// 连接zkserver
void ZkClient::Start()
{
    std::string host = MprpcApplication::GetInstance().GetConfig().Load("zookeeper_ip");
    std::string port = MprpcApplication::GetInstance().GetConfig().Load("zookeeper_port");
    std::string connstr = host + ":" + port;

    /*
    zookeeper_mt: 多线程版本
    zookeeper的API客户端程序提供了三个线程
    API调用线程
    网络I/O线程 pthread_create poll
    watcher回调线程 pthread_create
    */

    m_zhandle = zookeeper_init(connstr.c_str(), global_watcher, 30000, nullptr, nullptr, 0);
    if (m_zhandle == nullptr)
    {
        std::cout << "zookeeper_init error!" << std::endl;
        exit(EXIT_FAILURE);
    }

    sem_t sem;
    sem_init(&sem, 0, 0);
    zoo_set_context(m_zhandle, &sem);

    sem_wait(&sem);
    std::cout <<"zookeeper_init success!" << std::endl;
}

void ZkClient::Create(const char* path, const char* data, int datalen, int state)
{
    char path_buf[128];
    int buflen = sizeof(path_buf);
    int flag;
    // 先判断path表示的znode节点是否存在, 如果存在, 就不再重复创建
    flag = zoo_exists(m_zhandle, path, 0, nullptr);
    if (flag == ZNONODE) // 节点不存在
    {
        // 创建指定path的znode节点
        flag = zoo_create(m_zhandle, path, data, datalen, &ZOO_OPEN_ACL_UNSAFE, state, path_buf, buflen);
        if (flag == ZOK)
        {
            std::cout << "znode create success... path: " << path << std::endl;
        }
        else
        {
            std::cout << "flag: " << flag << std::endl;
            std::cout << "znode create error... path: " << path << std::endl;
            exit(EXIT_FAILURE);
        }   
    }   
}

std::string ZkClient::GetData(const char* path)
{
    char buf[64];
    int buflen = sizeof(buf);
    int flag = zoo_get(m_zhandle, path, 0, buf, &buflen, nullptr);
    if (flag != ZOK)
    {
        std::cout << "get znode error... path: " << path << std::endl;
        return "";
    }
    else
    {
        return buf;
    }
}