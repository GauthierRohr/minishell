/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 12:26:19 by grohr             #+#    #+#             */
/*   Updated: 2024/10/24 12:26:55 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (!haystack && !len)
		return (0);
	if (needle[0] == '\0' || needle == haystack)
		return ((char *)haystack);
	while (haystack[i] != '\0')
	{
		j = 0;
		while (haystack[i + j] == needle[j] && (i + j) < len)
		{
			if (haystack[i + j] == '\0' && needle[j] == '\0')
				return ((char *)&haystack[i]);
			j++;
		}
		if (needle[j] == '\0')
			return ((char *)(haystack + i));
		i++;
	}
	return (0);
}

/*#include <stdio.h>

int	main(void)
{
	const char	haystack[] = "Heyy ahah";
	const char	needle[] = "ahah";
	char		*result;

	result = ft_haystacknhaystack(haystack, needle, 11);
	if (result)
		printf("Found: %s\n", result);
	else
		printf("Not Found\n");
	return (0);
}*/
/*
Recherche une sous-chaîne dans une chaîne jusqu’à une longueur max donnée.
- - - -
Cherche la première occurrence de la chaîne needle dans la chaîne haystack,
mais en ne cherchant que dans les "len" premiers caractères de haystack.
Si needle est trouvé, renvoie un pointeur sur la 1ère occurrence
de needle dans haystack.
Sinon, elle renvoie NULL.
Si needle est une chaîne vide, haystacknhaystack retourne haystack.
*/