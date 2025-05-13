/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 17:30:08 by grohr             #+#    #+#             */
/*   Updated: 2025/05/13 18:28:51 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include "../inc/tokenizer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Extrait un token entre quotes, et retourne l’indice juste après la quote fermante
int extract_quoted_token(const char *input, int *i, char quote, char ***tokens, int *size)
{
	int		start;
	char	*token;

	(*i)++; // Skip opening quote
	start = *i;
	while (input[*i] && input[*i] != quote)
		(*i)++;
	if (input[*i] == quote)
	{
		token = extract_token(input, start, *i);
		if (!add_token(tokens, size, token))
			return (0);
		(*i)++; // Skip closing quote
	}
	return (1);
}

// Met à jour l’état des guillemets lors du parsing (general, simple quote, double quote)
static void	update_state_quote(t_state *state, char c)
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

// Fonction principale : découpe l'input en tokens en gérant quotes et caractères spéciaux
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
        if (state == STATE_GENERAL && input[i] == ' ')
        {
            if (curr_i > 0)
            {
                current[curr_i] = '\0';
                if (!add_token(&tokens, &size, strdup(current)))
                {
                    free(current);
                    return (NULL);
                }
                curr_i = 0;
            }
            i++;
            continue;
        }
        else if (state == STATE_GENERAL && is_special_char(input[i]))
        {
            if (curr_i > 0)
            {
                current[curr_i] = '\0';
                if (!add_token(&tokens, &size, strdup(current)))
                {
                    free(current);
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
                    return (NULL);
                }
                i += 2;
            }
            else
            {
                current[0] = input[i];
                current[1] = '\0';
                if (!add_token(&tokens, &size, strdup(current)))
                {
                    free(current);
                    return (NULL);
                }
                i++;
            }
            continue;
        }
        else if (state == STATE_GENERAL && (input[i] == '\'' || input[i] == '"'))
        {
            char quote = input[i];
            current[curr_i++] = input[i++]; // Garde guillemet
            while (input[i] && input[i] != quote)
                current[curr_i++] = input[i++];
            if (input[i] == quote)
                current[curr_i++] = input[i++]; // Garde guillemet
            current[curr_i] = '\0';
            if (!add_token(&tokens, &size, strdup(current)))
            {
                free(current);
                return (NULL);
            }
            curr_i = 0;
            state = STATE_GENERAL;
            continue;
        }
        else
        {
            current[curr_i++] = input[i++]; // Continue à construire le mot
        }
    }
    if (curr_i > 0)
    {
        current[curr_i] = '\0';
        if (!add_token(&tokens, &size, strdup(current)))
        {
            free(current);
            return (NULL);
        }
    }
    free(current);
    return (tokens);
}