#include "../../inc/tmp/request.hpp"
#include "../../inc/tmp/client.hpp"

int method_DELETE(Client client)
{
    // std::string root = server.get_root();
    // std::string full_path = root + path;
    std::cout << "full_path: " << full_path << std::endl;
    if (remove(full_path.c_str()) != 0)
    {
        std::cout << "Error deleting file" << std::endl;
        return 1;
    }
    else
    {
        std::cout << "File successfully deleted" << std::endl;
        return 0;
    }
}