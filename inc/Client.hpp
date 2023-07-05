/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heloufra <heloufra@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 14:40:30 by akhouya           #+#    #+#             */
/*   Updated: 2023/06/08 10:54:34 by akhouya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <vector>
#include <sstream>
#include "request.hpp"
#include "SocketServer.hpp"
#include "ResourceHandler.hpp"
#include "Config_parsing.hpp"

class Response;

class Client {
    private:
        ResourceHandler  _resourceHandler;
        Config &_config;
        int fd;
        request_t _request;
        std::string _buffer;
        bool _bad_request;
        int _port;
        int _sock;
        bool _first_body;
        std::string _body_file;
        int _body_lenght;
        std::string _body;
    public:
        Client(Config &config, int sock);
        void parse_request();
        void save_body(std::string &buffer, int &close_conn);
 
        /* Getters */
        int get_sock();
        int get_port();
        request_t get_request();
        std::string get_buffer();
        bool get_bad_request(){
            return _bad_request;
        }
        bool get_first_body() {
            return _first_body;
        }
        std::string get_body_file() {
            return _body_file;
        }
        std::string get_body() {
            return _body;
        }

        /* Setters */
        void set_sock(int sock);
        void set_request(request_t request);
        void set_buffer(std::string buffer);
        void set_bad_request(bool bad_request) {
            _bad_request = bad_request;
        }
        void set_first_body(bool first_body) {
            _first_body = first_body;
        }
        void set_body_file(std::string body_file) {
            _body_file = body_file;
        }
        void set_body(std::string body) {
            _body = body;
        }
};
