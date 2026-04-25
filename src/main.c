/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#           */
/*   Created: 2025/04/23 22:28:36 by grohr             #::
    #             */
/*   Updated: 2025/05/27 22:00:00 by grohr            ###   #fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	g_last_exit_status = 0;

void	choose_execution(char **cleaned_args, char ***env)
{
	if (contains_pipe(cleaned_args))
		find_args(cleaned_args, *env);
	else if (is_builtin(cleaned_args[0]))
		g_last_exit_status = execute_builtin(cleaned_args, env);
	else
		g_last_exit_status = execute_external(cleaned_args, *env);
}

void	execute_command(char **args, char ***env)
{
	char	**cleaned_args;
	int		stdin_backup;
	int		stdout_backup;

	if (!args || !args[0])
		return ;
	stdin_backup = -1;
	stdout_backup = -1;
	if (backup_stdio(&stdin_backup, &stdout_backup) == -1)
		return ;
	if (process_redirections(args) == -1)
	{
		restore_stdio(stdin_backup, stdout_backup);
		return ;
	}
	cleaned_args = clean_args(args);
	if (!cleaned_args || !cleaned_args[0])
	{
		restore_stdio(stdin_backup, stdout_backup);
		free_tab(cleaned_args);
		return ;
	}
	choose_execution(cleaned_args, env);
	restore_stdio(stdin_backup, stdout_backup);
	free_tab(cleaned_args);
}

void	process_args(char *user_input, char ***env)
{
	char	**args;

	if (!user_input)
	{
		printf("exit\n");
		return ;
	}
	if (user_input[0] == '\0')
	{
		free(user_input);
		return ;
	}
	add_history(user_input);
	args = tokenize_input(user_input, 0, 0);
	free(user_input);
	if (!args)
		return ;
	execute_command(args, env);
	free_tab(args);
}

int	main(int argc, char **argv, char **envp)
{
	char	*user_input;
	char	**env;

	(void)argc;
	(void)argv;
	env = dup_env(envp);
	if (!env)
		return (1);
	using_history();
	init_signals();
	while (1)
	{
		user_input = readline("minishell> ");
		if (!user_input)
		{
			printf("exit\n");
			break ;
		}
		process_args(user_input, &env);
	}
	free_env(env);
	clear_history();
	return (g_last_exit_status);
}
