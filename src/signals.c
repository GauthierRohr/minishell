#include "../inc/minishell.h"
// Contiendra les signaux (SIGINT, handler)

// Gère les comportements :
// ctrl-C (affiche une nouvelle ligne)
// ctrl-\ (ne fait rien)
// ctrl-D (gère readline qui renvoie NULL)

/* Signal handler for SIGINT (Ctrl+C) */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

/* SIGINT handler: Called when Ctrl+C is pressed */
void sigint_handler(int signum)
{
    (void)signum;  // Silence unused parameter warning

    // Write a newline so the prompt starts on a new line.
    write(1, "\n", 1);

    // Clear the current input and redisplay the prompt (GNU Readline functions).
    rl_on_new_line();
    rl_redisplay();
}

/*
 * ignore_sigquit - Sets up the signal handler to ignore SIGQUIT (Ctrl+\).
 */
void ignore_sigquit(void)
{
    struct sigaction sa_quit;
    memset(&sa_quit, 0, sizeof(sa_quit));
    sa_quit.sa_handler = SIG_IGN;  // Tell the system to ignore SIGQUIT
    sigemptyset(&sa_quit.sa_mask);
    if (sigaction(SIGQUIT, &sa_quit, NULL) == -1) {
        perror("sigaction(SIGQUIT)");
        exit(EXIT_FAILURE);
    }
}

/*
 * init_signals - Initializes signal handling:
 *   - Sets a custom handler for SIGINT (Ctrl+C).
 *   - Delegates the SIGQUIT (Ctrl+\) setting to ignore_sigquit().
 */
void init_signals(void)
{
    struct sigaction sa_int;
    memset(&sa_int, 0, sizeof(sa_int));
    sa_int.sa_handler = sigint_handler;
    sa_int.sa_flags = SA_RESTART;  // Restart system calls if interrupted
    sigemptyset(&sa_int.sa_mask);
    if (sigaction(SIGINT, &sa_int, NULL) == -1) {
        perror("sigaction(SIGINT)");
        exit(EXIT_FAILURE);
    }

    // Set the program to ignore SIGQUIT.
    ignore_sigquit();
}