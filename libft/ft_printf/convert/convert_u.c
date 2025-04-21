/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_u.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 21:50:09 by grohr             #+#    #+#             */
/*   Updated: 2025/01/23 15:20:52 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

static int	ft_putunsigned(unsigned int n)
{
	int			len;
	int			result;
	const char	*base;

	base = "0123456789";
	len = 0;
	if (n >= 10)
	{
		result = ft_putunsigned(n / 10);
		if (result == -1)
			return (-1);
		len = len + result;
	}
	result = write(1, &base[n % 10], 1);
	if (result == -1)
		return (-1);
	return (len + result);
}

int	convert_unsigned(unsigned int n)
{
	return (ft_putunsigned(n));
}
