#include "../../include/minishell.h"

//Done
t_env	*new_env_node(void)
{
	t_env	*new_node;
	
	new_node = ft_calloc(1, sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->key = NULL;
	new_node->value = NULL;
	new_node->keyvalue = NULL;
	return (new_node);
}

//Done
t_ecode	populate_env_node(t_env **node, char *keyval)
{
	t_ecode	status;
	if (!*node)
		return (NULL_NODE);
	if (!keyval)
	{
		ft_free((void **) node);
		return (NULL_STRING);
	}
	(*node)->key = get_key_from_keyvalue(keyval);
	if (!(*node)->key)
		return (free_env_node(node), MALLOC_ERROR);
	if (get_value_from_keyvalue(keyval, &(*node)->value) == MALLOC_ERROR)
		return (free_env_node(node), MALLOC_ERROR);
	if ((*node)->value)
	{
		(*node)->keyvalue = ft_strdup(keyval);
		if (!(*node)->keyvalue)
			return (free_env_node(node), MALLOC_ERROR);
	}
	return (SUCCESS);
}

//Done
t_env	*create_populated_env_node(char *keyval)
{
	t_env	*new_node;

	if (!keyval)
		return (NULL);
	new_node = new_env_node();
	if (!new_node)
		return (NULL);
	if (populate_env_node(&new_node, keyval) == MALLOC_ERROR) // Print error?
		return (NULL);
	return (new_node);
}

//Done
t_ecode	init_env_list(t_env **head, char **envp)
{
	t_env		*current;
	t_env		*new_node;
	size_t		i;

	if (!envp || !*envp)
		return (NULL_ERROR);
	i = 0;
	while (envp[i])
	{
		new_node = create_populated_env_node(envp[i]);
		if (!new_node)
			return (free_env_list(head), MALLOC_ERROR);
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
