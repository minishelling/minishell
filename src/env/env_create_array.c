/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env_create_array.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/31 13:32:45 by lprieri       #+#    #+#                 */
/*   Updated: 2024/10/31 18:03:11 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char			**create_env_array(t_env *env);
static t_ecode	copy_keyvalue_to_arr(t_env *env, \
					char ***env_array, ssize_t nodes_count);
char			**create_export_array(t_env *env);
static t_ecode	copy_key_or_keyval_to_arr(t_env *env, \
					char ***env_array, ssize_t nodes_count);

/**
 * @brief Creates an array of keyvalue strings from an env list.
 * 
 * @param env The head of the environment list.
 * 
 * @return The array of keyvalues strings.
 */
char	**create_env_array(t_env *env)
{
	char	**env_array;
	ssize_t	nodes_count;

	if (!env)
		return (NULL);
	nodes_count = count_keyvalue_env_nodes(env);
	if (nodes_count <= 0)
		return (NULL);
	env_array = (char **) ft_calloc(nodes_count + 1, sizeof(char *));
	if (!env_array)
		return (handle_perror("create_env_array"), NULL);
	if (copy_keyvalue_to_arr(env, &env_array, nodes_count))
		return (NULL);
	return (env_array);
}

static t_ecode	copy_keyvalue_to_arr(t_env *env, \
					char ***env_array, ssize_t nodes_count)
{
	ssize_t	i;

	i = 0;
	while (env && i < nodes_count)
	{
		if (!env->keyvalue)
		{
			env = env->next;
			continue ;
		}
		(*env_array)[i] = ft_strdup(env->keyvalue);
		if (!(*env_array)[i])
			return (ft_free_2d((void ***) env_array), \
			handle_perror("create_env_array"), MALLOC_ERROR);
		i++;
		env = env->next;
	}
	(*env_array)[nodes_count] = NULL;
	return (SUCCESS);
}

/**
 * @brief Creates an array of keyvalue or key strings from an env list.
 * 
 * @param env The head of the environment list.
 * 
 * @return The array of keyvalues or keys strings.
 */
char	**create_export_array(t_env *env)
{
	char	**env_array;
	ssize_t	nodes_count;

	if (!env)
		return (NULL);
	nodes_count = count_key_env_nodes(env);
	if (nodes_count <= 0)
		return (NULL);
	env_array = (char **) ft_calloc(nodes_count + 1, sizeof(char *));
	if (!env_array)
		return (handle_perror("create_export_array"), NULL);
	if (copy_key_or_keyval_to_arr(env, &env_array, nodes_count))
		return (NULL);
	return (env_array);
}

static t_ecode	copy_key_or_keyval_to_arr(t_env *env, \
					char ***env_array, ssize_t nodes_count)
{
	ssize_t	i;

	i = 0;
	while (i < nodes_count)
	{
		if (env->keyvalue)
			(*env_array)[i] = ft_strdup(env->keyvalue);
		else
			(*env_array)[i] = ft_strdup(env->key);
		if (!(*env_array)[i])
			return (ft_free_2d((void ***) env_array), \
			handle_perror("create_export_array"), MALLOC_ERROR);
		i++;
		env = env->next;
	}
	env_array[nodes_count] = NULL;
	return (SUCCESS);
}
