/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_putendl_fd.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/16 10:05:42 by lprieri       #+#    #+#                 */
/*   Updated: 2024/02/02 16:36:10 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
* Function name: ft_putendl_fd
* Description: Outputs the string ’s’ to the given file descriptor
*	followed by a newline.
* External functs.: write
* Return value: void
* Parameters:
*	char *s: The string to output.
*	int fd: The file descriptor on which to write.
*/

#include "../libft.h"

void	ft_putendl_fd(char *s, int fd)
{
	int	i;

	i = 0;
	if (!s || fd < 0)
		return ;
	while (s[i])
	{
		write(fd, &s[i], 1);
		i++;
	}
	write(fd, "\n", 1);
}

/*
* Test 1
*/

// int	main(void)
// {
// 	char	*str;
// 	int		fd;

// 	str = NULL;
// 	fd = 1;
// 	ft_putendl_fd(str, fd);
// 	return (0);
// }
