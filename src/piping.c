/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piping.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                               +J      +#+           */
/*   Created: 2025/05/13 17:28:43 by grohr             #+#    #+#             */
/*   Updated: 2025/05/19 19:30:00 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int contains_pipe(char **args)
{
    int i;

    i = 0;
    if (!args)
        return 0;
    while (args[i] != NULL)
    {
        if (strcmp(args[i], "|") == 0)
            return 1;
        i++;
    }
    return 0;
}

void find_args(char **args, char **envp)
{
    int num_cmds = 1, i = 0, cmd_index = 0;
    while (args[i] != NULL)
    {
        if (strcmp(args[i], "|") == 0)
            num_cmds++;
        i++;
    }
    
    char ***commands = malloc((num_cmds + 1) * sizeof(char **));
    if (!commands)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    
    i = 0;
    while (args[i] != NULL)
    {
        int token_count = 0, j = i;
        while (args[j] != NULL && strcmp(args[j], "|") != 0)
        {
            token_count++;
            j++;
        }
        
        char **cmd = malloc((token_count + 1) * sizeof(char *));
        if (!cmd)
        {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        
        int token_index = 0;
        while (args[i] != NULL && strcmp(args[i], "|") != 0)
        {
            cmd[token_index] = ft_strdup(args[i]);
            token_index++;
            i++;
        }
        cmd[token_index] = NULL;
        commands[cmd_index++] = cmd;
        
        if (args[i] != NULL && strcmp(args[i], "|") == 0)
            i++;
    }
    commands[cmd_index] = NULL;
    
    execute_pipeline(commands, num_cmds, envp);
    
    for (i = 0; i < num_cmds; i++)
    {
        int k = 0;
        while (commands[i][k] != NULL)
        {
            free(commands[i][k]);
            k++;
        }
        free(commands[i]);
    }
    free(commands);
}

void execute_pipeline(char ***commands, int num_cmds, char **envp)
{
    int i;
    int in_fd = STDIN_FILENO;
    int pipefd[2];
    pid_t pid;
    int status, last_status = 0;
    int stdin_backup = dup(STDIN_FILENO);
    int stdout_backup = dup(STDOUT_FILENO);

    for (i = 0; i < num_cmds; i++)
    {
        if (i < num_cmds - 1)
        {
            if (pipe(pipefd) == -1)
            {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
        }

        pid = fork();
        if (pid < 0)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            // Gestion des redirections pour chaque commande
            if (process_redirections(commands[i]) == -1)
                exit(1);

            if (in_fd != STDIN_FILENO)
                dup2(in_fd, STDIN_FILENO);
            if (i < num_cmds - 1)
            {
                close(pipefd[0]);
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[1]);
            }

            if (is_builtin(commands[i][0]))
                exit(execute_builtin(commands[i], &envp));
            else
            {
                execvp(commands[i][0], commands[i]);
                fprintf(stderr, "%s: command not found\n", commands[i][0]);
                exit(127);
            }
        }
        else
        {
            if (i < num_cmds - 1)
            {
                close(pipefd[1]);
                if (in_fd != STDIN_FILENO)
                    close(in_fd);
                in_fd = pipefd[0];
            }
        }
    }

    // Restaurer les descripteurs standards dans le parent
    dup2(stdin_backup, STDIN_FILENO);
    dup2(stdout_backup, STDOUT_FILENO);
    close(stdin_backup);
    close(stdout_backup);

    for (i = 0; i < num_cmds; i++)
    {
        wait(&status);
        if (WIFEXITED(status))
            last_status = WEXITSTATUS(status);
    }

    g_last_exit_status = last_status;
}