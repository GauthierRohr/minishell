/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_set1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 22:28:08 by grohr             #+#    #+#             */
/*   Updated: 2025/05/08 18:37:19 by grohr            ###   ########.fr       */
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

char	*expand_vars(const char *token, char **env)
{
	char	*expanded;
	char	var_name[256];
	const char	*ptr = token;
	int		i = 0, j = 0;
	char	*value;

	expanded = malloc(4096); // buffer large (à améliorer plus tard avec realloc)
	if (!expanded)
		return (NULL);

	while (*ptr)
	{
		if (*ptr == '$' && *(ptr + 1) && (ft_isalpha(*(ptr + 1)) || *(ptr + 1) == '_'))
		{
			ptr++;
			i = 0;
			while (*ptr && (ft_isalnum(*ptr) || *ptr == '_'))
				var_name[i++] = *ptr++;
			var_name[i] = '\0';
			value = get_env_value(var_name, env);
			if (value)
			{
				while (*value)
					expanded[j++] = *value++;
			}
			else
			{
				printf("VAR '%s' not found in env!\n", var_name);  // Debugging line
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
int	builtin_echo(char **args, char ***env)
{
	int	i;
	int	n_option;
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
		char *tmp = expand_vars(args[i], *env); 	// remplace les $VAR
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
int	builtin_cd(char **args, char ***env)
{
	char	*home_path;
	char	current_dir[1024];
	char	*target_dir;

	(void)env;
	if (!args[1])
	{
		home_path = getenv("HOME");
		if (!home_path)
		{
			printf("cd: HOME not set\n");
			return (1);
		}
		target_dir = home_path;
	}
	else
		target_dir = args[1];
	if (getcwd(current_dir, sizeof(current_dir)) == NULL)
	{
		perror("cd: getcwd error");
		return (1);
	}
	if (chdir(target_dir) != 0)
	{
		perror("cd");
		return (1);
	}
	return (0);
}
 
// Gère la commande pwd pour afficher le répertoire courant
// 
// @return 0 en cas de succès, 1 en cas d'erreur
// 
int	builtin_pwd(void)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)))
	{
		printf("%s\n", cwd);
		return (0);
	}
	perror("pwd");
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
        // Afficher l'environnement (comme avant)
        i = 0;
        while ((*env)[i])
        {
            printf("declare -x %s\n", (*env)[i]);
            i++;
        }
        return (0);
    }

    i = 1;
    while (args[i])
    {
        equal_sign = ft_strchr(args[i], '=');
        if (equal_sign)
        {
            *equal_sign = '\0'; // Temporairement couper pour vérifier la clé
            env_index = find_env_var(*env, args[i]);
            *equal_sign = '='; // Remettre l'égal

            if (env_index != -1)
            {
                // Variable existe déjà - la remplacer
                free((*env)[env_index]);
                (*env)[env_index] = ft_strdup(args[i]);
            }
            else
            {
                // Nouvelle variable - l'ajouter
                env_size = count_array(*env);
                new_env = malloc(sizeof(char *) * (env_size + 2));
                if (!new_env)
                    return (1);
                
                ft_memcpy(new_env, *env, sizeof(char *) * env_size);
                new_env[env_size] = ft_strdup(args[i]);
                new_env[env_size + 1] = NULL;
                
                free(*env);
                *env = new_env;
            }
        }
        i++;
    }
    return (0);
}
