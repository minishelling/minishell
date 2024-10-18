#include "../../include/minishell.h"

/**
 * @brief Frees all environment nodes with its variables.
 * It sets all nodes addresses to NULL.
 * 
 * @param head A pointer to the address of the head node.
 * 
 * @return void
 */
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

/**
 * @brief Frees the variables in an environment node, sets the to NULL
 * and frees the node itself while also setting it to NULL afterwards.
 * 
 * @param node The environment node to be freed.
 * 
 * @return void
 */
void	free_env_node(t_env **node)
{
	if (!(*node))
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
