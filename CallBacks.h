#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <functional>
#include <iostream>
#include <stdio.h>
#include <memory>
#include "TcpConnection.h"
class TcpConnection;
typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
typedef std::function<void(const TcpConnectionPtr &)> ConnectionStatusCallBack;
typedef std::function<void(const TcpConnectionPtr &)> ClosingCallBack;
typedef std::function<void(const TcpConnectionPtr &)> ReadCompleteCallBack;
typedef std::function<void(const TcpConnectionPtr &)> WriteCompleteCallBack;

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

#endif // CALLBACKS_H
