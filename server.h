#ifndef SERVER_H
#define SERVER_H

class server
{
private:
    /* data */
    Acceptor accpetor_;
    Eventloop loop_;
public:
    server(/* args */);
    ~server();
};

server::server(/* args */)
{
}

server::~server()
{
}


#endif