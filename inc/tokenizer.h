#ifndef TOKENIZER_H
# define TOKENIZER_H

/*
** États possibles pendant l'analyse des tokens
*/
typedef enum s_state
{
	STATE_GENERAL,
	STATE_IN_SINGLE_QUOTE,
	STATE_IN_DOUBLE_QUOTE
}	t_state;

typedef struct s_token
{
	char	*value;
	t_state	quote_state;
}	t_token;

// tokenizer.c
int 	extract_quoted_token(const char *input, int *i, char quote, char ***tokens, int *size);
char 	**tokenize_input(const char *input, int size, int i);
// char 	**expand_all_tokens(char **tokens, char **env);

// tokenizer_utils.c
int		is_special_char(char c);
char	*extract_token(const char *input, int start, int end);
int 	add_token(char ***tokens, int *size, char *value);

#endif