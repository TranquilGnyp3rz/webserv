/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   network.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhouya <akhouya@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 12:59:59 by akhouya           #+#    #+#             */
/*   Updated: 2023/05/26 09:51:13 by akhouya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/SocketServer.hpp"
#include "../../inc/Client.hpp"
#include <vector>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

void print_fd_set(fd_set *set)
{
    int max_fd = FD_SETSIZE;

    printf("File descriptors in the set:\n");
    for (int fd = 0; fd < max_fd; ++fd)
    {
        if (FD_ISSET(fd, set))
        {
            printf("%d ", fd);
        }
    }
    printf("\n");
}
int select_socket(fd_set *working_set, int max_sd, int *rc)
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
int accept_socket(fd_set *working_set, int i, int *max_sd, int *new_sd, int *end_server, fd_set *master_set)
{
    std::cout << " Listening socket is readable" << std::endl;
    *new_sd = accept(i, NULL, NULL);
    if (*new_sd < 0)
    {
        if (errno != EWOULDBLOCK)
        {
            perror("  accept() failed");
            *end_server = true;
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

void readable_sock(int i, fd_set *master_set, int *max_sd, std::map<int, WBS::Client> &clients)
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
            clients.insert(std::make_pair(i, WBS::Client(i)));
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
// find a integer in socketserver class by _sock
int find_socket(std::vector<WBS::SocketServer> sockets, int sock)
{
    std::vector<WBS::SocketServer>::iterator it;
    for (it = sockets.begin(); it != sockets.end(); it++)
    {
        if (it->get_sock() == sock)
            return 0;
    }
    return 1;
}

int find_client(std::vector<WBS::Client> clients, int sock)
{

    for (int i = 0; i < clients.size(); i++)
    {
        if (clients[i].get_sock() == sock)
            return i;
    }
    return 0;
}
void multiplexing(std::vector<WBS::SocketServer> sockets, fd_set *master_set, int *max_sd)
{
    int new_sd, rc, i;
    int desc_ready, end_server = false;
    fd_set working_set;
    std::map<int, WBS::Client> clients;
    while (end_server == false)
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
                    if (accept_socket(&working_set, i, max_sd, &new_sd, &end_server, master_set) == -1)
                        break;
                }
                else
                    readable_sock(i, master_set, max_sd, clients);
            }
        }
    }
}

int main()
{
    std::vector<int> ports;
    fd_set master_set;
    ports.push_back(7000);
    ports.push_back(9088);
    int max_sd, i;
    std::vector<WBS::SocketServer> sockets;
    FD_ZERO(&master_set);
    for (i = 0; i < ports.size(); i++)
    {
        sockets.push_back(WBS::SocketServer(AF_INET6, SOCK_STREAM, 0, ports[i], INADDR_ANY));
        std::cout << "socket = " << sockets[i].get_sock() << std::endl;
        max_sd = sockets[i].get_sock();
        FD_SET(sockets[i].get_sock(), &master_set);
    }
    multiplexing(sockets, &master_set, &max_sd);
    for (i = 0; i <= max_sd; ++i)
    {
        if (FD_ISSET(i, &master_set))
            close(i);
    }
}
