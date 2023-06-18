#ifndef REQUEST_HPP
# define REQUEST_HPP
#include<map>

#include <iostream>


typedef struct request_s {
    std::string method;
    std::string path;
    std::string http_version;
    std::map<std::string, std::string>  headers;
    std::string body_file;
    int lenght_body;
    bool    headerdone;
}   request_t;

#endif