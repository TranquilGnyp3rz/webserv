#include "Server.hpp"
#include "Socket.hpp"

int main()
{
    Server server(8080);
    server.run();
    return 0;
}