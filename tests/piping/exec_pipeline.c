//#include "../../inc/minishell.h"
#include <sys/wait.h>

void	restore_stdio(int in, int out)
{
	dup2(in, STDIN_FILENO);
	dup2(out, STDOUT_FILENO);
	close(in);
	close(out);
}

int	execute_pipeline(char ***cmds, int n, char **envp)
{
	int		i = 0;
	int		in_fd = STDIN_FILENO, pipefd[2];
	pid_t	*pids = malloc(n * sizeof(pid_t));
	int		status, backup_in = dup(STDIN_FILENO), backup_out = dup(STDOUT_FILENO);

	if (!pids || backup_in == -1 || backup_out == -1)
		return (free(pids), g_last_exit_status = 1, 1);
	while (i < n)
	{
		if (i < n - 1 && pipe(pipefd) == -1)
			return (perror("pipe"), free(pids), restore_stdio(backup_in, backup_out), 1);
		pids[i] = fork();
		if (pids[i] < 0)
			return (perror("fork"), free(pids), restore_stdio(backup_in, backup_out), 1);
		else if (pids[i] == 0)
			exec_child_pipeline(cmds[i], in_fd, (i < n - 1) ? pipefd[1] : -1, envp);
		if (in_fd != STDIN_FILENO)
			close(in_fd);
		if (i < n - 1)
		{
			close(pipefd[1]);
			in_fd = pipefd[0];
		}
		i++;
	}
	restore_stdio(backup_in, backup_out);
	i = 0;
	while (i < n)
	{
		waitpid(pids[i], &status, 0);
		if (WIFEXITED(status) && i == n - 1)
			g_last_exit_status = WEXITSTATUS(status);
		i++;
	}
	free(pids);
	return (0);
}

int	find_args(char **args, char **envp)
{
	char	***cmds;
	int		num;

	cmds = split_pipeline(args);
	if (!cmds)
		return (g_last_exit_status = 1, 1);
	for (num = 0; cmds[num]; num++) {}
	execute_pipeline(cmds, num, envp);
	for (int i = 0; cmds[i]; i++)
		free_tab(cmds[i]);
	free(cmds);
	return (0);
}
