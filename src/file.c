/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbohm <gbohm@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 14:14:04 by gbohm             #+#    #+#             */
/*   Updated: 2023/03/14 17:32:53 by gbohm            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <fcntl.h>
#include "libft.h"
#include "get_next_line.h"
#include "pipex.h"

int	get_infile2(const char *fspath, int *fd)
{
	*fd = open(fspath, O_RDONLY | O_CREAT, 0644);
	return (*fd == -1);
}

int	get_write_fd2(const char *fspath, int append, int *fd)
{
	int	flags;

	flags = O_WRONLY | O_CREAT | O_APPEND;
	if (!append)
		flags |= O_TRUNC;
	*fd = open(fspath, flags, 0644);
	return (*fd == -1);
}

int	output(const char *file, int fd, int is_here_doc)
{
	int		outfile;
	char	*line;
	size_t	len;

	if (get_write_fd2(file, is_here_doc, &outfile))
		return (1);
	while (1)
	{
		line = get_next_line(fd);
		if (line == NULL)
			break ;
		len = ft_strlen(line);
		write(outfile, line, len);
		free(line);
	}
	close(outfile);
	return (0);
}
