/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 17:28:08 by grohr             #+#    #+#             */
/*   Updated: 2025/05/27 15:07:45 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_cmd_path.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

static char	*ft_strjoin3(const char *s1, const char *s2, const char *s3)
{
	int		len;
	char	*res;

	len = strlen(s1) + strlen(s2) + strlen(s3);
	res = malloc(len + 1);
	if (!res)
		return (NULL);
	strcpy(res, s1);
	strcat(res, s2);
	strcat(res, s3);
	return (res);
}

static char	*ft_getenv(const char *name, char **envp)
{
	int		len;
	int		i;

	i = 0;
	len = strlen(name);
	while (envp[i])
	{
		if (strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
			return (envp[i] + len + 1);
		i++;
	}
	return (NULL);
}

char	*ft_check_cmd_path(char *token, char *cmd)
{
	char	*full_path;

	full_path = ft_strjoin3(token, "/", cmd);
	if (!full_path)
		return (NULL);
	if (access(full_path, X_OK) == 0)
		return (full_path);
	free(full_path);
	return (NULL);
}

char	*ft_get_cmd_path(char *cmd, char **envp)
{
	char	*path;
	char	*path_copy;
	char	*token;
	char	*full_path;

	path = ft_getenv("PATH", envp);
	if (!path)
		return (NULL);
	path_copy = strdup(path);
	if (!path_copy)
		return (NULL);
	token = strtok(path_copy, ":");
	while (token)
	{
		full_path = ft_check_cmd_path(token, cmd);
		if (full_path)
		{
			free(path_copy);
			return (full_path);
		}
		token = strtok(NULL, ":");
	}
	free(path_copy);
	return (NULL);
}
