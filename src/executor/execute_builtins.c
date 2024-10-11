#include "../../include/minishell.h"

void	handle_builtin(t_shell *shell, t_cmd *cmd)
{
	int	std_backup[2];

	if (create_std_backup(std_backup))
		exit(EXIT_FAILURE); // Clean nicely and exit.
	if (cmd->latest_in != STDIN_FILENO)
	{
		if (dup_and_close(cmd->latest_in, STDIN_FILENO))
			exit(EXIT_FAILURE); // Clean nicely and exit.
	}
	if (cmd->latest_out != STDOUT_FILENO)
	{
		if (dup_and_close(cmd->latest_out, STDOUT_FILENO))
			exit(EXIT_FAILURE); // Clean nicely and exit.
	}
	shell->exit_code = execute_builtin(shell, cmd->args);
	if (dup_and_close(std_backup[STDIN_FILENO], STDIN_FILENO))
		exit(EXIT_FAILURE); // Clean nicely and exit.
	if (dup_and_close(std_backup[STDOUT_FILENO], STDOUT_FILENO))
		exit(EXIT_FAILURE); // Clean nicely and exit.
}

t_ecode	execute_builtin(t_shell *shell, char **cmd_args)
{
	t_builtin	builtin_code;

	t_ecode	(*builtins_jumptable[])(t_shell *, char **) = {echo_builtin,
		cd_builtin,	pwd_builtin, export_builtin, declare_builtin,
		unset_builtin, env_builtin, exit_builtin};
	if (!shell || !cmd_args)
		return (NULL_ERROR);
	builtin_code = check_builtin(cmd_args[0]);
	if (builtin_code == NON_BUILTIN)
		return (PROCEED);
	else
		return (builtins_jumptable[builtin_code](shell, cmd_args));
}
