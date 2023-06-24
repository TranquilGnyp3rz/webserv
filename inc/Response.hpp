#ifndef RESPONSE_HPP
#define RESPONSE_HPP
#include <iostream>
#include <map>
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
# include "request.hpp"
# include "marcos.hpp"

class Client;

class Response
{
    private:
        std::map<int, std::string> httpResponses;
        std::map<std::string, std::string> fileExtensions;
    public:
        Response();
        int send(int socket, Client &client, Config &config);
        int send_error(int socket, int status);
        int handleGet(int socket, std::string path);
        int handlePost(int socket, std::string path);
        int handleDelete(int socket, std::string path);
        int handle(Client &client);
        int is_request_well_formed(Client &client, request_t &request);
        std::string custom_error(const std::string& status);
        bool containsAnyChar(const std::string& str, const std::string& charSet);
};
#endif 