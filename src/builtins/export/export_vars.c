/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 20:34:22 by grohr             #+#    #+#             */
/*   Updated: 2025/06/05 20:48:20 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"
#include <stdio.h>
#include <stdlib.h>

/*
** print_export_vars:
** Affiche toutes les variables exportées
*/
void	print_export_vars(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		printf("declare -x %s\n", env[i]);
		i++;
	}
}

/*
** add_env_var:
** Ajoute une nouvelle variable dans env (réallocation)
*/
static int	add_env_var(char *arg, char ***env)
{
	int		env_size;
	char	**new_env;

	env_size = count_array(*env);
	new_env = malloc(sizeof(char *) * (env_size + 2));
	if (!new_env)
		return (1);
	ft_memcpy(new_env, *env, sizeof(char *) * env_size);
	new_env[env_size] = ft_strdup(arg);
	if (!new_env[env_size])
	{
		free(new_env);
		return (1);
	}
	new_env[env_size + 1] = NULL;
	free(*env);
	*env = new_env;
	return (0);
}

/*
** update_env_var:
** Met à jour une variable existante dans env
*/
static int	update_env_var(int index, char *arg, char **env)
{
	free(env[index]);
	env[index] = ft_strdup(arg);
	return (env[index] == NULL);
}

/*
** print_invalid_identifier_error:
** Affiche un message d'erreur pour identifiant invalide
*/
static void	print_invalid_identifier_error(char *arg)
{
	ft_putstr_fd("export: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
}

/*
** update_or_add_var:
** Met à jour ou ajoute une variable exportée
*/
int	update_or_add_var(char *arg, char ***env)
{
	char	*equal_sign;
	char	*var_name;
	int		env_index;
	int		ret;

	equal_sign = ft_strchr(arg, '=');
	if (equal_sign)
		var_name = ft_substr(arg, 0, equal_sign - arg);
	else
		var_name = ft_strdup(arg);
	if (!var_name)
		return (1);
	if (!is_valid_identifier(var_name))
	{
		print_invalid_identifier_error(arg);
		free(var_name);
		g_last_exit_status = 1;
		return (1);
	}
	free(var_name);
	env_index = find_env_var(*env, arg);
	if (env_index != -1)
		ret = update_env_var(env_index, arg, *env);
	else
		ret = add_env_var(arg, env);
	return (ret);
}
