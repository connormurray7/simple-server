#include <sys/event.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string>

#include "ListeningSocket.h"

using std::string;

ListeningSocket::ListeningSocket(string local_addr, string port) {
    this->local_addr = local_addr;
    this->port = port;
}

int ListeningSocket::get_local_socket() {
    struct addrinfo* addr;
    struct addrinfo hints;
    
    memset(&hints, 0, sizeof hints);
    hints.ai_family = PF_UNSPEC; //all protocols
    hints.ai_flags = AI_PASSIVE;
    hints.ai_socktype = SOCK_STREAM; //tcp
    int error = getaddrinfo(local_addr.c_str(), port.c_str(), &hints, &addr);
    if (error) {
        //TODO log error
    }

    // open a TCP socket
    int local_socket = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
    bind(local_socket, addr->ai_addr, addr->ai_addrlen);
    listen(local_socket, 5);
    return local_socket;
}
