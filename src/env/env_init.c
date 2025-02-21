/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env_init.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/31 13:32:47 by lprieri       #+#    #+#                 */
/*   Updated: 2024/10/31 18:03:55 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_env	*new_env_node(void);
t_ecode	populate_env_node(t_env **node, char *keyval);
t_env	*create_populated_env_node(char *keyval);
t_ecode	create_env_list(t_env **head, char **envp);

/**
 * @brief Creates a new environment node.
 * It allocates memory using ft_calloc,
 * and initializes each variable to NULL
 * 
 * @param void
 * 
 * @return A pointer to the newly created t_env node.
 */
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

/**
 * @brief Populates the variables of an env node with the data in keyval.
 * It retrieves the key and value from keyval
 * and assigns them to the corresponding variables.
 * It also duplicates the keyval string to the corresponding variable.
 * 
 * @param node A double pointer to the node to be populated.
 * @param keyval The string containing the key-value information.
 * 
 * @return If any of the allocations fail, it frees the node
 * as well as previous allocations, and returns FAILURE.
 * It returns SUCCESS otherwise.
 */
t_ecode	populate_env_node(t_env **node, char *keyval)
{
	char	*value;
	int		status;

	if (!*node)
		return (FAILURE);
	if (!keyval)
	{
		ft_free((void **) node);
		return (FAILURE);
	}
	(*node)->key = get_key_from_keyvalue(keyval);
	if (!(*node)->key)
		return (free_env_node(node), MALLOC_ERROR);
	value = NULL;
	if (get_value_from_keyvalue(keyval, &value) == MALLOC_ERROR)
		return (free_env_node(node), MALLOC_ERROR);
	status = SUCCESS;
	if (value)
	{
		status = update_value_in_env_node(*node, value);
		ft_free((void **) &value);
	}
	return (status);
}

/**
 * @brief Creates a new environment node
 * and populates its variables with the data in keyval.
 * 
 * @param keyval The string containing the key-value information.
 * 
 * @return The new populated node on success.
 * Returns NULL if keyval is NULL, or if the new node failed memory allocation.
 */
t_env	*create_populated_env_node(char *keyval)
{
	t_env	*new_node;

	if (!keyval)
		return (NULL);
	new_node = new_env_node();
	if (!new_node)
		return (NULL);
	if (populate_env_node(&new_node, keyval) == MALLOC_ERROR)
		return (handle_perror("create_populated_env_node"), NULL);
	return (new_node);
}

/**
 * @brief Creates the environment list from the environment array.
 * It creates all the nodes in the environment list
 * and initializes them with keys and if applicable values and keyvalues.
 * 
 * @param head A ptr to the address where the head node is going to be stored.
 * @param envp The environment array.
 * 
 * @return FAILURE if envp is NULL or if the first string is NULL,
 * or if a node failed to be allocated.
 * Returns SUCCESS otherwise.
 */
t_ecode	create_env_list(t_env **head, char **envp)
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
