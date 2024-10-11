#include "../../include/minishell.h"

int	executor(t_shell *shell, t_cmd *cmd)
{
	t_builtin	is_builtin;
	
	if (!cmd->args[0])
	{
		shell->exit_code = SUCCESS;
		return (shell->exit_code);
	}
	is_builtin = check_builtin(cmd->args[0]);
	if (is_builtin == NON_BUILTIN)
		handle_non_builtin(shell, cmd);
	else
		handle_builtin(shell, cmd);
	return (shell->exit_code);
}

void	handle_non_builtin(t_shell *shell, t_cmd *cmd)
{
	shell->parent = fork();
	if (shell->parent == -1)
		exit(EXIT_FAILURE);
	else if (!shell->parent)
		run_child(shell, cmd);
	do_parent_duties(shell, cmd);
}

t_ecode	handle_redirs_in_child(t_cmd *cmd)
{
	if (cmd->latest_in == ERROR || cmd->latest_in == ERROR)
		exit(EXIT_FAILURE);
	if (cmd->latest_in != STDIN_FILENO)
	{
		if (dup_and_close(cmd->latest_in, STDIN_FILENO))
			exit(EXIT_FAILURE);
	}
	if (cmd->latest_out != STDOUT_FILENO)
	{
		if (dup_and_close(cmd->latest_out, STDOUT_FILENO))
			exit(EXIT_FAILURE);
	}
	return (SUCCESS);
}

void	run_child(t_shell *shell, t_cmd *cmd)
{
	char	*cmd_path;
	char	**env_array;

	init_signals(CHILD_NON_INTERACTIVE);
	handle_redirs_in_child(cmd);
	if (cmd && cmd->args)
		cmd_path = get_cmd_path(shell, cmd->args[0]);
	if (!cmd_path && shell->exit_code == 126)
	{
		handle_cmd_err(cmd, strerror(EACCES));
		exit(EXIT_CMD_NOT_EXECUTABLE);
	}
	env_array = create_env_array(shell->env_list);
	execve(cmd_path, cmd->args, env_array);
	handle_cmd_err(cmd, strerror(ENOENT));
	exit(EXIT_CMD_NOT_FOUND);
}

void	do_parent_duties(t_shell *shell, t_cmd *cmd)
{
	int	wstatus;

	waitpid(shell->parent, &wstatus, 0);
	if (WIFEXITED(wstatus) == true)
	{
		if (g_signalcode == SIGINT)
			shell->exit_code = EXIT_SIGINT;
		else	
		shell->exit_code = WEXITSTATUS(wstatus);
	}
	else if (WIFSIGNALED(wstatus) == true)
		shell->exit_code = EXIT_SIGQUIT;
	close_all_fds_in_cmd(cmd);
}
