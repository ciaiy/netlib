#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <stdio.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <cstring>


using namespace std;

int main(void) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    perror("socket");
    struct sockaddr_in addr;
    addr.sin_port = htons(2333);
    addr.sin_family = AF_INET;
    //inet_pton(AF_INET, "127.0.0.1", &(addr.sin_addr.s_addr));
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    bind(sockfd, (struct sockaddr*)&addr, sizeof(struct sockaddr_in));
    perror("bind");
    socklen_t len = sizeof(struct  sockaddr_in);
    int clientfd = accept(sockfd, (struct sockaddr*)&addr, &len);
    perror("accept");
    cout << "recv a new connect" << clientfd <<endl;
    char msg[1024] = {0};
    int recvnum = 0;
    recvnum = read(clientfd, msg, 11);
    cout << "recvnum" << recvnum << endl;
    recvnum = read(clientfd, msg, 1024);
    cout << "recvnum" << recvnum << endl;;
    sleep(1000);
    cout << "connect finished" << endl;

    return 0;
}