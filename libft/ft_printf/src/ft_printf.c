/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_printf.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/03 14:31:17 by lprieri       #+#    #+#                 */
/*   Updated: 2024/01/27 12:38:33 by lisandro      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

static int	ft_print_arg(const char *arg, va_list args)
{
	int		i;
	int		len;
	int		conv;

	i = 0;
	len = 0;
	conv = 0;
	while (arg && arg[i])
	{
		if (arg[i] == '%')
		{
			i++;
			conv = ft_conv_specifier(args, arg[i]);
			if (conv == -1)
				return (-1);
			len += conv;
		}
		else
			len += ft_putchar_printf(arg[i]);
		i++;
	}
	return (len);
}

int	ft_conv_specifier(va_list args, const char c)
{
	int	len;

	len = 0;
	if (c == 'c')
		len += ft_putchar_printf(va_arg(args, int));
	else if (c == 's')
		len += ft_putstr_printf(va_arg(args, char *));
	else if (c == 'p')
		len += ft_putptr_printf(va_arg(args, void *));
	else if (c == 'd' || c == 'i')
		len += ft_putnbr_printf(va_arg(args, int));
	else if (c == 'u')
		len += ft_putuns_printf(va_arg(args, unsigned int));
	else if (c == 'x')
		len += ft_puthex_printf(va_arg(args, unsigned int), "0123456789abcdef");
	else if (c == 'X')
		len += ft_puthex_printf(va_arg(args, unsigned int), "0123456789ABCDEF");
	else if (c == '%')
		len += ft_putchar_printf('%');
	else
		len = -1;
	return (len);
}

int	ft_printf(const char *arg, ...)
{
	va_list	args;
	int		len;

	va_start(args, arg);
	len = 0;
	if (!arg)
		return (-1);
	len += ft_print_arg(arg, args);
	va_end(args);
	return (len);
}
