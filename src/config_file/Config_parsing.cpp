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
    // std::string line_trimmed2;
    while (getline(_configfile, line))
    {
        /*-----------------------Skip spaces and remove comments-----------------------*/
        line_trimmed = trim_line(line);
        if (line_trimmed.empty() || line_trimmed[0] == '#')
            continue;
        if (line_trimmed.find('#'))
        {
            std::string::size_type pos = line_trimmed.find('#');
            line_trimmed = line_trimmed.substr(0, pos);
        }

        /*-----------------------Found the keyword 'server' and '{'-----------------------*/
        if(line_trimmed.substr(0, 6) == "server" && !_serverBlock)
        {
            if (trim_line(line_trimmed.substr(6)) != "{")
                print_error_exit("Missing { after server");
            _serverBlock = true;
            _curlebracket.push('{');
            Server server;
            _servers.push_back(server);
            continue;
        }

        /*-----------------------Found the keyword 'location' and '{'-----------------------*/
        else if (line_trimmed.substr(0, 8) == "location" && _serverBlock )
        {
            // std::cout << "hell no" << std::endl;
            if (line_trimmed.find('{') == std::string::npos)
                print_error_exit("Missing { after location");
            _curlebracket.push('{');
            Location location;
            size_t pos = line_trimmed.find('{');
            std::string locationName = line_trimmed.substr(8, pos - 8);
            locationName = trim_line(locationName);
            location.set_locationName(locationName);
            while(std::getline(_configfile, line))
            {
                line_trimmed = trim_line(line);
                if (line_trimmed == "}")
                    break;

                /*----------------Skip spaces and remove comments in location scoop---------------*/
                if (line_trimmed.empty() || line_trimmed[0] == '#')
                    continue;
                if (line_trimmed.find('#'))
                {
                    std::string::size_type pos = line_trimmed.find('#');
                    line_trimmed = line_trimmed.substr(0, pos);
                }
                continue;
            }
            _servers.back().set_locations(location);
            // fill all the location and push it to the vector of location after the while loop
            // size_t pos = line_trimmed.find('{');
            // use substr().

        }

        /*-----------------------We are in a scoop of server-----------------------*/
        else if (_serverBlock)
        {
            if (line_trimmed == "}")
            {
                if (_curlebracket.empty())
                    print_error_exit("Missing {");
                char curly = _curlebracket.top();
                _curlebracket.pop();
                if (curly != '{')
                    print_error_exit("Missing {");
                if (_curlebracket.empty())
                    _serverBlock = false;
                continue;
            }

            fill_server_attribute(line_trimmed);
        }
        // std::stringstream ss(line_trimmed);
        else
            print_error_exit("Invalid config file");

    }
    _configfile.close();
}

