
NAME = minishell

FLAGS = -g

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

INCLUDE_DIR = include

HEADER_FILES = $(INCLUDE_DIR)/env.h \
			   $(INCLUDE_DIR)/errors.h \
			   $(INCLUDE_DIR)/minishell.h

SRC_DIR		= src
SRCS_FILES	= $(shell find $(SRC_DIR) -iname "*.c")

OBJ_DIR = obj
OBJECTS		:= $(SRCS_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADER_FILES)
	@mkdir -p $(@D)
	$(CC) $(FLAGS) -I$(INCLUDE_DIR) -c $< -o $@

$(NAME): $(OBJECTS) $(LIBFT)
	cc -o $(NAME) $(OBJECTS) $(LIBFT) -lreadline

clean:
	rm -f $(OBJECTS)
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) clean

re: fclean all

.PHONY: all clean fclean re
