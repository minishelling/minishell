NAME = minishell

FLAGS = -g

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

SRC_DIR = src
ENV_DIR = $(SRC_DIR)/env
ENV_VAR_INIT_SHELL_DIR = $(SRC_DIR)/env_var_init_shell
PRINT_DIR = $(SRC_DIR)/print
UTILS_DIR = $(SRC_DIR)/utils
LEXER_PARSER_DIR = $(SRC_DIR)/lexer_parser
INCLUDE_DIR = include

SRC_FILES = $(SRC_DIR)/main.c \
			$(LEXER_PARSER_FILES) \
			$(ENV_VAR_INIT_SHELL_FILES) \
			$(PRINT_FILES) \
			$(UTILS_FILES) \
			$(ENV_FILES) 

ENV_FILES = $(ENV_DIR)/env_init.c \
			$(ENV_DIR)/env_utils.c \
			$(ENV_DIR)/env_free.c \
			$(ENV_DIR)/env_print.c 

ENV_VAR_INIT_SHELL_FILES =  $(ENV_VAR_INIT_SHELL_DIR)/init_shell.c \
							$(ENV_VAR_INIT_SHELL_DIR)/env_var_list.c \
							$(ENV_VAR_INIT_SHELL_DIR)/env_var.c
							
PRINT_FILES = $(PRINT_DIR)/arrays.c

UTILS_FILES =	$(UTILS_DIR)/list_token_utils.c \
				$(UTILS_DIR)/print_utils.c \
				$(UTILS_DIR)/errors.c \
				$(UTILS_DIR)/free.c

LEXER_PARSER_FILES = $(LEXER_PARSER_DIR)/lexer_parser.c \
                     $(LEXER_PARSER_DIR)/lexer.c \
                     $(LEXER_PARSER_DIR)/lexer_jt_funcs.c \
                     $(LEXER_PARSER_DIR)/syntaxer.c \
                     $(LEXER_PARSER_DIR)/syntaxer_jt_funcs.c \
                     $(LEXER_PARSER_DIR)/expander.c

HEADER_FILES = $(INCLUDE_DIR)/env.h \
			   $(INCLUDE_DIR)/errors.h \
			   $(INCLUDE_DIR)/minishell.h

OBJECTS = $(SRC_FILES:.c=.o)

all: $(NAME)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJECTS) $(LIBFT)
	cc -o $(NAME) $(OBJECTS) $(LIBFT) -lreadline

%.o: %.c $(HEADER_FILES)
	cc $(FLAGS) -I$(INCLUDE_DIR) -c $< -o $@

clean:
	rm -f $(OBJECTS)

fclean: clean
	rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) clean

re: fclean all

.PHONY: all clean fclean re
