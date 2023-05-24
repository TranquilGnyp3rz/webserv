
#include "../../inc/config_file/Config_parsing.hpp"

int main(int ac, char **av, char **env)
{
    (void)env;
    Config cf;
    if (ac == 2){
        cf.HandleConfigFile(av[1]);
    }
    std::cout << "done" << std::endl;
    return 0;
}