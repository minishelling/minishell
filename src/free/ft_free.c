/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_free.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/17 15:44:50 by lprieri       #+#    #+#                 */
/*   Updated: 2024/07/17 15:45:51 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/free.h"

void	ft_free(void **var)
{
	if (!var || !*var)
		return ;
	free(*var);
	*var = NULL;
	return ;
}
