/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heloufra <heloufra@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 17:44:51 by akhouya           #+#    #+#             */
/*   Updated: 2023/06/09 12:44:59 by akhouya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include<iostream>
#include<fstream>
#include "../../inc/Client.hpp"
#include <fstream>
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
    std::string filename = "/goinfre/akhouya/muchowebs/";
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
    //append to file
    file.open(filename, std::ios::out | std::ios::app);
    file << buffer;
    file.close();
    // exit(0);


    // file.open(filename, std::ios::out | std::ios::app);
    // file << buffer;
    // file.close();
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
            //remove \r from http version
            request.http_version.erase(request.http_version.size() - 1);
            
            continue;
        }
        //if there is only \r\n
        if (line == "\r")
        {
            request.headerdone = true;
            request.body_file = generate_filename();
            break ;
        }

        else if (request.headerdone == false)
        {
            // erase /r from line
            line.erase(line.size() - 1);
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

Client::Client(int sock) {
    _sock = sock;
    _body_file.assign("ss");
}
Client::Client(int port, int sock) {
    _body_lenght = 0;
    _port = port;
    _sock = sock;
    _buffer = "";
    _body = "";
    //assign number socket
    _body_file.assign(std::to_string(sock));
}

request_t Client::get_request() {
    return _request;
}

void Client::set_request(request_t request) {
    _request = request;
}


std::string Client::get_buffer() {
    return _buffer;
}


void Client::set_buffer(std::string buffer) {
    _buffer = buffer;
}

int Client::get_sock() {
    return _sock;
}

void Client::set_sock(int sock) {
    _sock = sock;
}

void Client::parse_request() {
    _request = split_lines(_buffer);
}
// check if hexadicimal number


void Client::save_body(std::string &buffer, int &close_conn) {

    std::string body(buffer);

    if(find_key(_request.headers, "Transfer-Encoding") == "chunked")
    {
        if (_body_lenght != 0) {
            std::cout << "body lenght " << _body_lenght << std::endl;
            std::cout << "body size " << body.size() << std::endl;
            if (body.size() < _body_lenght + 2)
            {
                write_in_file(_body_file, body);
                _body_lenght -= body.size();
                // _body.substr(body.size());
            }
            else {
                write_in_file(_body_file, body.substr(0, _body_lenght));
                body = body.substr(_body_lenght + 2);
                _body_lenght = 0;

            }
            std::cout << "body lenght " << _body_lenght << std::endl;
            std::cout << "body size " << body.size() << std::endl;
            // exit(0);
        }
        if (_body_lenght == 0) {

            while ( body.find("\r\n") != std::string::npos)
            {
                int size = 0;
                    
                std::string::size_type pos = body.find("\r\n");
                std::string size_str = body.substr(0, pos);
                try {
                    std::cout << "size_str: " << size_str << std::endl;
        
                    _body_lenght = std::stoi(size_str, 0, 16);
                }
                catch (std::exception &e) {
                    std::cout << "error stoi" << std::endl;
                    std::cout << e.what() << std::endl;
                    close_conn = true;
                    break;
                }
                if(_body_lenght == 0)
                {
                    close_conn = true;
                    break;
                }
                body = body.substr(pos + 2);
                if (body.size() < _body_lenght + 2)
                {
                    write_in_file(_body_file, body);
                    _body_lenght -= body.size();
                    break;
                }
                write_in_file(_body_file, body.substr(0, _body_lenght));
                body = body.substr(_body_lenght + 2);
                _body_lenght = 0;
            }
        }
            
        
       
    }
    else if (find_key(_request.headers, "Content-Length") != "")
    {
        // exit(0);
        int size = std::stoi(find_key(_request.headers, "Content-Length"));
        if (_body_lenght  < size) {
            std::cout << "-------" << body << std::endl;
            write_in_file(_body_file, body.substr(0, size - _body_lenght));
            std::ifstream in_file(_body_file, std::ios::binary);
            in_file.seekg(0, std::ios::end);
            int file_size = in_file.tellg();
            _body_lenght = file_size;
            // std::cout << _body_lenght  << "--------"<< size<< std::endl;
            // std::cout << _body_file << std::endl;
        }
        if (_body_lenght == size) {
            // read_from_file(_request.body_file);
            close_conn = true;
        }
    }
    else {
        close_conn = true;
        if (_request.method == "POST" || _request.method == "PUT") {
            _bad_request = true;
        }
        
    }
    std::cout << "close conne ction: " << close_conn << std::endl;
}
    
int Client::respond()
{
    this->_response.send(_sock);
    return 0;
}
