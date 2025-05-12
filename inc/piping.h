#ifndef PIPING_H
# define PIPING_H

void    execute_piped(char **cmd1, char **cmd2);
int     contains_pipe(char **args);
void    find_args(char **args);

#endif