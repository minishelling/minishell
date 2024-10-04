#include "../../include/minishell.h"

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

size_t	count_cmds(t_cmd *head)
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

t_ecode	create_std_backup(int backup[2])
{
	backup[STDIN_FILENO] = dup(STDIN_FILENO);
	if (backup[STDIN_FILENO] == -1)
		return (DUP_ERROR);
	backup[STDOUT_FILENO] = dup(STDOUT_FILENO);
	if (backup[STDOUT_FILENO] == -1)
		return (DUP_ERROR);
	return (SUCCESS);
}

t_ecode	dup_and_close(int oldfd, int newfd)
{
	int	status;

	fprintf(stderr, "In dup_n_close oldfd: |%d|\n", oldfd);
	fprintf(stderr, "In dup_n_close newfd: |%d|\n", newfd);
	status = dup2(oldfd, newfd);
	if (status == -1)
	{
		perror("dup2");
		return (FAILURE);
	}
	status = close(oldfd);
	if (status == -1)
	{
		perror("close");
		return (FAILURE);
	}
	return (SUCCESS);
}






















// void	redirect_io(t_shell *shell, int io_fd, int io_target)
// {
// 	if (dup2(io_fd, io_target) == -1)
// 		return ; //Error
// 	if (close(io_fd) == -1)
// 		return ; //Closing error. Is minishell still functional?
// }

// void	open_outfile(t_shell *shell)
// {
	
// }