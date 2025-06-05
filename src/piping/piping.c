#include "../../inc/minishell.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*  

1) pipeline_utils.c (fonctions utilitaires liées aux commandes / pipeline)

    contains_pipe

    count_commands (static)

    extract_command (static)

    split_pipeline_commands

    free_tab (j'imagine que tu as une fonction free_tab, sinon à gérer)

2) exec_helpers.c (fonctions d’exécution et d’execve)

    get_cmd_path_or_error (static)

    handle_exec_and_exit (static)

    exec_child_process

    is_builtin / execute_builtin (si tu veux, sinon à part)

3) process_management.c (fork, pipe, wait, dup...)

    setup_stdio_and_pids (static)

    wait_and_cleanup (static)

    fork_and_exec (static) — celle que je t’ai faite pour factoriser fork

    handle_pipe_close (static) — celle pour gérer fermeture pipe

4) main_pipeline.c (fonction principale)

    execute_pipeline (non static car fonction publique)

    find_args (fonction publique)


Concernant le static et les .h :

    Dans le .h (exposé/public), tu mets les fonctions appelées depuis l’extérieur (ou d’autres fichiers) :

        execute_pipeline

        find_args

        contains_pipe (utile aussi hors pipeline)

        split_pipeline_commands (si utilisée ailleurs)

        exec_child_process (à voir, souvent static)

    Tout le reste en static pour limiter la visibilité au fichier.

Résumé des fonctions à mettre dans les .h (visibles hors fichier) :

    execute_pipeline

    find_args

    contains_pipe (peut-être utile hors pipeline)

    split_pipeline_commands (si utilisée ailleurs)

Exemple minishell_pipeline.h minimal :

#ifndef MINISHELL_PIPELINE_H
#define MINISHELL_PIPELINE_H

int contains_pipe(char **args);
char ***split_pipeline_commands(char **args, int *num_cmds);
void execute_pipeline(char ***cmds, int n, char **envp);
void find_args(char **args, char **envp);

#endif


*/

int contains_pipe(char **args)
{
	int i = 0;

	if (!args)
		return (0);
	while (args[i])
	{
		if (strcmp(args[i], "|") == 0)
			return (1);
		i++;
	}
	return (0);
}

static int count_commands(char **args)
{
	int i = 0;
	int count = 1;

	while (args[i])
	{
		if (strcmp(args[i], "|") == 0)
			count++;
		i++;
	}
	return (count);
}

static char **extract_command(char **args, int *i)
{
	int start = *i;
	int len = 0;
	int j = 0;
	char **cmd;

	while (args[*i] && strcmp(args[*i], "|") != 0)
	{
		len++;
		(*i)++;
	}
	cmd = malloc((len + 1) * sizeof(char *));
	if (!cmd)
		return (NULL);
	while (j < len)
	{
		cmd[j] = ft_strdup(args[start + j]);
		j++;
	}
	cmd[len] = NULL;
	if (args[*i])
		(*i)++;
	return (cmd);
}

static char ***split_pipeline_commands(char **args, int *num_cmds)
{
	int i = 0;
	int index = 0;
	char ***commands;

	*num_cmds = count_commands(args);
	commands = malloc((*num_cmds + 1) * sizeof(char **));
	if (!commands)
		return (NULL);
	while (args[i])
	{
		commands[index++] = extract_command(args, &i);
		if (!commands[index - 1])
			return (free(commands), NULL);
	}
	commands[index] = NULL;
	return (commands);
}

static char *get_cmd_path_or_error(char **args, char **envp)
{
	char *cmd_path;

	if (strchr(args[0], '/'))
		cmd_path = ft_strdup(args[0]);
	else
		cmd_path = ft_get_cmd_path(args[0], envp); // ✅ Correction ici
	if (!cmd_path)
	{
		fprintf(stderr, "%s: command not found\n", args[0]);
		free_tab(args);
		exit(127);
	}
	return (cmd_path);
}


