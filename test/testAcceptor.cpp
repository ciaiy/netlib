#include "../Eventloop.h"
#include "../Acceptor.h"
#define BUG
int main() {
    Eventloop loop_;
    Acceptor acceptor_(&loop_, 2333, "127.0.0.1");
    loop_.loop();
    return 0;
}