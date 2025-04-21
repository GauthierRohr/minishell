/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:05:24 by grohr             #+#    #+#             */
/*   Updated: 2024/10/15 13:43:35 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putstr_fd(char *s, int fd)
{
	if (s)
		write(fd, s, ft_strlen(s));
}

/* #include <fcntl.h>

int	main(void)
{
	int	fd = open("putstr_output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

	if (fd == -1)
		return (1);
	ft_putstr_fd("Je putstr ton fichier\n", fd);
	close(fd);
	return (0);
} */