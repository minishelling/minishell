/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strchr.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/06 17:23:23 by lprieri       #+#    #+#                 */
/*   Updated: 2024/02/18 11:33:14 by lisandro      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
* Function name: ft_strchr
* Description: returns a pointer to the first occurrence
*	of the character c in the string s.
* External functs.: none
* Return value: pointer to the matched character
*	or NULL if the character is not found.
* Parameters:
*	const char *s
*	int c
*/

#include "../libft.h"

char	*ft_strchr(const char *s, int c)
{
	char	x;

	x = (char) c;
	while (*s)
	{
		if (*s == x)
			return ((char *)s);
		s++;
	}
	if (*s == x)
		return ((char *)s);
	return (NULL);
}

/*
* Test 1 - Seg fault
*/

// int	main(void)
// {
// 	char	*str;
// 	char	c;
// 	char	*result;

// 	str = NULL;
// 	c = '\0';
// 	result = ft_strchr(str, c);
// 	if (result != NULL)
// 	{
// 		printf("%s\n", result); 		//Custom function
// 		printf("%s\n", str);			//Original string
// 		printf("%s\n", strchr(str, c));	//Original function
// 	}
// 	else
// 	{
// 		printf("Null\n");
// 	}
// }

/*
* Test 2 - Match
*/

// int	main(void)
// {
// 	char	*str;
// 	char	c;
// 	char	*result;

// 	str = "Blablabla";
// 	c = 'a';
// 	result = ft_strchr(str, c);
// 	if (result != NULL)
// 	{
// 		printf("%s\n", result); 		//Custom function
// 		printf("%s\n", str);			//Original string
// 		printf("%s\n", strchr(str, c));	//Original function
// 	}
// 	else
// 	{
// 		printf("Null\n");
// 	}
// }

/*
* Test 3 - Null terminator match
*/

// int	main(void)
// {
// 	char	*str;
// 	char	c;
// 	char	*result;

// 	str = "Blablabla";
// 	c = '\0';
// 	result = ft_strchr(str, c);
// 	if (result != NULL)
// 	{
// 		printf("%s\n", result); 		//Custom function
// 		printf("%s\n", str);			//Original string
// 		printf("%s\n", strchr(str, c));	//Original function
// 	}
// 	else
// 	{
// 		printf("Null\n");
// 	}
// }

/*
* Test 4 - No match
*/

// int	main(void)
// {
// 	char	*str;
// 	char	c;
// 	char	*result;

// 	str = "Blablabla";
// 	c = '5';
// 	result = ft_strchr(str, c);
// 	if (result != NULL)
// 	{
// 		printf("%s\n", result); 		//Custom function
// 		printf("%s\n", str);			//Original string
// 		printf("%s\n", strchr(str, c));	//Original function
// 	}
// 	else
// 	{
// 		printf("Null\n");
// 		printf("%s\n", strchr(str, c));	//Original function
// 	}
// }
