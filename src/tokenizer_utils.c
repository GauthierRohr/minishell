/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 17:29:58 by grohr             #+#    #+#             */
/*   Updated: 2025/05/13 18:22:36 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/tokenizer.h"
#include "../inc/minishell.h"
#include <stdlib.h>
#include <string.h>

// Vérifie si un caractère est un opérateur spécial (|, <, >)
int	is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

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
int add_token(char ***tokens, int *size, char *value)
{
    char **new_tokens;
    int i;

    new_tokens = malloc(sizeof(char *) * (*size + 2));
    if (!new_tokens)
    {
        free(value);
        return (0);
    }
    for (i = 0; i < *size; i++)
        new_tokens[i] = (*tokens)[i];
    new_tokens[i] = value;
    new_tokens[i + 1] = NULL;
    free(*tokens);
    *tokens = new_tokens;
    (*size)++;
    return (1);
}
