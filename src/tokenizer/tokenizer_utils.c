/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                   :+:    :+:           */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 17:29:58 by grohr             #+#    #+#             */
/*   Updated: 2025/06/03 14:08:28 by cjauregu       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/tokenizer.h"
#include "../inc/minishell.h"
#include <stdlib.h>
#include <string.h>

// Alloue et retourne une sous-chaîne de `input` entre `start` et `end`
char	*extract_token(const char *input, int start, int end)
{
	int		len;
	char	*token;

	len = end - start;
	token = malloc(len + 1);
	if (!token)
		return (NULL);
	strncpy(token, &input[start], len);
	token[len] = '\0';
	return (token);
}

// Ajoute dynamiquement un nouveau token à la liste existante de tokens
int	add_token(char ***tokens, int *size, char *value)
{
	char	**new_tokens;
	int		i;

	new_tokens = malloc(sizeof(char *) * (*size + 2));
	if (!new_tokens)
	{
		free(value);
		return (0);
	}
	i = 0;
	while (i < *size)
	{
		new_tokens[i] = (*tokens)[i];
		i++;
	}
	new_tokens[i] = value;
	new_tokens[i + 1] = NULL;
	free(*tokens);
	*tokens = new_tokens;
	(*size)++;
	return (1);
}

void	update_state_quote(t_state *state, char c)
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

int	flush_token(t_tok *t)
{
	char	*token;

	t->current[t->curr_i] = '\0';
	{
		token = strdup(t->current);
		if (!token)
			return (0);
		if (!add_token(&t->tokens, &t->size, token))
			return (0);
	}
	t->curr_i = 0;
	return (1);
}

int  handle_quote_state(const char *src, int *pi, t_state *pstate)
{
    if (*pstate == STATE_GENERAL)
    {
        if (src[*pi] == '\'' || src[*pi] == '"')
        {
            if (src[*pi] == '\'')
                *pstate = STATE_IN_SINGLE_QUOTE;
            else
                *pstate = STATE_IN_DOUBLE_QUOTE;
            *pi = *pi + 1;
            return 1;
        }
    }
    else
    {
        if (((*pstate) == STATE_IN_SINGLE_QUOTE && src[*pi] == '\'') ||
            ((*pstate) == STATE_IN_DOUBLE_QUOTE && src[*pi] == '"'))
        {
            *pstate = STATE_GENERAL;
            *pi = *pi + 1;
            return 1;
        }
    }
    return 0;
}