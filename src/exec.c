// Exécution externe
//
// Fork + execve : pour lancer des binaires.
// Gérer les erreurs de path, permissions, etc.

#include "../inc/minishell.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Supprime les ' et " d'un token (in-place), pour afficher la bonne sortie dans le terminal
static void	remove_quotes_inplace(char *str)
{
	int	i = 0;
	int	j = 0;

	while (str[i])
	{
		if (str[i] != '\"' && str[i] != '\'')
			str[j++] = str[i];
		i++;
	}
	str[j] = '\0';
}

// Exécute une commande externe via fork + execvp
int	execute_external(char **args, char **envp)
{
	pid_t	pid;
	int		status;

	(void)envp; // temporairement pas utilisé, a voir ce qu'on en fait
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (1);
	}
	if (pid == 0)
	{
		int i = 0;
		while (args[i])
		{
			remove_quotes_inplace(args[i]); // Supprime quotes avant exec parce que j'avais
			i++;							// laissé dans les tokens pour pas confondre l;exec de ' et "
		}
		if (execvp(args[0], args) == -1)
		{
			perror("execvp");
			exit(EXIT_FAILURE);
		}
	}
	else
		waitpid(pid, &status, 0);
	return (WEXITSTATUS(status));
}
