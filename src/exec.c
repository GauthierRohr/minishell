/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 22:29:03 by grohr             #+#    #+#             */
/*   Updated: 2025/05/13 18:30:09 by grohr            ###   ########.fr       */
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
char *remove_quotes(const char *str)
{
    char *result;
    int len;
    int i;
    int j;

    if (!str)
        return (NULL);
    
    len = ft_strlen(str);
    result = malloc(len + 1);
    if (!result)
        return (NULL);
    
    i = 0;
    j = 0;
    
    if ((len >= 2) && 
        ((str[0] == '\'' && str[len - 1] == '\'') || 
         (str[0] == '"' && str[len - 1] == '"')))
    {
        i = 1;
        while (i < len - 1)
        {
            result[j++] = str[i++];
        }
    }
    else
    {
        while (str[i])
        {
            result[j++] = str[i++];
        }
    }
    
    result[j] = '\0';
    //printf("DEBUG: remove_quotes('%s') -> '%s'\n", str, result);
    return (result);
}

char *remove_partial_quotes(const char *str)
{
    char *result = malloc(strlen(str) + 1);
    int i = 0, j = 0;
    t_state state = STATE_GENERAL;

    if (!result)
        return (NULL);

    while (str[i])
    {
        if (state == STATE_GENERAL && (str[i] == '\'' || str[i] == '"'))
        {
            if (str[i] == '\'')
                state = STATE_IN_SINGLE_QUOTE;
            else
                state = STATE_IN_DOUBLE_QUOTE;
            i++;
            continue;
        }
        else if ((state == STATE_IN_SINGLE_QUOTE && str[i] == '\'') ||
                 (state == STATE_IN_DOUBLE_QUOTE && str[i] == '"'))
        {
            state = STATE_GENERAL;
            i++;
            continue;
        }
        result[j++] = str[i++];
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
