#include <sys/event.h>
#include <sys/time.h>
#include <err.h>
#include <unistd.h>
#include <netdb.h>
#include <iostream>
#include <string>

#include "Poller.h"

void conn_delete(int fd);
void send_msg(int fd, std::string s);
void recv_msg(int num);
int conn_add(int fd);

KQueuePoller::KQueuePoller() {}

EPollPoller::EPollPoller() {}


void KQueuePoller::loop_forever(int local_socket) {
    struct kevent evSetListening;

    int kq = kqueue();
    
    EV_SET(&evSetListening, local_socket, EVFILT_READ, EV_ADD, 0, 0, NULL);
    if (kevent(kq, &evSetListening, 1, NULL, 0, NULL) == -1) {
        err(1, "kevent");
    }

    struct kevent evSet;
    struct kevent evList[32];
    int nev, i;
    struct sockaddr_storage addr;
    socklen_t socklen = sizeof(addr);
    int fd;
    
    while(1) {
        nev = kevent(kq, NULL, 0, evList, 32, NULL);
        if (nev < 1) {
            err(1, "kevent");
        }
        for (i=0; i<nev; i++) {
            if (evList[i].flags & EV_EOF) {
                printf("disconnect\n");
                fd = evList[i].ident;
                EV_SET(&evSet, fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
                if (kevent(kq, &evSet, 1, NULL, 0, NULL) == -1)
                    err(1, "kevent");
                conn_delete(fd);
            }
            else if (evList[i].ident == local_socket) {
                fd = accept(evList[i].ident, (struct sockaddr *)&addr, &socklen);
                if (fd == -1)
                    err(1, "accept");
                if (conn_add(fd) == 0) {
                    EV_SET(&evSet, fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
                    if (kevent(kq, &evSet, 1, NULL, 0, NULL) == -1)
                        err(1, "kevent");
                    send_msg(fd, "welcome!\n");
                } else {
                    printf("connection refused\n");
                    close(fd);
                }
            }
            else if (evList[i].flags == EVFILT_READ) {
                recv_msg(evList[i].ident);
            }
        }
    }
}

void EPollPoller::loop_forever(int local_socket) {}




void conn_delete(int fd) {
    //TODO fill
}

void send_msg(int fd, std::string s) {
    //TODO fill 
}
void recv_msg(int num) {
    //TODO fill
}

int conn_add(int fd) {
    //TODO
    return 0;
}
