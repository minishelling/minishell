#include "../../include/minishell.h"

/*
** This file contains functions to manage and change environment variables:
**
** - bool function to check if environment variable already exists
** - getenv function: if env_var with NAME exists, it's VALUE is returned
** - our own setenv function:
** it sets a (new) environment variable (used in export builtin)
**
*/

/*
** bool function to check if environment variable already exists
** this version is NOT making use of getenv()
*/
bool	env_var_exists(char *name, t_env_list *env_list)
{
	int	len_name;

	if (!name)
		return (false);
	len_name = ft_strlen(name) + 1;
	while (env_list != NULL)
	{
		if (ft_strncmp(env_list->name, name, len_name) == 0)
			return (true);
		env_list = env_list->next;
	}
	return (false);
}

/*
** our own getenv function:
** the NAME of the environment variable is search in env_list
** if env_var exists, it's VALUE is returned
*/
char	*env_var_get_env(char *key, t_env *env_list)
{
	int		len_key;

	if (!key || !env_list)
		return (NULL);
	// if (ft_strncmp(name, "?", 2) == 0)
	// 	return (g_status.exit_str);
	len_key = ft_strlen(key) + 1;
	//env_var_print_linked_list(env_list); //!

	while (env_list)
	{
		if (ft_strncmp(env_list->key, key, len_key) == 0)
			return (env_list->value);
		env_list = env_list->next;
	}
	return (NULL);
}

/*
** our own setenv() function: it sets a (new) environment variable
*/
void	env_var_set_env(char *envar, t_env **env_list)
{
	int		len_key;
	t_env	*new_var;
	t_env	*current;
	t_env	*prev;

	if (!envar)
		return ;
	current = *env_list;
	prev = NULL;
	new_var = env_var_create_new_node(envar);
	if (!new_var || !new_var->key)
		return ;
	len_key = ft_strlen(new_var->key) + 1;
	while (current != NULL)
	{
		if (ft_strncmp(current->key, new_var->key, len_key) == 0)
		{
			new_var->next = current->next;
			if (prev == NULL)
				*env_list = new_var;
			else
				prev->next = new_var;
			env_var_free_node(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
	env_var_add_to_end_list(env_list, new_var);
}
