#include "../src/Server.h"

using namespace std;

void readCompleteCallBack(const TcpConnectionPtr &con) {
    if(con->getReadBuffer()->getReadIndex() != con->getReadBuffer()->getSize()) {
        exit(-1);
    }

   con->shutdown();
}

int main(void) {
    Server server(2333, "127.0.0.1");
    server.setReadCompleteCallBack(readCompleteCallBack);
    server.start();
}
