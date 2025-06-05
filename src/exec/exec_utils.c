/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 22:29:03 by grohr             #+#    #+#             */
/*   Updated: 2025/05/27 16:34:26 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void adjust_quote_bounds(const char *str, t_quote_bounds *bounds)
{
    int len;
    len = ft_strlen(str);
    if (len >= 3 && ((str[0] == '+') || (str[0] == '-')))
    {
        if (((str[1] == '"') && (str[len - 1] == '"')) ||
            ((str[1] == '\'') && (str[len - 1] == '\'')))
        {
            bounds->start = 1;
            bounds->i = 2;
            bounds->end = len - 1;
            return;
        }
    }
    if (len >= 2 &&
        (((str[0] == '"') && (str[len - 1] == '"')) ||
         ((str[0] == '\'') && (str[len - 1] == '\''))))
    {
        bounds->i = 1;
        bounds->end = len - 1;
    }
}

/* Copies characters from source to dest using a struct for indexes */
static void copy_range(const char *source, int src_end, char *dest,
                       t_copy_indexes *indexes)
{
    while (indexes->src < src_end)
    {
        dest[indexes->dest] = source[indexes->src];
        indexes->dest = indexes->dest + 1;
        indexes->src = indexes->src + 1;
    }
    dest[indexes->dest] = '\0';
}

static char *init_remove_quotes_vars(const char *str, t_quote_bounds *bounds)
{
    char *result;
    bounds->orig_len = ft_strlen(str);
    bounds->start = 0;
    bounds->i = 0;
    bounds->end = bounds->orig_len;
    result = malloc(bounds->orig_len + 1);
    return result;
}

/* Removes surrounding quotes (and preserves leading + or -) */
char *remove_quotes(const char *str)
{
    t_quote_bounds bounds;
    char *result;
    t_copy_indexes indexes;
    
    if (str == NULL)
        return NULL;
    result = init_remove_quotes_vars(str, &bounds);
    if (result == NULL)
        return NULL;
    adjust_quote_bounds(str, &bounds);
    if (bounds.start == 1)
        result[0] = str[0];
    indexes.src = bounds.i;
    indexes.dest = bounds.start;
    copy_range(str, bounds.end, result, &indexes);
    return result;
}