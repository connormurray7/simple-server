///Thanks to the man page of EPoll, this
///was relatively painfree to implement
///http://man7.org/linux/man-pages/man7/epoll.7.html

#if defined(unix) || defined(__unix__) || defined(__unix)

#include "Poller.h"

#include <string>
#include <iostream>
#include <sys/epoll.h>

using std::string;
using std::to_string;
using std::cout;
using std::endl;
using std::runtime_error;

EPollPoller::EPollPoller() {}

void EPollPoller::loop_forever(int local_socket) {
    listening_socket = local_socket;
    epollfd = epoll_create1(0);

    if (epollfd == -1) {
        throw runtime_error("Could not create epoll fd");
    }

    ev.events = EPOLLIN;
    ev.data.fd = listening_socket;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listening_socket, &ev) == -1) {
        throw runtime_error("Unable to listen for requests on local socket, ", listening_socket);
    }

    while(1) {
       int num_events = epoll_wait(epollfd, events, MAX_EVENTS, -1);
       if (num_events == -1) {
           throw runtime_error("Error waiting for incoming request");
       }

       for (int event = 0; event < num_events; ++event) {
           handle_request(event);
       }
    }
}

void EPollPoller::handle_request(int event) {
   if (events[event].data.fd == listening_socket) {
       add_connection(event);
   } else {
       //do_use_fd(events[n].data.fd);
   }
}

void EPollPoller::add_connection(int event) {
   socklen_t addrlen = sizeof(addr);
   int conn_sock = accept(listening_socket, (struct sockaddr *) &addr, &addrlen);
   if (conn_sock == -1) {
       throw runtime_error("Unable to accept new request");
   }
   handle_request(conn_sock);
   ev.events = EPOLLIN | EPOLLET;
   ev.data.fd = conn_sock;
   if (epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock, &ev) == -1) {
       throw runtime_error("Error accepting request with epoll");
   }
}

void EPollPoller::close_connection(int event){}

#endif
