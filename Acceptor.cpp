#include "Acceptor.h"

void Acceptor::handleRead()
{   
    int newconfd = socket_.accept();
    if (newconfd != -1)
    {
        if (newConnectionCallBack_)
        {
            newConnectionCallBack_(newconfd);
        }
        else
        {
            close(newconfd);
        }
    }else {
        if(errno == EMFILE) {   // 无可用文件描述符， 优雅关闭
            close(idlefd);
            idlefd = socket_.accept();
            close(idlefd);
            idlefd = open("/dev/null", O_RDONLY);
        }
    }
}


Acceptor::Acceptor(Eventloop *loop, int port, string address)
    : loop_(loop),
      channel_(loop_, socket_.getSockfd()),
      socket_(port, address)
{
    log(DEBUG, "acceptor", __LINE__, "constructor start");
    idlefd = open("/dev/null", O_RDONLY);
    if(idlefd == -1) {
        perror("idlefd open error");
    }
    socket_.listen();
    channel_.setReadCallBack(std::bind(&Acceptor::handleRead, this));
    channel_.enableRead();
    log(DEBUG, "acceptor", __LINE__, "constructor end");
}

Acceptor::~Acceptor()
{
    channel_.disableAll();
    channel_.remove();
    close(idlefd);
}

void Acceptor::setNewConnectionCallBack(newConnectionCallBack cb)
{
    newConnectionCallBack_ = std::move(cb);
}
