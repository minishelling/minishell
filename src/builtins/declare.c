#include "../../include/minishell.h"

static char	**sort_env_list(t_env *env_list_head);
static char	**create_declare_array(t_env *env_head);
static t_ecode	create_declare_string(t_env *curr_node, char **curr_str);
static void	bubblesort_array(char **arr, ssize_t count);

/**
 * @brief Prints the environment list with the 'declare' format.
 * The values are quoted, and if a variable doesn't have a value
 * it simply prints the key.
 * @param shell A pointer to the shell structure.
 * @param cmd_args The array of command arguments.
 * @return If there's a malloc failure it returns FAILURE and prints
 * an error message with the errno value. On success it returns SUCCESS.
 */
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

/**
 * @brief It creates a declare (env) array out of an environment list
 * and sorts it using the bubblesort algorithm.
 * @param env_list_head The head node of the environment list.
 * @return The sorted declare array. 
 */
static char	**sort_env_list(t_env *env_list_head)
{
	char	**env_arr;
	ssize_t	count;

	env_arr = create_declare_array(env_list_head);
	if (!env_arr)
		return (NULL);
	count = count_key_env_nodes(env_list_head);
	bubblesort_array(env_arr, count);
	return (env_arr);
}

/**
 * @brief Creates an an environment array with the declare format.
 * The format includes quotes around the variable's values,
 * and if a variable has no value it simply includes the key.
 * @param env_head The head node of the environment list.
 * @return The environment array, or NULL on failure.
 */
static char	**create_declare_array(t_env *env_head)
{
	char	**env_array;
	ssize_t	nodes_count;
	ssize_t	i;

	if (!env_head)
		return (NULL);
	nodes_count = count_key_env_nodes(env_head);
	env_array = ft_calloc (nodes_count + 1, sizeof(char *));
	if (!env_array)
		return (NULL);
	i = 0;
	while (i < nodes_count)
	{
		if (create_declare_string(env_head, &env_array[i]))
			return (ft_free_2d((void ***) &env_array), NULL);
		i++;
		env_head = env_head->next;
	}
	env_array[nodes_count] = NULL;
	return (env_array);
}

/**
 * @brief Given an environment node,
 * it creates the current string for the declare array.
 * If the node has a value, it adds quotes around it,
 * if it doesn't then it just uses the key for the string.
 * @param curr_node The current environment node.
 * @param curr_str The address of the position in the declare array
 * that's going to be used to create the string.
 * @return On success it returns SUCCESS, on failure it prints an error with errno,
 * and returns FAILURE, which indicates to the parent function
 * that it has to free the array.
 */
static t_ecode	create_declare_string(t_env *curr_node, char **curr_str)
{
	size_t	strlen;

	if (curr_node->value)
	{
		strlen = ft_strlen(curr_node->key) + ft_strlen("=\"") +
			ft_strlen(curr_node->value) + ft_strlen("\"");
		*curr_str = ft_calloc(strlen + 1, sizeof(char));
		if (!*curr_str)
			return (handle_perror("create_declare_string"), FAILURE);
		ft_strlcpy(*curr_str, curr_node->key, ft_strlen(curr_node->key) + 1);
		ft_strlcat(*curr_str, "=\"", strlen + 1);
		ft_strlcat(*curr_str, curr_node->value, strlen + 1);
		ft_strlcat(*curr_str, "\"", strlen + 1);
	}
	else
	{
		*curr_str = ft_strdup(curr_node->key);
		if (!*curr_str)
			return (handle_perror("create_declare_string"), FAILURE);
	}
	return (SUCCESS);
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
