/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_next_line_utils.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/24 12:34:32 by lprieri       #+#    #+#                 */
/*   Updated: 2024/05/25 21:03:19 by lisandro      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../get_next_line.h"

char	*gnl_strjoin(char const *s1, char const *s2)
{
	char	*str;
	ssize_t	len_s1;
	ssize_t	len_s2;
	ssize_t	i;

	len_s1 = gnl_strlen((char *) s1);
	len_s2 = gnl_strlen((char *) s2);
	str = malloc (len_s1 + len_s2 + 1);
	if (!str)
		return (NULL);
	i = 0;
	while (s1 && s1[i] && i < len_s1)
	{
		str[i] = s1[i];
		i++;
	}
	i = 0;
	while (s2 && s2[i] && i < len_s2)
	{
		str[len_s1 + i] = s2[i];
		i++;
	}
	str[len_s1 + len_s2] = '\0';
	return (str);
}

ssize_t	gnl_checknl(char *str)
{
	ssize_t	i;

	if (str == NULL)
		return (-1);
	i = 0;
	while (str[i])
	{
		if (str[i] == '\n')
			return (i);
		i++;
	}
	return (-1);
}

char	*gnl_strdup(char const *str)
{
	char	*new;
	ssize_t	len;
	ssize_t	i;

	len = gnl_strlen((char *) str);
	if (len == 0)
		return (NULL);
	new = malloc (len + 1);
	if (new == NULL)
		return (NULL);
	i = 0;
	while (str && str[i] && i < len)
	{
		new[i] = str[i];
		i++;
	}
	new[i] = '\0';
	return (new);
}

ssize_t	gnl_strlen(char *str)
{
	ssize_t	i;

	i = 0;
	while (str && str[i])
		i++;
	return (i);
}

void	gnl_free(int fd, char **arr)
{
	if (arr[fd])
	{
		free(arr[fd]);
		arr[fd] = NULL;
	}
}
