#include "../../inc/code_delete/request.hpp"
#include "../../inc/code_delete/client.hpp"
#include "../../inc/config_file/Config_parsing.hpp"
#include "../../inc/config_file/Server.hpp"
#include "../../inc/config_file/Location.hpp"

#include <sys/stat.h>
#include <ftw.h>
#include <unistd.h>
// #include "../../inc/config_file/server.hpp"

bool can_delete(const char* path)
{
    struct stat path_info;
    if (stat(path, &path_info) != 0) 
        return false;

    if (S_ISDIR(path_info.st_mode))
    {
        if (access(path, R_OK | W_OK | X_OK) == 0)
            return true;
    }
    else
    {
        if (access(path, R_OK | W_OK) == 0)
            return true;
    }
    return false;
}

static int delete_file(const char* path, const struct stat* sb, int typeflag, struct FTW* ftwbuf)
{
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

int code_check_delete(std::string path)
{
    if (access(path.c_str(), F_OK) == -1)
        return 404;
    struct stat path_info;
    if (stat(path.c_str(), &path_info) != 0)
        return 404;
    else
    {
        if (S_ISDIR(path_info.st_mode))
        {
            if(path[path.size() - 1] != '/')
                return 409;
            if(can_delete(path.c_str()))
            {
                if (nftw(path.c_str(), delete_file, 64, FTW_DEPTH | FTW_PHYS) == -1)
                    return 500;
                else
                    return 204;
            }
            else
                return 403;
        } 
        else
        {
            if (remove(path.c_str()) != 0)
                return 500;
        }

    }
return 200;
}

std::string search_location(Config& config, WBS::Client& client, std::string path)
{
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
        return "";

    /*---------here we have the location all the locations that match the path, we need to find the one with the longest path-------------*/
    std::vector<Location>::iterator it2 = locations.begin();
    for(std::vector<Location>::iterator it = locations.begin(); it != locations.end(); it++)
    {
        if (it->get_locationName().length() > it2->get_locationName().length())
            it2 = it;
    }
    std::string root = it2->get_root();
    std::string path_no_location = path.substr(it2->get_locationName().length() + 1);   // myfile.txt
    std::string full_path = root + path_no_location;                                // ./media/myfile.txt
    return full_path;        
}

int code_DELETE(WBS::Client& client, Config& config)
{
    
    std::string path = client.get_request().path;
    path = "/okey/my/folder/myfile.txt";             // get it from client -> request -> path
    request_t request;                              // get it from client -> request
    request.host = "127.0.0.1:8002";                // get it from client -> request -> host
    client.set_request(request);                    // already done in client

    std::string full_path = search_location(config, client, path);
    if (full_path.size() == 0)
        return 404;

    return (code_check_delete(full_path));
}

std::string getHttpResponse(int statusCode) {
    std::string httpResponse;

    switch (statusCode) {
        case 200:
            httpResponse = "HTTP/1.1 200 OK\n";
            httpResponse += "Content-Type: application/json\n\n";
            httpResponse += "{\n";
            httpResponse += "  \"message\": \"Resource deleted successfully.\"\n";
            httpResponse += "}\n";
            break;
        case 204:
            httpResponse = "HTTP/1.1 204 No Content\n";
            break;
        case 403:
            httpResponse = "HTTP/1.1 403 Forbidden\n";
            break;
        case 404:
            httpResponse = "HTTP/1.1 404 Not Found\n";
            break;
        case 409:
            httpResponse = "HTTP/1.1 409 Conflict\n";
            break;
        case 500:
            httpResponse = "HTTP/1.1 500 Internal Server Error\n";
            break;
        default:
            httpResponse = "HTTP/1.1 500 Internal Server Error\n";
            break;
    }

    return httpResponse;
}
void send_Response(WBS::Client client, Config& config)
{
    int statusCode = code_DELETE(client, config);
    std::string httpResponse = getHttpResponse(statusCode);
    std::cout << httpResponse << std::endl;
    //still to write in the socket
}
