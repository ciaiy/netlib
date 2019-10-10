#include "../Server.h"

void push(const TcpConnectionPtr &con) {
    
}

int main(void) {
    Server server(2333, "127.0.0.1");
    server.start();
}