/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#           */
/*   Created: 2025/04/23 22:28:36 by grohr             #::
    #             */
/*   Updated: 2025/05/27 22:00:00 by grohr            ###   #fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int g_last_exit_status = 0;

void free_env(char **env)
{
    int i = 0;

    if (!env)
        return;
    while (env[i])
    {
        free(env[i]);
        i++;
    }
    free(env);
}

void free_tab(char **tab)
{
    int i;

    if (!tab)
        return;
    i = 0;
    while (tab[i])
    {
        free(tab[i]);
        i++;
    }
    free(tab);
}

void print_tab(char **tab)
{
    int i;

    i = 0;
    while (tab && tab[i])
    {
        printf("%stoken[%s%d%s] :%s %s->%s  %s  %s<-%s\n", CYAN, RST, i, CYAN, RST, RED, RST, tab[i], RED, RST);
        i++;
    }
    printf("\n");
}

void execute_command(char **args, char ***env)
{
    char **cleaned_args;

    if (!args || !args[0])
        return;

    // Handle redirections
    int stdin_backup = -1, stdout_backup = -1;
    if (backup_stdio(&stdin_backup, &stdout_backup) == -1)
        return;

    if (process_redirections(args) == -1)
    {
        restore_stdio(stdin_backup, stdout_backup);
        return;
    }

    cleaned_args = clean_args(args);
    if (!cleaned_args || !cleaned_args[0])
    {
        restore_stdio(stdin_backup, stdout_backup);
        free_tab(cleaned_args);
        return;
    }

    if (contains_pipe(cleaned_args))
        find_args(cleaned_args, *env);
    else if (is_builtin(cleaned_args[0]))
        g_last_exit_status = execute_builtin(cleaned_args, env);
    else
        g_last_exit_status = execute_external(cleaned_args, *env);

    restore_stdio(stdin_backup, stdout_backup);
    free_tab(cleaned_args);
}

int main(int argc, char **argv, char **envp)
{
    char *user_input;
    char **args;
    char **env;

    (void)argc;
    (void)argv;
    env = dup_env(envp);
    if (!env)
        return (1);

    using_history();
    user_input = NULL;
    init_signals();
    while (1)
    {
        user_input = readline("minishell> ");
        if (user_input == NULL) // Ctrl+D
        {
            printf("exit\n");
            break;
        }
        if (user_input[0] == '\0')
        {
            free(user_input);
            continue;
        }
        add_history(user_input);

        args = tokenize_input(user_input, 0, 0);
        free(user_input);
        if (!args)
            continue;

        // Handle variable assignments
        if (ft_strchr(args[0], '=') && ft_strchr(args[0], '=') != args[0])
        {
            char *tmp[3];
            tmp[0] = "export";
            tmp[1] = args[0];
            tmp[2] = NULL;
            builtin_export(tmp, &env);
            free_tab(args);
            continue;
        }

        execute_command(args, &env);
        free_tab(args);
    }

    free_env(env);
    clear_history();
    return (g_last_exit_status);
}