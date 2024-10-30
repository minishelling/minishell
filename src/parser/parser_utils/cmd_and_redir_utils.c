#include "../../../include/minishell.h"

t_cmd	*new_cmd(void);
t_redir	*new_redir(void);
t_redir	*last_redir(t_redir *list);
void	add_redir_in_back(t_redir **list, t_redir *new_redir);

/**
 * @brief Creates a new command structure with default file descriptors.
 *
 * Allocates memory for a `t_cmd` structure and initializes its latest
 * input and output to standard input and output file descriptors.
 *
 * @return Pointer to the newly allocated `t_cmd` structure, or NULL if
 *         memory allocation fails.
 */
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

/**
 * @brief Creates a new redirection structure with default values.
 *
 * Allocates memory for a `t_redir` structure, setting its redirection
 * ID to `REDIR`, file pointer to NULL, file descriptor to -1, and
 * next pointer to NULL.
 *
 * @return Pointer to the newly allocated `t_redir` structure, or NULL if
 *         memory allocation fails.
 */
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

/**
 * @brief Finds the last redirection node in a list.
 *
 * Traverses a linked list of `t_redir` structures to return the last node.
 *
 * @param list Pointer to the head of the redirection list.
 * @return Pointer to the last redirection node, or NULL if the list is empty.
 */
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

/**
 * @brief Adds a redirection node to the end of a redirection list.
 *
 * Inserts a new redirection node at the back of a linked list of
 * `t_redir` structures. If the list is empty, the new node becomes the head.
 *
 * @param list Pointer to the head of the redirection list.
 * @param new_redir Pointer to the new redirection node to be added.
 */
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
