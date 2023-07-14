#ifndef RESOURCEHANDLER_HPP
#define RESOURCEHANDLER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <ctime>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include "Config.hpp"
#include "Server.hpp"
#include "Location.hpp"
#include "Client.hpp"
#include "response.hpp"

class ResourceHandler {
    private:
        Client &_client;
        std::vector<Server> &_servers;
        std::map<int, std::string> httpResponses;
        std::map<std::string, std::string> _mimeTypes;
        std::map<std::string, std::string> _headers;
    
        response_t handle_location(Server &server, std::vector<Location> &locations);
        response_t handle_method(Server &server, Location &locations);
        response_t get_file(Server  &server, Location  &location);
        response_t get_directory(Server  &server, Location  &location);
        response_t delete_file(Server  &server, Location  &location);
        std::string get_mime_type(std::string path);
        std::string get_last_modified(std::string path);
        std::string get_date();
        std::string get_headers(std::map<std::string, std::string> &headers);
        
        response_t costum_error_page(int error_code);
        std::string custom_error(const std::string& status);
        response_t check_request( void );
        std::string random_string( size_t length );
        bool location_match(std::string location, std::string path);
        int get_file_size(int fd);
    public:
        ResourceHandler(Config &config, Client &client);
        response_t handle_request();
};

#endif