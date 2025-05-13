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

/* // Gère les espaces : coupe un token quand un espace est détecté (hors guillemets)
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
 */

// Fonction principale : découpe l'input en tokens en gérant quotes et caractères spéciaux
char **tokenize_input(const char *input, int size, int i)
{
	char	**tokens = NULL;
	t_state	state = STATE_GENERAL;
	char	*current = malloc(strlen(input) + 1); // tampon pour construire le token
	int		curr_i = 0;

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
					return (free(current), NULL);
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
					return (free(current), NULL);
				curr_i = 0;
			}
			if ((input[i] == '<' && input[i + 1] == '<') ||
				(input[i] == '>' && input[i + 1] == '>'))
			{
				current[0] = input[i];
				current[1] = input[i + 1];
				current[2] = '\0';
				if (!add_token(&tokens, &size, strdup(current)))
					return (free(current), NULL);
				i += 2;
			}
			else
			{
				current[0] = input[i++];
				current[1] = '\0';
				if (!add_token(&tokens, &size, strdup(current)))
					return (free(current), NULL);
			}
			continue;
		}
		else if ((state == STATE_GENERAL && (input[i] == '\'' || input[i] == '"')) ||
				state == STATE_IN_SINGLE_QUOTE || state == STATE_IN_DOUBLE_QUOTE)
		{
			char quote = input[i++];
			while (input[i] && input[i] != quote)
				current[curr_i++] = input[i++];
			if (input[i] == quote)
				i++;
			update_state_quote(&state, quote);
		}
		else
			current[curr_i++] = input[i++];
	}
	if (curr_i > 0)
	{
		current[curr_i] = '\0';
		if (!add_token(&tokens, &size, strdup(current)))
			return (free(current), NULL);
	}
	free(current);
	return (tokens);
}

