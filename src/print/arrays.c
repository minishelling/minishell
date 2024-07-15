/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   arrays.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/03 15:41:01 by lprieri       #+#    #+#                 */
<<<<<<< HEAD
/*   Updated: 2024/07/10 20:44:44 by lprieri       ########   odam.nl         */
=======
/*   Updated: 2024/07/13 12:35:48 by lprieri       ########   odam.nl         */
>>>>>>> 54960a345359b2734c792fab35776f7d6234e030
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_print_2d_arr(char **arr)
{
	int	i;

	if (!arr || !*arr)
	{
		ft_putstr_fd((char *) get_error_msg(ERR_NULL), STDERR_FILENO);
		return ;
	}
	i = 0;
	while (arr[i])
	{
		ft_printf("%s\n", arr[i]);
		i++;
	}
}

void	ft_print_3d_arr(char ***arr)
{
	int	i;
	int	j;

	if (!arr || !*arr)
	{
		ft_putstr_fd((char *) get_error_msg(ERR_NULL), STDERR_FILENO);
		return ;
	}
	i = 0;
	while (arr[i])
	{
		j = 0;
		while (arr[j])
		{
			ft_printf("%s\n", arr[i][j]);
			j++;
		}
		i++;
	}
}
