/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piping.c                                           :+:      :+:    :+:   */
/*                                                    +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#           */
/*   Created: 2025/05/13 17:28:43 by grohr             #::
    #             */
/*   Updated: 2025/05/27 22:00:00 by grohr            ###   #fr       */
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
        return (0);
    while (args[i] != NULL)
    {
        if (strcmp(args[i], "|") == 0)
            return (1);
        i++;
    }
    return (0);
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
        perror("minishell: malloc");
        g_last_exit_status = 1;
        return;
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
            perror("minishell: malloc");
            g_last_exit_status = 1;
            free(commands);
            return;
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
        free_tab(commands[i]);
    free(commands);
}

void execute_pipeline(char ***commands, int num_cmds, char **envp)
{
    int i;
    int in_fd = STDIN_FILENO;
    int pipefd[2];
    pid_t *pids = malloc(num_cmds * sizeof(pid_t));
    int status;
    int stdin_backup = dup(STDIN_FILENO);
    int stdout_backup = dup(STDOUT_FILENO);

    if (!pids || stdin_backup == -1 || stdout_backup == -1)
    {
        perror("minishell: pipeline setup");
        free(pids);
        if (stdin_backup != -1)
            close(stdin_backup);
        if (stdout_backup != -1)
            close(stdout_backup);
        g_last_exit_status = 1;
        return;
    }

    for (i = 0; i < num_cmds; i++)
    {
        if (i < num_cmds - 1)
        {
            if (pipe(pipefd) == -1)
            {
                perror("minishell: pipe");
                free(pids);
                close(stdin_backup);
                close(stdout_backup);
                g_last_exit_status = 1;
                return;
            }
        }

        pids[i] = fork();
        if (pids[i] < 0)
        {
            perror("minishell: fork");
            free(pids);
            close(stdin_backup);
            close(stdout_backup);
            g_last_exit_status = 1;
            return;
        }
        else if (pids[i] == 0)
        {
            // Set up input
            if (in_fd != STDIN_FILENO)
            {
                dup2(in_fd, STDIN_FILENO);
                close(in_fd);
            }
            // Set up output for pipes
            if (i < num_cmds - 1)
            {
                close(pipefd[0]);
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[1]);
            }

            // Handle redirections
            int redirect_status = process_redirections(commands[i]);
            char **cleaned_args = clean_args(commands[i]);

            if (!cleaned_args || !cleaned_args[0])
            {
                free_tab(cleaned_args);
                exit(redirect_status == -1 ? 1 : 0);
            }

            // Execute command
            if (is_builtin(cleaned_args[0]))
            {
                int ret = execute_builtin(cleaned_args, &envp);
                free_tab(cleaned_args);
                exit(ret);
            }
            else
            {
                char *cmd_path = NULL;
                if (strchr(cleaned_args[0], '/'))
                    cmd_path = ft_strdup(cleaned_args[0]);
                else
                    cmd_path = ft_get_cmd_path(cleaned_args[0], envp);
                if (!cmd_path)
                {
                    fprintf(stderr, "%s: command not found\n", cleaned_args[0]);
                    free_tab(cleaned_args);
                    exit(127);
                }
                execve(cmd_path, cleaned_args, envp);
                perror(cleaned_args[0]);
                free(cmd_path);
                free_tab(cleaned_args);
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

    // Close remaining pipe fds in parent
    if (in_fd != STDIN_FILENO)
        close(in_fd);

    // Restore parent's stdio
    dup2(stdin_backup, STDIN_FILENO);
    dup2(stdout_backup, STDOUT_FILENO);
    close(stdin_backup);
    close(stdout_backup);

    // Wait for all child processes
    for (i = 0; i < num_cmds; i++)
    {
        waitpid(pids[i], &status, 0);
        if (WIFEXITED(status) && i == num_cmds - 1)
            g_last_exit_status = WEXITSTATUS(status);
    }

    free(pids);
}