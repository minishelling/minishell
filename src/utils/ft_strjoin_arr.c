/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strjoin_arr.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/31 13:33:46 by lprieri       #+#    #+#                 */
/*   Updated: 2024/10/31 13:33:47 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Concatenates str to every element of the array.
 * 
 * @param arr An array of strings.
 * @param str A string to concatenate to every element of the array.
 * @return A new array where each element has str concatenated to it,
 * or NULL on failure.
 */
char	**ft_strjoin_arr(char **arr, char *str)
{
	char	**new;
	size_t	i;

	if (!arr || !str)
		return (NULL);
	i = 0;
	while (arr[i])
		i++;
	new = (char **) ft_calloc(i + 1, sizeof(char *));
	if (!new)
		return (handle_perror("ft_strjoin_arr"), NULL);
	i = 0;
	while (arr[i])
	{
		new[i] = ft_strjoin(str, arr[i]);
		if (!new[i])
			return (ft_free_2d((void ***) &new),
				handle_perror("ft_strjoin_arr"), NULL);
		i++;
	}
	new[i] = NULL;
	return (new);
}
