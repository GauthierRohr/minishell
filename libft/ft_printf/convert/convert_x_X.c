/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_x_X.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 21:50:12 by grohr             #+#    #+#             */
/*   Updated: 2025/01/23 15:24:00 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

int	convert_hex(unsigned int n, char letter)
{
	char	*base;
	int		len;
	int		result;

	if (letter == 'X')
		base = "0123456789ABCDEF";
	else
		base = "0123456789abcdef";
	len = 0;
	if (n >= 16)
	{
		result = convert_hex(n / 16, letter);
		if (result == -1)
			return (-1);
		len += result;
	}
	result = write(1, &base[n % 16], 1);
	if (result == -1)
		return (-1);
	return (len + result);
}
