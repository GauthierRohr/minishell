/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piping.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 17:28:43 by grohr             #+#    #+#             */
/*   Updated: 2025/05/13 17:28:43 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int contains_pipe(char **args)
{
    int i;

    i = 0;
    if (!args)
        return 0;  // Safety check
    while (args[i] != NULL) {
        if (strcmp(args[i], "|") == 0)
            return 1;  // Pipe found
        i++;
    }
    return 0;  // No pipe found
}

void find_args(char **args)
{
    int i;
    int j;
    char **cmd1;
    char **cmd2;

    i = 0;
    j = 0;
    cmd1 = malloc(4 * sizeof(char *));
    cmd2 = malloc(4 * sizeof(char *));
    if (!cmd1 || !cmd2)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    // Process tokens until the pipe symbol is encountered.
    // Instead of checking for args[i][0] != '|', we compare the whole token.
    while (args[i] != NULL && strcmp(args[i], "|") != 0)
    {
        // Trim the double quotes from the argument
        cmd1[i] = ft_strtrim(args[i], "\"");
        i++;
    }
    cmd1[i] = NULL;
    
    // Skip the pipe token.
    if (args[i] != NULL)
        i++;
    
    // Process remaining tokens (after the pipe)
    while (args[i] != NULL && args[i][0] != '\0')
    {
        cmd2[j] = ft_strtrim(args[i], "\"");
        j++;
        i++;
    }
    cmd2[j] = NULL;
    execute_piped(cmd1, cmd2);
    // Free the trimmed strings here.
    for (i = 0; cmd1[i] != NULL; i++)
        free(cmd1[i]);
    for (j = 0; cmd2[j] != NULL; j++)
        free(cmd2[j]);
    free(cmd1);
    free(cmd2);
}



void execute_piped(char **cmd1, char **cmd2)
{
    int pipefd[2];
    pid_t pid1, pid2;

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    // First child process (Writer)
    if ((pid1 = fork()) == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pid1 == 0)
    {  // Child 1
        close(pipefd[0]);   // Close read end of the pipe
        dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to pipe's write end
        close(pipefd[1]);   // Close pipe write end (it's duplicated now)
        execvp(cmd1[0], cmd1);  // Execute first command
        perror("execvp");
        exit(EXIT_FAILURE);
    }
    // Second child process (Reader)
    if ((pid2 = fork()) == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pid2 == 0) {  // Child 2
        close(pipefd[1]);   // Close write end of the pipe
        dup2(pipefd[0], STDIN_FILENO); // Redirect stdin to pipe's read end
        close(pipefd[0]);   // Close pipe read end (it's duplicated now)
        execvp(cmd2[0], cmd2);  // Execute second command
        perror("execvp");
        exit(EXIT_FAILURE);
    }
    // Parent process: Close pipe ends and wait for both children
    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
}