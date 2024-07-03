/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_calloc.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/10 12:19:34 by lprieri       #+#    #+#                 */
/*   Updated: 2024/02/02 16:36:10 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
* Function name: ft_calloc
* Description: Allocates memory for an array of nmemb elements of size bytes each
*	and returns a pointer to the allocated memory.
*	The memory is set to zero.
*	If nmemb or size is 0, then calloc() returns either NULL,
*	or a unique pointer value that can be freed.
*	starting at the location pointed to by s,
*	by writing zeros (bytes containing '\0') to that area.
*	If the multiplication of nmemb and size would result in integer over‐flow,
*	then  calloc() returns an error.
* External functs.: malloc
* Return value: void
* Parameters:
*	size_t nmemb
*	size_t size
*/

#include "../libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*ptr;

	if (nmemb == 0 || size == 0)
	{
		ptr = malloc (0);
		return (ptr);
	}
	if ((nmemb * size) / nmemb != size)
		return (NULL);
	ptr = malloc (nmemb * size);
	if (ptr == NULL)
		return (NULL);
	ft_bzero(ptr, (nmemb * size));
	return (ptr);
}

/*
* Test 1
*/

// int	main(void)
// {
// 	char	*str;
// 	size_t	len;

// 	len = 50;
// 	str = (char *) ft_calloc (50, len);
// 	if (str == NULL)
// 	{
// 		printf("NULL\n");
// 		return (0);
// 	}
// 	strcpy(str, "lalal");
// 	printf("%s\n", str);
// 	free (str);
// }

/*
* Test 2 - Tests if len and nmemb 0 still creates a pointer that can be freed.
*/

// int	main(void)
// {
// 	char	*str;

// 	str = (char *) ft_calloc (0, 0);
// 	if (str == NULL)
// 	{
// 		printf("NULL\n");
// 		return (0);
// 	}
// 	free (str);
// }

/*
* Test 3 - Tests if the string contains null characters
*	If it doesn't it should print Null.
*	If it only does at the end it should print c in the first attempt.
*	We search for c afterwards to check if that works too.
*/

// int	main(void)
// {
// 	char	*str;
// 	size_t	len;

// 	len = 3;
// 	str = (char *) ft_calloc (50, len);
// 	if (str == NULL)
// 	{
// 		printf("NULL\n");
// 		return (0);
// 	}
// 	str[148] = 'c';
// 	printf("%s\n", (char *) ft_memchr(str, '\0', 150));
// 	printf("%s\n", (char *) ft_memchr(str, 'c', 150));
// 	free (str);
// }
