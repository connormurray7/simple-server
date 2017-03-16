#pragma once

#include <netdb.h>

#include <string>
#include <memory>
#include <folly/MPMCQueue.h>

#include "RequestHandler.h"

#define MAX_EVENTS 1024
#define BUFFER_SIZE 4096

///Poller listens to a local socket and offloads any 
///incoming request to a queue (a folly multi-producer, multi-
///consumser queue) so that each request can be dealt with
///with a threadpool or some other type of consumer.
class Poller {
public:

    Poller(std::shared_ptr<folly::MPMCQueue<Request>> q);

    //Virtual destrutor, no cleanup necessary at Poller level
    virtual ~Poller() {};
    
    ///Default method to receive request. Returns request object.
    Request receive_request(int fd);    

    ///Main driver for the server. Blocks forever
    ///unless interrupted.
    virtual void loop_forever(int local_socket) = 0;
    
    ///Facebook's Folly Multi-Producer Multi-Consumer queue
    ///Allows the Poller to accept requests immediately
    //and place them on the queue for a threadpool to process.
    std::shared_ptr<folly::MPMCQueue<Request>> queue;

private:

    virtual void handle_request(int event) = 0;
    std::atomic<bool> running;
};

#if defined(unix) || defined(__unix__) || defined(__unix)

#include <sys/epoll.h>

///Linux implementation of a Poller.
///Uses epoll(7) to queue requests.
class EPollPoller : public Poller {
public:
    
    ///Empty Poller.
    EPollPoller(std::shared_ptr<folly::MPMCQueue<Request>> queue);

    //Nothing to cleanup, exists for factory creation.
    ~EPollerPoller();
    
    ///Monitors local_socket, blocks
    ///unless interrupted.
    void loop_forever(int local_socket);
    
private:
    
    void add_connection(int event);
    void handle_request(int event);
    void close_connection(int event);

    int fd;
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
    KQueuePoller(std::shared_ptr<folly::MPMCQueue<Request>> queue);

    //Nothing to cleanup, exists for factory creation.
    ~KQueuePoller();

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

Poller::Poller(std::shared_ptr<folly::MPMCQueue<Request>> q) {
    queue = q;
    running.exchange(true);
}

Request Poller::receive_request(int fd) {
    char buf[BUFFER_SIZE];
    recv(fd, buf, sizeof(buf), 0);
    Request req(fd, std::string(buf));
    return req;
}
