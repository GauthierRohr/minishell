/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 22:28:10 by grohr             #+#    #+#             */
/*   Updated: 2025/05/27 14:49:57 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

/* builtins_main.c */
int	is_builtin(const char *cmd);
int	execute_builtin(char **args, char ***env);

/* builtins_set1.c */
int	builtin_echo(char **args, char ***env);
int	builtin_cd(char **args, char ***env);
int	builtin_pwd(void);
int	count_array(char **array);
int	builtin_export(char **args, char ***env);

/* builtins_set2.c */
int	find_env_var(char **env, const char *var);
int	builtin_unset(char **args, char ***env);
int	builtin_env(char **env);
int	builtin_exit(char **args, char ***env);

#endif
