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
    // printf("shutdown\n");
    setConnectionStatus(KDisconnecting);

    if(!channel_.isWriting()) {
        handleClose();
    }
    // channel_.disableAll();
    // connectionStatusCallBack_(shared_from_this());
    // closingCallBack_(shared_from_this());
    // close(this->getSockfd());
    // loop_->removeChannel(&channel_);
}

void TcpConnection::handleWrite()
{
    // printf("INFO, handleWrite writable bytes : %d\n", writeBuffer_.writableBytes());
    // perror("--write test begin");

    if (channel_.isWriting())
    {
        int write_num = 0;
        while (writeBuffer_.writableBytes() > 0)
        {
            write_num = socket_.write(writeBuffer_.begin() + writeBuffer_.getWriteIndex(), writeBuffer_.writableBytes());
            writeBuffer_.hasWrite(write_num);
            // printf("** has write : %d\n", write_num);
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

    }
    // printf("getstatus = %d   writableBytes = %d\n", channel_.getStatus(), writeBuffer_.writableBytes());
    if(getConnectionStatus() == KDisconnecting && writeBuffer_.writableBytes() == 0) {
        handleClose();
    }
}

void TcpConnection::send(char *buf, int len)
{
    int write_num = 0;
    int remain_num = len - writeBuffer_.getSize();
    // printf("sendmsg : %s, len :%d\n", buf, len);
    // if (KDisconnected)
    // {
    //     return;
    // }

    // if (!channel_.isWriting() && writeBuffer_.getWriteIndex() == 0)
    // {
    //     printf("ready to write!\n buf = %s, len = %d\n", buf, len);
    //     char temp[100] = "test_info";
    //     write_num = socket_.write(temp, 100);
    //     if (write_num >= 0)
    //     {
    //         remain_num == len - write_num;

    //         if (remain_num == 0 && writeCompleteCallBack_)
    //         {
    //             loop_->addInPendingFunctors(std::bind(writeCompleteCallBack_, shared_from_this()));
    //         }
    //     }
    //     else
    //     {
    //         perror("send error");
    //         handleError();
    //     }
    // }
    if (remain_num > 0)
    {
        writeBuffer_.append(buf, remain_num);
    }
    printf("writeBuffer_.size:%d writeBuffer_.getIndex:%d\n", writeBuffer_.getSize(), writeBuffer_.getWriteIndex());
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
        printf("ERROR, fd:%d, read_num = 0\n", getSockfd());
        loop_->addInPendingFunctors(bind(&TcpConnection::handleClose, this));
    }
}

void TcpConnection::handleClose()
{
    printf("WARN, fd:%d, handleClose\n", getSockfd());
    if (getConnectionStatus() != KDisconnected)
    {
        setConnectionStatus(KDisconnected);
        channel_.disableAll();
        connectionStatusCallBack_(shared_from_this());
        closingCallBack_(shared_from_this());
        loop_->removeChannel(&channel_);
        close(channel_.getFd());
        channel_.setStatus(KDELETED);
        // printf("WARN has remove fd:%d\n", getSockfd());
    }
}

void TcpConnection::renew()
{
    connectionStatusCallBack_(shared_from_this());
    channel_.setStatus(KNEW);
    channel_.enableRead();
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