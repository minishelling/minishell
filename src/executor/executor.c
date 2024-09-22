#include "../../include/minishell.h"

typedef enum e_builtin
{
	ECHO,
	CD,
	PWD,
	EXPORT,
	DECLARE,
	UNSET,
	ENV,
	EXIT,
	NON_BUILTIN,
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

int	execute_cmd_list(t_shell *shell, t_cmd *cmds_list)
{
	size_t	cmds_count;
	size_t	i;
	t_builtin	is_builtin;
	int		dup_status;

	cmds_count = count_cmds(cmds_list);
	//printf("cmds_count in execute_cmd_list: %ld\n", cmds_count);
	i = 0;
	while (i < cmds_count)
	{
		//Create pipe if not the last cmd
		if (i < cmds_count - 1)
		{
			shell->status = pipe(shell->pipefd);
			if (shell->status == -1)
				return (1);
		}

		is_builtin = check_builtin(cmds_list->args[0]);
		if (is_builtin == NULL_CMD)
			exit(EXIT_FAILURE) ;
		else if (is_builtin == NON_BUILTIN)
		{
			//New process for non-builtin cmds
			shell->parent = fork();
			if (shell->parent == -1)
				return (1);
			else if (!shell->parent)
			{
				// printf("Does it run the child\n");
				run_child(shell, cmds_list, cmds_count, i);
			}
			do_parent_duties(shell, &cmds_list, cmds_count, i);
		}
		else
		{
			//Handling builtins without fork
			if (i > 0)
			{
				dup_status = dup2(shell->read_fd, STDIN_FILENO);
				close(shell->read_fd);
				if (dup_status == -1)
					printf("Dup status: %d\n", dup_status);
			}
			if (i < cmds_count - 1)
			{
				dup_status = dup2(shell->pipefd[WRITE_END], STDOUT_FILENO);
				close(shell->pipefd[WRITE_END]);
				// printf("Dup status: %d\n", dup_status);
				shell->read_fd = shell->pipefd[READ_END];
			}
			shell->status = execute_builtin(shell, cmds_list->args);
			cmds_list = cmds_list->next;
		}
		i++;
	}
	return (WEXITSTATUS(shell->status));
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
		if (status)
			printf("First dup status: %d\n", status);
	}
	
	// printf("Reached checkpoint 1 in run_child\n");
	if (current_child < cmds_count - 1)
	{
		status = close(shell->pipefd[READ_END]); // This should be in the upper one, where the dup is?!!!
		// printf("Close status: %d\n", status);
		status = dup2(shell->pipefd[WRITE_END], STDOUT_FILENO);
		// printf("Second dup status: %d\n", status);
	}
	// printf("Reached checkpoint 2 in run_child\n");
	if (cmds_head && cmds_head->args)
		cmd_path = get_cmd_path(shell, cmds_head->args[0]);
	env_array = create_env_array(shell->env_list);
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
