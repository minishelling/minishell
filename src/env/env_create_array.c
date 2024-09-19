#include "../../include/minishell.h"

//Done
char	**create_env_array(t_env *env)
{
	char	**env_array;
	ssize_t	nodes_count;
	ssize_t	i;

	if (!env)
		return (NULL);
	nodes_count = count_keyvalue_env_nodes(env);
	if (nodes_count <= 0)
		return (NULL);
	env_array = ft_calloc (nodes_count + 1, sizeof(char *));
	if (!env_array)
		return (NULL);
	i = 0;
	while (env && i < nodes_count)
	{
		if (!env->keyvalue)
		{
			env = env->next;
			continue ;
		}
		env_array[i] = ft_strdup(env->keyvalue);
		if (!env_array[i])
			return (ft_free_2d((void ***) &env_array), NULL);
		i++;
		env = env->next;
	}
	env_array[nodes_count] = NULL;
	return (env_array);
}

//Done
char	**create_export_array(t_env *env)
{
	char	**env_array;
	ssize_t	nodes_count;
	ssize_t	i;

	if (!env)
		return (NULL);
	nodes_count = count_key_env_nodes(env);
	if (nodes_count <= 0)
		return (NULL);
	env_array = ft_calloc (nodes_count + 1, sizeof(char *));
	if (!env_array)
		return (NULL);
	i = 0;
	while (i < nodes_count)
	{
		if (env->keyvalue)
			env_array[i] = ft_strdup(env->keyvalue);
		else
			env_array[i] = ft_strdup(env->key);
		if (!env_array[i])
			return (ft_free_2d((void ***) &env_array), NULL);
		i++;
		env = env->next;
	}
	env_array[nodes_count] = NULL;
	return (env_array);
}
