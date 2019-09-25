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
    setConnectionStatus(KDisconnected);
    channel_.disableAll();
    connectionStatusCallBack_(shared_from_this());
    closingCallBack_(shared_from_this());
    close(this->getSockfd());
    loop_->removeChannel(&channel_);
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
        printf("INFO, fd: %d, read_num = %d\n", getSockfd(), read_num);
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
    if (channel_.getStatus() != KDELETED)
    {
        setConnectionStatus(KDisconnected);
        channel_.disableAll();
        connectionStatusCallBack_(shared_from_this());
        closingCallBack_(shared_from_this());
        loop_->removeChannel(&channel_);
        close(channel_.getFd());
        printf("WARN has remove fd:%d\n", getSockfd());
    }
}

void TcpConnection::renew() {
    setConnectionStatus(KConnecting);
    connectionStatusCallBack_(shared_from_this());
    channel_.setStatus(KNEW);
    channel_.enableRead();
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
    printf("DEBUG, TcpConnection connectEstablished end\n");
}