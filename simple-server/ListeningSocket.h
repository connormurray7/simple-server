#pragma once

#include <string>

class ListeningSocket {
public:
    
    ListeningSocket() {}

    int get_local_socket(std::string& local_addr, std::string& port);

private:

};
