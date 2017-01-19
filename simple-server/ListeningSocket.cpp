#include <sys/event.h>
#include <err.h>
#include <netdb.h>
#include <string>
#include <iostream>

#include "ListeningSocket.h"

int get_local_socket(std::string& local_addr, std::string& port) {
    struct addrinfo *addr;
    struct addrinfo hints;
    
    /* open a TCP socket */
    memset(&hints, 0, sizeof hints);
    hints.ai_family = PF_UNSPEC; /* any supported protocol */
    hints.ai_flags = AI_PASSIVE; /* result for bind() */
    hints.ai_socktype = SOCK_STREAM; /* TCP */
    int error = getaddrinfo ("127.0.0.1", "8080", &hints, &addr);
    if (error) {
        errx(1, "getaddrinfo failed: %s", gai_strerror(error));
    }
    
    int local_socket = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
    bind(local_socket, addr->ai_addr, addr->ai_addrlen);
    listen(local_socket, 5);

}
