/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_free_3d.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/17 15:45:09 by lprieri       #+#    #+#                 */
/*   Updated: 2024/09/07 13:23:05 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_free_3d(void ****arr)
{
	int	i;
	int	j;

	if (!arr || !*arr)
		return ;
	i = 0;
	while ((*arr)[i] != NULL)
	{
		j = 0;
		while ((*arr)[i][j] != NULL)
		{
			free((*arr)[i][j]);
			j++;
		}
		free((*arr)[i]);
		i++;
	}
	free(*arr);
	*arr = NULL;
	return ;
}
