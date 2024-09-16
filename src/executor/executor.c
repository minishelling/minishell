#include "../../include/minishell.h"

// static void	run_child_single_command(t_shell *shell, t_cmd *cmd);

int	executor(t_shell *shell, t_cmd *cmds_list)
{
	int	status;

	if (!ft_strncmp(shell->cmd_list->args[0], "cd", 2))
	{
		printf("Executing cd\n");
		builtin_cd(&shell, shell->cmd_list->args[1]);
	}
	status = execute_cmd_list(shell, cmds_list);
	return (status);
}


// void	execute_single_command(t_shell *shell, t_cmd *cmd)
// {
// 	shell->parent = fork();
// 	if (shell->parent == -1)
// 		return ;
// 	else if (shell->parent == 0)
// 		run_child_single_command(shell, cmd);
// 	waitpid(shell->parent, &shell->status, 0);
// 	// printf("Finished waiting for children. Status: %d\n", shell->status);
// }

static size_t	count_cmds(t_cmd *head)
{
	size_t	count;

	if (!head || !head->args)
		return (0);
	count = 0;
	while (head && head->args)
	{
		count++;
		head = head->next;
	}
	return (count);
}

int	execute_cmd_list(t_shell *shell, t_cmd *cmds_list)
{
	size_t	cmds_count;
	size_t	i;

	cmds_count = count_cmds(cmds_list);
	// printf("cmds_count in execute_cmd_list: %ld\n", cmds_count);
	i = 0;
	while (i < cmds_count)
	{

		//Create pipe
		if (i < cmds_count - 1)
		{
			shell->status = pipe(shell->pipefd);
			if (shell->status == -1)
				return (1);
		}

		//New process
		shell->parent = fork();
		if (shell->parent == -1)
			return (1);
		else if (!shell->parent)
		{
			// printf("Does it run the child\n");
			run_child(shell, cmds_list, cmds_count, i);
		}
		do_parent_duties(shell, &cmds_list, cmds_count, i);
		i++;
	}
	return (0);
}

void	run_child(t_shell *shell, t_cmd *cmds_head, size_t cmds_count, size_t current_child)
{
	char	*cmd_path;
	char	**env_array;
	int		status;

	// printf("Printing cmd list in run child:\n");
	// print_cmd(cmds_head);
	if (current_child > 0)
	{
		status = dup2(shell->read_fd, STDIN_FILENO); //Protect dup.
		// printf("First dup status: %d\n", status);
	}
	
	// printf("Reached checkpoint 1 in run_child\n");
	if (current_child < cmds_count - 1)
	{
		status = close(shell->pipefd[READ_END]);
		// printf("Close status: %d\n", status);
		status = dup2(shell->pipefd[WRITE_END], STDOUT_FILENO);
		// printf("Second dup status: %d\n", status);
	}
	// printf("Reached checkpoint 2 in run_child\n");
	if (cmds_head && cmds_head->args)
		cmd_path = get_cmd_path(shell, cmds_head->args[0]);
	env_array = env_create_array(shell->env_list);
	execve(cmd_path, cmds_head->args, env_array);
	// printf("Failed to execute. Exiting.\n");
	exit(EXIT_FAILURE);
}

void	do_parent_duties(t_shell *shell, t_cmd **curr_cmd, size_t cmds_count, size_t current_child)
{
	if (current_child > 0)
		close(shell->read_fd);
	if (current_child < cmds_count - 1)
	{
		close(shell->pipefd[WRITE_END]);
		shell->read_fd = shell->pipefd[READ_END];
	}
	waitpid(shell->parent, &shell->status, 0);
	*curr_cmd = (*curr_cmd)->next;
}

// static void	run_child_single_command(t_shell *shell, t_cmd *cmd)
// {
// 	char	*cmd_path;
// 	char	**env_array;

// 	// handle_redirections(shell, cmd);
// 	cmd_path = get_cmd_path(shell, shell->cmd_list->args[0]);
// 	env_array = env_create_array(shell->env_list);
// 	execve(cmd_path, cmd->args, env_array);

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
