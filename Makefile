NAME = minishell

FLAGS = -g -Wall -Werror -Wextra

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

SRC_DIR = src
SRC_FILES = $(ENV_FILES) $(PRINT_FILES) $(UTILS_FILES) \
			$(SRC_DIR)/main.c

ENV_DIR = $(SRC_DIR)/env
ENV_FILES = $(ENV_DIR)/env_free.c \
			$(ENV_DIR)/env_init.c \
			$(ENV_DIR)/env_print.c \
			$(ENV_DIR)/env_utils.c \

PRINT_DIR = $(SRC_DIR)/print
PRINT_FILES = $(PRINT_DIR)/arrays.c \

UTILS_DIR = $(SRC_DIR)/utils
UTILS_FILES = $(UTILS_DIR)/errors.c \

INCLUDE_DIR = include
HEADER_FILES = env.h \
			errors.h \
			minishell.h

OBJECTS = $(SRC_FILES:.c=.o)
OBJECTS_DIR = obj

all: $(NAME)

$(NAME): $(OBJECTS) $(LIBFT)
	cc -o $(NAME) $(FLAGS) $(OBJECTS) $(LIBFT)

$(OBJECTS): %.o:%.c
	cc $(FLAGS) -c $< -o $@

$(OBJECTS_DIR):
	@mkdir -p obj

$(LIBFT):
	@make -C $(LIBFT_DIR)

clean:
	rm -f $(OBJECTS)
	rm -fr $(OBJECTS_DIR)

fclean: clean
	rm -f $(NAME)

re: clean all

.PHONY: clean fclean re
