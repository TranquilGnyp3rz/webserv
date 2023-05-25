/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhouya <akhouya@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 14:40:30 by akhouya           #+#    #+#             */
/*   Updated: 2023/05/25 14:32:14 by akhouya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "request.hpp"
namespace WBS {
    class Client {
        private:
            request_t _request;
            std::string _buffer;
            int _sock;
        public:
            Client(int sock);
            // void split_lines(const std::string &str);
            // void split_words(const std::string &str);
            void parse_request();
            std::string get_buffer();
            void set_buffer(std::string buffer);
            int get_sock();
            void set_sock(int sock);
            request_t get_request();
            void set_request(request_t request);
        
    };
}

