
NAME = minishell

FLAGS = -Werror -Wall -Wextra -g

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

INCLUDE_DIR = include

HEADER_FILES = $(INCLUDE_DIR)/minishell.h

SRC_DIR		= src
SRCS_FILES	= $(shell find $(SRC_DIR) -iname "*.c")

OBJ_DIR = obj
OBJECTS		:= $(SRCS_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR) >/dev/null
	@echo "$(GREEN_LIGHT)Library libft created.$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADER_FILES)
	@mkdir -p $(@D)
	@$(CC) $(FLAGS) -I$(INCLUDE_DIR) -c $< -o $@
	@echo "$(GREEN)Compiling: $< into $@$(RESET)"

$(NAME): $(OBJECTS) $(LIBFT)
	@cc -o $(NAME) $(OBJECTS) $(LIBFT) -lreadline
	@echo "$(GREEN_LIGHT)Project successfully compiled. Target 'minishell' created.$(RESET)"


clean:
	@rm -f $(OBJECTS)
	@rm -rf $(OBJ_DIR)
	@make -C $(LIBFT_DIR) clean >/dev/null
	@echo "$(GOLD)Cleanup: Object files and binaries removed.$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) clean >/dev/null
	@echo "$(YELLOW)Cleanup: Build artifacts and temporary files removed.$(RESET)";

re: fclean all
	@echo "$(YELLOW)Recompilation complete: Project successfully rebuilt.$(RESET)"

.PHONY: all clean fclean re

GREEN=\033[0;32m
GREEN_LIGHT=\033[1;32m
YELLOW=\033[0;33m
GOLD=\033[38;5;220m
RESET=\033[0m
