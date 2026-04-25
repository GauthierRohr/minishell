#include "../inc/minishell.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>

char *strip_quotes(char *str)
{
    size_t len;
    char *new_str;
    size_t i, j;

    if (!str)
        return (NULL);
    len = strlen(str);
    if (len == 0)
        return (strdup(""));

    new_str = malloc(len + 1);
    if (!new_str)
        return (NULL);

    i = 0;
    j = 0;
    while (i < len)
    {
        if (str[i] == '"' || str[i] == '\'')
        {
            i++;
            continue;
        }
        new_str[j++] = str[i++];
    }
    new_str[j] = '\0';
    return (new_str);
}

int handle_input_redirect(char *file)
{
    char *clean_file;
    int fd;

    if (!file)
    {
        fprintf(stderr, "minishell: input redirect: No such file or directory\n");
        g_last_exit_status = 1;
        return (-1);
    }

    clean_file = strip_quotes(file);
    if (!clean_file || !clean_file[0])
    {
        free(clean_file);
        fprintf(stderr, "minishell: input redirect: No such file or directory\n");
        g_last_exit_status = 1;
        return (-1);
    }

    fd = open(clean_file, O_RDONLY);
    if (fd == -1)
    {
        fprintf(stderr, "minishell: %s: No such file or directory\n", clean_file);
        free(clean_file);
        g_last_exit_status = 1;
        return (-1);
    }

    free(clean_file);
    if (dup2(fd, STDIN_FILENO) == -1)
    {
        perror("minishell: dup2 input");
        close(fd);
        g_last_exit_status = 1;
        return (-1);
    }
    close(fd);
    return (0);
}

int handle_output_redirect(char *file)
{
    char *clean_file;
    int fd;

    if (!file)
    {
        fprintf(stderr, "minishell: output redirect: No such file or directory\n");
        g_last_exit_status = 1;
        return (-1);
    }

    clean_file = strip_quotes(file);
    if (!clean_file || !clean_file[0])
    {
        free(clean_file);
        fprintf(stderr, "minishell: output redirect: No such file or directory\n");
        g_last_exit_status = 1;
        return (-1);
    }

    fd = open(clean_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    free(clean_file);
    if (fd == -1)
    {
        perror("minishell: output redirect");
        g_last_exit_status = 1;
        return (-1);
    }
    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        perror("minishell: dup2 output");
        close(fd);
        g_last_exit_status = 1;
        return (-1);
    }
    close(fd);
    return (0);
}

int handle_append_redirect(char *file)
{
    char *clean_file;
    int fd;

    if (!file)
    {
        fprintf(stderr, "minishell: append redirect: No such file or directory\n");
        g_last_exit_status = 1;
        return (-1);
    }

    clean_file = strip_quotes(file);
    if (!clean_file || !clean_file[0])
    {
        free(clean_file);
        fprintf(stderr, "minishell: append redirect: No such file or directory\n");
        g_last_exit_status = 1;
        return (-1);
    }

    fd = open(clean_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
    free(clean_file);
    if (fd == -1)
    {
        perror("minishell: append redirect");
        g_last_exit_status = 1;
        return (-1);
    }
    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        perror("minishell: dup2 append");
        close(fd);
        g_last_exit_status = 1;
        return (-1);
    }
    close(fd);
    return (0);
}

int handle_heredoc(char *delimiter)
{
    int pipe_fd[2];
    char *line;

    if (!delimiter)
    {
        fprintf(stderr, "minishell: heredoc: delimiter required\n");
        g_last_exit_status = 1;
        return (-1);
    }

    if (pipe(pipe_fd) == -1)
    {
        perror("minishell: heredoc pipe");
        g_last_exit_status = 1;
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
        g_last_exit_status = 1;
        return (-1);
    }
    close(pipe_fd[0]);
    return (0);
}

int process_redirections(char **args)
{
    int i = 0;
    int j;

    if (!args)
        return (-1);

    while (args[i])
    {
        // Check for redirection operators and ensure the next argument exists
        if (strcmp(args[i], "<") == 0)
        {
            if (!args[i + 1])
            {
                fprintf(stderr, "minishell: syntax error near unexpected token `newline'\n");
                g_last_exit_status = 2;
                return (-1);
            }
            if (handle_input_redirect(args[i + 1]) == -1)
                return (-1);
            free(args[i]);
            free(args[i + 1]);
            args[i] = NULL;
            args[i + 1] = NULL;
            j = i;
            while (args[j + 2])
            {
                args[j] = args[j + 2];
                args[j + 2] = NULL;
                j++;
            }
            args[j] = NULL;
            continue;
        }
        else if (strcmp(args[i], ">") == 0)
        {
            if (!args[i + 1])
            {
                fprintf(stderr, "minishell: syntax error near unexpected token `newline'\n");
                g_last_exit_status = 2;
                return (-1);
            }
            if (handle_output_redirect(args[i + 1]) == -1)
                return (-1);
            free(args[i]);
            free(args[i + 1]);
            args[i] = NULL;
            args[i + 1] = NULL;
            j = i;
            while (args[j + 2])
            {
                args[j] = args[j + 2];
                args[j + 2] = NULL;
                j++;
            }
            args[j] = NULL;
            continue;
        }
        else if (strcmp(args[i], ">>") == 0)
        {
            if (!args[i + 1])
            {
                fprintf(stderr, "minishell: syntax error near unexpected token `newline'\n");
                g_last_exit_status = 2;
                return (-1);
            }
            if (handle_append_redirect(args[i + 1]) == -1)
                return (-1);
            free(args[i]);
            free(args[i + 1]);
            args[i] = NULL;
            args[i + 1] = NULL;
            j = i;
            while (args[j + 2])
            {
                args[j] = args[j + 2];
                args[j + 2] = NULL;
                j++;
            }
            args[j] = NULL;
            continue;
        }
        else if (strcmp(args[i], "<<") == 0)
        {
            if (!args[i + 1])
            {
                fprintf(stderr, "minishell: syntax error near unexpected token `newline'\n");
                g_last_exit_status = 2;
                return (-1);
            }
            if (handle_heredoc(args[i + 1]) == -1)
                return (-1);
            free(args[i]);
            free(args[i + 1]);
            args[i] = NULL;
            args[i + 1] = NULL;
            j = i;
            while (args[j + 2])
            {
                args[j] = args[j + 2];
                args[j + 2] = NULL;
                j++;
            }
            args[j] = NULL;
            continue;
        }
        i++;
    }
    return (0);
}