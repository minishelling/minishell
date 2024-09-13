#include "../../include/minishell.h"

/*	STATIC FUNCTION DECLARATIONS	*/

static t_env	*env_new_copied_node(char *keyvalue, t_ecode *status);

/*	FUNCTIONS */

t_ecode	env_init_list(t_env **head, char **envp)
{
	t_env		*current;
	t_env		*new_node;
	t_ecode	status;	
	size_t		i;

	if (!envp || !*envp)
		return (NULL_ERROR);
	i = 0;
	while (envp[i])
	{
		new_node = env_new_copied_node(envp[i], &status);
		if (status != SUCCESS)
			return (status);
		if (!*head)
		{
			*head = new_node;
			current = *head;
		}
		else
		{
			current->next = new_node;
			current = current->next;
		}
		i++;
	}
	return (SUCCESS);
}

static t_env	*env_new_copied_node(char *keyvalue, t_ecode *status)
{
	t_env	*new_node;

	*status = SUCCESS;
	new_node = env_new_node();
	if (!new_node)
	{
		*status = NEW_NODE_ERROR;
		return (NULL);
	}
	*status = env_copy_keyval(&new_node, keyvalue);
	if (*status != SUCCESS)
		return (NULL);
	return (new_node);
}
