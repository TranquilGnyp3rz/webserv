#include "../../inc/config_file/Location.hpp"

Location::Location()
{
    _locationNumber = 0;
    _locationName = "";
    _root = "";
    _index = "";
    _autoIndex = "";
    // _uploadPath = "";
    _redirection = "";
}

Location::~Location()
{
}

/*seters*/
void Location::set_locationNumber(int locationNumber)
{
    _locationNumber = locationNumber;
}

void Location::set_locationName(std::string locationName)
{
    _locationName = locationName;
}

void Location::set_allowedMethods(std::string allowedMethods)
{
    _allowedMethods.push_back(allowedMethods);
}

void Location::set_root(std::string root)
{
    _root = root;
}

void Location::set_index(std::string index)
{
    _index = index;
}

void Location::set_autoIndex(std::string autoIndex)
{
    _autoIndex = autoIndex;
}

// void Location::set_uploadPath(std::string uploadPath)
// {
//     _uploadPath = uploadPath;
// }

void Location::set_redirection(std::string redirection)
{
    _redirection = redirection;
}

/*geters*/
int                 Location::get_locationNumber()
{
    return _locationNumber;
}

std::string                 Location::get_locationName()
{
    return _locationName;
}

std::vector<std::string>    &Location::get_allowedMethods()
{
    return _allowedMethods;
}

std::string                 Location::get_root()
{
    return _root;
}

std::string                 Location::get_index()
{
    return _index;
}

std::string                 Location::get_autoIndex()
{
    return _autoIndex;
}

// std::string                 Location::get_uploadPath()
// {
//     return _uploadPath;
// }

std::string                 Location::get_redirection()
{
    return _redirection;
}

