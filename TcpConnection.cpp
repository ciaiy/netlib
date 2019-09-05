#include "TcpConnection.h"
#include "CallBacks.h"

void TcpConnection::setConnectionStatus(int status){
    connectionStatus = status;
}

void TcpConnection::handleError() {
    perror("handle error :");
}

void TcpConnection::shutdown() {
    setConnectionStatus(KDisconnected);
    channel_.disableAll();
    connectionStatusCallBack_(shared_from_this());
    closingCallBack_(shared_from_this());
}

void TcpConnection::handleWrite()
{
    if (channel_.isReading())
    {
        int write_num = socket_.write(writeBuffer_.peek(), writeBuffer_.readableBytes());
        
        if (writeBuffer_.readableBytes() > 0)
        {
            writeBuffer_.hasRead(write_num);
            if (write_num == 0)
            {
                if (writeCompleteCallBack_)
                {

                    writeCompleteCallBack_(shared_from_this());
                }
                channel_.disableWrite();
            }
        }
    }
}

void TcpConnection::send(char *buf, int len)
{
    int write_num = 0;
    int remain_num = len;

    if (KDisconnected)
    {
        return;
    }

    if (!channel_.isWriting() && writeBuffer_.getReadIndex() == 0)
    {
        write_num = socket_.write(buf, len);

        if (write_num >= 0)
        {
            remain_num == len - write_num;

            if (remain_num == 0 && writeCompleteCallBack_)
            {
                loop_->addInPendingFunctors(std::bind(writeCompleteCallBack_, shared_from_this()));
            }
        }
        else
        {
            perror("send error");
            handleError();
        }
    }

    if (remain_num > 0)
    {
        writeBuffer_.append(buf + write_num, remain_num);
        if (!channel_.isWriting())
        {
            channel_.enableWrite();
        }
    }
}

void TcpConnection::handleRead()
{
    int read_num = readBuffer_.readFd(socket_.getSockfd());
    if (read_num > 0)
    {
        readCompleteCallBack_(shared_from_this());
    }
    else if (read_num < 0)
    {
        handleError();
    }
    else
    {
        handleClose();
    }
}

void TcpConnection::handleClose()
{
    setConnectionStatus(KDisconnected);
    channel_.disableAll();
    connectionStatusCallBack_(shared_from_this());
    closingCallBack_(shared_from_this());
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
    connectionStatusCallBack_(shared_from_this());
}