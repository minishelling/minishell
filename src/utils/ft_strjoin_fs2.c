/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strjoin_fs2.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/31 13:33:50 by lprieri       #+#    #+#                 */
/*   Updated: 2024/10/31 13:33:50 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Returns the concatenation of s1 and s2,
 * while also freeing the second string in the process.
 * 
 * @param s1 The first string.
 * @param s2 A pointer to the second string.
 * 
 * @return If the first string is NULL it frees the second string,
 * and returns NULL. On malloc failure it prints an error message
 * and returns NULL as well. Otherwise it returns a new string
 * that is the product of concatenating s1 and s2.
 */
char	*ft_strjoin_fs2(const char *s1, char **s2)
{
	char	*dst;
	int		i;
	int		j;

	if (!s1)
		return (ft_free((void **) s2), NULL);
	if (!*s2)
		return ((char *) s1);
	dst = ft_calloc(ft_strlen(s1) + ft_strlen(*s2) + 1, sizeof(char));
	if (!dst)
		return (ft_free((void **) s2), NULL);
	i = 0;
	while (s1 && s1[i])
	{
		dst[i] = s1[i];
		i++;
	}
	j = 0;
	while (*s2 && (*s2)[j])
	{
		dst[i + j] = (*s2)[j];
		j++;
	}
	dst[i + j] = '\0';
	return (ft_free((void **) s2), dst);
}
