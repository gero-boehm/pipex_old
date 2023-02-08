/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbohm <gbohm@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 15:44:25 by gbohm             #+#    #+#             */
/*   Updated: 2023/02/08 14:17:28 by gbohm            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# ifndef TWO_COMMANDS_ONLY
#  define TWO_COMMANDS_ONLY 0
# endif

# define STDIN 0
# define STDOUT 1

# include <stddef.h>

int	split_params2(const char *cmd, char ***params);
int	is_whitespace(char c);
int	is_quote(char c);
int	get_command_path2(const char *cmd, char *const *envp, char **path);

int	run(int fd, int is_here_doc, const char **argv, char *const *envp);

int	get_infile2(const char *fspath, int *fd);
int	get_write_fd2(const char *fspath, int append, int *fd);
int	output(const char *file, int fd, int is_here_doc);

#endif
