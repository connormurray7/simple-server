#pragma once

//#include <folly/MPMCQueue.h>

class Poller {
	//Poller listens to a local socket and offloads any 
	//incoming request to a queue (a folly multi-producer, multi-
	//consumser queue) so that each request can be dealt with
	//with a threadpool or some other type of consumer.
public:
    virtual void loop_forever(int local_socket) = 0;
        //Main driver for the server. Blocks forever
        //unless interrupted. 
};

class KQueuePoller : public Poller {
    //FreeBSD implementation of a Poller.
    //Uses kqueue(2) to queue requests.
public:
    KQueuePoller();
        //Empty Poller.

    void loop_forever(int local_socket);
        //Monitors local_socket, blocks 
        //unless interrupted.
};

class EPollPoller : public Poller {
    //Linux implementation of a Poller.
    //Uses epoll(7) to queue requests.
public:
    EPollPoller();
        //Empty Poller.

    void loop_forever(int local_socket);
        //Monitors local_socket, blocks 
        //unless interrupted.
};
