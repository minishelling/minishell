#include "../../include/minishell.h"

/*
 * function to create a new env node that has all values set to NULL
 */
t_env	*env_var_init_node(void)
{
	t_env	*env_node;

	env_node = malloc(sizeof(t_env));
	if (!env_node)
		return (NULL);
	env_node->key = NULL;
	env_node->value = NULL;
	// env_node->has_value = false; //!!!!!!!
	env_node->next = NULL;
	return (env_node);
}

/*
** function that creates a new node containing environment variable in format of NAME=value
** also adds bool with 'has value' = true or false for every node.
*/
t_env	*env_var_create_new_node(char *env_var_str)
{
	t_env	*new_env_node;
	int				i;

	if (!env_var_str)
		return (NULL);
	new_env_node = env_var_init_node();
	// if (!new_env_node)
	// 	return (mini_error_test(error_print, 1, "(malloc) fail in initiating new_env_node"), NULL);
	i = 0;
	while (env_var_str[i] && env_var_str[i] != '=')
		i++;
	new_env_node->key = ft_substr(env_var_str, 0, i); // ! MALLOC
	if (new_env_node->key == NULL)
		return (env_var_free_node(new_env_node), NULL);
	if (env_var_str[i] == '\0')
		return (new_env_node);
	new_env_node->value = ft_substr(env_var_str, i + 1, ft_strlen(env_var_str + i + 1)); // ! MALLOC
	if (new_env_node->value == NULL)
		return (env_var_free_node(new_env_node), NULL);
	// new_env_node->has_value = true; //  !!!
	return (new_env_node);
}

/*
** function that adds a new node containing a new environment variable in format of NAME=value
** to the end of the linked list of environment variables.
*/
void	env_var_add_to_end_list(t_env **env_list, t_env *new_env_var)
{
	t_env	*current;

	if (env_list == NULL || new_env_var == NULL)
		return ;
	if (*env_list == NULL)
	{
		*env_list = new_env_var;
		return ;
	}
	current = *env_list;
	while (current->next != NULL)
		current = current->next;
	current->next = new_env_var;
}

size_t	env_var_size_has_value(t_env *env_list)
{
	size_t	ret_size;

	ret_size = 0;
	while (env_list != NULL)
	{
		// if (env_list->has_value)
		// 	ret_size++;							//!!!!
		env_list = env_list->next;
	}
	return (ret_size);
}

char	*env_var_make_cp(const t_env *env_node)
{
	char	*ret;

	if (!env_node || !env_node->key)
		return (NULL);
	ret = ft_calloc(sizeof(char), (ft_strlen(env_node->key) + \
				ft_strlen(env_node->value) + 2));
	if (!ret)
		return (NULL);
	ft_strlcat(ret, env_node->key, ft_strlen(env_node->key) + 1);
	ft_strlcat(ret, "=", ft_strlen(ret) + 2);
	ft_strlcat(ret, env_node->value, ft_strlen(ret) + ft_strlen(env_node->value) + 1);
	return (ret);
}

char	**env_var_to_cpp(t_env *env_list)
{
	t_env	*env_current;
	char			**env_ret;
	size_t			i;

	env_current = env_list;
	env_ret = ft_calloc(sizeof(char *), (env_var_size_has_value(env_list) + 1));
	if (!env_ret)
		return (NULL);
	i = 0;
	while (env_current != NULL)
	{
		// if (env_current->has_value)
		// {
		// 	env_ret[i] = env_var_make_cp(env_current);  //// !!!!!
		// 	i++;
		// }
		env_current = env_current->next;
	}
	return (env_ret);
}

/*
** print linked list of environment variables FOR TESTING!
*/
void	env_var_print_linked_list(t_env *env_list)
{
	printf("LINKED LISTOF ENV VARS:\n\n");
	while (env_list != NULL)
	{
		printf("%s", env_list->key);
		printf("=");
		printf("%s\n", env_list->value);
		// printf("%s\n", env_list->has_value ? "true" : "false");
		env_list = env_list->next;
	}
	// printf("\n\n");
}

/*
** function to free content and node in env_list
*/
void	env_var_free_node(t_env *env_var_node)
{
	free(env_var_node->key);
	if (env_var_node->value)
		free(env_var_node->value);
	free(env_var_node);
}

/*
** function to free whole of env_list
*/
void	env_var_free_list(t_env *env_list)
{
	t_env	*tmp_env;

	if (env_list == NULL)
		return ;
	while (env_list)
	{
		tmp_env = env_list->next;
		env_var_free_node(env_list);
		env_list = tmp_env;
	}
}

t_env	*env_var_get_env_node(char *key, t_env *env_list)
{
	t_env	*current_node;
	int				len_key;

	if (!key || !env_list)
		return (NULL);
	len_key = ft_strlen(key) + 1;
	current_node = env_list;
	while (current_node != NULL)
	{
		if (ft_strncmp(current_node->key, key, len_key) == 0)
			return (current_node);
		current_node = current_node->next;
	}
	return (NULL);
}
