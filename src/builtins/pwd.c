#include "../../inc/minishell.h"
#include <stdio.h>
#include <unistd.h>

/*
** builtin_pwd:
** Implémentation de la commande pwd
*/
int	builtin_pwd(void)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)))
	{
		printf("%s\n", cwd);
		g_last_exit_status = 0;
		return (0);
	}
	perror("minishell: pwd");
	g_last_exit_status = 1;
	return (1);
}
