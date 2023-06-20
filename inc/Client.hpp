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

class Client {
private:
    request_t _request;
    std::string _buffer;
    int _port;
    int _sock;
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
    void save_body(std::string &buffer, int size_buffer);
};
