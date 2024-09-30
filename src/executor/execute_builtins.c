#include "../../include/minishell.h"

int	handle_builtin(t_shell *shell, t_cmd *current_cmd, size_t cmds_count, size_t i)
{
	//MAKE STD_BACKUP
	int	std_backup[2];
	int	status;

	status = create_std_backup(std_backup);
	if (status == DUP_ERROR) //DUP ERROR might be useful for perror-ing. Otherwise FAILURE works.
		exit(EXIT_FAILURE); //Clean and exit.

	//REDIRECT INPUT: FROM LATEST_IN IF AVAILABLE, OTHERWISE FROM PIPE IF WE ARE NOT AT THE FIRST COMMAND.
	if (current_cmd->latest_in != STDIN_FILENO)
	{
		dup2(current_cmd->latest_in, STDIN_FILENO);
		close(current_cmd->latest_in);
	}
	else
	{
		if (i > 0)
		{
			if (dup2(shell->read_fd, STDIN_FILENO) == -1)
				return (1); // Print error.
			if (close(shell->read_fd) == -1)
				return (1); // Print error.
		}
	}

	//STORE READ_END FOR NEXT CMD
	if (i < cmds_count - 1)
		shell->read_fd = shell->pipefd[READ_END];


	//REDIRECT OUTPUT: TO LATEST_OUT IF AVAILABLE,
	if (current_cmd->latest_out != STDOUT_FILENO)
	{
		dup2(current_cmd->latest_out, STDOUT_FILENO);
		// close(current_cmd->latest_out);
		if (i < cmds_count - 1)
		{
			if (close(shell->pipefd[WRITE_END]) == -1)
				return (1); // Print error.
		}
	}
	//OTHERWISE TO PIPE[WRITE END] (IF WE ARE NOT AT THE LAST COMMAND).
	else
	{
		if (i < cmds_count - 1)
		{
			if (dup2(shell->pipefd[WRITE_END], STDOUT_FILENO) == -1)
				return (1); // Print error.
			if (close(shell->pipefd[WRITE_END]) == -1)
				return (1); // Print error.
		}
	}

	shell->status = execute_builtin(shell, current_cmd->args);

	// Bring back the STD_REDIRECTIONS.
	if (dup2(std_backup[STDIN_FILENO], STDIN_FILENO) == -1)
		return (1); // Print error.
	// close(std_backup[STDIN_FILENO]); //Protect
	if (dup2(std_backup[STDOUT_FILENO], STDOUT_FILENO) == -1)
		return (1); // Print error. Should it return or exit?
					// close(std_backup[STDOUT_FILENO]); //Protect
	return (SUCCESS);
}

t_ecode	execute_builtin(t_shell *shell, char **cmd_args)
{
	t_builtin	builtin_code;

	t_ecode	(*builtins_jumptable[])(t_shell *, char **) = {echo_builtin, cd_builtin,
		pwd_builtin, export_builtin, declare_builtin, unset_builtin, env_builtin, exit_builtin};
	if (!shell || !cmd_args)
		return (NULL_ERROR);
	builtin_code = check_builtin(cmd_args[0]);
	if (builtin_code == NULL_CMD)
		return (NULL_ERROR);
	else if (builtin_code == NON_BUILTIN)
		return (PROCEED);
	else
		return (builtins_jumptable[builtin_code](shell, cmd_args));
}