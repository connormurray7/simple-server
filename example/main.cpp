#include "EchoHandler.h"
#include "../simple-server/Server.h"


int main() {
    
    Server<EchoHandler> server("127.0.0.1", "8080", 1);
    server.run();
    
    return 0;
}


