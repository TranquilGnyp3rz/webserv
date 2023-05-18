NAME= webserv

SRC = ./src/config_file/main.cpp ./src/config_file/Config_parsing.cpp ./src/config_file/Server.cpp ./src/config_file/Location.cpp \

CC = c++
FLAGS= -Wall -Werror -Wextra -std=c++98
PARA  = -c 

OBJ= $(SRC:.cpp=.o)

%.o : %.cpp 
	@$(CC) $(PARA) $(FLAGS) $<  -o $@

all: $(NAME)
	
$(NAME): $(OBJ)
		@${CC} $(OBJ) $(FLAGS) -o ${NAME} 

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all