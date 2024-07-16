/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_next_line.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/24 12:34:38 by lprieri       #+#    #+#                 */
/*   Updated: 2024/05/25 21:02:24 by lisandro      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../get_next_line.h"

char	*get_next_line(int fd)
{
	static char	*rem[1024];
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
	{
		gnl_free(fd, rem);
		return (NULL);
	}
	if (rem[fd] && gnl_checknl(rem[fd]) >= 0)
		return (gnl_parse(fd, rem, &line));
	gnl_read(fd, rem);
	if (rem[fd] && gnl_checknl(rem[fd]) >= 0)
		return (gnl_parse(fd, rem, &line));
	else if (rem[fd] && rem[fd][0] == '\0')
	{
		gnl_free(fd, rem);
		return (NULL);
	}
	else
	{
		line = gnl_strdup(rem[fd]);
		gnl_free(fd, rem);
		return (line);
	}
	return (line);
}

void	gnl_read(int fd, char **rem)
{
	char	*buffer;
	ssize_t	bytes_read;
	char	*temp;

	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return ;
	buffer[0] = '\0';
	bytes_read = 1;
	while (bytes_read > 0 && gnl_checknl(buffer) < 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read < 0)
		{
			gnl_free(fd, &(*rem));
			free(buffer);
			return ;
		}
		buffer[bytes_read] = '\0';
		temp = gnl_strjoin(rem[fd], buffer);
		gnl_free(fd, &(*rem));
		rem[fd] = gnl_strdup(temp);
		free(temp);
	}
	free(buffer);
}

char	*gnl_parse(int fd, char **rem, char **line)
{
	char	*temp;

	temp = gnl_get_remainder(rem[fd]);
	*line = gnl_get_nl_line(rem[fd]);
	gnl_free(fd, rem);
	rem[fd] = gnl_strdup(temp);
	free(temp);
	return (*line);
}

char	*gnl_get_remainder(char *rem)
{
	char	*new_rem;
	ssize_t	nl_index;
	ssize_t	rem_len;
	ssize_t	i;

	nl_index = gnl_checknl(rem);
	rem_len = gnl_strlen(rem);
	new_rem = malloc(rem_len - nl_index + 1);
	if (!new_rem)
		return (NULL);
	i = 0;
	while (rem && rem[i] && i < (rem_len - nl_index))
	{
		new_rem[i] = rem[nl_index + 1 + i];
		i++;
	}
	new_rem[i] = '\0';
	return (new_rem);
}

char	*gnl_get_nl_line(char *rem)
{
	char	*line;
	ssize_t	line_len;
	ssize_t	i;

	line_len = gnl_checknl(rem);
	if (line_len < 0 && !(rem))
		return (NULL);
	line = malloc(line_len + 2);
	if (!line)
		return (NULL);
	i = 0;
	while (rem && rem[i] && i < line_len)
	{
		line[i] = rem[i];
		i++;
	}
	if (rem && rem[i] && rem[i] == '\n')
	{
		line[i] = rem[i];
		i++;
	}
	line[i] = '\0';
	return (line);
}
