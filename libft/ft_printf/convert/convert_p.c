/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_p.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 21:48:02 by grohr             #+#    #+#             */
/*   Updated: 2025/01/23 15:10:13 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf.h"

int	convert_ptr(void *ptr)
{
	unsigned long	p;
	char			*base;
	char			hex[17];
	int				len;
	int				i;

	if (!ptr)
		return (write(1, "0x0", 3));
	p = (unsigned long)ptr;
	base = "0123456789abcdef";
	i = 16;
	hex[i] = '\0';
	while (p)
	{
		hex[--i] = base[p % 16];
		p /= 16;
	}
	len = write(1, "0x", 2);
	if (len == -1)
		return (-1);
	i = write(1, &hex[i], 16 - i);
	if (i == -1)
		return (-1);
	return (len + i);
}
