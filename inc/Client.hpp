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
        Response    _response;
        request_t   _request;
        std::string _buffer;
        bool        _bad_request;
        int         _port;
        int         _sock;
    public:
        Client(int sock);
        Client(int port, int sock);
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
            return _request.first_body;
        }
        void set_first_body(bool first_body) {
            _request.first_body = first_body;
        }
};
