/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 19:39:42 by grohr             #+#    #+#             */
/*   Updated: 2025/01/23 15:08:01 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_conversion(char letter, va_list args)
{
	if (letter == 'c')
		return (convert_char(va_arg(args, int)));
	if (letter == 'd' || letter == 'i')
		return (convert_int(va_arg(args, int)));
	if (letter == 'p')
		return (convert_ptr(va_arg(args, void *)));
	if (letter == 's')
		return (convert_str(va_arg(args, char *)));
	if (letter == 'u')
		return (convert_unsigned(va_arg(args, unsigned int)));
	if (letter == 'x' || letter == 'X')
		return (convert_hex(va_arg(args, unsigned int), letter));
	if (letter == '%')
		return (write(1, "%", 1));
	return (0);
}

int	ft_printf(const char *format, ...)
{
	int		len;
	int		write_result;
	va_list	args;

	len = 0;
	va_start(args, format);
	while (*format)
	{
		if (*format == '%')
		{
			format++;
			write_result = ft_conversion(*format, args);
		}
		else
			write_result = write(1, format, 1);
		if (write_result == -1)
		{
			va_end(args);
			return (-1);
		}
		len += write_result;
		format++;
	}
	va_end(args);
	return (len);
}
