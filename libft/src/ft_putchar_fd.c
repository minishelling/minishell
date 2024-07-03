/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_putchar_fd.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/16 09:46:17 by lprieri       #+#    #+#                 */
/*   Updated: 2024/02/02 16:36:10 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
* Function name: ft_putchar_fd
* Description: Outputs the character ’c’ to the given file descriptor.
* External functs.: write
* Return value: void
* Parameters:
*	char c: The character to output.
*	int fd: The file descriptor on which to write.
*/

#include "../libft.h"

void	ft_putchar_fd(char c, int fd)
{
	if (fd < 0)
		return ;
	write(fd, &c, 1);
}

/*
* Test 1
*/

// int	main(void)
// {
// 	int		fd;
// 	char	ch;

// 	fd = 1;
// 	ch = 'L';
// 	ft_putchar_fd(ch, fd);
// 	ft_putchar_fd('\n', fd);
// }
