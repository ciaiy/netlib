#include "Buffer.h"

char *Buffer::peek() {
    return begin() + readIndex_;
}

int Buffer::readableBytes() {
    return buffer_.size() - readIndex_;
}

void Buffer::hasRead(int n) {
    readIndex_ += n;
}

int Buffer::readFd(int fd)
{
    int writable_ = writAble();
    char extraBuf[65535] = {0};
    struct iovec vec[2];
    vec[0].iov_base = begin() + writeIndex_;
    vec[0].iov_len = writable_;
    vec[1].iov_base = extraBuf;
    vec[1].iov_len = 65535;
    int read_num = 0;
    int iovcnt = (writable_ < sizeof(extraBuf) ? 2 : 1);
    read_num = readv(fd, vec, iovcnt);

    if (read_num < 0)
    {

        perror("readfd error");
    }
    else if (read_num <= writable_)
    {
        writeIndex_ += read_num;
    }
    else
    {
        writeIndex_ = buffer_.size();
        append(extraBuf, read_num - writable_);
    }
}

char *Buffer::begin()
{
    return static_cast<char *>(&*buffer_.begin());
}

int Buffer::writAble()
{
    return buffer_.size() - writeIndex_;
}

void Buffer::append(char *extrabuf, int len)
{
    if (len > buffer_.size() - writeIndex_)
    {
        int old_size = buffer_.size();
        buffer_.resize(old_size + len);
    }
    std::copy(extrabuf, extrabuf + len, begin() + writeIndex_);
    writeIndex_ += len;
}

Buffer::Buffer(int initSize)
    : buffer_(initSize),
      readIndex_(0),
      writeIndex_(0)
{
}

Buffer::~Buffer()
{
}