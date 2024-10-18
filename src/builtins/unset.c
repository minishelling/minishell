#include "../../include/minishell.h"

/**
 * @brief Removes an environment node from the environment list.
 * 
 * @param shell A pointer to the shell structure.
 * @param cmd_args The array of arguments of the given command.
 * 
 * @return On success it returns 0.
 * Failure only happens when a parameter is given as NULL.
 */
t_ecode	unset_builtin(t_shell *shell, char **cmd_args)
{
	char	*key;
	t_env	*env_node;
	t_env	*previous_node;

	if (!shell || !cmd_args)
		return (NULL_ERROR);
	if (!cmd_args[1])
		return (SUCCESS);
	key = cmd_args[1];
	env_node = find_env_node(shell->env_list, key);
	if (!env_node)
		return (SUCCESS);
	previous_node = shell->env_list;
	while (previous_node && previous_node->next != env_node)
		previous_node = previous_node->next;
	previous_node->next = env_node->next;
	free_env_node(&env_node);
	return (SUCCESS);
}
