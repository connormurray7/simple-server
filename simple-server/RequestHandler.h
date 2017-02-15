#pragma once

#include <string>

struct Request {
    
    Request() {}
    
    Request(int fd_in, std::string request_in)
        : fd(fd_in), request(request_in) {}
    
    int fd;
    std::string request;
};

struct Response {
    
    Response() {}
    
    Response(int fd_in, std::string response_in)
    : fd(fd_in), response(response_in) {}
    
    int fd;
    std::string response;
};

class RequestHandler {
public:
    
    RequestHandler() {}
    
    ~RequestHandler() {}

    virtual Response handle(Request& request) = 0;

private:

};
