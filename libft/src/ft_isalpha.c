/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_isalpha.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/04 10:47:20 by lprieri       #+#    #+#                 */
/*   Updated: 2024/02/02 16:36:10 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
* Function name: ft_isalpha
* Description: checks for an alphabetic character.
* External functs.: none
* Return value: int
* Parameters:
*	int c
*/

#include "../libft.h"

int	ft_isalpha(int c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		return (1);
	return (0);
}

/*
* Test 1 
*/

// int	main(void)
// {
// 	char	a, b, c;

// 	a = 'a';
// 	b = '0';
// 	c = '\t';
// 	printf("%d\n", ft_isalpha(a));
// 	printf("%d\n", ft_isalpha(b));
// 	printf("%d\n", ft_isalpha(c));
// }

/*
* Test 2
*/

// int	main(void)
// {
// 	char	a, b, c;

// 	a = 340;
// 	b = -5;
// 	c = 'A';
// 	printf("%d\n", ft_isalpha(a));
// 	printf("%d\n", ft_isalpha(b));
// 	printf("%d\n", ft_isalpha(c));
// }
