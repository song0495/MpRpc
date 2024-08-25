#include <iostream>
#include "mprpcapplication.h"
#include "user.pb.h"
#include "mprpcchannel.h"
#include "mprpccontroller.h"

int main(int argc, char** argv)
{
    // 整个程序启动以后, 想使用mprpc框架来享受rpc服务调用, 一定需要先调用框架的初始化函数(只初始化一次)
    MprpcApplication::Init(argc, argv);

    // 演示调用远程发布的rpc方法Login
    fixbug::UserServiceRpc_Stub stub(new MprpcChannel());
    // rpc方法的请求参数
    fixbug::LoginRequest request;
    request.set_name("zhangsan");
    request.set_pwd("123456");
    // rpc方法的响应
    fixbug::LoginResponse response;
    // 发起rpc方法的调用 同步rpc调用进程 MprpcChannel::callmethod
    MprpcController controller;
    stub.Login(&controller, &request, &response, nullptr);
    // RpcChannel->RpcChannel::callMethod 集中来做所有rpc方法调用的参数序列化和网络发送
    //stub.Login(nullptr, &request, &response, nullptr);

    // 一次rpc调用完成, 读取调用的结果
    if (controller.Failed())
        std::cout << controller.ErrorText() << std::endl;
    else
    {
        if (response.result().errcode() == 0)
            std::cout << "rpc login response success: " << response.success() << std::endl;
        else
            std::cout << "rpc login response error: " << response.result().errmsg() << std::endl;
    }


    // ===========================register============================
    fixbug::RegisterRequest reg_request;

    reg_request.set_id(6666);
    reg_request.set_name("mprpc");
    reg_request.set_pwd("654321");

    fixbug::RegisterResponse reg_response;
    MprpcController controller;
    stub.Register(&controller, &reg_request, &reg_response, nullptr);
    //stub.Register(nullptr, &reg_request, &reg_response, nullptr);

    if (controller.Failed())
        std::cout << controller.ErrorText() << std::endl;
    else
    {
        if (reg_response.result().errcode() == 0)
            std::cout << "rpc register response success: " << reg_response.success() << std::endl;
        else
            std::cout << "rpc register response error: " << reg_response.result().errmsg() << std::endl;
    }

    return 0;
}