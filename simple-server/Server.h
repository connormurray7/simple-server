#pragma once

#include <string>
#include <memory>
#include <thread>
#include <folly/MPMCQueue.h>

#include "ListeningSocket.h"
#include "Poller.h"
#include "Dequeuer.h"
#include "Request.h"

template<typename T>
class Server {
public:

    Server(std::string addr, std::string port, int num_threads)
        : address(addr), port(port), num_threads(num_threads) {}

    void run();

private:
    std::string address;
    std::string port;
    int num_threads;
};


template<typename T>
void Server<T>::run() {
    ListeningSocket socket(address, port);
    int local_socket = socket.get_socket_fd();

    auto queue = std::make_shared<folly::MPMCQueue<Request>>(1024);
    auto handler = std::shared_ptr<T>(new T());
    auto dequeuer = Dequeuer(queue, handler, num_threads);

    auto t = std::thread(&Dequeuer::begin, &dequeuer);
    
    KQueuePoller poller(queue);
    poller.loop_forever(local_socket);
}
