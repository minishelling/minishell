/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strlcpy.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/09 13:13:27 by lprieri       #+#    #+#                 */
/*   Updated: 2024/02/02 16:36:10 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
* Function name: ft_strlcpy
* Description: copies up to size - 1 characters
*	from the NUL-terminated string src to dst,
*	NUL-terminating the result.
* External functs.: none
* Return value: return the total length of the string it tried to create.
*	That means the length of src.
* Parameters:
*	char *dst
*	const char *src
*	size_t size
*/

#include "../libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size);

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	ret_value;
	char	*src_ptr;

	ret_value = ft_strlen(src);
	src_ptr = (char *)src;
	if (size == 0)
		return (ret_value);
	while (*src_ptr && (size - 1) > 0)
	{
		*dst = *src_ptr;
		dst++;
		src_ptr++;
		size--;
	}
	*dst = '\0';
	return (ret_value);
}

/*
* Test 1
*/

// int	main(void)
// {
// 	char	str1[10];
// 	char	str2[10];
// 	int		n;

// 	*str1 = '\0';
// 	*str2 = '\0';
// 	n = 4;
// 	printf("%zu\n", strlcpy(str1, "aaaa", n));
// 	printf("%s\n", str1);
// 	printf("%zu\n", ft_strlcpy(str2, "aaaa", n));
// 	printf("%s\n", str2);
// }

/*
* Test 2
*/

// int	main(void)
// {
// 	char	str1[10] = "Blablabla";
// 	char	str2[10] = "Blablabla";
// 	int		n;

// 	n = 10;
// 	printf("%zu\n", strlcpy(str1, "aaaa", n));
// 	printf("%s\n", str1);
// 	printf("%zu\n", ft_strlcpy(str2, "aaaa", n));
// 	printf("%s\n", str2);
// }