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
		return (FAILURE); //Print errno.
	return (SUCCESS);
}

static t_ecode	close_and_replace(int replacement, int *oldfd)
{
	if (close(*oldfd) == -1)
		perror("close");
	*oldfd = replacement;
	return (FAILURE);
}

static t_ecode	replace_redir_fd(t_cmd *cmd, t_redir *redir)
{
	int	status = SUCCESS;

	if (redir->redir_id == HERE || redir->redir_id == IN)
	{
		if (redir->fd == -1)
			return (close_and_replace(redir->fd, &cmd->latest_in));
		else if (cmd->latest_in == STDIN_FILENO)
			cmd->latest_in = redir->fd;
		else
			status = dup_and_close(redir->fd, cmd->latest_in);
	}
	else if (redir->redir_id == OUT || redir->redir_id == APP)
	{
		if (redir->fd == -1)
			return (close_and_replace(redir->fd, &cmd->latest_out));
		else if (cmd->latest_out == STDOUT_FILENO)
			cmd->latest_out = redir->fd;
		else
			status = dup_and_close(redir->fd, cmd->latest_out);
	}
	if (status)
	{
		fprintf(stderr, "DUP|CLOSE Failure in open_redirections.\n");
		return (FAILURE);
	}
	return (SUCCESS);
}

t_ecode	open_redirections(t_shell *shell, t_cmd *current_cmd)
{
	t_redir	*current_redir;

	if (!shell || !current_cmd)
	{
		fprintf(stderr, "Failed to open redirections\n");
		return (NULL_ERROR);
	}
	current_redir = current_cmd->redir;
	while (current_redir)
	{
		if (open_current_redir(current_redir->redir_id, current_redir->file, &current_redir->fd) != SUCCESS)
		{
			ft_putstr_fd("mini_shared: ", 2);
			perror(current_redir->file);
			return (FAILURE);
		}
		if (replace_redir_fd(current_cmd, current_redir) != SUCCESS)
			return (FAILURE);
		current_redir = current_redir->next;
	}
	return (SUCCESS);
}
