/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:04:54 by grohr             #+#    #+#             */
/*   Updated: 2024/10/23 19:17:35 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*new_elem;

	new_elem = (t_list *)malloc(sizeof(t_list));
	if (!new_elem)
		return (NULL);
	new_elem->content = content;
	new_elem->next = NULL;
	return (new_elem);
}

/* #include <stdio.h>

int main(void)
{
	t_list *elem = ft_lstnew("Test 1");
	if (elem)
	{
		printf("%s\n", (char *)elem->content);
		free(elem);
	}
	else
	{
		printf("Failed to create element.\n");
	}
	return (0);
} */