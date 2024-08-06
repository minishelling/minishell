/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   errors.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/03 15:44:25 by lprieri       #+#    #+#                 */
/*   Updated: 2024/07/03 15:52:42 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* STATIC FUNCTION DECLARATIONS */

static void	init_error_msgs_1(char **e_msg);

/* FUNCTIONS */

const char	*get_error_msg(int e_nbr)
{
	static const char	*e_msg[ERR_COUNT];
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
	e_msg[ERR_SUCCESS] = "Success.\n";
	e_msg[ERR_NULL] = "Error\nTrying to access a NULL pointer.\n";
}
