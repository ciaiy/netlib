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
    char msg[2048] = "hello,world";
    char recv_msg[2480];
    while(1) {
        int write_num = write(sockfd, msg, 2048);  
        printf("pid : %d write %d bytes\n", getpid(), write_num);
        int recv_num = read(sockfd, recv_msg, 2048);
        printf("pid : %d recv:%d\n", getpid(), recv_num);

        sleep(1);
    }
    close(sockfd);
    return 0;
}
