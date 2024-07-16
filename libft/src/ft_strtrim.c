/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strtrim.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/11 12:35:32 by lprieri       #+#    #+#                 */
/*   Updated: 2024/02/02 16:36:10 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
* Function name: ft_strtrim
* Description: Allocates (with malloc(3)) and returns a copy of ’s1’
*	with the characters specified in ’set’ removed
*	from the beginning and the end of the string.
* External functs.: malloc
* Return value: The trimmed string.
*	NULL if the allocation fails.
* Parameters:
*	char const *s1: The string to be trimmed.
*	char const *set: The reference set of characters to trim.
*/

#include "../libft.h"

char	*ft_strtrim(char const *s1, char const *set);

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*str;
	int		i;
	int		j;

	if (!s1 || !set)
		return (NULL);
	i = 0;
	j = ft_strlen(s1) - 1;
	while (ft_strchr(set, s1[i]) && s1[i])
		i++;
	if (i > j)
		return (ft_strdup(""));
	while (ft_strchr(set, s1[j]))
		j--;
	str = malloc (j - i + 2);
	if (str == NULL)
		return (NULL);
	ft_strlcpy(str, &s1[i], (j - i + 2));
	return (str);
}

/*
* Test 1
*/

// int	main(void)
// {
// 	char	str1[] = " ., lalala la. ,";
// 	char	set[] = " .,";
// 	char	*str;

// 	str = ft_strtrim(str1, set);
// 	printf("%lu\n", ft_strlen(str1));
// 	printf("%s\n", str);
// 	printf("%lu\n", ft_strlen(str));
// 	free (str);
// }

/*
* Test 2
*/

// int	main(void)
// {
// 	char	str1[] = "   .  , l,ala.la 1  ..,,   ,.";
// 	char	set[] = " .,";
// 	char	*str;

// 	str = ft_strtrim(str1, set);
// 	printf("%s\n", str);
// 	free (str);
// }

/*
* Test 3 - All the characters in str are in set -> returns empty string.
*/

// int	main(void)
// {
// 	char	str1[] = "   \n\t \n\n\n\n ";
// 	char	set[] = " \n\t";
// 	char	*str;

// 	str = ft_strtrim(str1, set);
// 	printf("%s\n", str);
// 	free (str);
// }

/*
* Test 4 - Empty set and str -> returns empty string.
*/

// int	main(void)
// {
// 	char	str1[] = "";
// 	char	set[] = "";
// 	char	*str;

// 	str = ft_strtrim(str1, set);
// 	printf("%s\n", str);
// 	free (str);
// }
