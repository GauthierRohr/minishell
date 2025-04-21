/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:05:17 by grohr             #+#    #+#             */
/*   Updated: 2024/10/15 10:39:21 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

/* #include <fcntl.h>

int	main(void)
{
	int	fd = open("putchar_output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

	if (fd == -1)
		return (1); // Erreur d'ouverture
	ft_putchar_fd('A', fd);
	ft_putchar_fd('\n', fd);
	close(fd);
	return (0);
}
*/
/*
Fonctionnalité : Écrire 1 caractère donné sur 1 descripteur de fichier spécifié.
Utilise la fonction système write pour envoyer le caractère vers un descripteur.
	c : Le caractère à écrire.
	fd : Le descripteur de fichier où écrire :
		- peut être 1 pour la sortie standard
		- 2 pour la sortie d’erreur, ou tout autre fichier ouvert
*/
/*
#include <fcntl.h> :
Ce fichier d’en-tête fait partie de la bibliothèque standard POSIX.
Il est nécessaire pour utiliser des fonctions et des constantes liées
à la manipulation de fichiers comme open(), O_WRONLY, O_CREAT, O_TRUNC, etc.
*/