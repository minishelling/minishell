/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strjoin.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/10 17:50:08 by lprieri       #+#    #+#                 */
/*   Updated: 2024/02/02 16:36:10 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
* Function name: ft_strjoin
* Description: Allocates (with malloc(3)) and returns a new string,
*	which is the result of the concatenationo of ’s1’ and ’s2’.
* External functs.: malloc.
* Return value: The new string. NULL if the allocation fails.
* Parameters:
*	char const *s1: The prefix string.
*	char const *s2: The suffix string.
*/

#include "../libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	size_t	len;
	size_t	i;
	size_t	j;

	len = ft_strlen(s1) + ft_strlen(s2);
	str = (char *) malloc (len + 1);
	if (str == NULL)
		return (NULL);
	i = 0 ;
	while (s1 && s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2 && s2[j])
	{
		str[i + j] = s2[j];
		j++;
	}
	str[i + j] = '\0';
	return (str);
}

/*
* Test 1 - using strjoin within another strjoin function
*/

// int	main(void)
// {
// 	char	str1[] = "my favorite animal is";
// 	char	str2[] = " ";
// 	char	str3[] = "the nyancat";
// 	char	*str;

// 	str = ft_strjoin(ft_strjoin(str1, str2), str3);
// 	printf("%s\n", str);
// 	free (str);
// }

/*
* Test 2 - Appends an empty string
*/

// int	main(void)
// {
// 	char	str1[] = "abc";
// 	char	str2[] = "";
// 	char	*str;

// 	str = ft_strjoin(str1, str2);
// 	printf("%s\n", str);
// 	free (str);
// }

/*
* Test 3 - Appends to an empty string
*/

// int	main(void)
// {
// 	char	str1[] = "";
// 	char	str2[] = "efghi";
// 	char	*str;

// 	str = ft_strjoin(str1, str2);
// 	printf("%s\n", str);
// 	free (str);
// }

/*
* Test 4 - Concatenates 2 strings
*/

// int	main(void)
// {
// 	char	str1[] = "abcd";
// 	char	str2[] = "efghi";
// 	char	*str;

// 	str = ft_strjoin(str1, str2);
// 	printf("%s\n", str);
// 	free (str);
// }
