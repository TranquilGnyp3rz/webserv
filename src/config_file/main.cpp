
#include "../../inc/config_file/Config_parsing.hpp"
#include "../../inc/tmp/client.hpp"
#include "../../inc/tmp/delete.hpp"

int main(int ac, char **av, char **env)
{
    (void)env;
    Config cf;
    if (ac == 2){
        cf.Handle_configFile(av[1]);
    }
    std::cout << "done" << std::endl;
    WBS::Client client(1);
    int i = method_DELETE(client, cf);
    std::cout << i << std::endl;
    return 0;
}