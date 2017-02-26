#include <string>
#include <iostream>
#include <memory>
#include <thread>
#include <folly/MPMCQueue.h>

#include "EchoHandler.h"
#include "../simple-server/ListeningSocket.h"
#include "../simple-server/Poller.h"
#include "../simple-server/RequestHandler.h"
#include "../simple-server/Dequeuer.h"
#include "../simple-server/Request.h"
#include "../simple-server/Server.h"

using std::string;
using std::cout;
using std::endl;
using std::make_shared;
using folly::MPMCQueue;

int main() {
    
    Server<EchoHandler> server("127.0.0.1", "8080", 1);
    server.run();
    
    //ListeningSocket socket("127.0.0.1", "8080");
    //int local_socket = socket.get_socket_fd();

    //auto queue = make_shared<MPMCQueue<Request>>(1024);
    //auto handler = std::shared_ptr<RequestHandler>(new EchoHandler());
    //auto dequeuer = Dequeuer(queue, handler, 1);

    //auto t = std::thread(&Dequeuer::begin, &dequeuer);
    //
    //KQueuePoller poller(queue);
    //poller.loop_forever(local_socket);
   
    return 0;
}


