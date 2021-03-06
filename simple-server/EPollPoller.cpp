///Thanks to the man page of EPoll, this
///was relatively painfree to implement
///http://man7.org/linux/man-pages/man7/epoll.7.html
///also this blog for some minor things:
///https://banu.com/blog/2/how-to-use-epoll-a-complete-example-in-c/

#if defined(unix) || defined(__unix__) || defined(__unix)

#include <string>
#include <iostream>
#include <memory>
#include <sys/epoll.h>
#include <folly/MPMCQueue.h>

#include "Request.h"
#include "Poller.h"

using std::string;
using std::to_string;
using std::cout;
using std::endl;
using std::runtime_error;
using std::shared_ptr;
using folly::MPMCQueue;

EPollPoller::EPollPoller(shared_ptr<MPMCQueue<Request>> queue)
    : Poller(queue) {}

EPollPoller::~EPollPoller() {}

void EPollPoller::loop_forever(int local_socket) {
    listening_socket = local_socket;
    fd = epoll_create1(0);

    if (fd == -1) {
        throw runtime_error("Could not create epoll fd");
    }

    ev.events = EPOLLIN;
    ev.data.fd = listening_socket;
    if (epoll_ctl(fd, EPOLL_CTL_ADD, listening_socket, &ev) == -1) {
        throw runtime_error("Unable to listen for requests on local socket");
    }

    while(1) {
       int num_events = epoll_wait(fd, events, MAX_EVENTS, -1);
       if (num_events == -1) {
           throw runtime_error("Error waiting for incoming request");
       }

       for (int event = 0; event < num_events; ++event) {
           handle_request(event);
       }
    }
}

bool incomingError(struct epoll_event event) {
    auto ev = event.events;
    return (ev & EPOLLERR) || (ev & EPOLLHUP) || !(ev & EPOLLIN);
}

void EPollPoller::handle_request(int event) {
   if(incomingError(events[event])) {
       close_connection(event);
   } else if (events[event].data.fd == listening_socket) {
       add_connection(event);
   } else {
       receive_request(events[event].data.fd); 
   }
}

void EPollPoller::add_connection(int event) {
    socklen_t addrlen = sizeof(addr);
    int conn_sock = accept(listening_socket, (struct sockaddr *) &addr, &addrlen);
    if (conn_sock == -1) {
       throw runtime_error("Unable to accept new request");
    }

    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = conn_sock;
    if (epoll_ctl(fd, EPOLL_CTL_ADD, conn_sock, &ev) == -1) {
       throw runtime_error("Error accepting request with epoll");
    }

    Request inbound = receive_request(conn_sock);
    queue->blockingWrite(std::forward<Request>(inbound));
}

void EPollPoller::close_connection(int fd){
    close(fd);
}


#endif
