#ifndef HISTORY_H
# define HISTORY_H

typedef struct s_history
{
	char				*line;
	struct s_history	*next;
}	t_history;

void	add_to_history(t_history **history, const char *line);
void	print_history(t_history *history);
void	free_history(t_history *history);

#endif