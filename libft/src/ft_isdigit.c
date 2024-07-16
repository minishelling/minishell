/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_isdigit.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/04 12:32:46 by lprieri       #+#    #+#                 */
/*   Updated: 2024/02/02 16:36:10 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
* Function name: ft_isdigit
* Description: checks for a digit (0 through 9).
* External functs.: none
* Return value: int
* Parameters:
*	int c
*/

#include "../libft.h"

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
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
// 	printf("%d\n", ft_isdigit(a));
// 	printf("%d\n", ft_isdigit(b));
// 	printf("%d\n", ft_isdigit(c));
// }

/*
* Test 2
*/

// int	main(void)
// {
// 	char	a, b, c;

// 	a = 340;
// 	b = -5;
// 	c = '\0';
// 	printf("%d\n", ft_isdigit(a));
// 	printf("%d\n", ft_isdigit(b));
// 	printf("%d\n", ft_isdigit(c));
// }