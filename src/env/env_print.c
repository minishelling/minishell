#include "../../include/minishell.h"

//Done
void	print_env_node(t_env *node)
{
	if (!node)
		return ;
	else
	{
		if (node->key)
			ft_printf("Key: %s\n", node->key);
		else
			ft_printf("No key\n");
		if (node->value)
			ft_printf("Value: %s\n\n\n", node->value);
		else
			ft_printf("No value\n");
		if (node->keyvalue)
			ft_printf("Key-value: %s\n", node->keyvalue);
		else
			ft_printf("No key-value\n");
	}
	return ;
}

//Done
void	print_env_list(t_env *head)
{
	if (!head)
		return ;
	while (head)
	{
		print_env_node(head);
		head = head->next;
	}
	return ;
}
