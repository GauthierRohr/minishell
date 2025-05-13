#ifndef EXEC_H
# define EXEC_H

char	*remove_quotes(const char *str);
char 	*remove_partial_quotes(const char *str);
int		execute_external(char **args, char **envp);

#endif