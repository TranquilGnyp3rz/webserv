
#include "../../inc/config_file/Config_parsing.hpp"

int main(int ac, char **av, char **env)
{
    Config cf;
    if (ac == 2){
        cf.HandleConfigFile(av[1]);

    }
}