/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strlcat.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/09 13:51:50 by lprieri       #+#    #+#                 */
/*   Updated: 2024/02/02 16:36:10 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
* Function name: ft_strlcat
* Description: appends the NUL-terminated string src to the end of dst.
*	It will append at most size - strlen(dst) - 1 bytes,
*	NUL-terminating the result.
* External functs.: none
* Return value: return the total length of the string it tried to create.
*	That means the initial length of dst plus the length of src.
*	Note that a byte for the NUL should be included in size.
*	strlcat() only operate on true “C” strings.
*	If strlcat() traverses size characters without finding a NUL,
*	the length of the string is considered to be size
*	and the destination string will not be NUL-terminated
*	(since there was no space for the NUL).
*	The strlcat() function appends the NUL-terminated string src
*	to the end of dst.  It will append at most size - strlen(dst) - 1 bytes,
*	NUL-termi‐nating the result.
* Parameters:
*	char *dst
*	const char *src
*	size_t size
*/

#include "../libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size);

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	dstlen;

	i = 0;
	dstlen = 0;
	while (dst[dstlen])
		dstlen++;
	while ((dstlen + i) < (size - 1) && src[i] && size > 0)
	{
		dst[dstlen + i] = (char) src[i];
		i++;
	}
	if ((dstlen + i) > (size) || size == 0)
		return (size + ft_strlen(src));
	else
	{
		dst[dstlen + i] = '\0';
		return (dstlen + ft_strlen(src));
	}
}

/*
* Test 1 - size > dstlen && size < (srclen + '\0')
*/

// int	main(void)
// {
// 	char	str1[10];
// 	char	str2[10];
// 	int		n;

// 	*str1 = '\0';
// 	*str2 = '\0';
// 	n = 4;
// 	printf("%zu\n", strlcat(str1, "aaaa", n));
// 	printf("%s\n", str1);
// 	printf("%zu\n", ft_strlcat(str2, "aaaa", n));
// 	printf("%s\n", str2);
// }

/*
* Test 2 - size < dstlen
*/

// int	main(void)
// {
// 	char	str1[10] = "Blablabla";
// 	char	str2[10] = "Blablabla";
// 	int		n;

// 	n = 10;
// 	printf("%zu\n", strlcat(str1, "aaaa", n));
// 	printf("%s\n", str1);
// 	printf("%zu\n", ft_strlcat(str2, "aaaa", n));
// 	printf("%s\n", str2);
// }

/*
* Test 3 - size > dstlen - Undefined results
*/

// int	main(void)
// {
// 	char	str1[10] = "Blablabla";
// 	char	str2[10] = "Blablabla";
// 	int		n;

// 	n = 20;
// 	printf("%zu\n", strlcat(str1, "aaaa", n));
// 	printf("%s\n", str1);
// 	printf("%zu\n", ft_strlcat(str2, "aaaa", n));
// 	printf("%s\n", str2);

// 	// printf("%zu\n", ft_strlcat(str1, "aaaa", n));
// 	// printf("%s\n", str1);
// 	// printf("%zu\n", strlcat(str2, "aaaa", n));
// 	// printf("%s\n", str2);
// }
