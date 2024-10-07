#ifndef MINISHELL_H
# define MINISHELL_H

# define _GNU_SOURCE // FOR SIGACTION...

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
# include <stddef.h>
# include "../libft/libft.h"

//# define MINISHARED_PROMPT "\001\033[1;32m\002Mini_shared<🤜 🤛> \001\033[0m\002"
#define MINISHARED_PROMPT "\001\033[38;5;196m\002M\001\033[38;5;202m\002i\001\033[38;5;208m\002n\001\033[38;5;214m\002i\001\033[38;5;220m\002_\001\033[38;5;226m\002s\001\033[38;5;190m\002h\001\033[38;5;154m\002a\001\033[38;5;118m\002r\001\033[38;5;82m\002e\001\033[38;5;120m\002d\001\033[0m\002<🤜 🤛> \001\033[0m\002"
#define ERR_PROMPT "Mini_shared: \001\033[0m\002"

# define META_CHARS_PLUS_SET " \t\n|&;()><\'\"$"
# define ERROR -1

#define RESET_COLOR "\033[0m"
#define MAGENTA_TEXT "\033[0;35m"
#define WHITE_TEXT "\033[0;37m"
#define MAGENTA_BACKGROUND "\033[45m"
#define WHITE_BACKGROUND "\033[47m"
#define GREY_BACKGROUND "\033[48;5;24m"

# define READ_END 0
# define WRITE_END 1

extern int	g_exitcode;

typedef enum	signal_mode
{
	INTERACTIVE,
	NON_INTERACTIVE,
	HEREDOC
}	t_signal_mode;

typedef enum exit_code
{
	EXIT_CMD_NOT_EXECUTABLE = 126,
	EXIT_CMD_NOT_FOUND = 127,
	EXIT_SIGINT = 130,
	EXIT_SIGQUIT = 131
}	t_exit_code;

typedef enum e_codes
{
	INIT = -1,
	SUCCESS = 0,
	FAILURE,
	PROCEED,
	INVALID_OPTION,
	INVALID_FILE,
	NULL_ERROR,
	NULL_ENV,
	NULL_NODE,
	NULL_STRING,
	NULL_ARRAY,
	NULL_CURPATH_LIST,
	DUP_ERROR,
	COUNT_ERROR,
	CWD_ERROR,
	MALLOC_ERROR,
	HOME_ERROR,
	ACCESS_ERROR,
	CHDIR_ERROR,
	CDPATH_ERROR,
	CDPATH_NULL,
	ENV_ERROR,
	NEW_NODE_ERROR,
	COUNT
}	t_ecode;

enum e_parsing_error
{
	PARSING_OK,
	ERR_SYNTAX_NL,
	ERR_UNCLOSED_QUOTES,
	ERR_SYNTAX_UNEXPECT_OPEN,
	ERR_SYNTAX_UNEXPECT_CLOSE,
	ERR_SYNTAX_PIPE,
	ERR_SYNTAX_AND,
	ERR_SYNTAX_REDIR,
	ERR_SYNTAX_SEMICOL,
	ERR_SYNTAX_ERROR,
	ERR_MEM
};

typedef enum e_token_id 
{
	SPACE_CHAR,
	TAB_CHAR,
	NL,
	PIPE,
	AND_OPR,
	SEMICOL,
	PAR_OPEN,
	PAR_CLOSE,
	LT,
	GT,
	SQUOTE,
	DQUOTE,
	ENV_VAR,
	WORD,
	OR_OPR,
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
	ECHO, //0
	CD,
	PWD,
	EXPORT,
	DECLARE,
	UNSET,
	ENV,
	EXIT,
	NON_BUILTIN, //8
	NULL_CMD,
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
} t_tree_type;

typedef struct s_tree
{
	struct s_tree 	*left;
	struct s_tree	*right;
	t_tree_type		type;
	t_token 		*start_token;
    t_token 		*end_token;
	t_cmd			*cmd_list;
} t_tree;

typedef struct s_shell 
{
	int			pipefd[2];
	pid_t		parent;
	int			read_fd;
	int			status;
    char 		*input;
    t_token 	*token;
	t_tree		*tree;
	t_env		*env_list;
	t_cmd		*cmd_list;
} t_shell;


