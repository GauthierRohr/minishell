/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 16:15:27 by grohr             #+#    #+#             */
/*   Updated: 2025/05/05 16:15:28 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

// Cleans args array by removing NULL entries after redirections
char **clean_args(char **args)
{
    char **new_args;
    int i, j, count = 0;

    if (!args)
        return (NULL);
    
    // Count non-NULL arguments
    for (i = 0; args[i]; i++)
        if (args[i] != NULL)
            count++;
    
    // Allocate new array
    new_args = malloc((count + 1) * sizeof(char *));
    if (!new_args)
        return (NULL);
    
    // Copy non-NULL arguments
    j = 0;
    for (i = 0; args[i]; i++)
    {
        if (args[i] != NULL)
        {
            new_args[j] = strdup(args[i]);
            if (!new_args[j])
            {
                // Free partially allocated array on failure
                while (j > 0)
                    free(new_args[--j]);
                free(new_args);
                return (NULL);
            }
            j++;
        }
    }
    new_args[j] = NULL; // Ensure NULL termination
    
    return (new_args);
}

// Backs up stdio file descriptors
int backup_stdio(int *stdin_backup, int *stdout_backup)
{
    *stdin_backup = dup(STDIN_FILENO);
    *stdout_backup = dup(STDOUT_FILENO);
    if (*stdin_backup == -1 || *stdout_backup == -1)
    {
        perror("minishell: stdio backup");
        if (*stdin_backup != -1)
            close(*stdin_backup);
        if (*stdout_backup != -1)
            close(*stdout_backup);
        return (-1);
    }
    return (0);
}

// Restores stdio file descriptors
void restore_stdio(int stdin_backup, int stdout_backup)
{
    if (stdin_backup != -1)
    {
        dup2(stdin_backup, STDIN_FILENO);
        close(stdin_backup);
    }
    if (stdout_backup != -1)
    {
        dup2(stdout_backup, STDOUT_FILENO);
        close(stdout_backup);
    }
}

// Handles entire redirection process and returns cleaned args
char **handle_redirections(char **args, char ***env)
{
    int stdin_backup = -1;
    int stdout_backup = -1;
    char **cleaned_args;

    if (!args)
        return (NULL);
    
    if (backup_stdio(&stdin_backup, &stdout_backup) == -1)
        return (NULL);
    
    if (process_redirections(args) == -1)
    {
        restore_stdio(stdin_backup, stdout_backup);
        return (NULL);
    }
    
    cleaned_args = clean_args(args);
    if (!cleaned_args)
    {
        restore_stdio(stdin_backup, stdout_backup);
        return (NULL);
    }
    
    // Execute command if there are valid arguments
    if (cleaned_args[0] && cleaned_args[0][0] != '\0')
    {
        if (is_builtin(cleaned_args[0]))
            execute_builtin(cleaned_args, env);
        else if (contains_pipe(cleaned_args))
            find_args(cleaned_args);
        else
            execute_external(cleaned_args, *env);
    }
    
    restore_stdio(stdin_backup, stdout_backup);
    return (cleaned_args);
}