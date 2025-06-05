/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtol.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 22:28:19 by grohr             #+#    #+#             */
/*   Updated: 2025/06/05 19:53:40 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"../../inc/minishell.h"
#include <limits.h>

long	ft_strtol(const char *str, char **endptr, int base)
{
	long	result;
	int		sign;
	int		i;

	result = 0;
	sign = 1;
	i = 0;
	while (str[i] == ' ' || (str[i] >= '\t' && str[i] <= '\r'))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	if (base != 10 || !ft_isdigit(str[i]))
	{
		if (endptr)
			*endptr = (char *)str + i;
		return (0);
	}
	while (ft_isdigit(str[i]))
	{
		if (result > (LONG_MAX - (str[i] - '0')) / 10)
		{
			if (endptr)
				*endptr = (char *)str + i;
			return ((sign == 1) ? LONG_MAX : LONG_MIN);
		}
		result = result * 10 + (str[i] - '0');
		i++;
	}
	if (endptr)
		*endptr = (char *)str + i;
	return (result * sign);
}
