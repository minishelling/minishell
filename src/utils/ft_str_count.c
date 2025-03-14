/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_str_count.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/31 13:33:45 by lprieri       #+#    #+#                 */
/*   Updated: 2024/10/31 13:33:46 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Counts the number of strings in an array.
 * 
 * @param cmd_args The array with strings.
 * 
 * @return If the array is NULL or the first element of the array is NULL,
 * it returns 0, else it returns the number of elements the array has.
 */
size_t	ft_str_count(char **cmd_args)
{
	size_t	count;

	if (!cmd_args || !*cmd_args)
		return (0);
	count = 0;
	while (cmd_args[count])
		count++;
	return (count);
}
