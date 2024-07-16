/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_itoa.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/10/16 10:29:40 by lprieri       #+#    #+#                 */
/*   Updated: 2024/02/02 16:36:10 by lprieri       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
* Function name: ft_itoa
* Description: Allocates (with malloc(3)) and returns a string
*	representing the integer received as an argument.
*	Negative numbers must be handled.
* External functs.: malloc
* Return value: char *
* Parameters:
*	int n
*/

#include "../libft.h"

static unsigned int	count_int_length(long long n)
{
	unsigned int	i;

	i = 0;
	if (n < 0)
	{
		n *= (-1);
		i++;
	}
	if (n == 0)
		i++;
	while (n > 0)
	{
		n /= 10;
		i++;
	}
	return (i);
}

static void	ft_intstr(char *str, int n)
{
	long long		nbr;
	unsigned int	nbr_len;
	unsigned int	i;

	nbr = n;
	nbr_len = count_int_length(nbr);
	str[nbr_len] = '\0';
	i = 0;
	if (nbr == 0)
	{
		str[i] = '0';
		return ;
	}
	if (nbr < 0)
	{
		str[i] = '-';
		nbr *= (-1);
	}
	while (i < nbr_len && nbr > 0)
	{
		str[nbr_len - 1 - i] = nbr % 10 + '0';
		nbr /= 10;
		i++;
	}
}

char	*ft_itoa(int n)
{
	char			*str;
	unsigned int	nbr_len;

	nbr_len = count_int_length((long long)n);
	str = (char *) malloc (nbr_len + 1);
	if (str == NULL)
		return (NULL);
	ft_intstr(str, n);
	return (str);
}

/*
* Test 1 - Negative number with lots of 0s.
*/

// int	main(void)
// {
// 	int		n;
// 	char	*str;

// 	n = -100000;
// 	str = ft_itoa(n);
// 	printf("%s\n", str);
// 	free (str);
// }

/*
* Test 2 - Min integer
*/

// int	main(void)
// {
// 	int		n;
// 	char	*str;

// 	n = -2147483648;
// 	str = ft_itoa(n);
// 	printf("%s\n", str);
// 	free (str);
// }

/*
* Test 3 - Max integer
*/

// int	main(void)
// {
// 	int		n;
// 	char	*str;

// 	n = 2147483647;
// 	str = ft_itoa(n);
// 	printf("%s\n", str);
// 	free (str);
// }

/*
* Test 4 - Integer overflow
*/

// int	main(void)
// {
// 	int		n;
// 	char	*str;

// 	n = 2147483648;
// 	str = ft_itoa(n);
// 	printf("%s\n", str);
// 	free (str);
// }
