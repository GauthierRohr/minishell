/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 22:28:10 by grohr             #+#    #+#             */
/*   Updated: 2025/06/05 21:07:47 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

/////////////////* export *//////////////////
// 1-export_utils.c
int	count_array(char **array);
int	is_valid_identifier(const char *name);
int	find_env_var(char **env, char *arg);

// 2-export_vars.c
void	print_export_vars(char **env);
int	update_or_add_var(char *arg, char ***env);

// 3-export.c
int	builtin_export(char **args, char ***env);
/////////////////////////////////////////////

/* builtins_main.c */
int	is_builtin(const char *cmd);
int	execute_builtin(char **args, char ***env);

/* cd.c */
int	builtin_cd(char **args, char ***env);

/* echo.c */
int	builtin_echo(char **args, char ***env);

/* env.c */
int	builtin_env(char **env);

/* exit.c */
int	builtin_exit(char **args, char ***env);

/* pwd.c */
int	builtin_pwd(void);

/* unset.c */
int	builtin_unset(char **args, char ***env);





#endif
