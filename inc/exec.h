/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                              :+:    :+:           */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 22:28:10 by grohr             #+#    #+#             */
/*   Updated: 2025/06/03 14:34:26 by cjauregu       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# define CMD_NOT_FOUND        127
# define PERMISSION_DENIED    126

typedef struct s_quote_bounds
{
    int orig_len;
    int start;
    int i;
    int end;
} t_quote_bounds;

typedef struct s_copy_indexes
{
    int src;
    int dest;
} t_copy_indexes;

char	*remove_quotes(const char *str);
char	*remove_partial_quotes(const char *str);
int		execute_external(char **args, char **envp);

#endif
