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

void execute_child(char **command, char **envp, int in_fd, int pipefd[2])
{
    setup_child_io(in_fd, pipefd);
    int redirect_status = process_redirections(command);
    char **args = clean_args(command);
    if (args == NULL || args[0] == NULL)
    {
        free_tab(args);
        exit((redirect_status == -1) ? 1 : 0);
    }
    if (is_builtin(args[0]))
    {
        int ret = execute_builtin(args, &envp);
        free_tab(args);
        exit(ret);
    }
    execute_external(args, envp);
}

void run_pipeline_loop(char ***commands, int num_cmds, char **envp, pid_t *pids)
{
    int i = 0, in_fd = STDIN_FILENO, pipefd[2];
    while (i < num_cmds)
    {
        if (i < num_cmds - 1)
            if (pipe(pipefd) == -1) {
                cleanup_pipeline(pids);
                return;
            }
        pid_t pid = fork();
        if (pid < 0)    { 
            cleanup_pipeline(pids);
            return;
        }
        if (pid == 0)
            if (i < num_cmds - 1)
                execute_child(commands[i], envp, in_fd, pipefd);
            else
                execute_child(commands[i], envp, in_fd, NULL);
        pids[i] = pid;
        if (i < num_cmds - 1)
        {
            close(pipefd[1]);
            if (in_fd != STDIN_FILENO)
                close(in_fd);
            in_fd = pipefd[0];
        }
        i = i + 1;
    }
}

void execute_pipeline(char ***commands, int num_cmds, char **envp)
{
    pid_t *pids = malloc(num_cmds * sizeof(pid_t));
    int stdin_b = dup(STDIN_FILENO), stdout_b = dup(STDOUT_FILENO);
    int status;
    if (pids == NULL)
    {
        perror("minishell: pipeline setup");
        g_last_exit_status = 1;
        return;
    }
    if (stdin_b == -1 || stdout_b == -1)
    {
        perror("minishell: pipeline setup");
        free(pids);
        g_last_exit_status = 1;
        return;
    }
    run_pipeline_loop(commands, num_cmds, envp, pids);
    if (STDIN_FILENO != stdin_b)
        close(STDIN_FILENO);
    restore_stdio(stdin_b, stdout_b);
    wait_children(pids, num_cmds, &status);
    free(pids);
    return;
}