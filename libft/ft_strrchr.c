/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:06:00 by grohr             #+#    #+#             */
/*   Updated: 2024/10/24 12:31:45 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	while (i >= 0)
	{
		if (s[i] == (char) c)
			return ((char *) s + i);
		i--;
	}
	return (NULL);
}

/* #include <stdio.h>

int	main(void)
{
	char	s[] = "Salopette eheh";
	char	c = 'l';
	printf("%s", ft_strrchr(s, c));
}
 */
/*
1.  strrchr cherche la dernière occurrence d’un caractère dans une chaîne
2.	Si le caractère est trouvé, renvoie un pointeur vers cette position
3.	Renvoie NULL si non trouvé
4.	Si je recherches '\0', elle renverra un pointeur vers le caractère
	de fin de chaîne (nul)
*/
/*
Pourquoi faire return (*s[i]) n'est pas ok :

char s[] = "Hello World!";
char *result = ft_strrchr(s, 'o');

	1.	Si on utilise return ((char *) s + i); :
	•	Supposons que l’index i corresponde à la dernière occurrence
		de 'o' dans “Hello World!” (qui est à l’index 7).
	•	s + i pointera à l’adresse de s[7], donc result sera un pointeur
		vers la sous-chaîne “orld!” à partir de 'o'.

	2.	Si on utilisait return (*s[i]); :
	•	Ce serait incorrect, car cela tenterait de déréférencer le caractère
		(c’est-à-dire renvoyer la valeur du caractère à cette position)
		alors que ce qu’on veut, c’est un pointeur vers cette position,
		pas le caractère lui-même.

Conclusion

return ((char *) s + i);
est correct car la fonction doit renvoyer un pointeur vers
la dernière occurrence du caractère trouvé, et s + i calcule cet
emplacement dans la chaîne. Utiliser *s[i] est incorrect car cela
renverrait un caractère et non un pointeur.
*/