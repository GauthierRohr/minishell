/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_set1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 22:28:08 by grohr             #+#    #+#             */
/*   Updated: 2025/04/23 22:33:45 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// Gère la commande echo avec option -n
// 
// @return 0 en cas de succès
// 
int	builtin_echo(char **args)
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
		clean_arg = remove_quotes(args[i]);
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
int	builtin_export(char **args, char ***env)
{
	int		i;
	int		env_index;
	char	**new_env;
	int		env_size;
	char	*equal_sign;

	if (!args[1])
	{
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
		equal_sign = strchr(args[i], '=');
		if (equal_sign)
		{
			*equal_sign = '\0';
			env_index = find_env_var(*env, args[i]);
			*equal_sign = '=';
			if (env_index != -1)
			{
				free((*env)[env_index]);
				(*env)[env_index] = ft_strdup(args[i]);
			}
			else
			{
				env_size = count_array(*env);
				new_env = malloc(sizeof(char *) * (env_size + 2));
				if (!new_env)
					return (1);
				env_size = -1;
				while ((*env)[++env_size])
					new_env[env_size] = (*env)[env_size];
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
