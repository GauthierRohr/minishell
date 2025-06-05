/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                        :+:    :+:           */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#           */
/*   Created: 2025/04/23 22:28:19 by grohr             #+#    #+#             */
/*   Updated: 2025/06/03 18:04:25 by cjauregu       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	free_env(char **env)
{
	int	i;

	i = 0;
	if (!env)
		return ;
	while (env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
}

void	free_tab(char **tab)
{
	int	i;

	if (!tab)
		return ;
	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

void	cleanup_pipeline(pid_t *pids)
{
	perror("minishell: pipeline error");
	free(pids);
	g_last_exit_status = 1;
}

char *get_command_path(char **args, char **envp)
{
    char *result;
    
    if (strchr(args[0], '/') != NULL)
        result = ft_strdup(args[0]);
    else
        result = ft_get_cmd_path(args[0], envp);
    return result;
}