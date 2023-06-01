/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heloufra <heloufra@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 23:20:25 by heloufra          #+#    #+#             */
/*   Updated: 2023/05/30 18:53:13 by heloufra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServer.hpp"


WebServer::WebServer(int port)
{
}

WebServer::WebServer(WebServer const & src)
{
    *this = src;
}

WebServer::~WebServer()
{

}

WebServer & WebServer::operator=(WebServer const & src)
{

    return *this;
}

void WebServer::run()
{

    std::vector<int> ports;
    fd_set master_set;
    ports.push_back(7000);
    ports.push_back(9088);
    int max_sd, i;
    std::vector<SocketServer> sockets;
    FD_ZERO(&master_set);
    for (i =0 ; i < ports.size(); i++)
    {
        sockets.push_back(SocketServer(AF_INET6, SOCK_STREAM, 0, ports[i], INADDR_ANY));
        std::cout << "socket = " << sockets[i].get_sock() << std::endl;
        max_sd = sockets[i].get_sock();
        FD_SET(sockets[i].get_sock(), &master_set);
    }
    accepter(sockets, &master_set, &max_sd);
    for (i = 0; i <= max_sd; ++i)
    {
        if (FD_ISSET(i, &master_set))
            close(i);
    }
}

// void WebServer::accepter()
// {
    
// }

// void WebServer::handler(int client_fd)
// {
//     int valread;
//     char buffer[1024] = {0};
//     valread = read(client_fd, buffer, 1024);
//     std::cout << buffer << std::endl;
//     responder(client_fd);
// }


int WebServer::select_socket(fd_set *working_set, int max_sd, int *rc)
{

    *rc = select(max_sd + 1, working_set, NULL, NULL, NULL);
    if (*rc < 0)
    {
        perror("  select() failed");
        return -1;
    }

    if (*rc == 0)
    {
        std::cout << "  select() timed out.  End program." << std::endl;
        return -1;
    }
    return 0;
}

int WebServer::accept_socket(fd_set *working_set, int i, int *max_sd, int *new_sd, int *end_Webserver, fd_set *master_set)
{
    std::cout << " Listening socket is readable" << std::endl;
    *new_sd = accept(i, NULL, NULL);
    if (*new_sd < 0)
    {
        if (errno != EWOULDBLOCK)
        {
            perror("  accept() failed");
            *end_Webserver = true;
        }
        return -1;
    }
    else
    {
        std::cout << "  New incoming connection - " << *new_sd << std::endl;
        FD_SET(*new_sd, master_set);
        if (*new_sd > *max_sd)
        {
            *max_sd = *new_sd;
        }
    }
    return 0;
}

void WebServer::handler(int i, fd_set *master_set, int *max_sd, std::map<int, Client> &clients)
{

    std::cout << "Discriptor " << i << "is readale" << std::endl;

    int close_conn = false;
    int rc, len;
    char buffer[20000];
    while (true)
    {
        rc = recv(i, buffer, sizeof(buffer), 0);
        if (rc < 0)
        {
            if (errno != EWOULDBLOCK)
            {
                perror("  recv() failed");
                close_conn = true;
            }
            break;
        }
        if (rc == 0)
        {

            std::cout << "  Connection closed" << std::endl;
            close_conn = true;
            break;
        }
        len = rc;
        std::cout << "  " << len << " bytes received" << std::endl;
        if (clients.find(i) == clients.end())
        {
            clients.insert(std::make_pair(i, Client(i)));
            clients.find(i)->second.set_buffer(buffer);
        }
        else
        {
            clients.find(i)->second.set_buffer(clients.find(i)->second.get_buffer() + buffer);
        }
        if (clients.find(i)->second.get_buffer().find("\r\n\r\n") != std::string::npos)
        {
            clients.find(i)->second.parse_request();
            std::cout << "method = " << clients.find(i)->second.get_request().method << std::endl;
            std::cout << "path = " << clients.find(i)->second.get_request().path << std::endl;
            std::cout << "http_version = " << clients.find(i)->second.get_request().http_version << std::endl;
            std::map<std::string, std::string>::const_iterator iter;
            const std::map<std::string, std::string> &headers = clients.find(i)->second.get_request().headers;
            for (iter = headers.begin(); iter != headers.end(); ++iter)
            {
                std::cout << iter->first << " : " << iter->second << std::endl;
            }
        }
        // response 
        rc = send(i, buffer, len, 0);

        if (rc < 0)
        {
            perror("  send() failed");
            close_conn = true;
            break;
        }
    }
    if (close_conn)
    {
        close(i);
        FD_CLR(i, master_set);
        if (i == *max_sd)
        {
            while (FD_ISSET(*max_sd, master_set) == false)
            {
                *max_sd -= 1;
            }
        }
    }
}

int WebServer::find_socket(std::vector<SocketServer> sockets, int sock)
{
    std::vector<SocketServer>::iterator it;
    for (it = sockets.begin(); it != sockets.end(); it++)
    {
        if (it->get_sock() == sock)
            return 0;
    }
    return 1;
}

int WebServer::find_client(std::vector<Client> clients, int sock)
{

    for (int i = 0; i < clients.size(); i++)
    {
        if (clients[i].get_sock() == sock)
            return i;
    }
    return 0;
}

void WebServer::accepter(std::vector<SocketServer> sockets, fd_set *master_set, int *max_sd)
{
    int new_sd, rc, i;
    int desc_ready, end_Webserver = false;
    fd_set working_set;
    std::map<int, Client> clients;
    while (end_Webserver == false)
    {

        memcpy(&working_set, master_set, sizeof(*master_set));
        std::cout << "Waiting on select()..." << std::endl;
        if (select_socket(&working_set, *max_sd, &rc) == -1)
            break;
        desc_ready = rc;
        for (i = 3; i <= *max_sd && desc_ready > 0; i++)
        {
            if (FD_ISSET(i, &working_set))
            {
                desc_ready -= 1;
                if (find_socket(sockets, i) == 0)
                {
                    if (accept_socket(&working_set, i, max_sd, &new_sd, &end_Webserver, master_set) == -1)
                        break;
                }
                else
                    handler(i, master_set, max_sd, clients);
            }
        }
    }
}
