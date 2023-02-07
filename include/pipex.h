/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbohm <gbohm@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 15:44:25 by gbohm             #+#    #+#             */
/*   Updated: 2023/02/07 13:33:02 by gbohm            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# ifndef TWO_COMMANDS_ONLY
#  define TWO_COMMANDS_ONLY 0
# endif

# include <stddef.h>

typedef struct s_vector {
	size_t	size;
	char	*str;
}	t_vector;

int	split_params2(const char *cmd, char ***params);
int	is_whitespace(char c);
int	is_quote(char c);

#endif
