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
	current_cmd->latest_in = STDIN_FILENO;
	current_cmd->latest_out = STDOUT_FILENO;
	return (current_cmd);
}

void free_args(char ***args)
{
    int i;

    if (*args == NULL)
        return;

    i = 0;
    while ((*args)[i])
    {
        free((*args)[i]);
        i++;
    }
    free(*args);
	*args = NULL;
}

t_cmd	*free_cmd(t_cmd *cmd)
{
	t_cmd	*current_cmd;

	current_cmd = cmd->next;
	if (cmd->args)
		free_args(&cmd->args);
	cmd->args = NULL;
	if (cmd->redir != NULL)
		free_redir_list(cmd->redir);
	cmd->redir = NULL;
	free(cmd);
	return (current_cmd);
}
