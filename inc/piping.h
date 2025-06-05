/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piping.h                                            :+:    :+:           */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 22:28:10 by grohr             #+#    #+#             */
/*   Updated: 2025/06/05 16:13:52 by cjauregu       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPING_H
# define PIPING_H

void	execute_pipeline(char ***commands, int num_cmds, char **envp);
int		contains_pipe(char **args);
void	find_args(char **args, char **envp);

#endif
