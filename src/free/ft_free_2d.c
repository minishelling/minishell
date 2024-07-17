/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_free_2d.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/17 15:44:59 by lprieri       #+#    #+#                 */
/*   Updated: 2024/07/17 15:45:36 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/free.h"

void	ft_free_2d(void ***arr)
{
	int	i;

	if (!arr || !*arr)
		return ;
	i = 0;
	while ((*arr)[i] != NULL)
	{
		free((*arr)[i]);
		i++;
	}
	free(*arr);
	*arr = NULL;
	return ;
}
