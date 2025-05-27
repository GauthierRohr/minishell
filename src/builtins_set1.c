/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_set1.c                                    :+:      :+:    :+:   */
/*                                                    +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:
    #           */
/*                                                +#           */
/*   Created: 2025/04/23 22:28:08 by grohr             #::
    #             */
/*   Updated: 2025/05/27 22:00:00 by grohr            ###   #fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include <string.h>

char *get_env_value(const char *name, char **env)
{
    int i;
    size_t len;

    if (!name || !env)
        return (NULL);
    len = ft_strlen(name);
    i = 0;
    while (env[i])
    {
        if (ft_strncmp(env[i], name, len) == 0 && env[i][len] == '=')
            return (env[i] + len + 1);
        i++;
    }
    return (NULL);
}

char *expand_vars(const char *token, char **env)
{
    char *expanded;
    char var_name[256];
    const char *ptr = token;
    int i = 0, j = 0;
    char *value;
    char *exit_status_str;

    expanded = malloc(4096);
    if (!expanded)
        return (NULL);

    while (*ptr)
    {
        if (*ptr == '$' && *(ptr + 1))
        {
            ptr++;
            if (*ptr == '?')
            {
                exit_status_str = ft_itoa(g_last_exit_status);
                if (!exit_status_str)
                {
                    free(expanded);
                    return (NULL);
                }
                ft_strlcpy(expanded + j, exit_status_str, 4096 - j);
                j += ft_strlen(exit_status_str);
                free(exit_status_str);
                ptr++;
                continue;
            }
            else if (ft_isalpha(*ptr) || *ptr == '_')
            {
                i = 0;
                while (*ptr && (ft_isalnum(*ptr) || *ptr == '_'))
                    var_name[i++] = *ptr++;
                var_name[i] = '\0';
                value = get_env_value(var_name, env);
                if (value)
                {
                    ft_strlcpy(expanded + j, value, 4096 - j);
                    j += ft_strlen(value);
                }
            }
            else
            {
                expanded[j++] = '$';
                if (*ptr)
                    expanded[j++] = *ptr++;
            }
        }
        else
            expanded[j++] = *ptr++;
    }
    expanded[j] = '\0';
    return (expanded);
}

int builtin_echo(char **args, char ***env)
{
    int i = 1;
    int n_option = 0;
    char *clean_arg;
    char *tmp;
    int first_non_empty = 1;

    if (args[1] && ft_strcmp(args[1], "-n") == 0)
    {
        n_option = 1;
        i = 2;
    }

    while (args[i])
    {
        clean_arg = NULL;
        if (args[i][0] == '\'' && args[i][ft_strlen(args[i]) - 1] == '\'')
        {
            tmp = remove_quotes(args[i]);
            clean_arg = tmp ? ft_strdup(tmp) : ft_strdup("");
            free(tmp);
        }
        else
        {
            tmp = remove_partial_quotes(args[i]);
            clean_arg = tmp ? expand_vars(tmp, *env) : ft_strdup("");
            free(tmp);
        }

        if (clean_arg && clean_arg[0] != '\0')
        {
            if (!first_non_empty)
                printf(" ");
            printf("%s", clean_arg);
            first_non_empty = 0;
        }
        free(clean_arg);
        i++;
    }

    if (!n_option)
        printf("\n");

    g_last_exit_status = 0;
    return (0);
}

int builtin_cd(char **args, char ***env)
{
    char *path;
    int ret;

    if (!args[1])
        path = get_env_value("HOME", *env);
    else
    {
        path = expand_vars(args[1], *env);
        if (!path)
            path = ft_strdup(args[1]);
    }
    if (!path)
    {
        ft_putstr_fd("cd: memory error\n", 2);
        g_last_exit_status = 1;
        return (1);
    }
    ret = chdir(path);
    if (ret == -1)
    {
        ft_putstr_fd("cd: ", 2);
        ft_putstr_fd(path, 2);
        ft_putstr_fd(": No such file or directory\n", 2);
        g_last_exit_status = 1;
    }
    else
        g_last_exit_status = 0;
    free(path);
    return (g_last_exit_status);
}

int builtin_pwd(void)
{
    char cwd[1024];

    if (getcwd(cwd, sizeof(cwd)))
    {
        printf("%s\n", cwd);
        g_last_exit_status = 0;
        return (0);
    }
    perror("minishell: pwd");
    g_last_exit_status = 1;
    return (1);
}

int count_array(char **array)
{
    int i;

    i = 0;
    while (array[i])
        i++;
    return (i);
}

static int is_valid_identifier(const char *name)
{
    int i = 0;
    if (!name || !name[0] || (!ft_isalpha(name[0]) && name[0] != '_'))
        return (0);
    while (name[i])
    {
        if (!ft_isalnum(name[i]) && name[i] != '_')
            return (0);
        i++;
    }
    return (1);
}

int builtin_export(char **args, char ***env)
{
    int i;
    int env_index;
    char **new_env;
    int env_size;
    char *equal_sign;
    char *var_name;

    if (!args[1])
    {
        i = 0;
        while ((*env)[i])
        {
            printf("declare -x %s\n", (*env)[i]);
            i++;
        }
        g_last_exit_status = 0;
        return (0);
    }

    i = 1;
    while (args[i])
    {
        equal_sign = ft_strchr(args[i], '=');
        if (equal_sign || !args[i][0])
        {
            var_name = equal_sign ? ft_substr(args[i], 0, equal_sign - args[i]) : ft_strdup(args[i]);
            if (!var_name || !is_valid_identifier(var_name))
            {
                ft_putstr_fd("export: `", 2);
                ft_putstr_fd(args[i], 2);
                ft_putstr_fd("': not a valid identifier\n", 2);
                free(var_name);
                g_last_exit_status = 1;
                i++;
                continue;
            }
            free(var_name);
            if (equal_sign)
                *equal_sign = '\0';
            env_index = find_env_var(*env, args[i]);
            if (equal_sign)
                *equal_sign = '=';

            if (env_index != -1)
            {
                free((*env)[env_index]);
                (*env)[env_index] = ft_strdup(args[i]);
                if (!(*env)[env_index])
                {
                    g_last_exit_status = 1;
                    return (1);
                }
            }
            else
            {
                env_size = count_array(*env);
                new_env = malloc(sizeof(char *) * (env_size + 2));
                if (!new_env)
                {
                    g_last_exit_status = 1;
                    return (1);
                }
                
                ft_memcpy(new_env, *env, sizeof(char *) * env_size);
                new_env[env_size] = ft_strdup(args[i]);
                if (!new_env[env_size])
                {
                    free(new_env);
                    g_last_exit_status = 1;
                    return (1);
                }
                new_env[env_size + 1] = NULL;
                
                free(*env);
                *env = new_env;
            }
        }
        else if (!is_valid_identifier(args[i]))
        {
            ft_putstr_fd("export: `", 2);
            ft_putstr_fd(args[i], 2);
            ft_putstr_fd("': not a valid identifier\n", 2);
            g_last_exit_status = 1;
            i++;
            continue;
        }
        i++;
    }
    return (g_last_exit_status);
}