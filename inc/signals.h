#ifndef SIGNALS_H
# define SIGNALS_H

void    sigint_handler(int signum);
void    init_signals(void);
void    restart_shell(const char *exec_path);

#endif