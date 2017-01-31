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

    int conn_sock, nfds, epollfd;

    epollfd = epoll_create1(0);
    if (epollfd == -1) {
       perror("epoll_create1");
       exit(EXIT_FAILURE);
    }

    ev.events = EPOLLIN;
    ev.data.fd = local_socket;
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, local_socket, &ev) == -1) {
       perror("epoll_ctl: listen_sock");
       exit(EXIT_FAILURE);
    }

    while(1) {
       nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
       if (nfds == -1) {
           perror("epoll_wait");
           exit(EXIT_FAILURE);
       }

       for (int event = 0; event < num_events; ++n) {
           handle_request(event);
       }
    }
}

void EPollPoller::handle_request(int event) {
   if (events[event].data.fd == local_socket) {
       add_connection(event);
   } else {
       //do_use_fd(events[n].data.fd);
   }
}

void EPollPoller::add_connection(int event) {
   socklen_t addrlen = sizeof(addr);
   conn_sock = accept(local_socket, (struct sockaddr *) &addr, &addrlen);
   if (conn_sock == -1) {
       perror("accept");
       exit(EXIT_FAILURE);
   }
   handle_request(conn_sock);
   ev.events = EPOLLIN | EPOLLET;
   ev.data.fd = conn_sock;
   if (epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock, &ev) == -1) {
       perror("epoll_ctl: conn_sock");
       exit(EXIT_FAILURE);
   }
}

#endif
