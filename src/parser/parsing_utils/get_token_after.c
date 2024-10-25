#include "../../../include/minishell.h"

t_token	*skip_whitespace_and_get_next_token(t_token *token)
{
	t_token	*return_token;

	if (!token)
		return (NULL);
	return_token = token->next;
	while (return_token && (return_token->id == SPACE_CHAR
			|| return_token->id == TAB_CHAR || return_token->id == NL))
		return_token = return_token->next;
	return (return_token);
}

t_token	*get_after_pipe_token(t_token *token)
{
	t_token	*t_previous;

	if (!token)
		return (NULL);
	while (token)
	{
		t_previous = token;
		token = token->next;
		if (t_previous->id == PIPE)
			break ;
	}
	return (token);
}

t_token	*get_after_arith_expan_token(t_token *first_arith_expan_token)
{
	t_token	*cur_token;

	cur_token = first_arith_expan_token->next;
	while (cur_token && cur_token->id && cur_token->id != ARITH_EXPAN)
		cur_token = cur_token->next;
	return (cur_token);
}
