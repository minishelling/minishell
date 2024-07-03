/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   copy_envp.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/27 10:52:00 by lprieri       #+#    #+#                 */
/*   Updated: 2024/07/03 15:39:32 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// STATIC VARIABLE DECLARATIONS:
static char	*get_key(char *keyvalue);
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

// t_env	*create_env_node(char *keyvalue, char *key, char *value)
// {
	
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
			exit(1) ;
		printf("%s\n", current->keyvalue);
		current->key = get_key(current->keyvalue);
		if (!current->key)
			exit(1) ;
		free(current->keyvalue);
		free(current->key);
		return ;
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
	printf("%s\n", key); //
	return (key);
}
