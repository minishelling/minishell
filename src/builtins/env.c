#include "../../include/minishell.h"

/**
 * @brief Prints the environment list.
 * 
 * @param shell A pointer to the shell structure.
 * @param cmd_args An array with the commmand arguments.
 * 
 * @return If the shell structure or the environment list
 * are NULL, or if an option is passed it returns FAILURE.
 * Otherwise it returns SUCCESS.
 */
t_ecode	env_builtin(t_shell *shell, char **cmd_args)
{
	if (!shell || !shell->env_list)
		return (NULL_ERROR);
	if (cmd_args[1])
	{
		handle_builtin_err(cmd_args[0], NULL,
			"options and arguments are not implemented.");
		return (FAILURE);
	}
	print_env_list(shell->env_list);
	return (SUCCESS);
}
