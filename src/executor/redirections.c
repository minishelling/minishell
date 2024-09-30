#include "../../include/minishell.h"

static t_ecode	open_current_redir(t_redir_id redir_id, char *redir_file, int *fd)
{
	if (!redir_file)
		return (SUCCESS);
	if (redir_id == HERE)
		*fd = ft_atoi(redir_file);
	else if (redir_id == IN)
		*fd = open(redir_file, O_RDONLY);
	else if (redir_id == OUT)
		*fd = open(redir_file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	else if (redir_id == APP)
		*fd = open(redir_file, O_WRONLY | O_APPEND | O_CREAT, 0644);
	if (*fd == -1)
		return (FAILURE);
	return (SUCCESS);
}

static t_ecode	close_and_replace(int replacement, int *oldfd)
{
	if (close(*oldfd) == -1)
		perror("close");
	*oldfd = replacement;
	return (FAILURE);
}

t_ecode	open_redirections(t_shell *shell, t_cmd *head)
{
	t_cmd	*current_cmd;
	t_redir	*current_redir;
	t_ecode	status = SUCCESS;

	if (!shell || !head)
		return (NULL_ERROR);
	current_cmd = head;
	while (current_cmd)
	{
		current_redir = current_cmd->redir;
		while (current_redir)
		{
			if (open_current_redir(current_redir->redir_id, current_redir->file, &current_redir->fd))
			{
				ft_putstr_fd("mini_shared: ", 2);
				ft_putstr_fd(current_redir->file, 2);
				perror("");
			}
			else
			{
				current_redir = current_redir->next;
				continue ;
			}
			if (current_redir->redir_id == HERE || current_redir->redir_id == IN)
			{
				if (current_redir->fd == -1)
					return (close_and_replace(current_redir->fd, &current_cmd->latest_in)); //Should go to next command actually.
				else if (current_cmd->latest_in == STDIN_FILENO)
					current_cmd->latest_in = current_redir->fd;
				else
					status = dup_and_close(current_redir->fd, current_cmd->latest_in);
			}
			else if (current_redir->redir_id == OUT || current_redir->redir_id == APP)
			{
				if (current_redir->fd == -1)
					return (close_and_replace(current_redir->fd, &current_cmd->latest_out)); //Should go to next command actually.
				else if (current_cmd->latest_out == STDOUT_FILENO)
					current_cmd->latest_out = current_redir->fd;
				else
					status = dup_and_close(current_redir->fd, current_cmd->latest_out);
			}
			if (status)
			{
				printf("DUP|CLOSE Failure in open_redirections.\n");
				return (FAILURE);
			}
			current_redir = current_redir->next;
		}
		current_cmd = current_cmd->next;
	}
	return (SUCCESS);
}
