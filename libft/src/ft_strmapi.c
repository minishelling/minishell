/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strmapi.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/15 15:08:20 by lprieri       #+#    #+#                 */
/*   Updated: 2024/02/02 16:36:10 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
* Function name: ft_strmapi
* Description: Applies the function ’f’ to each character of the
*	string ’s’, and passing its index as first argument
*	to create a new string (with malloc(3)) resulting
*	from successive applications of ’f’.
* External functs.: malloc
* Return value: The string created from the successive applications of ’f’.
*	Returns NULL if the allocation fails.
* Parameters:
*	char const *s: The string on which to iterate.
*	void (*f)(unsigned int, char): The function to apply to each character.
*/

#include "../libft.h"

char		*ft_strmapi(char const *s, char (*f)(unsigned int, char));

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	i;
	char			*str;

	i = 0;
	str = (char *) malloc (ft_strlen(s) + 1);
	if (str == NULL)
		return (NULL);
	while (s[i])
	{
		str[i] = f(i, s[i]);
		i++;
	}
	str[i] = '\0';
	return (str);
}

/*
* Test 1
*/

// static char	ft_f(unsigned int index, char ch)
// {
// 	char	c;

// 	c = ch + 1;
// 	return (c);
// }

// int	main(void)
// {
// 	char	*str;
// 	char	*str2;
// 	char	(*f_ptr)(unsigned int, char);

// 	str = "Bla";
// 	f_ptr = ft_f;
// 	str2 = ft_strmapi(str, ft_f);
// 	printf("%s\n", str2);
// 	free (str2);
// }
