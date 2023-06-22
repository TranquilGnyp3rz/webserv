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
#include "Response.hpp"

class Client {
    private:
        Response   _response;
        request_t _request;
        std::string _buffer;
        bool _bad_request;
        int _port;
        int _sock;
        bool _first_body;
        std::string _body_file;
    public:
        Client(int sock);
        Client(int port, int sock);
        // void split_lines(const std::string &str);
        // void split_words(const std::string &str);
        void parse_request();
        std::string get_buffer();
        void set_buffer(std::string buffer);
        int get_sock();
        void set_sock(int sock);
        request_t get_request();
        //get port
        int get_port();
        void set_request(request_t request);
        void save_body(std::string &buffer, int &close_conn);
        int  respond();
        void set_bad_request(bool bad_request) {
            _bad_request = bad_request;
        }
        bool get_bad_request(){
            return _bad_request;
        }
        bool get_first_body() {
            return _first_body;
        }
        void set_first_body(bool first_body) {
            _first_body = first_body;
        }
        void set_body_file(std::string body_file) {
            _body_file = body_file;
        }
        std::string get_body_file() {
            return _body_file;
        }
};
