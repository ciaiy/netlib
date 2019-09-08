#include "../Socket.h"
#include <string.h>
int main(void) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1) {
        perror("socket error");
    }
    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(2333);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    int ret_value = connect(sockfd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret_value == -1) {
        perror("connect error");
    }
    printf("sockfd = %d \n", sockfd);
    char msg[100] = "hello,world";
    int write_num = write(sockfd, msg, 100);  
    printf("write %d bytes\n", write_num);
    sleep(10);
    close(sockfd);
    return 0;
}