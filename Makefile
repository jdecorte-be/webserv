NAME		= webserv

INCS_PATH	= ./includes
OBJ_DIR		= .obj

SRCS		= main.cpp \
			  client/client.cpp \
			  parsing/parse_conf.cpp \
			  parsing/requete.cpp \
			  parsing/servers.cpp \
			  parsing/conf.cpp \
			  server/server.cpp \
			  server/socket.cpp \
			  utils/utils_server.cpp \
			  cgi/cgi.cpp

OBJS		= $(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o))

CXX			= clang++
CXXFLAGS	= -Wall -Werror -Wextra -std=c++98

# Colors
GREEN		= \033[1;32m
CYAN		= \033[1;36m
YELLOW		= \033[1;33m
RED			= \033[1;31m
RESET		= \033[0m

TOTAL		= $(words $(SRCS))
COUNT		= 0

all:		$(NAME)

$(NAME):	$(OBJS)
			@$(CXX) $(CXXFLAGS) -I $(INCS_PATH) -o $(NAME) $(OBJS)
			@printf "$(GREEN)✔ $(NAME) compiled successfully!$(RESET)\n"

$(OBJ_DIR)/%.o: %.cpp
			@mkdir -p $(dir $@)
			$(eval COUNT=$(shell echo $$(($(COUNT)+1))))
			@printf "$(CYAN)[$(COUNT)/$(TOTAL)]$(RESET) Compiling $(YELLOW)$<$(RESET)\n"
			@$(CXX) $(CXXFLAGS) -I $(INCS_PATH) -c $< -o $@

clean:
			@rm -rf $(OBJ_DIR)
			@printf "$(RED)✗ Object files removed$(RESET)\n"

fclean:		clean
			@rm -f $(NAME)
			@printf "$(RED)✗ $(NAME) removed$(RESET)\n"

re:			fclean all

.PHONY:		all clean fclean re