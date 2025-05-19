#include "../inc/minishell.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>

// Enlève les quotes autour d'un nom de fichier
char *strip_quotes(char *str)
{
    size_t len;
    char *new_str;
    int i, j;

    if (!str)
        return (NULL);
    len = strlen(str);
    if (len == 0)
        return (strdup(""));

    // Gérer les guillemets simples ou doubles entourant complètement
    if ((str[0] == '"' && str[len - 1] == '"') || (str[0] == '\'' && str[len - 1] == '\''))
    {
        new_str = malloc(len - 1); // len - 2 + 1 pour \0
        if (!new_str)
            return (NULL);
        for (i = 1, j = 0; i < (int)len - 1; i++) // Cast int pour compatibilité
            new_str[j++] = str[i];
        new_str[j] = '\0';
        return (new_str);
    }
    return (strdup(str));
}

int handle_input_redirect(char *file)
{
    char *clean_file = strip_quotes(file);
    int fd;

    if (!clean_file || !clean_file[0])
    {
        free(clean_file);
        fprintf(stderr, "minishell: input redirect: No such file or directory\n");
        g_last_exit_status = 1;
        return (-1);
    }
    fd = open(clean_file, O_RDONLY);
    free(clean_file);
    if (fd == -1)
    {
        perror("minishell: input redirect");
        g_last_exit_status = 1;
        return (-1);
    }
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
    char *clean_file = strip_quotes(file);
    int fd;

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
    char *clean_file = strip_quotes(file);
    int fd;

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
    int redirect_failed = 0;

    while (args[i])
    {
        if (strcmp(args[i], "<") == 0 && args[i + 1])
        {
            if (handle_input_redirect(args[i + 1]) == -1)
                redirect_failed = 1; // Mark failure but continue
            // Déplacer les arguments pour supprimer "<" et le fichier
            free(args[i]);
            free(args[i + 1]);
            args[i] = NULL;
            args[i + 1] = NULL;
            j = i;
            while (args[j + 2])
            {
                args[j] = args[j + 2];
                j++;
            }
            args[j] = NULL;
            continue;
        }
        else if (strcmp(args[i], ">") == 0 && args[i + 1])
        {
            if (handle_output_redirect(args[i + 1]) == -1)
                redirect_failed = 1;
            // Déplacer les arguments pour supprimer ">" et le fichier
            free(args[i]);
            free(args[i + 1]);
            args[i] = NULL;
            args[i + 1] = NULL;
            j = i;
            while (args[j + 2])
            {
                args[j] = args[j + 2];
                j++;
            }
            args[j] = NULL;
            continue;
        }
        else if (strcmp(args[i], ">>") == 0 && args[i + 1])
        {
            if (handle_append_redirect(args[i + 1]) == -1)
                redirect_failed = 1;
            // Déplacer les arguments pour supprimer ">>" et le fichier
            free(args[i]);
            free(args[i + 1]);
            args[i] = NULL;
            args[i + 1] = NULL;
            j = i;
            while (args[j + 2])
            {
                args[j] = args[j + 2];
                j++;
            }
            args[j] = NULL;
            continue;
        }
        else if (strcmp(args[i], "<<") == 0 && args[i + 1])
        {
            if (handle_heredoc(args[i + 1]) == -1)
                redirect_failed = 1;
            // Déplacer les arguments pour supprimer "<<" et le délimiteur
            free(args[i]);
            free(args[i + 1]);
            args[i] = NULL;
            args[i + 1] = NULL;
            j = i;
            while (args[j + 2])
            {
                args[j] = args[j + 2];
                j++;
            }
            args[j] = NULL;
            continue;
        }
        i++;
    }
    return (redirect_failed ? -1 : 0);
}