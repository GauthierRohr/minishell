/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_main.c                                     :+:    :+:           */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 21:48:34 by grohr             #+#    #+#             */
/*   Updated: 2025/06/02 15:50:06 by cjauregu       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

// Vérifie si une commande est un builtin
//
// return 1 si c'est un builtin, 0 sinon
//
int	is_builtin(const char *cmd)
{
	if (!cmd)
		return (0);
	return (
		ft_strcmp(cmd, "echo") == 0
		|| ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "pwd") == 0
		|| ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0
		|| ft_strcmp(cmd, "env") == 0
		|| ft_strcmp(cmd, "exit") == 0
	);
}

// Exécute une commande builtin
//
// return 0 en cas de succès, code d'erreur sinon
//
int	execute_builtin(char **args, char ***env)
{
	if (!args || !args[0])
		return (1);
	if (ft_strcmp(args[0], "echo") == 0)
		return (builtin_echo(args, env));
	else if (ft_strcmp(args[0], "cd") == 0)
		return (builtin_cd(args, env));
	else if (ft_strcmp(args[0], "pwd") == 0)
		return (builtin_pwd());
	else if (ft_strcmp(args[0], "export") == 0)
		return (builtin_export(args, env));
	else if (ft_strcmp(args[0], "unset") == 0)
		return (builtin_unset(args, env));
	else if (ft_strcmp(args[0], "env") == 0)
		return (builtin_env(*env));
	else if (ft_strcmp(args[0], "exit") == 0)
		return (builtin_exit(args, env));
	return (1);
}
