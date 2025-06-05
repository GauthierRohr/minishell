#include "../../inc/minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Affiche une erreur spécifique au cd
static void	builtin_cd_print_error(char *path)
{
	ft_putstr_fd("cd: ", 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
}

// Récupère le chemin à utiliser avec cd selon args/env
static char	*builtin_cd_get_path(char **args, char **env)
{
	char	*path;

	if (!args[1])
		path = get_env_value("HOME", env);
	else if (args[2])
		path = NULL;
	else
	{
		path = expand_vars(args[1], env);
		if (!path)
			path = ft_strdup(args[1]);
	}
	return (path);
}

int	builtin_cd(char **args, char ***env)
{
	char	*path;
	int		ret;

	if (args[2])
	{
		ft_putstr_fd("cd: too many arguments\n", 2);
		return (1);
	}
	path = builtin_cd_get_path(args, *env);
	if (!path)
	{
		ft_putstr_fd("cd: memory error\n", 2);
		g_last_exit_status = 1;
		return (1);
	}
	ret = chdir(path);
	if (ret == -1)
	{
		builtin_cd_print_error(path);
		g_last_exit_status = 1;
	}
	else
		g_last_exit_status = 0;
	free(path);
	return (g_last_exit_status);
}
