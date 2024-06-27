/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   copy_envp.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/27 10:52:00 by lprieri       #+#    #+#                 */
/*   Updated: 2024/06/27 12:46:09 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "include/minishell.h"

t_env	*init_env_list(char **envp)
{
	t_env	*env_list;
	
	if (!envp)
		return (NULL);
	env_list = NULL;
	
}
