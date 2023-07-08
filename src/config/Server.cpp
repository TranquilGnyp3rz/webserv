#include "Server.hpp"

Server::Server()
{
    _listen = "";
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
void Server::set_listen(std::string listen)
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

void Server::set_cgipath(std::string cgipath)
{
    _cgipath.push_back(cgipath);
}

void Server::set_cgiextension(std::string cgiextension)
{
    _cgiextension.push_back(cgiextension);
}

void Server::set_locations(Location locations)
{
    _locations.push_back(locations);
}

/*geters*/
std::string Server::get_listen()
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

std::vector<std::pair<int, std::string> >& Server::get_error_pages()
{
    return _error_pages;
}

std::string Server::get_uploadPath()
{
    return _uploadPath;
}

std::vector<std::string>& Server::get_cgipath()
{
    return _cgipath;
}

std::vector<std::string>& Server::get_cgiextension()
{
    return _cgiextension;
}

std::vector<Location>& Server::get_locations()
{
    return _locations;
}

void Server::sort_locations()
{
   std::reverse(_locations.begin(), _locations.end());
}

int Server::get_port()
{
    int port = atoi(_listen.c_str());
    return port;
}
