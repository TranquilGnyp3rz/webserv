#include "../../inc/tmp/request.hpp"
#include "../../inc/tmp/client.hpp"
#include "../../inc/config_file/Config_parsing.hpp"
#include "../../inc/config_file/Server.hpp"
#include "../../inc/config_file/Location.hpp"
// #include "../../inc/config_file/server.hpp"

int method_DELETE(WBS::Client client, Config& config)
{
    
    std::string path = client.get_request().path;
    path = "/okey/my/folder/myfile.txt";             // get it from client -> request -> path
    request_t request;                              // get it from client -> request
    request.host = "127.0.0.1:8002";                // get it from client -> request -> host
    client.set_request(request);                    // already done in client
    std::vector<Location> locations;                
    for (std::vector<Server>::iterator it = config.get_servers().begin(); it != config.get_servers().end(); it++)
    {
        if (client.get_request().host != it->get_host() + ':' + it->get_listen())
            continue;
        for (std::vector<Location>::iterator it2 = it->get_locations().begin(); it2 != it->get_locations().end(); it2++)
        {
            if (path.find(it2->get_locationName() + '/') != std::string::npos)
                locations.push_back(*it2);
        }
    }
    if (locations.size() == 0)
    {
        std::cout << "Location not found" << std::endl; // TODO: change to error 404
        return 1;
    }
            std::cout << "Location found" << std::endl;
            for(std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++)
            {
            std::cout << it->get_locationName() << std::endl;
            std::cout << it->get_root() << std::endl;
            std::cout << "-------------------" << std::endl;
            }

    //here we have the location all the locations that match the path, we need to find the one with the longest path
    std::vector<Location>::iterator it2 = locations.begin();
    for(std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++)
    {
        if (it->get_locationName().length() > it2->get_locationName().length())
            it2 = it;
    }
            std::cout << "Location with longest path: " << it2->get_locationName() << std::endl;
            std::cout << "Root: " << it2->get_root() << std::endl;

    std::string root = it2->get_root();
    std::string path_no_location = path.substr(it2->get_locationName().length() + 1);   // myfile.txt
    std::string full_path = root + path_no_location;                                // /var/www/my/folder/myfile.txt
            std::cout << "Full path: " << full_path << std::endl;

    // std::string root = it2->get_root();
    // std::string path_no_location = path.substr(it2->get_locationName().length());
    // std::string full_path = root + path_no_location;

    // if (remove(full_path.c_str()) != 0)
    // {
    //     std::cout << "Error deleting file" << std::endl;
    //     return 1;
    // }
    // else
    // {
    //     std::cout << "File successfully deleted" << std::endl;
    //     return 0;

return 0;
}