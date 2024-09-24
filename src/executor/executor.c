#include "../../include/minishell.h"

typedef enum e_builtin
{
	ECHO, //0
	CD,
	PWD,
	EXPORT,
	DECLARE,
	UNSET,
	ENV,
	EXIT,
	NON_BUILTIN, //8
	NULL_CMD,
}	t_builtin;

t_builtin	check_builtin(char *cmd_name)
{
	size_t	cmd_name_len;

	if (!cmd_name)
		return (NULL_CMD);
	// printf("CMD_NAME in check_builtin: %s\n", cmd_name);
	cmd_name_len = ft_strlen(cmd_name);
	if (!ft_strncmp(cmd_name, "echo", cmd_name_len))
		return (ECHO);
	else if (!ft_strncmp(cmd_name, "cd", cmd_name_len))
		return (CD);
	else if (!ft_strncmp(cmd_name, "pwd", cmd_name_len))
		return (PWD);
	else if (!ft_strncmp(cmd_name, "export", cmd_name_len))
		return (EXPORT);
	else if (!ft_strncmp(cmd_name, "declare", cmd_name_len))
		return (DECLARE);
	else if (!ft_strncmp(cmd_name, "unset", cmd_name_len))
		return (UNSET);
	else if (!ft_strncmp(cmd_name, "env", cmd_name_len))
		return (ENV);
	else if (!ft_strncmp(cmd_name, "exit", cmd_name_len))
		return (EXIT);
	else
		return (NON_BUILTIN);
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

int	executor(t_shell *shell, t_cmd *cmds_list)
{
	int	status;

	// if (fcntl(cmds_list->latest_in, F_GETFD) == -1)
    // 	perror("Invalid latest_in");
	// else
	// 	printf("Not giving an error \n");
	// if (fcntl(cmds_list->latest_out, F_GETFD) == -1)
	// 	perror("Invalid latest_out");
	// else
	// 	printf("Not giving an error \n");
	status = execute_cmd_list(shell, cmds_list);
	return (status);
}


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

t_ecode	open_redirs(t_shell *shell, t_cmd *head)
{
	t_cmd	*current_cmd;
	t_redir	*current_redir;

	if (!shell || !head)
		return (NULL_ERROR);
	current_cmd = head;
	while (current_cmd)
	{
		current_redir = current_cmd->redir;
		while (current_redir)
		{
			if (current_redir->redir_id == HERE)
				current_redir->fd = ft_atoi(current_redir->file);
			else if (current_redir->redir_id == IN)
				current_redir->fd = open(current_redir->file, O_RDONLY);
			else if (current_redir->redir_id == OUT)
				current_redir->fd = open(current_redir->file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
			else if (current_redir->redir_id == APP)
				current_redir->fd = open(current_redir->file, O_WRONLY | O_APPEND | O_CREAT, 0644);
			else
			{
				current_redir = current_redir->next;
				continue ;
			}
			if (current_redir->fd == -1)
				printf("Problem opening.\n"); //Problem opening. Perror, Alex and good bye.
			if (current_redir->redir_id == HERE || current_redir->redir_id == IN)
			{
				if (current_cmd->latest_in != STDIN_FILENO)
				{
					if (dup2(current_redir->fd, current_cmd->latest_in) == -1)
						printf("Dup failure in open_redirs\n"); //Protect
					// printf("The fd we want to close: %d\n", current_redir->fd);
					// printf("The latest_in fd: %d\n", current_cmd->latest_in);
					// if (close(current_redir->fd) == -1)
					// 	printf("It isn't closing\n"); //Protect
					// if (fcntl(current_cmd->latest_in, F_GETFD) == -1)
					// 	perror("Invalid current_cmd->latest_in before dup2");
					// if (close(current_cmd->latest_in) == -1) //Not recycling fds.
						// printf("It isn't closing\n"); //Protect
				}
				else
					current_cmd->latest_in = current_redir->fd;
			}
			else if (current_redir->redir_id == OUT || current_redir->redir_id == APP)
			{
				if (current_cmd->latest_out != STDOUT_FILENO)
				{
					if (dup2(current_redir->fd, current_cmd->latest_out) == -1)
						printf("Dup failure in open_redirs\n"); //Protect
					if (close(current_redir->fd) == -1)
						printf("It isn't closing\n"); //Protect
					// if (close(current_cmd->latest_out) == -1) //Not recycling fds.
					// 	printf("It isn't closing\n"); //Protect
				}
				else
					current_cmd->latest_out = current_redir->fd;
			}
			current_redir = current_redir->next;
		}
		current_cmd = current_cmd->next;
	}
	return (SUCCESS);
}

// t_ecode	handle_redirs(t_shell *shell, t_cmd *head)
// {
// 	t_cmd	*current_cmd;

// 	if (!shell || !head)
// 		return (NULL_ERROR);
// 	current_cmd = head;
	
// }

int	execute_cmd_list(t_shell *shell, t_cmd *cmds_list)
{
	size_t		cmds_count;
	size_t		i;
	t_builtin	is_builtin;
	t_cmd		*current_cmd;

	cmds_count = count_cmds(cmds_list);
	// printf("cmds_count in execute_cmd_list: %ld\n", cmds_count);
	i = 0;
	current_cmd = cmds_list;
	while (i < cmds_count)
	{
		//Create pipe if not the last cmd
		if (i < cmds_count - 1)
		{
			if (pipe(shell->pipefd) == -1)
				return (1); //Print error.
		}

		is_builtin = check_builtin(current_cmd->args[0]);
		// printf("is_builtin: %d\n", is_builtin);

		// if (is_builtin == NULL_CMD) //commented so testing wouldn't exit minishared
			// exit(EXIT_FAILURE) ;

		if (is_builtin == NON_BUILTIN)
		{
			//New process for non-builtin cmds
			shell->parent = fork();
			if (shell->parent == -1)
				return (1); //Print error.
			else if (!shell->parent)
			{
				// printf("Does it run the child\n");
				run_child(shell, current_cmd, cmds_count, i);
			}
			do_parent_duties(shell, &current_cmd, cmds_count, i);
		}
		else
		{
			handle_builtin(shell, current_cmd, cmds_count, i);
			current_cmd = current_cmd->next;
		}
		i++;
	}
	// close(shell->read_fd);
	return (shell->status);
}

int	handle_builtin(t_shell *shell, t_cmd *current_cmd, size_t cmds_count, size_t i)
{
	//MAKE STD_BACKUP
	int	std_backup[2];

	std_backup[STDIN_FILENO] = dup(STDIN_FILENO);
	std_backup[STDOUT_FILENO] = dup(STDOUT_FILENO);

	//REDIRECT INPUT: FROM LATEST_IN IF AVAILABLE, OTHERWISE FROM PIPE IF WE ARE NOT AT THE FIRST COMMAND.
	if (current_cmd->latest_in != STDIN_FILENO)
	{
		dup2(current_cmd->latest_in, STDIN_FILENO);
		// close(current_cmd->latest_in);
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

void	run_child(t_shell *shell, t_cmd *current_cmd, size_t cmds_count, size_t current_child)
{
	char	*cmd_path = NULL;
	char	**env_array;

	if (current_cmd->latest_in != STDIN_FILENO)
	{
		if (dup2(current_cmd->latest_in, STDIN_FILENO) == -1)
		{
			printf("dup2 failed 1\n");
			perror("dup2:");
			printf("current_cmd in run_child: %s\n", current_cmd->args[0]);
			printf("latest_in in run_child: %d\n", current_cmd->latest_in);
			return ;
		}
		// close(current_cmd->latest_in);
	}
	else
	{
		if (current_child > 0)
		{
			if (dup2(shell->read_fd, STDIN_FILENO) == -1)
			{
				printf("dup2 failed 2\n");
				return;
			}
			close (shell->read_fd);
		}
	}
	if (current_cmd->latest_out != STDOUT_FILENO)
	{
		if (dup2(current_cmd->latest_out, STDOUT_FILENO) == -1)
		{
			printf("dup2 failed 3\n");
			return;
		}
		// close(current_cmd->latest_out);
	}
	else
	{
		if (current_child < cmds_count - 1)
		{
			if (dup2(shell->pipefd[WRITE_END], STDOUT_FILENO) == -1)
			{
				printf("dup2 failed 4\n");
				return;
			}
			close(shell->pipefd[WRITE_END]);
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
	// printf("Cat is saying all of this:\n");
	execve(cmd_path, current_cmd->args, env_array);
	// exit(EXIT_FAILURE); //Print error.
}

void	do_parent_duties(t_shell *shell, t_cmd **curr_cmd, size_t cmds_count, size_t current_child)
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
	*curr_cmd = (*curr_cmd)->next;
}
