/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_memchr.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/09 12:31:37 by lprieri       #+#    #+#                 */
/*   Updated: 2024/02/02 16:36:10 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
* Function name: ft_memchr
* Description: scans the initial n bytes of the memory area
*	pointed to by s for the first instance of c.
* External functs.: none
* Return value: void *
* Parameters:
*	const void *s
*	int	c
*	size_t n
*/

#include "../libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*str;
	unsigned char	ch;
	size_t			i;

	str = (unsigned char *) s;
	ch = (unsigned char) c;
	i = 0;
	while (n > i)
	{
		if (str[i] == ch)
			return (&str[i]);
		i++;
	}
	return (NULL);
}

/*
* Test 1 - Searches for the first null terminator,
*	and compares the return with the original function.
*/

// int	main(void)
// {
// 	char	*s;
// 	int		c;
// 	int		n;

// 	s = "/|\x12\xff\x09\0\x42\042\0\42|\\";
// 	c = '\0';
// 	n = 12;
// 	printf("%s\n", (char *) ft_memchr(s, c, n));

// 	if (memchr(s, c, n) == ft_memchr(s, c, n))
// 		printf("Success\n");
// 	else
// 	{
// 		printf("Failed\n");
// 		printf("Original: %s\n", (char *) memchr(s, c, n));
// 		printf("Custom: %s\n", (char *) ft_memchr(s, c, n));
// 	}
// }

/*
* Test 2 - Searches for the first n character in the string "Buongiorno".
*	And compares the return with the original function.
*/

// int	main(void)
// {
// 	char	*s;
// 	int		c;
// 	int		n;

// 	s = "Buongiorno";
// 	c = 'n';
// 	n = 3;
// 	if (memchr(s, c, n) == ft_memchr(s, c, n))
// 		printf("Success\n");
// 	else
// 	{
// 		printf("Failed\n");
// 		printf("Original: %s\n", (char *) memchr(s, c, n));
// 		printf("Custom: %s\n", (char *) ft_memchr(s, c, n));
// 	}
// }
