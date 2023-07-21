NAME= webserv

CC = c++
FLAGS= -std=c++98 #-fsanitize=address #-Wall -Wextra -Werror 
PARA  = -c

SRC_DIR = ./src/
SRC_FILES = main.cpp Config.cpp Server.cpp Location.cpp  Client.cpp SocketServer.cpp WebServer.cpp ResourceHandler.cpp
SRC = $(addprefix $(SRC_DIR), $(SRC_FILES))

OBJ= $(SRC:.cpp=.o)

all: $(NAME)
	
$(NAME): $(OBJ)
		${CC} $(OBJ) -I./inc $(FLAGS) -o ${NAME} 

%.o : %.cpp 
	$(CC) $(PARA) -I./inc $(FLAGS) $<  -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all