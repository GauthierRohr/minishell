/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piping.h                                            :+:    :+:           */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 22:28:10 by grohr             #+#    #+#             */
/*   Updated: 2025/06/03 14:37:17 by cjauregu       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPING_H
# define PIPING_H

void	execute_pipeline(char ***commands, int num_cmds, char **envp);

//piping_utils.c
void    wait_children(pid_t *pids, int num_cmds, int *status);
void    execute_external(char **args, char **envp);
int     prepare_pipe(int i, int num_cmds, int pipefd[2], pid_t *pids);
void    setup_child_io(int in_fd, int pipefd[2]);
void    handle_parent_pipe(int *in_fd, int pipefd[2]);

//find_args.c
void	find_args(char **args, char **envp);
char    ***build_commands(char **args, int num_cmds);
void    free_cmds(char ***cmds, int num_cmds);
int         count_pipes(char **args);
char    **dup_tokens(char **args, int *i);

#endif