/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 14:16:58 by grohr             #+#    #+#             */
/*   Updated: 2025/04/02 17:23:10 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

// Known functions :
// ft_strlen
// ft_strchr
// ft_strjoin

size_t	ft_strlen_g(char *string)
{
	size_t	i;

	i = 0;
	if (!string)
		return (0);
	while (string[i])
		i++;
	return (i);
}

//strchr -> looking for '\n' in the line we want to read.

char	*ft_strchr_g(char *str, int c)
{
	size_t	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
	{
		if (str[i] == (char) c)
			return ((char *)&str[i]);
		i++;
	}
	return (0);
}

char	*ft_strjoin_g(char *s1, char *s2)
{
	size_t		i;
	size_t		j;
	char		*out;

	if (!s1)
	{
		s1 = (char *)malloc(sizeof(char) * 1);
		s1[0] = '\0';
	}
	if (!s2)
		return (NULL);
	out = (char *)malloc(sizeof(char) * (ft_strlen_g(s1)
				+ ft_strlen_g(s2) + 1));
	if (out == NULL)
		return (NULL);
	i = 0;
	j = 0;
	while (s1[i])
		out[j++] = s1[i++];
	i = 0;
	while (s2[i])
		out[j++] = s2[i++];
	out[j] = '\0';
	free(s1);
	return (out);
}

//Reads a line from a file descriptor
//--> returns a new str containing the line of text.

char	*get_next_line(int fd)
{
	char		*line;
	static char	*str;

	if (BUFFER_SIZE <= 0 || fd < 0)
		return (NULL);
	str = ft_add_line(fd, str);
	if (!str)
		return (NULL);
	line = ft_next_line(str);
	str = ft_remove_str_1st_line(str);
	return (line);
}

/*int	main(void)
{
	char	*str;
	int		i;
	int		fd;

	fd = open("../Tests/get_next_line/test.txt", O_RDONLY);
	i = 1;
	while (i < 8)
	{
		s = get_next_line(fd);
		printf("line [%d]: %s", i, s);
		free(s);
		i++;
	}
	close(fd);
	return (0);
}*/