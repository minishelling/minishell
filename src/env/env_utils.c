#include "../../include/minishell.h"

//Done
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
		return (NULL);
	j = 0;
	while (j < i)
	{
		key[j] = keyvalue[j];
		j++;
	}
	key[j] = '\0';
	return (key);
}

//Done
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
	value_len = ft_strlen(keyvalue);
	*value_ptr = (char *) ft_calloc(value_len + 1, sizeof(char));
	if (!*value_ptr)
		return (MALLOC_ERROR);
	i = 0;
	while (i < value_len)
	{
		(*value_ptr)[i] = keyvalue[i];
		i++;
	}
	(*value_ptr)[i] = '\0';
	return (SUCCESS);
}

//Done
t_env	*find_env_node(t_env *env, char *key)
{
	t_env	*node;
	int		isnt_key;

	node = env;
	while (node)
	{
		isnt_key = ft_strncmp(node->key, key, __SIZE_MAX__);
		if (isnt_key)
		{
			node = node->next;
			continue ;
		}
		return (node);
	}
	return (NULL);
}

//Done
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
