#include "get_cmd_path.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

static char	*ft_strjoin3(const char *s1, const char *s2, const char *s3)
{
	int		len = strlen(s1) + strlen(s2) + strlen(s3);
	char	*res = malloc(len + 1);
	if (!res)
		return (NULL);
	strcpy(res, s1);
	strcat(res, s2);
	strcat(res, s3);
	return (res);
}

static char	*ft_getenv(const char *name, char **envp)
{
	int		len = strlen(name);
	int		i = 0;

	while (envp[i])
	{
		if (strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
			return (envp[i] + len + 1);
		i++;
	}
	return (NULL);
}

char	*ft_get_cmd_path(char *cmd, char **envp)
{
	char	*path = ft_getenv("PATH", envp);
	char	*full_path;
	char	*token;
	char	*path_copy;

	if (!path)
		return (NULL);
	path_copy = strdup(path);
	if (!path_copy)
		return (NULL);
	token = strtok(path_copy, ":");
	while (token)
	{
		full_path = ft_strjoin3(token, "/", cmd);
		if (!full_path)
			break;
		if (access(full_path, X_OK) == 0)
		{
			free(path_copy);
			return (full_path);
		}
		free(full_path);
		token = strtok(NULL, ":");
	}
	free(path_copy);
	return (NULL);
}
