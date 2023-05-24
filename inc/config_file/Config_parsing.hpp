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
        void HandleConfigFile(std::string filepath);
        void CheckConfigFile();                                     // check if the config file is valid
        void ParseConfigFile();                                     // parse the config file
        void fill_server_attribute(std::string line_trimmed);      // fill the server attribute
        void fill_location_attribute(std::string line_trimmed, Location& location);    // fill the location attribute
        void print_vector();

};

    bool is_empty(std::ifstream &ifile);                               // check if the file is empty
    void print_error_exit(std::string message);                        // print the error message and exit
    std::string trim_line(std::string line);                        // remove spaces from the begining and the end of the line

#endif