void Config::fill_server_attribute(std::string line_trimmed)
{
       
    std::string key;
    std::string value;
    std::string checker;
    // std::cout << "line :" << line_trimmed << std::endl;
    std::stringstream ss(line_trimmed);
    std::getline(ss, key, ' ');
    std::getline(ss, value, ';');
    value = trim_line(value);
    ss >> checker;
    // std::cout << "key :[" << key << "]" << std::endl;
    // std::cout << "value :[" << value << "]" << std::endl;
    // std::cout << "checker :[" << checker << "]"<< std::endl;
    // std::string::size_type pos = line_trimmed.find(" ");
    // if (pos == std::string::npos)
    //     print_error_exit("Missing space between key and value");
    // key = line_trimmed.substr(0, pos);
    // value = line_trimmed.substr(pos + 1);
    if (value.empty())
        print_error_exit("Missing value after key");
    if (key == "listen" && checker.size() == 0)
        _servers.back().set_listen(std::atoi(value.c_str()));
    else if (key == "host" && checker.size() == 0)
        _servers.back().set_host(value);
    else if (key == "server_name" && checker.size() == 0)
        _servers.back().set_server_name(value);
    else if (key == "root" && checker.size() == 0)
        _servers.back().set_root(value);
    else if (key == "index" && checker.size() == 0)
        _servers.back().set_index(value);
    else if (key == "client_max_body_size" && checker.size() == 0)
        _servers.back().set_clientMaxBodySize(value);
    else if (key == "error_page" && checker.size() == 0)
    {
        std::string error_code;
        std::string error_page;
        std::string::size_type posc;
        std::string::size_type posp;
        
        while(value.size() != 0)
        {
            posc = value.find(" ");
            if (posc == std::string::npos)
                break;
            error_code = value.substr(0, posc);
            value = value.substr(posc + 1);
            value = trim_line(value);
            posp = value.find(" ");
            error_page = value.substr(0, posp);
            _servers.back().set_error_pages(atoi(error_code.c_str()), error_page);
            value = value.substr(posp + 1);
            value = trim_line(value);
        }
    }
    // else if (key == "upload_path" && checker.size() == 0)
    //     _servers.back().set_uploadPath(value);
    else if (key == "cgi_path" && checker.size() == 0)
    {
        std::string::size_type pos = value.find(" ");
        while(pos != std::string::npos)
        {
            _servers.back().set_cgipath(value.substr(0, pos));
            value = value.substr(pos + 1);
            value = trim_line(value);
            pos = value.find(" ");
        }
        _servers.back().set_cgipath(value.substr(0, pos));
    }

    else if (key == "cgi_ext" && checker.size() == 0)
    {
        std::string::size_type pos = value.find(" ");
        while(pos != std::string::npos)
        {
            _servers.back().set_cgiextension(value.substr(0, pos));
            value = value.substr(pos + 1);
            value = trim_line(value);
            pos = value.find(" ");
        }
        _servers.back().set_cgiextension(value.substr(0, pos));
    }
               
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
        print_error_exit("Invalid value for : " + key);
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
        std::cout << "listen : [" << it->get_listen() << "]" << std::endl;
        std::cout << "host : [" << it->get_host() << "]" << std::endl;
        std::cout << "server_name : [" << it->get_server_name() << "]" << std::endl;
        std::cout << "root : [" << it->get_root() << "]" << std::endl;
        std::cout << "index : [" << it->get_index() << "]" << std::endl;
        std::cout << "client_max_body_size : [" << it->get_clientMaxBodySize() << "]" << std::endl;
        // std::cout << "error_page : " << std::endl;
        // std::map<int, std::string> error_pages = it->get_error_pages();
        // for (std::map<int, std::string>::iterator it = error_pages.begin(); it != error_pages.end(); ++it)
        // {
        //     std::cout << "error_code : " << it->first << std::endl;
        //     std::cout << "error_page : " << it->second << std::endl;
        // }
        // std::cout << "upload_path : " << it->get_uploadPath() << std::endl;
        std::cout << "locations : [" << std::endl;
        for (std::vector<std::string>::iterator itc = it->get_cgipath().begin(); itc != it->get_cgipath().end(); ++itc)
        {
            std::cout << "cgi_path : [" << *itc << "]" << std::endl;
        }

        for (std::vector<std::string>::iterator itcx = it->get_cgiextension().begin(); itcx != it->get_cgiextension().end(); ++itcx)
        {
            std::cout << "cgi_ext : [" << *itcx << "]" << std::endl;
        }

        for (std::vector<std::pair<int, std::string> >::iterator ite = it->get_error_pages().begin(); ite != it->get_error_pages().end(); ++ite)
        {
            std::cout << "error_code : [" << ite->first << "] | " << "error_page : [" << ite->second << "]" << std::endl;
        }

        for (std::vector<Location>::iterator itl = it->get_locations().begin(); itl != it->get_locations().end(); ++itl)
        {
            std::cout << "locationName : [" << itl->get_locationName() << "]" << std::endl;
        }
        // std::vector<Location> locations = it->get_locations();
        // for (std::vector<Location>::iterator it = locations.begin(); it != locations.end(); ++it)
        // {
        //     std::cout << "location : " << it->get_location() << std::endl;
        //     std::cout << "root : " << it->get_root() << std::endl;
        // }
        // std::cout << "here" << std::endl;
    }
}