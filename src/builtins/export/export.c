/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 20:34:20 by grohr             #+#    #+#             */
/*   Updated: 2025/06/05 20:38:35 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

int	builtin_export(char **args, char ***env)
{
	int	i;

	if (!args[1])
	{
		print_export_vars(*env);
		g_last_exit_status = 0;
		return (0);
	}
	i = 1;
	while (args[i])
	{
		update_or_add_var(args[i], env);
		i++;
	}
	return (g_last_exit_status);
}
