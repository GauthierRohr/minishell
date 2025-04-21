#include "../inc/minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

int main(void)
{
    // Initialize history functionality
    using_history();
    char    *input;

    while (input != NULL)
    {
        input = readline("Minishell> ");
        if (input == NULL)
            break;
        // Check if the user typed "exit"
        if (strcmp(input, "exit") == 0)
        {
            free(input);
            break;
        }
        // Add non-empty input to history
        if (*input != '\0')
            add_history(input);
        free(input);
    }

    // Retrieve and print the history list
    HIST_ENTRY **hist_list = history_list();
    if (hist_list != NULL) {
        printf("\nHistory List:\n");
        for (int i = 0; hist_list[i] != NULL; i++)
        {
            printf("%d: %s\n", i + history_base, hist_list[i]->line);
        }
    }
    else
        printf("No history available.\n");

    return 0;
}
