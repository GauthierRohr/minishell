/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_partial_quotes.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 19:55:22 by grohr             #+#    #+#             */
/*   Updated: 2025/06/05 19:55:26 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"../../inc/minishell.h"

/* Removes partial quotes from the string without using for loops */
char *remove_partial_quotes(const char *str)
{
    char *result;
    int i;
    int j;
    t_state state;
    
    if (str == NULL)
        return NULL;
    result = malloc(strlen(str) + 1);
    if (result == NULL)
        return NULL;
    i = 0;
    j = 0;
    state = STATE_GENERAL;
    while (str[i] != '\0')
    {
        if (handle_quote_state(str, &i, &state) == 1)
            continue;
        result[j] = str[i];
        j = j + 1;
        i = i + 1;
    }
    result[j] = '\0';
    return result;
}