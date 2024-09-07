#include "../../include/minishell.h"

t_cmd	*new_cmd(void)
{
	t_cmd	*current_cmd;

	current_cmd = malloc(sizeof(t_cmd) * 1);
	if (!current_cmd)
		return (NULL);
	current_cmd->args = NULL;
	current_cmd->redir = NULL;
	current_cmd->next = NULL;
	return (current_cmd);
}

t_cmd	*last_cmd(t_cmd *cmd)
{
	t_cmd	*current_cmd;
	
	if (cmd == NULL)
		return (NULL);
		
	current_cmd = cmd;	
	while (current_cmd->next != NULL)
		current_cmd = current_cmd->next;
	return (current_cmd);
}

void	add_cmd_in_back(t_cmd **cmd_list_head, t_cmd *new_cmd)
{
	t_cmd	*current_cmd;

	if (!new_cmd)
		return ;
	else if (!(*cmd_list_head))
	{
		*cmd_list_head = new_cmd;
		return ;
	}
	current_cmd = last_cmd(*cmd_list_head);
	current_cmd->next = new_cmd;
	return ;
}

t_cmd	*free_cmd(t_cmd *cmd)
{
	t_cmd	*current_cmd;

	current_cmd = cmd->next;
	// if (cmd->args != NULL)
	// 	ft_free_split(cmd->args);
	cmd->args = NULL;
	if (cmd->redir != NULL)
		free_redir_list(cmd->redir);
	cmd->redir = NULL;
	free(cmd);
	return (current_cmd);
}

void	free_cmd_list(t_cmd *cmd_list_head)
{
	if (cmd_list_head == NULL)
		return ;
	while (cmd_list_head != NULL)
		cmd_list_head = free_cmd(cmd_list_head);
	return ;
}