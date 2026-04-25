/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:05:54 by grohr             #+#    #+#             */
/*   Updated: 2024/10/23 19:46:22 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while ((s1[i] || s2[i]) && i < n)
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}

/*
int main(void)
{
    char *str1 = "Hello";
    char *str2 = "Helium";
    size_t n = 5;
    
    int result = ft_strncmp(str1, str2, n);
    
    if (result == 0)
        printf("First %zu char are equal.\n", n);
    else if (result < 0)
        printf("s1 < s2 is ascii : s1 - s2 = %d", ft_strncmp(str1, str2, n));
    else
        printf("s1 > s2 is ascii : s1 - s2 = %d", ft_strncmp(str1, str2, n));
} */
/*
Compare deux chaînes de caractères sur les n premiers caractères.
Retourne un entier basé sur les différences rencontrées.
*/