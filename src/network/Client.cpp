/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhouya <akhouya@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 17:44:51 by akhouya           #+#    #+#             */
/*   Updated: 2023/06/09 12:44:59 by akhouya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/Client.hpp"
#include <fstream>
std::string generate_filename() {
    std::string filename = "/tmp/";
    std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
    for (int i = 0; i < 10; i++)
    {
        filename += charset[rand() % charset.length()];
    }
    filename += ".txt";
    return filename;
}
// write in randome file
void write_in_file(std::string filename, std::string buffer) {
    std::ofstream file;
    file.open(filename, std::ios::out | std::ios::app);
    file << buffer;
    file.close();
}
std::string trim(const std::string& str) 
{  
    std::string s = str;
    s.erase(0, s.find_first_not_of(' '));
    s.erase(s.find_last_not_of('\r')+1);
    s.erase(s.find_last_not_of(' ')+1);
    return s;
}
//get port
int WBS::Client::get_port() {
    return _port;
}
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
//find a key in map 
static std::string find_key(std::map<std::string, std::string> map, std::string key) {
    std::map<std::string, std::string>::iterator it;
    it = map.find(key);
    if (it != map.end())
        return it->second;
    return "";
}


static request_t split_lines(const std::string &str) {
    std::vector<std::string>  vectors;
    std::istringstream iss(str);
    std::string line;
    request_t request;
    bool firstline = false;
    request.headerdone = false;
    //check it there \r\n
    while (std::getline(iss, line, '\n') && request.headerdone == false)
    {
        if (firstline == false) {
            firstline = true;
            vectors = (split_words(line, ' '));
            request.method = vectors.at(0);
            request.path = vectors.at(1);
            request.http_version = vectors.at(2);
            continue;
        }
        //if there is only \r\n
        if (line == "\r")
        {
            request.headerdone = true;
            request.body_file = generate_filename();
            break ;
        }
        else if (request.headerdone == false){
            vectors = split_words(line, ':');
            vectors.at(0) = trim(vectors.at(0));
            vectors.at(1) = trim(vectors.at(1));
            request.headers.insert(std::pair<std::string, std::string>((vectors.at(0)), vectors.at(1)));
        }

       

        
    }
    // if (headerdone == true)
    // {
    //     std::getline(iss, line, '');
    //     if ( find_key(request.headers, "transfer-encoding") == "chunked")
    // {
    //     // request.body_file = line;
    // }
        
    // else
    // {
    //     int size = std::stoi(find_key(request.headers, "content-length"));
    //     if (size < request.body_file.size()) {
    //         request.body_file += line.substr(0, size - request.body_file.size());
    //     }
            
    //     // request.body_file += line;
    // }
    // }
     
    return request;
}

// WBS::Client::Client(int sock) {
//     _sock = sock;
// }
WBS::Client::Client(int port, int sock) {
    _request.lenght_body = 0;
    _port = port;
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


void WBS::Client::save_body(std::string &buffer, int size_buffer) {
    std::string::size_type pos = buffer.find("\r\n\r\n");
    
    std::string body = buffer.substr(pos + 4);
    
    if(find_key(_request.headers, "Content-Type") == "chunked")
    {
        while (body.find("\r\n") != std::string::npos)
        {
            int size = 0;
            std::string::size_type pos = body.find("\r\n");
            std::string size_str = body.substr(0, pos);
            size = std::stoi(size_str, 0, 16);
            body = body.substr(pos + 2);
            if (size == 0)
                break;
            write_in_file(_request.body_file, body.substr(0, size));
            body = body.substr(size + 2);
        }
    }
    else
    {
        int size = std::stoi(find_key(_request.headers, "Content-Length"));
        if (_request.lenght_body < size) {
                
        
        write_in_file(_request.body_file, body.substr(0, size - _request.lenght_body));
        _request.lenght_body += size - _request.lenght_body;
        }
    }
}
    
