/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_striteri.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/15 19:49:28 by lprieri       #+#    #+#                 */
/*   Updated: 2024/02/02 16:36:10 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
* Function name: ft_striteri
* Description: Applies the function ’f’ on each character of
*	the string passed as argument, passing its index
*	as first argument. Each character is passed by
*	address to ’f’ to be modified if necessary.
* External functs.: none
* Return value: void
* Parameters:
*	char *s: The string on which to iterate.
*	void (*f)(unsigned int, char*): The function to apply to each character.
*/

#include "../libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char *))
{
	unsigned int	i;

	i = 0;
	if (!s)
		return ;
	while (s[i])
	{
		f(i, &s[i]);
		i++;
	}
	return ;
}

/*
* Test 1
*/

// void	ft_f(unsigned int index, char *strptr)
// {
// 	printf("Iteration %d: %s\n", index + 1, strptr);
// }

// int	main(void)
// {
// 	char	*str;

// 	str = "0123456789";
// 	ft_striteri(str, ft_f);
// }
