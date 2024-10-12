#include "../../include/minishell.h"

/**
 * @brief Prints all the variables in an environment node, if they exist.
 * It prints a default message if the string was NULL.
 * @param node The node whose variables need to be printed.
 * @return void
 */
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

/**
 * @brief Prints the variables in all nodes of the environment list.
 * 
 * @param head The head node of the environment list.
 * @return void
 */
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
