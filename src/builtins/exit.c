/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 20:27:21 by grohr             #+#    #+#             */
/*   Updated: 2025/06/05 20:27:21 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include <stdlib.h>

static void	exit_numeric_error(char *arg)
{
	ft_putstr_fd("exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
}

static char	*prepare_exit_arg(char *arg)
{
	char	*clean_arg;

	clean_arg = remove_quotes(arg);
	if (!clean_arg)
		clean_arg = ft_strdup(arg);
	return (clean_arg);
}

static int	check_exit_arg_error(char *arg, char *orig_arg)
{
	char *endptr;

	while (*arg == ' ' || (*arg >= '\t' && *arg <= '\r'))
		arg++;
	if (*arg == '\0' || (arg[0] == '-' && !ft_isdigit(arg[1])))
	{
		exit_numeric_error(orig_arg);
		return (255);
	}
	ft_strtol(arg, &endptr, 10);
	while (*endptr == ' ' || (*endptr >= '\t' && *endptr <= '\r'))
		endptr++;
	if (*endptr != '\0')
	{
		exit_numeric_error(orig_arg);
		return (2);
	}
	return (0);
}

static char	*handle_exit_args(char **args)
{
	char	*arg;

	if (args[2])
	{
		ft_putstr_fd("exit: too many arguments\n", 2);
		g_last_exit_status = 1;
		return (NULL);
	}
	arg = prepare_exit_arg(args[1]);
	return (arg);
}

int	builtin_exit(char **args, char ***env)
{
	long	code;
	char	*arg;
	int		err_code;

	(void)env;
	ft_putstr_fd("exit\n", 1);
	if (!args[1])
		exit(g_last_exit_status);
	arg = handle_exit_args(args);
	if (!arg)
		return (1);
	err_code = check_exit_arg_error(arg, args[1]);
	if (err_code != 0)
	{
		free(arg);
		exit(err_code);
	}
	code = ft_strtol(arg, NULL, 10);
	free(arg);
	code = code % 256;
	if (code < 0)
		code += 256;
	g_last_exit_status = (int)code;
	exit(g_last_exit_status);
}
