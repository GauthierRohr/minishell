/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:04:45 by grohr             #+#    #+#             */
/*   Updated: 2024/10/23 19:16:58 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	while (lst)
	{
		f(lst->content);
		lst = lst->next;
	}
}

/* #include <stdio.h>

void print_content(void *content)
{
	printf("%s\n", (char *)content);
}

int main(void)
{
	t_list *list = ft_lstnew(strdup("First"));
	ft_lstadd_back(&list, ft_lstnew(strdup("Second")));
	ft_lstadd_back(&list, ft_lstnew(strdup("Third")));

	printf("List contents:\n");
	ft_lstiter(list, print_content);

	// Free the list
	ft_lstclear(&list, free);
	return (0);
} */