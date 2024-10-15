#include "../../include/minishell.h"

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
	print_env(shell->env_list);
	return (SUCCESS);
}
