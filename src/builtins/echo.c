/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 19:34:53 by grohr             #+#    #+#             */
/*   Updated: 2025/06/05 20:12:09 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/utils.h"
#include "../../inc/minishell.h"
#include <stdio.h>
#include <stdlib.h>

static int	append_to_expanded(char *expanded, int pos, const char *str)
{
	int len = ft_strlen(str);
	if (pos + len >= 4096)
		return (-1);
	ft_strlcpy(expanded + pos, str, 4096 - pos);
	return (pos + len);
}

static int	expand_var_name(const char *token, int *i, char **env, char *expanded, int pos)
{
	char var_name[256];
	int j = 0;

	while (token[*i] && (ft_isalnum(token[*i]) || token[*i] == '_') && j < 255)
		var_name[j++] = token[(*i)++];
	var_name[j] = '\0';
	char *value = get_env_value(var_name, env);
	if (value)
		pos = append_to_expanded(expanded, pos, value);
	return (pos);
}

static int	expand_dollar(const char *token, int *i, char **env, char *expanded, int pos)
{
	if (token[*i] == '?')
	{
		char *status = ft_itoa(g_last_exit_status);
		if (!status) return (-1);
		pos = append_to_expanded(expanded, pos, status);
		free(status);
		(*i)++;
	}
	else if (ft_isalpha(token[*i]) || token[*i] == '_')
		pos = expand_var_name(token, i, env, expanded, pos);
	else
	{
		if (pos < 4095) expanded[pos++] = '$';
		if (token[*i] && pos < 4095) expanded[pos++] = token[(*i)++];
	}
	return (pos);
}

char	*expand_vars(const char *token, char **env)
{
	char *expanded = malloc(4096);
	int i = 0, pos = 0;
	if (!expanded) return NULL;
	while (token[i] && pos < 4095)
	{
		if (token[i] == '$' && token[i + 1])
		{
			i++;
			pos = expand_dollar(token, &i, env, expanded, pos);
			if (pos == -1) break;
		}
		else
			expanded[pos++] = token[i++];
	}
	expanded[pos] = '\0';
	return expanded;
}


// Modifié pour retourner un booléen si l'argument était entre quotes simples
static char	*clean_arg_quotes(char *arg, int *is_single_quoted)
{
	char	*tmp;
	char	*clean_arg;

	*is_single_quoted = 0;
	if (arg[0] == '\'' && arg[ft_strlen(arg) - 1] == '\'')
	{
		*is_single_quoted = 1;
		tmp = remove_quotes(arg);
		if (tmp)
		{
			clean_arg = ft_strdup(tmp);
			free(tmp);
		}
		else
			clean_arg = ft_strdup("");
	}
	else
	{
		tmp = remove_partial_quotes(arg);
		if (tmp)
			clean_arg = tmp;
		else
			clean_arg = ft_strdup("");
	}
	return (clean_arg);
}

static void	print_arg(char *arg, char **env, int *first_non_empty)
{
	int is_single_quoted;
	char *clean_arg = clean_arg_quotes(arg, &is_single_quoted);
	char *to_print;

	if (!clean_arg || clean_arg[0] == '\0')
	{
		free(clean_arg);
		return ;
	}
	if (is_single_quoted)
		to_print = clean_arg;
	else
	{
		to_print = expand_vars(clean_arg, env);
		free(clean_arg);
		if (!to_print)
			return ;
	}
	if (!*first_non_empty)
		printf(" ");
	printf("%s", to_print);
	free(to_print);
	*first_non_empty = 0;
}

int	builtin_echo(char **args, char ***env)
{
	int	i;
	int	n_option;
	int	first_non_empty;

	i = 1;
	n_option = 0;
	first_non_empty = 1;
	if (args[1] && ft_strcmp(args[1], "-n") == 0)
	{
		n_option = 1;
		i = 2;
	}
	while (args[i])
	{
		print_arg(args[i], *env, &first_non_empty);
		i++;
	}
	if (!n_option)
		printf("\n");
	g_last_exit_status = 0;
	return (0);
}
