#pragma once

#include <netdb.h>

///Poller listens to a local socket and offloads any 
///incoming request to a queue (a folly multi-producer, multi-
///consumser queue) so that each request can be dealt with
///with a threadpool or some other type of consumer.
class Poller {
public:

    ///Main driver for the server. Blocks forever
    ///unless interrupted. 
    virtual void loop_forever(int local_socket) = 0;

private:
    virtual void handle_request(int event) = 0;
};


#if defined(unix) || defined(__unix__) || defined(__unix)

///Linux implementation of a Poller.
///Uses epoll(7) to queue requests.
class EPollPoller : public Poller {
public:
    
    ///Empty Poller.
    EPollPoller();
    
    ///Monitors local_socket, blocks
    ///unless interrupted.
    void loop_forever(int local_socket);
    
private:
    
    void handle_request(int event);

    struct sockaddr_storage addr;
};


#else

#include <sys/event.h>

///FreeBSD implementation of a Poller.
///Uses kqueue to queue requests.
class KQueuePoller : public Poller {
public:

    ///Empty Poller.
    KQueuePoller();

    ///Monitors local_socket, blocks 
    ///unless interrupted.
    void loop_forever(int local_socket);

private:
    
    void handle_request(int event);
    void add_connection(int event);
    void close_connection(int event);

    int kq;
    int listening_socket;
    struct kevent event_set;
    struct kevent event_list[128];
    struct kevent event_set_listening;
    struct sockaddr_storage addr;
};

#endif
