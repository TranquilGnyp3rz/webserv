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


class Response
{
    private:
        std::map<int, std::string> httpResponses;
        std::map<std::string, std::string> fileExtensions;
    public:
        Response();
        int send(int socket);
        std::string custom_error(const std::string& status);
};
#endif 