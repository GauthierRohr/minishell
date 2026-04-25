/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:06:06 by grohr             #+#    #+#             */
/*   Updated: 2024/10/23 19:49:17 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub;
	size_t	i;

	if (!s)
		return (NULL);
	else if (start >= ft_strlen(s))
		len = 0;
	else if (len > ft_strlen(s + start))
		len = ft_strlen(s + start);
	sub = (char *)malloc((len + 1) * sizeof(char));
	if (!sub)
		return (NULL);
	i = 0;
	while (i < len)
	{
		sub[i] = s[start + i];
		i++;
	}
	sub[i] = '\0';
	return (sub);
}

/* #include <stdio.h>

int main(void)
{
    char	*str = "How are you ?";
    char	*substr;
	int		start = 4;
	int		len = 3;

    substr = ft_substr(str, start, len);
    printf("Sous-chaîne extraite : \"%s\"\n", substr);

    free(substr);
    return 0;
} */
/*
	1.	Alloue et retourne une nouvelle chaîne :
		crée un nouvel espace en mémoire pour une sous-chaîne.
	2.	Extrait une partie d’une chaîne de caractères :
		La sous-chaîne est extraite de la chaîne s à partir de l’index start.
	3.	Limite la longueur de la sous-chaîne :
		Sous-chaîne de longueur max, même si la chaîne source est + longue.
	4.	Gestion des erreurs :
		Si la chaîne s est nulle ou si l’allocation échoue, retourne NULL.
	5.	Gère les cas où start est trop grand :
		Si start dépasse la longueur de s, elle renvoie une chaîne vide.
*/