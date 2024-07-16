/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_puthex_printf.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/12/06 16:45:18 by lprieri       #+#    #+#                 */
/*   Updated: 2024/01/27 12:39:43 by lisandro      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../ft_printf.h"

int	ft_puthex_printf(unsigned int nbr, char *base)
{
	unsigned long long	n;
	int					len;

	n = nbr;
	len = 0;
	if (n < 16)
		len += ft_putchar_printf(base[n]);
	else
	{
		len += ft_puthex_printf(n / 16, base);
		len += ft_putchar_printf(base[n % 16]);
	}
	return (len);
}
