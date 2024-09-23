#include "../../include/minishell.h"

t_ecode	env_builtin(t_shell *shell, char **cmd_args)
{
	if (!shell || !shell->env_list)
		return (NULL_ERROR);
	if (cmd_args[1])
	{
		ft_putstr_fd("env: options and arguments are not implemented.\n", 2);
		return (FAILURE);
	}
	print_env(shell->env_list);
	return (SUCCESS);
}
