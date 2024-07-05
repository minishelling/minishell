/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   copy_envp.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/27 10:52:00 by lprieri       #+#    #+#                 */
/*   Updated: 2024/07/04 13:21:10 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// STATIC VARIABLE DECLARATIONS:
static char	*get_key(char *keyvalue);
static char	*get_value(char *keyvalue);
static t_env	*new_env_node(void);
// static void	copy_envp(t_env **env_list, char **envp);

// FUNCTIONS:

// t_env	*init_env_list(char **envp)
// {
// 	t_env	*env_list;

// 	if (!envp)
// 		return (NULL);
// 	env_list = (t_env *) malloc(sizeof(t_env));
// 	if (env_list == NULL)
// 		return (NULL);
	
// }

int	count_envp_vars(char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i])
		i++;
	return (i);
}

void	copy_envp(t_env **env_head, char **envp)
{
	t_env	*current;
	int		i;
	
	current = *env_head;
	while (envp && *envp)
	{
		i = 0;
		while (envp[i] && (*envp)[i] != '\n')
			i++;
		current->keyvalue = ft_substr(*envp, 0, i);
		if (!current->keyvalue)
			exit(1); //
		while (i > 0)
		{
			*envp++;
			i--;
		}
		printf("COPY ENVP TEST: %s\n", *envp);
		printf("%s\n", current->keyvalue);
		current->key = get_key(current->keyvalue);
		printf("%s\n", current->key);
		current->value = get_value(current->keyvalue);
		printf("%s\n", current->value);
		if (!current->key || current->value)
			exit(1); //
		current->next = new_env_node(); // Was it like this with no args?
		if (current->next == NULL)
			exit(1); //
		current = current->next;
	}
}

static char	*get_key(char *keyvalue)
{
	char	*key;
	int		i;
	int		j;

	if (!keyvalue || !keyvalue[0])
		return (NULL);
	i = 0;
	while (keyvalue[i] && keyvalue[i] != '=')
		i++;
	key = ft_calloc(i, sizeof(char));
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

static char	*get_value(char *keyvalue)
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

static t_env	*new_env_node(void)
{
	t_env	*new_node;
	
	new_node = ft_calloc(1, sizeof(t_env));
	if (!new_node)
		return (NULL);
	return (new_node);
}
