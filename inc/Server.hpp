/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heloufra <heloufra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 23:20:09 by heloufra          #+#    #+#             */
/*   Updated: 2023/05/20 14:35:49 by heloufra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP
# include <iostream>
# include <string>
# include <stdexcept>
# include <unistd.h>
# include <sys/types.h>
# include <sys/time.h>
# include <fcntl.h>
# include "Socket.hpp"


class Server
{
    private:
        Socket *_socket;
        void accepter();
        void handler(int client_fd);
        void responder(int client_fd);
    public:
        Server();
        ~Server();
        Server(Server const & src);
        Server(int port = 8080);
        Server & operator=(Server const & src);
        void run();
};

#endif