#include "WebServer.hpp"

int main(int argc, char *argv[]) {
    if (argc != 2)
    {
        exit(1);
    }
    WebServer server(argv[1]);
    server.run();
    return 0;
}