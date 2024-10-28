#include "../../../include/minishell.h"

t_cmd	*new_cmd(void);
t_redir	*new_redir(void);
t_redir	*last_redir(t_redir *list);
void	add_redir_in_back(t_redir **list, t_redir *new_redir);

t_cmd	*new_cmd(void)
{
	t_cmd	*cur_cmd;

	cur_cmd = (t_cmd *)ft_calloc(1, sizeof(t_cmd));
	if (!cur_cmd)
		return (NULL);
	cur_cmd->latest_in = STDIN_FILENO;
	cur_cmd->latest_out = STDOUT_FILENO;
	return (cur_cmd);
}

t_redir	*new_redir(void)
{
	t_redir	*redir;

	redir = malloc(sizeof(t_redir) * 1);
	if (!redir)
		return (NULL);
	redir->redir_id = REDIR;
	redir->file = NULL;
	redir->fd = -1;
	redir->next = NULL;
	return (redir);
}

t_redir	*last_redir(t_redir *list)
{
	t_redir	*current_redir;

	if (!list)
		return (NULL);
	current_redir = list;
	while (current_redir->next != NULL)
		current_redir = current_redir->next;
	return (current_redir);
}

void	add_redir_in_back(t_redir **list, t_redir *new_redir)
{
	t_redir	*current_redir;

	if (!new_redir)
		return ;
	else if (!*list)
	{
		*list = new_redir;
		return ;
	}
	current_redir = last_redir(*list);
	current_redir->next = new_redir;
	return ;
}
