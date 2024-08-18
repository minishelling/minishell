/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   errors.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/03 15:46:03 by lprieri       #+#    #+#                 */
/*   Updated: 2024/07/03 15:46:04 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_H
# define ERRORS_H

# include "minishell.h"

typedef enum e_codes
{
	SUCCESS = 0,
	FAILURE,
	PROCEED,
	ERR_NULL,
	ERR_COUNT,
	CWD_ERROR,
	MALLOC_ERROR,
	HOME_ERROR,
	CHDIR_ERROR,
	CDPATH_NULL,
	NULL_STRING,
	ACCESS_ERROR,

}	t_ecode;

const char	*get_error_msg(int e_nbr);

#endif