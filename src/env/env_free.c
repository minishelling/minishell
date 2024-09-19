#include "../../include/minishell.h"

//Done
void	free_env_list(t_env	**head)
{
	t_env	*current;
	t_env	*temp;

	if (!head || !*head)
		return ;
	current = *head;
	while (current)
	{
		temp = current;
		current = current->next;
		free_env_node(&temp);
	}
	return ;
}

//Done
void	free_env_node(t_env **node)
{
	if (!*node)
		return ;
	if ((*node)->keyvalue)
		ft_free((void **) &(*node)->keyvalue);
	if ((*node)->key)
		ft_free((void **) &(*node)->key);
	if ((*node)->value)
		ft_free((void **) &(*node)->value);
	ft_free((void **) node);
	return ;
}
