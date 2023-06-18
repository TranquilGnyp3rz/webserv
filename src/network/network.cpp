/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   network.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhouya <akhouya@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 12:59:59 by akhouya           #+#    #+#             */
/*   Updated: 2023/06/09 11:35:36 by akhouya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/SocketServer.hpp"
#include "../../inc/Client.hpp"
#include "../../inc/request.hpp"
#include <fstream>
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
        std::cout << "  select() timed out. End program." << std::endl;
        return -1;
    }
    return 0;
}
int accept_socket(fd_set *working_set, int i, int *max_sd, int *new_sd, int *end_server, fd_set *master_set, std::map<int, WBS::Client> &clients, int ports)
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
        clients.insert(std::make_pair(*new_sd, WBS::Client(ports, *new_sd)));
        std::cout << "body lenght " << clients.find(*new_sd)->second.get_request().lenght_body<< std::endl;
        if (*new_sd > *max_sd)
        {
            *max_sd = *new_sd;
        }
    }
    return 0;
}
// void    save_body(std::string &buffer, t_request &request)
// {
//     // std::string::size_type pos = buffer.find("\r\n\r\n");
//     // if (pos != std::string::npos)
//     // {
//     //     body = buffer.substr(pos + 4);
//     //     buffer.erase(pos + 4);
//     //     len = buffer.length();
//     // }
    
// }
//function read from file
void read_from_file(const std::string filename)
{
    std::cout << "read from file" << std::endl;
    std::ifstream file(filename); 
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            // Process each line from the file
            std::cout << line << std::endl;
        }
        
        file.close();
    } else {
        std::cout << "Unable to open the file." << std::endl;
    }
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
            clients.find(i)->second.set_buffer(buffer);

        }
        else
        {
            clients.find(i)->second.set_buffer(clients.find(i)->second.get_buffer() + buffer);
        }

        if (clients.find(i)->second.get_buffer().find("\r\n\r\n") != std::string::npos)
        {
            std::string buf(buffer);
            clients.find(i)->second.parse_request();
            clients.find(i)->second.save_body( buf, len);
            std::cout << "method = " << clients.find(i)->second.get_request().method << std::endl;
            std::cout << "path = " << clients.find(i)->second.get_request().path << std::endl;
            std::cout << "http_version = " << clients.find(i)->second.get_request().http_version << std::endl;
            std::map<std::string, std::string>::const_iterator iter;
            const std::map<std::string, std::string> &headers = clients.find(i)->second.get_request().headers;
            
            for (iter = headers.begin(); iter != headers.end(); ++iter)
            {
                std::cout << iter->first << ":" << iter->second << std::endl;
            }
            
            //printf port
            std::cout << "port = " << clients.find(i)->second.get_port() << std::endl;
            // read content of file
            std::cout << "body_file = " << clients.find(i)->second.get_request().body_file << std::endl;
            read_from_file(clients.find(i)->second.get_request().body_file);
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
        clients.erase(i);
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
int find_sockets(std::vector<WBS::SocketServer> sockets, int sock)
{
    std::vector<WBS::SocketServer>::iterator it;
    for (it = sockets.begin(); it != sockets.end(); it++)
    {
        if (it->get_sock() == sock)
            return it->get_port();
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
                    if (accept_socket(&working_set, i, max_sd, &new_sd, &end_server, master_set, clients, find_sockets(sockets, i)) == -1)
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
