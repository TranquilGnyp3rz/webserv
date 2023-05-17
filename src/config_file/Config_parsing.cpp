#include "../../inc/config_file/Config_parsing.hpp"

Config::Config(): _serverBlock(false)
{
    
}

Config::~Config()
{

}
void Config::HandleConfigFile(std::string filepath)
{
    this->_filePath = filepath;
    this->_configfile.open(_filePath.c_str());
    CheckConfigFile();
    ParseConfigFile();
    print_vector();
}
void Config::CheckConfigFile()
{
    if (_configfile.is_open())
    {
        if (is_empty(_configfile))
            print_error_exit("Empty config file");
    }
    else
        print_error_exit("Unable to open " + this->_filePath);
}

void Config::ParseConfigFile()
{
    std::string line;
    std::string line_trimmed;
    while (getline(_configfile, line))
    {
        line_trimmed = trim_line(line);
        if (line_trimmed.empty() || line_trimmed[0] == '#')
            continue;
        // std::cout << "line :" << line_trimmed << std::endl;
        if(line_trimmed.find("server") != std::string::npos && !_serverBlock)
        {
            if (line_trimmed.find("{") == std::string::npos)
                print_error_exit("Missing { after server");
            _serverBlock = true;
            Server server;
            _servers.push_back(server);
            continue;
        }
        else if (_serverBlock)
        {
            fill_server_attribute(line_trimmed);
        }
        // std::stringstream ss(line_trimmed);

    }
    _configfile.close();
}

void Config::fill_server_attribute(std::string line_trimmed)
{
        std::cout << "line :" << line_trimmed << std::endl;
    if (line_trimmed.find("}") != std::string::npos ) // bricoulage, hydhaa
    {
        return;
    }
    std::string key;
    std::string value;
    std::string::size_type pos = line_trimmed.find(" ");
    if (pos == std::string::npos)
        print_error_exit("Missing space between key and value");
    key = line_trimmed.substr(0, pos);
    value = line_trimmed.substr(pos + 1);
    if (key == "listen")
        _servers.back().set_listen(std::atoi(value.c_str()));
    else if (key == "host")
        _servers.back().set_host(value);
    else if (key == "server_name")
        _servers.back().set_server_name(value);
    else if (key == "root")
        _servers.back().set_root(value);
    else if (key == "index")
        _servers.back().set_index(value);
    else if (key == "client_max_body_size")
        _servers.back().set_clientMaxBodySize(value);
    else if (key == "error_page")
    {
        // handle multiple error pages, need to be implemented, a loop while and stringstream will be good
        std::string::size_type pos = value.find(" ");
        if (pos == std::string::npos)
            print_error_exit("Missing space between key and value");
        std::string error_code = value.substr(0, pos);
        std::string error_page = value.substr(pos + 1);
        _servers.back().set_error_pages(atoi(error_code.c_str()), error_page);
    }
    else if (key == "upload_path")
        _servers.back().set_uploadPath(value);
    else if (key == "location")
    {
        return;
        // std::string::size_type pos = value.find(" ");
        // if (pos == std::string::npos)
        //     print_error_exit("Missing space between key and value");
        // std::string location = value.substr(0, pos);
        // std::string root = value.substr(pos + 1);
        // Location loc;
        // loc.set_location(location);
        // loc.set_root(root);
        // _servers.back().set_locations(loc);
    }
    else
        // print_error_exit(key);
        return;

}

bool is_empty(std::ifstream &ifile)
{
    return ifile.peek() == std::ifstream::traits_type::eof();
}

void print_error_exit(std::string message)
{
    std::cerr << "Error : " << message << std::endl;
    exit(1);
}

std::string trim_line(std::string line)
{
    std::string::size_type pos = line.find_last_not_of(" \t");
    if (pos != std::string::npos)
    {
        line.erase(pos + 1);
        pos = line.find_first_not_of(" \t");
        if (pos != std::string::npos)
            line.erase(0, pos);
    }
    else
        line.erase(line.begin(), line.end());
    return line;
}

void Config::print_vector()
{
    std::cout << "Servers : " << std::endl;
    for (std::vector<Server>::iterator it = _servers.begin(); it != _servers.end(); ++it)
    {
        std::cout << "listen : " << it->get_listen() << std::endl;
        std::cout << "host : " << it->get_host() << std::endl;
        std::cout << "server_name : " << it->get_server_name() << std::endl;
        std::cout << "root : " << it->get_root() << std::endl;
        std::cout << "index : " << it->get_index() << std::endl;
        std::cout << "client_max_body_size : " << it->get_clientMaxBodySize() << std::endl;
        // std::cout << "error_page : " << std::endl;
        // std::map<int, std::string> error_pages = it->get_error_pages();
        // for (std::map<int, std::string>::iterator it = error_pages.begin(); it != error_pages.end(); ++it)
        // {
        //     std::cout << "error_code : " << it->first << std::endl;
        //     std::cout << "error_page : " << it->second << std::endl;
        // }
        std::cout << "upload_path : " << it->get_uploadPath() << std::endl;
        std::cout << "locations : " << std::endl;
        // std::vector<Location> locations = it->get_locations();
        // for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); ++it)
        // {
        //     std::cout << "location : " << it->get_location() << std::endl;
        //     std::cout << "root : " << it->get_root() << std::endl;
        // }
    }
}