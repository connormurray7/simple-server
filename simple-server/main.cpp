#include <sys/event.h>
#include <err.h>
#include <netdb.h>
#include <string>
#include <iostream>

#include "Poller.h"

int main() {
    
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
    
    int kq;
    struct kevent evSet;
    
    kq = kqueue();
    
    EV_SET(&evSet, local_socket, EVFILT_READ, EV_ADD, 0, 0, NULL);
    if (kevent(kq, &evSet, 1, NULL, 0, NULL) == -1) {
        err(1, "kevent");
    }
    KQueuePoller poller;
    poller.loop_forever(kq, local_socket);
    
    return 0;
}
