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
    int Index_;




public:
    Buffer();
    char *begin();
    
    /* 读了n个字节 */
    void hasRead(int n);
    
    /* 写了n个字符 */
    void hasWrite(int n);

    /* 从fd中读取数据 */
    int readFd(int fd);

    /* 尾加数据函数， 若空间不够， 则扩容 */
    void append(char *extrabuf, int len);

    /* 得到数据尾部地址 */
    char *peek();

    /* 还可写的字节数 */  
    int writableBytes();

    /* 还可读的字节数 */
    int readableBytes();

    /* 当前的读下标 */
    int getReadIndex()
    {
        return Index_;
    }

    /* 当前的写下标 */
    int getWriteIndex() {
        return Index_;
    }

    /* 得到buffer的大小 */
    int getSize() {
        return buffer_.size();
    }

    /* 清空缓冲区 */
    void clearBuffer();
    
    ~Buffer();
};



#endif // !BUFFER_H
