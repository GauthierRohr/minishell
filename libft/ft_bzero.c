/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:04:12 by grohr             #+#    #+#             */
/*   Updated: 2024/10/23 20:02:47 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	unsigned char	*ptr;
	size_t			i;

	ptr = (unsigned char *)s;
	if (n == 0)
		return ;
	i = 0;
	while (i < n)
	{
		ptr[i] = 0;
		i++;
	}
}

/*
#include <stdio.h>
#include <strings.h> // Pour comparer avec la fonction bzero standard

int	main(void)
{
	char str[50] = "Test de bzero!";
	printf("Avant ft_bzero : %s\n", str);
	ft_bzero(str, 6);

	//Après ft_bzero
	/!\ Il peut ne plus être possible d'afficher la chaîne correctement,
	car le '\0' met fin à l'affichage de la chaîne.
	printf("Après ft_bzero : %s\n", str);
	
	// Vérif avec fction standard bzero
	char str2[50] = "Test de bzero!";
	bzero(str2, 6);
	printf("Avec bzero standard : %s\n", str2);
	return (0);
}
*/
/*
Initialise à zéro (met des octets de valeur 0) les
n premiers octets de la zone mémoire pointée par s.
*/
/*  Pourquoi convertir en unsigned char ?
	En C, chaque type de donnée a une taille spécifique en mémoire.
	Un int ou float peut être de 4 octets, tandis qu’un char est de 1 octet.

	Le type void* est un pointeur générique, ce qui signifie qu’il n’a pas
	d’infos sur le type exact qu’il pointe.
	Pour manipuler des zones mémoire au niveau des octets, il est crucial d’avoir
	un type de pointeur qui se réfère explicitement à chaque octet,
	et c’est là qu’intervient unsigned char*.

	•	char est le type qui garantit la manipulation d’un seul octet.
	•	unsigned garantit que chaque octet est traité comme une valeur
		entre 0 et 255 (plutôt qu’entre -128 et 127 comme avec signed char).

	Pourquoi une valeur supérieure à 127 serait interprétée comme négative ?
	127 en binaire est 01111111.
	Valeurs de 128 à 255 : Ces valeurs sont représentées par leurs équivalents
						   négatifs lorsqu’on les interprète comme signed char.

	Exemple : 128 en binaire est 10000000 : interprété comme -128 en signed char.
	Exemple : 255 en binaire est 11111111 : interprété comme -1 en signed char.
*/