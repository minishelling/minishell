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

t_ecode	open_redirections(t_shell *shell, t_cmd *head)
{
	t_cmd	*current_cmd;
	t_redir	*current_redir;

	if (!shell || !head)
		return (NULL_ERROR);
	current_cmd = head;
	while (current_cmd)
	{
		printf("in open_redirections. current_cmd: %s\n", current_cmd->args[0]);
		current_redir = current_cmd->redir;
		while (current_redir)
		{
			if (open_current_redir(current_redir->redir_id, current_redir->file, &current_redir->fd))
				return (FAILURE);
			else
			{
				current_redir = current_redir->next;
				continue ;
			}
			if (current_redir->redir_id == HERE || current_redir->redir_id == IN)
			{
				if (current_cmd->latest_in != STDIN_FILENO)
				{
					if (dup_and_close(current_redir->fd, current_cmd->latest_in))
						printf("Dup | close failure in open_redirections\n"); //Protect
				}
				else
					current_cmd->latest_in = current_redir->fd;
			}
			else if (current_redir->redir_id == OUT || current_redir->redir_id == APP)
			{
				if (current_cmd->latest_out != STDOUT_FILENO)
				{
					if (dup_and_close(current_redir->fd, current_cmd->latest_out))
						printf("Dup | close failure in open_redirections\n"); //Protect
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
