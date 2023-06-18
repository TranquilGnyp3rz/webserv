/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketServer.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heloufra <heloufra@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 11:56:13 by akhouya           #+#    #+#             */
/*   Updated: 2023/06/01 13:20:21 by akhouya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_SERVER_HPP
#define SOCKET_SERVER_HPP

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <fcntl.h>
#include <vector>
#include <sys/ioctl.h>
#include <errno.h>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
// create class simplesocket in namespace wbs

namespace WBS
{
    class SocketServer
    {
    private:
        sockaddr_in6 _address;
        int _sock;
        int _connection;
        int _port;
        int _domain;

    public:
        SocketServer();
        SocketServer(int domain, int service, int protocol, int port, u_long interface);
        void connect_to_network(int sock);
        void test_connection(int);
        struct sockaddr_in6 get_addr();
        int get_sock();
        int get_port();
        int get_connection();
        int set_non_blocking(int sock);
        //copy constructor
        SocketServer(const SocketServer &other);
        //copy assignment operator
        SocketServer &operator=(const SocketServer &other);
        
        ~SocketServer();
    };
}

#endif