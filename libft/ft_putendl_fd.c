/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:05:19 by grohr             #+#    #+#             */
/*   Updated: 2024/10/15 10:46:13 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putendl_fd(char *s, int fd)
{
	if (s)
	{
		ft_putstr_fd(s, fd);
		ft_putchar_fd('\n', fd);
	}
}

/* #include <fcntl.h>

int	main(void)
{
	int	fd = open("putendl_output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

	if (fd == -1)
		return (1); // Erreur d'ouverture
	ft_putendl_fd("Je putstr ton fichier + retour à la ligne", fd);
	close(fd);
	return (0);
} */