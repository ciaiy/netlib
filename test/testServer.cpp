#include "../Server.h"

using namespace std;

void readCompleteCallBack(const TcpConnectionPtr &con) {
    // cout << con->getReadBuffer()->begin()  << ":::::" << con->getReadBuffer()->getSize() << endl;
    if(con->getReadBuffer()->getReadIndex() != con->getReadBuffer()->getSize()) {
        printf("!=\n");
        exit(-1);
    }

    // char buf[1024]= "HTTP/1.1 200 OK\r\nServer:Apache Tomcat/5.0.12\r\nDate:Mon,6Oct2003 13:13:33 GMT\r\nContent-Type:text/html\r\nLast-Moified:Mon,6 Oct 2003 13:23:42 GMT\r\nContent-Length:23\r\n\r\n<body>hellohello</body>";
    // con->send(buf, strlen(buf));
    con->shutdown();
}

int main(void) {
    Server server(2333, "127.0.0.1");
    server.setReadCompleteCallBack(readCompleteCallBack);
    server.start();
}
