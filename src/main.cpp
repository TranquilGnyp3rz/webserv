#include "WebServer.hpp"

int main(int argc, char *argv[]) {
    if (argc != 2)
    {
        std::cout << "No configurations's file is provided" << std::endl;
        exit(1);
    }
    WebServer server(argv[1]);
    server.run();
    return 0;
}