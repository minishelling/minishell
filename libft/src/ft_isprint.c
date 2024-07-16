/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_isprint.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/04 13:39:55 by lprieri       #+#    #+#                 */
/*   Updated: 2024/02/02 16:36:10 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
* Function name: ft_isprint
* Description: checks for any printable character including space.
* External functs.: none
* Return value: int
* Parameters:
*	int c
*/

#include "../libft.h"

int	ft_isprint(int c)
{
	if (c >= 32 && c <= 126)
		return (1);
	return (0);
}

/*
* Test 1 - FT function
*/

// int	main(void)
// {
// 	int		i;
// 	char	c;

// 	i = -1;
// 	c = i;
// 	while (i < 300)
// 	{
// 		c = i;
// 		if (ft_isprint(i) == 1)
// 			printf("%d is TRUE: %c\n", i, c);
// 		else
// 			printf("%d is FALSE: %c\n", i, c);
// 		i++;
// 	}
// }

/*
* Test 2 - Original function
*/

// int	main(void)
// {
// 	int		i;
// 	char	c;

// 	i = -1;
// 	c = i;
// 	while (i < 300)
// 	{
// 		c = i;
// 		if (isprint(i) >= 1)
// 			printf("%d is TRUE: %c\n", i, c);
// 		else
// 			printf("%d is FALSE: %c\n", i, c);
// 		i++;
// 	}
// }
