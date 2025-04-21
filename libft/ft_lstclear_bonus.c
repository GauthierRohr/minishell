/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:04:39 by grohr             #+#    #+#             */
/*   Updated: 2024/10/23 19:16:25 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*current;
	t_list	*next;

	if (!lst || !*lst)
		return ;
	current = *lst;
	while (current)
	{
		next = current->next;
		ft_lstdelone(current, del);
		current = next;
	}
	*lst = NULL;
}

/* #include <stdio.h>

void del_content(void *content)
{
	free(content);
}

int main(void)
{
	t_list *list = ft_lstnew(strdup("First"));
	ft_lstadd_back(&list, ft_lstnew(strdup("Second")));
	ft_lstadd_back(&list, ft_lstnew(strdup("Third")));

	ft_lstclear(&list, del_content);
	if (list == NULL)
	{
		printf("List cleared successfully.\n");
	}
	return (0);
} */