/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 22:28:36 by grohr             #+#    #+#             */
/*   Updated: 2025/04/23 23:28:12 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

void	free_env(char **env)
{
	int	i = 0;

	if (!env)
		return ;
	while (env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
}

void	free_tab(char **tab)
{
	int	i;

	if (!tab)
		return ;
	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

void	print_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab && tab[i])
	{
		printf("%stoken[%s%d%s] :%s %s->%s  %s  %s<-%s\n", CYAN, RST, i, CYAN, RST, RED, RST, tab[i], RED, RST);
		i++;
	}
	printf("\n");
}

int	main(int argc, char **argv, char **envp)
{
	char		*user_input;
	char		**args;
	char		**env;

	(void)argc;
	(void)argv;
	env = dup_env(envp);
	if (!env)
		return (1);
	using_history();
	user_input = NULL;
	while (1)
	{
		user_input = readline("minishell> ");
		// Si l'utilisateur appuie sur Ctrl+D (EOF)
		if (user_input == NULL)
		{
			write(1, "exit\n", 5);
			break;
		}
		// Ignore lignes vides + les ajoute pas à l'historique
		if (user_input[0] == '\0')
		{
			free(user_input);
			continue;
		}
		add_history(user_input);

		// ==== ✳️ GESTION DES BUILTINS ET COMMANDES ✳️ ====
		// ➤ Parsing de l'input :
		args = tokenize_input(user_input, 0, 0, 0);
		if (is_builtin(args[0]))
			execute_builtin(args, &env);
		else
			execute_external(args, env); 	//si c'est pas un builtin, on execute

		// ➤ Vérifier si c’est un builtin (cd, echo, pwd, export, unset, env, exit.)
		// ➤ Sinon, lancer l'exec : execve avec fork
		// ➤ Gérer les pipes/redirections plus tard ici ou avec redirect.c

		if (strcmp(user_input, "exit") == 0)
		{
			free(user_input);
			break;
		}
		/////////////////////////////////////
		// DEBUGGING : A RETIRER PLUS TARD //
		/////////////////////////////////////
		printf("%sShell original:\n%s", GREEN, RST);
		FILE *fp = popen(user_input, "r");
		if (fp)
		{
		    char buffer[1024];
		    while (fgets(buffer, sizeof(buffer), fp) != NULL)
		    {
		        printf("%s", buffer);  // Affiche tel quel sans modification
		    }
		    pclose(fp);
		}
		printf("%s=================%s\n", RED, RST);
		printf("%s=== DEBUGGING ===%s\n", RED, RST);
		printf("%s=================%s\n", RED, RST);
		printf("\n%sCommande reçue :%s %s\n\n", CYAN, RST, user_input); 
		print_tab(args);
		/////////////////////////////////////
		//          FIN DEBUGGING		   //
		/////////////////////////////////////
		free(user_input);
		free_tab(args); // free du tableau de tokens
	}

	// ==== ✳️ TODO: LIBÉRATION MÉMOIRE STRUCTURES ====
	// ➤ structures d'environnement, de commandes, etc.
	// ➤ Penser à tout free ici (liste, historique, etc.)
	free_env(env);

	return (0);
}
