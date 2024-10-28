#include "../../../include/minishell.h"

t_token	*new_token(void);
t_token	*last_token(t_token *list);
t_token	*copy_token(t_token *token);
void	add_token_in_back(t_token **list, t_token *new_token);

t_token	*new_token(void)
{
	t_token	*new;

	new = ft_calloc(1, sizeof(t_token));
	if (!new)
		return (NULL);
	new->next = NULL;
	new->id = -1;
	new->str = NULL;
	return (new);
}

t_token	*last_token(t_token *list)
{
	t_token	*cur_token;

	if (!list)
		return (NULL);
	cur_token = list;
	while (cur_token->next)
		cur_token = cur_token->next;
	return (cur_token);
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
	if (!dup_token->str)
		return (free_token(&dup_token), NULL);
	dup_token->next = NULL;
	return (dup_token);
}

void	add_token_in_back(t_token **list, t_token *new_token)
{
	if (!new_token)
		return ;
	if (!(*list))
	{
		*list = new_token;
		return ;
	}
	last_token(*list)->next = new_token;
	return ;
}
