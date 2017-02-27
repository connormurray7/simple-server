### _Introduction_

The famous [C10K Problem](http://www.kegel.com/c10k.html#nb) was the inspiration for this project. Similar to [libevent](http://libevent.org), this hooks into **epoll** on Linux based systems, and **kqueue** on FreeBSB  based systems (including macOS).

  
The goal of this project is to create a simple and approachable asynchronous rest web server. Since this is a learning experience for me, I make no commitments for performance.

### _Dependencies_

This project requires [Facebook's Folly](https://github.com/facebook/folly). Instructions to install on a variety of different operating systems are on that page.

### _How to Use_

There are two main classes that users of this project should be aware of, `Server` and `RequestHandler`. `Server` is of type T that must implement the `RequestHandler` abstract class, if you forget to extend it it will throw a static assert error at compile-time.

For example, a class that just echos back what comes in

`EchoHandler.h`

	#include <string>
	#include "RequestHandler.h"

	class EchoHandler : public RequestHandler {
	public:

	    EchoHandler(){}
	
	    Response handle(Request& request) {
	        Response response(request.fd, request.msg);
	        return response;
	    }
	};

Then the main would look like

	#include "EchoHandler.h"
	#include "Server.h"
	
	
	int main() {
	    
	    Server<EchoHandler> server("127.0.0.1", "8080", 1);
	    server.run();
	    
	    return 0;
	}

Where a `Server` takes a local address, a port, and the number of threads that you want to handle the messages. And where `run()` is a blocking call that will never return. 

All of this is contained in the `example/` directory.