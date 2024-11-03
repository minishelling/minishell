/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env_count.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/31 13:32:43 by lprieri       #+#    #+#                 */
/*   Updated: 2024/10/31 18:02:25 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

ssize_t	count_keyvalue_env_nodes(t_env *env_list);
ssize_t	count_key_env_nodes(t_env *env_list);
ssize_t	count_envp_keys(char **envp);
ssize_t	count_values_from_env_node(t_env *env_list, char *key);

/**
 * @brief Counts the number of key-value strings in an environment list.
 * 
 * @param env_list The head node of the environment list.
 * 
 * @return The number of nodes that contain a key-value string.
 */
ssize_t	count_keyvalue_env_nodes(t_env *env_list)
{
	ssize_t	count;

	count = 0;
	while (env_list && env_list->key)
	{
		if (env_list->keyvalue)
			count++;
		env_list = env_list->next;
	}
	return (count);
}

/**
 * @brief Counts the number of keys in the environment list.
 * 
 * @param env_list The head node of the environment list.
 * 
 * @return The number of keys/nodes in the environment list.
 */
ssize_t	count_key_env_nodes(t_env *env_list)
{
	ssize_t	count;

	count = 0;
	while (env_list && env_list->key)
	{
		count++;
		env_list = env_list->next;
	}
	return (count);
}

/**
 * @brief Counts the number of keys in the environment array.
 * 
 * @param envp The environment array.
 * @return The number of keys in envp.
 * 
 * Returns -1 if envp is NULL or if the first string is NULL.
 */
ssize_t	count_envp_keys(char **envp)
{
	ssize_t	i;

	if (!envp || !*envp)
		return (-1);
	i = 0;
	while (envp[i])
		i++;
	return (i);
}

/**
 * @brief Counts the number of values
 * that are separated by a colon in an environment node.
 * 
 * @param env_list The head node of the environment list.
 * @param key The name of the target environment node.
 * 
 * @return The number of values in an environment node.
 * Returns -1 on failure.
 */
ssize_t	count_values_from_env_node(t_env *env_list, char *key)
{
	t_env	*node;
	ssize_t	i;
	char	**values;

	if (!env_list)
		return (-1);
	node = find_env_node(env_list, key);
	if (!node)
		return (-1);
	values = ft_split(node->value, ':');
	if (!values)
		return (-1);
	i = 0;
	while (values[i])
		i++;
	ft_free_2d((void ***) &values);
	return (i);
}
