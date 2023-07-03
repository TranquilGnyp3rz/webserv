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

// void read_from_file(const std::string filename)
// {
//     std::cout << "read from file" << std::endl;
//     std::ifstream file(filename); 
//     if (file.is_open()) {
//         std::string line;
//         while (std::getline(file, line)) {
//             // Process each line from the file
//             std::cout << line << std::endl;
//         }
        
//         file.close();
//     } else {
//         std::cout << "Unable to open the file." << std::endl;
//     }
// }
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
    fd_set master_set;
    int max_sd, i;
    std::vector<SocketServer> sockets;
    FD_ZERO(&master_set);
    for (i =0 ; i < _ports.size(); i++)
    {
        sockets.push_back(SocketServer(AF_INET6, SOCK_STREAM, 0, _ports[i], INADDR_ANY));
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
int find_sockets(std::vector<SocketServer> sockets, int sock)
{
    std::vector<SocketServer>::iterator it;
    for (it = sockets.begin(); it != sockets.end(); it++)
    {
        if (it->get_sock() == sock){
            // std::cout << it->get_port()<< std::endl;
             return it->get_port();
        }
           
    }
    return 1;
}
void WebServer::accepter(std::vector<SocketServer> sockets, fd_set *master_set, int *max_sd)
{
    int new_sd, rc, i;
    int desc_ready, end_Webserver = false;
    fd_set working_set, response_set;

    memset(&working_set, 0, sizeof(working_set));
    memset(&response_set, 0, sizeof(response_set));
    while (end_Webserver == false)
    {

        memcpy(&working_set, master_set, sizeof(*master_set));
        std::cout << "Waiting on select()..." << std::endl;
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
                // write and html msg to broweser in the socket
                responder(_clients.find(i)->second);
                // close the socket
                if (true) {
                    // remove(_clients.find(i)->second.get_body_file().c_str());
                    std::cout << "close socket " << i << std::endl;
                    FD_CLR(i, &response_set);
                    close(i);
                    _clients.erase(i);
                }
                
                // exit(0);
                // if (_clients.find(i) != _clients.end())
                // {
                //     _clients.find(i)->second.send_response(i);
                //     if (_clients.find(i)->second.get_response().empty())
                //     {
                //         FD_CLR(i, master_set);
                //         _clients.erase(i);
                //     }
                // }
            }
        }
    }
}

void WebServer::handler(int i, fd_set *master_set, int *max_sd, fd_set *response_set)
{

    std::cout << "Discriptor " << i << "is readale" << std::endl;

    int close_conn = false;
    // std::cout << "close conne first" << close_conn << std::endl;
    int rc, len;
    char buffer[65536];
    // while (true)
    // {
        rc = recv(i, buffer, sizeof(buffer), 0);
        std::cout << "rc = " << rc << std::endl;
        // if (rc == -1)
        // {
        //     close_conn = true;
        //     break;
        // }
        if (rc < 0)
        {
            if (errno != EWOULDBLOCK)
            {
                perror("  recv() failed");
                // close(i);
                close_conn = true;
            }
            // close_conn = true;
            // break;
        }
        if (rc == 0)
        {

            std::cout << "Connection closed" << std::endl;
            close_conn = true;
            // break;
        }
        len = rc;
        std::string body;
        std::cout << "  " << len << " bytes received" << std::endl;
        // while (i < 20)
        // {
        //     write (1, &buffer[i], 1);
        //     i++;
        // }
        std::string buf(buffer, rc);
        // std::cout << "buf = " << buf << std::endl;
        if (_clients.find(i) == _clients.end())
        {
            _clients.insert(std::make_pair(i, Client(i)));
            _clients.find(i)->second.set_buffer(buf);
        }
        else
        {
            
            _clients.find(i)->second.set_buffer(_clients.find(i)->second.get_buffer() + buf);
        }

        
        if (_clients.find(i)->second.get_buffer().find("\r\n\r\n") != std::string::npos)
        {
            // std::string buf(buffer);
            // std::cout << "-----------------------" << std::endl;
            // std::cout << _clients.find(i)->second.get_buffer() << std::endl;
            _clients.find(i)->second.parse_request();
            
            if (_clients.find(i)->second.get_first_body() == false) {
                std::string::size_type pos = _clients.find(i)->second.get_buffer().find("\r\n\r\n");
                body = _clients.find(i)->second.get_buffer().substr(pos + 4);
                _clients.find(i)->second.set_first_body(true);
                // if (body.find("\r\n") != std::string::npos)
                // {
                //     _clients.find(i)->second.set_body(body);
                //     return ;
                // }
                
                
            }
            else {
                body = buf;
                // std::cout << "----"<<body << std::endl;
                // exit(0);
                
            }
            
            // body = body.erase(0, 2);
            // ;std::cout << "---------body = --------" << body << std::endl;

            
            // std::cout << "---------close_conn = --------" << close_conn << std::endl;
            // if (_clients.find(i)->second.get_body() != "")
            // {
            //     body = _clients.find(i)->second.get_body() + body;
            //     _clients.find(i)->second.set_body("");
            // }
            _clients.find(i)->second.save_body( body, close_conn);
            // std::cout << "method = " << _clients.find(i)->second.get_request().method << std::endl;

            // std::cout << "path = " << _clients.find(i)->second.get_request().path << std::endl;
            // std::cout << "http_version = " << _clients.find(i)->second.get_request().http_version << std::endl;

            // std::map<std::string, std::string>::const_iterator iter;
            // const std::map<std::string, std::string> &headers = _clients.find(i)->second.get_request().headers;
            // // close_conn = true;
            // for (iter = headers.begin(); iter != headers.end(); ++iter)
            // {
            //     std::cout << iter->first << " : " << iter->second << std::endl;
            // }
            
            
        }
        // rc = responder(_clients.find(i)->second);


        // if (rc < 0)
        // {
        //     perror("  send() failed");
            
        //     close_conn = true;
        //     // break;
        // }
        // std::cout << "buffer = " << sizeof(buffer) << std::endl;
        
    // }
    // std::cout << "buffer = " <<  << std::endl;

    if (close_conn)
    {
        // close(i);
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
    // exit(0);
}

int WebServer::responder(Client &client)
{
    client.respond();
    return 0;
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
        clients.insert(std::make_pair(*new_sd, Client(ports, *new_sd)));

        std::cout << clients.find(*new_sd)->second.get_body_file() << std::endl;
        // std::cout << "body lenght " << clients.find(*new_sd)->second.get_request().lenght_body<< std::endl;
        FD_SET(*new_sd, master_set);
        if (*new_sd > *max_sd)
        {
            *max_sd = *new_sd;
        }
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


