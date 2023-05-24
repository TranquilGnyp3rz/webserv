/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketServer.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhouya <akhouya@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 11:56:13 by akhouya           #+#    #+#             */
/*   Updated: 2023/05/21 11:51:16 by akhouya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef SOCKET_SERVER_HPP
#define SOCKET_SERVER_HPP
#include <iostream>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
//create class simplesocket in namespace wbs

namespace WBS {
    class SocketServer {
        private:
            sockaddr_in6 _address;
            int _sock;
            int _connection;
            int _port;
            int _domain;
        public:
            SocketServer(int domain, int service, int protocol, int port, u_long interface);
            void connect_to_network(int sock);
            void test_connection(int);
            struct sockaddr_in6 get_addr();
            int get_sock();
            int get_connection();
            int set_non_blocking(int sock);
            ~SocketServer();
    };
}


#endif