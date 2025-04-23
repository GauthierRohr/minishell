/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_main.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 21:48:34 by grohr             #+#    #+#             */
/*   Updated: 2025/04/23 21:48:36 by grohr            ###   ########.fr       */
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
		strcmp(cmd, "echo") == 0
		|| strcmp(cmd, "cd") == 0
		|| strcmp(cmd, "pwd") == 0
		|| strcmp(cmd, "export") == 0
		|| strcmp(cmd, "unset") == 0
		|| strcmp(cmd, "env") == 0
		|| strcmp(cmd, "exit") == 0
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
	if (strcmp(args[0], "echo") == 0)
		return (builtin_echo(args));
	else if (strcmp(args[0], "cd") == 0)
		return (builtin_cd(args, env));
	else if (strcmp(args[0], "pwd") == 0)
		return (builtin_pwd());
	else if (strcmp(args[0], "export") == 0)
		return (builtin_export(args, env));
	else if (strcmp(args[0], "unset") == 0)
		return (builtin_unset(args, env));
	else if (strcmp(args[0], "env") == 0)
		return (builtin_env(*env));
	else if (strcmp(args[0], "exit") == 0)
		return (builtin_exit(args));
	return (1);
}