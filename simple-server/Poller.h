#pragma once

//#include <folly/MPMCQueue.h>

///Poller listens to a local socket and offloads any 
///incoming request to a queue (a folly multi-producer, multi-
///consumser queue) so that each request can be dealt with
///with a threadpool or some other type of consumer.
class Poller {
public:

    ///Main driver for the server. Blocks forever
    ///unless interrupted. 
    virtual void loop_forever(int local_socket) = 0;
};


///FreeBSD implementation of a Poller.
///Uses kqueue(2) to queue requests.
class KQueuePoller : public Poller {
public:

    ///Empty Poller.
    KQueuePoller();

    ///Monitors local_socket, blocks 
    ///unless interrupted.
    void loop_forever(int local_socket);
};

class EPollPoller : public Poller {
    ///Linux implementation of a Poller.
    ///Uses epoll(7) to queue requests.
public:

    ///Empty Poller.
    EPollPoller();

    ///Monitors local_socket, blocks 
    ///unless interrupted.
    void loop_forever(int local_socket);
};
