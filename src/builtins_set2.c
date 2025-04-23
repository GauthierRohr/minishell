/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_set2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 22:28:19 by grohr             #+#    #+#             */
/*   Updated: 2025/04/23 22:28:20 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

//Recherche une variable d'environnement
//
// var Nom de la variable à chercher
// return : Index de la variable ou -1 si non trouvée
//
// also used in builtins_set1.c 
//
int	find_env_var(char **env, const char *var)
{
	int		i;
	size_t	len;

	i = 0;
	len = strlen(var);
	while (env[i])
	{
		if (ft_strncmp(env[i], var, len) == 0 && env[i][len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

// Gère la commande unset pour supprimer des variables d'environnement
//
// return 0 en cas de succès
// 
int	builtin_unset(char **args, char ***env)
{
	int		i;
	int		j;
	int		env_index;
	char	**new_env;
	int		env_size;

	i = 1;
	while (args[i])
	{
		env_index = find_env_var(*env, args[i]);
		if (env_index != -1)
		{
			env_size = count_array(*env);
			new_env = malloc(sizeof(char *) * env_size);
			if (!new_env)
				return (1);
			j = 0;
			env_size = 0;
			while ((*env)[j])
			{
				if (j != env_index)
					new_env[env_size++] = (*env)[j];
				else
					free((*env)[j]);
				j++;
			}
			new_env[env_size] = NULL;
			free(*env);
			*env = new_env;
		}
		i++;
	}
	return (0);
}

// Gère la commande env pour afficher l'environnement
//
// return 0 en cas de succès
//
int	builtin_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		printf("%s\n", env[i]);
		i++;
	}
	return (0);
}

// Gère la commande exit pour quitter le shell
//
// return exit_code, ou 0 si erreur
// 
int	builtin_exit(char **args)
{
	int	exit_code;

	printf("exit\n");
	if (!args[1])
		exit(0);
	exit_code = ft_atoi(args[1]);
	exit(exit_code);
	return (0);
}
