#include "../../include/minishell.h"


t_cmd *new_cmd(void)
{
	t_cmd *cur_cmd;

	cur_cmd = malloc(sizeof(t_cmd) * 1);
	if (!cur_cmd)
		return (NULL);
	cur_cmd->args = NULL;
	cur_cmd->redir = NULL;
	cur_cmd->next = NULL;
	cur_cmd->latest_in = STDIN_FILENO;
	cur_cmd->latest_out = STDOUT_FILENO;
	return (cur_cmd);
}

/**
 * @brief Frees an array of strings and sets it to NULL.
 * 
 * This function frees each string within the array pointed to by `args`, 
 * then frees the array itself, and finally sets the array pointer to NULL.
 * 
 * @param args A pointer to an array of strings (char ***).
 */
void free_args(char ***args)
{
	int i;

	if (!(*args))
		return;
	i = 0;
	while ((*args)[i])
	{
		free((*args)[i]);
		(*args)[i] = NULL;
		i++;
	}
	free(*args);
	*args = NULL;
}

void free_cmd(t_cmd **cmd)
{
	if (*cmd == NULL)
		return;
	if ((*cmd)->args)
		free_args(&(*cmd)->args);
	if ((*cmd)->redir)
		free_redir_list(&(*cmd)->redir);
	free(*cmd);
	*cmd = NULL;
}

