#include "../../include/minishell.h"

/**
 * @brief Gets the key from a keyvalue string.
 * It extracts a substring of anything preceding a '=' character,
 * or the full string if there's no '=' char.
 * 
 * @param keyvalue The keyvalue string to extract the key from.
 * 
 * @return The key extracted from the key-value string.
 * Returns NULL if there was an error allocating memory,
 * or if the keyvalue string was NULL or empty.
 */
char	*get_key_from_keyvalue(char *keyvalue)
{
	char	*key;
	int		i;
	int		j;

	if (!keyvalue || !keyvalue[0])
		return (NULL);
	i = 0;
	while (keyvalue[i] && keyvalue[i] != '=')
		i++;
	key = ft_calloc(i + 1, sizeof(char));
	if (!key)
	{
		handle_perror("get_key_from_keyvalue");
		return (NULL);
	}
	j = 0;
	while (j < i)
	{
		key[j] = keyvalue[j];
		j++;
	}
	key[j] = '\0';
	return (key);
}

/**
 * @brief Extracts the value portion of a key-value string.
 * 
 * This function locates the '=' character in the key-value string and extracts 
 * the substring following it. If there is no value after the '=', an empty 
 * string is assigned to `value_ptr`.
 * 
 * @param[in] keyvalue The key-value string from which to extract the value.
 * @param[out] value_ptr A pointer to the string where the extracted value 
 *                       will be stored.
 * 
 * @return An error code if the key-value string is invalid or memory allocation
 *         fails. Returns SUCCESS on successful extraction and assignment.
 */
t_ecode	get_value_from_keyvalue(char *keyvalue, char **value_ptr)
{
	size_t	value_len;
	size_t	i;

	if (!keyvalue || !keyvalue[0])
		return (NULL_ERROR);
	while (*keyvalue && *keyvalue != '=')
		keyvalue++;
	if (*keyvalue != '=')
		return (NULL_STRING);
	keyvalue++;
	value_len = ft_strlen(keyvalue);
	*value_ptr = (char *) ft_calloc(value_len + 1, sizeof(char));
	if (!*value_ptr)
	{
		handle_perror("get_value_from_keyvalue");
		return (MALLOC_ERROR);
	}
	i = 0;
	while (i < value_len)
	{
		(*value_ptr)[i] = keyvalue[i];
		i++;
	}
	(*value_ptr)[i] = '\0';
	return (SUCCESS);
}

/**
 * @brief Finds an environment node by matching the specified key.
 * 
 * This function traverses the environment list to find a node with a key 
 * matching the provided key parameter.
 * 
 * @param[in] env The head node of the environment list.
 * @param[in] key The key to match with the node's key.
 * 
 * @return The matching environment node if found. Returns NULL if a node 
 *         with the given key does not exist or if either the head node or 
 *         the key is NULL.
 */
t_env	*find_env_node(t_env *env, char *key)
{
	t_env	*node;
	int		isnt_key;

	if (!env || !key)
		return (NULL);
	node = env;
	isnt_key = 0;
	while (node)
	{
		if (node && node->key)
			isnt_key = ft_strncmp(node->key, key, max_len(node->key, key));
		if (isnt_key)
		{
			node = node->next;
			continue ;
		}
		return (node);
	}
	return (NULL);
}

/**
 * @brief Retrieves the last node of the environment list.
 * 
 * This function traverses the environment list starting from the head node 
 * and returns the last node in the list.
 * 
 * @param[in] head The head node of the environment list.
 * 
 * @return The last node of the environment list, or NULL if the head node 
 *         is NULL.
 */
t_env	*get_last_env_node(t_env *head)
{
	t_env	*last;

	if (!head)
		return (NULL);
	last = head;
	while (last && last->next)
		last = last->next;
	return (last);
}

/**
 * @brief Retrieves the value associated with a given key from the environment 
 *        list.
 * 
 * This function iterates over an environment list to locate a node matching 
 * the specified key. If found, it returns the value of that node; otherwise, 
 * it returns NULL.
 * 
 * @param[in] env_list  The head node of the environment list.
 * @param[in] token_key  The key used to find the corresponding environment 
 *                       variable.
 * 
 * @return The value of the environment variable if found, or NULL if the 
 *         environment list is NULL, the key is NULL or empty, or if no 
 *         matching node is found.
 */
char	*get_env_value_from_key(t_env *env_list, char *token_key)
{
	t_env	*cur;
	size_t	token_key_len;

	if (!env_list || !token_key || !token_key[0])
		return (NULL);
	token_key_len = ft_strlen(token_key);
	cur = env_list;
	while (cur)
	{
		if (token_key_len == ft_strlen(cur->key)
			&& ft_strncmp(token_key, cur->key, token_key_len) == 0)
			return (cur->value);
		cur = cur->next;
	}
	return (NULL);
}
