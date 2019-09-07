/* 以此做为服务器日志记录系统 */
/* 仅用于debug与输出一些必要信息 */

#include <iostream>
#include <cstring>

namespace {
    #define DEBUG 0
    #define INFO 1
    #define WARNNING 2
    #define ERROR 3
}

void log(int type, char *file, int line, char *msg);