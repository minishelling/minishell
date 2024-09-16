#include "../../include/minishell.h"

ssize_t	env_count_keys(char **envp)
{
	int	i;

	if (!envp || !*envp)
		return (-1);
	i = 0;
	while (envp[i])
		i++;
	return (i);
}

t_env	*env_new_node(void)
{
	t_env	*new_node;
	
	new_node = ft_calloc(1, sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->keyvalue = NULL;
	new_node->key = NULL;
	new_node->value = NULL;
	return (new_node);
}

t_env	*env_new_populated_node(char *key, char *value)
{
	t_env	*new_node;
	
	new_node = env_new_node();
	if (!new_node)
		return (NULL);
	if (env_update_key(new_node, key))
	{
		ft_free((void **) &new_node);
		return (NULL);
	}
	if (env_update_value(new_node, value))
	{
		ft_free((void **) &new_node->key);
		ft_free((void **) &new_node->keyvalue);
		ft_free((void **) &new_node);
		return (NULL);
	}
	return (new_node);
}

char	*env_get_key(char *keyvalue)
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

char	*env_get_value(char *keyvalue)
{
	char	*value;
	int		i;
	int		value_len;
	

	if (!keyvalue || !keyvalue[0])
		return (NULL);
	i = 0;
	while (*keyvalue && *keyvalue != '=')
		keyvalue++;
	keyvalue++;
	value_len = ft_strlen(keyvalue);
	value = ft_calloc(value_len + 1, sizeof(char));
	if (!value)
		return (NULL);
	i = 0;
	while (i < value_len)
	{
		value[i] = keyvalue[i];
		i++;
	}
	value[i] = '\0';
	return (value);
}

t_ecode	env_copy_keyval(t_env **new_node, char *keyvalue)
{
	(*new_node)->keyvalue = ft_strdup(keyvalue);
	if (!(*new_node)->keyvalue)
		return (MALLOC_ERROR);
	(*new_node)->key = env_get_key((*new_node)->keyvalue);
	if (!(*new_node)->key)
		return (MALLOC_ERROR);
	(*new_node)->value = env_get_value((*new_node)->keyvalue);
	if (!(*new_node)->value)
		return (MALLOC_ERROR);
	(*new_node)->next = NULL;
	return (SUCCESS);
}

t_env	*env_find_node(t_env *env, char *key)
{
	t_env	*node;
	size_t	key_len;
	int		isnt_key;

	node = env;
	key_len = ft_strlen(key);
	while (node)
	{
		isnt_key = ft_strncmp(node->key, key, key_len + 1);
		if (isnt_key)
		{
			node = node->next;
			continue ;
		}
		return (node);
	}
	return (NULL);
}
t_ecode	update_pwd(t_env *pwd_node)
{
	if (!pwd_node)
	{
		pwd_node = env_new_populated_node("PWD", getcwd(NULL, PATH_MAX));
		if (!pwd_node)
			return (MALLOC_ERROR);
	}
	else
	{
		if (env_update_value(pwd_node, getcwd(NULL, PATH_MAX)))
			return (MALLOC_ERROR);
	}
	return (SUCCESS);
}

// t_ecode	update_oldpwd(t_env	*oldpwd_node, char *cwd)
// {
// 	if (!oldpwd_node)
// 	{
// 		oldpwd_node = env_new_populated_node("PWD", getcwd(NULL, PATH_MAX));
// 		if (!pwd_node)
// 			return (MALLOC_ERROR);
// 	}
// 	else
// 	{
// 		if (env_update_value(pwd_node, getcwd(NULL, PATH_MAX)))
// 			return (MALLOC_ERROR);
// 	}
// 	return (SUCCESS);
// }

size_t	env_count_values(t_env *env, char *key)
{
	t_env	*node;
	size_t	i;
	char	**values;

	node = env_find_node(env, key);
	if (!node)
		return (ENV_ERROR);
	values = ft_split(node->value, ':');
	if (!values)
		return (-1); //The CD_MALLOC enum should be negative
	i = 0;
	while (values[i])
		i++;
	return (i);
}

t_ecode	env_update_keyvalue(t_env *node)
{
	char	*temp;

	if (!node)
		return (NULL_ERROR);
	temp = ft_strdup(node->key);
	if (!temp)
		return (MALLOC_ERROR);
	temp = ft_strjoin_fs1(&temp, "=");
	if (!temp)
		return (MALLOC_ERROR);
	temp = ft_strjoin_fs1(&temp, node->value);
	if (!temp)
		return (MALLOC_ERROR);
	if (node->keyvalue)
		ft_free((void **) &node->keyvalue);
	node->keyvalue = ft_strdup(temp);
	ft_free((void **) &temp);
	if (!node->keyvalue)
		return (MALLOC_ERROR);
	return (SUCCESS);
}

t_ecode env_update_key(t_env *node, char *key)
{
	if (!node)
		return (NULL_ERROR);
	if (node->key)
		ft_free((void **) node->key);
	if (!key)
		node->key = NULL;
	else
	{
		node->key = ft_strdup(key);
		if (!node->key)
			return (MALLOC_ERROR);
	}
	if (env_update_keyvalue(node))
		return (MALLOC_ERROR);
	return (SUCCESS);
}

t_ecode	env_update_value(t_env *node, char *value)
{
	if (!node)
		return (NULL_ERROR);
	if (node && node->value)
		ft_free((void **) &node->value);
	if (!value)
		node->value = NULL;
	else
	{
		node->value = ft_strdup(value);
		if (!node->value)
			return (MALLOC_ERROR);
	}
	if (env_update_keyvalue(node))
		return (MALLOC_ERROR);
	return (SUCCESS);
}

//create_node creates the node if it doesn't exist.
t_ecode	env_update_node(t_env *head, char *key, char *value, bool create_node)
{
	t_env	*node;

	node = env_find_node(head, key);
	if (!node && create_node == false)
		return (NULL_ERROR);
	else if (!node && create_node == true) // I would also have to add it to the list.
	{
		node = env_new_populated_node(key, value);
		if (!node)
			return (MALLOC_ERROR);
		//ADD NODE TO THE LIST
	}
	else
	{
		if (env_update_value(node, value))
			return (MALLOC_ERROR);
	}
	return (SUCCESS);
}

t_ecode	env_free_node(t_env **node)
{
	if (!*node)
		return (SUCCESS);
	if ((*node)->keyvalue)
	{
		ft_free((void **) &(*node)->keyvalue);
		ft_free((void **) &(*node)->key);
		if ((*node)->value)
			ft_free((void **) &(*node)->value);
	}
	ft_free((void **) node);
	return (SUCCESS);
}