int		init_shell(char **envp, t_shell *shell);
int		tokenize(t_shell *shell, char *input);
int		syntax(t_shell *shell);
t_token	*expand(t_token *start_token, t_token *end_token, t_env *env_list);
bool	join_word_and_env_var_tokens(t_shell *shell);
int		parse(t_shell *shell);

void	set_pos_end_space_or_word(char *str, size_t *pos, t_token_id *token_id);
void	set_pos_end_quote(char *str, size_t *pos, t_token_id *token_id);
void	set_pos_end_and_opr(char *str, size_t *pos, t_token_id *token_id);
void	set_pos_end_semicol(char *str, size_t *pos, t_token_id *token_id);
void	set_pos_end_parentheses(char *str, size_t *pos, t_token_id *token_id);
void	set_pos_end_redir(char *str, size_t *pos, t_token_id *token_id);
void	set_pos_end_env_var(char *str, size_t *pos, t_token_id *token_id);
void	set_pos_end_pipe(char *str, size_t *pos, t_token_id *token_id);

int	syntax_pipe(t_token *t_prev, t_token *t_cur, t_env *env_list);
int	syntax_and_opr(t_token *t_prev, t_token *t_cur, t_env *env_list);
int	syntax_semicol(t_token *t_prev, t_token *t_cur, t_env *env_list);
int	syntax_parens(t_token *t_prev, t_token *t_cur, t_env *env_list);
int	syntax_redir(t_token *t_prev, t_token *t_cur, t_env *env_list);
int	syntax_misc(t_token *t_prev, t_token *t_cur, t_env *env_list);
int	syntax_word(t_token *t_prev, t_token *t_cur, t_env *env_list);

int	parser_space(t_cmd *cmd, t_token *token);
int	parser_pipe(t_cmd *cmd_node, t_token *token);
int	parser_and_opr(t_cmd *cmd, t_token *token);
int	parser_semicol(t_cmd *cmd, t_token *token);
int	parser_par_open(t_cmd *cmd, t_token *token);
int	parser_par_close(t_cmd *cmd, t_token *token);
int	parser_redir(t_cmd *cmd, t_token *token);
int	parser_or_opr(t_cmd *cmd, t_token *token);
int	add_new_arg(t_cmd *cmd, t_token *token);

t_token	*new_token(void);
void	add_token_in_back(t_token **t_list, t_token *new);
t_token_id	get_token_id(char c);
t_token	*get_after_space_token(t_token *token);
t_token	*get_after_pipe_token(t_token *token);
char	*get_expanded_value(char *str, size_t pos,size_t *len_env_var, t_env *env_list);
t_token	*free_token_node(t_token *t_node);
void	free_last_token(t_token *t_list, t_token *(*f) (t_token *));
t_token	*copy_token(t_token *t_node);
t_token	*last_token(t_token *token_list_head);
void 	free_token_list(t_token *token_list);
t_token	*free_token_str(t_token *token);

t_env	*new_env_var(char *env_var_str);
void	add_env_var_in_back(t_env **env_var, t_env *new_env_var);
char	*get_env_value_from_key(t_env *env_head, char *key);
// void	free_env_node(t_env *env_var_node);
// void	free_env_list(t_env *env_list);

t_cmd	*new_cmd(void);
t_cmd	*cmd_last(t_cmd *cmd);
void	add_cmd_in_back(t_cmd **cmd_list_head, t_cmd *new_cmd);
t_cmd	*free_cmd(t_cmd *cmd);
void	free_cmd_list(t_cmd *cmd_list_head);

t_redir	*new_redir(void);
void	add_redir_in_back(t_redir **redir_list_head, t_redir *new_redir);
void	free_redir_list(t_redir *redir_list_head);
t_redir	*last_redir(t_redir *redir_list_head);
t_redir_id	redir_ident(char *str);

size_t	cmd_size(t_cmd *t_list);
void	print_env(t_env *env_list);
void	print_redir(t_redir *redir_list_head);
void	print_token(t_token *head);
void	print_cmd(t_cmd *cmd_list_head);
void 	handle_parsing_err(t_shell *shell, int err_no, void *param);

t_token	*get_after_word_token(t_token *token);

