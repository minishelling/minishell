#include "../../include/minishell.h"

t_token	*list_token_new(void)
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

t_token	*list_token_last(t_token *t_list)
{
	if (t_list == NULL)
		return (NULL);
	while (t_list->next != NULL)
		t_list = t_list->next;
	return (t_list);
}

t_token	*list_token_cpy_node(t_token *t_node)
{
	t_token	*t_return;

	if (!t_node)
		return (NULL);
	t_return = list_token_new();
	if (!t_return)
		return (NULL);
	t_return->id = t_node->id;
	t_return->str = ft_strdup(t_node->str);
	if (t_return->str == NULL)
		return (free(t_return), NULL);
	t_return->next = NULL;
	return (t_return);
}

void	list_token_add_back(t_token **t_list, t_token *new)
{
	t_token	*tmp;

	if (!new)
		return ;
	else if (!(*t_list))
	{
		*t_list = new;
		return ;
	}
	tmp = list_token_last(*t_list);
	tmp->next = new;
	return ;
}

//	NAVIGATION FUNCTIONS

t_token	*list_token_skip_space(t_token *t_current)
{
	t_token	*t_return;

	if (t_current == NULL)
		return (NULL);
	t_return = t_current->next;
	if (t_return == NULL)
		return (NULL);
	if (t_return->id == SPACE_CHAR || t_return->id == TAB_CHAR || t_return->id == NL)
		t_return = t_return->next;
	return (t_return);
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

//	FREE FUNCTIONS

t_token	*list_token_free_node_str(t_token *t_node)
{
	t_token	*t_tmp;

	t_tmp = t_node->next;
	if (t_node == NULL)
		return (NULL);
	if (t_node->str != NULL)
		free(t_node->str);
	t_node->str = NULL;
	free(t_node);
	return (t_tmp);
}

t_token	*list_token_free_node_non_word(t_token *t_node)
{
	t_token	*t_tmp;

	t_tmp = t_node->next;
	if (t_node == NULL)
		return (NULL);
	if (t_node->id != WORD)
		free(t_node->str);
	free(t_node);
	return (t_tmp);
}

t_token	*list_token_free_node(t_token *t_node)
{
	t_token	*t_tmp;

	t_tmp = t_node->next;
	if (t_node == NULL)
		return (NULL);
	free(t_node);
	return (t_tmp);
}

void	list_token_free_last(t_token *t_list, t_token *(*f) (t_token *))
{
	t_token	*t_previous;

	if (t_list == NULL)
		return ;
	while (t_list->next != NULL)
	{
		t_previous = t_list;
		t_list = t_list->next;
	}
	t_previous->next = f(t_list);
}

void	list_token_free_list(t_token *t_list, t_token *(*f) (t_token *))
{
	if (t_list == NULL)
		return ;
	while (t_list != NULL)
		t_list = f(t_list);
	return ;
}
