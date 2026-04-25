/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 15:47:46 by grohr             #+#    #+#             */
/*   Updated: 2025/04/13 15:48:35 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*table;
	size_t	tot_size;
	size_t	i;

	if (count == 0 || size == 0)
		return (malloc(0));
	tot_size = count * size;
	table = malloc(tot_size);
	if (!table)
		return (NULL);
	i = 0;
	while (i < tot_size)
	{
		((unsigned char *)table)[i] = 0;
		i++;
	}
	return (table);
}

/* #include <stdio.h>

int main()
{
    size_t	count = 5;
    size_t	size = sizeof(int);
	int	*arr;
	size_t	i;

	arr = (int *)ft_calloc(count, size);
    if (arr == NULL)
    {
        printf("Error\n");
        return (1);
    }
	i = 0;
	while (i < count)
	{
        printf("arr[%zu] = %d\n", i, arr[i]);
		i++;
    }
    free(arr);
    return (0);
} */