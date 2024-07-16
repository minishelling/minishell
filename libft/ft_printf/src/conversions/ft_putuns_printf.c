/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_putuns_printf.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/12/06 16:45:03 by lprieri       #+#    #+#                 */
/*   Updated: 2024/01/27 12:38:58 by lisandro      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../ft_printf.h"

int	ft_putuns_printf(unsigned int nbr)
{
	int		len;
	char	digit;

	len = 0;
	digit = 0;
	if (nbr == 0)
	{
		write (1, "0", 1);
		len++;
	}
	if (nbr > 0 && nbr < 10)
	{
		digit = nbr + '0';
		write (1, &digit, 1);
		len++;
	}
	if (nbr >= 10)
	{
		len += ft_putuns_printf(nbr / 10);
		digit = (nbr % 10) + '0';
		write (1, &digit, 1);
		len++;
	}
	return (len);
}
