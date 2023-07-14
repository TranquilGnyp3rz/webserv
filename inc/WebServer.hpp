#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include <iostream>
#include <string>
#include <stdexcept>
#include <map>
#include <vector>
#include <algorithm>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "ResourceHandler.hpp"
#include "SocketServer.hpp"
#include "Client.hpp"
#include "Config.hpp"


class WebServer {
    private:
        Config _config;
        std::map<int, Client> _clients;
        std::vector<int> _ports;

        int select_socket(fd_set *working_set, int max_sd, int *rc, fd_set *response_set);
        int accept_socket(fd_set *working_set, int i, int *max_sd, int *new_sd, int *end_Webserver, fd_set *master_set, std::map<int, Client> &clients, int ports);
        void readable_sock(int i, fd_set *master_set, int *max_sd, std::map<int, Client> &clients);
        int find_socket(std::vector<SocketServer> sockets, int sock);
        int find_client(std::vector<Client> clients, int sock);
        void accepter(std::vector<SocketServer> &sockets, fd_set *master_set, int *max_sd);
        void handler(int i, fd_set *master_set, int *max_sd, fd_set *response_set);
        
    public:
        WebServer(std::string config_file);
        void run();
};

int find_sockets(std::vector<SocketServer> sockets, int sock);
#endif