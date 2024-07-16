/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strdup.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/10 14:27:36 by lprieri       #+#    #+#                 */
/*   Updated: 2024/02/12 15:04:30 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
* Function name: ft_strdup
* Description: returns a pointer to a new string
*	which is a duplicate of the string s.
* External functs.: malloc
* Return value: On success, it returns a pointer to the duplicated string.
*	It returns NULL if insufficient memory was available
* Parameters:
*	const char *s
*/

#include "../libft.h"

char	*ft_strdup(const char *s)
{
	char	*str;
	int		strlen;
	int		i;

	if (!s)
		return (NULL);
	strlen = ft_strlen(s);
	str = (char *) malloc (strlen + 1);
	if (str == NULL)
		return (NULL);
	i = 0;
	while (i < strlen)
	{
		str[i] = s[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

/*
* Test 1 - Duplicates a string twice
*/

// int	main(void)
// {
// 	char	*str = "lalalala";
// 	char	*str2;
// 	char	*str3;

// 	str2 = strdup(str);
// 	str3 = strdup(str);
// 	printf("%s\n", str2);
// 	printf("%s\n", str3);
// 	free (str2);
// 	free (str3);
// }

/*
* Test 2 - Duplicates an empty string
*/

// int	main(void)
// {
// 	char	*str = "";
// 	char	*str2;

// 	str2 = strdup(str);
// 	printf("%s\n", str2);
// 	free (str2);
// }
