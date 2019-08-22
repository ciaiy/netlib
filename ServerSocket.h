#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <stdio.h>
#include <arpa/inet.h>
#include <fcntl.h>

using std::string;

class ServerSocket
{
private:
    /* data */
    int port_;
    int sockfd_;
    string address;
    struct sockaddr_in myaddr_;
    socklen_t addrlen_;
public:
    ServerSocket(int port, string address);
    void setNonblock();
    void listen(int num = 1000);  
    int accept();  
    void setNonblock(bool set = true);
    void setKeepAlive(bool set);
    void setKeepAlive(int keepIdle, int keepInterval, int keepCount);
    int getsockfd();
    ~ServerSocket();
};

ServerSocket::ServerSocket(int port, string address) : port_(port) , address(address)
{
    addrlen_ = sizeof(myaddr_);
    int ret_value = inet_pton(AF_INET, address.c_str(), &(myaddr_.sin_addr.s_addr));
    if(ret_value == -1) {
        perror("inet_pton error");
        delete(this);
    }
   
    sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd_ == -1) {
        perror("socket error");
        delete(this);
    }
    
    ret_value = bind(sockfd_, (struct sockaddr*)&myaddr_, addrlen_);
    if(ret_value == -1) {
        perror("bind error");
        delete(this);
    } 
}

ServerSocket::~ServerSocket()
{
    close(sockfd_);
}


#endif // !SERVERSOCKET_H
