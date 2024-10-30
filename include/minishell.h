#ifndef MINISHELL_H
# define MINISHELL_H

# define _GNU_SOURCE

# include <stdint.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <signal.h>
# include <stddef.h>
# include <sys/wait.h>
# include <sys/time.h>
# include <errno.h>
# include <linux/limits.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <string.h>
# include <dirent.h>
# include "../libft/libft.h"
# include <assert.h>
# include <sys/types.h>
# include <sys/stat.h>

# define MINISHARED_PROMPT \
"\001\033[38;5;93m\002M\001\033[38;5;99m\002i\001\033[38;5;111m\002n\
\001\033[38;5;63m\002i\001\033[38;5;75m\002_\001\033[38;5;81m\002s\
\001\033[38;5;118m\002h\001\033[38;5;154m\002a\001\033[38;5;190m\002r\
\001\033[38;5;226m\002e\001\033[38;5;214m\002d\001\033[0m\002: \
\001\033[0m\002"

# define ERR_PROMPT "Mini_shared: "

# define META_CHARS_PLUS_SET " \t\n|&()<>\'\"$"
# define ERROR -1
# define OFF 0
# define ON 1

# define RESET_COLOR "\033[0m"
# define MAGENTA_TEXT "\033[0;35m"
# define WHITE_TEXT "\033[0;37m"
# define MAGENTA_BACKGROUND "\033[45m"
# define WHITE_BACKGROUND "\033[47m"

typedef enum signal_mode
{
	INTERACTIVE,
	PARENT_NON_INTERACTIVE,
	BLOCK,
	CHILD_NON_INTERACTIVE,
	PARENT_IGNORE,
	PARENT_HEREDOC,
	CHILD_HEREDOC,
}	t_signal_mode;

typedef enum exit_code
{
	EXIT_SUCCESSFULLY = 0,
	EXIT_FAIL = 1,
	EXIT_NUM_ARG_REQ = 2,
	EXIT_SYNTAX_ERROR = 2,
	EXIT_MEM_FAILURE = 3,
	EXIT_CMD_NOT_EXECUTABLE = 126,
	EXIT_CMD_NOT_FOUND = 127,
	EXIT_SIGNAL_CODE = 128,
	EXIT_SIGINT = 130,
	EXIT_SIGQUIT = 131
}	t_exit_code;

typedef enum e_codes
{
	INIT = -1,
	SUCCESS = 0,
	FAILURE,
	SYNTAX_FAILURE,
	MALLOC_ERROR,
	PROCEED,
	NULL_ERROR,
	NULL_ENV,
	NULL_NODE,
	NULL_STRING,
	COUNT
}	t_ecode;

typedef enum e_parsing_codes
{
	PARSING_OK,
	ERR_CMD_SUBSTIT,
	ERR_SYNTAX_NL,
	ERR_UNCLOSED_QUOTES,
	ERR_SYNTAX_OPEN_PAR,
	ERR_SYNTAX_CLOSE_PAR,
	ERR_SYNTAX_PIPE,
	ERR_SYNTAX_OR,
	ERR_SYNTAX_AND,
	ERR_SYNTAX_REDIR,
	ERR_SYNTAX_AMPER,
	ERR_SYNTAX_ERROR,
	ERR_BG_PROCESS,
	ERR_PARSING_ERROR,
	ERR_MEM,
	ERR_EXPAND,
	ERR_CMD,
	SIGINT_HDOC,
}	t_ecode_p;

typedef enum e_token_id
{
	SPACE_CHAR,
	TAB_CHAR,
	NL,
	PIPE,
	AND_OPR,
	PAR_OPEN,
	PAR_CLOSE,
	LT,
	GT,
	SQUOTE,
	DQUOTE,
	ENV_VAR,
	WORD,
	OR_OPR,
	AMPERSAND,
	ARITH_EXPAN
}	t_token_id;

typedef enum e_redir_id
{
	REDIR = 0,
	IN,
	HERE,
	OUT,
	APP
}	t_redir_id;

typedef enum e_builtin
{
	ECHO,
	CD,
	PWD,
	EXPORT,
	DECLARE,
	UNSET,
	ENV,
	EXIT,
	NON_BUILTIN,
	BUILTIN_COUNT
}	t_builtin;

