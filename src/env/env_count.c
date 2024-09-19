#include "../../include/minishell.h"

//Done
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

//Done
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

//Done
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

//Done
ssize_t	count_values_from_env_node(t_env *env, char *key)
{
	t_env	*node;
	ssize_t	i;
	char	**values;

	node = find_env_node(env, key);
	if (!node)
		return (ENV_ERROR);
	values = ft_split(node->value, ':');
	if (!values)
		return (-1);
	i = 0;
	while (values[i])
		i++;
	ft_free_2d((void ***) &values);
	return (i);
}
