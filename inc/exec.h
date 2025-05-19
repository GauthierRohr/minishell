#ifndef EXEC_H
# define EXEC_H

#define CMD_NOT_FOUND        127
#define PERMISSION_DENIED    126

char	*remove_quotes(const char *str);
char 	*remove_partial_quotes(const char *str);
int		execute_external(char **args, char **envp);

#endif