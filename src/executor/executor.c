#include "../../include/minishell.h"

void	executor(t_shell *shell)
{
	int		status;
	char	*cmd_path;
	// pid_t	pid;
	char	**env_array;

	env_array = env_create_array(shell->env_list);
	cmd_path = get_cmd_path(shell, shell->cmd_list->args[0]);
	// printf("In executor\n");
	// pid = fork();
	// if (pid == -1)
	// 	return ;
	// if (pid == 0)
	printf("Cmd_path: %s\n", cmd_path);
	status = execve(cmd_path, shell->cmd_list->args, env_array);
	// perror("Errno: ");
	printf("Status: %d\n", status);
	ft_free((void **) &cmd_path);
	ft_free_2d((void ***) &env_array);

	
	// ft_print_2d_arr(env_array);
}


// int	execute_single_command(t_shell *shell, char *cmd_name)
// {

// }












// void	executor(t_shell *shell)
// {
// 	size_t	cmds_nbr;
// 	size_t	i;

// 	if (!shell->cmd_list)
// 		return ; //Null
// 	else
// 	{
// 		i = 0;
// 		cmds_nbr = cmd_size(shell->cmd_list);
// 		while (i < cmds_nbr)
// 		{
// 			if (i < cmds_nbr - 1 && pipe(shell->pipefd) == -1)
// 				return ; // Pipe failure
// 			shell->parent = fork();
// 			if (shell->parent == -1)
// 				return ; //Fork failure
// 			else if (!shell->parent)
// 				run_child(shell, i);
// 			do_parent_duties(shell, i);
// 			i++;
// 		}
// 		waitpid(shell->parent, &shell->status, 0);
// 		while (wait(NULL) != -1)
// 			;
// 		// print_cmd(shell->cmd_list);
// 	}
// }

// void	run_child(t_shell *shell, size_t i, size_t cmds_nbr)
// {
// 	if (i == 0)
// 	{
// 		redirect_input(t_shell *shell, )
// 	}

// 	if (i < cmds_nbr - 1)
// 	{
// 		close (shell->pipefd[READ_END]); //Protect.

// 		// Check the command. If there is a redirect, redirect to the redirect (if outfile).
// 		// If there is no redirect, then redirect to the pipe,
// 		// unless it's the last command.

		
// 		redirect_io(shell, shell->pipefd[WRITE_END], STDOUT_FILENO);
// 	}
// 	else
// 	{
// 		open_outfile(t_shell *shell);
// 		redirect_io(shell, shell->)
// 	}
// }
