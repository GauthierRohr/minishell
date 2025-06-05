/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 22:28:10 by grohr             #+#    #+#             */
/*   Updated: 2025/06/05 20:04:40 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include "builtins.h"
# include "builtins_extra.h"
# include "env.h"
# include "exec.h"
# include "free.h"
# include "get_cmd_path.h"
# include "main.h"
# include "piping.h"
# include "redirect.h"
# include "signals.h"
# include "tokenizer.h"
# include "utils.h"

# define RED     "\033[1;31m"
# define GREEN   "\033[1;32m"
# define YELLOW  "\033[1;33m"
# define BLUE    "\033[1;34m"
# define MAGENTA "\033[1;35m"
# define CYAN    "\033[1;36m"
# define RST   	"\033[0m"

# include "limits.h"
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <fcntl.h>
# include <signal.h>
# include <dirent.h>
# include <termios.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <sys/ioctl.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <curses.h>

extern int	g_last_exit_status;

#endif // MINISHELL_H
