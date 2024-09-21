#include "../../include/minishell.h"

t_ecode	env_builtin(t_shell *shell, char **cmd_args)
{
	if (!shell || !shell->env_list)
		return (NULL_ERROR);
	(void) cmd_args;
	print_env(shell->env_list);
	return (SUCCESS);
}
