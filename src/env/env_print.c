#include "../../include/minishell.h"

void	env_print_node(t_env *node)
{
	if (!node)
		return ;
	else
	{
		if (node->keyvalue)
			ft_printf("Key-value: %s\n", node->keyvalue);
		else
			ft_printf("No key-value\n");
		if (node->key)
			ft_printf("Key: %s\n", node->key);
		else
			ft_printf("No key\n");
		if (node->value)
			ft_printf("Value: %s\n\n\n", node->value);
		else
			ft_printf("No value\n");
	}
}

void	env_print_list(t_env *head)
{
	printf ("Lisandro's\n");
	if (!head)
		return ;
	while(head != NULL)
	{
		ft_printf("Key-value: %s\n", head->keyvalue);
		ft_printf("Key: %s\n", head->key);
		ft_printf("Value: %s\n\n\n", head->value);
		head = head->next;
	}
}
