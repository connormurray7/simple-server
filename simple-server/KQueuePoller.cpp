//Thanks to Eric Radman for hist post on Kqueue:
//http://eradman.com/posts/kqueue-tcp.html

#if defined(__APPLE__) || defined(__MACH__) || defined(__FreeBSD__)

#include "Poller.h"
#include "Request.h"

#include <string>
#include <iostream>
#include <netdb.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/event.h>
#include <folly/MPMCQueue.h>

using std::string;
using std::to_string;
using std::shared_ptr;
using std::cout;
using std::endl;
using std::runtime_error;
using folly::MPMCQueue;

int conn_add(int fd);

KQueuePoller::KQueuePoller(shared_ptr<MPMCQueue<Request>> queue)
    : Poller(queue) {}

void KQueuePoller::loop_forever(int local_socket) {
    listening_socket = local_socket;
    kq = kqueue();
    
    EV_SET(&event_set_listening, local_socket, EVFILT_READ, EV_ADD, 0, 0, NULL);
    if (kevent(kq, &event_set_listening, 1, NULL, 0, NULL) == -1) {
        throw runtime_error("Local socket reading error, fd: " + to_string(local_socket));
    }

    int nev = 0;
    while(1) {
        nev = kevent(kq, NULL, 0, event_list, 32, NULL);
        if (nev < 1) {
            throw runtime_error("kevent error");
        }
        for (int event = 0; event < nev; event++) {
            handle_request(event);
        }
    }
}

void KQueuePoller::handle_request(int event) {

    if (event_list[event].flags & EV_EOF) {
        close_connection(event);
    }
    else if (event_list[event].ident == listening_socket) {
        add_connection(event);
    }
    else if (event_list[event].flags == EVFILT_READ) {
        receive_request(event_list[event].ident);
    }
}

void KQueuePoller::add_connection(int event) {
    socklen_t addrlen = sizeof(addr);
    int fd = accept(event_list[event].ident, (struct sockaddr *)&addr, &addrlen);
    if (fd == -1) {
        throw runtime_error("Unable to accept new connections");
    }
    EV_SET(&event_set, fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
    if (kevent(kq, &event_set, 1, NULL, 0, NULL) == -1) {
        throw runtime_error("Unable to add new connections");
    }
    Request inbound = receive_request(fd);
    queue->blockingWrite(std::forward<Request>(inbound));
}

void KQueuePoller::close_connection(int event) {
    int fd = event_list[event].ident;
    EV_SET(&event_set, fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
    if (kevent(kq, &event_set, 1, NULL, 0, NULL) == -1) {
        throw runtime_error("Unable to close connection");
    }
    close(fd);
}

#endif
