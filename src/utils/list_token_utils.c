#include "../../include/minishell.h"

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
	t_token *token;

	if (token_list_head == NULL)
		return (NULL);

	token = token_list_head;
	while (token->next != NULL)
		token = token->next;
	return (token);
}

t_token	*copy_token(t_token *token)
{
	t_token	*return_token;

	if (!token)
		return (NULL);
	return_token = new_token();
	if (!return_token)
		return (NULL);
	return_token->id = token->id;
	return_token->str = ft_strdup(token->str);
	if (return_token->str == NULL)
		return (free(return_token), NULL);
	return_token->next = NULL;
	return (return_token);
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

//	NAVIGATION FUNCTIONS

t_token	*skip_space_token(t_token *token)
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

t_token	*list_token_skip_pipe(t_token *t_current)
{
	t_token	*t_previous;

	if (t_current == NULL)
		return (NULL);
	while (t_current != NULL)
	{
		t_previous = t_current;
		t_current = t_current->next;
		if (t_previous->id == PIPE)
			break ;
	}
	return (t_current);
}

t_token	*free_token_node(t_token *t_node)
{
	t_token	*t_tmp;

	t_tmp = t_node->next;
	if (t_node == NULL)
		return (NULL);
	free(t_node);
	return (t_tmp);
}

void	free_last_token(t_token *token, t_token *(*f) (t_token *))
{
	t_token	*previous_token;

	if (token == NULL)
		return ;
	while (token->next != NULL)
	{
		previous_token = token;
		token = token->next;
	}
	previous_token->next = f(token);
}

void	free_token_list(t_token *t_list, t_token *(*f) (t_token *))
{
	if (t_list == NULL)
		return ;
	while (t_list != NULL)
		t_list = f(t_list);
	return ;
}
