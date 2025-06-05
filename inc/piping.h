/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piping.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 22:28:10 by grohr             #+#    #+#             */
/*   Updated: 2025/06/05 21:29:31 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPING_H
# define PIPING_H

/* child_pipeline.c */
void	exec_child_pipeline(char **cmd, int in_fd, int out_fd, char **envp);

/* exec_pipeline.c */
//void	restore_stdio(int in, int out);
void	execute_pipeline(char ***cmds, int n, char **envp);
void	find_and_execute_pipeline(char **args, char **envp);

/* piping.c */
int		contains_pipe(char **args);
int		handle_pipes(char **args, char **envp);

/* split_pipeline.c */
char	***split_pipeline(char **args);


void	find_args(char **args, char **envp);

#endif
