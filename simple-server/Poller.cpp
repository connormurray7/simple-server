//Thanks to Eric Radman for hist post on Kqueue:
//http://eradman.com/posts/kqueue-tcp.html

#include <sys/event.h>
#include <sys/time.h>
#include <unistd.h>
#include <netdb.h>
#include <string>
#include <iostream>
#include <stdexcept>

#include "Poller.h"

using std::string;
using std::to_string;
using std::cout;
using std::endl;
using std::runtime_error;

void conn_delete(int fd);
string receive_request(int num);
void send_response(int s, string msg, ...);
int conn_add(int fd);

KQueuePoller::KQueuePoller() {}

void KQueuePoller::loop_forever(int local_socket) {
    listening_socket = local_socket;
    kq = kqueue();
    
    EV_SET(&event_set_listening, local_socket, EVFILT_READ, EV_ADD, 0, 0, NULL);
    if (kevent(kq, &event_set_listening, 1, NULL, 0, NULL) == -1) {
        throw runtime_error("Local socket reading error, fd: " + to_string(local_socket));
    }

    int nev = 0;
    while(1) {
        nev = kevent(kq, NULL, 0, event_list, 32, NULL);
        if (nev < 1) {
            throw runtime_error("kevent error");
        }
        for (int event = 0; event < nev; event++) {
            handle_request(event);
        }
    }
}

void KQueuePoller::handle_request(int event) {

    if (event_list[event].flags & EV_EOF) {
        close_connection(event);
    }
    else if (event_list[event].ident == listening_socket) {
        add_connection(event);
    }
    else if (event_list[event].flags == EVFILT_READ) {
        cout << "here 3" << endl;
        receive_request(event_list[event].ident);
    }
}

void KQueuePoller::add_connection(int event) {
    socklen_t socklen = sizeof(addr);
    int fd = accept(event_list[event].ident, (struct sockaddr *)&addr, &socklen);
    if (fd == -1) {
        throw runtime_error("Unable to accept new connections");
    }
    if (conn_add(fd) == 0) {
        EV_SET(&event_set, fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
        if (kevent(kq, &event_set, 1, NULL, 0, NULL) == -1) {
            throw runtime_error("Unable to add new connections");
        }
        string inbound = receive_request(fd);
        send_response(fd, inbound);
    } else {
        cout << "Refusing connection" << endl;
        close(fd);
    }
}

void KQueuePoller::close_connection(int event) {
    cout << "disconnect" << endl;
    int fd = event_list[event].ident;
    EV_SET(&event_set, fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
    if (kevent(kq, &event_set, 1, NULL, 0, NULL) == -1) {
        throw runtime_error("Unable to close connection");
    }
    conn_delete(fd);
}

void send_response(int s, string msg, ...) {
    int len = msg.size() + 1;
    char buf[len];

    msg.copy(buf, len);
    send(s, buf, len, 0);
}


string receive_request(int num) {
    char buf[1024];
    
    recv(num, buf, sizeof(buf), 0);
    cout << buf << endl;
    return string(buf);
}

int conn_add(int fd) {
    //TODO
    return 0;
}

void conn_delete(int fd) {
    //TODO fill
}


EPollPoller::EPollPoller() {}
void EPollPoller::loop_forever(int local_socket) {}
void EPollPoller::handle_request(int event) {}
