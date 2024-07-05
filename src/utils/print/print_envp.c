/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   print_envp.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/03 21:34:44 by lprieri       #+#    #+#                 */
/*   Updated: 2024/07/03 21:34:44 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	print_env_list(t_env *head, int node_nbr)
{
	if (!head || !head->keyvalue || !head->key || !head->value)
		return ;
	printf("Node number: %i\n", node_nbr);
	printf("Key-value: %s\n", head->keyvalue);
	printf("Key: %s\n", head->key);
	printf("Value: %s\n", head->value);
	head = head->next;
	print_env_list(head, node_nbr++);
}
