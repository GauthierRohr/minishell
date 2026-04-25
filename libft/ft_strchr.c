/* ************************************************************************** */
/*	*/
/*	:::	  ::::::::   */
/*   ft_strchr.c	:+:	  :+:	:+:   */
/*	+:+ +:+	 +:+	 */
/*   By: grohr <grohr@student.42lausanne.ch>	+#+  +:+	   +#+	*/
/*	+#+#+#+#+#+   +#+	   */
/*   Created: 2024/10/03 13:05:29 by grohr	 #+#	#+#	 */
/*   Updated: 2024/10/24 11:58:15 by grohr	###   ########.fr	   */
/*	*/
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	size_t	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == (char) c)
			break ;
		i++;
	}
	if (s[i] == (char) c)
		return ((char *) s + i);
	return (NULL);
}

/* #include <stdio.h>

int	main(void)
{
	char	s[] = "Salopette eheh";
	char	c = 'l';
	printf("%s", ft_strchr(s, c));
}*/
/*
Recherche la première occurrence d’un caractère dans une chaîne de caractères
et renvoie un pointeur vers celle-ci.
*/