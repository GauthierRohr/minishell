#include "../inc/minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

void	print_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab && tab[i])
	{
		printf("token[%d] = -->  %s  <--\n", i, tab[i]);
		i++;
	}
}

int main(void)
{
	char    	*user_input;
	char		**args;
	t_history	*history = NULL;

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
		// Ignore les lignes vides + les ajoute pas à l'historique
		if (user_input[0] == '\0')
		{
			free(user_input);
			continue;
		}
		add_history(user_input);
		add_to_history(&history, user_input); // A retirer plus tard

		// ==== ✳️ GESTION DES BUILTINS ET COMMANDES ✳️ ====
		// ➤ Parsing de l'input :
		args = tokenize_input(user_input, 0, 0, 0);
		print_tab(args); 								// debug -> print tokens, a retirer plus tard

		// ➤ Vérifier si c’est un builtin (cd, echo, pwd, export, unset, env, exit.)
		// ➤ Sinon, lancer l'exec : execve avec fork
		// ➤ Gérer les pipes/redirections plus tard ici ou avec redirect.c

		if (strcmp(user_input, "exit") == 0)
		{
			free(user_input);
			break;
		}
		printf("\nCommande reçue : %s\n", user_input); // ➤ echo pour debug user_input

		
		free(user_input);
		free_history(history);
	}

	// ==== ✳️ TODO: LIBÉRATION MÉMOIRE STRUCTURES ====
	// ➤ Quand vous avez des structures d'environnement, de commandes, etc.
	// ➤ Pensez à tout free ici (liste, historique, etc.)

	return (0);
}