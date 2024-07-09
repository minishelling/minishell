/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env_utils.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/04 00:11:05 by lprieri       #+#    #+#                 */
/*   Updated: 2024/07/04 00:11:05 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

ssize_t	env_count_keys(char **envp)
{
	int	i;

	if (!envp || !*envp)
		return (-1);
	i = 0;
	while (envp[i])
		i++;
	return (i);
}

t_env	*env_new_node(void)
{
	t_env	*new_node;
	
	new_node = ft_calloc(1, sizeof(t_env));
	if (!new_node)
		return (NULL);
	return (new_node);
}

char	*env_get_key(char *keyvalue)
{
	char	*key;
	int		i;
	int		j;

	if (!keyvalue || !keyvalue[0])
		return (NULL);
	i = 0;
	while (keyvalue[i] && keyvalue[i] != '=')
		i++;
	key = ft_calloc(i + 1, sizeof(char));
	if (!key)
		return (NULL);
	j = 0;
	while (j < i)
	{
		key[j] = keyvalue[j];
		j++;
	}
	key[j] = '\0';
	return (key);
}

char	*env_get_value(char *keyvalue)
{
	char	*value;
	int		i;
	int		j;
	int		value_len;
	
	if (!keyvalue || !keyvalue[0])
		return (NULL);
	while (*keyvalue && *keyvalue != '=')
		*keyvalue++;
	value_len = ft_strlen(keyvalue);
	value = ft_calloc(value_len, sizeof(char));
	if (!value)
		return (NULL);
	i = 0;
	*keyvalue++;
	while (keyvalue[i])
	{
		value[i] = keyvalue[i];
		i++;
	}
	return (value);
}

t_env_ecode	env_copy_keyval(t_env **new_node, char *keyvalue)
{
	(*new_node)->keyvalue = ft_strdup(keyvalue);
	if (!(*new_node)->keyvalue)
		return (ENV_GETKEYVALUE_ERROR);
	(*new_node)->key = env_get_key((*new_node)->keyvalue);
	if (!(*new_node)->key)
		return (ENV_GETKEY_ERROR);
	(*new_node)->value = env_get_value((*new_node)->keyvalue);
	if (!(*new_node)->value)
		return (ENV_GETVALUE_ERROR);
	(*new_node)->next = NULL;
	return (ENV_SUCCESS);
}
