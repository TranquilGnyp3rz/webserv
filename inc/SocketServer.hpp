#ifndef SOCKETSERVER_HPP
#define SOCKETSERVER_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <fcntl.h>
#include <vector>
#include <sys/ioctl.h>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

class SocketServer {
    private:
        sockaddr_in _address;
        int _sock;
        int _connection;
        int _port;
        int _domain;
    public:
        SocketServer();
        SocketServer(int domain, int service, int protocol, int port, u_long interface);
        void connect_to_network(int &sock);
        void test_connection(int);
        struct sockaddr_in get_addr();
        int get_sock();
        int get_port();
        int get_connection();
        int set_non_blocking(int &sock);
        //copy constructor
        SocketServer(const SocketServer &other);
        //copy assignment operator
        SocketServer &operator=(const SocketServer &other);
        
        ~SocketServer();
};

#endif