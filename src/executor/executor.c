#include "../../include/minishell.h"

void	executor(t_shell *shell)
{
	size_t	cmds_nbr;
	size_t	i;

	if (!shell->cmd_list)
		return ; //Null
	else
	{
		i = 0;
		cmds_nbr = cmd_size(shell->cmd_list);
		while (i < cmds_nbr)
		{
			if (i < cmds_nbr - 1 && pipe(shell->pipefd) == -1)
				return ; // Pipe failure
			shell->parent = fork();
			if (shell->parent == -1)
				return ; //Fork failure
			else if (!shell->parent)
				run_child(shell, i);
			do_parent_duties(shell, i);
			i++;
		}
		waitpid(shell->parent, &shell->status, 0);
		while (wait(NULL) != -1)
			;
		// print_cmd(shell->cmd_list);
	}
}

void	run_child(t_shell *shell, size_t i, size_t cmds_nbr)
{
	if (i < cmds_nbr - 1)
	{
		close (shell->pipefd[READ_END]); //Protect.

		// Check the command. If there is a redirect, redirect to the redirect (if outfile).
		// If there is no redirect, then redirect to the pipe,
		// unless it's the last command.

		
		redirect_io(shell, shell->pipefd[WRITE_END], STDOUT_FILENO);
	}
}
