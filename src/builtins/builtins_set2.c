/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_set2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#           */
/*   Created: 2025/04/23 22:28:19 by grohr             #+#    #+#             */
/*   Updated: 2025/05/20 18:05:00 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// Recherche une variable d'environnement
int find_env_var(char **env, const char *var)
{
    int i;
    size_t len;

    if (!env || !var)
        return (-1);
    len = ft_strlen(var);
    i = 0;
    while (env[i])
    {
        if (ft_strncmp(env[i], var, len) == 0 && env[i][len] == '=')
            return (i);
        i++;
    }
    return (-1);
}

// Vérifie si une variable est définie par l'utilisateur
static int is_user_defined_var(const char *var)
{
    const char *system_vars[] =
    {
        "PATH", "HOME", "USER", "SHELL", "PWD",
        "OLDPWD", "SHLVL", "_", "TERM", "LANG",
        "MallocNanoZone", "SECURITYSESSIONID", "COMMAND_MODE",
        "__CFBundleIdentifier", "LaunchInstanceID", "__CF_USER_TEXT_ENCODING",
        "XPC_SERVICE_NAME", "SSH_AUTH_SOCK", "XPC_FLAGS", "LOGNAME",
        "TMPDIR", "ORIGINAL_XDG_CURRENT_DESKTOP", "HOMEBREW_PREFIX",
        "HOMEBREW_CELLAR", "HOMEBREW_REPOSITORY", "INFOPATH",
        "TERM_PROGRAM", "TERM_PROGRAM_VERSION", "COLORTERM",
        "GIT_ASKPASS", "VSCODE_GIT_ASKPASS_NODE", "VSCODE_GIT_ASKPASS_EXTRA_ARGS",
        "VSCODE_GIT_ASKPASS_MAIN", "VSCODE_GIT_IPC_HANDLE", "VSCODE_INJECTION",
        "ZDOTDIR", "USER_ZDOTDIR", "VSCODE_PROFILE_INITIALIZED",
        NULL
    };

    char *name;
    int i = 0;

    name = ft_substr(var, 0, ft_strchr(var, '=') - var);
    if (!name)
        return (0);

    while (system_vars[i])
    {
        if (ft_strcmp(name, system_vars[i]) == 0)
        {
            free(name);
            return (0); // C'est une variable système
        }
        i++;
    }
    free(name);
    return (1); // C'est une variable utilisateur
}

int builtin_unset(char **args, char ***env)
{
    int i;
    int env_index;

    if (!args[1])
    {
        i = 0;
        while ((*env)[i])
        {
            if (is_user_defined_var((*env)[i]))
            {
                free((*env)[i]);
                env_index = i;
                while ((*env)[env_index + 1])
                {
                    (*env)[env_index] = (*env)[env_index + 1];
                    env_index++;
                }
                (*env)[env_index] = NULL;
            }
            else
            {
                i++;
            }
        }  
        g_last_exit_status = 0;
        return (0);
    }

    i = 1;
    while (args[i])
    {
        env_index = find_env_var(*env, args[i]);
        if (env_index != -1 && is_user_defined_var((*env)[env_index]))
        {
            free((*env)[env_index]);
            while ((*env)[env_index + 1])
            {
                (*env)[env_index] = (*env)[env_index + 1];
                env_index++;
            }
            (*env)[env_index] = NULL;
        }
        i++;
    }
    g_last_exit_status = 0;
    return (0);
}

int builtin_env(char **env)
{
    int i;
    
    i = 0;
    while (env[i])
    {
        printf("%s\n", env[i]);
        i++;
    }
    g_last_exit_status = 0;
    return (0);
}

long ft_strtol(const char *str, char **endptr, int base)
{
    long result = 0;
    int sign = 1;
    int i = 0;

    while (str[i] == ' ' || (str[i] >= '\t' && str[i] <= '\r'))
        i++;

    if (str[i] == '+' || str[i] == '-')
    {
        if (str[i] == '-')
            sign = -1;
        i++;
    }

    if (base != 10 || !ft_isdigit(str[i]))
    {
        if (endptr)
            *endptr = (char *)str + i;
        return (0);
    }

    while (ft_isdigit(str[i]))
    {
        if (result > (LONG_MAX - (str[i] - '0')) / 10)
        {
            if (endptr)
                *endptr = (char *)str + i;
            return (sign == 1) ? LONG_MAX : LONG_MIN;
        }
        result = result * 10 + (str[i] - '0');
        i++;
    }

    if (endptr)
        *endptr = (char *)str + i;
    return (result * sign);
}

int builtin_exit(char **args, char ***env)
{
    long code;
    char *endptr;
    char *arg;

    (void)env;
    ft_putstr_fd("exit\n", 1);
    if (!args[1])
        exit(g_last_exit_status);
    if (args[2])
    {
        ft_putstr_fd("exit: too many arguments\n", 2);
        g_last_exit_status = 1;
        return (1);
    }

    arg = remove_quotes(args[1]);
    if (!arg)
        arg = ft_strdup(args[1]);

    while (*arg == ' ' || (*arg >= '\t' && *arg <= '\r'))
        arg++;

    if (arg[0] == '\0')
    {
        ft_putstr_fd("exit: ", 2);
        ft_putstr_fd(args[1], 2);
        ft_putstr_fd(": numeric argument required\n", 2);
        free(arg);
        exit(255); // Changed from 2 to 255 to match bash
    }

    if (arg[0] == '-' && !ft_isdigit(arg[1]))
    {
        ft_putstr_fd("exit: ", 2);
        ft_putstr_fd(args[1], 2);
        ft_putstr_fd(": numeric argument required\n", 2);
        free(arg);
        exit(255); // Changed from 2 to 255 to match bash
    }

    code = ft_strtol(arg, &endptr, 10);
    while (*endptr == ' ' || (*endptr >= '\t' && *endptr <= '\r'))
        endptr++;
    if (*endptr != '\0')
    {
        ft_putstr_fd("exit: ", 2);
        ft_putstr_fd(args[1], 2);
        ft_putstr_fd(": numeric argument required\n", 2);
        free(arg);
        exit(2); // Changed from 2 to 255 to match bash
    }

    free(arg);
    code = code % 256;
    if (code < 0)
        code += 256;

    g_last_exit_status = (int)code;
    exit(g_last_exit_status);
}