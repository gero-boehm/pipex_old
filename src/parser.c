/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbohm <gbohm@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 20:27:21 by gbohm             #+#    #+#             */
/*   Updated: 2023/02/07 13:33:22 by gbohm            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "pipex.h"
#include "libft.h"

static int	is_escaped(char *str, unsigned long i)
{
	int	slashes;

	slashes = 0;
	while (i > 0 && str[--i] == '\\')
		slashes++;
	return (slashes % 2);
}

static int	get_end_quote2(char *str, char quote, unsigned long *i)
{
	while (str[++(*i)])
	{
		if (str[*i] != quote)
			continue ;
		if (is_escaped(str, *i))
			continue ;
		break ;
	}
	return (str[*i] == 0);
}

static int	mark_split_points(char *str, size_t len)
{
	unsigned long	i;

	i = 0;
	while (i < len)
	{
		if (is_whitespace(str[i]))
			str[i] = 31;
		if (is_quote(str[i]) && !is_escaped(str, i)
			&& get_end_quote2(str, str[i], &i))
			return (1);
		i++;
	}
	return (0);
}

static int	remove_quotes2(char **params)
{
	char	**cursor;
	char	*trimmed;
	size_t	len;

	cursor = params;
	while (*cursor != NULL)
	{
		if (is_quote(**cursor))
			**cursor = ' ';
		len = ft_strlen(*cursor);
		if (is_quote((*cursor)[len - 1]) && !is_escaped(*cursor, len))
			(*cursor)[len - 1] = ' ';
		if (ft_strtrim2(*cursor, " ", &trimmed))
			return (1);
		free(*cursor);
		*cursor = trimmed;
		cursor++;
	}
	return (0);
}

int	split_params2(const char *cmd, char ***params)
{
	char			*str;
	size_t			len;

	if (ft_strtrim2(cmd, " \t\r\n\v\f", &str))
		return (1);
	len = ft_strlen(str);
	if (mark_split_points(str, len))
		return (free(str), 2);
	if (ft_split2(str, 31, params))
		return (free(str), 3);
	if (remove_quotes2(*params))
		return (free(str), ft_arrfree((void **) *params), 4);
	return (0);
}
