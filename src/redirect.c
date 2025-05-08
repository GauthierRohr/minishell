/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+         +:+     */
/*   By: cjauregu <cjauregu@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 10:00:00 by cjauregu        #+#    #+#             */
/*   Updated: 2025/05/05 12:00:00 by cjauregu       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>

// Handles input redirection '<'
int handle_input_redirect(char *file)
{
    int fd = open(file, O_RDONLY);
    if (fd == -1)
    {
        perror("minishell: input redirect");
        return (-1);
    }
    if (dup2(fd, STDIN_FILENO) == -1)
    {
        perror("minishell: dup2 input");
        close(fd);
        return (-1);
    }
    close(fd);
    return (0);
}

// Handles output redirection '>' (truncate mode)
int handle_output_redirect(char *file)
{
    int fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        perror("minishell: output redirect");
        return (-1);
    }
    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        perror("minishell: dup2 output");
        close(fd);
        return (-1);
    }
    close(fd);
    return (0);
}

// Handles append output redirection '>>'
int handle_append_redirect(char *file)
{
    int fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1)
    {
        perror("minishell: append redirect");
        return (-1);
    }
    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        perror("minishell: dup2 append");
        close(fd);
        return (-1);
    }
    close(fd);
    return (0);
}

// Handles here-document '<<' with delimiter
int handle_heredoc(char *delimiter)
{
    int pipe_fd[2];
    char *line;

    if (pipe(pipe_fd) == -1)
    {
        perror("minishell: heredoc pipe");
        return (-1);
    }
    while (1)
    {
        line = readline("> ");
        if (!line || strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }
        write(pipe_fd[1], line, strlen(line));
        write(pipe_fd[1], "\n", 1);
        free(line);
    }
    close(pipe_fd[1]);
    if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
    {
        perror("minishell: dup2 heredoc");
        close(pipe_fd[0]);
        return (-1);
    }
    close(pipe_fd[0]);
    return (0);
}

// Processes all redirections in the command arguments
int process_redirections(char **args)
{
    int i = 0;
    while (args[i])
    {
        if (strcmp(args[i], "<") == 0 && args[i + 1])
        {
            if (handle_input_redirect(args[i + 1]) == -1)
                return (-1);
            args[i] = NULL; // Mark for removal
            i += 2;
        }
        else if (strcmp(args[i], ">") == 0 && args[i + 1])
        {
            if (handle_output_redirect(args[i + 1]) == -1)
                return (-1);
            args[i] = NULL;
            i += 2;
        }
        else if (strcmp(args[i], ">>") == 0 && args[i + 1])
        {
            if (handle_append_redirect(args[i + 1]) == -1)
                return (-1);
            args[i] = NULL;
            i += 2;
        }
        else if (strcmp(args[i], "<<") == 0 && args[i + 1])
        {
            if (handle_heredoc(args[i + 1]) == -1)
                return (-1);
            args[i] = NULL;
            i += 2;
        }
        else
            i++;
    }
    return (0);
}
