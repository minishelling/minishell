#include "../../include/minishell.h"

t_token	*get_after_space_token(t_token *token)
{
	t_token	*return_token;

	if (token == NULL)
		return (NULL);
	return_token = token->next;
	if (return_token == NULL)
		return (NULL);
	if (return_token->id == SPACE_CHAR || return_token->id == TAB_CHAR || return_token->id == NL)
		return_token = return_token->next;
	return (return_token);
}

t_token	*get_after_pipe_token(t_token *token)
{
	t_token	*t_previous;

	if (token == NULL)
		return (NULL);
	while (token != NULL)
	{
		t_previous = token;
		token = token->next;
		// if (t_previous->id == PIPE || t_previous->id == AND_OPR || t_previous->id == OR_OPR)
		if (t_previous->id == PIPE)
			break ;
	}
	return (token);
}

t_token	*get_after_word_token(t_token *token)
{
	t_token	*t_previous;

	if (token == NULL)
		return (NULL);
	while (token != NULL)
	{
		t_previous = token;
		token = token->next;
		if (t_previous->id == WORD)
			break ;
	}
	return (token);
}

t_token *get_after_arith_expan_token(t_token *token)
{

	if (token == NULL)
		return (NULL);
	if (token->next)
		token = token->next;
	while (token)
	{
		//printf ("	in the arith_expan thingie, token is %s\n", token->str);
		if (token->id == ARITH_EXPAN)
		{
			return (token->next);
			break ;
		}
		token = token->next;
	}
	return (token);
}
