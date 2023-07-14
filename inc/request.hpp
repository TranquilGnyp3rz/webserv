#ifndef REQUEST_HPP
#define REQUEST_HPP
#include<map>

#include <iostream>


typedef struct request_s {

    std::string method;
    std::string path;
    std::string http_version;
    std::string body_file;
    std::string body;

    int body_lenght;

    bool    headerdone;
    bool    first_body;

    std::map<std::string, std::string>  headers;
}   request_t;

#endif