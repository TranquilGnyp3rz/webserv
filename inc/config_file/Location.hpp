#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <vector>
#include <string>

class Location
{
    private:
        int                         _locationNumber;
        std::string                 _locationName;
        std::vector<std::string>    _allowedMethods;
        std::string                 _root;
        std::string                 _index;
        std::string                 _autoIndex;
        std::string                 _uploadPath;
        std::string                 _redirection;
    public:
        Location();
        ~Location();

    /*seters*/
        void set_locationNumber(int locationNumber);
        void set_locationName(std::string locationName);
        void set_allowedMethods(std::vector<std::string> allowedMethods);
        void set_root(std::string root);
        void set_index(std::string index);
        void set_autoIndex(std::string autoIndex);
        void set_uploadPath(std::string uploadPath);
        void set_redirection(std::string redirection);

    /*geters*/
        int                         get_locationNumber();
        std::string                 get_locationName();
        std::vector<std::string>    get_allowedMethods();
        std::string                 get_root();
        std::string                 get_index();
        std::string                 get_autoIndex();
        std::string                 get_uploadPath();
        std::string                 get_redirection();

};

#endif