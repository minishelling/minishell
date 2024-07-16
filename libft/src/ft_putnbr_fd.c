/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_putnbr_fd.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/16 10:09:53 by lprieri       #+#    #+#                 */
/*   Updated: 2024/02/02 16:36:10 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
* Function name: ft_putnbr_fd
* Description: Outputs the integer ’n’ to the given file descriptor.
* External functs.: write
* Return value: none
* Parameters:
*	int n: The integer to output.
*	int fd: The file descriptor on which to write.
*/

#include "../libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	long			nbr;
	char			digit;

	if (fd < 0)
		return ;
	nbr = n;
	digit = 0;
	if (n < 0)
	{
		write(fd, "-", 1);
		nbr = -nbr;
	}
	if (nbr == 0)
		write(fd, "0", 1);
	if (nbr > 0 && nbr < 10)
	{
		digit = nbr + '0';
		write (fd, &digit, 1);
	}
	if (nbr >= 10)
	{
		ft_putnbr_fd(nbr / 10, fd);
		digit = (nbr % 10) + '0';
		write (fd, &digit, 1);
	}
}

/*
* Test 1
*/

// int	main(void)
// {
// 	int	n;
// 	int	fd;

// 	n = -2147483648;
// 	fd = 1;
// 	ft_putnbr_fd(n, fd);
// }

/*
* Test 2
*/

// int	main(void)
// {
// 	int	n;
// 	int	fd;

// 	n = 10;
// 	fd = 1;
// 	ft_putnbr_fd(n, fd);
// }

/*
* Test 3
*/

// int	main(void)
// {
// 	int	n;
// 	int	fd;

// 	n = 55510;
// 	fd = 1;
// 	ft_putnbr_fd(n, fd);
// }
