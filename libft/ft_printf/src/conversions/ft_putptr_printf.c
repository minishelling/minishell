/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_putptr_printf.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/12/06 16:44:10 by lprieri       #+#    #+#                 */
/*   Updated: 2024/01/27 12:39:29 by lisandro      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../ft_printf.h"

static int	ft_puthex_ptr(unsigned long long nbr, char *base)
{
	unsigned long long	n;
	int					len;

	len = 0;
	n = nbr;
	if (n < 16)
		len += ft_putchar_printf(base[n]);
	else
	{
		len += ft_puthex_ptr(n / 16, base);
		len += ft_putchar_printf(base[n % 16]);
	}
	return (len);
}

int	ft_putptr_printf(void *ptr)
{
	int				len;
	unsigned long	nbr;

	if (ptr == NULL)
		len = ft_putstr_printf("(nil)");
	else
	{
		nbr = (unsigned long long) ptr;
		len = ft_putstr_printf("0x");
		len += ft_puthex_ptr(nbr, "0123456789abcdef");
	}
	return (len);
}