static void handle_exec_and_exit(char *path, char **args, char **envp)
{
	execve(path, args, envp);
	perror(args[0]);
	free(path);
	free_tab(args);
	exit(127);
}

static void exec_child_process(char **cmd, int in_fd, int *pipefd, char **envp)
{
	char	**args;
	char	*cmd_path;
	int		redirect_status;

	if (in_fd != STDIN_FILENO)
		dup2(in_fd, STDIN_FILENO);
	if (pipefd)
	{
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);
	}
	redirect_status = process_redirections(cmd);
	args = clean_args(cmd);
	if (!args || !args[0])
		exit(redirect_status == -1 ? 1 : 0);
	if (is_builtin(args[0]))
		exit(execute_builtin(args, &envp));
	cmd_path = get_cmd_path_or_error(args, envp);
	handle_exec_and_exit(cmd_path, args, envp);
}

static void wait_and_cleanup(pid_t *pids, int num_cmds, int stdio[2])
{
	int i = 0;
	int status;

	while (i < num_cmds)
	{
		waitpid(pids[i], &status, 0);
		if (WIFEXITED(status) && i == num_cmds - 1)
			g_last_exit_status = WEXITSTATUS(status);
		i++;
	}
	dup2(stdio[0], STDIN_FILENO);
	dup2(stdio[1], STDOUT_FILENO);
	close(stdio[0]);
	close(stdio[1]);
	free(pids);
}

static int setup_stdio_and_pids(pid_t **pids, int num_cmds, int *stdio)
{
	*pids = malloc(num_cmds * sizeof(pid_t));
	if (!*pids)
		return (perror("minishell: malloc"), 1);
	stdio[0] = dup(STDIN_FILENO);
	stdio[1] = dup(STDOUT_FILENO);
	if (stdio[0] == -1 || stdio[1] == -1)
	{
		perror("minishell: dup");
		free(*pids);
		return (1);
	}
	return (0);
}

static void fork_and_exec(pid_t *pids, int i, char ***cmds, int in_fd,
		int *pfd, int n, char **envp)
{
	pids[i] = fork();
	if (pids[i] == -1)
	{
		perror("minishell: fork");
		free(pids);
		g_last_exit_status = 1;
		exit(EXIT_FAILURE);
	}
	if (pids[i] == 0)
		exec_child_process(cmds[i], in_fd, (i < n - 1 ? pfd : NULL), envp);
}

static void handle_pipe_close(int *pfd, int *in_fd)
{
	close(pfd[1]);
	if (*in_fd != STDIN_FILENO)
		close(*in_fd);
	*in_fd = pfd[0];
}

void execute_pipeline(char ***cmds, int n, char **envp)
{
	int		i = 0;
	int		in_fd = STDIN_FILENO;
	int		pfd[2];
	int		stdio[2];
	pid_t	*pids;

	if (setup_stdio_and_pids(&pids, n, stdio))
		return ((void)(g_last_exit_status = 1));
	while (i < n)
	{
		if (i < n - 1 && pipe(pfd) == -1)
			return (perror("minishell: pipe"), free(pids),
					(void)(g_last_exit_status = 1));
		fork_and_exec(pids, i, cmds, in_fd, pfd, n, envp);
		if (i < n - 1)
			handle_pipe_close(pfd, &in_fd);
		i++;
	}
	if (in_fd != STDIN_FILENO)
		close(in_fd);
	wait_and_cleanup(pids, n, stdio);
}

void find_args(char **args, char **envp)
{
	int i = 0;
	int num_cmds;
	char ***commands;

	commands = split_pipeline_commands(args, &num_cmds);
	if (!commands)
	{
		perror("minishell: malloc");
		g_last_exit_status = 1;
		return;
	}
	while (i < num_cmds)
	{
		if (!commands[i])
		{
			g_last_exit_status = 1;
			return;
		}
		i++;
	}
	execute_pipeline(commands, num_cmds, envp);
	i = 0;
	while (i < num_cmds)
		free_tab(commands[i++]);
	free(commands);
}
