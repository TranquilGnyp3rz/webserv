#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <vector>
#include <string>

class Location
{
    private:
        int                         _locationNumber;
        std::string                 _locationName;
        std::string                 _autoIndex;
        std::string                 _index;
        std::string                 _root;
        // std::string                 _uploadPath;
        std::string                 _redirection;
        std::vector<std::string>    _allowedMethods;
    public:
        Location();
        ~Location();
        Location(const Location &b);

    /*seters*/
        void set_locationNumber(int locationNumber);
        void set_locationName(std::string locationName);
        void set_allowedMethods(std::string allowedMethods);
        void set_root(std::string root);
        void set_index(std::string index);
        void set_autoIndex(std::string autoIndex);
        // void set_uploadPath(std::string uploadPath);
        void set_redirection(std::string redirection);

    /*geters*/
        int                         get_locationNumber();
        std::string                 get_locationName();
        std::vector<std::string>    &get_allowedMethods();
        std::string                 get_root();
        std::string                 get_index();
        std::string                 get_autoIndex();
        // std::string                 get_uploadPath();
        std::string                 get_redirection();

        bool operator < (const Location &b) const;
        bool isMethodAllowed(std::string method);

};

#endif