typedef struct s_curpath
{
	char				*dir;
	struct s_curpath	*previous;
	struct s_curpath	*next;
}	t_curpath;

typedef struct s_redir
{
	t_redir_id		redir_id;
	int				fd;
	char			*file;
	struct s_redir	*next;
}					t_redir;

typedef struct s_token
{
	t_token_id		id;
	char			*str;
	struct s_token	*next;
}	t_token;

typedef struct s_cmd
{
	char			**args;
	t_redir			*redir;
	int				latest_in;
	int				latest_out;
	struct s_cmd	*next;
}					t_cmd;

typedef struct s_env
{
	char			*key;
	char			*value;
	char			*keyvalue;
	struct s_env	*next;
}	t_env;

typedef enum e_tree_type
{
	T_AND_OPR,
	T_OR_OPR,
	CMD,
	T_PIPE
}	t_tree_type;

typedef struct s_tree
{
	struct s_tree	*left;
	struct s_tree	*right;
	t_tree_type		type;
	t_token			*start_token;
	t_token			*end_token;
	t_cmd			*cmd;
}	t_tree;

typedef struct s_shell
{
	pid_t		parent;
	uint8_t		exit_code;
	char		*input;
	t_token		*token;
	t_tree		*tree;
	t_env		*env_list;
	bool		debug_mode;
}	t_shell;

extern int	g_signalcode;

// ================ FUNCTION POINTERS ================ //

typedef t_ecode_p \
			(*t_lexer_func)(char *str, size_t *pos, t_token_id *token_id);
typedef t_ecode_p \
			(*t_syntax_func)(t_token *prev, t_token *cur, int *par_count);
typedef t_ecode_p \
			(*t_parser_func)(t_cmd *cmd, t_token *token);

// ================ TOKENIZATION ================ //

t_ecode_p	tokenize(t_shell *shell, char *input);
t_token		*new_token(void);
t_token_id	get_token_id(char c);
void		add_token_in_back(t_token **t_list, t_token *new);
t_token		*copy_token(t_token *t_node);
t_token		*last_token(t_token *token_list_head);
t_token		*skip_whitespace_and_get_next_token(t_token *token);
t_token		*get_after_matching_arith_expan(t_token *token);
void		remove_space_tokens(t_token **list, t_token *prev_token);

// TOKENIZATION JUMP TABLE

t_ecode_p	advance_pos_quote(char *str, size_t *pos, t_token_id *token_id);
t_ecode_p	advance_pos_parens(char *str, size_t *pos, t_token_id *token_id);
t_ecode_p	advance_pos_redir(char *str, size_t *pos, t_token_id *token_id);
t_ecode_p	advance_pos_env_var(char *str, size_t *pos, t_token_id *token_id);
t_ecode_p	advance_pos_pipe(char *str, size_t *pos, t_token_id *token_id);
t_ecode_p	advance_pos_space_or_word(char *str, size_t *pos, \
	t_token_id *token_id);
t_ecode_p	advance_pos_and_operator(char *str, size_t *pos, \
	t_token_id *token_id);

// ================ SYNTAX ================ //

t_ecode_p	syntax(t_shell *shell);

// SYNTAX JUMP TABLE
t_ecode_p	syntax_pipe(t_token *prev_token, t_token *cur_token, \
	int *par_count);
t_ecode_p	syntax_and_opr(t_token *prev_token, t_token *cur_token, \
	int *par_count);
t_ecode_p	syntax_open_paren(t_token *prev_token, t_token *cur_token, \
	int *par_count);
t_ecode_p	syntax_close_paren(t_token *prev_token, t_token *cur_token, \
	int *par_count);
t_ecode_p	syntax_redir(t_token *prev_token, t_token *cur_token, \
	int *par_count);
t_ecode_p	syntax_noop(t_token *prev_token, t_token *cur_token, \
	int *par_count);
t_ecode_p	syntax_word(t_token *prev_token, t_token *cur_token, \
	int *par_count);
t_ecode_p	syntax_quote(t_token *prev_token, t_token *cur_token, \
	int *par_count);
