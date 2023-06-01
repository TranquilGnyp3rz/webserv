# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: heloufra <heloufra@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/18 21:20:22 by heloufra          #+#    #+#              #
#    Updated: 2023/05/18 21:21:54 by heloufra         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CPP = c++
FLAGS = -Wall -Wextra -Werror -std=c++98
NAME = webserv

SOCKET = ./srcs/socket/Sockets.cpp


socket: $(SOCKET)
	$(CPP) $(FLAGS) $(SOCKET) -o socket
