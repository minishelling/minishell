#include "../../include/minishell.h"

t_ecode	exit_builtin(t_shell *shell, char **cmd_args)
{
	(void) shell;
	(void) cmd_args;
	printf("Exit: not implemented yet.\n");
	return (SUCCESS);
}
