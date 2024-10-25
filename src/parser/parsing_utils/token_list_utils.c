#include "../../../include/minishell.h"

t_token	*new_token(void)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->next = NULL;
	new->id = -1;
	new->str = NULL;
	return (new);
}

t_token	*last_token(t_token *token_list_head)
{
	t_token	*token;

	if (token_list_head == NULL)
		return (NULL);
	token = token_list_head;
	while (token->next)
		token = token->next;
	return (token);
}

t_token	*copy_token(t_token *token)
{
	t_token	*dup_token;

	if (!token)
		return (NULL);
	dup_token = new_token();
	if (!dup_token)
		return (NULL);
	dup_token->id = token->id;
	dup_token->str = ft_strdup(token->str);
	if (dup_token->str == NULL)
		return (free(dup_token), NULL);
	dup_token->next = NULL;
	return (dup_token);
}

void	add_token_in_back(t_token **token_list_head, t_token *new_token)
{
	if (!new_token)
		return ;
	if (!(*token_list_head))
	{
		*token_list_head = new_token;
		return ;
	}
	last_token(*token_list_head)->next = new_token;
	return ;
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

t_token	*handle_arith_expan(t_token **head, t_token **cur_open, \
	t_token **cur_close)
{
	t_token	*outer_open;
	t_token	*outer_close;

	while (*cur_open && cur_close && (*cur_open)->id == PAR_OPEN \
		&& (*cur_close)->id == PAR_CLOSE)
	{
		outer_open = (*cur_open);
		outer_close = *cur_close;
		(*cur_open) = previous_token_if_exists(*head, (*cur_open));
		(*cur_close) = (*cur_close)->next;
	}
	if (!safe_assign_str(&outer_open->str, "((") \
	|| !safe_assign_str(&outer_close->str, "))"))
		exit (FAILURE); //!
	outer_open->id = ARITH_EXPAN;
	outer_close->id = ARITH_EXPAN;
	return (*head);
}