t_ecode_p	syntax_or_opr(t_token *prev_token, t_token *cur_token, \
	int *par_count);
t_ecode_p	syntax_env_var(t_token *prev_token, t_token *cur_token, \
	int *par_count);
t_ecode_p	syntax_ampersand(t_token *prev_token, t_token *cur_token, \
	int *par_count);

// ================ APPENDING ================ //

t_ecode_p	append(t_shell *shell);

// ================ HDOC ================ //

t_ecode_p	handle_hdocs(t_shell *shell, t_token *token_list);
t_ecode_p	read_hdoc_input(t_shell *shell, const char *file_name, \
	const char **delimiter);

// ================ ABSTRACT SYNTAX TREE ================ //

t_tree		*make_tree(t_shell *shell, t_token *start_token, \
	t_token *end_token);
t_tree		*init_leaf_node(t_shell *shell, t_token *start_token, \
	t_token *end_token);
t_tree		*init_tree_node(t_shell *shell, t_token *op_token);
t_token		*get_matching_paren(t_token *start_token);
t_token		*ignore_first_parens(t_token *start_token, t_token **end_token);
t_token		*non_null_previous(t_token *list, t_token *before_what);
t_tree		*process_arith_expan(t_shell *shell, t_token *start_token, \
	t_token *end_token);

// ================ CMD MAKING ================ //

t_ecode_p	parse(t_shell *shell);
void		make_cmd(t_shell *shell, t_cmd **cmd, t_token *start_token, \
	t_token *end_token);
t_cmd		*new_cmd(void);

// PARSING JUMP TABLE
t_ecode_p	parser_redir(t_cmd *cmd, t_token *token);
t_ecode_p	parser_arith_expan(t_cmd *cmd_node, t_token *token);
t_ecode_p	parser_add_env_var(t_cmd *cmd, t_token *token);
t_ecode_p	parser_add_new_arg(t_cmd *cmd, t_token *token);

//EXPANTION
void		expand(t_shell *shell, t_token *start_token, t_token *end_token, \
	t_env *env_list);
char		*get_env_value_from_key(t_env *env_, char *key);
void		handle_var_sign(t_shell *shell, char **str, char **expanded_str, \
	t_env *env_list);

//REDIRECTION
t_redir		*new_redir(void);
void		add_redir_in_back(t_redir **list, t_redir *new_redir);
void		free_redir_list(t_redir **list);
t_ecode		open_redirections(t_cmd *cmd);

// ================ PARSING SIDE OF EXECUTION ================ //

int			traverse_tree_and_execute(t_shell *shell, t_tree *node, \
				t_tree *parent_node, int prev_exit_code);
int			handle_pipe_subtree(t_shell *shell, t_tree *tree_node);

// ================ FREEING PARSING STRUCTS ================ //

void		free_token(t_token **token);
void		free_token_list(t_token **list);
void		free_args(char ***args);
void		free_cmd(t_cmd **cmd);
void		free_tree(t_tree **node);

// ================ PRINTING PARSING STRUCTS ================ //

void		print_env(t_env *env_list);
void		print_tokens(t_token *list);
void		print_cmd(t_cmd *cmd);
void		print_tree(t_tree *tree);
void		print_tree_nodes(t_tree *node, int level);

// ================ ERROR HANDLING ================ //

void		handle_parsing_err(t_shell *shell, t_ecode_p err_no);
void		handle_cmd_err(t_shell *shell, t_cmd *cmd, char *err_msg);
void		handle_perror(char *str);
void		handle_builtin_err(char *cmd_name, char *arg, char *err_msg);

//CLEANING
void		clean_nicely(t_shell *shell);
void		clean_nicely_and_exit(t_shell *shell, int exit_code);

//SIGNALS

void		init_signals(t_signal_mode signal_mode);

void		set_signals(t_signal_mode signal_mode);
void		print_heredoc_newline(void);

// ================ ENV ================ //

