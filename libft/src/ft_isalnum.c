/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_isalnum.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/04 12:45:08 by lprieri       #+#    #+#                 */
/*   Updated: 2024/02/02 16:36:10 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
* Function name: ft_isalnum
* Description: checks for an alphanumeric character;
*	it is equivalent to (isalpha(c) || isdigit(c)).
* External functs.: none
* Return value: int
* Parameters:
*	int c
*/

#include "../libft.h"

int	ft_isalnum(int c)
{
	if ((c >= '0' && c <= '9')
		|| (c >= 'A' && c <= 'Z')
		|| (c >= 'a' && c <= 'z'))
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

// 	a = '0';
// 	b = 'a';
// 	c = 'A';
// 	d = 300;
// 	e = '\0';
// 	f = 32;

// 	printf("%d\n", ft_isalnum(a));
// 	printf("%d\n", ft_isalnum(b));
// 	printf("%d\n", ft_isalnum(c));
// 	printf("%d\n", ft_isalnum(d));
// 	printf("%d\n", ft_isalnum(e));
// 	printf("%d\n", ft_isalnum(f));
// }
