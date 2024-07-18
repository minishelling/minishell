// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        ::::::::            */
// /*   ft_strjoin_fs1.c                                   :+:    :+:            */
// /*                                                     +:+                    */
// /*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
// /*                                                   +#+                      */
// /*   Created: 2024/07/12 12:44:53 by lprieri       #+#    #+#                 */
// /*   Updated: 2024/07/12 12:44:53 by lprieri       ########   odam.nl         */
// /*                                                                            */
// /* ************************************************************************** */

// #include "../../include/utils.h"

// char	*ft_strjoin_fs1(char *s1, const char *s2)
// {
// 	char	*dst;
// 	size_t	len;
// 	int		i;
// 	int		j;

// 	if (!s1)
// 		return (NULL);
// 	len = ft_strlen(s1) + ft_strlen(s2);
// 	dst = malloc (sizeof(char) * (len + 1));
// 	if (!dst)
// 		return (ft_free((void **) &s1), NULL);
// 	i = 0;
// 	while (s1 && s1[i])
// 	{
// 		dst[i] = s1[i];
// 		i++;
// 	}
// 	j = 0;
// 	while (s2 && s2[j])
// 	{
// 		dst[i + j] = s2[j];
// 		j++;
// 	}
// 	dst[i + j] = '\0';
// 	return (ft_free((void **) &s1), dst);
// }
