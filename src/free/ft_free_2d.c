/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_free_2d.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/31 13:33:04 by lprieri       #+#    #+#                 */
/*   Updated: 2024/10/31 13:33:05 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Frees a dynamically allocated 2D array and sets its pointer to NULL.
 *
 * This function safely frees memory allocated to a 2D array and sets the 
 * array pointer to `NULL` to prevent dangling pointers. It accepts a 
 * triple pointer representing a 2D array, checks that it’s valid, and 
 * performs the following actions:
 * - Iterates through each element of the array, freeing any dynamically 
 *   allocated memory.
 * - Frees the main array pointer itself after all elements are freed.
 * - Sets the main pointer to `NULL` to prevent future use.
 *
 * @param[in,out] arr A triple pointer to the 2D array to be freed.
 *                    If the pointer is valid, this memory will be freed,
 *                    and the pointer will be set to NULL.
 *
 * @note This function is designed to handle 2D arrays where the inner
 *       elements and the outer array have been dynamically allocated.
 */
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
