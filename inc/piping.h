#ifndef PIPING_H
# define PIPING_H

void execute_pipeline(char ***commands, int num_cmds, char **envp);
int     contains_pipe(char **args);
void find_args(char **args, char **envp);

#endif