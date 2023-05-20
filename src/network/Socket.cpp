/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heloufra <heloufra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 20:41:10 by heloufra          #+#    #+#             */
/*   Updated: 2023/05/20 14:40:38 by heloufra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket()
{
    _domain = 0;
    _type = 0;
    _protocol = 0;
    _fd = 0;
    _port = 0;
}

Socket::Socket(Socket const & cpy)
{
    *this = cpy;
}

Socket & Socket::operator=(Socket const & cpy)
{
    if (this != &cpy)
    {
        _domain = cpy._domain;
        _type = cpy._type;
        _protocol = cpy._protocol;
        _fd = cpy._fd;
        _port = cpy._port;
    }
    return (*this);
}

Socket::Socket(int domain, int type, int protocol, int port)
{
    _domain = domain;
    _type = type;
    _protocol = protocol;
    _fd = socket(_domain, _type, _protocol);
    _port = port;
    if (_fd == -1)
        throw std::runtime_error("socket error");

    _address.sin_family = _domain;
    _address.sin_port = htons(port);
    _address.sin_addr.s_addr = INADDR_ANY;
    
    if (bind(_fd, (struct sockaddr *)& _address, sizeof( _address)) < 0)
        throw std::runtime_error("bind error");
}

void Socket::listenSocket(int backlog)
{
    if (listen(_fd, backlog) < 0)
        throw std::runtime_error("listen error");
}

int Socket::getFd() const
{
    return (_fd);
}

void Socket::setFd(int fd)
{
    _fd = fd;
}

int Socket::getPort() const
{
    return (_port);
}

struct sockaddr_in Socket::getAddress() const
{
    return (_address);
}

void Socket::setPort(int port)
{
    _port = port;
}

Socket::~Socket()
{
    close(_fd);
}
