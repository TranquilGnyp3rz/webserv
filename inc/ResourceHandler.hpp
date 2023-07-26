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
#include <sys/wait.h>
#include <algorithm>
#include <cstring>
#include "Config.hpp"
#include "Server.hpp"
#include "Location.hpp"
#include "Client.hpp"
#include "response.hpp"

class Client;

class ResourceHandler {
    private:
        Client &_client;
        std::vector<Server> &_servers;
        std::map<int, std::string> httpResponses;
        std::map<std::string, std::string> _mimeTypes;
        std::map<std::string, std::string> _headers;
        std::string _target;
    
        response_t handle_location(Server &server, std::vector<Location> &locations);
        response_t handle_method(Server &server, Location &locations);
  
        response_t GET(Server  &server, Location  &location);
        response_t DELETE(Server  &server, Location  &location);
        response_t POST(Server  &server, Location  &location);

        response_t get_index(Server  &server, Location  &location);
        response_t get_directory(Server  &server, Location  &location);
        response_t handler_cgi(Server  &server, Location  &location, std::string script_path);
        char       **set_cgi_envv(Server  &server, Location  &location, std::string script_path);
        bool        to_cgi(Server &server, std::string path);
        char        *get_cgi_bin(Server &server, Location &location, std::string script_path);
        std::string get_mime_type(std::string path);
        std::string get_last_modified(std::string path);
        std::string get_date();
        std::string get_headers(std::map<std::string, std::string> &headers);
        char        **convert_map_to_cgi_envv(std::map<std::string, std::string> &headers, std::map<std::string, std::string> &headers2);
        response_t dynamic_page(int status, bool config, Server &server);
        std::string generate_page(const std::string& status);
        std::string generate_headers(std::string status, std::string method, std::string request_target, int fd);
        response_t check_request( void );
        std::string random_string( size_t length );
        bool location_match(std::string location, std::string path);
        int get_file_size(int fd);
        std::string string_upper_copy(std::string str);
    public:
        ResourceHandler(Config &config, Client &client);
        response_t handle_request();
};

#endif