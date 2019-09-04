#ifndef BUFFER_H
#define BUFFER_H

#include <vector>
#include <sys/uio.h>
#include <stdio.h>

class Buffer
{
private:
    /* data */
    std::vector<char> buffer_;
    int readIndex_;
    int writeIndex_;


    int writAble();
    char *begin();

public:
    Buffer(int initSize = 1024 * 4);
    
    /* 从fd中读取数据 */
    int readFd(int fd);

    /* 尾加数据函数， 若空间不够， 则扩容 */
    void append(char *extrabuf, int len);

    char *peek();

    void hasRead(int n);

    int readableBytes();

    int getReadIndex()
    {
        return readIndex_;
    }

    int getSize() {
        return buffer_.size();
    }

    ~Buffer();
};



#endif // !BUFFER_H
