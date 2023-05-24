/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhouya <akhouya@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 17:44:51 by akhouya           #+#    #+#             */
/*   Updated: 2023/05/24 18:05:10 by akhouya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Client.hpp"
#include <vector>
#include <sstream>
std::vector<std::string>    split_words(const std::string &str) {
    std::vector<std::string> words;

    if (str.at(str.length() - 1) != '\r')
        exit(0);
    std::istringstream iss(str);
    while (iss)
    {
        std::string word;
        iss >> word;
        // std::cout << word << std::endl;
        words.push_back(word);
    }
    return words;
}
request_t split_lines(const std::string &str) {
    std::vector<std::vector<std::string> >  vectors;
    std::istringstream iss(str);
    std::string line;
    request_t request;
    while (std::getline(iss, line, '\n'))
    {
        vectors.push_back(split_words(line));
    }
    for (int i = 0; i < vectors.size(); i++)
    {
        for(int j = 0; j < vectors.at(i).size(); j++) {
            switch (i) {
                case 0: {
                    switch (j) {
                    case 0:
                        request.method = vectors.at(i).at(j);
                    case 1:
                        request.path = vectors.at(i).at(j);
                    case 2:
                        request.http_version = vectors.at(i).at(j);
                    }
                case 1:
                    request.host =  vectors.at(i).at(1);
                }
            }
                
        }
        }

    return request;
}

WBS::Client::Client(int sock) {
    _sock = sock;
    // _buffer = "";
    // _request = {};
    // _request.method = "";
}

request_t WBS::Client::get_request() {
    return _request;
}

void WBS::Client::set_request(request_t request) {
    _request = request;
}


std::string WBS::Client::get_buffer() {
    return _buffer;
}


void WBS::Client::set_buffer(std::string buffer) {
    _buffer = buffer;
}

int WBS::Client::get_sock() {
    return _sock;
}

void WBS::Client::set_sock(int sock) {
    _sock = sock;
}

void WBS::Client::parse_request() {
    _request = split_lines(_buffer);
}