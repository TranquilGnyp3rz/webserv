#include "../../inc/config_file/Config_parsing.hpp"

Config::Config(): _serverBlock(false)
{
    
}

Config::~Config()
{

}
void Config::Handle_configFile(std::string filepath)
{
    this->_filePath = filepath;
    this->_configfile.open(_filePath.c_str());
    Check_configFile();
    Parse_ConfigFile();
    if(_curlebracket.size() != 0)
        print_error_exit("Curlebracket '}' is missing");
    // Print_vector();
    Check_complete_config_object();
    Print_vector();
}

void Config::Check_complete_config_object()
{
    if (_servers.size() == 0)
        print_error_exit("Missing server block");
        
    //-------------------------Server---------------------------------
    for (std::vector<Server>::iterator it = _servers.begin(); it != _servers.end(); it++)
    {
        if (it->get_listen() == "") 
            print_error_exit("Need to specify a port (listen)");
        if (!is_valid_port(it->get_listen()))
            print_error_exit("Unvalid value for port (listen)");
        if (it->get_host() == "")
            it->set_host("127.0.0.1");
        if (!is_valid_host(it->get_host()))
            print_error_exit("Unvalid value for host");
        if (it->get_server_name().find(" ") != std::string::npos)
            print_error_exit("Unvalid value for server_name (don't use space)");
        if (!is_valid_index(it->get_index()) && it->get_index() != "")
            print_error_exit("Unvalid value for index");
        if (it->get_error_pages().size() == 0)
            print_error_exit("Error_page is missing");
        if (it->get_clientMaxBodySize() != "")
            if (!is_number(it->get_clientMaxBodySize()) || atoi(it->get_clientMaxBodySize().c_str()) == 0)
                print_error_exit("Unvalid value for client_max_body_size");
        if (it->get_uploadPath() == "")
            print_error_exit("Upload_path is missing");
        if(it->get_uploadPath().find(" ") != std::string::npos)
            print_error_exit("Unvalid value for upload_path");
        if (it->get_cgiextension().size() == 0)
            print_error_exit("Cgi extension is missing");
        for (std::vector<std::string>::iterator it2 = it->get_cgiextension().begin(); it2 != it->get_cgiextension().end(); it2++)
        {
            if (*it2 != ".py" && *it2 != ".sh" && *it2 != ".php")
                print_error_exit("Unvalid value for cgi extension");
        }

        //-------------------------location---------------------------------
        if (it->get_locations().size() == 0)
            print_error_exit("Location block is missing");
        for (std::vector<Location>::iterator it2 = it->get_locations().begin(); it2 != it->get_locations().end(); it2++)
        {
            if (it2->get_locationName() == "")
                it2->set_locationName("/");
            if (it2->get_root() == "")
                print_error_exit("Need to specify a root for location block");
            if (it2->get_root().find(" ") != std::string::npos)
                print_error_exit("Unvalid value for root");
            if (it2->get_index() == "")
                it2->set_index(it->get_index());
            if (it2->get_index().find(" ") != std::string::npos)
                print_error_exit("Unvalid value for index");
            if (it2->get_autoIndex() == "")
                it2->set_autoIndex("off");
            if (it2->get_autoIndex() != "on" && it2->get_autoIndex() != "off")
                print_error_exit("Unvalid value for autoindex");
            if (it2->get_redirection().find(" ") != std::string::npos)
                print_error_exit("Unvalid value for redirection");
            if (it2->get_allowedMethods().size() == 0)
                print_error_exit("Missing allowed_methods");
            if (is_valid_methods(it2->get_allowedMethods()).size() != 0)
                print_error_exit("Unvalid method for allowed_methods : " + is_valid_methods(it2->get_allowedMethods()));
        }
    }
}

void Config::Check_configFile()
{
    if (_configfile.is_open())
    {
        if (is_empty(_configfile))
            print_error_exit("Empty config file");
    }
    else
        print_error_exit("Unable to open " + this->_filePath);
}

