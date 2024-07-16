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
bool	env_var_exists(char *name, t_env_list *env_var_list)
{
	int	len_name;

	if (!name)
		return (false);
	len_name = ft_strlen(name) + 1;
	while (env_var_list != NULL)
	{
		if (ft_strncmp(env_var_list->name, name, len_name) == 0)
			return (true);
		env_var_list = env_var_list->next;
	}
	return (false);
}

/*
** our own getenv function:
** the NAME of the environment variable is search in env_var_list
** if env_var exists, it's VALUE is returned
*/
char	*env_var_get_env(char *name, t_env_list *env_var_list)
{
	int		len_name;

	if (!name || !env_var_list)
		return (NULL);
	// if (ft_strncmp(name, "?", 2) == 0)
	// 	return (g_status.exit_str);
	len_name = ft_strlen(name) + 1;
	env_var_print_linked_list(env_var_list); //!

	while (env_var_list)
	{
		if (ft_strncmp(env_var_list->name, name, len_name) == 0)
			return (env_var_list->value);
		env_var_list = env_var_list->next;
	}
	return (NULL);
}

/*
** our own setenv() function: it sets a (new) environment variable
*/
void	env_var_set_env(char *envar, t_env_list **env_var_list)
{
	int				len_name;
	t_env_list	*new_var;
	t_env_list	*current;
	t_env_list	*prev;

	if (!envar)
		return ;
	current = *env_var_list;
	prev = NULL;
	new_var = env_var_create_new_node(envar);
	if (!new_var || !new_var->name)
		return ;
	len_name = ft_strlen(new_var->name) + 1;
	while (current != NULL)
	{
		if (ft_strncmp(current->name, new_var->name, len_name) == 0)
		{
			new_var->next = current->next;
			if (prev == NULL)
				*env_var_list = new_var;
			else
				prev->next = new_var;
			env_var_free_node(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
	env_var_add_to_end_list(env_var_list, new_var);
}
