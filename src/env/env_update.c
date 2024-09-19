#include "../../include/minishell.h"

//Done
t_ecode	add_last_env_node(t_env **head, t_env *env)
{
	t_env	*last;

	if (!*head || !env)
		return (NULL_NODE);
	last = get_last_env_node(*head);
	last->next = env;
	return (SUCCESS);
}

//Done
t_ecode	update_env_node(t_env **head, char *key, char *value, bool create_node)
{
	t_env	*node;
	char	*keyval;

	if (!*head || !key)
		return (NULL_ERROR);
	node = find_env_node(*head, key);
	if (!node && create_node == false)
		return (NULL_NODE);
	else if (!node && create_node == true) // I would also have to add it to the list.
	{
		if (key && !value)
			node = create_populated_env_node(key);
		else
		{
			keyval = ft_strjoin(key, "=");
			if (!keyval)
				return (MALLOC_ERROR);
			keyval = ft_strjoin_fs1(&keyval, value);
			if (!keyval)
				return (MALLOC_ERROR);
			node = create_populated_env_node(keyval);
		}
		if (!node)
			return (MALLOC_ERROR);
		return (add_last_env_node(head, node));
	}
	if (update_value_in_env_node(node, value))
		return (MALLOC_ERROR);
	return (SUCCESS);
}

//Done
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
		return (MALLOC_ERROR);
	return (update_keyvalue_in_env_node(node));
}

//Done
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

//Done
t_ecode	update_pwd(t_env *env_head)
{
	t_env	*pwd_node;
	char	*keyval;

	if (!env_head)
		return (NULL_ERROR);
	pwd_node = find_env_node(env_head, "PWD");
	if (!pwd_node)
	{
		keyval = ft_strjoin("PWD", "=");
		if (!keyval)
			return (MALLOC_ERROR);
		keyval = ft_strjoin_fs1(&keyval, getcwd(NULL, PATH_MAX));
		if (!keyval)
			return (MALLOC_ERROR);
		pwd_node = create_populated_env_node(keyval);
		if (!pwd_node)
			return (ft_free((void **) &keyval), MALLOC_ERROR);
		return (ft_free((void **) &keyval), SUCCESS);
	}
	return (update_value_in_env_node(pwd_node, getcwd(NULL, PATH_MAX)));
}
