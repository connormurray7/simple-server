#pragma once

#include <string>

struct Response {
    
    Response() {}
    
    Response(int fd_in, std::string msg_in)
    : fd(fd_in), msg(msg_in) {}
    
    int fd;
    std::string msg;
};
