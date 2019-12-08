#include "Buffer.h"

void Buffer::clearBuffer() {
    buffer_.clear();
    Index_ = 0;
    Index_ = 0;
}

char *Buffer::peek() {
    return begin() + Index_;
}

int Buffer::readableBytes() {
    return buffer_.size() - Index_;
}

void Buffer::hasRead(int n) {
    Index_ += n;
}

int Buffer::readFd(int fd)
{
    buffer_.clear();
    Index_ = 0;
    char extraBuf[65535] = {0};
    struct iovec vec[2];
    vec[0].iov_base = begin() + Index_;
    vec[0].iov_len = readableBytes();
    vec[1].iov_base = extraBuf;
    vec[1].iov_len = 65535;
    int read_num = 0;
    int iovcnt = (readableBytes() < sizeof(extraBuf) ? 2 : 1);
    read_num = readv(fd, vec, iovcnt);
    if (read_num < 0)
    {

        perror("readfd error");
    }
    else if (read_num <= readableBytes())
    {
        Index_ = read_num;
    }
    else
    {
        Index_ = buffer_.size();
        int append_num = read_num - readableBytes();
        append(extraBuf, append_num);
        hasRead(append_num);
    }
    return read_num;
}

void Buffer::hasWrite(int num) {
    Index_ += num;
}

char *Buffer::begin()
{
    return static_cast<char *>(&*buffer_.begin());
}

int Buffer::writableBytes()
{
    return buffer_.size() - Index_;
}

void Buffer::append(char *extrabuf, int len)
{
    if (len > buffer_.size() - Index_)
    {
        int old_size = buffer_.size();
        buffer_.resize(old_size + len);
    }
    std::copy(extrabuf, extrabuf + len, begin() + Index_);
}

Buffer::Buffer():
      Index_(0)
{
}

Buffer::~Buffer()
{
}