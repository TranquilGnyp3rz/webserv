#include "../../inc/config_file/Server.hpp"

Server::Server()
{
    _listen = 0;
    _host = "";
    _server_name = "";
    _root = "";
    _index = "";
    _clientMaxBodySize = "";
    _uploadPath = "";
}

Server::~Server()
{
}

/*seters*/
void Server::set_listen(int listen)
{
    _listen = listen;
}

void Server::set_host(std::string host)
{
    _host = host;
}

void Server::set_server_name(std::string server_name)
{
    _server_name = server_name;
}

void Server::set_root(std::string root)
{
    _root = root;
}

void Server::set_index(std::string index)
{
    _index = index;
}

void Server::set_clientMaxBodySize(std::string clientMaxBodySize)
{
    _clientMaxBodySize = clientMaxBodySize;
}

void Server::set_error_pages(int error_code, std::string error_pages)
{
    _error_pages.push_back(std::make_pair(error_code, error_pages));
}


void Server::set_uploadPath(std::string uploadPath)
{
    _uploadPath = uploadPath;
}

void Server::set_locations(std::vector<Location> locations)
{
    _locations = locations;
}

/*geters*/
int Server::get_listen()
{
    return _listen;
}

std::string Server::get_host()
{
    return _host;
}

std::string Server::get_server_name()
{
    return _server_name;
}

std::string Server::get_root()
{
    return _root;
}

std::string Server::get_index()
{
    return _index;
}

std::string Server::get_clientMaxBodySize()
{
    return _clientMaxBodySize;
}

std::vector<std::pair<int, std::string> > Server::get_error_pages()
{
    return _error_pages;
}

std::string Server::get_uploadPath()
{
    return _uploadPath;
}

std::vector<Location> Server::get_locations()
{
    return _locations;
}
