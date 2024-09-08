#include "../../include/minishell.h"

static size_t	count_env_nodes(t_env *env_list)
{
	size_t	count;

	if (!env_list || !env_list->key)
		return (0);
	count = 0;
	while (env_list && env_list->next)
	{
		count++;
		env_list = env_list->next;
	}
	return (count + 1); // The +1 counts the last node (not the null node).
}

char	**env_create_array(t_env *env)
{
	char	**env_array;
	size_t	nodes_count;
	size_t	i;

	if (!env)
		return (NULL);
	nodes_count = count_env_nodes(env);
	env_array = ft_calloc (nodes_count + 1, sizeof(char *));
	if (!env_array)
		return (NULL);
	i = 0;
	while (i < nodes_count)
	{
		env_array[i] = ft_strdup(env->keyvalue);
		if (!env_array[i]) // A node can't be null. It has to have at least a key. Otherwise we didn't do a good job removing the node when unsetting.
		{
			ft_free_2d((void ***) &env_array);
			return (NULL);
		}
		i++;
		env = env->next;
	}
	env_array[nodes_count] = NULL;
	return (env_array);
}
