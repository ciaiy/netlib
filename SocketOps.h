#ifndef SOCKETOPS_H
#define SOCKETOPS_H

#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

namespace socketopt
{
void Listen(int sockfd, int num)
{
    int ret_value = listen(sockfd, num);
    if (ret_value == -1)
    {
        perror("listen error");
    }
}

void Bind(int sockfd, struct sockaddr *addr, socklen_t len)
{
    int ret_value = bind(sockfd, addr, len);
    if (ret_value == -1)
    {
        perror("binf error");
    }
}

int Accept(int sockfd, struct sockaddr *addr, socklen_t *len)
{
    int ret_value = accept(sockfd, addr, len);
    if (ret_value == -1)
    {
        perror("accept error");
    }
    return ret_value;
}

int Socket(int domin, int type, int protocol)
{
    int ret_value = socket(domin, type, protocol);
    if (ret_value == -1)
    {
        perror("socket error");
    }
    return ret_value;
}

void setNonblock(int sockfd_, bool set)
{
    int flag = fcntl(sockfd_, F_GETFL, 0);
    flag |= SOCK_NONBLOCK;
    if (set)
    {
        flag |= SOCK_NONBLOCK;
    }
    else
    {
        flag &= ~SOCK_NONBLOCK;
    }
    fcntl(sockfd_, F_SETFL, flag);
}

void closeKeepAlive(int sockfd) {
    int keepAlive = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, (void *)&keepAlive, sizeof(keepAlive));
}

void setKeepAlive(int sockfd, int keepIdle, int keepInterval, int keepCount) {
    int idle = keepIdle;
    int interval = keepInterval;
    int count = keepCount;
    int ret_value = 0;
    ret_value = setsockopt(sockfd, SOL_TCP, TCP_KEEPIDLE, (void *)&idle, sizeof(idle));
    if(ret_value == -1) {
        perror("set keep-alive idle error");
    }
    ret_value = setsockopt(sockfd, SOL_TCP, TCP_KEEPINTVL, (void *)&interval, sizeof(interval));
    if(ret_value == -1) {
        perror("set keep-alive interval error");
    }
    ret_value = setsockopt(sockfd, SOL_TCP, TCP_KEEPCNT, (void *)&count, sizeof(count));
    if(ret_value == -1) {
        perror("set keep-alive count error");
    }
}

};     // namespace socketopt
#endif // !SOCKETOPS_H
