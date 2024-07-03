/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_bzero.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/05 11:49:37 by lprieri       #+#    #+#                 */
/*   Updated: 2024/02/02 16:36:10 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
* Function name: ft_bzero
* Description: Erases the data in the n bytes of the memory
*	starting at the location pointed to by s,
*	by writing zeros (bytes containing '\0') to that area.
* External functs.: none
* Return value: void
* Parameters:
*	void *s
*	size_t n
*/

#include "../libft.h"

void	ft_bzero(void *s, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		*(char *)(s + i) = '\0';
		i++;
	}
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
// 	ft_bzero(str + 2, 4);
// 	printf("%s\n", str);
// 	free (str);
// }