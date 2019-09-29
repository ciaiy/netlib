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

public:

    void shutdown();
    void send(char *buf, int len);
    void setConnectionStatus(int status);
    void connectEstablished();
    void handleError();
    /* 加重新使用该fd的Channel加入到poller里 */
    void renew();
    Buffer *getReadBuffer() {
        return &readBuffer_;
    }
    Buffer *getWriteBuffer() {
        return &writeBuffer_;
    }
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
        readCompleteCallBack_ = std::move(cb);
    }

    void setWriteCompleteCallBack(WriteCompleteCallBack cb)
    {
        writeCompleteCallBack_ = std::move(cb);
    }

    void setClosingCallBack(ClosingCallBack cb)
    {
        closingCallBack_ = std::move(cb);
    }

    void setConnectionStatusCallBack(ConnectionStatusCallBack cb)
    {
        connectionStatusCallBack_ = std::move(cb);
    }

    void setErrorCallBack(errorCallBack cb)
    {
        errorCallBack_ = std::move(cb);
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