#include "TcpConnection.h"
#include "CallBacks.h"

void TcpConnection::setConnectionStatus(int status)
{
    connectionStatus = status;
}

void TcpConnection::handleError()
{
    perror("handle error :");
}

void TcpConnection::shutdown()
{
    // shutdown并不能关闭文件描述符， 
    socket_.Shutdown();
    handleClose();
}

void TcpConnection::handleWrite()
{

        int write_num = 0;
        while (writeBuffer_.writableBytes() > 0)
        {
            write_num = socket_.write(writeBuffer_.begin() + writeBuffer_.getWriteIndex(), writeBuffer_.writableBytes());
            writeBuffer_.hasWrite(write_num);
        }

        if(write_num == -1) {
            handleError();
        }

        if (writeBuffer_.writableBytes() == 0)
        {
            writeBuffer_.clearBuffer();
            if (writeCompleteCallBack_)
            {
                writeCompleteCallBack_(shared_from_this());
            }
            channel_.disableWrite();
        }

    // if(getConnectionStatus() == KDisconnecting && writeBuffer_.writableBytes() == 0) {
    //     handleClose();
    // }
}

void TcpConnection::send(char *buf, int len)
{
    int write_num = 0;
    int remain_num = len - writeBuffer_.getSize();
    
    if (remain_num > 0)
    {
        writeBuffer_.append(buf, remain_num);
    }
    
    if (!channel_.isWriting())
    {
        channel_.enableWrite();
    }
}

void TcpConnection::handleRead()
{
    if(getConnectionStatus() == KDisconnected) {
        // 放弃读取
        return ;
    }
    int read_num = readBuffer_.readFd(socket_.getSockfd());
    if (read_num > 0)
    {
        // printf("INFO, fd: %d, read_num = %d\n", getSockfd(), read_num);
        if (readCompleteCallBack_)
        {
            readCompleteCallBack_(shared_from_this());
        }
    }
    else if (read_num < 0)
    {
        handleError();
    }
    else
    {
        // printf("ERROR, fd:%d, read_num = 0\n", getSockfd());
        //loop_->addInPendingFunctors(bind(&TcpConnection::handleClose, this));
        handleClose();
    }
}

void TcpConnection::handleClose()
{
    // printf("WARN, fd:%d, handleClose\n", getSockfd());
   
        log("DEBUG", "TcpConnection", __LINE__, "channelfd:", channel_.getFd());
        setConnectionStatus(KDisconnected);
        channel_.disableAll();
        connectionStatusCallBack_(shared_from_this());
        closingCallBack_(shared_from_this());
        close(channel_.getFd());
}

void TcpConnection::renew()
{
    log("INFO", "TcpConnection", __LINE__, "renew", channel_.getFd(), "status", channel_.getStatus());
    connectionStatusCallBack_(shared_from_this());
    channel_.enableRead();
    socket_.setNonblock(true);
    setConnectionStatus(KConnected);
}

TcpConnection::TcpConnection(Eventloop *loop, int sockfd) : loop_(loop),
                                                            connectionStatus(KConnecting),
                                                            socket_(sockfd),
                                                            channel_(loop_, socket_.getSockfd())
{
    channel_.setCloseCallBack(
        std::bind(&TcpConnection::handleClose, this));
    channel_.setErrorCallBack(
        std::bind(&TcpConnection::handleError, this));
    channel_.setReadCallBack(
        std::bind(&TcpConnection::handleRead, this));
    channel_.setWriteCallBack(
        std::bind(&TcpConnection::handleWrite, this));
    socket_.setKeepAlive(true);
    socket_.setNonblock(true);
    connectEstablished();
}

void TcpConnection::connectEstablished()
{
    setConnectionStatus(KConnected);
    channel_.enableRead();
    if (connectionStatusCallBack_)
    {
        connectionStatusCallBack_(shared_from_this());
    }
    // printf("DEBUG, TcpConnection connectEstablished end\n");
}