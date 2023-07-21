#include "SocketServer.hpp"

#include <arpa/inet.h>
SocketServer::SocketServer(int domain, int service, int protocol, int port, u_long interface) {
    _port = port;
    _domain = domain;

    _sock = socket(domain, service, protocol);
    test_connection(_sock);
    test_connection(set_non_blocking(_sock));
    connect_to_network(_sock);
    // test_connection(_connection);
}

void SocketServer::test_connection(int connection) {
    if (connection < 0) {
        std::cerr << "connection failed" << std::endl;
        exit(-1);
    }
}

int SocketServer::get_sock() {
    return _sock;
}

int SocketServer::get_connection() {
    return _connection;
}

struct sockaddr_in SocketServer::get_addr() {
    return _address;
}

int SocketServer::set_non_blocking(int sock) {
        int on, rc = 1;
        rc = setsockopt(sock, SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on));
        if (rc < 0) {
            perror("setsockopt() failed");
            close(sock);
            return -1;
        }
        rc = setsockopt(sock, SOL_SOCKET,  SO_NOSIGPIPE, (char *)&on, sizeof(on));
        if (rc < 0) {
            perror("setsockopt() failed");
            close(sock);
            return -1;
        }
        rc = fcntl(sock, F_SETFL, O_NONBLOCK);
        if (rc < 0) {
           perror("fcntl() failed");
           close(sock);
           return -1;
        }
    return 0;
}

void SocketServer::connect_to_network(int sock) {

    struct sockaddr_in addr;
   //bind socket to port
    int rc, on = 1;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family    = AF_INET;
    // memcpy(&addr.sin_addr, &inaddr_any, sizeof(inaddr_any));
    // addr.sin_addr.s_addr = inet_addr("10.11.8.10");
    inet_aton("10.11.8.10", &myaddr.sin_addr.s_addr);
    addr.sin_port  = htons(_port);
    rc = bind(sock, (struct sockaddr *)&addr, sizeof(addr));


    if (rc < 0) {
        std::cerr << "bind() failed" << rc << std::endl;
        exit(-1);
    }
    //listen to socket
    rc = listen(sock, 32767);
    if (rc < 0)
    {
       perror("listen() failed");
       exit(-1);
    }
    return ;
}

SocketServer::~SocketServer() {
    // close(_sock);
}

//copy constructor
SocketServer::SocketServer(const SocketServer &other) {
    *this = other;
}

//copy assignment operator
SocketServer &SocketServer::operator=(const SocketServer &other) {
    if (this != &other) {
        _address = other._address;
        _sock = other._sock;
        _connection = other._connection;
        _port = other._port;
        _domain = other._domain;
    }
    return *this;
}

// get port
int SocketServer::get_port() {
    return _port;
}