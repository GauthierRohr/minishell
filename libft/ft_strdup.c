/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:05:31 by grohr             #+#    #+#             */
/*   Updated: 2024/10/23 19:42:17 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char	*cpy;
	int		len;
	int		i;

	len = 0;
	i = 0;
	while (s1[len])
		len++;
	cpy = (char *)malloc(sizeof(char) * (len + 1));
	if (!cpy)
		return (NULL);
	i = 0;
	while (i < len)
	{
		cpy [i] = s1[i];
		i++;
	}
	cpy[len] = '\0';
	return (cpy);
}

/* #include <stdio.h>

int main(void)
{
    const char	*ori = "Comment est votre blanquette?";
    char		*cpy = ft_strdup(ori);

    if (cpy == NULL)
    {
        printf("Erreur d'allocation de mémoire!\n");
        return (1);
    }
    printf("Original chain : %s\n", ori);
    printf("Dup chain : %s\n", cpy);
    free(cpy);
    const char *vide = "";
    cpy = ft_strdup(vide);
    printf("\nChaîne vide dupliquée : \"%s\"\n", cpy);
    free(cpy);
    return 0;
} */
/*
	1.	Allouer de la mémoire :
Alloue dynamiquement de la mémoire pour contenir 1 cpy d’1 chaîne de caract.
	2.	Copier la chaîne :
Copie tous les caractères de la chaîne d’entrée dans la nouvelle zone mémoire,
y compris le caractère nul \0 de fin de chaîne (+1 au malloc)
	3.	Retourner un pointeur :
Renvoie un pointeur vers cette nouvelle chaîne allouée en mémoire.
	4.	Gérer les erreurs de mémoire :
Si l’allocation de mémoire échoue, elle renvoie NULL.
*/