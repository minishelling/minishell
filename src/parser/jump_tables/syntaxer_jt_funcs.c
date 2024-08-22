#include "../../../include/minishell.h"

int	syntax_id_pipe(t_token *t_prev, t_token *t_cur, t_env *env_list)
{
	t_token	*t_next;

	(void) env_list;
	t_next = get_after_space_token((t_token *) t_cur);
	if (t_prev == NULL || t_next == NULL)
		return (ERR_SYNTAX_PIPE);
	if (t_prev)
		printf ("t_prev is %s\n", t_prev->str);
	if (t_cur)
		printf ("t_cur is %s\n", t_cur->str);
	if (t_next)
		printf ("t_next is %s\n", t_next->str);

	if (t_cur->id == PIPE && (t_prev->id == GT || t_prev->id == LT))
		return (ERR_SYNTAX_PIPE);
	if ((t_cur->id == PIPE || t_cur->id == OR_OPR) && (t_next->id == PIPE || t_next->id == OR_OPR))
		{
			printf ("dsfsdfs\n");
			return (ERR_SYNTAX_PIPE);
		}
	if ((t_cur->id == OR_OPR && (t_next->id == OR_OPR || t_next->id == PIPE)) ||
		(t_cur->id == PIPE && t_next->id == OR_OPR))
			return (ERR_SYNTAX_PIPE);
	

	return (0);
}

// bool	syntax_id_redir_envvar(t_token *t_next, t_env *_list)
// {
// 	size_t	len;
// 	char	*env_value;

// 	env_value = get_expanded_value(t_next->str, 0, &len, env_list);
// 	if (!env_value)
// 		return (0);
// 	if (ft_strchr(env_value, ' '))
// 		return (1);
// 	return (0);
// }

int	syntax_id_redir(t_token *t_prev, t_token *t_cur, t_env *env_list)
{
	t_token	*t_next;
	(void)env_list;

	t_next = get_after_space_token((t_token *) t_cur);
	(void) t_prev;
	if (t_next == NULL)
		return (ERR_SYNTAX_REDIR);
	// if (!(t_next->id == WORD || t_next->id == SQUOTE || \
	// 	t_next->id == DQUOTE || t_next->id == ENV_VAR))
	// 	return (ERR_SYNTAX_REDIR);
	// if (t_next->id == ENV_VAR)
	// 	if (syntax_id_redir_envvar(t_next, env_list))
	// 		return (ERR_SYNTAX_REDIR);
	return (0);
}

int	syntax_id_misc(t_token *t_prev, t_token *t_cur, t_env *env_list)
{
	(void) t_prev;
	(void) t_cur;
	(void) env_list;
	return (0);
}

int	syntax_id_parentheses(t_token *t_prev, t_token *t_cur, t_env *env_list)
{
	//t_token	*t_next;

	(void) t_prev;
	(void) env_list;
	(void) t_cur;
	// t_next = get_after_space_token((t_token *) t_cur);
	// if (t_next == NULL)
	// 	return (1);
	return (0);	
}

int	syntax_id_and_opr(t_token *t_prev, t_token *t_cur, t_env *env_list)
{
	t_token	*t_next;

	(void) env_list;
	t_next = get_after_space_token((t_token *) t_cur);
	if (t_prev == NULL || t_next == NULL)
		return (ERR_SYNTAX_AND);
	if (t_cur->id == AND_OPR && (t_prev->id == GT || t_prev->id == LT))
		return (ERR_SYNTAX_AND);
	if (t_cur->id == AND_OPR && (t_next->str[0]== '&' || t_next->id == OR_OPR || t_next->id == AND_OPR))
		return (ERR_SYNTAX_AND);
	return(0);
}

int	syntax_id_semicol(t_token *t_prev, t_token *t_cur, t_env *env_list)
{
	(void) env_list;
	(void) t_cur;
	if (t_prev == NULL || t_prev->id == PIPE)
		return (ERR_SYNTAX_SEMICOL);
	if (t_cur->id == SEMICOL && (t_prev->id == GT || t_prev->id == LT))
		return (ERR_SYNTAX_SEMICOL);
	return(0);
}