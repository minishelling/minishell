/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env_init.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/27 10:52:00 by lprieri       #+#    #+#                 */
/*   Updated: 2024/07/10 09:07:21 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/env.h"

/*	STATIC FUNCTION DECLARATIONS	*/

static t_env	*env_new_copied_node(char *keyvalue, t_env_ecode *status);

/*	FUNCTIONS */

t_env_ecode	env_init_list(t_env **head, char **envp)
{
	t_env		*current;
	t_env		*new_node;
	t_env_ecode	status;	
	size_t		i;

	if (!envp || !*envp)
		return (ENV_NULL);
	i = 0;
	while (envp[i])
	{
		new_node = env_new_copied_node(envp[i], &status);
		if (status != ENV_SUCCESS)
			return (status);
		if (!*head)
		{
			*head = new_node;
			current = *head;
		}
		else
		{
			current->next = new_node;
			current = current->next;
		}
		i++;
	}
}

static t_env	*env_new_copied_node(char *keyvalue, t_env_ecode *status)
{
	t_env	*new_node;

	*status = ENV_SUCCESS;
	new_node = env_new_node();
	if (!new_node)
	{
		*status = ENV_NEW_NODE_ERROR;
		return (NULL);
	}
	*status = env_copy_keyval(&new_node, keyvalue);
	if (*status != ENV_SUCCESS)
		return (NULL);
	return (new_node);
}
