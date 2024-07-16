/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_memcmp.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/09 12:42:17 by lprieri       #+#    #+#                 */
/*   Updated: 2024/02/02 16:36:10 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
* Function name: ft_memcmp
* Description: returns an integer less than, equal to, or greater than zero
*	if the first n bytes of s1 is found, respectively, to be less than,
*	to match, or be greater than the first n bytes of s2.
* External functs.: none
* Return value: int
* Parameters:
*	const void *s1
*	const void *s2
*	size_t n
*/

#include "../libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*str1;
	unsigned char	*str2;
	size_t			i;

	str1 = (unsigned char *) s1;
	str2 = (unsigned char *) s2;
	i = 0;
	while (n > i)
	{
		if (str1[i] != str2[i])
			return (str1[i] - str2[i]);
		i++;
	}
	return (0);
}

/*
* Test 1 - n < the point where the strings being compared
*	start to differ -> return = 0
*/

// int	main(void)
// {
// 	char	*str1;
// 	char	*str2;
// 	int		n;

// 	str1 = "lalala5l6aac";
// 	str2 = "lalala5l6";
// 	n = 7;
// 	printf("%d\n", memcmp(str1, str2, n));
// 	printf("%d\n", ft_memcmp(str1, str2, n));
// }

/*
* Test 2 - n > the point where the strings being compared start to differ
*/

// int	main(void)
// {
// 	char	*str1;
// 	char	*str2;
// 	int		n;

// 	str1 = "lalala5l6aac";
// 	str2 = "lalala5l6";
// 	n = 20;
// 	printf("%d\n", memcmp(str1, str2, n));
// 	printf("%d\n", ft_memcmp(str1, str2, n));
// }
