#include "../../include/minishell.h"

/**
 * @brief Appends a node to the environment list.
 * 
 * @param head The head node of the environment list.
 * @param node The node to add to the list.
 * 
 * @return It returns SUCCESS if the node is added to the end of the list,
 * or if the head is NULL and it puts the node first on the list,
 * or if there's no node to add.
 */
t_ecode	add_last_env_node(t_env **head, t_env *node)
{
	t_env	*last;

	if (!node)
		return (SUCCESS);
	if (!*head)
	{
		*head = node;
		return (SUCCESS);
	}
	last = get_last_env_node(*head);
	last->next = node;
	return (SUCCESS);
}

/**
 * @brief Updates an environment's node value and keyvalue with the new value.
 * 
 * @param head The head node of the environment list.
 * @param key The key to find the target node, or to use when creating a new one.
 * @param value The new value that's going to update the value and keyvalue strings.
 * @param create_node If true it creates a new node if it fails to find it
 * in the environment list, and appends it to the end of the list.
 * 
 * @return SUCCESS if the value and keyvalue strings were updated successfully.
 * An ERROR code if memory allocation fails.
 */
t_ecode	update_env_node(t_env **head, char *key, char *value, bool create_node)
{
	t_env	*node;
	char	*keyval;

	if (!key)
		return (NULL_ERROR);
	node = find_env_node(*head, key);
	if (!node && create_node == false)
		return (NULL_NODE);
	else if (!node && create_node == true)
	{
		if (key && !value)
			node = create_populated_env_node(key);
		else
		{
			keyval = ft_strjoin(key, "=");
			if (!keyval)
				return (handle_perror("update_env_node"), MALLOC_ERROR);
			keyval = ft_strjoin_fs1(&keyval, value);
			if (!keyval)
				return (handle_perror("update_env_node"), MALLOC_ERROR);
			node = create_populated_env_node(keyval);
		}
		if (!node)
			return (handle_perror("update_env_node"), MALLOC_ERROR);
		return (add_last_env_node(head, node));
	}
	if (update_value_in_env_node(node, value))
		return (handle_perror("update_env_node"), MALLOC_ERROR);
	return (SUCCESS);
}

/**
 * @brief Updates the value string in an environment node with the new value.
 * 
 * @param node The target node whose value needs updating.
 * @param value The new value.
 * 
 * @return SUCCESS if the value is successfully replaced,
 * or if the new value is NULL and there was no value,
 * or if the new value is NULL and there was a value the latter is freed.
 * It returns an ERROR code otherwise.
 */
t_ecode	update_value_in_env_node(t_env *node, char *value)
{
	if (!node)
		return (NULL_NODE);
	if (!value && !node->value)
		return (SUCCESS);
	else if (!value && node->value)
	{
		ft_free((void **) &node->value);
		return (update_keyvalue_in_env_node(node));
	}
	if (node->value)
		ft_free((void **) &node->value);
	node->value = ft_strdup(value);
	if (!node->value)
		return (handle_perror("update_value_in_env_node"), MALLOC_ERROR);
	return (update_keyvalue_in_env_node(node));
}

/**
 * @brief Takes an environment node and updates the keyvalue string
 * based on the key and value strings.
 * 
 * @param node The node whose keyvalue string is to be modified.
 * 
 * @return It returns SUCCESS in the event that there is no value string,
 * or if it successfully created a new string out of the key and value strings.
 * It returns ERROR codes in case of failure.
 */
t_ecode	update_keyvalue_in_env_node(t_env *node)
{
	char	*temp;

	if (!node)
		return (NULL_NODE);
	if (!node->value && !node->keyvalue)
		return (SUCCESS);
	if (!node->value && node->keyvalue)
		return (ft_free((void **) &node->keyvalue), SUCCESS);
	temp = ft_strjoin(node->key, "=");
	if (!temp)
		return (handle_perror("update_keyvalue_in_env_node"), MALLOC_ERROR);
	temp = ft_strjoin_fs1(&temp, node->value);
	if (!temp)
		return (handle_perror("update_keyvalue_in_env_node"), MALLOC_ERROR);
	if (node->keyvalue)
		ft_free((void **) &node->keyvalue);
	node->keyvalue = ft_strdup(temp);
	ft_free((void **) &temp);
	if (!node->keyvalue)
		return (handle_perror("update_keyvalue_in_env_node"), MALLOC_ERROR);
	return (SUCCESS);
}
