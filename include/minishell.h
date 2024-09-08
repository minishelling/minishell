#ifndef MINISHELL_H
#define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <signal.h>
# include <stddef.h>
# include <sys/wait.h>
# include <sys/time.h>
# include <errno.h>
# include <limits.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <string.h>
# include "../libft/libft.h"
# include "env.h"
# include "errors.h"

//# define MINISHARED_PROMPT "\001\033[1;32m\002Mini_shared<🤜 🤛> \001\033[0m\002"
#define MINISHARED_PROMPT "\001\033[38;5;196m\002M\001\033[38;5;202m\002i\001\033[38;5;208m\002n\001\033[38;5;214m\002i\001\033[38;5;220m\002_\001\033[38;5;226m\002s\001\033[38;5;190m\002h\001\033[38;5;154m\002a\001\033[38;5;118m\002r\001\033[38;5;82m\002e\001\033[38;5;120m\002d\001\033[0m\002<🤜 🤛> \001\033[0m\002"
#define ERR_PROMPT "\001\033[38;5;196m\002M\001\033[38;5;202m\002i\001\033[38;5;208m\002n\001\033[38;5;214m\002i\001\033[38;5;220m\002_\001\033[38;5;226m\002s\001\033[38;5;190m\002h\001\033[38;5;154m\002a\001\033[38;5;118m\002r\001\033[38;5;82m\002e\001\033[38;5;120m\002d\001\033[0m\002: \001\033[0m\002"

# define META_CHARS_SET " \t\n|&;()><"
# define SPECIAL_CHARS "\'\"$"
# define ERROR -1

#define RESET_COLOR "\033[0m"
#define MAGENTA_TEXT "\033[0;35m"
#define WHITE_TEXT "\033[0;37m"
#define MAGENTA_BACKGROUND "\033[45m"
#define WHITE_BACKGROUND "\033[47m"

# define READ_END 0
# define WRITE_END 1


enum e_parsing_error
{
	PARSING_OK,
	ERR_UNCLOSED_QUOTES,
	ERR_UNCLOSED_PAREN,
	ERR_SYNTAX_PIPE,
	ERR_SYNTAX_AND,
	ERR_SYNTAX_REDIR,
	ERR_SYNTAX_SEMICOL,
	ERR_SYNTAX_ERROR
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
	OR_OPR
}	t_token_id;

typedef enum e_redir_id 
{
	REDIR = 0,
	IN,
	HERE,
	OUT,
	APP
}	t_redir_id;

typedef struct s_redir
{
	t_redir_id		redir;
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
	struct s_cmd	*next;
}					t_cmd;

typedef struct s_env_list
{
	char				*name;
	char				*value;
	bool				has_value;
	struct s_env_list	*next;
}				t_env_list;

typedef struct s_shell 
{
	int			pipefd[2];
	pid_t		parent;
	int			read_fd;
	int			write_fd;
	int			status;
    char 		*input;
    t_token 	*token;
	t_token		*syntax; //?
	t_env		*env_list;
	t_cmd		*cmd_list;
} t_shell;


int		init_shell(char **envp, t_shell *shell);
t_token	*tokenize(char *input);
int		syntax(t_shell *shell);
t_token	*expand(t_token *token_list_head, t_env *env_list);
bool	concat_word_tokens(t_shell *shell);
int		parse(t_shell *shell);

void	set_pos_end_space_or_word(char *str, size_t *pos, t_token_id token_id);
void	set_pos_end_quote(char *str, size_t *pos, t_token_id token_id);
void	set_pos_end_and_opr(char *str, size_t *pos, t_token_id token_id);
void	set_pos_end_semicol(char *str, size_t *pos, t_token_id token_id);
void	set_pos_end_parentheses(char *str, size_t *pos, t_token_id token_id);
void	set_pos_end_redir(char *str, size_t *pos, t_token_id token_id);
void	set_pos_end_env_var(char *str, size_t *pos, t_token_id token_id);
void	set_pos_end_pipe(char *str, size_t *pos, t_token_id token_id);

int	syntax_id_pipe(t_token *t_prev, t_token *t_cur, t_env *env_list);
int	syntax_id_and_opr(t_token *t_prev, t_token *t_cur, t_env *env_list);
int	syntax_id_semicol(t_token *t_prev, t_token *t_cur, t_env *env_list);
int	syntax_id_parentheses(t_token *t_prev, t_token *t_cur, t_env *env_list);
int	syntax_id_redir(t_token *t_prev, t_token *t_cur, t_env *env_list);
int	syntax_id_misc(t_token *t_prev, t_token *t_cur, t_env *env_list);

int	parser_space(t_cmd *cmd, t_token *token);
int	parser_pipe(t_cmd *cmd_node, t_token *token);
int	parser_and_opr(t_cmd *cmd, t_token *token);
int	parser_semicol(t_cmd *cmd, t_token *token);
int	parser_par_open(t_cmd *cmd, t_token *token);
int	parser_par_close(t_cmd *cmd, t_token *token);
int	parser_redir(t_cmd *cmd, t_token *token);
int	parser_or_opr(t_cmd *cmd, t_token *token);
int	parser_word(t_cmd *cmd, t_token *token);

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
void	free_token_list(t_token *t_list, t_token *(*f) (t_token *));
t_token	*free_token_str(t_token *token);

t_env	*new_env_var(char *env_var_str);
void	add_env_var_in_back(t_env **env_var, t_env *new_env_var);
char	*get_env_value_from_key(t_env *env_head, char *key);
void	free_env_node(t_env *env_var_node);
void	free_env_list(t_env *env_list);

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
void	handle_error(t_shell *shell, int err_no, void *param);

char	*ft_strjoin_fs1(char *s1, const char *s2);
t_token	*get_after_word_token(t_token *token);

//FREE

void	ft_free_2d(void ***arr);
void	ft_free_3d(void ****arr);
void	ft_free(void **var);


//PRINT

void	ft_print_2d_arr(char **arr);
void	ft_print_3d_arr(char ***arr);

/* EXECUTOR */

void	executor(t_shell *shell);

//EXECUTOR UTILS

void	redirect_io(t_shell *shell, int io_fd, int io_target);

char	*get_cmd_path(t_shell *shell, char *cmd_name);

#endif
