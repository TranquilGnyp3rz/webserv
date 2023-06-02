
#include "../../inc/config_file/Config_parsing.hpp"
#include "../../inc/code_delete/client.hpp"
#include "../../inc/code_delete/delete.hpp"

int main(int ac, char **av, char **env)
{
    (void)env;
    Config cf;
    if (ac == 2){
        cf.Handle_configFile(av[1]);
    }
    // std::cout << "done" << std::endl;

    WBS::Client client(1);
    send_Response(client, cf);
    return 0;
}