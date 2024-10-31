NAME = minishell

FLAGS = -Werror -Wall -Wextra -g

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

INCLUDE_DIR = include

HEADER_FILES = $(INCLUDE_DIR)/minishell.h

SRC_DIR		= src
SRC_FILES = $(SRC_DIR)/main.c \
				$(SRC_DIR)/signals.c \
				$(BUILTINS_FILES) \
				$(CD_FILES) \
				$(ENV_FILES) \
				$(ERROR_HANDLING_FILES) \
				$(EXECUTOR_FILES) \
				$(FREE_FILES) \
				$(PARSER_FILES) \
				$(LEXER_JT_FUNCS_FILES) \
				$(SYNTAXER_JT_FUNCS_FILES) \
				$(PARSER_JT_FUNCS_FILES) \
				$(PARSER_UTILS_FILES) \
				$(PRINT_UTILS_FILES) \
				$(UTILS_FILES)

BUILTINS_DIR = $(SRC_DIR)/builtins
BUILTINS_FILES = $(BUILTINS_DIR)/declare.c \
				$(BUILTINS_DIR)/echo.c \
				$(BUILTINS_DIR)/env.c \
				$(BUILTINS_DIR)/exit.c \
				$(BUILTINS_DIR)/export.c \
				$(BUILTINS_DIR)/pwd.c \
				$(BUILTINS_DIR)/unset.c

CD_DIR = $(BUILTINS_DIR)/cd
CD_FILES = $(CD_DIR)/cd_utils.c \
			$(CD_DIR)/cd.c \
			$(CD_DIR)/chdir_cdpath.c \
			$(CD_DIR)/chdir_default.c \
			$(CD_DIR)/chdir_functions.c

ENV_DIR = $(SRC_DIR)/env
ENV_FILES = $(ENV_DIR)/env_count.c \
			$(ENV_DIR)/env_create_array.c \
			$(ENV_DIR)/env_free.c \
			$(ENV_DIR)/env_init.c \
			$(ENV_DIR)/env_print.c \
			$(ENV_DIR)/env_update.c \
			$(ENV_DIR)/env_utils.c \
			$(ENV_DIR)/env_utils2.c

ERROR_HANDLING_DIR = $(SRC_DIR)/error_handling
ERROR_HANDLING_FILES = $(ERROR_HANDLING_DIR)/clean.c \
						$(ERROR_HANDLING_DIR)/handle_builtin_err.c \
						$(ERROR_HANDLING_DIR)/handle_parsing_err.c

EXECUTOR_DIR = $(SRC_DIR)/executor
EXECUTOR_FILES = $(EXECUTOR_DIR)/execute_builtins.c \
				$(EXECUTOR_DIR)/executor_utils.c \
				$(EXECUTOR_DIR)/executor.c \
				$(EXECUTOR_DIR)/get_cmd_path.c \
				$(EXECUTOR_DIR)/pipe_subtree.c \
				$(EXECUTOR_DIR)/redirections.c

FREE_DIR = $(SRC_DIR)/free
FREE_FILES = $(FREE_DIR)/ft_free_2d.c \
			$(FREE_DIR)/ft_free.c

PARSER_DIR = $(SRC_DIR)/parser
PARSER_FILES = $(PARSER_DIR)/appender.c \
				$(PARSER_DIR)/AST_build_tree.c \
				$(PARSER_DIR)/AST_handle_parens.c \
				$(PARSER_DIR)/expand_env_var.c \
				$(PARSER_DIR)/expander.c \
				$(PARSER_DIR)/hdoc_read_input.c \
				$(PARSER_DIR)/hdocs.c \
				$(PARSER_DIR)/lexer.c \
				$(PARSER_DIR)/parser.c \
				$(PARSER_DIR)/syntaxer.c \
				$(PARSER_DIR)/traverse_and_execute.c

JUMP_TABLES_FUNCS_DIR = $(PARSER_DIR)/jump_tables_funcs

LEXER_JT_FUNCS_DIR = $(JUMP_TABLES_FUNCS_DIR)/lexer_jt_funcs
LEXER_JT_FUNCS_FILES = $(LEXER_JT_FUNCS_DIR)/lexer_jt1.c \
						$(LEXER_JT_FUNCS_DIR)/lexer_jt2.c

PARSER_JT_FUNCS_DIR = $(JUMP_TABLES_FUNCS_DIR)/parser_jt_funcs
PARSER_JT_FUNCS_FILES = $(PARSER_JT_FUNCS_DIR)/parser_jt1.c

SYNTAXER_JT_FUNCS_DIR = $(JUMP_TABLES_FUNCS_DIR)/syntaxer_jt_funcs
SYNTAXER_JT_FUNCS_FILES = $(SYNTAXER_JT_FUNCS_DIR)/syntaxer_jt1.c \
							$(SYNTAXER_JT_FUNCS_DIR)/syntaxer_jt2.c \
							$(SYNTAXER_JT_FUNCS_DIR)/syntaxer_jt3.c 


PARSER_UTILS_DIR = $(PARSER_DIR)/parser_utils
PARSER_UTILS_FILES = $(PARSER_UTILS_DIR)/cmd_and_redir_utils.c \
					$(PARSER_UTILS_DIR)/free_tokens_and_cmds.c \
					$(PARSER_UTILS_DIR)/free_tree.c \
					$(PARSER_UTILS_DIR)/get_token_utils.c \
					$(PARSER_UTILS_DIR)/token_list_utils.c \
					$(PARSER_UTILS_DIR)/token_str_utils.c

PRINT_UTILS_DIR = $(SRC_DIR)/print_utils
PRINT_UTILS_FILES = $(PRINT_UTILS_DIR)/print_cmd.c \
					$(PRINT_UTILS_DIR)/print_tokens_n_tree.c

UTILS_DIR = $(SRC_DIR)/utils
UTILS_FILES = $(UTILS_DIR)/append_suffix.c \
				$(UTILS_DIR)/close_utils.c \
				$(UTILS_DIR)/ft_is_natural.c \
				$(UTILS_DIR)/ft_str_count.c \
				$(UTILS_DIR)/ft_strjoin_arr.c \
				$(UTILS_DIR)/ft_strjoin_fs1.c \
				$(UTILS_DIR)/ft_strjoin_fs2.c \
				$(UTILS_DIR)/max_len.c \
				$(UTILS_DIR)/print_arrays.c


OBJ_DIR = obj
OBJECTS		:= $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

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
