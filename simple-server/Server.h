#pragma once

#include <string>
#include <memory>
#include <thread>
#include <type_traits>
#include <folly/MPMCQueue.h>

#include "RequestHandler.h"
#include "ListeningSocket.h"
#include "Poller.h"
#include "Dequeuer.h"
#include "Request.h"

///Top level class of this project. Requires a type T, that extends the
///abstract class RequestHandler. EchoHandler is given as an example,
///other implementations could include parsing an HTTP message and 
///writing back another HTTP message based on your use-case.
template<typename T>
class Server {
static_assert(std::is_base_of<RequestHandler, T>::value, "T must inherit from RequestHandler");
public:

    ///Requires a local address and port. Listens to a socket 
    ///created at this address. Also takes an optional number of
    ///threads (default 1) that will be used in a threadpool to handle events.
    Server(std::string addr, std::string port, int num_threads)
        : address(addr), port(port), num_threads(num_threads) {}

    Server(std::string addr, std::string port)
        : address(addr), port(port), num_threads(1) {}

    ///Blocking call, this function will not return unless
    ///an exception is thrown when trying to listen to the 
    ///address and port given.
    void run();

private:
    std::string address;
    std::string port;
    int num_threads;
};

#if defined(unix) || defined(__unix__) || defined(__unix)

template<typename T>
void Server<T>::run() {
    ListeningSocket socket(address, port);
    int local_socket = socket.get_socket_fd();

    auto queue = std::make_shared<folly::MPMCQueue<Request>>(1024);
    auto handler = std::shared_ptr<T>(new T());
    Dequeuer dequeuer(queue, handler, num_threads);

    dequeuer.begin();
    
    EPollPoller poller(queue);
    poller.loop_forever(local_socket);
}

#else

template<typename T>
void Server<T>::run() {
    ListeningSocket socket(address, port);
    int local_socket = socket.get_socket_fd();

    auto queue = std::make_shared<folly::MPMCQueue<Request>>(1024);
    auto handler = std::shared_ptr<T>(new T());
    Dequeuer dequeuer(queue, handler, num_threads);

    dequeuer.begin();
    
    KQueuePoller poller(queue);
    poller.loop_forever(local_socket);
}

#endif

