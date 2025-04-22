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
int	add_token(char ***tokens, int *size, char *token)
{
	char	**new_tokens;
	int		i;

	new_tokens = malloc(sizeof(char *) * (*size + 2));
	if (!new_tokens)
		return (0);
	i = 0;
	while (i < *size)
	{
		new_tokens[i] = (*tokens)[i];
		i++;
	}
	new_tokens[i++] = token;
	new_tokens[i] = NULL;
	free(*tokens);
	*tokens = new_tokens;
	(*size)++;
	return (1);
}
