/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heloufra <heloufra@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 01:51:17 by heloufra          #+#    #+#             */
/*   Updated: 2023/06/01 18:19:25 by heloufra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WebSERVER_HPP
# define WebSERVER_HPP
# include <iostream>
# include <string>
# include <stdexcept>
# include <unistd.h>
# include <sys/types.h>
# include <sys/time.h>
# include <fcntl.h>
# include <vector>
# include <sys/ioctl.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <errno.h>
# include <stdio.h>
# include <stdlib.h>
# include "SocketServer.hpp"
# include "Client.hpp"
#include <map>
#include <algorithm>
#include "Config_parsing.hpp"


class WebServer
{
    private:
        Config _config;
        std::map<int, Client> _clients;
        std::vector<int> _ports;
        
        /** Methods **/

        int responder(Client &client);
        
        /* Networking */
        int select_socket(fd_set *working_set, int max_sd, int *rc);
        int accept_socket(fd_set *working_set, int i, int *max_sd, int *new_sd, int *end_Webserver, fd_set *master_set);
        void readable_sock(int i, fd_set *master_set, int *max_sd, std::map<int, Client> &clients);
        int find_socket(std::vector<SocketServer> sockets, int sock);
        int find_client(std::vector<Client> clients, int sock);
        void accepter(std::vector<SocketServer> sockets, fd_set *master_set, int *max_sd);
        void handler(int i, fd_set *master_set, int *max_sd);
        /* Request */
        int well_formed_request(Client &client);
        
    public:
        WebServer(std::string config_file);
        ~WebServer();
        WebServer(WebServer const & src);
        WebServer & operator=(WebServer const & src);
        void run();
};

#endif