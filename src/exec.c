/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 22:29:03 by grohr             #+#    #+#             */
/*   Updated: 2025/05/27 16:34:26 by grohr            ###   ########.fr       */
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
    int i, j;
    int start = 0;

    if (!str)
        return (NULL);
    len = ft_strlen(str);
    result = malloc(len + 1);
    if (!result)
        return (NULL);
    // Preserve leading + or -
    if (len >= 3 && (str[0] == '+' || str[0] == '-') &&
        ((str[1] == '"' && str[len - 1] == '"') || (str[1] == '\'' && str[len - 1] == '\'')))
    {
        result[start++] = str[0]; // Keep + or -
        i = 2;  // Skip leading quote
        len -= 1;  // Ignore closing quote
    }
    else if (len >= 2 &&
        ((str[0] == '"' && str[len - 1] == '"') || (str[0] == '\'' && str[len - 1] == '\'')))
    {
        i = 1;  // Skip leading quote
        len -= 1;  // Ignore closing quote
    }
    else
    {
        i = 0;  // No quotes to remove
    }
    // Copy remaining characters
    j = start;
    while (i < len)
    {
        result[j++] = str[i++];
    }
    result[j] = '\0';
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

//char *expand_vars(const char *token, char **env, t_state quote_state);

// exec.c - fonction execute_external
int execute_external(char **args, char **envp)
{
    pid_t pid;
    int status;
    struct stat st;
    int i;

    // Pour chaque argument, enlever les quotes et faire l’expansion
    for (i = 0; args[i]; i++)
    {
        char *tmp = remove_quotes(args[i]);
        free(args[i]);
        args[i] = tmp;

        tmp = expand_vars(args[i], envp);
        free(args[i]);
        args[i] = tmp;
    }

    // Supprimer les arguments vides (ex: après expansion d’une variable vide)
    args = clean_args(args);
    if (!args || !args[0] || args[0][0] == '\0')
    {
        // Comme bash : pas de commande -> succès
        return 0;
    }

    pid = fork();
    if (pid < 0)
    {
        perror("fork");
        return 1;
    }
    else if (pid == 0)
    {
        if (strchr(args[0], '/'))
        {
            if (stat(args[0], &st) == 0)
            {
                if (S_ISDIR(st.st_mode))
                {
                    fprintf(stderr, "%s: is a directory\n", args[0]);
                    exit(PERMISSION_DENIED);
                }
                if (access(args[0], X_OK) != 0)
                {
                    fprintf(stderr, "%s: Permission denied\n", args[0]);
                    exit(PERMISSION_DENIED);
                }
            }
        }
        char *cmd_path = NULL;
        if (strchr(args[0], '/'))
            cmd_path = ft_strdup(args[0]);
        else
            cmd_path = ft_get_cmd_path(args[0], envp);
        if (!cmd_path)
        {
            fprintf(stderr, "%s: command not found\n", args[0]);
            exit(CMD_NOT_FOUND);
        }
        execve(cmd_path, args, envp);
        perror(args[0]);
        free(cmd_path);
        exit(CMD_NOT_FOUND);
    }
    else
    {
        waitpid(pid, &status, 0);
        g_last_exit_status = WEXITSTATUS(status);
        return WEXITSTATUS(status);
    }
}
