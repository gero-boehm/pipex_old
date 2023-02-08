/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbohm <gbohm@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 15:44:18 by gbohm             #+#    #+#             */
/*   Updated: 2023/02/08 20:52:44 by gbohm            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "get_next_line.h"
#include "libft.h"
#include "pipex.h"

int	get_here_doc(const char *eof)
{
	char	*line;
	int		fd;

	if (get_write_fd2("here_doc", 0, &fd))
		return (1);
	while (1)
	{
		write(1, "heredoc> ", 9);
		line = get_next_line(STDIN);
		if (line == NULL || ft_strncmp(line, eof, ft_strlen(eof)) == 0)
			break ;
		write(fd, line, ft_strlen(line));
		free(line);
	}
	free(line);
	close(fd);
	return (0);
}

int	main(int argc, const char **argv, char *const *envp)
{
	int	is_here_doc;
	int	infile;

	is_here_doc = ft_strncmp(argv[1], "here_doc", 8) == 0;
	if (is_here_doc)
	{
		if (argc < 4)
		{
			ft_putendl_fd("pipex: must have at least here_doc, "
				"limiter and outfile", 2);
			return (1);
		}
		if (get_here_doc(argv[2]))
			return (2);
	}
	else if (argc < 3)
		return (ft_putendl_fd("pipex: must have at least "
				"infile and outfile", 2), 1);
	if (get_infile2(argv[1], &infile))
		return (3);
	if (envp == NULL)
		return (4);
	run(infile, is_here_doc, argv, envp);
	unlink("here_doc");
	// system("leaks pipex");
	return (0);
}
