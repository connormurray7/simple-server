#include <sys/event.h>
#include <sys/time.h>
#include <unistd.h>
#include <netdb.h>
#include <string>
#include <iostream>

#include "Poller.h"

using std::string;
using std::cout;
using std::endl;

void conn_delete(int fd);
void send_msg(int fd, string s);
void recv_msg(int num);
int conn_add(int fd);

KQueuePoller::KQueuePoller() {}

void KQueuePoller::loop_forever(int local_socket) {
    listening_socket = local_socket;
    kq = kqueue();
    
    EV_SET(&evSetListening, local_socket, EVFILT_READ, EV_ADD, 0, 0, NULL);
    if (kevent(kq, &evSetListening, 1, NULL, 0, NULL) == -1) {
        //TODO log error
        cout << "There was an error setting this up" << endl;
    }

    int nev = 0;
    while(1) {
        cout << "Starting to handle requests" << endl;
        nev = kevent(kq, NULL, 0, evList, 32, NULL);
        if (nev < 1) {
            //TODO log error
        }
        for (int event = 0; event < nev; event++) {
            cout << "handling request" << endl;
            handle_request(event);
        }
    }
}

void KQueuePoller::handle_request(int event) {

    if (evList[event].flags & EV_EOF) {
        printf("disconnect\n");
        int fd = evList[event].ident;
        EV_SET(&evSet, fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
        if (kevent(kq, &evSet, 1, NULL, 0, NULL) == -1) {
            //TODO log error
        }
        conn_delete(fd);
    }
    else if (evList[event].ident == listening_socket) {
       socklen_t socklen = sizeof(addr);
        int fd = accept(evList[event].ident, (struct sockaddr *)&addr, &socklen);
        if (fd == -1) {
            //TODO log error
        }
        if (conn_add(fd) == 0) {
            EV_SET(&evSet, fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
            if (kevent(kq, &evSet, 1, NULL, 0, NULL) == -1) {
                //TODO log error
            }
            send_msg(fd, "welcome!\n");
        } else {
            printf("connection refused\n");
            close(fd);
        }
    }
    else if (evList[event].flags == EVFILT_READ) {
        recv_msg(evList[event].ident);
    }
}

void conn_delete(int fd) {
    //TODO fill
}

void send_msg(int fd, string s) {
    //TODO fill 
    cout << s << endl;
}
void recv_msg(int num) {
    //TODO fill
}

int conn_add(int fd) {
    //TODO
    return 0;
}

EPollPoller::EPollPoller() {}
void EPollPoller::loop_forever(int local_socket) {}
void EPollPoller::handle_request(int event) {}
