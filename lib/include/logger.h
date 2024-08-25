#pragma once
#include <string>
#include "lockqueue.h"

enum Loglevel
{
    INFO, // 普通信息
    ERROR, // 错误信息
};

// mprpc框架提供的日志系统
class Logger
{
public:
    // 获取日志的单例
    static Logger& GetInstance();
    // 设置日志级别
    void SetLogLevel(Loglevel level);
    // 写日志
    void Log(std::string msg);

private:
    int m_logLevel; // 记录日志级别
    LockQueue<std::string> m_lckQue; // 日志缓冲队列

    Logger();
    Logger(const Logger&) = delete;
    Logger(Logger&&) = delete;
};

// 定义宏 __VA_ARGS__宏用来接受不定数量的参数, 当可变参数的个数为0时, 编译报错, ##可以省略参数输入
#define LOG_INFO(logmsgformat, ...)                         \
    do                                                      \
    {                                                       \
        Logger& logger = Logger::GetInstance();             \
        logger.SetLogLevel(INFO);                           \
        char buf[1024] = {0};                               \
        snprintf(buf, 1024, logmsgformat, ##__VA_ARGS__);   \
        logger.Log(buf);                                    \
    } while (0)

#define LOG_ERR(logmsgformat, ...)                          \
    do                                                      \
    {                                                       \
        Logger& logger = Logger::GetInstance();             \
        logger.SetLogLevel(ERROR);                          \
        char buf[1024] = {0};                               \
        snprintf(buf, 1024, logmsgformat, ##__VA_ARGS__);   \
        logger.Log(buf);                                    \
    } while (0)
    