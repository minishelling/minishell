/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_str_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tfeuer <tfeuer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 13:26:06 by tfeuer            #+#    #+#             */
/*   Updated: 2024/10/31 13:26:07 by tfeuer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int		safe_assign_str(char **dest, const char *src);
size_t	ft_strcspn(const char *str, const char *reject);

/**
 * @brief Safely assigns a string to a destination pointer.
 *
 * This function frees the memory allocated to the destination string (if 
 * any) and assigns a new string copied from the source. If the source 
 * string is NULL, it sets the destination pointer to NULL. In case of 
 * memory allocation failure, it calls `handle_perror` and returns an 
 * error code.
 *
 * @param dest Pointer to the destination string pointer that will be 
 *              assigned a new value.
 * @param src Pointer to the source string to be copied.
 * @return Returns SUCCESS on successful assignment, or an error code 
 *         if memory allocation fails.
 */
int	safe_assign_str(char **dest, const char *src)
{
	if (*dest)
	{
		free(*dest);
		*dest = NULL;
	}
	if (src)
	{
		*dest = ft_strdup(src);
		if (!*dest)
			return (handle_perror("safe_assign_str"), ERR_MEM);
	}
	else
		(*dest) = NULL;
	return (SUCCESS);
}

/**
 * @brief Calculates the length of the initial segment of a string not 
 *        containing any characters from a reject set.
 *
 * This function scans the string `str` for characters that are not 
 * included in the `reject` string, returning the number of characters 
 * at the start of `str` before any character from `reject` is found.
 *
 * @param str The input string to be scanned.
 * @param reject The string containing characters to reject.
 * @return The length of the initial segment of `str` not containing 
 *         any characters from `reject`.
 */
size_t	ft_strcspn(const char *str, const char *reject)
{
	size_t	count;

	count = 0;
	while (str[count])
	{
		if (ft_strchr(reject, str[count]))
			break ;
		count++;
	}
	return (count);
}
