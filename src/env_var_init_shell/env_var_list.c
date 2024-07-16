#include "../../include/minishell.h"

/*
 * function to create a new env node that has all values set to NULL
 */
t_env_list	*env_var_init_node(void)
{
	t_env_list	*env_node;

	env_node = malloc(sizeof(t_env_list));
	if (!env_node)
		return (NULL);
	env_node->name = NULL;
	env_node->value = NULL;
	env_node->has_value = false;
	env_node->next = NULL;
	return (env_node);
}

/*
** function that creates a new node containing environment variable in format of NAME=value
** also adds bool with 'has value' = true or false for every node.
*/
t_env_list	*env_var_create_new_node(char *env_var_str)
{
	t_env_list	*new_env_node;
	int				i;

	if (!env_var_str)
		return (NULL);
	new_env_node = env_var_init_node();
	// if (!new_env_node)
	// 	return (mini_error_test(error_print, 1, "(malloc) fail in initiating new_env_node"), NULL);
	i = 0;
	while (env_var_str[i] && env_var_str[i] != '=')
		i++;
	new_env_node->name = ft_substr(env_var_str, 0, i); // ! MALLOC
	if (new_env_node->name == NULL)
		return (env_var_free_node(new_env_node), NULL);
	if (env_var_str[i] == '\0')
		return (new_env_node);
	new_env_node->value = ft_substr(env_var_str, i + 1, ft_strlen(env_var_str + i + 1)); // ! MALLOC
	if (new_env_node->value == NULL)
		return (env_var_free_node(new_env_node), NULL);
	new_env_node->has_value = true;
	return (new_env_node);
}

/*
** function that adds a new node containing a new environment variable in format of NAME=value
** to the end of the linked list of environment variables.
*/
void	env_var_add_to_end_list(t_env_list **env_var_list, t_env_list *new_env_var)
{
	t_env_list	*current;

	if (env_var_list == NULL || new_env_var == NULL)
		return ;
	if (*env_var_list == NULL)
	{
		*env_var_list = new_env_var;
		return ;
	}
	current = *env_var_list;
	while (current->next != NULL)
		current = current->next;
	current->next = new_env_var;
}

size_t	env_var_size_has_value(t_env_list *env_list)
{
	size_t	ret_size;

	ret_size = 0;
	while (env_list != NULL)
	{
		if (env_list->has_value)
			ret_size++;
		env_list = env_list->next;
	}
	return (ret_size);
}

char	*env_var_make_cp(const t_env_list *env_node)
{
	char	*ret;

	if (!env_node || !env_node->name)
		return (NULL);
	ret = ft_calloc(sizeof(char), (ft_strlen(env_node->name) + \
				ft_strlen(env_node->value) + 2));
	if (!ret)
		return (NULL);
	ft_strlcat(ret, env_node->name, ft_strlen(env_node->name) + 1);
	ft_strlcat(ret, "=", ft_strlen(ret) + 2);
	ft_strlcat(ret, env_node->value, ft_strlen(ret) + ft_strlen(env_node->value) + 1);
	return (ret);
}

char	**env_var_to_cpp(t_env_list *env_list)
{
	t_env_list	*env_current;
	char			**env_ret;
	size_t			i;

	env_current = env_list;
	env_ret = ft_calloc(sizeof(char *), (env_var_size_has_value(env_list) + 1));
	if (!env_ret)
		return (NULL);
	i = 0;
	while (env_current != NULL)
	{
		if (env_current->has_value)
		{
			env_ret[i] = env_var_make_cp(env_current);
			i++;
		}
		env_current = env_current->next;
	}
	return (env_ret);
}

/*
** print linked list of environment variables FOR TESTING!
*/
void	env_var_print_linked_list(t_env_list *env_var_list)
{
	printf("LINKED LISTOF ENV VARS:\n\n");
	while (env_var_list != NULL)
	{
		printf("%s", env_var_list->name);
		printf("=");
		printf("%s\n", env_var_list->value);
		// printf("%s\n", env_var_list->has_value ? "true" : "false");
		env_var_list = env_var_list->next;
	}
	// printf("\n\n");
}

/*
** function to free content and node in env_var_list
*/
void	env_var_free_node(t_env_list *env_var_node)
{
	free(env_var_node->name);
	if (env_var_node->value)
		free(env_var_node->value);
	free(env_var_node);
}

/*
** function to free whole of env_var_list
*/
void	env_var_free_list(t_env_list *env_var_list)
{
	t_env_list	*tmp_env;

	if (env_var_list == NULL)
		return ;
	while (env_var_list)
	{
		tmp_env = env_var_list->next;
		env_var_free_node(env_var_list);
		env_var_list = tmp_env;
	}
}

t_env_list	*env_var_get_env_node(char *name, t_env_list *env_var_list)
{
	t_env_list	*current_node;
	int				len_name;

	if (!name || !env_var_list)
		return (NULL);
	len_name = ft_strlen(name) + 1;
	current_node = env_var_list;
	while (current_node != NULL)
	{
		if (ft_strncmp(current_node->name, name, len_name) == 0)
			return (current_node);
		current_node = current_node->next;
	}
	return (NULL);
}
