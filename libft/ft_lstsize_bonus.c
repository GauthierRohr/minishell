/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:04:57 by grohr             #+#    #+#             */
/*   Updated: 2024/10/23 19:18:21 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	int	count;

	count = 0;
	while (lst)
	{
		count++;
		lst = lst->next;
	}
	return (count);
}

/* #include <stdio.h>

int main(void)
{
	t_list *list = ft_lstnew("First");
	ft_lstadd_back(&list, ft_lstnew("Second"));
	ft_lstadd_back(&list, ft_lstnew("Third"));

	int size = ft_lstsize(list);
	printf("Size of the list: %d\n", size);

	// Free the list
	ft_lstclear(&list, free);
	return (0);
} */