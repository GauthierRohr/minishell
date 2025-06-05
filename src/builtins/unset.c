#include "../../inc/minishell.h"
#include <stdlib.h>

// Renvoie le nom de la variable (jusqu’au '=') ou une copie complète si '=' absent
static char	*get_var_name(const char *env_var)
{
	char	*equal;

	if (!env_var)
		return (NULL);
	equal = ft_strchr(env_var, '=');
	if (!equal)
		return (ft_strdup(env_var));
	return (ft_substr(env_var, 0, equal - env_var));
}

// Vérifie si le nom correspond à une variable système
static int	protected_var_list(const char *name)
{
	const char	*protected[] = {
		"PATH", "HOME", "USER", "SHELL", "PWD", "OLDPWD", "SHLVL", "_",
		"TERM", "LANG", "MallocNanoZone", "SECURITYSESSIONID", "COMMAND_MODE",
		"__CFBundleIdentifier", "LaunchInstanceID", "__CF_USER_TEXT_ENCODING",
		"XPC_SERVICE_NAME", "SSH_AUTH_SOCK", "XPC_FLAGS", "LOGNAME", "TMPDIR",
		"ORIGINAL_XDG_CURRENT_DESKTOP", "HOMEBREW_PREFIX", "HOMEBREW_CELLAR",
		"HOMEBREW_REPOSITORY", "INFOPATH", "TERM_PROGRAM",
		"TERM_PROGRAM_VERSION", "COLORTERM", "GIT_ASKPASS",
		"VSCODE_GIT_ASKPASS_NODE", "VSCODE_GIT_ASKPASS_EXTRA_ARGS",
		"VSCODE_GIT_ASKPASS_MAIN", "VSCODE_GIT_IPC_HANDLE",
		"VSCODE_INJECTION", "ZDOTDIR", "USER_ZDOTDIR",
		"VSCODE_PROFILE_INITIALIZED", NULL};
	int			i;

	i = 0;
	while (protected[i])
	{
		if (ft_strcmp(name, protected[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

// Renvoie 1 si la variable est considérée comme système (non supprimable)
static int	is_protected_var(const char *env_var)
{
	char	*name;
	int		ret;

	name = get_var_name(env_var);
	if (!name)
		return (1);
	ret = protected_var_list(name);
	free(name);
	return (ret);
}

// Supprime une entrée de l’environnement en décalant à gauche
static void	shift_env_left(char ***env, int start)
{
	int	i;

	i = start;
	while ((*env)[i + 1])
	{
		(*env)[i] = (*env)[i + 1];
		i++;
	}
	(*env)[i] = NULL;
}

// Supprime une variable par son nom si elle est présente et non protégée
static void	unset_var_by_name(char *name, char ***env)
{
	int	index;

	index = find_env_var(*env, name);
	if (index != -1 && !is_protected_var((*env)[index]))
	{
		free((*env)[index]);
		shift_env_left(env, index);
	}
}

// Supprime toutes les variables (si aucun arg) ou celles précisées (si args)
int	builtin_unset(char **args, char ***env)
{
	int	i;

	if (!args[1])
	{
		i = 0;
		while ((*env)[i])
		{
			if (!is_protected_var((*env)[i]))
			{
				free((*env)[i]);
				shift_env_left(env, i);
			}
			else
				i++;
		}
	}
	else
	{
		i = 1;
		while (args[i])
			unset_var_by_name(args[i++], env);
	}
	g_last_exit_status = 0;
	return (0);
}
