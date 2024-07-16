/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_memset.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/04 16:13:35 by lprieri       #+#    #+#                 */
/*   Updated: 2024/02/02 16:36:10 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
* Function name: ft_memset
* Description: fills the first n bytes of the memory area pointed to by s
*	with the constant byte c.
* External functs.: none
* Return value: The memset() function returns a pointer to the memory area s.
* Parameters:
*	void *s
*	int c
*	size_t n
*/

#include "../libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		*(char *)(s + i) = c;
		i++;
	}
	return (s);
}

/*
* Test 1
*/

// int	main(void)
// {
// 	char	*str;

// 	str = (char *) malloc (100);
// 	if (!str)
// 		return (0);
// 	ft_strlcpy(str, "Lalala", 7);
// 	printf("%s\n", str);
// 	str = (char *) ft_memset(str, '4', 4);
// 	printf("%s\n", str);
// 	free (str);
// }
