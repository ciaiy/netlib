#include "../Server.h"
#include <functional>

void pingpong(const TcpConnectionPtr & con) {
    printf("pingpong:\n");
    Buffer *readBuf = con->getReadBuffer();
    printf("info : %s\n", readBuf->begin());
    int i = readBuf->readableBytes();
    printf("readIndex : %d\n", readBuf->getReadIndex());
    con->send(readBuf->begin(), readBuf->getReadIndex());
}

int main(void) {
    Server server(2333, "127.0.0.1");
    server.setReadCompleteCallBack(pingpong);
    server.start();
    return 0;
}   