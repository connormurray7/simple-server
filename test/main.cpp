#include "../simple-server/ListeningSocket.h"
#include "../simple-server/Poller.h"

int main() {
    
    ListeningSocket socket("127.0.0.1", "8080");
    int local_socket = socket.get_local_socket();
    
    KQueuePoller poller;
    poller.loop_forever(local_socket);
    
    return 0;
}
