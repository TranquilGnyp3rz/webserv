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

    bool is_empty(std::ifstream &ifile);                               // check if the file is empty
    void print_error_exit(std::string message);                        // print the error message and exit
    std::string trim_line(std::string line);                        // remove spaces from the begining and the end of the line

#endif