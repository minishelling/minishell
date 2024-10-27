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

t_token	*get_after_arith_expan_token(t_token *first_arith_expan_token)
{
	t_token	*cur_token;

	cur_token = first_arith_expan_token->next;
	while (cur_token && cur_token->id && cur_token->id != ARITH_EXPAN)
		cur_token = cur_token->next;
	return (cur_token);
}

t_token	*non_null_previous(t_token *start_token, t_token *before_what)
{
	t_token	*return_token;

	return_token = start_token;
	while (return_token->next && return_token->next != before_what)
		return_token = return_token->next;
	return (return_token);
}

t_token	*previous_token_if_exists(t_token *head, t_token *target)
{
	t_token	*current;

	if (head == target)
		return (NULL);
	current = head;
	while (current && current->next != target)
		current = current->next;
	return (current);
}