t_token *find_last_log_op_token_nip(t_token *token_head, t_token *end_token);
t_tree 	*make_tree(t_shell *shell, t_token *start_token, t_token *end_token);
void	print_tree(t_tree *node, int level);
void 	remove_space_tokens(t_token **head);
t_token *remove_token(t_token *start_token, t_token *token_to_remove);
void 	free_tree(t_tree *node);
void 	remove_subshell_parens(t_shell *shell);
t_cmd	*make_cmd(t_shell *shell, t_token *start_token, t_token *end_token);
void print_tree_with_cmds(t_tree *node, int level);
t_token *get_after_arith_expan_token(t_token *token);
int parser_arith_expan(t_cmd *cmd_node, t_token *token);
int pre_execute(t_shell *shell, t_tree *node, t_tree *parent_node, int prev_exit_code);
void free_cmd_list(t_cmd *cmd_list);

int parser_env_var(t_cmd *cmd_node, t_token *token);
bool	join_quotes_tokens(t_shell *shell);
char *process_double_quotes(char **str_ptr, char *expanded_str, t_env *env_list);
char *process_single_quotes(char **str_ptr, char *expanded_str);
char *process_unquoted(char **str_ptr, char *expanded_str, t_env *env_list);
t_token *previous_token_if_exists(t_token *head, t_token *target);
t_token *handle_arith_expan(t_token **head, t_token **cur_open, t_token **cur_close);
void handle_heredocs(t_token *token_list) ;
t_ecode	open_redirections(t_shell *shell, t_cmd *head);
void print_tree_verbose(t_tree *node, int level);
int handle_pipe_subtree(t_shell *shell, t_tree *tree_node);
void handle_cmd_err(t_cmd *cmd, char *err_msg);
void handle_perror(char *str);
t_token *non_null_previous(t_token *start_token, t_token *before_what);
bool is_dquote(t_token *token);
bool is_squote(t_token *token);
bool is_word(t_token *token);
bool is_env_var(t_token *token);



//SIGNALS

void	set_signals(t_signal_mode signal_mode);
void 	print_heredoc_newline(void);

//ENV - Lisandro

ssize_t	count_keyvalue_env_nodes(t_env *env_list);
ssize_t	count_key_env_nodes(t_env *env_list);
ssize_t	count_envp_keys(char **envp);
ssize_t	count_values_from_env_node(t_env *env_list, char *key);
char	**create_env_array(t_env *env);
char	**create_export_array(t_env *env);
void	free_env_list(t_env	**head);
void	free_env_node(t_env **node);
t_env	*new_env_node(void);
t_ecode	populate_env_node(t_env **node, char *keyval);
t_env	*create_populated_env_node(char *keyval);
t_ecode	init_env_list(t_env **head, char **envp);
void	print_env_node(t_env *node);
void	print_env_list(t_env *head);
t_ecode	add_last_env_node(t_env **head, t_env *env);
t_ecode	update_env_node(t_env **head, char *key, char *value, bool create_node);
t_ecode	update_value_in_env_node(t_env *node, char *value);
t_ecode	update_keyvalue_in_env_node(t_env *node);
t_ecode	update_pwd(t_env *env_head);
char	*get_key_from_keyvalue(char *keyvalue);
t_ecode	get_value_from_keyvalue(char *keyvalue, char **value_ptr);
t_env	*find_env_node(t_env *env, char *key);
t_env	*get_last_env_node(t_env *head);


t_ecode	update_pwd(t_env *pwd_node);
t_ecode	update_oldpwd(t_env	*oldpwd_node, char *cwd);

// UTILS

char	*ft_strjoin_fs1(char **s1, const char *s2);
char	*ft_strjoin_fs2(const char *s1, char **s2);
t_ecode	append_suffix(char **str, char *suffix, bool duplicate);

//FREE

void	ft_free_2d(void ***arr);
void	ft_free_3d(void ****arr);
void	ft_free(void **var);


//PRINT

void	ft_print_2d_arr(char **arr);
void	ft_print_3d_arr(char ***arr);

/* EXECUTOR */

int	executor(t_shell *shell, t_cmd *cmd);

//EXECUTOR UTILS

