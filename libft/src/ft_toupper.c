/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_toupper.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/06 16:23:37 by lprieri       #+#    #+#                 */
/*   Updated: 2024/02/02 16:36:10 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
* Function name: ft_toupper
* Description: convert lowercase letters to uppercase.
* External functs.: none
* Return value: the value returned is that of the converted letter,
*	or c if the conversion was not possible.
* Parameters:
*	int c: The character value to be converted.
*/

#include "../libft.h"

int	ft_toupper(int c)
{
	if (c >= 'a' && c <= 'z')
		c -= 32;
	return (c);
}

/*
* Test 1
*/

// int	main(void)
// {
// 	char	str[] = "uppercase sentence.";
// 	int		i;

// 	i = 0;
// 	while (i < ft_strlen(str))
// 	{
// 		str[i] = ft_toupper(str[i]);
// 		i++;
// 	}
// 	printf("%s\n", str);
// }
