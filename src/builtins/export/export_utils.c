/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 20:34:27 by grohr             #+#    #+#             */
/*   Updated: 2025/06/05 20:52:26 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"
#include <stdlib.h>

// Compte le nombre d'éléments dans un tableau de chaînes NULL-terminé
int	count_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
		i++;
	return (i);
}

// Vérifie si name est un nom de variable valide (alpha ou _, alphanum après)
int	is_valid_identifier(const char *name)
{
	int	i;

	if (!name || !name[0])
		return (0);
	if (!ft_isalpha(name[0]) && name[0] != '_')
		return (0);
	i = 1;
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

// Cherche l'index d'une variable dans env, renvoie -1 si non trouvée
int	find_env_var(char **env, char *arg)
{
	int		i;
	char	*equal_pos;
	int		len;

	if (!env || !arg)
		return (-1);
	equal_pos = ft_strchr(arg, '=');
	if (equal_pos)
		len = (int)(equal_pos - arg);
	else
		len = (int)ft_strlen(arg);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], arg, len) == 0 && env[i][len] == '=')
			return (i);
		i++;
	}
	return (-1);
}
