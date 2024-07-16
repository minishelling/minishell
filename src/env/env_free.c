/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env_free.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/09 19:35:47 by lprieri       #+#    #+#                 */
/*   Updated: 2024/07/09 19:35:47 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

// #include "../../include/minishell.h"
// #include "../../include/env.h"

// void	env_free_list(t_env	**head)
// {
// 	t_env	*current;
// 	t_env	*temp;

// 	if (!head || !*head)
// 		return ;
// 	current = *head;
// 	while (current != NULL)
// 	{
// 		free(current->keyvalue);
// 		free(current->key);
// 		free(current->value);
// 		temp = current;
// 		current = current->next;
// 		free(temp);
// 	}
// 	free(current);
// }
