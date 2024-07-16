/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_printf.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: lprieri <lprieri@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/11/03 15:17:36 by lprieri       #+#    #+#                 */
/*   Updated: 2024/01/27 12:41:31 by lisandro      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>

/* FT_PRINTF */
int	ft_printf(const char *arg, ...);
int	ft_conv_specifier(va_list args, const char c);

/* UTILS */
int	ft_putstr_printf(const char *str);
int	ft_putchar_printf(const char c);
int	ft_putptr_printf(void *ptr);
int	ft_putnbr_printf(int nbr);
int	ft_putuns_printf(unsigned int nbr);
int	ft_puthex_printf(unsigned int nbr, char *base);

#endif