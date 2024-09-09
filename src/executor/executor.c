#include "../../include/minishell.h"

static void	run_child_single_command(t_shell *shell, t_cmd *cmd);

void	executor(t_shell *shell)
{
	// int		status;
	// pid_t	pid;

	
	// printf("In executor\n");
	// pid = fork();
	// if (pid == -1)
	// 	return ;
	// if (pid == 0)
	execute_single_command(shell, shell->cmd_list);
	// status = execve(cmd_path, shell->cmd_list->args, env_array);
	// perror("Errno: ");
	// printf("Status: %d\n", status);

	
	// ft_print_2d_arr(env_array);
}


void	execute_single_command(t_shell *shell, t_cmd *cmd)
{
	shell->parent = fork();
	if (shell->parent == -1)
		return ;
	else if (shell->parent == 0)
		run_child_single_command(shell, cmd);
	waitpid(shell->parent, &shell->status, 0);
	printf("Finished waiting for children. Status: %d\n", shell->status);
}

// void	execute_two_commands(t_shell *shell, t_cmd *cmd)
// {
// 	shell->status = pipe(shell->pipefd);
// 	if (shell->status == -1)
// 		return (-1);
// 	shell->parent = fork();
// 	if (shell->parent == -1)
// 		return (-1);
// 	else if (shell->parent == 0)
// 		run_child(shell, cmd);
// 	close(shell->pipefd[WRITE_END]);
// 	while (wait(NULL) != -1)
// 		;
// 	printf("Finished waiting for children, returning now.\n");
// 	return (0);
// }

static void	run_child_single_command(t_shell *shell, t_cmd *cmd)
{
	char	*cmd_path;
	char	**env_array;

	// handle_redirections(shell, cmd);
	cmd_path = get_cmd_path(shell, shell->cmd_list->args[0]);
	env_array = env_create_array(shell->env_list);
	execve(cmd_path, cmd->args, env_array);

}

// void	run_child_one(t_shell *shell, t_cmd *cmd)
// {
// 	char	*cmd_path;
// 	char	**env_array;

// 	handle_redirections(shell, cmd);
// 	cmd_path = get_cmd_path(shell, shell->cmd_list->args[0]);
// 	env_array = env_create_array(shell->env_list);
// 	execve(cmd_path, cmd->args, env_array);
// 	exit(EXIT_FAILURE);
// }

// void	run_child_two(t_shell *shell, t_cmd *cmd)
// {
// 	char	*cmd_path;
// 	char	**env_array;

// 	handle_redirections(shell, cmd);
// 	cmd_path = get_cmd_path(shell, shell->cmd_list->args[0]);
// 	env_array = env_create_array(shell->env_list);
// 	execve(cmd_path, cmd->args, env_array);
// 	exit(EXIT_FAILURE);
// }

// void	handle_redirections(t_shell *shell, t_cmd *cmd)
// {
// 	int	input_fd;

// 	close(shell->pipefd[READ_END]);
// 	input_fd = handle_input(shell, cmd);
// 	dup2(input_fd, STDIN_FILENO);
// 	close(input_fd);
// 	dup2(shell->pipefd[WRITE_END], STDOUT_FILENO);
// 	close(shell->pipefd[WRITE_END]);
// }

// int	handle_input(t_shell *shell, t_cmd *cmd)
// {
// 	t_redir	*iterator;
// 	char	*infile_name;
// 	int		infile_fd;

// 	(void) shell;
// 	if (!cmd || cmd->redir)
// 		return (0);
// 	iterator = cmd->redir;
// 	while (iterator->redir)
// 	{
// 		if (iterator->redir == IN || iterator->redir == HERE) //Change HERE to HDOC
// 			infile_name = iterator->file;
// 		iterator = iterator->next;
// 	}
// 	infile_fd = open(infile_name, O_CREAT);
// 	return (infile_fd);
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
