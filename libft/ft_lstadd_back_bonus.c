/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:04:28 by grohr             #+#    #+#             */
/*   Updated: 2024/10/24 11:38:49 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*last;

	if (!lst || !new)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	last = ft_lstlast(*lst);
	last->next = new;
}

/* #include <stdio.h>

int main(void)
{
	t_list *list = ft_lstnew("First");
	t_list *new_elem = ft_lstnew("Second");

	ft_lstadd_back(&list, new_elem);
	printf("First element: %s\n", (char *)list->content);
	printf("Second element: %s\n", (char *)list->next->content);

	// Free the list
	ft_lstclear(&list, free);
	return (0);
} */