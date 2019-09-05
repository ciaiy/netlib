#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include "Eventloop.h"
#include "Buffer.h"
#include "Socket.h"
#include "CallBacks.h"

#include <functional>
#include <iostream>
#include <memory>


class TcpConnection : public std::enable_shared_from_this<TcpConnection>
{
private:
    /* data */
    Eventloop *loop_;
    Socket socket_;
    Channel channel_;

    int connectionStatus;
    bool isReading_;
    bool isWriting_;

    Buffer readBuffer_;
    Buffer writeBuffer_;

    /* 连接状态改变回调函数 */
    ConnectionStatusCallBack connectionStatusCallBack_;
    /* 即将关闭TcpConnection回调函数 */
    ClosingCallBack closingCallBack_;
    /* 读完成回调函数 */
    ReadCompleteCallBack readCompleteCallBack_;
    /* 写完成回调函数 */
    WriteCompleteCallBack writeCompleteCallBack_;
    /* 错误回调函数 */
    errorCallBack errorCallBack_;

    void handleRead();
    void handleWrite();
    void handleClose();

    void shutdown();
    void send(char *buf, int len);

public:
    void setConnectionStatus(int status);
    void connectEstablished();
    void handleError();
    TcpConnection(Eventloop *loop, int sockfd);
    ~TcpConnection(){};

    int getConnectionStatus()
    {
        return connectionStatus;
    }

    int getSockfd()
    {
        return socket_.getSockfd();
    }

    void setReadCompleteCallBack(ReadCompleteCallBack cb)
    {
        std::bind(readCompleteCallBack_, cb);
    }

    void setWriteCompleteCallBack(WriteCompleteCallBack cb)
    {
        std::bind(writeCompleteCallBack_, cb);
    }

    void setClosingCallBack(ClosingCallBack cb)
    {
        std::bind(closingCallBack_, cb);
    }

    void setConnectionStatusCallBack(ConnectionStatusCallBack cb)
    {
        std::bind(connectionStatusCallBack_, cb);
    }

    void setErrorCallBack(errorCallBack cb)
    {
        std::bind(errorCallBack_, cb);
    }

    bool reading()
    {
        return isReading_;
    }

    bool writing()
    {
        return isWriting_;
    }
};

namespace
{
#define KDisconnected 0
#define KDisconnecting 1
#define KConnected 2
#define KConnecting 3
}; // namespace



#endif