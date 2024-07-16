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

# define MINISHARED_PROMPT "\001\033[1;32m\002<🤜 🤛> \001\033[0m\002"
# define META_CHARS_SET " \t\n|&;()><"
# define SPECIAL_CHARS "\'\"$"
# define ERROR -1


// typedef enum e_token_type {
//     META_CHAR,
//     CONTROL_OP,
//     REDIR_OP,
//     WORD,
//     OTHER
// } e_token_type;


typedef enum e_token_id {
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
	SHELL_VAR,
	WORD
}	t_token_id;

typedef enum e_redir_id {
	REDIR = 0,
	IN,
	HERE,
	OUT,
	APP
}	t_redir_id;

typedef struct s_token
{
	t_token_id		id;
	char			*str;
	struct s_token	*next;
}	t_token;

typedef struct s_redir
{
	t_redir_id		redir;
	int				fd;
	char			*file;
	struct s_redir	*next;
}					t_redir;

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
	t_env_list	*env_list;
} t_shell;


int		init_shell(char **envp, t_shell *shell);
void tokenize_input(t_shell *shell);
//void add_token(t_shell *shell, e_token_type type, e_token_id id, char *str);

t_token	*lexer(const char *input);
//void print_tokens(t_token *token);
void	list_token_print(t_token *top);
t_token	*syntax(t_token *top, t_env_list *env_list);
t_token	*expander(t_token *t_input, t_env_list *env_var_list);
void	token_id_misc(const char *inp, size_t *pos, const t_token_id val);
void	token_id_pipe(const char *inp, size_t *pos, const t_token_id val);
void	token_id_ampersand(const char *inp, size_t *pos, const t_token_id val);
void	token_id_semicol(const char *inp, size_t *pos, const t_token_id val);
void	token_id_parentheses(const char *inp, size_t *pos, const t_token_id val);
void	token_id_redir(const char *inp, size_t *pos, const t_token_id val);
void	token_id_quote(const char *inp, size_t *pos, const t_token_id val);
void	token_id_shvar(const char *inp, size_t *pos, const t_token_id val);
bool	syntax_id_pipe(t_token *t_prev, t_token *t_cur, \
		t_env_list *env_list);
bool	syntax_id_ampersand(t_token *t_prev, t_token *t_cur, \
		t_env_list *env_list);
bool	syntax_id_semicol(t_token *t_prev, t_token *t_cur, \
		t_env_list *env_list);
bool	syntax_id_parentheses_open(t_token *t_prev, t_token *t_cur, \
		t_env_list *env_list);
bool	syntax_id_parentheses_close(t_token *t_prev, t_token *t_cur, \
		t_env_list *env_list);
bool	syntax_id_redir(t_token *t_prev, t_token *t_cur, \
		t_env_list *env_list);
bool	syntax_id_misc(t_token *t_prev, t_token *t_cur, \
		t_env_list *env_list);
void	complexer(t_shell *shell);
char	*env_var_get_env(char *name, t_env_list *env_var_list);
void	env_var_free_node(t_env_list *env_var_node);
t_token	*list_token_new(void);
void	list_token_add_back(t_token **t_list, t_token *new);
t_token_id	get_char_id(const char c);
t_token	*list_token_skip_space(t_token *t_current);
char	*expander_get_shell_var(const char *str, const int pos, \
		size_t *len_sh_var, t_env_list *env_var_list);
t_token	*list_token_free_node(t_token *t_node);
void	list_token_free_last(t_token *t_list, t_token *(*f) (t_token *));
t_token	*list_token_cpy_node(t_token *t_node);
void	list_token_free_list(t_token *t_list, t_token *(*f) (t_token *));
t_env_list	*env_var_create_new_node(char *env_var_str);
void	env_var_add_to_end_list(t_env_list **env_var_list, t_env_list *new_env_var);
void	env_var_print_linked_list(t_env_list *env_var_list);
void	env_var_free_list(t_env_list *env_var_list);
char	*env_var_get_env(char *name, t_env_list *env_var_list);

int		init_shell_update_SHLVL(t_env_list **env_var_list);
t_env_list	*env_var_get_env_node(char *name, t_env_list *env_var_list);


// t_env_ecode	env_init_list(t_env **head, char **envp);
// t_env	*env_new_node(void);

#endif