ssize_t		count_keyvalue_env_nodes(t_env *env_list);
ssize_t		count_key_env_nodes(t_env *env_list);
ssize_t		count_envp_keys(char **envp);
ssize_t		count_values_from_env_node(t_env *env_list, char *key);
char		**create_env_array(t_env *env);
char		**create_export_array(t_env *env);
void		free_env_list(t_env	**head);
void		free_env_node(t_env **node);
t_env		*new_env_node(void);
t_ecode		populate_env_node(t_env **node, char *keyval);
t_env		*create_populated_env_node(char *keyval);
t_ecode		create_env_list(t_env **head, char **envp);
void		print_env_node_debugging(t_env *node);
void		print_env_list(t_env *head);
t_ecode		add_last_env_node(t_env **head, t_env *node);
t_ecode		update_env_node(t_env **head, char *key, char *value, \
	bool create_node);
t_ecode		update_value_in_env_node(t_env *node, char *value);
t_ecode		update_keyvalue_in_env_node(t_env *node);
t_ecode		update_pwd(t_env *env_head);
char		*get_key_from_keyvalue(char *keyvalue);
t_ecode		get_value_from_keyvalue(char *keyvalue, char **value_ptr);
t_env		*find_env_node(t_env *env, char *key);
t_env		*get_last_env_node(t_env *head);

// UTILS

char		**ft_strjoin_arr(char **arr, char *str);
char		*ft_strjoin_fs1(char **s1, const char *s2);
char		*ft_strjoin_fs2(const char *s1, char **s2);
t_ecode		append_suffix(char **str, char *suffix, bool duplicate);
void		close_all_fds_in_process(void);
void		close_all_fds_in_cmd(t_cmd *cmd);
size_t		ft_str_count(char **cmd_args);
bool		ft_is_natural(char *arg);
size_t		max_len(char *s1, char *s2);
int			safe_assign_str(char **dest, const char *src);
size_t		ft_strcspn(const char *str, const char *reject);

// ================ FREE ================ //

void		ft_free_2d(void ***arr);
void		ft_free_3d(void ****arr);
void		ft_free(void **var);

//PRINT

void		ft_print_2d_arr(char **arr);
void		ft_print_3d_arr(char ***arr);

// ================ EXECUTOR ================ //

int			executor(t_shell *shell, t_cmd *cmd);

//EXECUTOR UTILS

t_builtin	check_builtin(char *cmd_name);
size_t		count_cmds(t_cmd *head);
t_ecode		create_std_backup(int backup[2]);
t_ecode		dup_and_close(int oldfd, int newfd);
bool		ft_ispath(const char *str);

char		*get_cmd_path(t_shell *shell, char *cmd_name);
void		handle_builtin(t_shell *shell, t_cmd *cmd);

// ================	BUILTINS ================ //

t_ecode		echo_builtin(t_shell *shell, char **cmd_args);
t_ecode		cd_builtin(t_shell *shell, char **cmd_args);
t_ecode		env_builtin(t_shell *shell, char **cmd_args);
t_ecode		unset_builtin(t_shell *shell, char **cmd_args);
t_ecode		pwd_builtin(t_shell *shell, char **cmd_args);
t_ecode		declare_builtin(t_shell *shell, char **cmd_args);
t_ecode		export_builtin(t_shell *shell, char **args);
t_ecode		exit_builtin(t_shell *shell, char **cmd_args);

//	cd_utils.c
t_ecode		update_oldpwd_pwd(t_env **env_list, char *cwd);
char		*get_home(void);
bool		has_cdpath_prefix(char *directory);
t_ecode		check_curpath_access(char *curpath);

//	chdir_cdpath.c
t_ecode		chdir_cdpath(t_env **env_list, char *directory, char *cwd);
t_ecode		traverse_and_chdir_cdpath(char **cdpath_values, \
			ssize_t values_count, char *directory);
t_ecode		chdir_null_cdpath(char *directory, ssize_t *i, int8_t *null_flag);
t_ecode		chdir_cdpath_value(char *cdpath_value, char *directory, ssize_t *i);

//	chdir_default.c
t_ecode		chdir_default(t_env **env_list, char *directory, char *cwd);

//	chdir_functions.c
t_ecode		chdir_home(t_env **env_list, char *cwd);
t_ecode		chdir_tilde(t_env **env_list, char *cwd);
t_ecode		chdir_dash(t_env **env_list, char *cwd);
t_ecode		cd_builtin(t_shell *shell, char **cmd_args);

#endif
