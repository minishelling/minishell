/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strnstr.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/09 17:02:53 by lprieri       #+#    #+#                 */
/*   Updated: 2024/02/02 16:36:10 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
* Function name: ft_strnstr
* Description: The strnstr() function locates the first occurrence
*	of the null-terminated string little in the string big,
*	where not more than len characters are searched.
*	Characters that appear after a ‘\0’ character are not searched.
* External functs.: none
* Return value: If little is an empty string, big is returned;
*	if little occurs nowhere in big, NULL is returned;
*	otherwise a pointer to the first character
*	of the first occurrence of little is returned.
* Parameters:
*	const char *big
*	const char *little
*	size_t len
*/

#include "../libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len);

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;

	if (!(little[0]))
		return ((char *) big);
	i = 0;
	j = 0;
	while (big[i] && little[j] && len > 0)
	{
		j = 0;
		while (big[i] == little[j] && little[j] && len > 0)
		{
			i++;
			j++;
			len --;
		}
		if (!(little[j]))
			return ((char *) &big[i - j]);
		i -= j;
		len += j;
		i++;
		len--;
	}
	return (NULL);
}

/*
* Test 1 - len = 0
*/

// int	main(void)
// {
// 	char	*big;
// 	char	*little;
// 	size_t	len;

// 	big = "fbiuewbfiewHolafiuewfwiebiw";
// 	little = "Hola";
// 	len = 0;
// 	if (ft_strnstr(big, little, len) == NULL)
// 		printf("NULL\n");
// 	else
// 	{
// 		printf("%s\n", ft_strnstr(big, little, len));
// 	}
// }

/*
* Test 2 - len > biglen
*/

// int	main(void)
// {
// 	char	*big;
// 	char	*little;
// 	size_t	len;

// 	big = "fbiuewbfiewHolafiuewfwiebiw";
// 	little = "Hola";
// 	len = 50;
// 	if (ft_strnstr(big, little, len) == NULL)
// 		printf("NULL\n");
// 	else
// 	{
// 		printf("%s\n", ft_strnstr(big, little, len));
// 	}
// }

/*
* Test 3 - len is just not enough to scan little
*/

// int	main(void)
// {
// 	char	*big;
// 	char	*little;
// 	size_t	len;

// 	big = "0123456789Holafiuewfwiebiw";
// 	little = "Hola";
// 	len = 13;
// 	if (ft_strnstr(big, little, len) == NULL)
// 		printf("NULL\n");
// 	else
// 	{
// 		printf("%s\n", ft_strnstr(big, little, len));
// 	}
// }

/*
* Test 4 - little is empty && len > biglen
*/

// int	main(void)
// {
// 	char	*big;
// 	char	*little;
// 	size_t	len;

// 	big = "0123456789Holafiuewfwiebiw";
// 	little = "";
// 	len = 50;
// 	if (ft_strnstr(big, little, len) == NULL)
// 		printf("NULL\n");
// 	else
// 	{
// 		printf("%s\n", ft_strnstr(big, little, len));
// 	}
// }
