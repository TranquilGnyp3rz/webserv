#include "../../inc/tmp/request.hpp"
#include "../../inc/tmp/client.hpp"
#include "../../inc/config_file/Config_parsing.hpp"
#include "../../inc/config_file/Server.hpp"
#include "../../inc/config_file/Location.hpp"

#include <sys/stat.h>
#include <ftw.h>
#include <unistd.h>
// #include "../../inc/config_file/server.hpp"

bool canDelete(const char* path) {
    struct stat path_info;
    if (stat(path, &path_info) != 0) {
        std::cerr << "Error retrieving file/directory information: " << strerror(errno) << std::endl;
        return false;
    }

    if (S_ISDIR(path_info.st_mode)) {
        if (access(path, R_OK | W_OK | X_OK) == 0) {
            return true;
        }
    } else {
        if (access(path, R_OK | W_OK) == 0) {
            return true;
        }
    }

    return false;
}

static int deleteFile(const char* path, const struct stat* sb, int typeflag, struct FTW* ftwbuf) {
    (void) sb;
    (void) typeflag;
    (void) ftwbuf;
    if (remove(path) == -1) 
    {
        std::cerr << "Error deleting file/directory: " << strerror(errno) << std::endl;

    } else {
        std::cout << "Deleted file/directory: " << path << std::endl;
    }

    return 0;
}


int method_DELETE(Client client, Config& config)
{
    
    std::string path = client.get_request().path;
    path = "/okey/my/folder/myfile/";             // get it from client -> request -> path
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
        if (locations.size() != 0)
            break;
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
    std::string full_path = root + path_no_location;                                // ./media/myfile.txt
            std::cout << "Full path: " << full_path << std::endl;
    
    /*------------------------------- begin check and delete -------------------------------*/

    if (access(full_path.c_str(), F_OK) == -1) {
        std::cout << "File/directory does not exist: " << full_path << std::endl;
        return 404;
    }
    struct stat path_info;
    if (stat(full_path.c_str(), &path_info) != 0) {
        std::cerr << "Error retrieving file/directory information: " << strerror(errno) << std::endl;
        return 404;
    }
    else
    {
        if (S_ISDIR(path_info.st_mode)) {
            std::cout << "The path refers to a directory." << std::endl;
            if(full_path[full_path.size() - 1] != '/')
                return 409;
            if(canDelete(full_path.c_str()))
            {
                if (nftw(full_path.c_str(), deleteFile, 64, FTW_DEPTH | FTW_PHYS) == -1) {
                    std::cerr << "Error traversing directory: " << strerror(errno) << std::endl;
                    return 500;
                }
                else
                    return 204;
            }
            else
            {
                std::cout << "Cannot delete directory: " << full_path << std::endl;
                return 403;
            }
        } 
        else {
            std::cout << "The path refers to a file." << std::endl;
            if (remove(full_path.c_str()) != 0)
            {
                std::cout << "Error deleting file" << std::endl;
                return 500;
            }
            else
            {
                std::cout << "File successfully deleted" << std::endl;
                return 204;
            }
        }

    }

    return 0;
}