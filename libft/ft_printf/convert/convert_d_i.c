/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_d_i.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 21:47:58 by grohr             #+#    #+#             */
/*   Updated: 2025/01/23 15:15:19 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

static int	ft_putnbr(int n)
{
	int	len;
	int	result;

	len = 0;
	if (n == -2147483648)
		return (write(1, "-2147483648", 11));
	if (n < 0)
	{
		result = write(1, "-", 1);
		if (result == -1)
			return (-1);
		len += result;
		n = -n;
	}
	if (n >= 10)
	{
		result = ft_putnbr(n / 10);
		if (result == -1)
			return (-1);
		len += result;
	}
	result = write(1, &"0123456789"[n % 10], 1);
	if (result == -1)
		return (-1);
	return (len + result);
}

int	convert_int(int n)
{
	return (ft_putnbr(n));
}
