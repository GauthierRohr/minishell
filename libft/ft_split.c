/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:05:27 by grohr             #+#    #+#             */
/*   Updated: 2025/04/11 18:21:44 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_words(const char *s, char c)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] && s[i] == c)
			i++;
		if (s[i] && s[i] != c)
		{
			count++;
			while (s[i] && s[i] != c)
				i++;
		}
	}
	return (count);
}

static char	*malloc_word(const char *s, int start, int finish)
{
	char	*word;
	int		i;

	word = (char *)malloc((finish - start + 1) * sizeof(char));
	if (!word)
		return (NULL);
	i = 0;
	while (start < finish)
		word[i++] = s[start++];
	word[i] = '\0';
	return (word);
}

char	**ft_split(char const *s, char c)
{
	char	**split;
	int		i;
	int		j;
	int		start;

	if (!s)
		return (NULL);
	split = (char **)malloc((count_words(s, c) + 1) * sizeof(char *));
	if (!split)
		return (NULL);
	i = 0;
	j = 0;
	while (s[i])
	{
		while (s[i] && s[i] == c)
			i++;
		start = i;
		while (s[i] && s[i] != c)
			i++;
		if (i > start)
			split[j++] = malloc_word(s, start, i);
	}
	split[j] = NULL;
	return (split);
}

/* #include <stdio.h>

int	main(void)
{
	char	**result;
	int		i;

	// Test 1 : Chaîne simple avec un espace comme délimiteur
	result = ft_split("Bonjour tout le monde", ' ');
	for (i = 0; result[i] != NULL; i++)
		printf("Test 1 - Mot %d : %s\n", i, result[i]);

	// Test 2 : Chaîne avec plusieurs délimiteurs consécutifs
	result = ft_split("Ceci,,,est,,,un,,,test", ',');
	for (i = 0; result[i] != NULL; i++)
		printf("Test 2 - Mot %d : %s\n", i, result[i]);

	// Test 3 : Chaîne avec délimiteurs en début et fin
	result = ft_split(",,,42,,,School,,,", ',');
	for (i = 0; result[i] != NULL; i++)
		printf("Test 3 - Mot %d : %s\n", i, result[i]);

	// Test 4 : Chaîne vide
	result = ft_split("", ' ');
	if (!result[0])
		printf("Test 4 - Résultat : Chaîne vide\n");

	// Test 5 : Délimiteur absent dans la chaîne
	result = ft_split("Libft", ',');
	for (i = 0; result[i] != NULL; i++)
		printf("Test 5 - Mot %d : %s\n", i, result[i]);

	// Test 6 : Délimiteur unique dans la chaîne
	result = ft_split(",,,,,", ',');
	if (!result[0])
		printf("Test 6 - Résultat : Aucun mot trouvé\n");

	// Libérer la mémoire
	for (i = 0; result[i] != NULL; i++)
		free(result[i]);
	free(result);

	return (0);
} */