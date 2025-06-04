/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                         :+:    :+:           */
/*                                                    +:+         +:+         */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#           		      */
/*   Created: 2025/05/13 17:30:08 by grohr             #+#    #    #          */
/*   Updated: 2025/06/03 13:49:01 by cjauregu       ########   odam.nl        */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include "../inc/tokenizer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static char	**fail_tokenizer(t_tok *t)
{
	free(t->current);
	free_tab(t->tokens);
	return (NULL);
}

static int	process_tokenizer_loop(t_tok *t)
{
	while (t->input[t->pos])
	{
		update_state_quote(&t->state, t->input[t->pos]);
		if (t->state != STATE_GENERAL)
		{
			if (!handle_char_quote(t))
				return (0);
		}
		else if (t->input[t->pos] == ' ')
		{
			if (!handle_space(t))
				return (0);
		}
		else if (is_special_char(t->input[t->pos]))
		{
			if (!handle_special(t))
				return (0);
		}
		else
		{
			if (!handle_regular(t))
				return (0);
		}
	}
	return (1);
}

char	**tokenize_input(const char *input, int size, int i)
{
	t_tok	t;

	t.input = input;
	t.pos = i;
	t.size = size;
	t.state = STATE_GENERAL;
	t.curr_i = 0;
	t.tokens = NULL;
	t.current = malloc(strlen(input) + 1);
	if (!t.current)
		return (NULL);
	if (!process_tokenizer_loop(&t))
		return (fail_tokenizer(&t));
	if (t.curr_i > 0)
		if (!flush_token(&t))
			return (fail_tokenizer(&t));
	free(t.current);
	return (t.tokens);
}
