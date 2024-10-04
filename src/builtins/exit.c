#include "../../include/minishell.h"

t_ecode	exit_builtin(t_shell *shell, char **cmd_args)
{
	(void) shell;
	(void) cmd_args;
	ft_putstr_fd("exit\n", 2);
	exit(SUCCESS);
	return (SUCCESS);
}
