#pragma once
#include <string>
#include <google/protobuf/service.h>

class MprpcController: public google::protobuf::RpcController
{
public:
    MprpcController();
    virtual void Reset();
    virtual bool Failed() const;
    virtual std::string ErrorText() const;
    virtual void SetFailed(const std::string& reason);

    // 目前未实现的具体功能
    virtual void StartCancel();
    virtual bool IsCanceled() const;
    virtual void NotifyOnCancel(google::protobuf::Closure* callback);

private:
    bool m_failed; // rpc方法执行过程中的状态
    std::string m_errText; // rpc方法执行过程中的错误信息
};