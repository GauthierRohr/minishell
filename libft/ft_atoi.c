/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 12:41:33 by grohr             #+#    #+#             */
/*   Updated: 2024/10/24 11:42:03 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *str)
{
	int	i;
	int	final;
	int	sign;

	i = 0;
	final = 0;
	sign = 1;
	while ((str[i] >= 9 && str[i] <= 13) || (str[i] == ' '))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -sign;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		final = final * 10 + str[i] - 48;
		i++;
	}
	return (final * sign);
}

/*int	main(void)
{
	char str[] = "   --++-3630letelephoneduperenoel2626";
	int	result = ft_atoi(str);
	
	printf("%d", result);
	return (0);
}*/
/*
Convertit une chaîne de caractères en un entier (int).
Ignore les espaces blancs en début, gère les signes (+ ou -)
et continue jusqu’à rencontrer un caractère non numérique.
*/