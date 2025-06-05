/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env_value.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 19:55:15 by grohr             #+#    #+#             */
/*   Updated: 2025/06/05 19:55:15 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** get_env_value:
** Cherche la variable 'name' dans env et retourne sa valeur (après '=')
*/
#include"../../inc/minishell.h"

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
		if (ft_strncmp(env[i], name, len) == 0 &&
			env[i][len] == '=')
			return (env[i] + len + 1);
		i++;
	}
	return (NULL);
}

