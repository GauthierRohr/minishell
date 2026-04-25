/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:05:47 by grohr             #+#    #+#             */
/*   Updated: 2024/10/23 19:45:14 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*new_str;
	unsigned int	i;

	if (!s || !f)
		return (NULL);
	new_str = (char *)malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (!new_str)
		return (NULL);
	i = 0;
	while (s[i])
	{
		new_str[i] = f(i, s[i]);
		i++;
	}
	new_str[i] = '\0';
	return (new_str);
}

/*
//TEST A RETIRER HEIN
char	transform(unsigned int i, char c)
{
	if (i % 2 == 0)
		return (c - 32);
	return (c);
}

int	main(void)
{
	char	*str = "abcdefg";
	char	*result;

	result = ft_strmapi(str, transform);
	printf("Résultat : %s\n", result);
	free(result);
	return (0);
}
*/