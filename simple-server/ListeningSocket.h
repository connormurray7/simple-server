#pragma once

#include <string>

///Handles the opening and listening of
///the local socket where every request
///passes through.
class ListeningSocket {
public:
    
    ///Socket requires a local addr, for example
    ///"127.0.0.1". And a port, for example "8080".
    ListeningSocket(std::string local_addr, std::string port);

    ///Opens and returns the descriptor for the socket that is being
    ///listened to. If an error occurs, the descriptor will be -1.
    int get_local_socket();

private:
    
    std::string local_addr;
    std::string port;
};
