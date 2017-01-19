#pragma once


class Poller {
public:
    virtual void loop_forever(int local_socket) = 0;
};

class KQueuePoller : public Poller {
public:
    KQueuePoller();
    void loop_forever(int local_socket);
};

class EPollPoller : public Poller {};
