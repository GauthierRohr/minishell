/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:04:42 by grohr             #+#    #+#             */
/*   Updated: 2024/10/23 19:16:37 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdelone(t_list *lst, void (*del)(void *))
{
	if (lst)
	{
		if (del)
			del(lst->content);
		free(lst);
	}
}

/* #include <stdio.h>

void del_content(void *content)
{
	free(content);
}

int main(void)
{
	t_list *elem = ft_lstnew(malloc(20)); // Allocate some memory for content
	if (!elem)
		return (1);
	elem->content = strdup("To be deleted");
	printf("Before deletion: %s\n", (char *)elem->content);
	ft_lstdelone(elem, del_content);
	printf("Element deleted.\n");

	return (0);
} */