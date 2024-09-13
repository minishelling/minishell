#include "../../include/minishell.h"

void	env_free_list(t_env	**head)
{
	t_env	*current;
	t_env	*temp;

	if (!head || !*head)
		return ;
	current = *head;
	while (current != NULL)
	{
		free(current->keyvalue);
		free(current->key);
		free(current->value);
		temp = current;
		current = current->next;
		free(temp);
	}
	free(current);
}
