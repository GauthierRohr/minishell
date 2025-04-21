/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 15:10:00 by grohr             #+#    #+#             */
/*   Updated: 2025/04/13 15:48:13 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	in_set(char c, char const *set)
{
	size_t	i;

	i = 0;
	while (set[i])
	{
		if (c == set[i])
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*result;
	size_t	start;
	size_t	end;
	size_t	len;

	if (!s1 || !set)
		return (NULL);
	start = 0;
	while (s1[start] && in_set(s1[start], set))
		start++;
	end = ft_strlen(s1);
	while (end > start && in_set(s1[end - 1], set))
		end--;
	len = end - start + 1;
	result = (char *)malloc(len * sizeof(char));
	if (!result)
		return (NULL);
	ft_strlcpy(result, &s1[start], len);
	return (result);
}

/* int	main(void)
{
	char	*str = "   xpfwqwD!Arigato :){]}   ";
	char	*set = " xpfqwD!{[]}";
	char	*result = ft_strtrim(str, set);

	printf("Avant trimming: \"%s\"\n", str);
	printf("Après trimming: \"%s\"\n", result);
	free(result);

	return (0);
} */
/*
Le trimming (ou trim) fait référence au processus de suppression des espaces
ou de caractères non désirés au début et à la fin d’une chaîne de caractères.
*/