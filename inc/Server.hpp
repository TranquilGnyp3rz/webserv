#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include "Location.hpp"

class Location;
class Server
{
private:
    std::string                                 _listen;
	std::string									_host;
    std::string                                 _server_name;
	std::string									_root;
	std::string									_index;
    std::string                                 _clientMaxBodySize;
    std::string									_uploadPath;
    std::vector<std::pair<int, std::string> >	_error_pages;
    std::vector<std::string>					_cgipath;
    std::vector<std::string>					_cgiextension;
    std::vector<Location> 						_locations;
public:
    Server();
    ~Server();

    /*seters*/
    void set_listen(std::string listen);
    void set_host(std::string host);
    void set_server_name(std::string server_name);
    void set_root(std::string root);
    void set_index(std::string index);
    void set_clientMaxBodySize(std::string clientMaxBodySize);
    void set_error_pages(int error_code, std::string error_pages);
    void set_uploadPath(std::string uploadPath);
    void set_cgipath(std::string cgipath);
    void set_cgiextension(std::string cgiextension);
    void set_locations(Location locations);

    /*geters*/
    std::string									get_listen();
    std::string                                 get_host();
    std::string									get_server_name();
    std::string									get_root();
    std::string									get_index();
    std::string									get_clientMaxBodySize();
    std::vector<std::pair<int, std::string> >& 	get_error_pages();
    std::string									get_uploadPath();
    std::vector<std::string>&					get_cgipath();
    std::vector<std::string>&					get_cgiextension();
    std::vector<Location>&					    get_locations();

    /*methods*/
    void sort_locations();


};

#endif