t_builtin	check_builtin(char *cmd_name);
size_t		count_cmds(t_cmd *head);
t_ecode		create_std_backup(int backup[2]);
t_ecode		dup_and_close(int oldfd, int newfd);

char	*get_cmd_path(t_shell *shell, char *cmd_name);
void 	run_child(t_shell *shell, t_cmd *cmd);
void	do_parent_duties(t_shell *shell);
void	handle_builtin(t_shell *shell, t_cmd *cmd);
t_ecode	execute_builtin(t_shell *shell, char **cmd_args);
void	handle_non_builtin(t_shell *shell, t_cmd *cmd);

//BUILTINS

t_ecode echo_builtin(t_shell *shell, char **cmd_args);
t_ecode	cd_builtin(t_shell *shell, char **cmd_args);
t_ecode	env_builtin(t_shell *shell, char **cmd_args);
t_ecode	unset_builtin(t_shell *shell, char **cmd_args);
t_ecode	pwd_builtin(t_shell *shell, char **cmd_args);
t_ecode	declare_builtin(t_shell *shell, char **cmd_args);
t_ecode	export_builtin(t_shell *shell, char **args);
t_ecode	exit_builtin(t_shell *shell, char **cmd_args);


//	CD

t_ecode	cd_builtin(t_shell *shell, char **cmd_args);
t_ecode	chdir_home(t_env **env_list, char **cwd);
t_ecode	update_oldpwd_pwd(t_env **env_list, char **cwd);
t_ecode	chdir_tilde(t_env **env_list, char **cwd);
t_ecode	chdir_dash(t_env **env_list, char **cwd);
t_ecode	chdir_cdpath(t_env **env_list, char *directory, char **cwd);
t_ecode	traverse_and_chdir_cdpath(char **cdpath_values, ssize_t values_count, char *directory);
t_ecode	check_curpath_access(char *curpath);
t_ecode	chdir_null_cdpath(char *directory, ssize_t *i, int8_t *null_flag);
t_ecode chdir_cdpath_value(char *cdpath_value, char *directory, ssize_t *i);
t_ecode	chdir_default(t_env **env_list, char *directory, char **cwd);



// t_ecode	builtin_cd(t_shell **shell, char *directory);
// t_ecode	chdir_home(t_env *env_head, char **cwd);
// t_ecode	chdir_cdpath(t_env *env_list, char *directory, char **cwd);
// t_ecode	append_suffix(char **str, char *suffix, bool duplicate);

// t_ecode	chdir_cwd(char *directory, int *null_flag);
// t_ecode chdir_cdpath_value(char **curpath, char *directory);
// t_ecode	loop_cdpath_values(char ***values, char *directory);



//	CURPATH

// t_curpath	*curpath_new_node(char *dir);
// void		curpath_del_node(t_curpath **node);
// void		curpath_del_list(t_curpath **head);
// t_curpath	*get_last_curpath_node(t_curpath *head);
// void		curpath_add_back(t_curpath **head, t_curpath *new);
// t_ecode	create_and_add_back_curpath_node(t_curpath **head, char *directory);
// void		curpath_del_last(t_curpath **head);
// char		*curpath_concat(t_curpath *head);
// void		curpath_print(t_curpath *head);
// int			curpath_check_access(char *curpath);
// t_ecode		curpath_check_access_and_chdir(char *curpath);
// t_ecode		curpath_prepare(char **curpath, char *directory, char *cwd);
// t_ecode		curpath_trim(char **curpath);
// t_ecode		init_and_populate_curpath_list(char **curpath, char ***dirs, t_curpath **final_dirs);
// t_ecode		parse_curpath_dirs(t_curpath **final_dirs);
// t_ecode		remove_previous_dir(t_curpath **final_dirs, char ***dirs, int *i);
// t_ecode		check_access_and_add_back(t_curpath **final_dirs, char ***dirs, int *i);
// t_ecode		remove_curpath_node(t_curpath **head, t_curpath **node);
// void		curpath_del_node_definitely(t_curpath **node);


bool		has_cdpath_prefix(char *directory);
// t_ecode		check_for_special_cd_cases(t_env *env, char *directory, char **curpath);
char		*get_home(void);

// ERROR

const char	*get_error_msg(int e_nbr);

#endif
