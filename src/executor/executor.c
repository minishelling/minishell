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
	init_signals(PARENT_IGNORE);
	if (shell->parent == -1)
		exit(EXIT_FAILURE);
	else if (!shell->parent)
		run_child(shell, cmd);
	do_parent_duties(shell, cmd);
}

void	run_child(t_shell *shell, t_cmd *cmd)
{
	char	*cmd_path = NULL;
	char	**env_array;
	t_ecode	status = SUCCESS;

	init_signals(CHILD_NON_INTERACTIVE);
	if (cmd->latest_in == ERROR || cmd->latest_in == ERROR)
		exit(EXIT_FAILURE);
	if (cmd->latest_in != STDIN_FILENO)
		status = dup_and_close(cmd->latest_in, STDIN_FILENO);
	if (status)
	{
		ft_putstr_fd("Dup error in run_child |->STDIN|\n", 2);
		return ;
	}
	if (cmd->latest_out != STDOUT_FILENO)
		status = dup_and_close(cmd->latest_out, STDOUT_FILENO);
	if (status)
	{
		ft_putstr_fd("Dup error in run_child |->STDOUT|\n", 2);
		return ;
	}
	// fprintf(stderr, "How come it prints this?\n");
	if (cmd && cmd->args)
	{
		// fprintf(stderr, "And this?\n");
		cmd_path = get_cmd_path(shell, cmd->args[0]);
		// fprintf(stderr, "But it doesnt print this???\n");
	}
	// fprintf(stderr, "cmd_path: %s\n", cmd->args[0]);
	env_array = create_env_array(shell->env_list);
	execve(cmd_path, cmd->args, env_array);
	handle_cmd_err(cmd, "command not found");
	exit(127);
}

void	do_parent_duties(t_shell *shell, t_cmd *cmd)
{
	int	wstatus;

	init_signals(PARENT_NON_INTERACTIVE);
	waitpid(shell->parent, &wstatus, 0);
	if (WIFEXITED(wstatus) == true)
	{
		if (g_signalcode == SIGINT)
			shell->exit_code = EXIT_SIGINT;
		else	
		shell->exit_code = WEXITSTATUS(wstatus);
	}
	else if (WIFSIGNALED(wstatus) == true)
	{
		shell->exit_code = EXIT_SIGQUIT;
	}
	fprintf(stderr, "Parent duties:\nSignal code: %d\n", g_signalcode);
	printf ("Exit code: %d\n", shell->exit_code);
	close_all_fds_in_cmd(cmd);
}
