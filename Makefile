NAME= webserv

SRC = ./src/main.cpp ./src/config/Config_parsing.cpp ./src/config/Server.cpp ./src/config/Location.cpp  ./src/network/Client.cpp ./src/network/SocketServer.cpp ./src/server/WebServer.cpp ./src/respond/ResourceHandler.cpp

CC = c++
FLAGS= -std=c++98 #-fsanitize=address -g3
PARA  = -c 

OBJ= $(SRC:.cpp=.o)

all: $(NAME)
	
$(NAME): $(OBJ)
		@${CC} $(OBJ) -I./inc $(FLAGS) -o ${NAME} 
%.o : %.cpp 
	@$(CC) $(PARA) -I./inc $(FLAGS) $<  -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all
