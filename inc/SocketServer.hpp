/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketServer.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heloufra <heloufra@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 11:56:13 by akhouya           #+#    #+#             */
/*   Updated: 2023/05/27 17:59:00 by heloufra         ###   ########.fr       */
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