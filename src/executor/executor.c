#include "../../include/minishell.h"

int	executor(t_shell *shell, t_cmd *cmd)
{
	t_builtin	is_builtin;

	// fprintf(stderr, "Executing cmd: %s, address: %p\n", cmd->args[0], cmd);
	is_builtin = check_builtin(cmd->args[0]);
	if (is_builtin == NULL_CMD)
	{
		fprintf(stderr, "NULL CMD\n");
		// exit(EXIT_FAILURE) ; //commented so testing wouldn't exit minishared
	}
	if (is_builtin == NON_BUILTIN)
	{
		handle_non_builtin(shell, cmd);
	}
	else
	{
		handle_builtin(shell, cmd);
	}
	return (shell->status);
}

void	handle_non_builtin(t_shell *shell, t_cmd *cmd)
{
	shell->parent = fork();
	if (shell->parent == -1)
		exit(EXIT_FAILURE);
	else if (!shell->parent)
	{
		run_child(shell, cmd);
	}
	do_parent_duties(shell);
}

void	run_child(t_shell *shell, t_cmd *cmd)
{
	char	*cmd_path = NULL;
	char	**env_array;
	t_ecode	status = SUCCESS;

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
	if (cmd && cmd->args)
		cmd_path = get_cmd_path(shell, cmd->args[0]);
	env_array = create_env_array(shell->env_list);
	execve(cmd_path, cmd->args, env_array);
	handle_cmd_err(cmd, "command not found");
	exit(EXIT_FAILURE);
}

void	do_parent_duties(t_shell *shell)
{
	int	wstatus;

	wstatus = 0;
	waitpid(shell->parent, &wstatus, 0);
	shell->status = WEXITSTATUS(wstatus);
}
