/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_set2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 22:28:19 by grohr             #+#    #+#             */
/*   Updated: 2025/05/13 15:53:37 by grohr            ###   ########.fr       */
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
    // Liste des variables système à conserver
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
    
    // Extraire le nom de la variable (avant le '=')
    name = ft_substr(var, 0, ft_strchr(var, '=') - var);
    if (!name)
        return (0);
    
    // Vérifier si c'est une variable système
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

// Gère la commande unset
int builtin_unset(char **args, char ***env)
{
    int i;
    int env_index;

    if (!args[1])
    {
        // Sans arguments, supprimer toutes les variables utilisateur
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

    // Avec arguments, supprimer les variables spécifiées
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

// Gère la commande env
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

/* Convertit une chaîne en long, gérant les signes + et -. Stocke le pointeur
 * de fin dans endptr. Retourne le résultat ou 0 si erreur non numérique. */
int ft_atoi(const char *str)
{
    int result = 0;
    int sign = 1;
    int i = 0;

    // Skip leading whitespace
    while (str[i] == ' ' || (str[i] >= '\t' && str[i] <= '\r'))
        i++;
    // Handle sign
    if (str[i] == '+' || str[i] == '-')
    {
        if (str[i] == '-')
            sign = -1;
        i++;
    }
    // Convert number
    while (str[i] >= '0' && str[i] <= '9')
    {
        if (result > (INT_MAX - (str[i] - '0')) / 10) // Prevent overflow
            return (sign == 1) ? INT_MAX : INT_MIN;

        result = result * 10 + (str[i] - '0');
        i++;
    }
    return result * sign;
}
// Gère la commande exit
int builtin_exit(char **args, char ***env)
{
    int code;
    char *arg;
    
    (void)env; // Ignore env
    ft_putstr_fd("exit\n", 1);
    if (!args[1])
        exit(g_last_exit_status);
    if (args[2])
    {
        ft_putstr_fd("exit: too many arguments\n", 2);
        g_last_exit_status = 1;
        return (1);
    }
    // Handle quotes around the argument
    arg = remove_quotes(args[1]);
    if (!arg)
        arg = ft_strdup(args[1]);
    // Check if the argument is a valid number
    for (int i = 0; arg[i]; i++)
    {
        if ((arg[i] < '0' || arg[i] > '9') && arg[i] != '-' && arg[i] != '+')
        {
            ft_putstr_fd("exit: ", 2);
            ft_putstr_fd(args[1], 2);
            ft_putstr_fd(": numeric argument required\n", 2);
            free(arg);
            exit(2);
        }
    }
    code = ft_atoi(arg);
    free(arg);
    // Adjust exit code to match Bash behavior
    if (code > 255)
        code = code % 256;
    else if (code < 0)
        exit(156);
    g_last_exit_status = code;
    exit(code);
}