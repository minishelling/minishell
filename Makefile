NAME = minishell

FLAGS = -g -Wall -Werror -Wextra

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

SRC_DIR = src
SRC_FILES = $(BUILTINS_FILES) \
			$(ENV_FILES) \
			$(FREE_FILES) \
			$(PRINT_FILES) \
			$(UTILS_FILES) \
			$(SRC_DIR)/main.c

BUILTINS_DIR = $(SRC_DIR)/builtins
BUILTINS_FILES = $(CD_FILES)

CD_DIR = $(BUILTINS_DIR)/cd
CD_FILES = $(CD_DIR)/cd.c \
			$(CD_DIR)/curpath.c \

ENV_DIR = $(SRC_DIR)/env
ENV_FILES = $(ENV_DIR)/env_free.c \
			$(ENV_DIR)/env_init.c \
			$(ENV_DIR)/env_print.c \
			$(ENV_DIR)/env_utils.c \

ERRORS_DIR = $(SRC_DIR)/errors
ERRORS_FILES = 

FREE_DIR = $(SRC_DIR)/free
FREE_FILES = $(FREE_DIR)/ft_free_2d.c \
			$(FREE_DIR)/ft_free_3d.c \
			$(FREE_DIR)/ft_free.c

PRINT_DIR = $(SRC_DIR)/print
PRINT_FILES = $(PRINT_DIR)/arrays.c \

UTILS_DIR = $(SRC_DIR)/utils
UTILS_FILES = $(UTILS_DIR)/errors.c \
			$(UTILS_DIR)/ft_strjoin_fs1.c \
			$(UTILS_DIR)/ft_strjoin_fs2.c \

INCLUDE_DIR = include
HEADER_FILES = builtins.h \
			env.h \
			errors.h \
			free.c \
			minishell.h \
			utils.h


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

run: all clean
	@clear
	@./minishell ..

.PHONY: clean fclean re
