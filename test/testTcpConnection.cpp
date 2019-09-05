#include "../TcpConnection.h"

int main(void) {
    Socket socket_(2333, "127.0.0.1");
    Eventloop *loop_ = new Eventloop();
    int sockfd = socket_.getSockfd();
    printf("TcpConnection construct begin\n");
    TcpConnection conn(loop_, sockfd);
    return 0;
}