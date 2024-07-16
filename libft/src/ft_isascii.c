/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_isascii.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/04 13:16:37 by lprieri       #+#    #+#                 */
/*   Updated: 2024/02/02 16:36:10 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
* Function name: ft_isascii
* Description: checks whether c is a 7-bit unsigned char value
*	that fits into the ASCII character set.
* External functs.: none
* Return value: int
* Parameters:
*	int c
*/

#include "../libft.h"

int	ft_isascii(int c)
{
	if (c >= 0 && c <= 127)
		return (1);
	return (0);
}

/*
* Test 1
*/

// int	main(void)
// {
// 	char	a, b, c;
// 	char	d, e, f;
// 	char	x, y, z;

// 	a = '0';
// 	b = 'a';
// 	c = 'A';
// 	d = 300;
// 	e = '\0';
// 	f = 32;
// 	x = 127;
// 	y = -5;
// 	z = 0;

// 	printf("%d\n", ft_isascii(a));
// 	printf("%d\n", ft_isascii(b));
// 	printf("%d\n", ft_isascii(c));
// 	printf("%d\n", ft_isascii(d));
// 	printf("%d\n", ft_isascii(e));
// 	printf("%d\n", ft_isascii(f));
// 	printf("%d\n", ft_isascii(x));
// 	printf("%d\n", ft_isascii(y));
// 	printf("%d\n", ft_isascii(z));
// }