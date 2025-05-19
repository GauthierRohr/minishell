#include "../inc/minishell.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

char **clean_args(char **args)
{
    char **new_args;
    int i, j, count = 0;

    if (!args)
        return (NULL);

    i = 0;
    while (args[i])
    {
        if (args[i] && args[i][0] != '\0')
            count++;
        i++;
    }

    new_args = malloc((count + 1) * sizeof(char *));
    if (!new_args)
        return (NULL);

    i = 0;
    j = 0;
    while (args[i])
    {
        if (args[i] && args[i][0] != '\0')
        {
            new_args[j] = strdup(args[i]);
            if (!new_args[j])
            {
                while (j > 0)
                    free(new_args[--j]);
                free(new_args);
                return (NULL);
            }
            j++;
        }
        i++;
    }
    new_args[j] = NULL;
    return (new_args);
}

int backup_stdio(int *stdin_backup, int *stdout_backup)
{
    *stdin_backup = dup(STDIN_FILENO);
    *stdout_backup = dup(STDOUT_FILENO);
    if (*stdin_backup == -1 || *stdout_backup == -1)
    {
        perror("minishell: stdio backup");
        if (*stdin_backup != -1)
            close(*stdin_backup);
        if (*stdout_backup != -1)
            close(*stdout_backup);
        return (-1);
    }
    return (0);
}

void restore_stdio(int stdin_backup, int stdout_backup)
{
    if (stdin_backup != -1)
    {
        dup2(stdin_backup, STDIN_FILENO);
        close(stdin_backup);
    }
    if (stdout_backup != -1)
    {
        dup2(stdout_backup, STDOUT_FILENO);
        close(stdout_backup);
    }
}

char **handle_redirections(char **args, char ***env)
{
    int stdin_backup = -1;
    int stdout_backup = -1;
    char **cleaned_args;
    int redirect_status;

    if (!args || !args[0])
        return (NULL);

    if (backup_stdio(&stdin_backup, &stdout_backup) == -1)
        return (NULL);

    redirect_status = process_redirections(args);
    cleaned_args = clean_args(args);

    if (redirect_status == -1)
    {
        restore_stdio(stdin_backup, stdout_backup);
        free_tab(cleaned_args);
        return (NULL);
    }

    if (cleaned_args && cleaned_args[0])
    {
        if (contains_pipe(cleaned_args))
            find_args(cleaned_args, *env);
        else if (is_builtin(cleaned_args[0]))
            execute_builtin(cleaned_args, env);
        else
            execute_external(cleaned_args, *env);
    }

    restore_stdio(stdin_backup, stdout_backup);
    return (cleaned_args);
}