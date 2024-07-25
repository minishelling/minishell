/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strjoin_fs2.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/25 12:49:40 by lprieri       #+#    #+#                 */
/*   Updated: 2024/07/25 12:49:40 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/utils.h"

char	*ft_strjoin_fs2(const char *s1, char **s2)
{
	char	*dst;
	size_t	len;
	int		i;
	int		j;

	if (!s1)
	{
		ft_free((void **) s2);
		return (NULL);
	}
	if (!*s2)
		return ((char *) s1);
	len = ft_strlen(s1) + ft_strlen(*s2);
	dst = malloc (sizeof(char) * (len + 1));
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