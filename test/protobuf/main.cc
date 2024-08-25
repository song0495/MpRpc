#include <iostream>
#include <string>
#include "test.pb.h"

void test1()
{
    fixbug::LoginRequest l_req;
    l_req.set_name("zhangsan");
    l_req.set_pwd("123456");

    // 序列化
    std::string send_str;
    if (l_req.SerializeToString(&send_str))
        std::cout << send_str << std::endl;
    
    // 反序列化
    fixbug::LoginRequest rl_req;
    if (rl_req.ParseFromString(send_str))
    {
        std::cout << rl_req.name() << std::endl;
        std::cout << rl_req.pwd() << std::endl;
    }
}

void test2()
{
    fixbug::LoginResponse l_rsp;
    fixbug::Error* error = l_rsp.mutable_error();
    error->set_errcode(1);
    error->set_errmsg("timeout...");
}

void test3()
{
    fixbug::GetFriendListsResponse gf_rsp;
    fixbug::Error* error = gf_rsp.mutable_error();
    error->set_errcode(0);
    error->set_errmsg("");

    fixbug::User* user1 = gf_rsp.add_friendlist();
    user1->set_name("zhangsan");
    user1->set_age(22);
    user1->set_sex(fixbug::User::Man);

    fixbug::User* user2 = gf_rsp.add_friendlist();
    user2->set_name("lisi");
    user2->set_age(24);
    user2->set_sex(fixbug::User::Man);

    std::cout << gf_rsp.friendlist_size() << std::endl;
    std::cout << user1->name() << " " << user1->age() << " " << user1->sex() << std::endl;
    std::cout << user2->name() << " " << user2->age() << " " << user2->sex() << std::endl;
}

int main()
{
    //test1();
    //test2();
    test3();

    return 0;
}