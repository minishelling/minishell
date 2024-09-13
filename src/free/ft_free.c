/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_free.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/17 15:44:50 by lprieri       #+#    #+#                 */
/*   Updated: 2024/09/13 13:31:03 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_free(void **var)
{
	if (!var || !*var)
		return ;
	free(*var);
	*var = NULL;
	return ;
}
