/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.h                                          :+:    :+:           */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 22:28:10 by grohr             #+#    #+#             */
/*   Updated: 2025/06/03 14:38:32 by cjauregu       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECT_H
# define REDIRECT_H

// redirect_utils.c

char	**clean_args(char **args);
int		backup_stdio(int *stdin_backup, int *stdout_backup);
void	restore_stdio(int stdin_backup, int stdout_backup);
char	**handle_redirections(char **args, char ***env);

int		handle_input_redirect(char *file);
int		handle_output_redirect(char *file);
int		handle_append_redirect(char *file);
int		handle_heredoc(char *delimiter);

// redirect.c
int		process_redirections(char **args);

#endif 
