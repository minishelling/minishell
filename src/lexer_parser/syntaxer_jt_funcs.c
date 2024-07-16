#include "../../include/minishell.h"

bool	syntax_id_pipe(t_token *t_prev, t_token *t_cur, \
		t_env_list *env_list)
{
	t_token	*t_next;

	(void) env_list;
	if (!t_cur || !t_cur->str)
		return (1);
	t_next = list_token_skip_space((t_token *) t_cur);
	if (t_prev == NULL || t_next == NULL)
		return (1);
	if (ft_strlen(t_cur->str) > 2)
		return (1);
	if (t_prev->id == PIPE || t_next->id == PIPE)
		return (1);
	return (0);
}

bool	syntax_id_redir_shvar(t_token *t_next, t_env_list *env_list)
{
	size_t	len;
	char	*sh_var;

	sh_var = expander_get_shell_var(t_next->str, 0, &len, env_list);
	if (!sh_var)
		return (1);
	if (ft_strchr(sh_var, ' '))
		return (1);
	return (0);
}

bool	syntax_id_redir(t_token *t_prev, t_token *t_cur, \
		t_env_list *env_list)
{
	t_token	*t_next;

	t_next = list_token_skip_space((t_token *) t_cur);
	if (!t_cur || !t_cur->str)
		return (1);
	(void) t_prev;
	if (t_next == NULL)
		return (1);
	if (!(t_next->id == WORD || \
		t_next->id == SQUOTE || \
		t_next->id == DQUOTE || \
		t_next->id == SHELL_VAR))
		return (1);
	if (t_next->id == SHELL_VAR)
		if (syntax_id_redir_shvar(t_next, env_list))
			return (1);
	return (0);
}

bool	syntax_id_misc(t_token *t_prev, t_token *t_cur, \
		t_env_list *env_list)
{
	(void) t_prev;
	(void) t_cur;
	(void) env_list;
	return (0);
}

bool	syntax_id_parentheses_open(t_token *t_prev, t_token *t_cur, \
		t_env_list *env_list)
{
	t_token	*t_next;

	(void) t_prev;
	(void) env_list;
	if (!t_cur || !t_cur->str)
		return (1);
	t_next = list_token_skip_space((t_token *) t_cur);
	if (t_next == NULL)
		return (1);
	return (0);	
}
bool	syntax_id_parentheses_close(t_token *t_prev, t_token *t_cur, \
		t_env_list *env_list)
{
	(void) env_list;
	(void) t_cur;
	if (t_prev == NULL)
		return (1);
	return(0);	
}

bool	syntax_id_ampersand(t_token *t_prev, t_token *t_cur, \
		t_env_list *env_list)
{
	(void) env_list;
	(void) t_cur;
	if (t_prev == NULL)
		return (1);
	return(0);
}

bool	syntax_id_semicol(t_token *t_prev, t_token *t_cur, \
		t_env_list *env_list)
{
	(void) env_list;
	(void) t_cur;
	if (t_prev == NULL)
		return (1);
	return(0);
}