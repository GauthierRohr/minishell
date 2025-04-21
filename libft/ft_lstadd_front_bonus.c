/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:04:35 by grohr             #+#    #+#             */
/*   Updated: 2024/10/23 19:15:56 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (lst && new)
	{
		new->next = *lst;
		*lst = new;
	}
}

/* #include <stdio.h>

int main(void)
{
	t_list *list = ft_lstnew("Second");
	t_list *new_elem = ft_lstnew("First");

	ft_lstadd_front(&list, new_elem);
	printf("First element: %s\n", (char *)list->content);
	printf("Second element: %s\n", (char *)list->next->content);

	// Free the list
	ft_lstclear(&list, free);
	return (0);
} */