/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:04:48 by grohr             #+#    #+#             */
/*   Updated: 2024/10/23 19:17:13 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstlast(t_list *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

/* #include <stdio.h>

int main(void)
{
	t_list *list = ft_lstnew("First");
	ft_lstadd_back(&list, ft_lstnew("Second"));
	ft_lstadd_back(&list, ft_lstnew("Third"));

	t_list *last = ft_lstlast(list);
	if (last)
	{
		printf("Last element: %s\n", (char *)last->content);
	}

	// Free the list
	ft_lstclear(&list, free);
	return (0);
} */