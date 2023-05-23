/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akhouya <akhouya@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 20:48:24 by heloufra          #+#    #+#             */
/*   Updated: 2023/05/21 11:49:10 by akhouya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#ifndef SOCKET_HPP
# define SOCKET_HPP
# include <iostream>
# include <sys/socket.h>
# include <netinet/in.h>
# include <unistd.h>
# include "macros.hpp"

class Socket
{
    private:
        int _domain;
        int _type;
        int _protocol;
        int _fd;
        int _port;
        struct sockaddr_in _address;
    public:
        Socket();
        Socket(Socket const & cpy);
        Socket & operator=(Socket const & cpy);
        Socket(int domain, int type, int protocol, int port);
        ~Socket();
        int getFd() const;
        void setFd(int fd);
        int getPort() const;
        void setPort(int port);
        struct sockaddr_in getAddress() const;
        void listenSocket(int backlog);
};
#endif