/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 21:42:07 by grohr             #+#    #+#             */
/*   Updated: 2025/03/13 18:02:50 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <unistd.h>
# include <stdarg.h>
# include "../libft.h"

//Functions
int	ft_printf(const	char *format, ...);
int	convert_char(char c);
int	convert_str(const char *str);
int	convert_ptr(void *ptr);
int	convert_int(int n);
int	convert_unsigned(unsigned int n);
int	convert_hex(unsigned int n, char letter);

#endif