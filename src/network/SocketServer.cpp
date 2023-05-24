/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhouya <akhouya@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 11:56:18 by akhouya           #+#    #+#             */
/*   Updated: 2023/05/21 12:59:03 by akhouya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/SocketServer.hpp"


WBS::SocketServer::SocketServer(int domain, int service, int protocol, int port, u_long interface) {
    _port = port;
    _domain = domain;

    _sock = socket(domain, service, protocol);
    test_connection(_sock);
    test_connection(set_non_blocking(_sock));
    connect_to_network(_sock);
    // test_connection(_connection);
}

void WBS::SocketServer::test_connection(int connection) {
    if (connection < 0) {
        std::cerr << "connection failed" << std::endl;
        exit(-1);
    }
}

int WBS::SocketServer::get_sock() {
    return _sock;
}

int WBS::SocketServer::get_connection() {
    return _connection;
}

struct sockaddr_in6 WBS::SocketServer::get_addr() {
    return _address;
}

int WBS::SocketServer::set_non_blocking(int sock) {
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

void WBS::SocketServer::connect_to_network(int sock) {

    struct sockaddr_in6 addr;
   //bind socket to port
    int rc, on = 1;
    memset(&addr, 0, sizeof(addr));
    addr.sin6_family    = AF_INET6;
    memcpy(&addr.sin6_addr, &in6addr_any, sizeof(in6addr_any));
    addr.sin6_port  = htons(_port);
    rc = bind(sock, (struct sockaddr *)&addr, sizeof(addr));


    if (rc < 0) {
        std::cerr << "bind() failed" << rc << std::endl;
        exit(-1);
    }
    //listen to socket
    rc = listen(sock, 10);
    if (rc < 0)
    {
       perror("listen() failed");
       exit(-1);
    }
    return ;
}

WBS::SocketServer::~SocketServer() {
    // close(_sock);
}