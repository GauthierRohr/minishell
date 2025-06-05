/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 22:29:03 by grohr             #+#    #+#             */
/*   Updated: 2025/06/05 19:58:56 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Exécution externe
//
// Fork + execve : pour lancer des binaires.
// Gérer les erreurs de path, permissions, etc.

#include "../../inc/minishell.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Processes each argument by removing quotes and expanding variables */
static void process_arguments(char ***pargs, char **envp)
{
    char **args;
    int i;
    char *tmp;
    
    args = *pargs;
    i = 0;
    while (args[i] != NULL)
    {
        tmp = remove_quotes(args[i]);
        free(args[i]);
        args[i] = tmp;
        tmp = expand_vars(args[i], envp);
        free(args[i]);
        args[i] = tmp;
        i = i + 1;
    }
    *pargs = clean_args(args);
}

/* Checks file permissions and if the path is a directory */
void check_path_and_permissions(const char *path)
{
    int stat_ret;
    struct stat st;
    
    stat_ret = stat(path, &st);
    if (stat_ret == 0)
    {
        if (S_ISDIR(st.st_mode))
        {
            fprintf(stderr, "%s: is a directory\n", path);
            exit(PERMISSION_DENIED);
        }
        if (access(path, X_OK) != 0)
        {
            fprintf(stderr, "%s: Permission denied\n", path);
            exit(PERMISSION_DENIED);
        }
    }
}

/* Executes the external command in the child process */
void exec_child(char **args, char **envp)
{
    char *cmd_path;
    
    if (strchr(args[0], '/') != NULL)
        check_path_and_permissions(args[0]);
    cmd_path = get_command_path(args, envp);
    if (cmd_path == NULL)
    {
        fprintf(stderr, "%s: command not found\n", args[0]);
        exit(CMD_NOT_FOUND);
    }
    execve(cmd_path, args, envp);
    perror(args[0]);
    free(cmd_path);
    exit(CMD_NOT_FOUND);
}

/* Executes an external command with argument processing */
int execute_external(char **args, char **envp)
{
    pid_t pid;
    int status;
    
    process_arguments(&args, envp);
    if (args == NULL || args[0] == NULL || args[0][0] == '\0')
        return 0;
    pid = fork();
    if (pid < 0)
    {
        perror("fork");
        return 1;
    }
    if (pid == 0)
    {
        exec_child(args, envp);
        return CMD_NOT_FOUND;
    }
    else
    {
        waitpid(pid, &status, 0);
        g_last_exit_status = WEXITSTATUS(status);
        return WEXITSTATUS(status);
    }
}
