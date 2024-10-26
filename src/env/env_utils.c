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
 * @brief Gets the value from a keyvalue string.
 * It extracts a substring of anything following a '=' character.
 * If there's nothing following a '=' char, then it returns an empty string.
 * 
 * @param keyvalue The keyvalue string to extract the value from.
 * @param value_ptr The a pointer to the string where to store the value information.
 * 
 * @return An ERROR code if there is no value or if it fails to assign memory for it.
 * Returns SUCCESS otherwise.
 */
t_ecode	get_value_from_keyvalue(char *keyvalue, char **value_ptr)
{
	size_t	value_len;
	size_t	i;

	if (!keyvalue || !keyvalue[0])
		return (NULL_ERROR);
	while (*keyvalue && *keyvalue!= '=')
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
 * @brief Finds an environment node by matching the key.
 * 
 * @param env The environment list head.
 * @param key The key that has to match the node's key.
 * 
 * @return The environment node if it is found.
 * Returns NULL if it wasn't found or if either the head
 * or the key were NULL in the first place.
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
 * @brief Gets the last node of the environment list.
 * 
 * @param head The head node of the environment list.
 * 
 * @return The last node of the env list,
 * or NULL if the head node was NULL.
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
 * @brief Gets the value of an environment node by passing the key
 * and the head node of the environment list.
 * 
 * @param env_head The head node of the environment list.
 * @param token_key The key that's used to find the correct node.
 * 
 * @return The value of the node if it was found. It returns
 * NULL if there's no environment list, the token key is NULL,
 * or if the node was not found.
 */
char *get_env_value_from_key(t_env *env_head, char *token_key) 
{
    t_env *cur;
    size_t token_key_len;

    if (!env_head || !token_key || !token_key[0])
        return (NULL);
    token_key_len = ft_strlen(token_key);
    cur = env_head;
    while (cur) {
        if (token_key_len == ft_strlen(cur->key) &&
            ft_strncmp(token_key, cur->key, token_key_len) == 0) 
            	return (cur->value);
        cur = cur->next;
    }
    return (NULL);
}
