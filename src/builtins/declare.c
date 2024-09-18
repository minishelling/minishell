#include "../../include/minishell.h"

static char	**create_full_env_array(t_env *env_head);
static char	**sort_env_list(t_env *env_list_head);
static void	bubblesort_array(char **arr, ssize_t count);

int	declare_builtin(t_shell *shell)
{
	char	**sorted_env;

	sorted_env = sort_env_list(shell->env_list);
	if (!sorted_env)
		return (FAILURE);
	ft_print_2d_arr(sorted_env);
	return (SUCCESS);
}

static char	**sort_env_list(t_env *env_list_head)
{
	char	**env_arr;
	ssize_t	count;

	env_arr = create_full_env_array(env_list_head);
	if (!env_arr)
		return (NULL);
	count = env_count_nodes(env_list_head);
	bubblesort_array(env_arr, count);
	return (env_arr);
}

static char	**create_full_env_array(t_env *env_head)
{
	char	**env_array;
	ssize_t	nodes_count;
	ssize_t	i;

	if (!env_head)
		return (NULL);
	nodes_count = env_count_nodes(env_head);
	env_array = ft_calloc (nodes_count + 1, sizeof(char *));
	if (!env_array)
		return (NULL);
	i = 0;
	while (i < nodes_count)
	{
		if (env_head->keyvalue)
			env_array[i] = ft_strdup(env_head->keyvalue);
		else
			env_array[i] = ft_strdup(env_head->key);
		if (!env_array[i])
			return (ft_free_2d((void ***) &env_array), NULL);
		i++;
		env_head = env_head->next;
	}
	env_array[nodes_count] = NULL;
	return (env_array);
}

static void	bubblesort_array(char **arr, ssize_t count)
{
	char	*temp;
	ssize_t	i;
	ssize_t	j;

	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (ft_strncmp(arr[j], arr[j + 1], __SIZE_MAX__) > 0)
			{
				temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}
