#include "../Server.h"

using namespace std;

void readCompleteCallBack(const TcpConnectionPtr &con) {
    cout << con->getReadBuffer()->begin()  << ":::::" << con->getReadBuffer()->getSize() << endl;
    if(con->getReadBuffer()->getReadIndex() != con->getReadBuffer()->getSize()) {
        printf("!=\n");
        exit(-1);
    }
}

int main(void) {
    Server server(2333, "127.0.0.1");
    server.setReadCompleteCallBack(readCompleteCallBack);
    server.start();
}