/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heloufra <heloufra@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 17:16:37 by heloufra          #+#    #+#             */
/*   Updated: 2023/06/01 18:15:15 by heloufra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServer.hpp"
#include "ResourceHandler.hpp"

WebServer::WebServer(std::string config_file)
{
    _config.Handle_configFile(config_file);
    for (int i = 0; i < _config.get_servers().size(); i++)
    {
        std::cout << "server " << i << " listen on port " << _config.get_servers()[i].get_listen() << std::endl;
        if (std::find(_ports.begin(), _ports.end(), std::atoi(_config.get_servers()[i].get_listen().c_str())) == _ports.end())
             _ports.push_back(std::atoi(_config.get_servers()[i].get_listen().c_str()));
    }
}

int find_sockets(std::vector<SocketServer> sockets, int sock)
{
    std::vector<SocketServer>::iterator it;
    for (it = sockets.begin(); it != sockets.end(); it++)
    {
        if (it->get_sock() == sock){
             return it->get_port();
        }
           
    }
    return 1;
}

void WebServer::run()
{
    fd_set master_set;
    int max_sd, i;
    std::vector<SocketServer> sockets;
    FD_ZERO(&master_set);

    for (i =0 ; i < _ports.size(); i++)
    {
        sockets.push_back(SocketServer(AF_INET6, SOCK_STREAM, 0, _ports[i], INADDR_ANY));
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

void WebServer::accepter(std::vector<SocketServer> &sockets, fd_set *master_set, int *max_sd)
{
    int new_sd, rc, i;
    int desc_ready, end_Webserver = false;
    fd_set working_set, response_set;

    memset(&working_set, 0, sizeof(working_set));
    memset(&response_set, 0, sizeof(response_set));
    while (end_Webserver == false)
    {

        memcpy(&working_set, master_set, sizeof(*master_set));
        // std::cout << "Waiting on select()..." << std::endl;
        if (select_socket(&working_set, *max_sd, &rc, &response_set) == -1)
            break;
        desc_ready = rc;
        for (i = 3; i <= *max_sd && desc_ready > 0; i++)
        {
            if (FD_ISSET(i, &working_set))
            {
                desc_ready -= 1;
                if (find_socket(sockets, i) == 0)
                {
                    if (accept_socket(&working_set, i, max_sd, &new_sd, &end_Webserver, master_set, _clients, find_sockets(sockets, i)) == -1)
                        break;
                }
                else
                    handler(i, master_set, max_sd, &response_set);
            }
            if (FD_ISSET(i , &response_set))
            {
                std::cout << "Discriptor " << i << "is writeable" << std::endl;
                if (_clients.find(i)->second.response()) {
                    // remove(_clients.find(i)->second.get_body_file().c_str());
                    std::cout << "close socket " << i << std::endl;
                    FD_CLR(i, &response_set);
                    close(i);
                    _clients.erase(i);
                }
            }
        }
    }
}




void WebServer::handler(int i, fd_set *master_set, int *max_sd, fd_set *response_set)
{

    // std::cout << "Discriptor " << i << "is readale" << std::endl;

    int close_conn = false;
    int rc;
    char buffer[65536];

    rc = recv(i, buffer, sizeof(buffer), 0);
    if (rc < 0)
    {
        if (errno != EWOULDBLOCK)
        {
            perror("  recv() failed");
            close_conn = true;
        }
    }
    if (rc == 0)
    {

        std::cout << "Connection closed" << std::endl;
        close_conn = true;
    }
    // std::cout << "  " << rc << " bytes received" << std::endl;
     std::string body;
    std::string buf(buffer, rc);
    std::string::size_type pos;

    // _clients.find(i)->second.set_buffer(_clients.find(i)->second.get_buffer() + buf);

    pos = _clients.find(i)->second.get_buffer().find("\r\n\r\n");
    if (pos != std::string::npos)
    {
        _clients.find(i)->second.parse_request();
        
        if (_clients.find(i)->second.get_first_body() == false) {
            body = _clients.find(i)->second.get_buffer().substr(pos + 4);
            _clients.find(i)->second.set_first_body(true);
        }
        else {
            body = buf;
        }
        _clients.find(i)->second.save_body( body, close_conn);
    }

    if (close_conn)
    {
        std::cout << "  Connection closed "<< i << std::endl;
        FD_SET(i, response_set);
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



int WebServer::select_socket(fd_set *working_set, int max_sd, int *rc, fd_set *response_set)
{

    *rc = select(max_sd + 1, working_set, response_set, NULL, NULL);
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

int WebServer::accept_socket(fd_set *working_set, int i, int *max_sd, int *new_sd, int *end_Webserver, fd_set *master_set, std::map<int, Client> &clients, int ports)
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
        // ResourceHandler resource_handler(this->_config, );
        clients.insert(std::make_pair(*new_sd, Client(this->_config, ports, *new_sd)));
        FD_SET(*new_sd, master_set);
        if (*new_sd > *max_sd)
            *max_sd = *new_sd;
    }
    return 0;
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

int WebServer::find_client(std::vector<Client> _clients, int sock)
{

    for (int i = 0; i < _clients.size(); i++)
    {
        if (_clients[i].get_sock() == sock)
            return i;
    }
    return 0;
}
