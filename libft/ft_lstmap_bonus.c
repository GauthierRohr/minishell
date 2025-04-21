/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 13:04:51 by grohr             #+#    #+#             */
/*   Updated: 2024/10/23 19:19:06 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_list;
	t_list	*new_elem;

	new_list = NULL;
	while (lst)
	{
		new_elem = ft_lstnew(f(lst->content));
		if (!new_elem)
		{
			ft_lstclear(&new_list, del);
			return (NULL);
		}
		ft_lstadd_back(&new_list, new_elem);
		lst = lst->next;
	}
	return (new_list);
}

/* #include <stdio.h>

void *duplicate_content(void *content)
{
	return strdup((char *)content);
}

void del_content(void *content)
{
	free(content);
}

void print_content(void *content)
{
	printf("%s\n", (char *)content);
}

int main(void)
{
	t_list *list = ft_lstnew(strdup("First"));
	ft_lstadd_back(&list, ft_lstnew(strdup("Second")));
	ft_lstadd_back(&list, ft_lstnew(strdup("Third")));

	t_list *new_list = ft_lstmap(list, duplicate_content, del_content);
	printf("Mapped list contents:\n");
	ft_lstiter(new_list, print_content);

	// Free the lists
	ft_lstclear(&list, del_content);
	ft_lstclear(&new_list, del_content);
	return (0);
} */