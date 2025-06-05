/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_handle.c                                  :+:    :+:           */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 17:29:58 by grohr             #+#    #+#             */
/*   Updated: 2025/05/13 18:22:36 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include "../inc/tokenizer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int	handle_char_quote(t_tok *t)
{
	t->current[t->curr_i++] = t->input[t->pos++];
	if (t->state == STATE_GENERAL)
		if (!flush_token(t))
			return (0);
	return (1);
}

int	handle_space(t_tok *t)
{
	if (t->curr_i > 0)
		if (!flush_token(t))
			return (0);
	t->pos++;
	return (1);
}

int	handle_special(t_tok *t)
{
	if (t->curr_i > 0)
		if (!flush_token(t))
			return (0);
	if ((t->input[t->pos] == '<' && t->input[t->pos + 1] == '<')
		|| (t->input[t->pos] == '>' && t->input[t->pos + 1] == '>'))
	{
		t->current[0] = t->input[t->pos];
		t->current[1] = t->input[t->pos + 1];
		t->current[2] = '\0';
		if (!add_token(&t->tokens, &t->size, strdup(t->current)))
			return (0);
		t->pos += 2;
	}
	else
	{
		t->current[0] = t->input[t->pos];
		t->current[1] = '\0';
		if (!add_token(&t->tokens, &t->size, strdup(t->current)))
			return (0);
		t->pos++;
	}
	return (1);
}

int	handle_regular(t_tok *t)
{
	t->current[t->curr_i++] = t->input[t->pos++];
	return (1);
}
