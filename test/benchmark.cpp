#include <iostream>
#include <unistd.h>
#include <stdlib.h>
using namespace std;
int main(int argc, char*argv[]) {
    pid_t pid;
    int num;
    cin >> num;
    for(int i = 0 ; i < num ; i++) {
        pid = fork();
        if(pid <= 0) {
            break;
        }
    }
    execl("./pp_client", "");
}