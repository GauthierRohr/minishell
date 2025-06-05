/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                         :+:    :+:           */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 22:28:10 by grohr             #+#    #+#             */
/*   Updated: 2025/06/03 14:44:22 by cjauregu       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

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

typedef struct s_tok {
	const char	*input;
	int			pos;
	char		**tokens;
	int			size;
	t_state		state;
	int			curr_i;
	char		*current;
}	t_tok;

// tokenizer.c
char	**tokenize_input(const char *input, int size, int i);

// tokenizer_utils.c
int		is_special_char(char c);
char	*extract_token(const char *input, int start, int end);
int		add_token(char ***tokens, int *size, char *value);
int		flush_token(t_tok *t);
void	update_state_quote(t_state *state, char c);
int			handle_quote_state(const char *src, int *pi, t_state *pstate);

//tokenizer_handle.c
int		handle_char_quote(t_tok *t);
int		handle_space(t_tok *t);
int		handle_special(t_tok *t);
int		handle_regular(t_tok *t);

#endif
