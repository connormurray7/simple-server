#pragma once

#include <netdb.h>
#include <string>
#include <folly/MPMCQueue.h>

#define MAX_EVENTS 128

///Poller listens to a local socket and offloads any 
///incoming request to a queue (a folly multi-producer, multi-
///consumser queue) so that each request can be dealt with
///with a threadpool or some other type of consumer.
class Poller {
public:

    Poller(folly::MPMCQueue<std::string>* q) {
        queue = q;
    }

    ///Main driver for the server. Blocks forever
    ///unless interrupted. 
    virtual void loop_forever(int local_socket) = 0;

private:

    virtual void handle_request(int event) = 0;
    
    folly::MPMCQueue<std::string>* queue;
};


#if defined(unix) || defined(__unix__) || defined(__unix)

#include <sys/epoll.h>

///Linux implementation of a Poller.
///Uses epoll(7) to queue requests.
class EPollPoller : public Poller {
public:
    
    ///Empty Poller.
    EPollPoller(folly::MPMCQueue<std::string>* queue);
    
    ///Monitors local_socket, blocks
    ///unless interrupted.
    void loop_forever(int local_socket);
    
private:
    
    void add_connection(int event);
    void handle_request(int event);
    void close_connection(int event);

    int epollfd;
    int listening_socket;
    struct epoll_event ev;
    struct epoll_event events[MAX_EVENTS];
    struct sockaddr_storage addr;
};


#else

#include <sys/event.h>

///FreeBSD implementation of a Poller.
///Uses kqueue to queue requests.
class KQueuePoller : public Poller {
public:

    ///Empty Poller.
    KQueuePoller(folly::MPMCQueue<std::string>* queue);

    ///Monitors local_socket, blocks 
    ///unless interrupted.
    void loop_forever(int local_socket);

private:
    
    void add_connection(int event);
    void handle_request(int event);
    void close_connection(int event);

    int kq;
    int listening_socket;
    struct kevent event_set;
    struct kevent event_list[MAX_EVENTS];
    struct kevent event_set_listening;
    struct sockaddr_storage addr;
};

#endif
