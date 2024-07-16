/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_putnbr_printf.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/12/06 16:44:48 by lprieri       #+#    #+#                 */
/*   Updated: 2024/01/27 12:40:43 by lisandro      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../ft_printf.h"

int	ft_putnbr_printf(int nbr)
{
	long	n;
	int		len;

	n = nbr;
	len = 0;
	if (n < 0)
	{
		n = -n;
		len += ft_putchar_printf('-');
	}
	if (n == 0)
		len += ft_putchar_printf('0');
	if (n > 0 && n < 10)
		len += ft_putchar_printf(n + '0');
	if (n >= 10)
	{
		len += ft_putnbr_printf(n / 10);
		len += ft_putchar_printf((n % 10) + '0');
	}
	return (len);
}
