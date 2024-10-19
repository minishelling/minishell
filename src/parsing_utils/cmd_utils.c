#include "../../include/minishell.h"

/**
 * @brief Allocates and initializes a new command structure.
 * 
 * This function allocates memory for a new `t_cmd` structure and initializes 
 * its fields to default values: `args`, `redir`, and `next` are set to NULL, 
 * while `latest_in` and `latest_out` are initialized to `STDIN_FILENO` and 
 * `STDOUT_FILENO`, respectively. If memory allocation fails, the function 
 * returns NULL.
 * 
 * @return t_cmd* Pointer to the newly allocated and initialized command structure, 
 * or NULL if memory allocation fails.
 */
t_cmd *new_cmd(void)
{
	t_cmd *current_cmd;

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
	if (cmd && *cmd && (*cmd)->args)
		free_args(&(*cmd)->args);
	if ((*cmd)->redir)
		free_redir_list(&(*cmd)->redir);
	free(*cmd);
	*cmd = NULL;
}

