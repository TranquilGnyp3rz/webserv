#ifndef RESOURCEHANDLER_HPP
#define RESOURCEHANDLER_HPP
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include "Config_parsing.hpp"
#include "Client.hpp"
#include "macros.hpp"
#include <ctime>

class ResourceHandler
{
    private:
        Client _client;
        std::vector<Server> &_servers;
        std::map<int, std::string> httpResponses;

        /* private methods */
        int costum_error_page(int error_code);
        std::string custom_error(const std::string& status);
        int check_request( void );
        std::string random_string( size_t length );
        bool location_match(std::string location, std::string path);
    public:
        ResourceHandler(Config &config, Client &client);
        int handle_request();
        int handle_location(Server &server, std::vector<Location> &locations);
        int handle_method(Server &server, Location &locations);
        int get_file(Server const &server, Location const &location);
        int delete_file(Server const &server, Location const &location);
        int post_file(Server const &server, Location const &location);
};

#endif