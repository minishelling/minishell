NAME = minishell
#BONUS_NAME = minishell_bonus

CC = cc
CFLAGS = -Wall -Wextra -Werror
#LIBFT_PTH = 

#LIBS = $(LIBFT_PTH)/libft.a

HDRS = -I./include -I$(LIBFT_PTH)/libft

CFILES = src/main.c
	
#BONUS_CFILES = 
	
SRC_DIR = src
#BONUS_SRC_DIR = bonus

OBJ_DIR = obj
#BONUS_OBJ_DIR = bonus_obj

OBJECTS = $(addprefix $(OBJ_DIR)/,$(notdir $(CFILES:%.c=%.o)))
#BONUS_OBJECTS = $(addprefix $(BONUS_OBJ_DIR)/,$(notdir $(BONUS_CFILES:%.c=%.o)))

all: $(NAME)
#bonus: $(BONUS_NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

#$(BONUS_OBJ_DIR):
#	@mkdir -p $(BONUS_OBJ_DIR)

#$(LIBFT_PTH)/libft.a:
#	@make -C $(LIBFT_PTH) >/dev/null
#	@echo "$(GREEN_LIGHT)Library libft created.$(RESET)"

$(NAME): $(LIBS) $(OBJ_DIR) $(OBJECTS)
	@$(CC) $(CFLAGS) $(OBJECTS) -o $(NAME)
#	@$(CC) $(CFLAGS) $(OBJECTS) $(LIBS) -o $(NAME)
	@echo "$(GREEN_LIGHT)Project successfully compiled. Target 'minishell' created.$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@$(CC) -c $(CFLAGS) $(HDRS) -o $@ $^
	@echo "$(GREEN)Compiling: $< into $@$(RESET)"

$(BONUS_NAME): $(LIBS) $(BONUS_OBJ_DIR) $(BONUS_OBJECTS)
	@$(CC) $(CFLAGS) $(BONUS_OBJECTS) -o $(BONUS_NAME)
#	@$(CC) $(CFLAGS) $(BONUS_OBJECTS) $(LIBS) -o $(BONUS_NAME)
	@echo "$(GREEN_LIGHT)Project successfully compiled. Target 'so_long_bonus' created.$(RESET)"

#$(BONUS_OBJ_DIR)/%.o: $(BONUS_SRC_DIR)/%.c | $(BONUS_OBJ_DIR)
	@$(CC) -c $(CFLAGS) $(HDRS) -o $@ $^
	@echo "$(GREEN)Compiling bonus: $< into $@$(RESET)"

clean:
	@rm -rf $(OBJ_DIR)
#	@rm -rf $(BONUS_OBJ_DIR)
#	@make -C $(LIBFT_PTH) clean >/dev/null
	@echo "$(GOLD)Cleanup: Object files and binaries removed.$(RESET)"

fclean: clean
	@rm -f $(NAME)
#	@rm -f $(BONUS_NAME)
#	@$(MAKE) -C $(LIBFT_PTH) fclean >/dev/null
	@echo "$(YELLOW)Cleanup: Build artifacts and temporary files removed.$(RESET)";

re: fclean all
	@echo "$(YELLOW)Recompilation complete: Project successfully rebuilt.$(RESET)"

GREEN=\033[0;32m
GREEN_LIGHT=\033[1;32m
YELLOW=\033[0;33m
GOLD=\033[38;5;220m
RESET=\033[0m

.PHONY: all clean fclean re #bonus
