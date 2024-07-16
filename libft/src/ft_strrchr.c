/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strrchr.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/06 14:03:54 by lprieri       #+#    #+#                 */
/*   Updated: 2024/02/05 15:09:43 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
* Function name: ft_strrchr
* Description: returns a pointer to the last occurrence
*	of the character c in the string s.
* External functs.: none
* Return value: pointer to the matched character
*	or NULL if the character is  not found.
* Parameters:
*	const char *s
*	int c
*/

#include "../libft.h"

char	*ft_strrchr(const char *s, int c);

char	*ft_strrchr(const char *s, int c)
{
	size_t			slen;
	unsigned char	x;

	x = (unsigned char) c;
	slen = ft_strlen(s);
	if (s[slen] == x)
		return ((char *)(s + slen));
	while (slen > 0)
	{
		slen--;
		if (s[slen] == x)
			return ((char *)(s + slen));
	}
	return (NULL);
}

/*
* Test 1 - Null terminator on empty string
*/

// int	main(void)
// {
// 	char	*str1;
// 	char	c;

// 	str1 = "";
// 	c = '\0';
// 	printf("%s\n", ft_strrchr(str1, c));
// 	printf("%s\n", strrchr(str1, c));
// }

/*
* Test 2
*/

// int	main(void)
// {
// 	char	*str1;
// 	char	c;

// 	str1 = "Blablablabla";
// 	c = 'b';
// 	printf("%s\n", ft_strrchr(str1, c));
// 	printf("%s\n", strrchr(str1, c));
// }

/*
* Test 3
*/

// int	main(void)
// {
// 	char	*str1;
// 	char	c;

// 	str1 = "Blablablabla";
// 	c = 'B';
// 	printf("%s\n", ft_strrchr(str1, c));
// 	printf("%s\n", strrchr(str1, c));
// }
