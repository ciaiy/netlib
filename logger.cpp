#include "logger.h"

void log(int type, char *file, int line, char *msg) {
    std::string s_type;
    if(type == DEBUG) {
        s_type = "DEBUG"; 
    }else if(type == INFO) {
        s_type = "INFO";
    }else if(type == WARNNING) {
        s_type = "WARNNING";
    }else if(type == ERROR) {
        s_type = "ERROR";
    }

    std::cout << "type : "<<s_type<<" - file : " << file << " - line : " << line << " - msg : " << msg << std::endl;
}