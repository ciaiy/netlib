#include "CallBacks.h"

void defalutConnectionStatusCallBack(const TcpConnectionPtr &con)
{
    std::cout << "connection status changed : " << std::endl;
}

void defaultClosingCallBack(const TcpConnectionPtr &con)
{
    std::cout << "connection closing" << std::endl;
}

void defaultReadCompleteCallBack(const TcpConnectionPtr &con)
{
    std::cout << "connection read finished" << std::endl;
}

void defaultWriteCompleteCallBack(const TcpConnectionPtr &con)
{
    std::cout << "connection write finished" << std::endl;
}

void defaultErrorCallBack()
{
    perror("connection error : ");
}
