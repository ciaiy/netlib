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

int main(int argc, const char** argv) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    addr.sin_port = htons(2333);
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &(addr.sin_addr.s_addr));
    int serverfd = connect(sockfd, (struct sockaddr*)&addr, sizeof(struct sockaddr_in));
    int temp;
    perror("connect");
    cout << "serverfd" << sockfd << endl;
    write(sockfd, "123123123123", strlen("123123123123"));
    write(sockfd, "123123123123", strlen("123123123123"));
    close(sockfd);
    return 0;
}