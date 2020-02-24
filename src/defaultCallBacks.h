#ifndef _DEFAULT_CALLBACKS_
#include <memory>
#include <iostream>
#include <functional>

class TcpConnection; 
typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
void defalutConnectionStatusCallBack(const TcpConnectionPtr &con);
void defaultClosingCallBack(const TcpConnectionPtr &con);
void defaultReadCompleteCallBack(const TcpConnectionPtr &con);
void defaultWriteCompleteCallBack(const TcpConnectionPtr &con);
void defaultErrorCallBack();

#endif // !DEFAULT_CALLBACKS__