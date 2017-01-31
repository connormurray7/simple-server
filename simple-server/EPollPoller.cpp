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
    socklen_t addrlen = sizeof(addr);

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

       for (int n = 0; n < nfds; ++n) {
           if (events[n].data.fd == local_socket) {
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
           } else {
               //do_use_fd(events[n].data.fd);
           }
       }
    }
}

void EPollPoller::handle_request(int event) {

}

#endif
