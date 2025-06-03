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

#include "../../inc/minishell.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void wait_children(pid_t *pids, int num_cmds, int *status)
{
    int i = 0;
    while (i < num_cmds)
    {
        waitpid(pids[i], status, 0);
        if (WIFEXITED(*status) && i == num_cmds - 1)
        {
            g_last_exit_status = WEXITSTATUS(*status);
        }
        i = i + 1;
    }
}

void setup_child_io(int in_fd, int pipefd[2])
{
    if (in_fd != STDIN_FILENO)
    {
        dup2(in_fd, STDIN_FILENO);
        close(in_fd);
    }
    if (pipefd != NULL)
    {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
    }
}

void execute_external(char **args, char **envp)
{
    char *cmd_path = NULL;
    if (strchr(args[0], '/') != NULL)
    {
        cmd_path = ft_strdup(args[0]);
    }
    else
    {
        cmd_path = ft_get_cmd_path(args[0], envp);
    }
    if (cmd_path == NULL)
    {
        fprintf(stderr, "%s: command not found\n", args[0]);
        free_tab(args);
        exit(127);
    }
    execve(cmd_path, args, envp);
    perror(args[0]);
    free(cmd_path);
    free_tab(args);
    exit(127);
}

int prepare_pipe(int i, int num_cmds, int pipefd[2], pid_t *pids)
{
    if (i < num_cmds - 1)
    {
        if (pipe(pipefd) == -1)
        {
            cleanup_pipeline(pids);
            return -1;
        }
    }
    return 0;
}

void handle_parent_pipe(int *in_fd, int pipefd[2])
{
    close(pipefd[1]);
    if (*in_fd != STDIN_FILENO)
        close(*in_fd);
    *in_fd = pipefd[0];
}
