/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_args.c                                        :+:      :+:    :+:   */
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

char **dup_tokens(char **args, int *i)
{
    int count = 0;
    int j = *i;
    while (args[j] != NULL && strcmp(args[j], "|") != 0)
    {
        count = count + 1;
        j = j + 1;
    }
    char **cmd = malloc((count + 1) * sizeof(char *));
    if (cmd == NULL)
    {
        perror("minishell: malloc");
        return NULL;
    }
    int k = 0;
    while (args[*i] != NULL && strcmp(args[*i], "|") != 0)
    {
        cmd[k] = ft_strdup(args[*i]);
        k = k + 1;
        *i = *i + 1;
    }
    cmd[k] = NULL;
    return cmd;
}

int count_pipes(char **args)
{
    int count = 0;
    int i = 0;
    while (args[i] != NULL)
    {
        if (strcmp(args[i], "|") == 0)
        {
            count = count + 1;
        }
        i = i + 1;
    }
    return count;
}

void free_cmds(char ***cmds, int num_cmds)
{
    int i = 0;
    while (i < num_cmds)
    {
        free_tab(cmds[i]);
        i = i + 1;
    }
    free(cmds);
}

char ***build_commands(char **args, int num_cmds)
{
    char ***cmds = malloc((num_cmds + 1) * sizeof(char **));
    if (cmds == NULL)
    {
        perror("minishell: malloc");
        return NULL;
    }
    int i = 0;
    int ci = 0;
    while (args[i] != NULL)
    {
        char **cmd = dup_tokens(args, &i);
        if (cmd == NULL)
        {
            free(cmds);
            return NULL;
        }
        cmds[ci] = cmd;
        ci = ci + 1;
        if (args[i] != NULL && strcmp(args[i], "|") == 0)
            i = i + 1;
    }
    cmds[ci] = NULL;
    return cmds;
}

void find_args(char **args, char **envp)
{
    int num_cmds = count_pipes(args) + 1;
    char ***cmds = build_commands(args, num_cmds);
    if (cmds == NULL)
    {
        g_last_exit_status = 1;
        return;
    }
    execute_pipeline(cmds, num_cmds, envp);
    free_cmds(cmds, num_cmds);
    return;
}