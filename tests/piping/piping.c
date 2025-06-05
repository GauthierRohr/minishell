/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piping.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 21:03:07 by grohr             #+#    #+#             */
/*   Updated: 2025/06/05 21:30:48 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "../../inc/minishell.h"

int	contains_pipe(char **args)
{
	int	i;

	i = 0;
	if (!args)
		return (0);
	while (args[i])
	{
		if (ft_strcmp(args[i], "|") == 0)
			return (1);
		i++;
	}
	return (0);
}

int	handle_pipes(char **args, char **envp)
{
	if (!contains_pipe(args))
		return (0);
	return (find_and_execute_pipeline(args, envp));
}
