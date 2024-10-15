#include "../../include/minishell.h"

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

t_redir	*last_redir(t_redir *redir_list_head)
{
	t_redir	*current_redir;
	
	if (redir_list_head == NULL)
		return (NULL);
	current_redir = redir_list_head;
	while (current_redir->next != NULL)
		current_redir = current_redir->next;
	return (current_redir);
}

void	add_redir_in_back(t_redir **redir_list_head, t_redir *new_redir)
{
	t_redir	*current_redir;

	if (!new_redir)
		return ;
	else if (!(*redir_list_head))
	{
		*redir_list_head = new_redir;
		return ;
	}
	current_redir = last_redir(*redir_list_head);
	current_redir->next = new_redir;
	return ;
}

void free_redir_list(t_redir *redir)
{
    t_redir *temp_redir;

    while (redir != NULL)
    {
        temp_redir = redir;
        redir = redir->next;

        // Free the file associated with the redirection
        if (temp_redir->file)
            free(temp_redir->file);

        // Free the redir node itself
        free(temp_redir);
    }
}