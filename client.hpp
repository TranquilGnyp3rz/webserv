#ifndef REQUEST_HPP
#define REQUEST_HPP
#include<map>

#include <iostream>
#include <sys/socket.h>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "requestt.hpp"
// typedef struct request_s {
//     std::string method;
//     std::string path;
//     std::string http_version;
//     std::string host;
//     std::map<std::string, std::string>  headers;
//     std::string body;

// }   request_t;
class Client {
private:
    request_t request;
    std::string response;
    int socket_fd;

public:
    Client(int new_socket_fd) : socket_fd(new_socket_fd) {}

    void set_request(const request_t& new_request) {
        request = new_request;
    }

    request_t get_request() const {
        return request;
    }

    int get_socket_fd() const {
        return socket_fd;
    }

    void close() {
        ::close(socket_fd);
    }
};
#endif