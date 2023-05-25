/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhouya <akhouya@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 17:44:51 by akhouya           #+#    #+#             */
/*   Updated: 2023/05/25 16:05:40 by akhouya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Client.hpp"
#include <vector>
#include <sstream>

static std::vector<std::string>    split_words(const std::string &str, char c) {
    std::vector<std::string> words;

    std::istringstream iss(str);
    while (iss)
    {
        std::string word;
        std::getline(iss, word, c);
        words.push_back(word);
    }
    return words;
}

static request_t split_lines(const std::string &str) {
    std::vector<std::vector<std::string> >  vectors;
    std::istringstream iss(str);
    std::string line;
    request_t request;
    bool firstline = false;
    while (std::getline(iss, line, '\n'))
    {
        if (firstline == false) {
            firstline = true;
            vectors.push_back(split_words(line, ' '));
            request.method = vectors.at(0).at(0);
            request.path = vectors.at(0).at(1);
            request.http_version = vectors.at(0).at(2);
            continue;
        }
            vectors.push_back(split_words(line, ':'));
            
            request.headers.insert(std::pair<std::string, std::string>(vectors.at(vectors.size() - 1).at(0), vectors.at(vectors.size() - 1).at(1)));
               
    }
    return request;
}

WBS::Client::Client(int sock) {
    _sock = sock;
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