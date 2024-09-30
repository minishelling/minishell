#include "../../include/minishell.h"

int	executor(t_shell *shell, t_cmd *cmds_list)
{
	int	status;

	status = execute_cmd_list(shell, cmds_list);
	return (status);
}

int	execute_cmd_list(t_shell *shell, t_cmd *cmds_list)
{
	size_t		cmds_count;
	size_t		i;
	t_builtin	is_builtin;
	t_cmd		*current_cmd;

	cmds_count = count_cmds(cmds_list);
	i = 0;
	current_cmd = cmds_list;
	while (i < cmds_count)
	{
		if (i < cmds_count - 1)
		{
			if (pipe(shell->pipefd) == -1)
				exit(EXIT_FAILURE); //Print error.
		}
		is_builtin = check_builtin(current_cmd->args[0]);
		// if (is_builtin == NULL_CMD) //commented so testing wouldn't exit minishared
			// exit(EXIT_FAILURE) ;
		if (is_builtin == NON_BUILTIN)
		{
			handle_non_builtin(shell, current_cmd, cmds_count, i);
		}
		else
		{
			handle_builtin(shell, current_cmd, cmds_count, i);
		}
		current_cmd = current_cmd->next;
		i++;
	}
	// close(shell->read_fd);
	return (shell->status);
}

int	handle_non_builtin(t_shell *shell, t_cmd *current_cmd, size_t cmds_count, size_t i)
{
	shell->parent = fork();
	if (shell->parent == -1)
		exit(EXIT_FAILURE); // Print error.
	else if (!shell->parent)
	{
		run_child(shell, current_cmd, cmds_count, i);
	}
	do_parent_duties(shell, cmds_count, i);
	return (SUCCESS);
}

void	run_child(t_shell *shell, t_cmd *current_cmd, size_t cmds_count, size_t current_child)
{
	char	*cmd_path = NULL;
	char	**env_array;

	if (current_cmd->latest_in != STDIN_FILENO)
	{
		if (dup_and_close(current_cmd->latest_in, STDIN_FILENO))
			printf("Dup | Close failed. Exiting now!\n");
	}
	else
	{
		if (current_child > 0)
		{
			if (dup_and_close(shell->read_fd, STDIN_FILENO))
				printf("Dup | Close failed. Exiting now!\n");
		}
	}
	if (current_cmd->latest_out != STDOUT_FILENO)
	{
		if (dup_and_close(current_cmd->latest_out, STDOUT_FILENO))
			printf("Dup | Close failed. Exiting now!\n");
	}
	else
	{
		if (current_child < cmds_count - 1)
		{
			if (dup_and_close(shell->pipefd[WRITE_END], STDOUT_FILENO))
				printf("Dup | Close failed. Exiting now!\n");
		}
	}
	if (current_child < cmds_count - 1)
	{
		if (close(shell->pipefd[READ_END]) == -1)
			return ; //Print error.
	}
	if (current_cmd && current_cmd->args)
		cmd_path = get_cmd_path(shell, current_cmd->args[0]);
	env_array = create_env_array(shell->env_list);
	execve(cmd_path, current_cmd->args, env_array);
	// exit(EXIT_FAILURE); //Print error.
}

void	do_parent_duties(t_shell *shell, size_t cmds_count, size_t current_child)
{
	int	wstatus;

	wstatus = 0;
	if (current_child > 0)
		close(shell->read_fd);
	if (current_child < cmds_count - 1)
	{
		close(shell->pipefd[WRITE_END]);
		shell->read_fd = shell->pipefd[READ_END];
	}
	waitpid(shell->parent, &wstatus, 0);
	shell->status = WEXITSTATUS(wstatus);
}
