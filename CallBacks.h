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

#endif // CALLBACKS_H
