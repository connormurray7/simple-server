#pragma once


class Poller {
public:
    Poller();

    virtual void loop_forever(int kq, int local_socket) = 0;
};

class KQueuePoller : public Poller {
public:
    void loop_forever(int local_socket);
};

class EPollPoller : public Poller {};
