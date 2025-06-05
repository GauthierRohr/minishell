/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grohr <grohr@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 19:56:09 by grohr             #+#    #+#             */
/*   Updated: 2025/06/05 21:18:53 by grohr            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

// utils
long	ft_strtol(const char *str, char **endptr, int base);
char	*get_env_value(const char *name, char **env);
char	*remove_partial_quotes(const char *str);

#endif