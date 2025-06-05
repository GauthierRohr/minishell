/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_pipeline.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 21:04:28 by grohr             #+#    #+#             */
/*   Updated: 2025/06/05 21:30:52 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "../../inc/minishell.h"

static int	count_cmds(char **args)
{
	int	count = 1;
	int	i = 0;

	while (args[i])
	{
		if (ft_strcmp(args[i], "|") == 0)
			count++;
		i++;
	}
	return (count);
}

char	***split_pipeline(char **args)
{
	int		i = 0, cmd_i = 0, j;
	int		num_cmds = count_cmds(args);
	char	***cmds = malloc((num_cmds + 1) * sizeof(char **));

	if (!cmds)
		return (NULL);
	while (args[i])
	{
		int	tok_count = 0, start = i;
		while (args[i] && ft_strcmp(args[i], "|") != 0)
		{
			tok_count++;
			i++;
		}
		cmds[cmd_i] = malloc((tok_count + 1) * sizeof(char *));
		if (!cmds[cmd_i])
			return (NULL); // pas de gestion fuite ici, à voir selon ton malloc wrapper
		j = 0;
		while (start < i)
			cmds[cmd_i][j++] = ft_strdup(args[start++]);
		cmds[cmd_i++][j] = NULL;
		if (args[i])
			i++; // skip '|'
	}
	cmds[cmd_i] = NULL;
	return (cmds);
}
