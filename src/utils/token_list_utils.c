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

// void	free_token_list(t_token *t_list, t_token *(*f) (t_token *))
// {
// 	if (t_list == NULL)
// 		return ;
// 	while (t_list != NULL)
// 		t_list = f(t_list);
// 	return ;
// }
t_token	*free_token_str(t_token *token)
{
	t_token	*next_token;

	next_token = token->next;
	if (token == NULL)
		return (NULL);
	if (token->str != NULL)
		free(token->str);
	token->str = NULL;
	free(token);
	return (next_token);
}
t_token	*free_token_non_word(t_token *token)
{
	t_token	*current_token;

	current_token = token->next;
	if (token == NULL)
		return (NULL);
	if (token->id != WORD)
		free(token->str);
	free(token);
	return (current_token);
}

void free_token_list(t_token **token_list)
{
	t_token *temp;

	while (*token_list)
	{
		temp = *token_list;
		*token_list = (*token_list)->next;
		if (temp->str)
			free(temp->str);
		free(temp);
	}
	*token_list = NULL;
}
