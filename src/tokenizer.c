/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#           */
/*   Created: 2025/05/13 17:30:08 by grohr             #+#    #    #             */
/*   Updated: 2025/05/27 16:21:08 by grohr            ###   #    #             */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include "../inc/tokenizer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void update_state_quote(t_state *state, char c)
{
    if (*state == STATE_GENERAL)
    {
        if (c == '\'')
            *state = STATE_IN_SINGLE_QUOTE;
        else if (c == '\"')
            *state = STATE_IN_DOUBLE_QUOTE;
    }
    else if (*state == STATE_IN_SINGLE_QUOTE && c == '\'')
        *state = STATE_GENERAL;
    else if (*state == STATE_IN_DOUBLE_QUOTE && c == '\"')
        *state = STATE_GENERAL;
}

char **tokenize_input(const char *input, int size, int i)
{
    char **tokens = NULL;
    t_state state = STATE_GENERAL;
    int input_len = strlen(input);
    int curr_i = 0;
    char *current = malloc(input_len + 1);

    if (!current)
        return (NULL);

    while (input[i])
    {
        update_state_quote(&state, input[i]);

        // Handle quoted strings
        if (state == STATE_IN_SINGLE_QUOTE || state == STATE_IN_DOUBLE_QUOTE)
        {
            current[curr_i++] = input[i++];
            if (state == STATE_GENERAL) // Quote closed
            {
                current[curr_i] = '\0';
                if (!add_token(&tokens, &size, strdup(current)))
                {
                    free(current);
                    free_tab(tokens);
                    return (NULL);
                }
                curr_i = 0;
            }
            continue;
        }

        // Handle spaces in general state
        if (state == STATE_GENERAL && input[i] == ' ')
        {
            if (curr_i > 0)
            {
                current[curr_i] = '\0';
                if (!add_token(&tokens, &size, strdup(current)))
                {
                    free(current);
                    free_tab(tokens);
                    return (NULL);
                }
                curr_i = 0;
            }
            i++;
            continue;
        }

        // Handle special characters (<, >, |, <<, >>)
        if (state == STATE_GENERAL && is_special_char(input[i]))
        {
            if (curr_i > 0)
            {
                current[curr_i] = '\0';
                if (!add_token(&tokens, &size, strdup(current)))
                {
                    free(current);
                    free_tab(tokens);
                    return (NULL);
                }
                curr_i = 0;
            }
            if ((input[i] == '<' && input[i + 1] == '<') || (input[i] == '>' && input[i + 1] == '>'))
            {
                current[0] = input[i];
                current[1] = input[i + 1];
                current[2] = '\0';
                if (!add_token(&tokens, &size, strdup(current)))
                {
                    free(current);
                    free_tab(tokens);
                    return (NULL);
                }
                i += 2;
            }
            else if (input[i] == '<' || input[i] == '>' || input[i] == '|')
            {
                current[0] = input[i];
                current[1] = '\0';
                if (!add_token(&tokens, &size, strdup(current)))
                {
                    free(current);
                    free_tab(tokens);
                    return (NULL);
                }
                i++;
            }
            continue;
        }

        // Handle regular characters
        current[curr_i++] = input[i++];
    }

    // Add the last token if any
    if (curr_i > 0)
    {
        current[curr_i] = '\0';
        if (!add_token(&tokens, &size, strdup(current)))
        {
            free(current);
            free_tab(tokens);
            return (NULL);
        }
    }

    free(current);
    return (tokens);
}