void Config::Parse_ConfigFile()
{
    std::string line;
    std::string line_trimmed;
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
            line_trimmed = trim_line(line_trimmed);
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

            /*-----------------------We are in a scoop of location-----------------------*/
            while(std::getline(_configfile, line))
            {
                line_trimmed = trim_line(line);
                if (line_trimmed == "}")
                {
                    if (_curlebracket.empty() || _curlebracket.top() != '{')
                        print_error_exit("Missing {");
                    _curlebracket.pop();
                    break;
                }

                /*----------------Skip spaces and remove comments in location scoop---------------*/
                if (line_trimmed.empty() || line_trimmed[0] == '#')
                    continue;
                if (line_trimmed.find('#'))
                {
                    std::string::size_type pos = line_trimmed.find('#');
                    line_trimmed = line_trimmed.substr(0, pos);
                }
                Fill_location_attribute(line_trimmed, location);
            }
            _servers.back().set_locations(location);
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
            Fill_server_attribute(line_trimmed);
        }
        else
            print_error_exit("Invalid config file");
    }
    _configfile.close();
}

void Config::Fill_location_attribute(std::string line_trimmed, Location& location){
    location.set_locationNumber(_servers.back().get_locations().size() + 1);
    std::string key;
    std::string value;
    std::string checker;
    if (line_trimmed.find(';') == std::string::npos)
        print_error_exit("Missing ';' in location scoop : [ " + line_trimmed + " ]");
    std::stringstream ss(line_trimmed);
    std::getline(ss, key, ' ');
    std::getline(ss, value, ';');
    value = trim_line(value);
    ss >> checker;
    
    if (key == "autoindex" && checker.size() == 0){
        if (value.empty())
            print_error_exit("Missing value after key");
        location.set_autoIndex(value);
    }
    else if (key == "index" && checker.size() == 0){
        if (value.empty())
            print_error_exit("Missing value after key");
        location.set_index(value);
    }
    else if (key == "root" && checker.size() == 0){
        if (value.empty())
            print_error_exit("Missing value after key");
        location.set_root(value);
    }
    else if (key == "return" && checker.size() == 0){
        if (value.empty())
            print_error_exit("Missing value after key");
        location.set_redirection(value);
    }
    else if (key == "allow_methods" && checker.size() == 0){
        if (value.empty())
            print_error_exit("Missing value after key");
        std::string::size_type pos = value.find(" ");
        while(pos != std::string::npos)
        {
            location.set_allowedMethods(value.substr(0, pos));
            value = value.substr(pos + 1);
            value = trim_line(value);
            pos = value.find(" ");
        }
        location.set_allowedMethods(value.substr(0, pos));

    }
    else
        print_error_exit("Unknown key : [ " + key + " ]");
}

