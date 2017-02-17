#pragma once

#include <string>

struct Request {
    
    Request() {}
    
    Request(int fd_in, std::string msg_in)
        : fd(fd_in), msg(msg_in) {}
    
    int fd;
    std::string msg;
};

struct Response {
    
    Response() {}
    
    Response(int fd_in, std::string msg_in)
    : fd(fd_in), msg(msg_in) {}
    
    int fd;
    std::string msg;
};

class RequestHandler {
public:
    
    RequestHandler() {}
    
    ~RequestHandler() {}

    virtual Response handle(Request& request) = 0;

private:

};
