/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_pipeline.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 21:05:19 by grohr             #+#    #+#             */
/*   Updated: 2025/06/05 21:30:38 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "../../inc/minishell.h"

void	exec_child_pipeline(char **cmd, int in_fd, int out_fd, char **envp)
{
	if (in_fd != STDIN_FILENO)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	if (out_fd != -1)
	{
		dup2(out_fd, STDOUT_FILENO);
		close(out_fd);
	}
	if (process_redirections(cmd) == -1)
		exit(1);
	char **cleaned = clean_args(cmd);
	if (!cleaned || !cleaned[0])
		exit(0);
	if (is_builtin(cleaned[0]))
	{
		int ret = execute_builtin(cleaned, &envp);
		free_tab(cleaned);
		exit(ret);
	}
	char *path = strchr(cleaned[0], '/') ? ft_strdup(cleaned[0]) : ft_get_cmd_path(cleaned[0], envp);
	if (!path)
	{
		fprintf(stderr, "%s: command not found\n", cleaned[0]);
		free_tab(cleaned);
		exit(127);
	}
	execve(path, cleaned, envp);
	perror(cleaned[0]);
	free(path);
	free_tab(cleaned);
	exit(127);
}
