#include "../Socket.h"
#include <sys/epoll.h>

int main(void) {
    Socket socket_(2333, "127.0.0.1");
    socket_.listen(1000);
    int epollfd_ = epoll_create(1000);    
    struct epoll_event ev;
    ev.data.ptr = static_cast<void *>(&socket_);
    ev.events = EPOLLIN;
    epoll_ctl(epollfd_, EPOLL_CTL_ADD, socket_.getSockfd(), &ev);
    perror("epollctl_1");
    epoll_ctl(epollfd_, EPOLL_CTL_DEL, socket_.getSockfd(), &ev);
    perror("epollctl_2");
    epoll_ctl(epollfd_, EPOLL_CTL_DEL, socket_.getSockfd(), &ev);
    perror("epollctl_3");


}