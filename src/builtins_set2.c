/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_set2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 22:28:19 by grohr             #+#    #+#             */
/*   Updated: 2025/05/08 19:43:19 by grohr            ###   ########.fr       */
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
    
    if (args[1]) // Si on a des arguments, erreur
    {
        ft_putstr_fd("unset: this shell only supports 'unset' without args\n", 2);
        return (1);
    }
    
    i = 0;
    while ((*env)[i])
    {
        if (is_user_defined_var((*env)[i]))
        {
            // Supprimer cette variable
            free((*env)[i]);
            // Décaler le reste du tableau
            env_index = i;
            while ((*env)[env_index + 1])
            {
                (*env)[env_index] = (*env)[env_index + 1];
                env_index++;
            }
            (*env)[env_index] = NULL;
            // On ne décrémente pas i pour vérifier la nouvelle case [i]
        }
        else
        {
            i++; // Passer à la suivante
        }
    }
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
    return (0);
}

// Gère la commande exit
int builtin_exit(char **args)
{
    int exit_code;

    printf("exit\n");
    if (!args[1])
        exit(0);
    exit_code = ft_atoi(args[1]);
    exit(exit_code);
    return (0);
}
