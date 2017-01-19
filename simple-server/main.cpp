#include "ListeningSocket.h"

#include "Poller.h"

int main() {
    
    std::string local_addr = "127.0.0.1";
    std::string port = "8080";
    
    ListeningSocket socket;
    int local_socket = socket.get_local_socket(local_addr, port);
    
    KQueuePoller poller;
    poller.loop_forever(local_socket);
    
    return 0;
}
