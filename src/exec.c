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

char *expand_vars(const char *token, char **env, t_state quote_state);

// The execute_external function without using errno.
int execute_external(char **args, char **envp)
{
    pid_t pid;
    int status;
    struct stat st;
    int i;

    // For each token:
    // 1. Remove enclosing quotes.
    // 2. Expand variables using STATE_GENERAL (i.e. no special quoting).
    for (i = 0; args[i]; i++)
    {
        char *tmp = remove_quotes(args[i]);
        free(args[i]);
        args[i] = tmp;
        
        tmp = expand_vars(args[i], envp, STATE_GENERAL);
        free(args[i]);
        args[i] = tmp;
    }

    // Remove tokens that are now empty (e.g. for an unset $EMPTY).
    args = clean_args(args);
    if (!args || !args[0] || args[0][0] == '\0')
    {
        // Mimic bash: if no command remains, return success.
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
        /* In the child process */
        // If the command appears to be a file path (contains a slash)
        if (strchr(args[0], '/'))
        {
            if (stat(args[0], &st) == 0)
            {
                if (S_ISDIR(st.st_mode))
                {
                    fprintf(stderr, "%s: Is a directory\n", args[0]);
                    exit(PERMISSION_DENIED);
                }
                if (access(args[0], X_OK) != 0)
                {
                    fprintf(stderr, "%s: Permission denied\n", args[0]);
                    exit(PERMISSION_DENIED);
                }
            }
        }
        // Attempt to execute the command.
        execvp(args[0], args);
        if (strchr(args[0], '/'))
        {
            fprintf(stderr, "%s: No such file or directory\n", args[0]);
            exit(CMD_NOT_FOUND);
        }
        else
        {
            fprintf(stderr, "%s: command not found\n", args[0]);
            exit(CMD_NOT_FOUND);
        }
    }
    else
    {
        /* In the parent process, wait for the child to finish */
        waitpid(pid, &status, 0);
        g_last_exit_status = WEXITSTATUS(status);
        return WEXITSTATUS(status);
    }
}
