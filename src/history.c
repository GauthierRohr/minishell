#include "../inc/history.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Ajoute une ligne à la fin de l'historique
void	add_to_history(t_history **history, const char *line)
{
	t_history	*new_node;
	t_history	*tmp;

	new_node = malloc(sizeof(t_history));
	if (!new_node)
		return ;
	new_node->line = strdup(line);
	new_node->next = NULL;
	if (!*history)
		*history = new_node;
	else
	{
		tmp = *history;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_node;
	}
}

// Affiche l'historique
void	print_history(t_history *history)
{
	int	i;

	i = 1;
	while (history)
	{
		printf("%d: %s\n", i, history->line);
		history = history->next;
		i++;
	}
}

// Libère l'historique complet
void	free_history(t_history *history)
{
	t_history	*tmp;

	while (history)
	{
		tmp = history->next;
		free(history->line);
		free(history);
		history = tmp;
	}
}