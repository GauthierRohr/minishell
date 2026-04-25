/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:05:35 by grohr             #+#    #+#             */
/*   Updated: 2024/10/23 19:42:39 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	unsigned int	i;

	if (!s || !f)
		return ;
	i = 0;
	while (s[i])
	{
		f(i, &s[i]);
		i++;
	}
}

/*
//TEST A RETIRER HEIN
#include <stdio.h>

void	uppercase(unsigned int i, char *c)
{
	if (*c >= 'a' && *c <= 'z')
		*c = *c - 32;
}

int	main(void)
{
	char str[] = "tout en majuscules!";

	ft_striteri(str, uppercase);
	printf("Chaîne modifiée : %s\n", str);
	return (0);
}
*/
/*
Comme strmapi sauf qu'on modifie directement la chaine existante plutot que
de créer un nouvelle chaine.
La fonction ft_striteri permet d’appliquer une fonction f à chaque caractère
d’une chaîne de caractères, en passant l’index du caractère et l’adresse du
caractère à la fonction.
Cela permet à la fonction f de modifier directement les caractères de
la chaîne en place.
Elle n’alloue pas de nouvelle chaîne, elle modifie directement celle qui
est passée en paramètre.
*/