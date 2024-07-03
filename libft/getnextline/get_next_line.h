/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_next_line.h                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/24 12:34:36 by lprieri       #+#    #+#                 */
/*   Updated: 2024/02/06 20:07:09 by lisandro      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <stdio.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif

/* GNL */
char	*get_next_line(int fd);
char	*gnl_parse(int fd, char **rem, char **line);
char	*gnl_get_remainder(char *rem);
char	*gnl_get_nl_line(char *rem);
void	gnl_read(int fd, char **rem);

/* UTILS */
char	*gnl_strjoin(char const *s1, char const *s2);
ssize_t	gnl_checknl(char *str);
char	*gnl_strdup(char const *str);
ssize_t	gnl_strlen(char *str);
void	gnl_free(int fd, char **arr);

#endif