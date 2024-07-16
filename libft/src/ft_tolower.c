/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_tolower.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/06 17:17:53 by lprieri       #+#    #+#                 */
/*   Updated: 2024/02/02 16:36:10 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
* Function name: ft_tolower
* Description: convert uppercase letters to lowercase.
* External functs.: none
* Return value: the value returned is that of the converted letter,
*	or c if the conversion was not possible.
* Parameters:
*	int c: The character value to be converted.
*/

#include "../libft.h"

int	ft_tolower(int c)
{
	if (c >= 'A' && c <= 'Z')
		c += 32;
	return (c);
}

/*
* Test 1
*/

// int	main(void)
// {
// 	char	str[] = "LOWERCASE SENTENCE.";
// 	int		i;

// 	i = 0;
// 	while (i < ft_strlen(str))
// 	{
// 		str[i] = ft_tolower(str[i]);
// 		i++;
// 	}
// 	printf("%s\n", str);
// }
