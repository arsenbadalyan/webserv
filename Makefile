# Main Variables
NAME     = webserv
COMPILER = c++
# CPPFLAGS = -Wall -Wextra -Werror -std=c++98
RM       = rm -rf
OUTPUT   = ./$(NAME)

# Main directories
SRC = src
OBJ = obj

# Folder directions
SRC_DIR := $(shell find $(SRC) -type d)
OBJ_DIR := $(foreach dir, $(SRC_DIR), $(addprefix $(OBJ), $(subst $(SRC), "", $(dir))))

# File directions
SRCS = $(shell find $(SRC) -type f -name '*.cpp')
OBJS = $(subst $(SRC), $(OBJ), $(SRCS:.cpp=.o))

# Libs
LIB_DIRS = $(shell find $(SRC) -type f -name '*.hpp')
LIBS = $(foreach libdir, $(LIB_DIRS), $(addprefix -I./, $(patsubst %/$(lastword $(subst /, ,$(libdir))),%,$(libdir))))

# Colors
RESET  = \033[0m
RED    = \033[31m
GREEN  = \033[32m
YELLOW = \033[33m
BLUE   = \033[34m

all: $(NAME)
	@echo > /dev/null

$(NAME): Makefile $(OBJS)
	@make WAIT_COMPILE_MSG
	@$(COMPILER) $(LIBS) -o $(NAME) $(OBJS) -g $(CPPFLAGS)
	@make DONE_MSG

$(OBJ)/%.o: $(SRC)/%.cpp $(LIB_DIRS)
	@mkdir -p $(OBJ) $(OBJ_DIR)
	@echo "${YELLOW}Compiling $<${RESET}"
	@$(COMPILER) $(CPPFLAGS) $(LIBS) -c $< -o $@

clean: DELETE_OBJ_MSG
	@$(RM) $(OBJ)

fclean: clean DELETE_PROGRAM_MSG
	@$(RM) $(NAME)

re: fclean all

# Helper messages
WAIT_COMPILE_MSG:
	@echo "${YELLOW}Please wait until program compiling...${RESET}"
DONE_MSG:
	@echo "${GREEN}! Webserv is ready !${RESET}"
DELETE_OBJ_MSG:
	@echo "${RED}Object files deleting...${RESET}"
DELETE_PROGRAM_MSG:
	@echo "${RED}! ATTENTION !\nWebserv is deleting...${RESET}"

.PHONY: all clean fclean re