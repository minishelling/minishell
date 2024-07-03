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

typedef enum e_codes
{
	ERR_SUCCESS = 0,
	ERR_NULL,
	ERR_COUNT
}	t_ecode;

const char	*get_error_msg(int e_nbr);

#endif