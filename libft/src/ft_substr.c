/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_substr.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/10 15:53:28 by lprieri       #+#    #+#                 */
/*   Updated: 2024/02/02 16:36:10 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
* Function name: ft_substr
* Description: Allocates (with malloc(3)) and returns a substring
*	from the string ’s’.
*	The substring begins at index ’start’ and is of maximum size ’len’.
* External functs.: malloc
* Return value: The substring.
*	NULL if the allocation fails.
* Parameters:
*	char const *s: The string from which to create the substring.
*	unsigned int start: The start index of the substring in the string ’s’.
*	size_t len: The maximum length of the substring.
*/

#include "../libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len);

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*str;
	size_t	s_len;

	s_len = ft_strlen(s);
	if (start > s_len)
	{
		start = s_len;
		len = 0;
	}
	if (len + start > s_len)
		len = s_len - start;
	str = (char *) malloc (len + 1);
	if (str == NULL)
		return (NULL);
	ft_strlcpy(str, &s[start], len + 1);
	return (str);
}

/*
* Test 1 - empty str
*/

// int	main(void)
// {
// 	char			*str;
// 	unsigned int	start;
// 	size_t			len;
// 	char			*substr;

// 	str = "";
// 	start = 2;
// 	len = ft_strlen(str);
// 	substr = ft_substr(str, start, len);
// 	printf("%s\n", substr);
// 	free (substr);
// }

/*
* Test 2 - start > len
*/

// int	main(void)
// {
// 	char			*str;
// 	unsigned int	start;
// 	size_t			len;
// 	char			*substr;

// 	str = "Blablablablabla";
// 	start = 100;
// 	len = ft_strlen(str);
// 	substr = ft_substr(str, start, len);
// 	printf("%s\n", substr);
// 	free (substr);
// }

/*
* Test 3 - len > start && start == 0
*/

// int	main(void)
// {
// 	char			*str;
// 	unsigned int	start;
// 	size_t			len;
// 	char			*substr;

// 	str = "Blablablablabla";
// 	start = 0;
// 	len = ft_strlen(str);
// 	substr = ft_substr(str, start, len);
// 	printf("%s\n", substr);
// 	free (substr);
// }

/*
* Test 4 - len == 1 && start == 0
*/

// int	main(void)
// {
// 	char			*str;
// 	unsigned int	start;
// 	size_t			len;
// 	char			*substr;

// 	str = "Blablablablabla";
// 	start = 0;
// 	len = 1;
// 	substr = ft_substr(str, start, len);
// 	printf("%s\n", substr);
// 	free (substr);
// }
