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
char *expand_vars(const char *token, char **env, t_state quote_state);

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

void find_args(char **args, char **envp)
{
    int num_cmds = 1, i = 0, cmd_index = 0;
    // Count the number of commands (pipe segments).
    while (args[i] != NULL)
    {
        if (strcmp(args[i], "|") == 0)
            num_cmds++;
        i++;
    }
    
    // Allocate an array to hold pointers to each command array.
    char ***commands = malloc((num_cmds + 1) * sizeof(char **));
    if (!commands)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    
    i = 0;
    while (args[i] != NULL)
    {
        // Count tokens until a pipe or end of input.
        int token_count = 0, j = i;
        while (args[j] != NULL && strcmp(args[j], "|") != 0)
        {
            token_count++;
            j++;
        }
        
        // Allocate array for this command (plus one for NULL terminator)
        char **cmd = malloc((token_count + 1) * sizeof(char *));
        if (!cmd)
        {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        
        int token_index = 0;
        while (args[i] != NULL && strcmp(args[i], "|") != 0)
        {
            // Remove surrounding quotes from the token.
            cmd[token_index] = ft_strtrim(args[i], "\"");
            token_index++;
            i++;
        }
        cmd[token_index] = NULL;
        commands[cmd_index++] = cmd;
        
        // Skip over the pipe token, if any.
        if (args[i] != NULL && strcmp(args[i], "|") == 0)
            i++;
    }
    commands[cmd_index] = NULL;
    
    // Now execute the complete pipeline.
    execute_pipeline(commands, num_cmds, envp);
    
    // Free allocated memory after pipe execution.
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
    int in_fd = STDIN_FILENO; // initial input: standard input
    int pipefd[2];
    pid_t pid;
    int status, last_status = 0;
    
    for (i = 0; i < num_cmds; i++)
    {
        // Create a pipe for all but the last command.
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
        else if (pid == 0)  /* Child process */
        {
            // If we have an input descriptor from the previous command, connect it.
            if (in_fd != STDIN_FILENO)
            {
                if (dup2(in_fd, STDIN_FILENO) == -1)
                {
                    perror("dup2 in_fd");
                    exit(EXIT_FAILURE);
                }
                close(in_fd);
            }
            // If not the last command, setup standard output to the pipe.
            if (i < num_cmds - 1)
            {
                close(pipefd[0]); // close read end
                if (dup2(pipefd[1], STDOUT_FILENO) == -1)
                {
                    perror("dup2 pipefd[1]");
                    exit(EXIT_FAILURE);
                }
                close(pipefd[1]);
            }
            
            // Process tokens in the current command:
            {
                int j = 0;
                while (commands[i][j] != NULL)
                {
                    char *tmp = remove_quotes(commands[i][j]);
                    free(commands[i][j]);
                    commands[i][j] = tmp;
                    
                    tmp = expand_vars(commands[i][j], envp, STATE_GENERAL);
                    free(commands[i][j]);
                    commands[i][j] = tmp;
                    
                    j++;
                }
            }
            // Clean up empty tokens.
            commands[i] = clean_args(commands[i]);
            if (!commands[i] || !commands[i][0] || commands[i][0][0] == '\0')
                exit(0);
            
            // If the command is a built-in, execute it.
            if (is_builtin(commands[i][0]))
            {
                exit(execute_builtin(commands[i], &envp));
            }
            else
            {
                // If the command contains a '/', check whether it is accessible.
                if (strchr(commands[i][0], '/'))
                {
                    struct stat st;
                    if (stat(commands[i][0], &st) == 0)
                    {
                        if (S_ISDIR(st.st_mode))
                        {
                            fprintf(stderr, "%s: Is a directory\n", commands[i][0]);
                            exit(PERMISSION_DENIED);
                        }
                        if (access(commands[i][0], X_OK) != 0)
                        {
                            fprintf(stderr, "%s: Permission denied\n", commands[i][0]);
                            exit(PERMISSION_DENIED);
                        }
                    }
                }
                // Execute external command.
                execvp(commands[i][0], commands[i]);
                // If execvp fails, determine an appropriate error message.
                if (strchr(commands[i][0], '/'))
                {
                    fprintf(stderr, "%s: No such file or directory\n", commands[i][0]);
                    exit(CMD_NOT_FOUND);
                }
                else
                {
                    fprintf(stderr, "%s: command not found\n", commands[i][0]);
                    exit(CMD_NOT_FOUND);
                }
            }
        }
        else  /* Parent process */
        {
            // For non-last commands, update in_fd for the next child.
            if (i < num_cmds - 1)
            {
                close(pipefd[1]); // Close write end in parent.
                if (in_fd != STDIN_FILENO)
                    close(in_fd);
                in_fd = pipefd[0];
            }
        }
    }
    
    // Wait for all child processes.
    for (i = 0; i < num_cmds; i++)
    {
        wait(&status);
        if (WIFEXITED(status))
            last_status = WEXITSTATUS(status);
    }
    
    exit(last_status);
}
