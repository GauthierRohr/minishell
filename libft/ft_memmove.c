/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:05:13 by grohr             #+#    #+#             */
/*   Updated: 2024/10/24 12:30:09 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	size_t	i;

	if (!dst && !src)
		return (0);
	if (dst > src)
	{
		i = len;
		while (i > 0)
		{
			i--;
			((unsigned char *)dst)[i] = ((unsigned char *)src)[i];
		}
	}
	else
	{
		i = 0;
		while (i < len)
		{
			((unsigned char *)dst)[i] = ((unsigned char *)src)[i];
			i++;
		}
	}
	return (dst);
}

/* #include <stdio.h>

int	main(void)
{
	int		i = 10;
	char	dst[18]="Salut";
	
	printf("dst avant : %s\n", dst);
	ft_memmove(dst, dst + 2, i);
	printf("dst après : %s\n", dst);
	return(0);
} */
/*
Copie n octets d’une zone mémoire vers une autre, en prenant en compte
le chevauchement possible des zones source et destination.
*/