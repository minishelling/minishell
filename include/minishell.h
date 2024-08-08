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

# define MINISHARED_PROMPT "\001\033[1;32m\002Minishared<🤜 🤛> \001\033[0m\002"
# define META_CHARS_SET " \t\n|&;()><"
# define SPECIAL_CHARS "\'\"$"
# define ERROR -1

#define RESET_COLOR "\033[0m"
#define MAGENTA_TEXT "\033[0;35m"
#define WHITE_TEXT "\033[0;37m"
#define MAGENTA_BACKGROUND "\033[45m"
#define WHITE_BACKGROUND "\033[47m"

typedef enum e_token_id 
{
	SPACE_CHAR,
	TAB_CHAR,
	NL,
	PIPE,
	AND_OPR,
	SEMI_COL,
	PAR_OPEN,
	PAR_CLOSE,
	LT,
	GT,
	SQUOTE,
	DQUOTE,
	ENV_VAR,
	WORD
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
	char		*pwd;
	int			pipefd[2];
	int			read_fd;
	int			write_fd;
    char 		*input;
    t_token 	*token;
	t_token		*syntax;
	t_env		*env_list;
	t_cmd		*cmd_list;
} t_shell;


int		init_shell(char **envp, t_shell *shell);
void 	tokenize_input(t_shell *shell);
//void add_token(t_shell *shell, e_token_type type, e_token_id id, char *str);

t_token	*lex(char *input);

t_token	*syntax(t_token *head, t_env *env_list);
t_token	*expand(t_token *token_list_head, t_env *env_list);
void	set_pos_end_space_or_word(char *str, size_t *pos, t_token_id token_id);
void	set_pos_end_quote(char *str, size_t *pos, t_token_id token_id);
void	set_pos_end_ampersand(char *str, size_t *pos, t_token_id token_id);
void	set_pos_end_semicol(char *str, size_t *pos, t_token_id token_id);
void	set_pos_end_parentheses(char *str, size_t *pos, t_token_id token_id);
void	set_pos_end_redir(char *str, size_t *pos, t_token_id token_id);
void	set_pos_end_env_var(char *str, size_t *pos, t_token_id token_id);
void	set_pos_end_pipe(char *str, size_t *pos, t_token_id token_id);

bool	syntax_id_pipe(t_token *t_prev, t_token *t_cur, t_env *env_list);
bool	syntax_id_ampersand(t_token *t_prev, t_token *t_cur, t_env *env_list);
bool	syntax_id_semicol(t_token *t_prev, t_token *t_cur, t_env *env_list);
bool	syntax_id_parentheses_open(t_token *t_prev, t_token *t_cur, t_env *env_list);
bool	syntax_id_parentheses_close(t_token *t_prev, t_token *t_cur, t_env *env_list);
bool	syntax_id_redir(t_token *t_prev, t_token *t_cur, t_env *env_list);
bool	syntax_id_misc(t_token *t_prev, t_token *t_cur, t_env *env_list);

void	parse(t_shell *shell);
void	env_var_free_node(t_env *env_var_node);
t_token	*new_token(void);
void	add_token_in_back(t_token **t_list, t_token *new);
t_token_id	get_token_id(char c);
t_token	*get_after_space_token(t_token *token);
t_token	*get_after_pipe_token(t_token *token);
char	*get_expanded_value(char *str, size_t pos, \
		size_t *len_env_var, t_env *env_list);
t_token	*free_token_node(t_token *t_node);
void	free_last_token(t_token *t_list, t_token *(*f) (t_token *));
t_token	*copy_token(t_token *t_node);
void	free_token_list(t_token *t_list, t_token *(*f) (t_token *));
t_env	*env_var_create_new_node(char *env_var_str);
void	env_var_add_to_end_list(t_env **env_var, t_env *new_env_var);
void	env_var_print_linked_list(t_env *env_list);
void	env_var_free_list(t_env *env_list);
//char	*env_var_get_env(char *key, t_env *env_list);

int		init_shell_update_SHLVL(t_env **env_list);
t_env	*env_var_get_env_node(char *name, t_env *env_list);

t_env_ecode	env_copy_keyval(t_env **new_node, char *keyvalue);
// t_env_ecode	env_init_list(t_env **head, char **envp);

char	*env_get_value_from_key(t_env *env_head, char *key);
t_token	*last_token(t_token *token_list_head);

char	*ft_strjoin_fs1(char *s1, const char *s2);
t_token	*free_token_str(t_token *token);
bool	concat_word_tokens(t_shell *shell);
t_token	*parser_id_space(t_cmd *cmd, t_token *token);
t_token	*parser_id_word(t_cmd *cmd, t_token *token);
t_token	*parser_id_redir(t_cmd *cmd, t_token *token);
t_redir_id	redir_identification(char *str);
t_token	*parser_id_pipe(t_cmd *cmd_node, t_token *token);
t_cmd	*new_cmd(void);
t_cmd	*cmd_last(t_cmd *cmd);
void	add_cmd_in_back(t_cmd **cmd_list_head, t_cmd *new_cmd);
t_cmd	*free_cmd(t_cmd *cmd);
void	free_cmd_list(t_cmd *cmd_list_head);
t_redir	*new_redir(void);
void	add_redir_in_back(t_redir **redir_list_head, t_redir *new_redir);
void	free_redir_list(t_redir *redir_list_head);
t_redir	*last_redir(t_redir *redir_list_head);

void	print_redir(t_redir *redir_list_head);
void	print_token(t_token *head);
void	print_cmd(t_cmd *cmd_list_head);


#endif
