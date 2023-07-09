/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heloufra <heloufra@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 17:44:51 by akhouya           #+#    #+#             */
/*   Updated: 2023/07/09 17:20:15 by heloufra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include<iostream>
#include<fstream>
#include "Client.hpp"
#include <fstream>
#include "ResourceHandler.hpp"


void read_from_file(const std::string filename)
{
    std::cout << "read from file" << std::endl;
    std::ifstream file(filename); 
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            // Process each line from the file
            std::cout << line << std::endl;
        }
        
        file.close();
    } else {
        std::cout << "Unable to open the file." << std::endl;
    }
}
std::string generate_filename() {
    std::string filename = "/tmp/";
    std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
    srand(time(NULL));
    for (int i = 0; i < 10; i++)
    {
        filename += charset[rand() % charset.length()];
    }
    filename += ".txt";
    return filename;
}
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
int Client::get_port() {
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
static std::string find_key(std::map<std::string, std::string> map, std::string key) {
    std::map<std::string, std::string>::iterator it;
    it = map.find(key);
    if (it != map.end())
        return it->second;
    return "";
}

Client::Client(Config &config, int sock): _config(config) {
 
    _sock = sock;
}
Client::Client(Config &config, int port, int sock): _config(config){

    _request.body_lenght = 0;
    _request.body = "";
    _request.first_body = false;
    _port = port;
    _sock = sock;
    _buffer = "";
    _request.body_file = generate_filename();
}

request_t Client::get_request() {
    return _request;
}


std::string Client::get_buffer() {
    return _buffer;
}

int Client::get_sock() {
    return _sock;
}

void Client::parse_request() {
    std::vector<std::string> vectors;
    std::istringstream iss(_buffer);
    std::string line;
    bool firstline = false;
    _request.headerdone = false;
    while (std::getline(iss, line, '\n') && _request.headerdone == false)
    {
        if (firstline == false) {
            firstline = true;
            vectors = (split_words(line, ' '));
            _request.method = vectors.at(0);
            _request.path = vectors.at(1);
            _request.http_version = vectors.at(2);
            _request.http_version.erase(_request.http_version.size() - 1);
            continue;
        }
        if (line == "\r")
        {
            _request.headerdone = true;
            break ;
        }

        else if (_request.headerdone == false)
        {
            line.erase(line.size() - 1);
            vectors = split_words(line, ':');
            vectors.at(0) = trim(vectors.at(0));
            vectors.at(1) = trim(vectors.at(1));
            _request.headers.insert(std::pair<std::string, std::string>((vectors.at(0)), vectors.at(1)));
        }
    }
}


void Client::save_body(std::string &buffer, int &close_conn) {

    std::string body(buffer);
    if (find_key(_request.headers, "Content-Length") != "")
        {
            int size = std::stoi(find_key(_request.headers, "Content-Length"));
            if (_request.body_lenght  < size) {
                write_in_file(_request.body_file, body.substr(0, size - _request.body_lenght));
                std::ifstream in_file(_request.body_file, std::ios::binary);
                in_file.seekg(0, std::ios::end);
                int file_size = in_file.tellg();
                _request.body_lenght = file_size;
            }
            if (_request.body_lenght == size) {
                close_conn = true;
            }
        }
    else if(find_key(_request.headers, "Transfer-Encoding") == "chunked")
    {
        if (_request.body_lenght != 0) {
            if (body.size() < _request.body_lenght + 2)
            {
                write_in_file(_request.body_file, body);
                _request.body_lenght -= body.size();
            }
            else {
                write_in_file(_request.body_file, body.substr(0, _request.body_lenght));
                body = body.substr(_request.body_lenght + 2);
                _request.body_lenght = 0;

            }
        }
        if (_request.body_lenght == 0) {
            if (_request.body != "")
            {
                body = _request.body + body;
                _request.body = "";
            }
            if (body.find("\r\n") == std::string::npos) {
                _request.body = body;
                body = "";
            }
            while ( body.find("\r\n") != std::string::npos)
            {
                int size = 0;
                    
                std::string::size_type pos = body.find("\r\n");
                std::string size_str = body.substr(0, pos);
                try {
                    // std::cout << "size_str: " << size_str << std::endl;
        
                    _request.body_lenght = std::stoi(size_str, 0, 16);
                }
                catch (std::exception &e) {
                    std::cout << "error stoi" << std::endl;
                    std::cout << e.what() << std::endl;
                    close_conn = true;
                    break;
                }
                if(_request.body_lenght == 0)
                {
                    close_conn = true;
                    break;
                }
                body = body.substr(pos + 2);
                if (body.size() < _request.body_lenght + 2)
                {
                    write_in_file(_request.body_file, body);
                    _request.body_lenght -= body.size();
                    break;
                }
                write_in_file(_request.body_file, body.substr(0, _request.body_lenght));
                body = body.substr(_request.body_lenght + 2);
                _request.body_lenght = 0;
            }
        }
    }
    else
        close_conn = true;
}

bool Client::response() {
  
    response_t response = ResourceHandler(_config, *this).handle_request();
    std::string str = "";
    char buffer[6000] = {0};
    str += response.headers;
    if (response.body ) {
        std::cout << "client : response : " << response.body_file << std::endl;
        int rc = read(response.body_file, buffer, 6000);
        std::cout << "client : buffer : " << rc <<buffer << std::endl;
        str += std::string(buffer);
    }
    
  
    std::cout << "-------------------------------------" << std::endl;
    std::cout << str << std::endl;
    std::cout << "-------------------------------------" << std::endl;
    send(_sock, str.c_str(), str.size(), 0);
    close(response.body_file);
    return true;
}