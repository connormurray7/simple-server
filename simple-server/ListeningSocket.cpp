#include <sys/event.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <err.h>
#include <netdb.h>
#include <string>
#include <iostream>

#include "ListeningSocket.h"

ListeningSocket::ListeningSocket(std::string local_addr, std::string port) {
    this->local_addr = local_addr;
    this->port = port;
}

int ListeningSocket::get_local_socket() {
    struct addrinfo *addr;
    struct addrinfo hints;
    setup_hints(hints);

    // open a TCP socket
    int local_socket = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
    bind(local_socket, addr->ai_addr, addr->ai_addrlen);
    listen(local_socket, 5);
    return local_socket;
}

void setup_hints(struct addrinfo& hints) {
    memset(&hints, 0, sizeof hints);
    hints.ai_family = PF_UNSPEC; //all protocols
    hints.ai_flags = AI_PASSIVE;
    hints.ai_socktype = SOCK_STREAM; //tcp
    int error = getaddrinfo(local_addr.c_str(), port.c_str(), &hints, &addr);
    if (error) {
        errx(1, "getaddrinfo failed: %s", gai_strerror(error));
    }
}
    