void Config::Fill_server_attribute(std::string line_trimmed)
{
       
    std::string key;
    std::string value;
    std::string checker;

    if (line_trimmed.find(';') == std::string::npos)
        print_error_exit("Missing ';' in server scoop");
    std::stringstream ss(line_trimmed);
    std::getline(ss, key, ' ');
    std::getline(ss, value, ';');
    value = trim_line(value);
    ss >> checker;
    if (key == "listen" && checker.size() == 0){
        if (value.empty())
            print_error_exit("Missing value after key : "+ key);
        _servers.back().set_listen(value.c_str());
    }
    else if (key == "host" && checker.size() == 0){
        if (value.empty())
            print_error_exit("Missing value after key : "+ key);
        _servers.back().set_host(value);
    }
    else if (key == "server_name" && checker.size() == 0){
        if (value.empty())
            print_error_exit("Missing value after key : "+ key);
        _servers.back().set_server_name(value);
    }
    else if (key == "index" && checker.size() == 0){
        if (value.empty())
            print_error_exit("Missing value after key : "+ key);
        _servers.back().set_index(value);
    }
    else if (key == "client_max_body_size" && checker.size() == 0){
        if (value.empty())
            print_error_exit("Missing value after key : "+ key);
        _servers.back().set_clientMaxBodySize(value);
    }
    else if (key == "upload_path" && checker.size() == 0){
        if (value.empty())
            print_error_exit("Missing value after key : "+ key);
        _servers.back().set_uploadPath(value);
    }
    else if (key == "error_page" && checker.size() == 0)
    {
        if (value.empty())
            print_error_exit("Missing value after key : "+ key);
        std::string error_code;
        std::string error_page;
        std::string::size_type posc;
        std::string::size_type posp;
        
        while(value.size() != 0)
        {
            posc = value.find(" ");
            if (posc == std::string::npos){
                if(!value.empty())
                    print_error_exit("Missing error_page_path after error code : " + value);
                break;
            }
            error_code = value.substr(0, posc);
            value = value.substr(posc + 1);
            value = trim_line(value);
            posp = value.find(" ");
            error_page = value.substr(0, posp);
            if (!is_number(error_code) || error_code.size() != 3)
                print_error_exit("Error code must be a number of 3 digits : " + error_code);
            _servers.back().set_error_pages(atoi(error_code.c_str()), error_page);
            value = value.substr(posp + 1);
            value = trim_line(value);
            if (posp == std::string::npos){
                break;
            }
        }
    }
    else if (key == "cgi_path" && checker.size() == 0)
    {
        if (value.empty())
            print_error_exit("Missing value after key : "+ key);
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
        if (value.empty())
            print_error_exit("Missing value after key : "+ key);
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
    
    else
        print_error_exit("Unknown key : [ " + key + " ]");
}

bool is_number(const std::string str) {
    return str.find_first_not_of("0123456789") == std::string::npos;
}

bool is_valid_port(std::string port)
{
    if (!is_number(port))
        return false;
    int port_int = std::atoi(port.c_str());
    if (port_int < 0 || port_int > 65535)
        return false;
    return true;
}

bool check_zero_ip(std::string ip)
{
    if (ip[0] == '0' && ip.size() != 1)
        return true;
    return false;
}

bool is_valid_host(std::string ip)
{
    std::string::size_type pos = ip.find(".");
    if (pos == std::string::npos)
        return false;
    std::string first = ip.substr(0, pos);
    ip = ip.substr(pos + 1);
    pos = ip.find(".");
    if (pos == std::string::npos)
        return false;
    std::string second = ip.substr(0, pos);
    ip = ip.substr(pos + 1);
    pos = ip.find(".");
    if (pos == std::string::npos)
        return false;
    std::string third = ip.substr(0, pos);
    ip = ip.substr(pos + 1);
    if (ip.empty())
        return false;
    std::string fourth = ip;
    if (first.size() > 3 || second.size() > 3 || third.size() > 3 || fourth.size() > 3)
        return false;
    if (!is_number(first) || !is_number(second) || !is_number(third) || !is_number(fourth))
        return false;
    if (std::atoi(first.c_str()) > 255 || std::atoi(second.c_str()) > 255 || std::atoi(third.c_str()) > 255 || std::atoi(fourth.c_str()) > 255)
        return false;
    if (check_zero_ip(first) || check_zero_ip(second) || check_zero_ip(third) || check_zero_ip(fourth))
        return false;
    return true;
}

bool is_valid_index(std::string index)
{
    std::size_t pos;
    while((pos = index.find('.')) != std::string::npos)
    {
        if (index.substr(0, pos) == "")
            return false;
        index = index.substr(pos + 1);
    }
    if (index != "html")
        return false;
    return true;
}

std::string is_valid_methods(std::vector<std::string> methods)
{
    for (std::vector<std::string>::iterator it = methods.begin(); it != methods.end(); it++)
    {
        if (*it != "GET" && *it != "POST" && *it != "DELETE")
            return *it;
    }
    return "";
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


void Config::Print_vector()
{
    for (std::vector<Server>::iterator it = _servers.begin(); it != _servers.end(); ++it)
    {   std::cout << "----------------------------------------" << std::endl;
        std::cout << "Servers : " << std::endl;
        
        std::cout << "listen : [" << it->get_listen() << "]" << std::endl;
        std::cout << "host : [" << it->get_host() << "]" << std::endl;
        std::cout << "server_name : [" << it->get_server_name() << "]" << std::endl;
        std::cout << "index : [" << it->get_index() << "]" << std::endl;
        std::cout << "client_max_body_size : [" << it->get_clientMaxBodySize() << "]" << std::endl;
        std::cout << "upload_path : [" << it->get_uploadPath() << "]" << std::endl;
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
            std::cout << "----------------------------------------" << std::endl;
            std::cout << "locationNumber : [" << itl->get_locationNumber() << "]" << std::endl;
            std::cout << "locationName : [" << itl->get_locationName() << "]" << std::endl;
            std::cout << "autoindex : [" << itl->get_autoIndex() << "]" << std::endl;
            std::cout << "index : [" << itl->get_index() << "]" << std::endl;
            std::cout << "root : [" << itl->get_root() << "]" << std::endl;
            std::cout << "return : [" << itl->get_redirection() << "]" << std::endl;
            std::cout << "allowed_methods : [";
            for(std::vector<std::string>::iterator itm = itl->get_allowedMethods().begin(); itm != itl->get_allowedMethods().end(); ++itm)
            {
                std::cout << *itm << " ";
            }
            std::cout << "]" << std::endl;

        }
    }
}