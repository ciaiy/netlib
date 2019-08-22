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