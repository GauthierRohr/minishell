/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:05:10 by grohr             #+#    #+#             */
/*   Updated: 2024/10/24 12:08:37 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t	i;

	i = 0;
	if (!dest && !src)
		return (NULL);
	if (dest != src)
	{
		while (i < n)
		{
			((unsigned char *)dest)[i] = ((unsigned char *)src)[i];
			i++;
		}
	}
	return (dest);
}

/* #include <stdio.h>

int	main(void)
{
	int		i = 10;
	char	dst[18]="Salut beauté";
	char	src[18]="Je te remplace";
	
	printf("dst avant : %s\n", dst);
	printf("src avant : %s\n", src);
	ft_memcpy(dst, src, i);
	printf("dst après : %s\n", dst);
	return(0);
} */
/* 
Copie des octets d’une zone mémoire vers une autre jusqu’à ce qu’un caractère
spécifique soit rencontré, ou jusqu’à ce que n octets aient été copiés.
*/