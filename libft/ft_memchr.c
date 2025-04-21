/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:05:01 by grohr             #+#    #+#             */
/*   Updated: 2024/10/23 19:36:46 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t				i;
	const unsigned char	*ptr;

	ptr = (const unsigned char *)s;
	i = 0;
	while (i < n)
	{
		if (ptr[i] == (unsigned char) c)
		{
			return ((void *)(ptr + i));
		}
		i++;
	}
	return (NULL);
}

/* #include <stdio.h>

int main(void)
{
    char 	str[] = "Bien le bonsoir";
    char 	ch = 'o';
    char	*res;
	int		n = 13;

    res = ft_memchr(str, ch, n);
    if (res != NULL)
        printf("'%c' found : %s\n", ch, res);
    else
        printf("'%c' not found within the first %d bytes.\n", ch, n);
    return 0;
} */
/*
Recherche la première occurrence d’un caractère spécifique dans une zone
mémoire et renvoie un pointeur vers celle-ci.
*/