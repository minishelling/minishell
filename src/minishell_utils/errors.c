/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   errors.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/03 15:44:25 by lprieri       #+#    #+#                 */
/*   Updated: 2024/09/13 15:12:33 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* STATIC FUNCTION DECLARATIONS */

static void	init_error_msgs_1(char **e_msg);

/* FUNCTIONS */

const char	*get_error_msg(int e_nbr)
{
	static const char	*e_msg[COUNT_ERROR];
	static int			is_initialized = 0;

	if (!is_initialized)
	{
		init_error_msgs_1((char **) e_msg);
		is_initialized = 1;
	}
	return (e_msg[e_nbr]);
}

static void	init_error_msgs_1(char **e_msg)
{
	e_msg[SUCCESS] = "Success.\n";
	e_msg[NULL_ERROR] = "Error\nTrying to access a NULL pointer.\n";
}
