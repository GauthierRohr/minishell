/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:05:42 by grohr             #+#    #+#             */
/*   Updated: 2024/10/24 12:29:15 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char	*restrict dst, const char *restrict src, size_t dstsize)
{
	size_t	i;

	i = 0;
	if (dstsize > 0)
	{
		while (src[i] && i < (dstsize - 1))
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
	while (src[i])
		i++;
	return (i);
}

/* #include <stdio.h>

int	main(void)
{
	char	dst[50] = "Salut les gars!!!";
	char	*src = "Merci";
	size_t	ah;

	printf("src: %s\n", src);
	printf("dst: %s\n", dst);
	ah = ft_strlcpy(dst, src, -1);

	printf("src: %s\n", src);
	printf("dst: %s\n", dst);
	printf("size of src: %zu\n", ah);

	return (0);
} */
/*
Copie une chaîne de caractères dans une autre avec une taille max prédéfinie.
Assure que la destination est toujours null-terminée.
*/