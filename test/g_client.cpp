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
    int num = 0;
    char msg[10];
    for(num = 0; num < 1000; num++) {
        bzero(msg, 10);
        sprintf(msg, "%d", num);
        int write_num = write(sockfd, msg, strlen(msg));  
        printf("pid : %d write %d bytes\n", getpid(), write_num);
//        sleep(1);
	char temp[64];
	int recv_num = read(sockfd, temp, 64);
	printf("recv %dbytes\n", recv_num);
    }
    close(sockfd);
    return 0;
}
