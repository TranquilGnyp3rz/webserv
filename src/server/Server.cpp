/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heloufra <heloufra@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 23:20:25 by heloufra          #+#    #+#             */
/*   Updated: 2023/05/27 17:58:37 by heloufra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server()
{
    _socket = new Socket(AF_INET, SOCK_STREAM, 0, 8080);
}

Server::Server(int port)
{
    _socket = new Socket(AF_INET, SOCK_STREAM, 0, port);
}

Server::Server(Server const & src)
{
    *this = src;
}

Server::~Server()
{
    delete _socket;
}

Server & Server::operator=(Server const & src)
{
    if (this != &src)
    {
        _socket = src._socket;
    }
    return *this;
}

void Server::run()
{
    _socket->listenSocket(10);
    std::cout << "Server is listening on port " << _socket->getPort() << std::endl;
    accepter();
}

void Server::accepter()
{
    struct sockaddr_in address;
    int new_socket;
    int addrlen = sizeof(_socket->getAddress());
    
    address = _socket->getAddress();
    while (1)
    {
        if ((new_socket = accept(_socket->getFd(), (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        handler(new_socket);
    }
}

void Server::handler(int client_fd)
{
    int valread;
    char buffer[1024] = {0};
    valread = read(client_fd, buffer, 1024);
    std::cout << buffer << std::endl;
    responder(client_fd);
}

void Server::responder(int client_fd)
{
    std::string hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
    send(client_fd, hello.c_str(), hello.length(), 0);
    std::cout << "Hello message sent" << std::endl;
    close(client_fd);
}

