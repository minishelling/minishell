#include "../../include/minishell.h"

/**
 * @brief Removes specified environment nodes from the environment list.
 * 
 * This function iterates over the command arguments and searches for 
 * corresponding environment nodes within the shell's environment list. 
 * When a matching node is found, it is removed from the list, updating 
 * any links in the list to maintain continuity. If the node to remove 
 * is the head of the list, the head is updated accordingly.
 * 
 * @param[in,out] shell A pointer to the shell structure that holds the 
 *                      environment list.
 * @param[in] cmd_args The array of command arguments specifying 
 *                     environment variables to remove.
 * 
 * @return On success, returns 0. Returns NULL_ERROR if any parameter is NULL.
 */
t_ecode	unset_builtin(t_shell *shell, char **cmd_args)
{
	t_env	*env_node;
	t_env	*previous_node;
	size_t	i;

	if (!shell || !cmd_args)
		return (NULL_ERROR);
	i = 1;
	while (cmd_args[i])
	{
		env_node = find_env_node(shell->env_list, cmd_args[i]);
		if (env_node)
		{
			previous_node = shell->env_list;
			while (previous_node && previous_node->next != env_node)
				previous_node = previous_node->next;
			if (previous_node)
				previous_node->next = env_node->next;
			if (env_node == shell->env_list)
				shell->env_list = shell->env_list->next;
			free_env_node(&env_node);
		}
		i++;
	}
	return (SUCCESS);
}
