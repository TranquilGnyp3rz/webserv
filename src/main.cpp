#include "WebServer.hpp"

int main(int argc, char *argv[]) {
    if (argc == 2 && !strcmp(argv[1], "-h"))
    {
        std::cout << "Usage: ./webserv [config_file]" << std::endl;
        std::cout << "config_file: path to the configuration file" << std::endl;
        std::cout << "Or use the default configuration located ./config/default.conf" << std::endl;
        exit(0);
    }
    if (argc == 1)
    {
        WebServer server("./config/default.conf");
        server.run();
        return 0;
    }
    if (argc != 2)
    {
        std::cout << "Wrong number of arguments" << std::endl;
        exit(1);
    }
    WebServer server(argv[1]);
    server.run();
    return 0;
}