#include "../../include/minishell.h"

/**
 * @brief Prints all the variables in an environment node, if they exist.
 * It prints a default message if the string was NULL.
 * 
 * This function is used for debugging purposes.
 * 
 * @param node The node whose variables need to be printed.
 * 
 * @return void
 */
void	print_env_node_debugging_debugging(t_env *node)
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
 * @brief Prints the variables in an environment node in a formatted way.
 * 
 * It prints the key string, followed by a '=' and then followed by the value.
 * 
 * 
 * 
 * @param node The node whose variables need to be printed.
 * 
 * @return void
 */
void	print_env_node(t_env *node)
{
	char	*str;
	size_t	str_len;

	if (!node)
		return ;
	str_len = ft_strlen(node->key) + ft_strlen("=") + ft_strlen(node->value);
	str = ft_calloc(str_len + 1, sizeof(char));
	if (!str)
	{
		handle_perror("print_env_node");
		return ;
	}
	ft_strlcpy(str, node->key, ft_strlen(node->key) + 1);
	ft_strlcat(str, "=", str_len + 1);
	ft_strlcat(str, node->value, str_len + 1);
	ft_putstr_fd(str, STDOUT_FILENO);
	ft_putchar_fd('\n', STDOUT_FILENO);
	ft_free((void **) &str);
	return ;
}

/**
 * @brief Prints the variables in all nodes of the environment list.
 * 
 * This function is used for debugging purposes.
 * 
 * @param head The head node of the environment list.
 * 
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
