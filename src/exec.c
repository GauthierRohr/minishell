/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 22:29:03 by grohr             #+#    #+#             */
/*   Updated: 2025/05/11 13:12:11 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Exécution externe
//
// Fork + execve : pour lancer des binaires.
// Gérer les erreurs de path, permissions, etc.

#include "../inc/minishell.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Supprime les ' et " d'un token, pour afficher la bonne sortie dans le terminal
char	*remove_quotes(const char *str)
{
	int		i;
	int		j;
	char	*result;

	if (!str)
		return (NULL);
	
	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] != '\"' && str[i] != '\'')
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}

// Exécute une commande externe via fork + execvp
int execute_external(char **args, char **envp)
{
    pid_t pid;
    int status;

    pid = fork();
	(void)envp; // temporairement pas utilisé, a voir ce qu'on en fait
    if (pid < 0)
    {
        perror("fork");
        g_last_exit_status = 1;
        return (1);
    }
	if (pid == 0)
	{
		int i = 0;
		while (args[i])
		{
			char *tmp = remove_quotes(args[i]);
			free(args[i]);
			args[i] = tmp;

			//remove_quotes(args[i]); // Supprime quotes avant exec parce que j'avais
			i++;							// laissé dans les tokens pour pas confondre l;exec de ' et "
		}
		if (execvp(args[0], args) == -1)
		{
			perror("execvp");
			exit(EXIT_FAILURE);
		}
	}
	else
    {
        waitpid(pid, &status, 0);
        g_last_exit_status = WEXITSTATUS(status);
        return (g_last_exit_status);
    }
	return (1);
}
