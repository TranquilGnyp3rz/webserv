#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <vector>
#include <sstream>
#include "request.hpp"
#include "response.hpp"
#include "SocketServer.hpp"
#include "Config.hpp"

#define CHUNKED_SIZE 4096

class Client {
    private:
        Config          &_config;
        request_t       _request;
        response_t      _response;
        std::string     _buffer;
        bool            _bad_request;
        int             _port;
        int             _sock;

    public:
        Client(Config &config, int sock);
        Client(Config &config, int port, int sock);
        void parse_request();
        void save_body(std::string &buffer, int &close_conn);
 
        /* Getters */
        int get_sock();
        int get_port();
        request_t get_request();
        std::string get_buffer();
        void  set_buffer(std::string buffer) {
            _buffer = buffer;
        }
        bool get_bad_request(){
            return _bad_request;
        }
        bool get_first_body() {
            return _request.first_body;
        }
        void set_first_body(bool first_body) {
            _request.first_body = first_body;
        }
        bool response();
};

#endif