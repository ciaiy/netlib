#ifndef SOCKET_H
#define SOCKET_H

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <stdio.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include "logger.h"

using std::string;

class Socket
{
private:
    /* data */
    int port_;
    int sockfd_;
    string address;
    struct sockaddr_in myaddr_;
    socklen_t addrlen_;

public:
    Socket(int port, string address);
    Socket(int sockfd);

    int accept();
    void listen(int num = 1000);
    void setNonblock();
    void setNonblock(bool set = true);
    void setKeepAlive(bool set);
    void setReuseAddr(bool set = true);
    string toString();
    int write(char *buf, int len);
    void Shutdown();

    int getSockfd();

    ~Socket();
};

#endif // !SOCKET_H
