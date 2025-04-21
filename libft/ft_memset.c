/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 14:48:41 by grohr             #+#    #+#             */
/*   Updated: 2024/10/09 13:22:12 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

void	*ft_memset(void *b, int c, size_t len)
{
	size_t			i;
	unsigned char	*ptr;

	ptr = (unsigned char *)b;
	i = 0;
	while (i < len)
	{
		ptr[i] = (unsigned char)c;
		i++;
	}
	return (b);
}

/* #include <stdio.h>
// Pour comparer avec la fction memset de la bibli standard
#include <string.h>

int	main(void)
{
	// Tab à init à zéro
	char str[50] = "Ceci est un test pour memset!";

	// Tab initial
	printf("Avant ft_memset : %s\n", str);
	
	// Remplir 5 premiers octets avec 'X'
	ft_memset(str, 'X', 5);
	
	// Tab après utilisation de ft_memset
	printf("Après ft_memset : %s\n", str);

	// Vérifier avec la fction standard memset
	char str2[50] = "Ceci est un test pour memset!";
	memset(str2, 'Y', 5);
	printf("Avec memset standard : %s\n", str2);
	
	return (0);
} */
/* 
Fonctionnalité : Remplit une zone mémoire avec un caractère
				 donné sur n octets.
 */