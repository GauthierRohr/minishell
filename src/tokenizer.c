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

// Gère les espaces : coupe un token quand un espace est détecté (hors guillemets)
static int	handle_space_token(const char *input, int *i, int *start,
	char ***tokens, int *size)
{
	char	*token;

	if (*start < *i)
	{
		token = extract_token(input, *start, *i);
		if (!add_token(tokens, size, token))
			return (0);
	}
	(*i)++;
	*start = *i;
	return (1);
}

// Finalise et ajoute le dernier token quand on atteint la fin de la ligne
static int	finalize_last_token(const char *input, int start, int i,
	char ***tokens, int *size)
{
	char	*token;

	if (start < i)
	{
		token = extract_token(input, start, i);
		if (!add_token(tokens, size, token))
			return (0);
	}
	return (1);
}

// Gère les caractères spéciaux comme | < > << >> en les isolant comme tokens
static int	handle_special_token(const char *input, int *i, int *start,
	char ***tokens, int *size)
{
	int		j;
	char	*token;

	if (*start < *i)
	{
		token = extract_token(input, *start, *i);
		if (!add_token(tokens, size, token))
			return (0);
	}
	j = *i + 1;
	if ((input[*i] == '<' && input[j] == '<')
		|| (input[*i] == '>' && input[j] == '>'))
		j++;
	token = extract_token(input, *i, j);
	if (!add_token(tokens, size, token))
		return (0);
	*i = j;
	*start = *i;
	return (1);
}

// Fonction principale : découpe l'input en tokens en gérant quotes et caractères spéciaux
char	**tokenize_input(const char *input, int size, int i, int start)
{
	char	**tokens = NULL;
	t_state	state = STATE_GENERAL;

	while (input[i])
	{
		if (state == STATE_GENERAL)
		{
			if (is_special_char(input[i]))
			{
				if (!handle_special_token(input, &i, &start, &tokens, &size))
					return (NULL);
				continue;
			}
			else if (input[i] == ' ')
			{
				if (!handle_space_token(input, &i, &start, &tokens, &size))
					return (NULL);
				continue;
			}
			else if (input[i] == '\'' || input[i] == '\"')
			{
				if (start < i)
				{
					char *pre_token = extract_token(input, start, i);
					if (!add_token(&tokens, &size, pre_token))
						return (NULL);
				}
				if (!extract_quoted_token(input, &i, input[i], &tokens, &size))
					return (NULL);
				start = i;
				continue;
			}
		}
		update_state_quote(&state, input[i]);
		i++;
	}
	if (!finalize_last_token(input, start, i, &tokens, &size))
		return (NULL);
	return (tokens);
}
