#pragma once

#include <string>

class ListeningSocket {
public:
    
    ListeningSocket(std::string local_addr, std::string port);

    int get_local_socket();

private:
    std::string local_addr;
    std::string port;
};
