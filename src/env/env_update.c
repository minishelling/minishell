/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env_update.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/31 13:32:50 by lprieri       #+#    #+#                 */
/*   Updated: 2024/10/31 18:06:06 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_ecode			add_last_env_node(t_env **head, t_env *node);
t_ecode			update_env_node(t_env **head, char *key, char *value, \
				bool create_node);
static t_ecode	create_and_add_node(t_env **head, char *key, char *value);
t_ecode			update_value_in_env_node(t_env *node, char *value);
t_ecode			update_keyvalue_in_env_node(t_env *node);

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
 * @param value The value that's going to update the value and keyvalue strings.
 * @param create_node If true it creates a new node if it fails to find it
 * in the environment list, and appends it to the end of the list.
 * 
 * @return SUCCESS if the value and keyvalue strings were updated successfully.
 * An ERROR code if memory allocation fails.
 */
t_ecode	update_env_node(t_env **head, char *key, char *value, bool create_node)
{
	t_env	*node;

	if (!key)
		return (NULL_ERROR);
	node = find_env_node(*head, key);
	if (!node && create_node == false)
		return (NULL_NODE);
	else if (!node && create_node == true)
		return (create_and_add_node(head, key, value));
	if (value)
	{
		if (update_value_in_env_node(node, value))
			return (handle_perror("update_env_node"), MALLOC_ERROR);
	}
	return (SUCCESS);
}

/**
 * @brief Creates a new node, populates it and adds it to the env list.
 * 
 * If there's only a key and no value, it populates the node only with a key.
 * If there's a value, even if it is an empty string, it will create a string
 * containing 'key=value' combination, and it will populate the node from there.
 * 
 * @param head A double pointer to the head node of the env list.
 * @param key The key string.
 * @param value The value string.
 * 
 * @return If there's a malloc failure it prints an error message and returns
 * MALLOC_ERROR. Else it will return the status of 'add_last_env_node'.
 */
static t_ecode	create_and_add_node(t_env **head, char *key, char *value)
{
	t_env	*node;
	char	*keyval;

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
		ft_free((void **)&keyval);
	}
	if (!node)
		return (handle_perror("update_env_node"), MALLOC_ERROR);
	return (add_last_env_node(head, node));
}

/**
 * @brief Updates the value string in an environment node with the new value.
 * 
 * If the new value starts with '~', it is expanded to the home directory 
 * before updating.
 * 
 * @param node The target node whose value needs updating.
 * @param value The new value to set.
 * 
 * @return SUCCESS if the value is successfully replaced,
 * or if the new value is NULL and there was no value.
 * It returns an ERROR code otherwise.
 */
t_ecode	update_value_in_env_node(t_env *node, char *value)
{
	if (!node)
		return (NULL_NODE);
	if (!value)
		return (SUCCESS);
	if (node->value)
		ft_free((void **) &node->value);
	if (value && value[0] == '~')
	{
		if (update_home_value(&value))
			return (handle_perror("update_value_in_env_node"), MALLOC_ERROR);
		return (update_keyvalue_in_env_node(node));
	}
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
