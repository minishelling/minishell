#include "../../include/minishell.h"

static char	**create_full_env_array(t_env *env_head);
static char	**sort_env_list(t_env *env_list_head);
static void	bubblesort_array(char **arr, ssize_t count);
static char	**ft_strjoin_arr(char **arr, char *str);

t_ecode	declare_builtin(t_shell *shell, char **cmd_args)
{
	char	**sorted_env;
	char	**declare_array;

	(void) cmd_args;
	sorted_env = sort_env_list(shell->env_list);
	if (!sorted_env)
		return (FAILURE);
	declare_array = ft_strjoin_arr(sorted_env, "declare -x ");
	ft_free_2d((void ***) &sorted_env);
	if (!declare_array)
		return (FAILURE);
	ft_print_2d_arr(declare_array);
	ft_free_2d((void ***) &declare_array);
	return (SUCCESS);
}

static char	**sort_env_list(t_env *env_list_head)
{
	char	**env_arr;
	ssize_t	count;

	env_arr = create_full_env_array(env_list_head);
	if (!env_arr)
		return (NULL);
	count = count_key_env_nodes(env_list_head);
	bubblesort_array(env_arr, count);
	return (env_arr);
}

static char	**create_full_env_array(t_env *env_head)
{
	char	**env_array;
	ssize_t	nodes_count;
	ssize_t	i;
	char	*temp;

	if (!env_head)
		return (NULL);
	nodes_count = count_key_env_nodes(env_head);
	env_array = ft_calloc (nodes_count + 1, sizeof(char *));
	if (!env_array)
		return (NULL);
	i = 0;
	while (i < nodes_count)
	{
		if (env_head->value)
		{
			temp = ft_strdup(env_head->key);
			temp = ft_strjoin_fs1(&temp, "=\"");
			temp = ft_strjoin_fs1(&temp, env_head->value);
			temp = ft_strjoin_fs1(&temp, "\"");
			env_array[i] = ft_strdup(temp);
			ft_free ((void **) &temp);
		}
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

/**
 * @brief Sorts a given array using the bubblesort algorithm.
 * 
 * @param arr The array to sort.
 * @param count The number of elements in the array.
 * @return void: The elements are sorted within the given array.
 */
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

/**
 * @brief Concatenates str to every element of the array.
 * 
 * @param arr An array of strings.
 * @param str A string to concatenate to every element of the array.
 * @return A new array where each element has str concatenated to it,
 * or NULL on failure.
 */
static char	**ft_strjoin_arr(char **arr, char *str)
{
	char	**new;
	size_t	i;

	if (!arr || !str)
		return (NULL);
	i = 0;
	while (arr[i])
		i++;
	new = (char **) ft_calloc(i + 1, sizeof(char *));
	i = 0;
	while (arr[i])
	{
		new[i] = ft_strjoin(str, arr[i]);
		if (!new[i])
			return (ft_free_2d((void ***) &new), NULL);
		i++;
	}
	new[i] = NULL;
	return (new);
}
