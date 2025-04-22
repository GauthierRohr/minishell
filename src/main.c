#include "../inc/minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

// Fonction principale de minishell
int main(void)
{
    char    *user_input;

    // Initialise la gestion de l'historique readline
    using_history();
    user_input = NULL;

    // Boucle principale du shell
    while (1)
    {
        user_input = readline("minishell> ");

        // Si l'utilisateur appuie sur Ctrl+D (EOF)
        if (user_input == NULL)
        {
            write(1, "exit\n", 5);
            break;
        }

        // Ignore les lignes vides mais les ajoute pas à l'historique
        if (user_input[0] == '\0')
        {
            free(user_input);
            continue;
        }

        // Ajoute l'input à l'historique readline
        add_history(user_input);

        // ==== ✳️ TO_DO: GESTION DES BUILTINS ET COMMANDES ====
        // ➤ Ici on doit parser l'input
        // ➤ Vérifier si c’est un builtin (cd, echo, etc.)
        // ➤ Sinon, lancer l'exec : execve avec fork
        // ➤ Gérer les pipes/redirections plus tard ici ou avec redirect.c

        if (strcmp(user_input, "exit") == 0)
        {
            free(user_input);
            break;
        }

        // ➤ Pour l'instant, simple echo pour debug
        printf("Commande reçue : %s\n", user_input);

        free(user_input);
    }

    // ==== ✳️ TODO: LIBÉRATION MÉMOIRE STRUCTURES ====
    // ➤ Quand vous avez des structures d'environnement, de commandes, etc.
    // ➤ Pensez à tout free ici (liste, historique, etc.)

    return (0);
}