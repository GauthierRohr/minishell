/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 14:17:23 by grohr             #+#    #+#             */
/*   Updated: 2025/03/13 17:52:26 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

// ft_add_line :
// --> Reads a line from fd
// --> Returns a string containing this line.
//
// read(int fd, void *buffer, size_t count) :
// --> reads fd for a "count" numbers of characters
// --> returns the ssize_t of byte read from file descriptor
// --> allocates *buffer with the content read from fd.
//
// Each call of read() keeps the actual reading position thanks to
// the internal kernel mechanism for managing file descriptors.

char	*ft_add_line(int fd, char *str)
{
	char	*buffer;
	ssize_t	bytes_read;

	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (NULL);
	bytes_read = 1 ;
	while ((ft_strchr_g(str, '\n') == 0) && bytes_read > 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			free(str);
			free(buffer);
			return (NULL);
		}
		buffer[bytes_read] = '\0';
		str = ft_strjoin_g(str, buffer);
	}
	free(buffer);
	return (str);
}

// Extracts the next line from str
// -->returns a new str (next_str) with the next line of text.

char	*ft_next_line(char *str)
{
	char	*next_str;
	int		i;

	i = 0;
	if (str[i] == 0)
		return (NULL);
	while (str[i] && str[i] != '\n')
		i++;
	next_str = (char *)malloc(sizeof(char) * (i + 2));
	if (!next_str)
		return (NULL);
	i = 0;
	while (str[i] && str[i] != '\n')
	{
		next_str[i] = str[i];
		i++;
	}
	if (str[i] == '\n')
	{
		next_str[i] = '\n';
		i++;
	}
	next_str[i] = '\0';
	return (next_str);
}

// Removes the next line of text from a string
// and returns a new string containing the remaining text

char	*ft_remove_str_1st_line(char *line)
{
	char	*string;
	int		i;
	int		j;

	i = 0;
	while (line[i] && line[i] != '\n')
		i++;
	if (!line[i])
	{
		free(line);
		return (NULL);
	}
	string = (char *)malloc(sizeof(char) * (ft_strlen_g(line) + 1 - i));
	if (!string)
	{
		free(line);
		return (NULL);
	}
	i++;
	j = 0;
	while (line[i])
		string[j++] = line[i++];
	string[j] = '\0';
	free(line);
	return (string);
}
