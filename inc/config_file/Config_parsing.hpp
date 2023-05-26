#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <utility>
#include "./Server.hpp"


class Config{
    private :
        std::vector<Server>         _servers;
        std::string                 _filePath;
        std::ifstream               _configfile;
        bool                        _serverBlock;                   // true if we are in a server block
        std::stack<char>            _curlebracket;                  // to check curlebracket
    

    public :
        Config();
        ~Config();
        void Handle_configFile(std::string filepath);
        void Check_configFile();                                     // check if the config file is valid
        void Parse_ConfigFile();                                     // parse the config file
        void Fill_server_attribute(std::string line_trimmed);      // fill the server attribute
        void Fill_location_attribute(std::string line_trimmed, Location& location);    // fill the location attribute
        void Print_vector();
        void Check_complete_config_object();                                 // check if the config object is valid

};
    //                      outil
    bool is_empty(std::ifstream &ifile);                               // check if the file is empty
    void print_error_exit(std::string message);                        // print the error message and exit
    std::string trim_line(std::string line);                       // remove spaces from the begining and the end of the line
    bool is_number(std::string str);                               // check if the string is a number
    bool is_valid_port(std::string port);                          // check if the port is valid
    bool is_valid_host(std::string ip);                              // check if the ip is valid
    bool check_zero_ip(std::string ip);                            // check if the ip is not like 127.00.1.1
    bool is_valid_index(std::string index);                        // check if the index is valid
    std::string is_valid_methods(std::vector<std::string> methods);                      // check if the method is valid

#endif