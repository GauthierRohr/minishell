/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_set1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 22:28:08 by grohr             #+#    #+#             */
/*   Updated: 2025/05/11 13:04:08 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

#include <string.h>

// Pour prendre en compte les echo $PWD ou autres commandes
char	*get_env_value(const char *name, char **env)
{
	int		i;
	size_t	len;

	if (!name || !env)
		return (NULL);
	len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, len) == 0 && env[i][len] == '=')
			return (env[i] + len + 1); // retourne juste la valeur
		i++;
	}
	return (NULL);
}

// Dans builtins_set1.c, modifier expand_vars pour gérer $?
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
            // pour "$?"
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
            // Gestion normale des vars d'env
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
                expanded[j++] = *ptr++;
            }
        }
        else
            expanded[j++] = *ptr++;
    }
    expanded[j] = '\0';
    return (expanded);
}

// Gère la commande echo avec option -n
// 
// return 0 en cas de succès
//
int builtin_echo(char **args, char ***env)
{
    int i;
    int n_option;
    char *clean_arg;

    n_option = 0;
    i = 1;
    if (args[1] && ft_strcmp(args[1], "-n") == 0)
    {
        n_option = 1;
        i = 2;
    }
    while (args[i])
    {
        char *tmp = expand_vars(args[i], *env);
        clean_arg = remove_quotes(tmp);
        free(tmp);                              
        printf("%s", clean_arg);
        free(clean_arg);
        
        if (args[i + 1])
            printf(" ");
        i++;
    }
    if (!n_option)
        printf("\n");
    g_last_exit_status = 0;
    return (0);
}

// Gère la commande cd : changer de répertoire
//
// return 0 en cas de succès, 1 en cas d'erreur
//
// getcwd(current_dir, sizeof(current_dir))
// Appelle getcwd pour stocker le chemin actuel dans le buffer current_dir.
// Le second argument est la taille max qu'on autorise à écrire dedans (ici 1024).
//
// chdir() tente de changer le répertoire de travail courant du processus
// vers le chemin donné (target_dir). Il retourne :
// 		-> 0 en cas de succès
// 		-> -1 en cas d'erreur (ex : chemin invalide, permissions refusées)
// Donc si chdir(...) != 0, cela signifie que le changement a échoué.
//
int builtin_cd(char **args, char ***env)
{
    char    *home_path;
    char    current_dir[1024];
    char    *target_dir;

    (void)env;
    if (!args[1])
    {
        home_path = getenv("HOME");
        if (!home_path)
        {
            ft_putstr_fd("minishell: cd: HOME not set\n", 2);
            g_last_exit_status = 1;
            return (1);
        }
        target_dir = home_path;
    }
    else
        target_dir = args[1];
    if (getcwd(current_dir, sizeof(current_dir)) == NULL)
    {
        perror("minishell: cd: getcwd error");
        g_last_exit_status = 1;
        return (1);
    }
    if (chdir(target_dir) != 0)
    {
        perror("minishell: cd");
        g_last_exit_status = 1;
        return (1);
    }
    g_last_exit_status = 0;
    return (0);
}
 
// Gère la commande pwd pour afficher le répertoire courant
// 
// @return 0 en cas de succès, 1 en cas d'erreur
// 
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

// Compte le nombre d'éléments dans un tableau de chaînes
// 
// @param array Tableau à compter
// @return Nombre d'éléments
//
// also used in builtins_set2.c 
int	count_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
		i++;
	return (i);
}

// Gère la commande export pour définir des variables d'environnement
//
// return 0 en cas de succès, 1 en cas d'erreur
//
int builtin_export(char **args, char ***env)
{
    int i;
    int env_index;
    char **new_env;
    int env_size;
    char *equal_sign;

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
        if (equal_sign)
        {
            *equal_sign = '\0';
            env_index = find_env_var(*env, args[i]);
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
        i++;
    }
    g_last_exit_status = 0;
    return (0);
}
